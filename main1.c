#include "main1.h"

// startuoju programa, kuri atrisama nuo terminalo

int main()
{
    switch(fork())
    {
    case -1:
        printf("Fork error\n");
        return -1;
    case 0:
        if(setsid() == -1)
            break;
        pagr();
        exit(0);
        break;
    default:
        break;
    }
    return 0;
}

void send16bits(unsigned short output)
{
    unsigned char i;
    for (i=16; i>0; i--)
    {
	unsigned short mask = 1 << (i - 1); // calculate bitmask
	gpioWrite(CLOCK, 0); // set clock to 0
    // Send one bit on the data pin
	if (output & mask)
        {
    	    gpioWrite(DATA, 1);
        }
	else
        {
    	    gpioWrite(DATA, 0);
        }
	gpioWrite(CLOCK, 1); // set clock to 1
  }
}

// Take a reg numer and data and send to the max7219
void MAX7219Send (unsigned char reg_number, unsigned char dataout)
{
    gpioWrite(LOAD, 0); // set LOAD 1 to start
    send16bits((reg_number << 8) + dataout);   // send 16 bits ( reg number + dataout )
    gpioWrite(LOAD, 0); // LOAD 0 to latch
    gpioDelay(15);
    gpioWrite(LOAD, 1); // set LOAD 1 to finish
}

void init7219()
{
    int ii,jj;
      //We need 3 output pins to control the Max7219: Data, Clock and Load
    gpioSetMode(DATA, PI_OUTPUT);
    gpioSetMode(CLOCK, PI_OUTPUT);
    gpioSetMode(LOAD, PI_OUTPUT);

    gpioDelay(50);

    MAX7219Send(SCAN_LIMIT, 7);     // set up to scan all eight digits
    MAX7219Send(DECODE_MODE, 0);   // Set BCD decode mode off
    MAX7219Send(SHUTDOWN, 1);      // come out of shutdown mode   / turn on the digits
    MAX7219Send(DISPLAY_TEST, 0);  // Disable test mode

    for (ii=1; ii<=8; ii++) MAX7219Send(ii,0);

    MAX7219Send(INTENSITY, 1);     // set brightness 0 to 15
    //Initial character schema load
    for(ii=0;ii<MAX_CHAR;ii++) ipcr_setchar(cc[ii],ii);
    return;
}

//signalo apdorojimo programa
void termination_handler (int signum)
{
    ipcr_set(-2,1);
    srvlog("Received signal to stop...");
    return;
}


// pagrindine programa - jau atrista nuo terminalo
int pagr()
{
    char bu[80]; // log teksto buferis
    unsigned long di[8]; //raidės imidžo šablonas
    int ipc; //ipcs prisijungimo indikatorius
    int i,k,ii;
    for (i=0;i<8;i++) di[i]=0; //inicializuoju kintamaji

    srvlog("Main module started");
    //Bandau jungtis prie ipc
    if ((ipc = ipcr_init()) < 0) //jeigu nepavyko
    {
	srvlog("Unfortunately, didn't connect to shmem...");
	srvlog("Main module accidentally stopped.");
	return -1; // programa baigia darba
    } //Pavyko
    //Bandau jungtis prie rpi PIO
    if (gpioInitialise() < 0 ) //jeigu nepavyko
    {
	srvlog("Unfortunately, didn't connect to PIO...");
	srvlog("Main module accidentally stopped.");
	gpioTerminate(); // dėl visa ko
	return -2; // programa baigia darba
    } //Pavyko

    ipcr_set(ipcr_get(0)+1,0);

    if (signal(SIGTERM, termination_handler) == SIG_IGN)
	signal(SIGTERM, SIG_IGN);
    if (signal(SIGINT, termination_handler) == SIG_IGN)
	signal(SIGINT, SIG_IGN);
    if (signal(SIGHUP, termination_handler) == SIG_IGN)
	signal(SIGHUP, SIG_IGN);
    if (signal(SIGKILL, termination_handler) == SIG_IGN)
	signal(SIGKILL, SIG_IGN);

    init7219();
// keli simboliai testui
//    for (i=49;i<126;i++) ipcr_settx(i);
    for (i=49;i<56;i++) ipcr_settx(i);
    ipcr_set(5,2); // postūmio konstanta
    //Ciklas
    while (ipcr_get(1) >= 0)
    {
	if (ipcr_get(1)==0) ipcr_settx(1); //jeigu eilėje nėra raidžių
	ii = ipcr_gettx(); //ištraukiu rodomos raidės indeksą
	// į di masyva padedu raidės trafaretą
	for (ipc=0;ipc<8;ipc++) di[ipc] = di[ipc] + 256*ipcr_getchar(ii,ipc);

	for (k=0;k<8;k++) { //vykdau išvedimą
	    for (i=0;i<8;i++) MAX7219Send(8-i,di[i]);//išvedu raidę
	    for (i=0;i<8;i++) di[i]=di[i] >> 1; //perstumiu per žingsnį
	    gpioDelay(ipcr_get(2)*10000); // vėlinimas postūmio žingsniui
	}
	// sustabdau raidę centre
	if (ii == 1) gpioDelay(ipcr_get(2)*200000);
	else gpioDelay(ipcr_get(2)*20000);
//	time_sleep(2.0);
	
    }
    //Užgesinu visa matricą
    for (ipc=0;ipc<8;ipc++) MAX7219Send(ipc+1,ipcr_getchar(0,ipc));
    // Programa planuotai stabdoma
    // Atsijungiama nuo PIO
    gpioTerminate();
    ipcr_set(0,0);
    // Atsijungiama nuo shmem
    ipcr_destroy();
    srvlog("Main module stopped.");
    return 0; //Pabaiga
}
