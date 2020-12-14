/***********************************************************************
 **
   Copyright (c) Electropix 2002.                                        
   All rights reserved. Reproduction in whole or in part is prohibited   
   without the written consent of the copyright holder.                  
                                                                         
   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 
   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Electropix Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **
 ***********************************************************************
 **
   Author: Brigitte Schuster
   Last update: 2009/10/09
   Modified: 
   File: udp_snd.c
   History:
        Version 0.1  2009/10/09
 **
 ***********************************************************************/

#include <stdio.h>
#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>


#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif

#include <windows.h>
#include <io.h>
#include <winsock2.h>

#include "viSportab.h"
#include "viSerror.h"
#include "elUdpPattern.h"     //UDP test patterns


#define DST_IP_ADDRESS  "192.168.1.1"
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

char* printnip(UInt32 ip)
{
   static char printbuf[40];
   sprintf(printbuf,"%d.%d.%d.%d", (ip >> 0)&0xff,(ip >> 8)&0xff,(ip >> 16)&0xff,(ip >> 24)&0xff);
   return printbuf;
}

//#define UDP_DEBUG

unsigned char *udpSetTestBuffer(int packetSize)
{
    int amountPatterns = DEFAULT_UTILIZED_PATTERNS;
    int i, j;
    unsigned char *pBuf;

    // Create the test reference content
    pBuf = (unsigned char *)malloc(packetSize * TOTAL_AMOUNT_PATTERNS);
    if (pBuf == NULL) {
	    printf (" Memory allocation failed, returning\n");
        return pBuf;
    }

    memset((char *) pBuf, 0, packetSize * TOTAL_AMOUNT_PATTERNS);

    for (i=0; i<amountPatterns; i++) {
        // create each packet
        pBuf[i * (packetSize)]     = DEFAULT_PATTERN_HEADER;
        pBuf[i * (packetSize) + 1] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packetSize) + 2] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packetSize) + 3] = DEFAULT_COUNTER_VALUE;
        pBuf[i * (packetSize) + 4] = DEFAULT_COUNTER_VALUE;
        for (j=5; j<packetSize; j++) {
            pBuf[i * (packetSize) + j] = patternTable[(j-5)%4 + i*4];
        }
    }

#ifdef UDP_DEBUG
    for (i=0; i<(packetSize)*amountPatterns; i++) {
        if (i%4 == 0) printf("\n");
        if (i%(packetSize) == 0) printf("\n");
        printf(" 0x%02X", pBuf[i]);
    }
#endif
    return pBuf;
}

int _tmain(int argc, _TCHAR* argv[])
{

    struct sockaddr_in snd_sin;
//    struct sockaddr_in rcv_sin;
//    int socklen;
    Int serverSocket;
    int i, nbytes;
    UInt8  sndBuf[BUFFER_SIZE];
    UInt16 gPort;
    UInt32 dstIpAddress;

    FILE *fp = 0;
    Bool fileEnable = False;
    Char ReadName[256];

    unsigned int counter;
    int packetSize = DEFAULT_PATTERN_LENGTH + DEFAULT_HEADER_LEN;
    unsigned char *pBuf;
    int amountPatterns = DEFAULT_UTILIZED_PATTERNS;
    double totalBytes = 0.0;
//    double targetBytes = MAX_BYTES;

 
    printf("usage: %s name [ipaddress [portnumber]]\n",argv[0]);
    printf("       if name == \"test\" then a test pattern is used\n");
    printf("                         else a file is sent in a forever loop\n");

    if (argc>=2)
    {
        if (strcmp(argv[1],"test")!=0)
            fileEnable = True;

        if (argc>=3)
        {
            dstIpAddress = inet_addr(argv[2]);
            printf(" Streaming IP address = %s\n",printnip(dstIpAddress));
            if (argc>=4)
            {
                gPort = atoi(argv[1]);
                printf(" Streaming port = %d\n",gPort);
            }
        }
    }
    if (init_ws2() != 0) {
       printf("Winsock initialization failed\n");
    }
    for (i=0;i<256;i++) {
        ReadName[i] = '\0';
    }

#if 0
    targetBytes = 1024.0 * atoi(argv[2]);
    packetSize = atoi(argv[4]);
#endif

    // Create a  UDP/IP socket
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf ("Allocating socket failed. Error: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Fill out the peer socket's address information.
    snd_sin.sin_family = AF_INET;
    snd_sin.sin_port = htons(gPort);  
    snd_sin.sin_addr.s_addr = dstIpAddress; //inet_addr("192.168.2.102");
    
    if (fileEnable == False)
    {
        pBuf = udpSetTestBuffer(packetSize);
        if (pBuf == NULL) return 0;
        counter = 0;
        i = 0;
   
        while (1) 
        {
            pBuf[i * (packetSize) + 1] = (counter >> 24)&0xFF; 
            pBuf[i * (packetSize) + 2] = (counter >> 16)&0xFF; 
            pBuf[i * (packetSize) + 3] = (counter >> 8)&0xFF; 
            pBuf[i * (packetSize) + 4] =  counter&0xFF; 
            if ((sendto(serverSocket, (char *)&pBuf[i * (packetSize)], packetSize, 0, 
                (struct sockaddr *)&snd_sin, sizeof(snd_sin)))==-1)
            {
                printf("Error while sending %s:%u...\r\n", inet_ntoa(snd_sin.sin_addr), ntohs(snd_sin.sin_port));
            } 
            totalBytes  += (double)packetSize;
            counter++;
            i++;
            if (i >= amountPatterns) i = 0;
            //Should wait a little bit here
        } //while

    }
    else
    {
        //Send a file
        if ((fp=fopen(ReadName,"rb"))==NULL) {
            printf("error : cannot open %s\n",ReadName);
            return(VIS_ERR_OPEN_FILE);
        }
        while (1)
        {
            fseek(fp,0,SEEK_SET);
            do 
            {
                nbytes = fread(&sndBuf[0], BUFFER_SIZE, sizeof(UInt8),fp);
                if ((sendto(serverSocket, (char *)&sndBuf[0], nbytes, 0, 
                            (struct sockaddr *)&snd_sin, sizeof(snd_sin)))==-1)
                {
                    printf("Error while sending %s:%u..%x\r\n", 
                                           inet_ntoa(snd_sin.sin_addr), 
                                           ntohs(snd_sin.sin_port), 
                                           WSAGetLastError());
                } 
            } while (nbytes >= 1);
        }
        
        fclose(fp);
    }
    return 0;
}
