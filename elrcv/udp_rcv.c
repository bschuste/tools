// tstLanEth.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <windows.h>
#include <io.h>
#include <winsock2.h>

#include "tstPattern.h"     //UDP test patterns


//#define DST_IP_ADDRESS  "192.168.1.1"
#define DST_IP_ADDRESS  "142.1.36.174"
#define BUFFER_SIZE 20480
#define MAX_BYTES   10.0 * 1024.0 *1024.0
#define UDP_MAX     4516


int init_ws2(void)
{
	WORD	ver_req;
	WSADATA wsa_data;
	int WsaError=0;

    ver_req = MAKEWORD(2, 0);
	if (WSAStartup(ver_req, &wsa_data) != 0)
	{
		WsaError = WSAGetLastError();
		printf("Error : Starting up WinSock2.0 DLL\n");
		return(WsaError);
	}
	if (LOBYTE(wsa_data.wVersion) != 2 || HIBYTE(wsa_data.wVersion) != 0)
	{
		WsaError = WSAGetLastError();
		printf("Error : Could not find WinSock2.0 DLL\n");
		WSACleanup();
		return(WsaError);
	}
	return(WsaError);
}


char* printnip(unsigned long ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%d.%d.%d.%d", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

//#define UDP_DEBUG

unsigned char *udpSetTestBuffer(int packet_size)
{
    int amount_patterns = DEFAULT_UTILIZED_PATTERNS;
    int i, j;
    unsigned char *pBuf;

    // Create the test reference content
    pBuf = (unsigned char *)malloc(packet_size * TOTAL_AMOUNT_PATTERNS);
    if (pBuf == NULL) {
	    printf (" Memory allocation failed, returning\n");
        return pBuf;
    }

    memset((char *) pBuf, 0, packet_size * TOTAL_AMOUNT_PATTERNS);

    for (i=0; i<amount_patterns; i++) {
        // create each packet
        pBuf[i * (packet_size)]     = DEFAULT_PATTERN_HEADER;
        pBuf[i * (packet_size) + 1] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packet_size) + 2] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packet_size) + 3] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packet_size) + 4] = DEFAULT_COUNTER_VALUE;
        for (j=5; j<packet_size; j++) {
            pBuf[i * (packet_size) + j] = patternTable[(j-5)%4 + i*4];
        }
    }

#ifdef UDP_DEBUG
    for (i=0; i<(packet_size)*amount_patterns; i++) {
        if (i%4 == 0) printf("\n");
        if (i%(packet_size) == 0) printf("\n");
        printf(" 0x%02X", pBuf[i]);
    }
#endif
    return pBuf;
}

#define UDP_NOT_CONNECTED

int _tmain(int argc, _TCHAR* argv[])
{
   struct sockaddr_in snd_sin;
#ifdef UDP_NOT_CONNECTED
   struct sockaddr_in rcv_sin;
   int socklen;
#endif
   int rc = 0, i, j, nbytes;
   double total_bytes = 0.0;
   double target_bytes = MAX_BYTES;
   unsigned char rcvBuf[BUFFER_SIZE];
   char c='\0';
   int nocheck = 0;
   unsigned short g_port;
   unsigned long  dst_ip;
   SOCKET	serverSocket = -1;   // Server listening socket
   FILE *fp = 0;
   bool file_enable = false;
   char    buffer[128];

    if (argc<2)
    {
      printf("usage: %s portnumber [size in KB, default is 3KB]\n",argv[0]);
      printf("example: %s 5006 10\n",argv[0]);
      return 0;
    }
    g_port = atoi(argv[1]);
	printf(" Streaming port = %d\n",g_port);
	if (init_ws2() != 0) {
		printf("Winsock initialization failed\n");
	}
    file_enable = true;
    if (argc == 3)
	{
        target_bytes = 1024.0 * atoi(argv[2]);
    }
	else
	{
		target_bytes = 1024.0 * 3;
	}
   nocheck = 1;

   // Create a  UDP/IP socket
   if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
   {
      printf ("Allocating socket failed. Error: %d\n", WSAGetLastError());
      WSACleanup();
      return -1;
   }



   { //RECEIVE

      // Only get from the selected peer port
      rcv_sin.sin_family = AF_INET;
      rcv_sin.sin_port   = htons(g_port);
      rcv_sin.sin_addr.s_addr = htonl(INADDR_ANY);
      socklen = sizeof(struct sockaddr_in);
      rc = bind(serverSocket, (struct sockaddr *)&rcv_sin, sizeof(struct sockaddr_in));
      if ( rc < 0 )
      {
        printf("Error binding socket to interface errno %d\n", WSAGetLastError());
        closesocket(serverSocket);
         WSACleanup();
        return rc;
      }

      for (i=0;i<BUFFER_SIZE;i++) 
	  {
        rcvBuf[i] = 0;
      }
	  {
        char outName[128];
        printf(" The application quits when %f bytes have been received\n",target_bytes);
        sprintf(outName,"%s",argv[0]);
        strcat(outName,".mpg");
        fp = fopen(outName,"wb");
        if (!fp)
            printf("Cannot open %s in current directory\n", outName);
  	    printf(" Output file name = %s\n", outName);
      }

      i = 0;

      do {
      nbytes = recvfrom( serverSocket,
                        (char *)&rcvBuf[0],
                         UDP_MAX,
                         0,
       		            (struct sockaddr *)&rcv_sin,
                         &socklen
                       );
		if (nbytes < 0) {
         printf("recvfrom(): nbytes %d errno %d\n", nbytes, WSAGetLastError());
		}
		else {
         total_bytes += (double)nbytes;
         if (fp) fwrite(&rcvBuf[0], nbytes, 1, fp);
		}
      }
      while ((!file_enable)|(total_bytes <= target_bytes));

      if (fp) fclose(fp);
      if (serverSocket > 0) closesocket(serverSocket);
      printf(" Total_bytes %f\n", total_bytes);

   } //receive
	return 0;
}
