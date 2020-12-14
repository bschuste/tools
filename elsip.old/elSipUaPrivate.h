/***********************************************************************
 **
   Copyright (c) Electropix 2006, 2007. All rights reserved. 

   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 
   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Electropix does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **
 ***********************************************************************/
/*!
 ***********************************************************************
 **
   \file 
       tmHttpServerPrivate.h
   \brief
       Useful definitions
   \author
       Brigitte Schuster
   \date
       Last update: 2006/07/14
   \todo
 ***********************************************************************/
#ifndef EL_SIPUA_SERVER_PRIVATE_H
#define EL_SIPUA_SERVER_PRIVATE_H


#define SIP_SERVER_COMPATIBILITY_NR         1   // Sip Server Compatibility number
#define SIP_SERVER_MAJOR_VERSION_NR         1   // Sip Server Major Version number
#define SIP_SERVER_MINOR_VERSION_NR         0   // Sip Server Minor Version number

#define MAX_BACKLOG              3
#define MAXFNSZ                  256
#define K_BYTES                  1024
#define M_BYTES                 (K_BYTES*K_BYTES)


#define SIP_STACK_SIZE              64*1024
 #define EL_SIP_NAME_MAXSIZE        256

#ifdef __TCS__
#define OSAL_TASK_EXIT           tmosalTaskExit

#define OSAL_TASK_CREATE         tmosalTaskExit
    err = tmosalTaskCreate( elSipServerTask,
                            &param,
                            taskPriority,
                            HTTP_STACK_SIZE, 
                           &sipdTaskId,
                            "SIPS", 
                            tmosalTaskStarted
                          );

#else

#define OSAL_TASK_EXIT           pthread_exit

#endif

//-----------------------------------------------------------------------------
// Types and defines
//-----------------------------------------------------------------------------
#if __TCS__

#define DBG_L0(fmt...) \
   DBG_PRINT( (dbgHttpServer, DBG_LEVEL_1, fmt) );

#define DBG_L1(fmt...) \
   if (dbgLevel > 0) DBG_PRINT( (dbgHttpServer, DBG_LEVEL_2, fmt) );

#define DBG_L2(fmt...) \
   if (dbgLevel > 1) DBG_PRINT( (dbgHttpServer, DBG_LEVEL_3, fmt) );

#else

#define DBG_L0(fmt...) \
   fprintf(stderr, fmt); 

#define DBG_L1(fmt...) \
   if (dbgLevel > 0) DBG_L0(fmt); 

#define DBG_L2(fmt...) \
   if (dbgLevel > 1) DBG_L0(fmt); 

#endif

#define DBG_ERR(a) \
   (a); \
   if (err != EL_OK) { \
      DBG_L0("Exited %s: error (0x%08x) at line %d, file %s\n", __FUNCTION__, err, __LINE__, __FILE__); \
      return err; \
   }\

//NO Return just signal a problem
#define DBG_ERR_NOR(a) \
   (a); \
   if (err != EL_OK) { \
      DBG_L0("Exited %s: error (0x%08x) at line %d, file %s\n", __FUNCTION__, err, __LINE__, __FILE__); \
   }

#define EL_SIP_SERVER_ASSERT(err) \
   DBG_ASSERT2((err == EL_OK),("Assertion error 0x%08x at line %d, file %s\n", err, __LINE__, __FILE__) );

#define PRINTERROR(s) \
DBG_L0("\n%s: error %d in at line %d, file %s\n", s, errno, __LINE__, __FILE__)


#define SOCKET  Int

#ifndef SOCKADDR_IN
#define SOCKADDR_IN struct sockaddr_in
#endif

typedef struct elSipServerParam_
{
    Char   *sipReceiverName;
    Char   *sipReceiverShortName;
    Char   *sipSenderName;
    Char   *sipSenderShortName;
    Char   *sipBranchName;
    UInt32  tagRcvId;
    UInt32  tagSndId;
    UInt32  callId;
    UInt32  cseqNumber;
    UInt32  sipLength;
    UInt8  *sdpData;

} elSipServerParam_t, *elSipServerParam_p;



int elSipGets (SOCKET remoteSocket, char *buf, int len, int echo);

int elSipPuts (SOCKET remoteSocket, Int len, Char *buf);

Int elSipServerReply ( 
    elSipServerParam_p pPar,
    SOCKET remoteSocket, 
    Char *sipInBuffer, 
    Char *sipOutBuffer);

#endif /* EL_SIPUA_SERVER_PRIVATE_H */
