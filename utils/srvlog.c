#include "srvlog.h"

int srvlog(char *st)
{
  FILE *fd;
  time_t t;
  char buf[2*MAX_MESSAGE_LENGHT];
  char ss[48];
  struct timeval tv;
  struct tm *t1;
  time(&t);
  t1=localtime(&t);
  sprintf(ss,"%s/%04d/%02d/%s_%02d.log",LOG_PATH,
        t1->tm_year+1900,t1->tm_mon+1,FILE_NAME,t1->tm_mday);
  sprintf(buf,"%s",st);
  if ((fd = fopen(ss,"a")) == NULL)
  {
        //if the folder does not exist
        if (errno != ENOENT) return -1; //continue if folder does not exist
        char pat[64];
        int er;
        sprintf(pat,"%s/%04d",LOG_PATH,t1->tm_year+1900);
        if ((er = mkdir(pat,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)) == -1)
        {
                if (errno != EEXIST)
                {
                printf("Problem: errno=%s,stat=%s\n",strerror(errno),strerror(EEXIST));
                printf("Year dedicated folder could not be created\n");
                return -1; // error with anually folder
                }
        }
        sprintf(pat,"%s/%04d/%02d",LOG_PATH,t1->tm_year+1900,t1->tm_mon+1);
        if ((er = mkdir(pat,S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)) == -1)
        {
                printf("Month dedicated folder could not be created\n");
                return -2; // error with Monthly folder
        }
        // repeating with file creation if folders correct
        fd = fopen(ss,"a");// new file
        if (fd == NULL)
        {
                printf("File is not opening\n");
                return -3; //File opening error
        }
  }
  gettimeofday(&tv,(struct timezone*)0);
  fprintf(fd,"%.2d:%.2d:%.2d.%03ld %s\n",
        t1->tm_hour,t1->tm_min,t1->tm_sec,
        tv.tv_usec/1000,buf);
  fflush(fd);
  fclose(fd);
}

