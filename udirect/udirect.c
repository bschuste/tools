

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
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
#define DEFAULT_PORT            4002
#define INVALID_SOCKET          -1

#define DEFAULT_IP_ADDRESS      "172.25.41.156"

char* printnip(unsigned long ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%ld.%ld.%ld.%ld", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

#define STRING_IP(ip)  ((ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff)
#define TRUE    1
#define FALSE   0

#define RTP_HEADER_SIZE 12
#define SIZE_OF_SPS_CODE							5
#define SIZE_OF_PPS_CODE							9

#define NALU_TYPE_SPS								7
#define NALU_TYPE_PPS								8
#define NALU_TYPE_IDR								5
#define NALU_TYPE_SLICE								1
#define NALU_TYPE_STAPA								24
#define NALU_TYPE_STAPB								25
#define NALU_TYPE_MTAP16							26
#define NALU_TYPE_MTAP24							27
#define NALU_TYPE_FUA								28
#define NALU_TYPE_FUB								29

#define RTP_TIMESTAMP(t0,t1,t2,t3) ((t0 << 24) | (t1 << 16) | (t2 << 8) | (t3))

//w400p
//    unsigned char spsHdr[] = {0x67, 0x42, 0xe0, 0x1f, 0x8d, 0x68, 0x0a, 0x03, 0x3a, 0x01, 0xe7, 0x3d, 0x40};
//    unsigned char ppsHdr[] = {0x68, 0xce, 0x17, 0x20, 0x00, 0x00};
//720p
unsigned char spsHdr[] = {0x67, 0x42, 0xe0, 0x1f, 0x8d, 0x68, 0x05, 0x00, 0x5b, 0xa0, 0x1e, 0x73, 0xd4};
unsigned char ppsHdr[] = {0x68, 0xce, 0x02, 0x5c, 0x80, 0x00};

int udirect_fix_headers(uint8_t *rcvBuf, int nbytes, uint8_t *sndBuf)
{
    int au_start = 0;
    static uint32_t cur_timestamp = 0;
    static uint32_t old_timestamp = 0;
    int nbytes_tosend = 0;

	old_timestamp = cur_timestamp;
	cur_timestamp = RTP_TIMESTAMP(rcvBuf[4], rcvBuf[5], rcvBuf[6], rcvBuf[7]);

	if (cur_timestamp != old_timestamp)
	{
		au_start = 1;
	}
	else
		au_start = 0;


	if ((au_start == 1) && ((rcvBuf[RTP_HEADER_SIZE]&0x1F) == NALU_TYPE_SPS))
	{
		memcpy (&sndBuf[0], &rcvBuf[0], RTP_HEADER_SIZE);
		nbytes_tosend = RTP_HEADER_SIZE;

		printf(" SPS header\n");
		memcpy (&sndBuf[nbytes_tosend], &spsHdr[0], sizeof(spsHdr));
		nbytes_tosend += sizeof(spsHdr);

		printf(" PPS header\n");
		memcpy (&sndBuf[nbytes_tosend], &ppsHdr[0], sizeof(ppsHdr));
		nbytes_tosend += sizeof(ppsHdr);
#if 0
		{
			int k;
			for (k=0;k<nbytes_tosend;k++)
			{
				printf(" 0x%02x", sndBuf[k]);
				if (k%4==3) printf("\n");
			}
			if (k%4!=3) printf("\n");
			usleep(1000);
		}
#endif
	}
	else
	{
		memcpy (&sndBuf[0], &rcvBuf[0], nbytes);
		nbytes_tosend = nbytes;
	}
	return nbytes_tosend;
}

int main(int argc, char **argv)
{
    int rc = 0;
	int	nbytes;
    unsigned int socklen;
    unsigned short lport; // listening port
    short lsocket = -1;   // listening socket
    unsigned short sport; // sending port
    short ssocket = -1;   // sending socket
    unsigned char rcvBuf[BUFFER_SIZE];
    unsigned char sndBuf[BUFFER_SIZE + 128];
    struct sockaddr_in rcv_sin;   // Server address structure
    struct sockaddr_in snd_sin;   // Server address structure
    unsigned long  lip_addr;

    {
	    printf(" %s [portnumber - default is %d] [target ip address - default is %s\n",
                 argv[0], DEFAULT_PORT, DEFAULT_IP_ADDRESS);
        printf("  version 1.0\n");
        printf("  send bug report or additional features request to brigitte.schuster@alcatel-lucent.com :-)\n");
    }

    lport = DEFAULT_PORT;
    sport = DEFAULT_PORT+2;
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
	memset((char *)&rcvBuf[0], 0, sizeof(BUFFER_SIZE));
	memset((char *)&sndBuf[0], 0, sizeof(BUFFER_SIZE + 128));

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
        //printf(" Received %d nbytes\n", nbytes);
        nbytes = udirect_fix_headers(&rcvBuf[0], nbytes, &sndBuf[0]);
        if (ssocket>INVALID_SOCKET)
        {
            sendto( ssocket,
                    (char *)&sndBuf[0],
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
