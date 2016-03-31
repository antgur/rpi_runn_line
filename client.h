#include <pigpio.h>
#include <stdio.h>
#include <string.h>

#define HIGH 1
#define LOW 0

void sighandler(int sig);
int childWork(int sockClient, char *ip);