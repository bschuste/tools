/*!
 ***********************************************************************
 **
   \file 
       elSipUa.h
   \brief
       Sip User Agent API
   \author
       Brigitte Schuster
   \date
       Last update: 2010/12/19
   \todo
 ***********************************************************************/
#ifndef EL_SIP_UA_H
#define EL_SIP_UA_H

#ifdef __TCS__
#include <sockets.h>
#include <mmio.h>
#include <custom_defs.h>
#include <tmosal.h>
#include <tmDbg.h>
#else

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

#endif


//! Base error number.
#define CID_COMP_SIP_SERVER        0xC0E80000
#define EL_SIP_SERVER_ERR_BASE    (CID_COMP_SIP_SERVER)

//! Base unique error number.
#define EL_SIP_SERVER_UNIQUE_BASE                                                \
        (EL_SIP_SERVER_ERR_BASE | EL_ERR_COMP_UNIQUE_START)                      \

//! Error: the given parameter is not valid.
#define EL_ERR_SIP_BAD_PARAMETER     (EL_SIP_SERVER_ERR_BASE | EL_ERR_BAD_PARAMETER)
//! Client socket is either already closed or does not exist
#define EL_ERR_SIP_CLIENT_SOCKET     (EL_SIP_SERVER_UNIQUE_BASE | 0x01)

#define EL_SIP_SERVER_PORT            5060

#define SIP_INVITE    "INVITE %s SIP/2.0\r\n" /* %s is sip:user:passwd@ipaddress:port" */
#define SIP_APP_STR   "content-type: application/sdp\r\n"
#define SIP_APP_LEN   "content-length: %d\r\n"
#define SIP_VIA       "Via: SIP/2.0/UDP %s;\r\n"

#define SIP_ACK       "CSeq: %d INVITE %s SIP/2.0\r\n"
#define SIP_OPTIONS   "CSeq: %d OPTIONS %s SIP/2.0\r\n"
#define SIP_BYE       "CSeq: %d BYE %s SIP/2.0\r\n"
#define SIP_REGISTER  "CSeq: %d REGISTER %s SIP/2.0\r\n"
#define SIP_CANCEL    "CSeq: %d CANCEL %s SIP/2.0\r\n"

#define SIP_ERR_INFO       100
#define SIP_ERR_OK         200
#define SIP_ERR_REDIREC    300
#define SIP_ERR_CLIENT     400
#define SIP_ERR_SERVER     500
#define SIP_ERR_GENERAL    600


#define SIP_REPLY_TRYING             "SIP/2.0 100 Trying\r\n"
#define SIP_REPLY_RINGING            "SIP/2.0 180 Ringing\r\n"
#define SIP_REPLY_SUCCESS            "SIP/2.0 200 OK\r\n"
#define SIP_REPLY_UNAUTHORIZED       "SIP/2.0 401 Unauthorized\n\n"
#define SIP_REPLY_NOT_FOUND          "SIP/2.0 404 Not found\n\n"
#define SIP_REPLY_INTERNAL_ERROR     "SIP/2.0 500 Server Internal Error\n\n"
#define SIP_REPLY_NOT_IMPLEMENTED    "SIP/2.0 501 Not Implemented\n\n"
#define SIP_REPLY_BUSY               "SIP/2.0 600 Busy Everywhere\n\n"
#define SIP_REPLY_DECLINE            "SIP/2.0 603 Decline\n\n"


//Via PROXY
#if 0
/* Sample of SIP REQUEST

INVITE sip:bob@biloxi.com SIP/2.0
Via: SIP/2.0/UDP pc33.atlanta.com;
branch=z9hG4bK776asdhds
Max-Forwards: 70
To: Bob <sip:bob@biloxi.com>
From: Alice <sip:alice@atlanta.com>;
tag=1928301774
Call-ID: a84b4c76e66710@pc33.atlanta.com
CSeq: 314159 INVITE
Contact: <sip:alice@pc33.atlanta.com>
Content-Type: application/sdp
Content-Length: 142
<DATA BLOB>
*/
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
/* %s is sip:user:passwd@ipaddress:port" */
#define SIP_INVITE    "INVITE %s SIP/2.0\r\n"
#define SIP_BRANCH    "branch=%s;\r\n"
#define SIP_APP_STR   "content-type: application/sdp\r\n"
#define SIP_APP_LEN   "content-length: %d\r\n"

