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
 ***********************************************************************/
/*!
 ***********************************************************************
 **
   \file 
       elSipFiles.c
   \brief
       Project Html files content for tmHttpServer
   \author
       Brigitte Schuster
   \date
       Last update: 2006/07/14
   \todo
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>

#include "../inc/elportab.h"
#include "../inc/elerr.h"

#include <elSipUa.h>
#include <elSipUaPrivate.h>

#ifdef __TCS__
DBG_UNIT_EXTERN(dbgHttpServer)
#endif

#define RXBUFSIZE       8192

static Int  rxwkin = 0, rxwkout = 0;
static Char rxwkbuf [RXBUFSIZE];
static Char capbuf[SIP_BUFFER_SIZE];

int elSipGets (SOCKET remoteSocket, char *buf, int len, int echo) 
{
    Int nRet = 0;
    Int i, thislen, loop;

    if (rxwkin <= rxwkout) {
        rxwkin = 0;
        rxwkout = 0;
        loop = 1;
        do {
            /*  Receive data from the client */
            DBG_L0 ("%s: receiving data from client\n", __FUNCTION__);
            thislen = recv (
                remoteSocket,       /*  Connected client */
                rxwkbuf + rxwkin,          /*  Receive buffer */
                RXBUFSIZE - 1 - rxwkin,           /*  Length of buffer */
                0                   /*  Flags */
            );
            DBG_L0 ("%s: len %d\n", __FUNCTION__, thislen);
            if (thislen < 0) {
                PRINTERROR("recv()");
                return (nRet);
            }
            rxwkbuf [rxwkin + thislen] = '\0';

            /*  Send data back to the client */
            /*strcpy (buf, "From the Server");*/
            /*wal3701: no printf ("%s", rxwkbuf + rxwkin); */
            if (echo != 0) 
            {
                DBG_L0 ("%s: sending back to client\n", __FUNCTION__);
                nRet = send (
                    remoteSocket,           /*  Connected socket */
                    rxwkbuf + rxwkin,       /*  Data buffer */
                    thislen,                /*  Lenght of data */
                    0                       /*  Flags */
                );
                if (nRet < 0) {
                    PRINTERROR("send()");
                    return (nRet);
                }
            }

            for (i = 0; i < thislen; i++) {
                if ((rxwkbuf [rxwkin + i] == '\r') || (rxwkbuf [rxwkin + i] == '\n')) {
                    loop = 0;
                    break;
                }
            }
            rxwkin += thislen;
        } while ((rxwkin < (RXBUFSIZE - 1)) && (loop != 0));
    }

    for (i = rxwkout; i < rxwkin; i++) {
        if ((rxwkbuf [i] == '\r') || (rxwkbuf [i] == '\n')) {
            /*  found end */
            /*  add NULL */
            rxwkbuf [i] = '\0';
            break;
        }
    }

    strcpy (buf, rxwkbuf + rxwkout);

    rxwkout = i + 2;        /*  for '\n' */
    DBG_L0(" %s\n", buf);

    return (0);
} /* elSipGets */

int elSipPuts (SOCKET remoteSocket, Int len, Char *buf) 
{
   Int nRet = 0;

   /*  Send data back to the client */
   DBG_L0("elSipPuts: strlen (buf) %d, len %d\n", strlen (buf), len);
   DBG_L0(" %s\n", buf);
   DBG_L0(" \n");
   
   nRet = send (
            remoteSocket,               /*  Connected socket */
            buf,                        /*  Data buffer */
            len ? len : strlen (buf),   /*  Length of data */
            0                           /*  Flags */
   );
   if (nRet < 0) {
      PRINTERROR("send()");
      return (-1);
   }
   DBG_L0("elSipPuts: send done\n");

   return (0);

} /* elSipPuts */


Int elSipServerReply ( 
    elSipServerParam_p pPar,
    SOCKET remoteSocket, 
    Char *sipInBuffer,
    Char *sipOutBuffer)
{
   elErrorCode_t err = EL_OK;
   Int i;
   fd_set fds;
   Bool foundPage = False;

   /*  flush all inputs */
   rxwkin = 0;
   rxwkout = 0;

   for (i=0;i<SIP_BUFFER_SIZE;i++) {
      capbuf[i] = '\0';
   }

   sscanf (sipInBuffer, "%*s %s", capbuf);
   if (capbuf [1] == '\0') 
   {
      strcpy (capbuf, "/index.htm");
   }
   DBG_L0("capuf %s\n", capbuf);

   FD_ZERO(&fds);
   FD_SET(remoteSocket, &fds);


   sprintf (
    sipOutBuffer,
    SIP_REPLY_SUCCESS
    "Via: SIP/2.0/UDP %s;\r\n"
    "branch=%s;\r\n"
    "To: %s <sip:%s>;\r\n"
    "tag=%d\r\n"
    "From: %s <sip:%s>;\r\n"
    "tag=%d\r\n"
    "Call-ID: %d\r\n"
    "CSeq: %d INVITE\r\n"
    "Contact: <sip:%s>\r\n"
    "Content-Type: application/sdp\r\n"
    "Content-Length: %d\r\n"
    "%s",
    pPar->sipReceiverName,
    pPar->sipBranchName,
    pPar->sipReceiverShortName, pPar->sipReceiverName,
    pPar->tagRcvId,
    pPar->sipSenderShortName, pPar->sipSenderName,
    pPar->tagSndId,
    pPar->callId,
    pPar->cseqNumber,
    pPar->sipSenderName,
    pPar->sipLength,
    pPar->sdpData 
    );

#if 0
   if (foundPage == False) {
      // check if table provides a not found function
      // it is up to the application now to look 
      // for this page
      // Notice that the call is used two arguments
      for (i=0;i<tableSize;i++) 
      {
         if ((strcmp ("/", pHtmlEntry[i].htmlName) == 0)) {
            httpArg = (tmHttpServerDisk_p)pHtmlEntry[i].htmlArg;
            httpArg->capbuf = &capbuf[0];
            err = pHtmlEntry[i].htmlFunc(pHtmlEntry[i].htmlArg);
            DBG_L0("%s: err = %d\n", __FUNCTION__, err);
            if (err == EL_OK) {
               foundPage = True;
            } 
         }
      }
   }
#endif

   if (foundPage == False) {
      // send a not found page if no entry has been found!

      for (i=0;i<SIP_BUFFER_SIZE;i++) {
         capbuf[i] = '\0';
      }
      sprintf(capbuf, SIP_REPLY_NOT_FOUND
                      "Server: %s\r\n\r\n",
                      SIP_SERVER_NAME);
      elSipPuts (remoteSocket, 0, capbuf);
   }
   return err;
}
