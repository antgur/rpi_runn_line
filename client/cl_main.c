#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/un.h>

#include <fcntl.h>
//#include "client.h"

#define MYPORT 4441
// starting program as system service

int pagr();

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

// main program just running as system service
int pagr()
{
int sockMain, sockClient, length, child;
struct sockaddr_in servAddr;

struct sockaddr_storage clAddr;
socklen_t len = sizeof(clAddr);
char ipstr[INET6_ADDRSTRLEN + 1];
int port;

    signal(SIGCHLD,SIG_IGN);

  if ((sockMain = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    { perror("Server can't open main socket.");
    exit(1);
    }
  bzero( &(servAddr.sin_zero), 8);
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;
  servAddr.sin_port = htons (MYPORT);
 
  if ( bind(sockMain, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))  \
							    ==-1 )
    { printf("Server binding problem:%d\n", ntohs(servAddr.sin_port));
    perror("Port is busy");
    exit(1);
    }

  printf("SERVER: port numbe - %d\n", ntohs(servAddr.sin_port));
  
    if ( listen (sockMain, 5) ==-1) {
    perror("listen");
    exit(1);
    }
  
  // waiting for the client TCP connection.
  for ( ; ; ) {
  if ( (sockClient = accept(sockMain, (struct sockaddr*)&clAddr, &len)) < 0)
    { perror("Incorrect client socket.");
    exit(1);
    }
  // Creating child process
  if ( (child = fork()) < 0)
    { 
    perror("Child process creation error.");
    exit(1);
    }
  else if (child == 0)  // child process running code follows
    { 
    close(sockMain);  // child process for the sockMain notinteresting any more
    if (clAddr.ss_family == AF_INET) //INET4
    {
	struct sockaddr_in *s = (struct sockaddr_in *)&clAddr;
	port = ntohs(s->sin_port);
	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
    } else { //INET6
	struct sockaddr_in6 *s = (struct sockaddr_in6 *)&clAddr;
	port = ntohs(s->sin6_port);
	inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
    }
    childWork(sockClient,ipstr);
    close(sockClient);
    exit(0);
    }
  // That is main process running alone
  close(sockClient);
  }
}