/* Sample of SIP RESPONSE

SIP/2.0 200 OK
Via: SIP/2.0/UDP server10.biloxi.com
;branch=z9hG4bKnashds8;received=192.0.2.3
Via: SIP/2.0/UDP bigbox3.site3.atlanta.com
;branch=z9hG4bK77ef4c2312983.1;received=192.0.2.2
Via: SIP/2.0/UDP pc33.atlanta.com
;branch=z9hG4bK776asdhds ;received=192.0.2.1
To: Bob <sip:bob@biloxi.com>;tag=a6c85cf
From: Alice <sip:alice@atlanta.com>;tag=1928301774
Call-ID: a84b4c76e66710@pc33.atlanta.com
CSeq: 314159 INVITE
Contact: <sip:bob@192.0.2.4>
Content-Type: application/sdp
Content-Length: 131
<DATA BLOB>
*/
#if 0
sprintf (
    sipOutBuffer,
    SIP_REPLY_SUCCESS,
    "Via: SIP/2.0/UDP %s;\r\n"
    "branch=%s;\r\n"
    "To: %s <sip:%s>;\r\n"
    "tag=%d\r\n"
    "From: %s <sip:%s>;\r\n"
    "tag=%d\r\n"
    "Call-ID: %s\r\n"
    "CSeq: %d INVITE\r\n"
    "Contact: <sip:%s>\r\n"
    "Content-Type: application/sdp\r\n"
    "Content-Length: %d\r\n"
    "%s",
    sipReceiverName,
    sipBranchName,
    sipReceiverShortName, sipReceiverName,
    tagRcvId,
    sipSenderShortName, sipSenderName,
    tagSndId,
    callId,
    cseqNumber,
    sipSenderName,
    sipLength,
    sdpData
);
#endif

#define SIP_SERVER_NAME              "ELECTROPIX.SIP SERVER/1.0"
#define SIP_BUFFER_SIZE              4096


typedef struct elSipServerHtmlHook
{ 
   Char            *htmlName;              //pointer to the name of the html page ex: "index.htm"
   elErrorCode_t  (*htmlFunc)(Void *arg);  //custom function to call to serve the predefined page 
   pVoid            htmlArg;               //function argument
} elSipServerHtmlHook_t, * elSipServerHtmlHook_p;

/* 
 * The following structure defines the argument to pass
 * the httpNotFound function. Indeed, in the case the Http server
 * cannot find the page in the http pages table, if entry "/"
 * exists in the http table, the related function is called 
 * with the following argument type...Usually this functionality 
 * is interesting because the page could be found on a storage device 
 * like a hard disk.
 */
typedef struct elSipServerDisk 
{
   pVoid           funcArg;  //holds the application argument
   Char           *capbuf;   //holds the page name
} elSipServerDisk_t, *elSipServerDisk_p;



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
    ptmSWVersion_t    pHttpVersion  //  O: pointer to structure to receive
                                    //     version information.
);
#endif

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerSend:
//
// DESCRIPTION: This function requests the server to send the data.  
//
// RETURN:      EL_OK
//
//-----------------------------------------------------------------------------
//
elErrorCode_t 
elSipServerSend(
    Char                 *psBuf,  //for ascii content, end of buffer is \0
    Int                   len     //for binary data, indicate amount of bytes
                                  //if ascii set len to 0
);

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerInit:
//
// DESCRIPTION: This function initializes the Http Server.  
//
// RETURN:      EL_OK
//
// NOTES:       It starts a task with the given priority
//
//-----------------------------------------------------------------------------
//
elErrorCode_t 
elSipServerInit(
    Int                    taskPriority
);

//-----------------------------------------------------------------------------
// FUNCTION:    elSipServerDeinit:
//
// DESCRIPTION: This function de-initializes the Sip UA Server.  
//
// RETURN:      EL_OK
//
// NOTES:       
//-----------------------------------------------------------------------------
//
elErrorCode_t elSipServerDeinit(
    Void
);



#endif /* EL_SIP_UA_H */
