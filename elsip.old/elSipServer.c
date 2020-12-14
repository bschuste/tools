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
       elSipServer.c
   \brief
       SIP User Agent Simple Server implementation
   \author
       Brigitte Schuster
   \date
       Last update: 2010/12/19
   \todo
 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <sys/stat.h>

#include "../inc/elportab.h"
#include "../inc/elerr.h"

#include <elSipUa.h>
#include <elSipUaPrivate.h>


static elSipServerParam_p pParam;

/* DEBUG SUPPORT  */
#ifdef __TCS__
DBG_UNIT(dbgSipServer)
#endif

/* Local globals */
static volatile Bool elSipServerRunning = False;
#ifdef __TCS__
static tmosalTaskHandle_t sipdTaskId;
#else
static pthread_t sipdThreadId = NULL;
#endif


//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerDbgInit:
//
// DESCRIPTION: Register the Sip Server to the tmDbg unit.
//
// RETURN:      Void
//
// NOTES:       None
//-----------------------------------------------------------------------------
//
static Void elSipServerDbgInit( Void )
{
    static Bool bInitDbg = True;

    if( bInitDbg )
    {
#ifdef __TCS__
        DBG_REGISTER_MODULE( dbgSipServer, "dbgSipServer" );
#endif
        bInitDbg = False;
    }
}


//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerGetSWVersion:
//
// DESCRIPTION: Get the Sip Server compatiblity number, major version
//              and minor version.
//
// RETURN:      EL_OK
//              EL_ERR_BAD_PARAMETER
//
// NOTES:       None.
//-----------------------------------------------------------------------------
//
#if 0

elErrorCode_t
elSipServerGetSWVersion (
    ptmSWVersion_t    pSipVersion  //  O: pointer to structure to receive
                                    //     version information.
    )
{
    elErrorCode_t    err = EL_OK;

    if( pSipVersion == Null )
    {
        err = EL_ERR_BAD_PARAMETER;
    }
    else
    {
        pSipVersion->compatibilityNr = EL_SIP_SERVER_COMPATIBILITY_NR;
        pSipVersion->majorVersionNr  = EL_SIP_SERVER_MAJOR_VERSION_NR;
        pSipVersion->minorVersionNr  = EL_SIP_SERVER_MINOR_VERSION_NR;
    }

    return err;
} // elSipServerGetSWVersion
#endif


static  SOCKET  clientSocket;

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerSend
//
// DESCRIPTION: 
//
// RETURN:      EL_OK
//
//
// NOTES:       
//-----------------------------------------------------------------------------
//
elErrorCode_t elSipServerSend (
    Char                 *psBuf,
    Int                   len
)
{
   elErrorCode_t err = EL_OK;
   
   if (clientSocket) {
      err = elSipPuts (clientSocket, len, psBuf);
   }
   else {
      err = EL_ERR_SIP_CLIENT_SOCKET;
   }

   return err;
}

#if 0
sprintf (
    sipOutBuffer,
    "INVITE sip:%s SIP/2.0\r\n"
    "Via: SIP/2.0/UDP %s;\r\n"
    "branch=%s\r\n"
    "Max-Forwards: %d\r\n"
    "To: %s <sip:%s>\r\n"
    "From: %s <sip:%s>;\r\n"
    "tag=%d\r\n"
    "Call-ID: %s\r\n"
    "CSeq: %d INVITE\r\n"
    "Contact: <sip:%s>\r\n"
    "Content-Type: application/sdp\r\n"
    "Content-Length: %d\r\n"
    "%s",
    sipReceiverName,
    sipDomainName,
    sipBranchName,
    maxForwards,
    sipReceiverShortName, sipReceiverName,
    sipSenderShortName, sipSenderName,
    tagId,
    callId
    cseqNumber,
    sipSenderName,
    sipLength,
    sdpData
);


#endif

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerTask
//
// DESCRIPTION: Sip Server listening and dispatching task.  
//
// RETURN:      EL_OK
//
//
// NOTES:       
//-----------------------------------------------------------------------------
//
#define MAX_UDP_PACKET_SIZE     65536

static Char sipInBuffer [MAX_UDP_PACKET_SIZE];
static Char sipOutBuffer[MAX_UDP_PACKET_SIZE];

