#include "client.h"
void sighandler(int sig) { }

int childWork(int sockClient, char *ip)
{
  int st;
  char ss[80];
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
  ipcr_settx(50);ipcr_settx(55);
// tęsti čia
  sprintf(ss,"Completed, client IP: %s -> %d",ip, sockClient);
  srvlog(ss);
  return 0;

}