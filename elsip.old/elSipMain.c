/***********************************************************************
 **
   Copyright (c) Electropix 2010.                                        
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
   Last update: 2010/12/19
   Modified:
   File: elsip.c
   History:
        Version 0.1  2010/12/19
 **
 ***********************************************************************/


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

#include "../inc/elportab.h"
#include "../inc/elerr.h"
#include "elSipUa.h"

#define BUFFER_SIZE             64
#define DEFAULT_IP_ADDRESS      "172.25.41.198"
#define DEFAULT_PORT            5060
#define INVALID_SOCKET          -1

/* 
 * sip:root@172.26.176.8:port
 */


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
    unsigned short g_port;
    if (argc < 3)
//   if ((argc!=1)&(argc!=2)&(argc!=3))
   {
        printf(" sip [portnumber - default is %d] [ipaddress - default is %s]\n",
                DEFAULT_PORT, DEFAULT_IP_ADDRESS);
        printf(" example: sip 46999 192.168.2.102\n");
        printf("  version 0.1\n");
        printf("  send e-mail to brigitte@electropix.com for additional features\n\n");
        return 0;
    }

    
    printf("  %s version 0.1\n", argv[0]);
    printf("  send e-mail to brigitte@electropix.com for additional features\n\n");
    if (argc > 1) g_port = atoi(argv[1]);

    elSipServerInit(0);

    printf("  Enter return to quit\n");
    
    getchar();
    elSipServerDeinit();
    
    printf(" Exiting elsip application...\n");

   return 0;
}
