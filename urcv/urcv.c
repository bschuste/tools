

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>


#define BUFFER_SIZE             100*1024
#define DEFAULT_PORT            32004
#define INVALID_SOCKET          -1


char* printnip(unsigned long ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%ld.%ld.%ld.%ld", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

#define STRING_IP(ip)  ((ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff)
#define TRUE    1
#define FALSE   0


int main(int argc, char **argv)
{
    int  rc = 0;
	int	nbytes;
    int i;
    unsigned int socklen;
    unsigned short lport;
    char writeFile[] = "file.hex";
    FILE *fpWrite = NULL; // retrieved file
    short lsocket = -1;   // listening socket
    unsigned char rcvBuf[BUFFER_SIZE];
    struct sockaddr_in rcv_sin;   // Server address structure

   {
	    printf(" %s [portnumber - default is %d]\n", argv[0], DEFAULT_PORT);
        printf("  version 1.0\n");
        printf("  send an e-mail to brigitte.schuster@anyconnect.com for additional features\n\n");
    }

    lport = DEFAULT_PORT;
	if (argc >= 2) lport = atoi(argv[1]);
    printf("  using port %d \n", lport);

    if((fpWrite = fopen(writeFile,"w"))==NULL) {
		        printf(" error: unable to open file %s errno %d\n", writeFile, errno);
		        return(-1);
    }
	// Create a  UDP/IP socket
	if ((lsocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf ("Allocating socket failed. Error: %d\n", errno);
		return 0;
	}

	// Fill out the local socket's address information.
	memset((char *)&rcv_sin, 0, sizeof(struct sockaddr_in));

	rcv_sin.sin_family = AF_INET;
	rcv_sin.sin_port = htons(lport);  
	rcv_sin.sin_addr.s_addr = htonl(INADDR_ANY);

    rc = bind(lsocket, (struct sockaddr *)&rcv_sin, sizeof(struct sockaddr_in));
    if ( rc < 0 )
    {
        printf("Error binding socket to interface. Error: %d\n", errno);
        return rc;
    }


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
        if ((nbytes == 0) | (nbytes < 0))
        {
            printf("recv(): connection ended\n");
            break;
        }
        printf(" Received %d nbytes\n", nbytes);
        if (fpWrite) 
        {
            fwrite (&rcvBuf[0], 1, nbytes, fpWrite);
            fflush(fpWrite);
        }
	} 
   if (lsocket) close(lsocket);
   if (fpWrite) fclose(fpWrite);

   printf(" Exiting %s application...\n", argv[0]);
   return 0;
}