Void *elSipServerTask (
   Void  *ptr
) 
{
   Int nRet = 0;
   Int16 nPort = EL_SIP_SERVER_PORT;
   SOCKET listenSocket;
   SOCKADDR_IN saServer;
   struct sockaddr_in addr;
   Int addrlen = sizeof (struct sockaddr_in);
   Int flag = 1;
   elSipServerParam_p  pParam = (elSipServerParam_p)ptr;

   DBG_L0 ("%s: creating Sip Server task, port %d\n", __FUNCTION__, nPort);

   /*  Create a TCP/IP stream socket to "listen" with */
   listenSocket = socket (
        AF_INET,        /* Address family */
        SOCK_STREAM,    /* Socket type */
        IPPROTO_TCP     /* Protocol */
   );
   if (listenSocket == -1) 
   {
        PRINTERROR("socket()");
        perror("socket");
        OSAL_TASK_EXIT(NULL);
   }
    
   /* Set socket for fast reply */
   setsockopt( listenSocket, IPPROTO_TCP, TCP_NODELAY, (Char*)&flag, sizeof(Int));

   /*  Fill in the address structure */
   saServer.sin_family = AF_INET;
   saServer.sin_addr.s_addr = htonl(INADDR_ANY);
   saServer.sin_port = htons(nPort);              /* Use port from command line */

   /*  bind the name to the socket */
   nRet = bind (
        listenSocket,                 /* Socket */
        (struct sockaddr *)&saServer, /* Our address */
        sizeof (struct sockaddr)      /* Size of address structure */
   );
   if (nRet < 0) 
   {
        PRINTERROR("bind()");
        perror("bind");
#ifdef __TCS__
        closesocket(listenSocket);
#else
        close(listenSocket);
#endif
        OSAL_TASK_EXIT(NULL);
   }
   /*  Set the socket to listen */
   DBG_L0("listen()\n");
   nRet = listen (
         listenSocket,   /* Bound socket */
         MAX_BACKLOG
   );
   if (nRet < 0) 
   {
         PRINTERROR("listen()");
#ifdef __TCS__
        closesocket(listenSocket);
#else
        close(listenSocket);
#endif
         OSAL_TASK_EXIT(NULL);
   }

   while (elSipServerRunning) 
   {
        /*  Wait for an incoming request */
        DBG_L0("Blocking at accept()\n");
        clientSocket = accept (
            listenSocket,   /* Listening socket */
            (struct sockaddr *) &addr,
            &addrlen
        );
        if (clientSocket == -1) {
            PRINTERROR("accept()");
        }
        else {
            /*
            We're connected to a client
            New socket descriptor returned already
            has clients address
            */
            DBG_L0 ("%s: we are connected to a client\n", __FUNCTION__);
            if (elSipGets (clientSocket, sipInBuffer, 4096, 0) == 0) {
                DBG_L0 ("First line is >%s<\n", sipInBuffer);
                if ((strstr (sipInBuffer, "INVITE /") != NULL) &&
                    (strstr (sipInBuffer, "SIP/") != NULL)) 
                {
                    elSipServerReply (pParam, clientSocket, sipInBuffer, sipOutBuffer);
                }
            }
        }
        OSAL_TASK_EXIT(NULL);
#ifdef __TCS__
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif

   } //while

   /*  Close SECOND sockets before exiting */
#ifdef __TCS__
   closesocket(listenSocket);
#else
   close(listenSocket);
#endif
   OSAL_TASK_EXIT(NULL);
}

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerInit:
//
// DESCRIPTION: This function initializes the Sip Server.  
//
// RETURN:      EL_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//

elErrorCode_t elSipServerInit(
    Int                    taskPriority
)
{
    elErrorCode_t err = EL_OK;
    
    elSipServerDbgInit();

    DBG_L0 ("%s: initialising Sip Server \n", __FUNCTION__);
    
    elSipServerRunning = True;
    
    pParam = (elSipServerParam_p) malloc(sizeof(elSipServerParam_t));
    if (pParam == NULL)
    {
        return EL_ERR_MALLOC;
    }
    else
    {
        pParam->sipReceiverName         = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->sipBranchName           = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->sipReceiverShortName    = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->sipReceiverName         = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->tagRcvId                = 0;
        pParam->sipSenderShortName      = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->sipSenderName           = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->tagSndId                = 0;
        pParam->callId                  = 0;
        pParam->cseqNumber              = 0;
        pParam->sipSenderName           = (Char *)malloc(EL_SIP_NAME_MAXSIZE);
        pParam->sipLength               = 0;
        pParam->sdpData                 = (UInt8 *)malloc(EL_SIP_NAME_MAXSIZE);
    }

#ifdef __TCS__
    err = OSAL_TASK_CREATE( elSipServerTask,
                            &param,
                            taskPriority,
                            SIP_STACK_SIZE, 
                           &sipdTaskId,
                            "SIPS", 
                            tmosalTaskStarted
                          );

    if (err != EL_OK)
    {
        DBG_L0 ("%s: failed to launch Sip Server, 0x%x\n", __FUNCTION__ ,err);
        elSipServerRunning = False;
        return err;
    }
#else

    if (pthread_create(&sipdThreadId, NULL, elSipServerTask, (void*)pParam) != 0)
    {
        DBG_L0 ("%s: failed to launch Sip Server, 0x%x\n", __FUNCTION__ ,err);
        elSipServerRunning = False;
        return EL_ERR_PTHREAD;
    }
#endif
    
    return err;
}


//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerDeinit:
//
// DESCRIPTION: This function de-initializes the Sip Server.  
//
// RETURN:      EL_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
elErrorCode_t elSipServerDeinit(
    Void
)
{
    elErrorCode_t err = EL_OK;
    
    DBG_L0 ("%s: Deinitialising Sip Server \n", __FUNCTION__);
    elSipServerRunning = False;
    
    if (pthread_join(sipdThreadId, NULL))
    {
        DBG_L0 ("%s: Failed to synchronize shutdown of Sip Server errno=%i\n", __FUNCTION__, errno);
    }
    free (pParam);
    
    return err;
}
