#include "client.h"
void sighandler(int sig) { }

int childWork(int sockClient, char *ip)
{
  int st,ii;
  char ss[80],rc[80];
  fcntl(sockClient, F_SETFL, O_NONBLOCK);
  if ((st = ipcr_init()) < 0)
  {
    sprintf(ss,"IPC init error (-2). Client IP: %s -> %d",ip, sockClient);
    srvlog(ss);
    return -2;
  }
  sprintf(ss,"New client IP: %s -> %d",ip, sockClient);
  srvlog(ss);
  if (ipcr_get(0) == 0) 
  {
    srvlog("Main process stopped");
    return -1;
  }
//  ipcr_settx(50);ipcr_settx(55);
  ii = 0;
  while (ii == 0) {

    if ((st = recv(sockClient,&rc,1,0)) > 0) {
        if (rc[0] != 10) if (rc[0] != 13) {
	    sprintf(ss,"received %c%c",rc[0],0); srvlog(ss);
    	    ipcr_settx(rc[0]);
    	    if (rc[0] == 126) ii = 1;
	}
    }
    if (rc[1] != ipcr_get(1)) {
	rc[1] = ipcr_get(1);
	sprintf(ss,"%d%c",rc[1],0);
        send(sockClient,ss,strlen(ss),0);
    }
    usleep(50 * 1000);
  }
// tęsti čia
  sprintf(ss,"Completed, client IP: %s -> %d",ip, sockClient);
  srvlog(ss);
  return 0;

}