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
       elSipClient.c
   \brief
       SIP User Agent Simple Client implementation
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
#include <dirent.h>
#include <sys/stat.h>
#include <sockets.h>

#ifdef __TCS__
#include <mmio.h>
#include <custom_defs.h>
#include <tmosal.h>
#include <tmDbg.h>
#endif

#include <elSipClient.h>
#include <elSipClientPrivate.h>


typedef struct elSipClientTaskParam {
    elSipClientHtmlHook_p pHtmlEntry;
    Int                    tableSize;
} elSipClientTaskParam_t, *elSipClientTaskParam_p;


/* DEBUG SUPPORT  */
#ifdef __TCS__
DBG_UNIT(dbgSipServer)
#endif

/* Local globals */
static volatile Bool elSipClientRunning = False;
static tmosalTaskHandle_t sipdTaskId;


//-----------------------------------------------------------------------------
// FUNCTION:    elSipClientDbgInit:
//
// DESCRIPTION: Register the Sip Server to the tmDbg unit.
//
// RETURN:      Void
//
// NOTES:       None
//-----------------------------------------------------------------------------
//
static Void
elSipClientDbgInit( Void )
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
// FUNCTION:    elSipClientGetSWVersion:
//
// DESCRIPTION: Get the Sip Server compatiblity number, major version
//              and minor version.
//
// RETURN:      EL_OK
//              EL_ERR_SIP_BAD_PARAMETER
//
// NOTES:       None.
//-----------------------------------------------------------------------------
//
elErrorCode_t
elSipClientGetSWVersion (
    ptmSWVersion_t    pSipVersion  //  O: pointer to structure to receive
                                    //     version information.
    )
{
    elErrorCode_t    err = EL_OK;

    if( pSipVersion == Null )
    {
        err = EL_ERR_SIP_BAD_PARAMETER;
    }
    else
    {
        pSipVersion->compatibilityNr = EL_SIP_SERVER_COMPATIBILITY_NR;
        pSipVersion->majorVersionNr  = EL_SIP_SERVER_MAJOR_VERSION_NR;
        pSipVersion->minorVersionNr  = EL_SIP_SERVER_MINOR_VERSION_NR;
    }

    return err;
} // elSipClientGetSWVersion

static  SOCKET  clientSocket;


//-----------------------------------------------------------------------------
// FUNCTION:    elSipClientSend
//
// DESCRIPTION: 
//
// RETURN:      EL_OK
//
//
// NOTES:       
//-----------------------------------------------------------------------------
//
elErrorCode_t elSipClientSend (
    Char                 *psBuf,
    Int                   len
)
{
   elErrorCode_t err = EL_OK;
   
   if (clientSocket) {
      err = iputs (clientSocket, len, psBuf);
   }
   else {
      err = EL_ERR_SIP_CLIENT_SOCKET;
   }

   return err;
}

//-----------------------------------------------------------------------------
// FUNCTION:    elSipClientTask
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

Void elSipClientTask(
   Void  *ptr
) 
{
   Int nRet;
   Int16 nPort;
   SOCKET listenSocket;
   SOCKADDR_IN saServer;
   struct sockaddr_in addr;
   Int addrlen = sizeof (struct sockaddr_in);
   Int flag = 1;
   elSipClientTaskParam_p  pParam = (elSipClientTaskParam_p)ptr;

   nPort = EL_SIP_SERVER_PORT;
   DBG_L0 ("%s: creating Sip Server task\n", __FUNCTION__);

   /*  Create a TCP/IP stream socket to "listen" with */
   listenSocket = socket (
        AF_INET,        /* Address family */
        SOCK_STREAM,    /* Socket type */
        IPPROTO_TCP     /* Protocol */
   );
   if (listenSocket == -1) 
   {
        PRINTERROR("socket()");
        tmosalTaskExit();
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
        closesocket(listenSocket);
#ifdef __TCS__
        tmosalTaskExit();
#endif
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
         closesocket(listenSocket);
#ifdef __TCS__
         tmosalTaskExit();
#endif
   }

   while (elSipClientRunning) 
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
            if (igets (clientSocket, sipInBuffer, 4096, 0) == 0) {
                DBG_L0 ("First line is >%s<\n", sipInBuffer);
                if ((strstr (sipInBuffer, "INVITE /") != NULL) &&
                    (strstr (sipInBuffer, "SIP/") != NULL)) 
                {
//                    elSipClientHtmlTable (pParam->pHtmlEntry, pParam->tableSize, clientSocket, sipInBuffer);
                }
            }
        }
#ifdef __TCS__
        tmosalTaskSleep(100); //NEW
#endif
        closesocket(clientSocket);

   } //while

   /*  Close SECOND sockets before exiting */
   closesocket(listenSocket);
#ifdef __TCS__
   tmosalTaskExit();
#else
#endif
}

//-----------------------------------------------------------------------------
// FUNCTION:    elSipClientInit:
//
// DESCRIPTION: This function initializes the Sip Server.  
//
// RETURN:      EL_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
static elSipClientTaskParam_t param; //TODO declare it in elSipClientInit()

elErrorCode_t 
elSipClientInit(
    Int                    taskPriority,
    elSipClientHtmlHook_p pHtmlEntry,
    Int                    tableSize
)
{
    elErrorCode_t err = EL_OK;
    
    elSipClientDbgInit();

    DBG_L0 ("%s: initialising Sip Server \n", __FUNCTION__);
    
    elSipClientRunning = True;
    
    param.pHtmlEntry = pHtmlEntry;
    param.tableSize  = tableSize;
    DBG_L0 ("%s: pHtmlEntry 0x%x \n", __FUNCTION__, pHtmlEntry);
    DBG_L0 ("%s: table size   %d \n", __FUNCTION__, tableSize);

    err = tmosalTaskCreate( elSipClientTask,
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
        elSipClientRunning = False;
        return err;
    }
    
    return err;
}


//-----------------------------------------------------------------------------
// FUNCTION:    elSipClientDeinit:
//
// DESCRIPTION: This function de-initializes the Sip Server.  
//
// RETURN:      EL_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
elErrorCode_t elSipClientDeinit(
    Void
)
{
    elErrorCode_t err = EL_OK;

    DBG_L0 ("%s: Deinitialising Sip Client \n", __FUNCTION__);
    elSipClientRunning = False;
    
    return err;
}
