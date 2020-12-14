// tstLanEth.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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
   unsigned int counter, ref_counter;
   int packet_size = DEFAULT_PATTERN_LENGTH + DEFAULT_HEADER_LEN;
   unsigned char *pBuf;
   int amount_patterns = DEFAULT_UTILIZED_PATTERNS;
   bool file_enable = false;
   char    buffer[128];

   if ((argc!=3)&(argc!=5)&(argc!=6))
   {
      printf("usage: %s portnumber ipaddress [1?packet_size:Kbytes] [nocheck]\n",argv[0]);
      printf("example with packet size:   %s 6000 192.168.1.1 1 64\n",argv[0]);
      printf("example with max byte size: %s 6000 192.168.1.1 0 10\n",argv[0]);
      printf("This application can check for data integrity (packet_size) or can be used to store a given amount of bytes\n");
      return 0;
   }
	g_port = atoi(argv[1]);
	printf(" Streaming port = %d\n",g_port);
	dst_ip = inet_addr(argv[2]);
	printf(" Streaming IP address = %s\n",printnip(dst_ip));
	if (init_ws2() != 0) {
		printf("Winsock initialization failed\n");
	}
    if (argc == 5)
        if (atoi(argv[3]) > 0) {
            packet_size = atoi(argv[4]);
        }
        else {
            target_bytes = 1024.0 * atoi(argv[4]);
            file_enable = true;
        }

   if (argc == 6) {
      nocheck = 1;
   }

   // Create a  UDP/IP socket
   if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
   {
      printf ("Allocating socket failed. Error: %d\n", WSAGetLastError());
      WSACleanup();
      return -1;
   }

   printf("Send or receive? ");
   fflush(stdout);
   gets(buffer);
   /* 
   printf("Peer IP address (%s): ", DST_IP_ADDRESS);
   fflush(stdout);
   buffer[0] = 0;
   gets(buffer);
   if (strlen(buffer) < 1) strcpy(buffer, DST_IP_ADDRESS);
   dst_ip = inet_addr(buffer);
   */
   
   // Fill out the peer socket's address information.
   snd_sin.sin_family = AF_INET;
   snd_sin.sin_port = htons(g_port);  
   snd_sin.sin_addr.s_addr = dst_ip; //inet_addr("192.168.2.102");
   
   pBuf = udpSetTestBuffer(packet_size);
   if (pBuf == NULL) return 0;

   if (buffer[0] == 's' || buffer[0] == 'S')
   { //SEND
      counter = 0;
      i = 0;
   
      while (1) {
//      while ( (total_bytes < ((Double)ptmNet->duration * ptmNet->bitRate)/8.0)
//          & ( udpSendTaskRunning == True ) ) {

        pBuf[i * (packet_size) + 1] = (counter >> 24)&0xFF; 
        pBuf[i * (packet_size) + 2] = (counter >> 16)&0xFF; 
        pBuf[i * (packet_size) + 3] = (counter >> 8)&0xFF; 
        pBuf[i * (packet_size) + 4] =  counter&0xFF; 
#ifdef UDP_NO_CONNECT
         if ((sendto(serverSocket, (char *)&pBuf[i * (packet_size)], packet_size, 0, 
            (struct sockaddr *)&local_sin, sizeof(local_sin)))==-1)
         {
            printf("Error while sending %s:%u...\r\n", inet_ntoa(local_sin.sin_addr), ntohs(local_sin.sin_port));
         } 
#else
        rc = send(serverSocket, (char *)&pBuf[i * (packet_size)], packet_size, 0);
#endif
        total_bytes  += (double)packet_size;
        counter++;
        i++;
        if (i >= amount_patterns) i = 0;
        //Should wait a little bit here
      } //while
   }
   else
   { //RECEIVE

#ifdef UDP_NOT_CONNECTED
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
#else

      rc = connect(serverSocket,(struct sockaddr*)&snd_sin, sizeof(struct sockaddr_in));
      if ( rc < 0 )
      {
         printf("Error connect socket to interface errno %d\n", WSAGetLastError());
         closesocket(serverSocket);
         WSACleanup();
         return rc;
      }
#endif

      for (i=0;i<BUFFER_SIZE;i++) {
        rcvBuf[i] = 0;
      }
      if (atoi(argv[3]) == 0) {
        char outName[128];
        printf(" The application quits when %f bytes have been received\n",target_bytes);
        sprintf(outName,"%s",argv[0]);
        strcat(outName,".mpg");
        fp = fopen(outName,"wb");
        if (!fp)
            printf("Cannot open %s in current directory\n", outName);
  	    printf(" Output file name = %s\n", outName);
      }

      ref_counter = 0; counter = 0;
      i = 0;

      do {
#ifdef UDP_NOT_CONNECTED
      nbytes = recvfrom( serverSocket,
                        (char *)&rcvBuf[0], 
                         UDP_MAX,
                         0,
       		            (struct sockaddr *)&snd_sin, 
                         &socklen
                       );
#else
      nbytes = recv(serverSocket, (char *)&rcvBuf[0], UDP_MAX, 0);
#endif
     if (nbytes < 0) {
         printf("recvfrom(): nbytes %d errno %d\n", nbytes, WSAGetLastError());
      }
      else {
         total_bytes += (double)nbytes;
         if (fp) fwrite(&rcvBuf[0], nbytes, 1, fp);
         if (nocheck == 0) {
         if (rcvBuf[0] != DEFAULT_PATTERN_HEADER)
            printf(" wrong header 0x%x\n", rcvBuf[0]);

         counter  = (rcvBuf[1] << 24);
         counter += (rcvBuf[2] << 16);
         counter += (rcvBuf[3] << 8);
         counter += (rcvBuf[4]);
         if (ref_counter > counter) {
            printf(" got %d more packets than expected\n", ref_counter - counter);
            ref_counter = counter;
         }
         else if (ref_counter < counter) {
            printf(" lost %d packets\n", counter - ref_counter);
            ref_counter = counter;
         }
         else {
             for (j=5; j<packet_size; j++) {
                if (rcvBuf[j] != pBuf[i * (packet_size) + j])
                    printf(" wrong data 0x%02X, counter %d, i %d, j %d\n", rcvBuf[j], counter, i, j);
             }
         }

         ref_counter++;
         i = ref_counter % amount_patterns;
         }
      }
      }
      while ((!file_enable)|(total_bytes <= target_bytes));

      if (fp) fclose(fp);
      if (serverSocket > 0) closesocket(serverSocket);
      printf(" Total_bytes %f\n", total_bytes);
   } //receive
   
   return 0;
}
