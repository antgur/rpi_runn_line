#if !defined(SRVLOG_H)
#define SRVLOG_H

#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#define LOG_PATH "/var/log"
#define FILE_NAME "srv2"
#define MAX_MESSAGE_LENGHT 256

int srvlog(char *st);

#endif
