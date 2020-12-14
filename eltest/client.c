////////////////////////////////////////////////////////////////////////
// Copyright (c) Medialive 2002. 							             
// All rights reserved. Reproduction in whole or in part is prohibited   
// without the written consent of the copyright holder.                  
//                                                                       
// This software has been developed by Tigress under contract    
//  n° 2002-07-15.                     			
// This code and information is provided "as is" without any warranty    
// of any kind, either expressed or implied, including but not limited   
// to the implied warranties of merchantability and/or fitness for any   
// particular purpose.                                                   
//                                                                       
// Author: Brigitte Schuster                                 
// Creation: 2002/07/18                                                  
// File: client.c
// History:
// 		Version 0.10	    2002/07/18      Brigitte Schuster
//
// 		Version 0.20	    2002/07/30      Brigitte Schuster
//                          Complete rcvMessage and sendMessage definition
//                          Add User Interface socket management
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Include Files
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#ifndef __TCS__
#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#endif

#include "defines.h"
#include "externs.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
// Global Variables
//
extern UInt32 dbgLevel;


////////////////////////////////////////////////////////////////////////
// Name    		clientDataConnect
// Abstract:	This function connects to a server in TCP mode
// 				
// In:      	Session structure.
//				IP address or host name
// Out:     	Session structure.
//				socket pointer
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/18
////////////////////////////////////////////////////////////////////////
int clientDataConnect(session_p fm_p) {
    
    int                    sockfd;
//    struct sockaddr_in     servAddr;
    serviceManager_p       serv_p;
    mediaError             err = MEDIA_OK;
        
    serv_p = fm_p->serv_p;
 
	DB_L10(" Open a socket\n");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return(MEDIA_ERR_SOCKET);
    }

    // Connect to the server  
	DB_L10(" Connect to the Control data file server\n");
    if (connect(sockfd, (struct sockaddr *)serv_p->controlServAddr_p, sizeof(struct sockaddr_in)) < 0) {
        perror("connect()");
        return(MEDIA_WRN_DATA_FILE);
    }
	
    serv_p->controlSocket     = sockfd;
	DB_L10(" Control data file connection successful\n");
    
 
	DB_L10(" Open a socket\n");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return(MEDIA_ERR_SOCKET);
    }
    
#ifdef __TCS__
    // Non-blocking socket
    fcntl(sockfd,F_SETFL,O_NONBLOCK);
#endif

    // Connect to the server  
	DB_L10(" Connect to the Data file server\n");
    if (connect(sockfd, (struct sockaddr *)serv_p->dataServAddr_p, sizeof(struct sockaddr_in)) < 0) {
	    serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE;
        perror("connect()");
        return(MEDIA_WRN_DATA_FILE);
    }
	
	if (serv_p->statusDataFile != DATA_FILE_EOS) serv_p->statusDataFile = DATA_FILE_OK;
    serv_p->dataSocket = sockfd;
	DB_L10(" Data file connection successful\n");
	DB_L10(" clientDataConnect::THE NETWORK IS CONNECTED\n");
     
    return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		clientMediaConnect
// Abstract:	This function connects to a server in TCP mode
// 				
// In:      	Session structure.
//				IP address or host name
// Out:     	Session structure.
//				socket pointer
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/18
////////////////////////////////////////////////////////////////////////
Int16 clientMediaConnect(session_p fm_p) {
    
    Int16                  sockfd;
    serviceManager_p       serv_p;
    mediaError             err = MEDIA_OK;
        
    serv_p = fm_p->serv_p;
    
	DB_L10(" Open a socket\n");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return(MEDIA_ERR_SOCKET);
    }

    // Connect to the server  
	DB_L10(" Connect to the Media server\n");
    if (connect(sockfd, (struct sockaddr *)serv_p->mediaServAddr_p , sizeof(struct sockaddr_in)) < 0) {
        perror("connect()");
        return(MEDIA_ERR_CONNECT);
    }
	DB_L10(" Connection successful\n");
    serv_p->mediaSocket      = sockfd;
        
    return(err);
}


