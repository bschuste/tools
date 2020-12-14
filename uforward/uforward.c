

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


#define BUFFER_SIZE             100*1024
#define DEFAULT_PORT            5006
#define INVALID_SOCKET          -1

#define DEFAULT_IP_ADDRESS      "192.168.240.67"

char* printnip(unsigned long ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%ld.%ld.%ld.%ld", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

#define STRING_IP(ip)  ((ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff)
#define TRUE    1
#define FALSE   0
static short lsocket = -1;   // listening socket
static short ssocket = -1;   // sending socket

void exit_callback (int sig)
{
    if (lsocket>INVALID_SOCKET) close(lsocket);
    if (ssocket>INVALID_SOCKET) close(ssocket);
}


int main(int argc, char **argv)
{
    int  rc = 0;
    int    nbytes;
    int i;
    unsigned int socklen;
    unsigned short lport; // listening port
    unsigned short sport; // sending port
    unsigned char rcvBuf[BUFFER_SIZE];
    struct sockaddr_in rcv_sin;   // Server address structure
    struct sockaddr_in snd_sin;   // Server address structure
    unsigned long  lip_addr;

    {
        printf(" %s [destination portnumber - default is %d] [destination target ip address - default is %s\n",
                 argv[0], DEFAULT_PORT, DEFAULT_IP_ADDRESS);
        printf("  version 1.0\n");
        printf("  send bug report or additional features request to brigitte.schuster@anyconnect.com :-)\n");
    }
    signal( SIGINT, exit_callback);

    lport = DEFAULT_PORT;
    sport = DEFAULT_PORT; //DEFAULT_PORT+2;
    if (argc >= 2) lport = atoi(argv[1]);
    printf("  receiving port %d, sending port %d\n", lport, sport);
    lip_addr = inet_addr(DEFAULT_IP_ADDRESS);
    if (argc == 3) lip_addr = inet_addr(argv[2]);
    printf("  sending to %s\n", printnip(lip_addr)); 


    // Create a receiving UDP/IP socket
    if ((lsocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf ("Allocating socket failed. Error: %d\n", errno);
        return 0;
    }
    int yes = 1;
    setsockopt(lsocket, SOL_SOCKET, SO_REUSEPORT,  &yes, sizeof(yes));
    setsockopt(lsocket, SOL_SOCKET, SO_REUSEADDR,  &yes, sizeof(yes));

    // Fill out the local socket's address information.
    memset((char *)&rcv_sin, 0, sizeof(struct sockaddr_in));

    rcv_sin.sin_family = AF_INET;
    rcv_sin.sin_port = htons(lport);
    rcv_sin.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(lsocket, (struct sockaddr *)&rcv_sin, sizeof(struct sockaddr_in));
    if ( rc < 0 )
    {
        close(lsocket);
        printf("Error binding socket to interface. Error: %d\n", errno);
        return rc;
    }


    // Create a sending UDP/IP socket
    if ((ssocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf ("Allocating socket failed. Error: %d\n", errno);
        return 0;
    }

    // Fill out the sender socket's address information.
    memset((char *)&snd_sin, 0, sizeof(struct sockaddr_in));

    snd_sin.sin_family = AF_INET;
    snd_sin.sin_port = htons(sport);
    snd_sin.sin_addr.s_addr = lip_addr;


    socklen = sizeof(struct sockaddr_in);
    for (i=0;i<BUFFER_SIZE;i++) {
        rcvBuf[i] = 0;
    }

    while (1)
    {
        nbytes = recvfrom( lsocket,
                          (char *)&rcvBuf[0], 
                           BUFFER_SIZE, 
                           0,
                             (struct sockaddr *)&rcv_sin,
                          &socklen
                         );
        if ((nbytes == 0) || (nbytes < 0))
        {
            printf("recv(): connection ended\n");
            break;
        }
        printf(" Received %d nbytes\n", nbytes);
        if (ssocket>INVALID_SOCKET) 
        {
            sendto( ssocket,
                    (char *)&rcvBuf[0], 
                    nbytes, 
                    0,
                       (struct sockaddr *)&snd_sin,
                    socklen
                  );
        }
    }
    if (lsocket>INVALID_SOCKET) close(lsocket);
    if (ssocket>INVALID_SOCKET) close(ssocket);

    printf(" Exiting %s application...\n", argv[0]);
    return 0;
}
