#if !defined(MAIN1_H)
#define MAIN1_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/un.h>
#include <pigpio.h>

#include <fcntl.h>
#include "utils/srvlog.h"
#include "utils/ipcr.h"

/*Port Definitions*/
#define DATA  10
#define CLOCK 11
#define LOAD  8

#define DECODE_MODE   0x09
#define INTENSITY     0x0a
#define SCAN_LIMIT    0x0b
#define SHUTDOWN      0x0c
#define DISPLAY_TEST  0x0f

// signal processing function
void termination_handler (int signum);
// preparation to send 
void MAX7219Send (unsigned char reg_number, unsigned char dataout);
// sending to MAC7219 funcion
void send16bits(unsigned short output);
// program as system service
int pagr();

unsigned char cha[MAX_CHAR][8];

#endif
