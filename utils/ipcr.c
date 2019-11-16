#include "ipcr.h"

int ipcr_init()
{
  keyshm = ftok(KEY_PATH, KEY_SYMB2); //OTF
  if(keyshm == -1) return -1;

  // Creating memory
  shmid = shmget(keyshm, sizeof(struct GpioData), (0644 | IPC_CREAT | IPC_EXCL));
  if(shmid == -1) // registering
  {
    if(errno == EEXIST)  // memory exists
    {
      shmid = shmget(keyshm, sizeof(struct GpioData), 0644);
      if(shmid == -1)
        return -1;
     mm = (struct GpioData *)shmat(shmid, (void *)0, 0);
     if(mm == (struct GpioData *)(-1))
       return -1;
     return 1;
    }
    else
      return -1;
  }
  else
  {
    mm = (struct GpioData *)shmat(shmid, (void *)0, 0);
    if(mm == (struct GpioData *)(-1))
      return -1;
    memset((void *)mm, 0, sizeof(struct GpioData));
    return 1;
  }
  return 1;
}

int ipcr_fill()
{
  int i;
  for (i=0; i < MAX_IPC_LENGHT; i++) mm->da[i] = 0;
  return 1;
}

int ipcr_set(int da, int nr)
{
  mm->da[nr] = da;
  return 1;
}

int ipcr_get(int nr)
{
  return mm->da[nr];
}

int ipcr_setchar(unsigned char ch[8], int nr)
{ 
  int i;
  for (i=0;i<8;i++) mm->ch[nr][i]=ch[i];
  return 0;
}

unsigned char ipcr_getchar(int chnr, int nr)
{
  return mm->ch[chnr][nr];
}

int ipcr_settx(int data) 
{
  mm->tx[mm->da[1]] = data;
  mm->da[1]++;
  return 0;
}

int ipcr_gettx() 
{
  int i,j;
  if (mm->da[1] == 0) return 0;
  j = mm->tx[0];
  for (i=0;i < mm->da[1];i++) mm->tx[i] = mm->tx[i+1];
  mm->tx[mm->da[1]] = 0;
  mm->da[1]--;
  return j;
}

int ipcr_destroy()
{
  int rez;

  rez = shmctl(shmid,IPC_RMID,0);

  return rez;
}