////////////////////////////////////////////////////////////////////////
// Name    		dataSendMessage
// Abstract:	If the TCP control socket is open, the client sends
// 				messages to the server
// In:      	Session structure.
//				control socket
// Out:     	Session structure.
//				status
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/18
////////////////////////////////////////////////////////////////////////
int dataSendMessage(session_p fm_p, messageType msgt, UInt8 *sndbuf_p, Int16 socket) {
    
    mediaError err = MEDIA_OK;
    Int16 nbytes;
        

    nbytes = 0;
   *sndbuf_p = 0xBE;
    nbytes +=1;
   *(sndbuf_p + nbytes) = 0xEF;
    nbytes +=1;
    
    switch (msgt) {
		case MSG_STATUS :   
		  *(sndbuf_p + nbytes) = 5; // length field
            nbytes +=1;
          *(sndbuf_p + nbytes) = MSG_STATUS; // message type
            nbytes +=1;
		  *(sndbuf_p + nbytes) = (fm_p->pic_p->xc>>24); // numero d'image intra courante MSB
            nbytes +=1;
		  *(sndbuf_p + nbytes) = (fm_p->pic_p->xc>>16) & 0x000000FF; // numero d'image intra courante
            nbytes +=1;
		  *(sndbuf_p + nbytes) = (fm_p->pic_p->xc>>8) & 0x000000FF; // numero d'image intra courante
            nbytes +=1;
		  *(sndbuf_p + nbytes) = fm_p->pic_p->xc & 0x000000FF; // numero d'image intra courante LSB
            nbytes +=1;
            DB_L10(" dataSendMessage:: sending MSG_STATUS\n");
			break;
	    default:             
	        err = MEDIA_ERR_MSG_TYPE;
	        return(err);
    }

    DB_L1(" dataSendMessage:: sending %d bytes on socket\n",nbytes);

    // Send a string from the client to the server.
    if (send( socket, sndbuf_p, nbytes , 0) < 0 ) 
    {
        printf ("Sending message to the server failed.");
        close(socket);	
	    err = MEDIA_ERR_SEND;
    }
    return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		rcvMessage
// Abstract:	If the TCP control socket is open, the client receives
// 				messages from the server
// In:      	Session structure.
//				control socket
// Out:     	Session structure.
//				error code
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/18
////////////////////////////////////////////////////////////////////////
int rcvMessage(session_p fm_p, UInt8 *rcvbuf_p, Int16 socket) {
    
    mediaError err = MEDIA_OK;
    Int16 nbytes, i;
    UInt8 *p;
    Int16 msgLength = 0;
    messageType msgt;
    p = rcvbuf_p;
    
	if ( (nbytes = recv(socket, rcvbuf_p, SMALL_BUFFER_SIZE*sizeof(UInt8), 0) <= 4) )
	{
        perror("rcvMessage()");
        return(MEDIA_ERR_RECV);
	}
    DB_L0(" rcvMessage:: received, nbytes = %d bytes on socket\n",nbytes);

    // Analyze buffer received
    if (*p == 0xBE) {
        p++;
	    if (*p == 0xEF) {
            p++;
	        msgLength = *p;
            DB_L0(" rcvMessage:: msgLength = %d\n",msgLength);
            p++;
	        msgt = (messageType)*p;
		
            switch (msgt) {
	            case MSG_UI_START : 
		            DB_L00(" rcvMessage:: MSG_START\n");
			        if (msgLength > 1) {
					    err = MEDIA_ERR_MSG_LENGTH;
			     	    break;
			        }
			     	break;
	            case MSG_STATUS: 
		            DB_L00(" rcvMessage:: MSG_STATUS\n");
			        if (msgLength > 1) {
					    err = MEDIA_ERR_MSG_LENGTH;
			     	    break;
			        }
			     	break;
	            case MSG_UI_CATEGORY: 
		            DB_L00(" rcvMessage:: MSG_CATEGORY\n");
			        if (msgLength > 1) {
					    err = MEDIA_ERR_MSG_LENGTH;
			     	    break;
			        }
			     	break;
	            case MSG_UI_RIGHTS: 
		            DB_L00(" rcvMessage:: MSG_RIGHTS\n");
			        if (msgLength > 1) {
					    err = MEDIA_ERR_MSG_LENGTH;
			     	    break;
			        }
				    
		            DB_L0(" rcvMessage::  = %d\n",msgt);
			     	break;
	            case MSG_UI_FILE_NAME: 
		            DB_L00(" rcvMessage:: MSG_UI_FILE_NAME\n"); 
			        if (msgLength > 255) {
					    err = MEDIA_ERR_MSG_LENGTH;
			     	    break;
			        }
        			for (i=0;i<msgLength-1;i++) {
                        p++;
        	        }
			     	break;
	            default:     
		            DB_L0(" rcvMessage:: unknown message type = %d\n",msgt);
		            err = MEDIA_ERR_MSG_TYPE;
	                return(err);
            }
        } else err = MEDIA_ERR_MSG_RCV;
    } else err = MEDIA_ERR_MSG_RCV;
	return(err);
}
