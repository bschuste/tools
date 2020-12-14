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
// Creation: 2002/08/02                                                  
// Modified: Brigitte Schuster                                                  
// File: tsmanager.c
// History:
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "error.h"


////////////////////////////////////////////////////////////////////////
// Global Variables
//
extern UInt32 dbgLevel;


////////////////////////////////////////////////////////////////////////
// Name    		tsPrint
// Abstract:	Print the list of header signal
// In:      	Buffer structure
// Out:     	Buffer structure, position bc and max pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/08/07
////////////////////////////////////////////////////////////////////////
mediaError tsPrint(fileManager_p buf_p) {
  
  	mediaError   err = MEDIA_OK;
    tsHeader_p   nexthdr_p;
    tsHeader_p   tshdr_p;
    session_p    fm_p;     				    
    
    tshdr_p   = buf_p->tshdr_p;
    nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
	fm_p = (session_p)buf_p->fm_p;
    
    // Print memory until the last TS header of the list
    if (buf_p == &fm_p->dataFm) DB_L00("tsPrint::dataFm\n"); 
    if (buf_p == &fm_p->outFm)  DB_L00("tsPrint::outFm\n"); 
    if (buf_p == &fm_p->inFm)   DB_L00("tsPrint::inFm\n");
    while (nexthdr_p != buf_p->tshdr_p) {
        DB_L0("tsPrint::tshdr_p 0x%lx",(UInt32)(tshdr_p));
        DB_L0(" nexthdr_p 0x%lx\n",(UInt32)(nexthdr_p));
        DB_L0("tsPrint::tshdr_p->rdSignal %d\n",tshdr_p->rdSignal);
        tshdr_p   = (tsHeader_p)nexthdr_p;
        nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
    }
    DB_L0("tsPrint::tshdr_p 0x%lx",(UInt32)(tshdr_p));
    DB_L0(" nexthdr_p 0x%lx\n",(UInt32)(nexthdr_p));
    DB_L0("tsPrint::tshdr_p->rdSignal %d\n",tshdr_p->rdSignal);

	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsAdd
// Abstract:	allocate memory, initialize a tsHeader structure and
//              add the tsHeader to the file manager list of headers
// In:      	Buffer structure
// Out:     	Buffer structure, position bc and max pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/08/07
////////////////////////////////////////////////////////////////////////
mediaError tsAdd(fileManager_p buf_p) {
  
  	mediaError   err = MEDIA_OK;
    tsHeader_p   nexthdr_p;
    tsHeader_p   tshdr_p;
    
    tshdr_p   = buf_p->tshdr_p;
    nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
    
    // Find the last TS header of the circular list
    while (nexthdr_p != buf_p->tshdr_p) {
        tshdr_p   = nexthdr_p;
        nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
    }
    
    nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
    // allocate structure memory
    if ((nexthdr_p = (tsHeader_p) malloc(sizeof(tsHeader_t))) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	memset(nexthdr_p,sizeof(tsHeader_t),0);
	
	// allocate header memory
    if ((nexthdr_p->hdr_p = (UInt8*) malloc(sizeof(UInt8)*TS_PACKET_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);

    // Initialize the next header pointer
    nexthdr_p->nexthdr_p = buf_p->tshdr_p;
    
    // Initialize the header id
//    tshdr_p->nexthdr_p->id = tshdr_p->id+1; 
    
    // Initialize the Signal
    nexthdr_p->rdSignal = 0; 

    // Initialize the header content
	nexthdr_p->hdr_p[0] = SYNC_BYTE;
	nexthdr_p->hdr_p[1] = 0x00;
	nexthdr_p->hdr_p[2] = tshdr_p->hdr_p[2];
	nexthdr_p->hdr_p[3] = (tshdr_p->hdr_p[3])%16;
	
    tshdr_p->nexthdr_p = (void *)nexthdr_p;
    
    // Update the calling structure to point to the new header
    buf_p->tshdr_p = nexthdr_p;
        
	return(err);
}



////////////////////////////////////////////////////////////////////////
// Name    		tsListOpen
// Abstract:	allocate memory, initialize the first tsHeader structure 
//              add the tsHeader to the file manager list of headers
// In:      	session structure
// Out:     	session structure, list of headers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/09/23
////////////////////////////////////////////////////////////////////////
mediaError tsListOpen(session_p fm_p) {
  
  	mediaError   err = MEDIA_OK;
    tsHeader_p   tshdr_p;
    Int16        i;
    
    // allocate structure memory
    if ((tshdr_p = (tsHeader_p) malloc(sizeof(tsHeader_t))) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	memset(tshdr_p,sizeof(tsHeader_t),0);
	
	// allocate header memory
    if ((tshdr_p->hdr_p = (UInt8*) malloc(sizeof(UInt8)*TS_PACKET_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
    
    // Initialize the Signal
    tshdr_p->rdSignal = 0; 
    
    // Initialize the header id
   // tshdr_p->id = 1; 

    // Initialize the header content
	tshdr_p->hdr_p[0] = SYNC_BYTE;
	tshdr_p->hdr_p[1] = 0x00;
	tshdr_p->hdr_p[2] = 0xA1;
	tshdr_p->hdr_p[3] = 0x10;

    fm_p->inFm.tshdr_p  = tshdr_p;
    fm_p->outFm.tshdr_p = tshdr_p;

    // Initialize the next header pointer
    tshdr_p->nexthdr_p = (void *)fm_p->inFm.tshdr_p;;

    // Add two headers to the list. The input buffer needs the TS headers first
//    for (i=0; i< ( (PICTURE_SIZE*(fm_p->dafh_p->D + 2)/(TS_PACKET_SIZE-4)) ); i++) {
    for (i=0;i<(PICTURE_SIZE/(TS_PACKET_SIZE-4));i++) {
        tsAdd(&fm_p->inFm);
    }
    
	// Point to the same headers upon init
	fm_p->outFm.tshdr_p = fm_p->inFm.tshdr_p;
	
	// Print the list
//    tsPrint(&fm_p->inFm);
//    tsPrint(&fm_p->outFm);

	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsClose
// Abstract:	free all allocated memory in the ts list
// In:      	Buffer structure
// Out:     	Buffer structure, position bc and max pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/08/07
////////////////////////////////////////////////////////////////////////
mediaError tsClose(session_p fm_p) {
  
  	mediaError   err = MEDIA_OK;
    tsHeader_p   nexthdr_p;
    tsHeader_p   tshdr_p;
    
    tshdr_p   = fm_p->inFm.tshdr_p;
    nexthdr_p = tshdr_p->nexthdr_p;
    
    // Free memory until the last TS header of the list
    while (nexthdr_p != fm_p->inFm.tshdr_p) {
        tshdr_p   = (tsHeader_p)nexthdr_p;
        nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
        free(tshdr_p->hdr_p);
        free(tshdr_p);
    }

	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsInitInputBuffer
// Abstract:	initialize all buffer pointers during a session
// In:      	File Manager structure
// Out:     	File Manager structure with correct pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/10/04
////////////////////////////////////////////////////////////////////////
mediaError tsInitInputBuffer(fileManager_p buf_p){

  	mediaError err = MEDIA_OK;
	session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;
	buf_p->bc      = 0;
	buf_p->tsbc    = 0;
  	buf_p->max     = 0;	                // the input buffer is empty when bc == max
  	buf_p->tsmax   = 2*BUFFER_SIZE;	    // the input buffer is empty when tsbc == tswr
	buf_p->toggle  = 0;
	buf_p->tswr    = 0;
	
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsResetInputBuffer
// Abstract:	force read pointers value
// In:      	File Manager structure
// Out:     	File Manager structure with correct pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/10/04
////////////////////////////////////////////////////////////////////////
mediaError tsResetInputBuffer(fileManager_p buf_p){

  	mediaError err = MEDIA_OK;
	session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;
	buf_p->bc      = buf_p->tswr;
	buf_p->tsbc    = buf_p->tswr;
  	buf_p->max     = buf_p->tswr;
	
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		getStreamPosition
// Abstract:	compute the current position in bytes in the stream.
// In:      	File Manager structure
// Out:     	position in bytes since the beginning of the stream
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/08
////////////////////////////////////////////////////////////////////////
mediaError getStreamPosition(fileManager_p buf_p, streamPosition_p pos_p){

  	mediaError err = MEDIA_OK;
	session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;
    pos_p->bc   = buf_p->bc;
    pos_p->max  = buf_p->max;
    pos_p->tsbc = buf_p->tsbc;
	DB_L1(" getStreamPosition::buffer status bc    %ld\n",buf_p->bc);
	DB_L1(" getStreamPosition::buffer status max   %ld\n",buf_p->max);
	DB_L1(" getStreamPosition::buffer status tswr  %ld\n",buf_p->tswr);
	DB_L1(" getStreamPosition::buffer status tsbc  %ld\n",buf_p->tsbc);
	DB_L1(" getStreamPosition::buffer status tsmax %ld\n",buf_p->tsmax);
	return(err);
	
}

////////////////////////////////////////////////////////////////////////
// Name    		setStreamPosition
// Abstract:	set the stream position given the number of bytes
//              in the stream since the beginning
// In:      	File Manager structure
//				position in bytes since the beginning of the stream
// Out:     	error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/08
////////////////////////////////////////////////////////////////////////
mediaError setStreamPosition(fileManager_p buf_p, streamPosition_p pos_p){

  	mediaError err = MEDIA_OK;
    session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;
    buf_p->bc   = pos_p->bc;
	if ( (fm_p->serv_p->serviceType != TCP_TYPE)||(buf_p->type == TS_TYPE) )
        buf_p->max  = pos_p->max;
    buf_p->tsbc = pos_p->tsbc;
    DB_L10(" \n");
   	DB_L1(" setStreamPosition::buffer status bc    %ld\n",buf_p->bc);
	DB_L1(" setStreamPosition::buffer status max   %ld\n",buf_p->max);
	DB_L1(" setStreamPosition::buffer status tswr  %ld\n",buf_p->tswr);
	DB_L1(" setStreamPosition::buffer status tsbc  %ld\n",buf_p->tsbc);
	DB_L1(" setStreamPosition::buffer status tsmax %ld\n",buf_p->tsmax);
	
	return(err);
	
}

////////////////////////////////////////////////////////////////////////
// Name    		getPacket
// Abstract:	get a packet from a circular buffer.
//              two functional modes:
//              - transparent, the packet size == payload size
//              - packet, where TS packets are parsed 
//                the packet size > payload size
//              reads from a file or from the network through TCP
// In:      	Buffer structure
//              flag, if flag == 1, force the load of the input buffer
//              update only the write pointer
//              if flag == 0, load and update payload pointes
// Out:     	Buffer structure
//              Pointer to the first byte of the TS packet in the buffer
//              bc, max, tsbc, tswr of the buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/09/25
////////////////////////////////////////////////////////////////////////
mediaError getPacket(fileManager_p buf_p,  UInt8 flag){
  
  	mediaError     err = MEDIA_OK;
	session_p      fm_p;
	Int32          nbytes = 0;
	Int32          bufSize;
	UInt8          p[BUFFER_SIZE];
#ifndef __TCS__
    fd_set         sockets;
    struct timeval timeout;
#endif
              	
	fm_p = (session_p)buf_p->fm_p;
	if (buf_p->type == VIDEO_TYPE)
  	    nbytes = BUFFER_SIZE; // default size of the packet
    else
  	    nbytes = TS_PACKET_SIZE; // default size of the packet
	
	
	/* Fill in the buffer and update the write pointer */
#ifndef __TCS__
	if (buf_p == &fm_p->dataFm) {
	    if (buf_p->bc == buf_p->max) { // the buffer is empty
	        if (buf_p->max != buf_p->tswr) {
	            DB_L00(" getPacket::data buffer ERROR buf_p->max != buf_p->tswr\n");	
	            DB_L0(" getPacket::data buffer ERROR, buf_p->tswr  = %ld\n",buf_p->tswr);	
   			    DB_L0(" getPacket::data buffer ERROR, buf_p->tsmax = %ld\n",buf_p->tsmax);
   			    DB_L0(" getPacket::data buffer ERROR, buf_p->bc    = %ld\n",buf_p->bc);
   			    DB_L0(" getPacket::data buffer ERROR, buf_p->max   = %ld\n",buf_p->max);
		        return(MEDIA_ERR_TS_ERROR); 
			}          
	        if (fm_p->serv_p->controlLevel > NO_CONTROL) { 
	            DB_L1(" getPacket::data buffer empty, buf_p->tswr  = %ld\n",buf_p->tswr);	
   			    DB_L1(" getPacket::data buffer empty, buf_p->tsmax = %ld\n",buf_p->tsmax);
   			    DB_L1(" getPacket::data buffer empty, buf_p->bc    = %ld\n",buf_p->bc);
   			    DB_L1(" getPacket::data buffer empty, buf_p->max   = %ld\n",buf_p->max);
                FD_ZERO( &sockets );
                FD_SET( fm_p->serv_p->dataSocket, &sockets );   /* socket we listen for clients */
                timeout.tv_usec = 0; 
#ifdef linux
                timeout.tv_sec  = 1; /* wait for 1s */
#else
                timeout.tv_sec  = 5; /* wait for 5s */
#endif

                if (select( fm_p->serv_p->dataSocket + 1, &sockets, NULL, NULL, &timeout ) < 0 ){
                    perror(" getPacket::select()");
                }
                if (FD_ISSET( fm_p->serv_p->dataSocket, &sockets ) ) {  // if received packets
			        if ((buf_p->tsmax - buf_p->tswr) >= BUFFER_SIZE ) {
		                nbytes = recv(fm_p->serv_p->dataSocket, (buf_p->buf + buf_p->tswr), BUFFER_SIZE*sizeof(UInt8), 0);
    			        // Save the stream locally for debug purposes	
// 		                fwrite((buf_p->buf + buf_p->tswr), nbytes, sizeof(UInt8), fm_p->dataFm.fiP);
   		                DB_L1(" getPacket:: received totally, nbytes = %ld bytes on datasocket \n", nbytes);
		                if (nbytes <= 0) {			
   		                    DB_L1(" getPacket::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		                return(MEDIA_EOF);
			            }
			        }
		            else {
		                nbytes = recv(fm_p->serv_p->dataSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
    			        // Save the stream locally for debug purposes	
//	    	            fwrite(&p[0], nbytes, sizeof(UInt8), fm_p->dataFm.fiP);
   		                DB_L1(" getPacket:: received totally, nbytes = %ld bytes on datasocket \n", nbytes);
		                if (nbytes <= 0) {			
   		                    DB_L1(" getPacket::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		                return(MEDIA_EOF);
			            }
			            // Copy into the input buffer the right amount of bytes
			            if (nbytes > (buf_p->tsmax - buf_p->tswr)) {
			                memcpy((buf_p->buf + buf_p->tswr),&p[0],(buf_p->tsmax - buf_p->tswr));
			                memcpy(buf_p->buf,&p[(buf_p->tsmax - buf_p->tswr)],nbytes-(buf_p->tsmax - buf_p->tswr));
				        } 
				        else {
			                memcpy((buf_p->buf + buf_p->tswr),&p[0],nbytes);
				        }
			        }
    		        // Circular buffer
			        if ( (buf_p->tswr + nbytes) >= buf_p->tsmax ) {
			            if (buf_p->toggle == 0) buf_p->toggle = 1;
			            else if (buf_p->toggle == 1) buf_p->toggle = 0;
			            buf_p->tswr = (buf_p->tswr + nbytes) - buf_p->tsmax;
	                    DB_L1(" getPacket::Circular buffer, buf_p->toggle = %d\n",buf_p->toggle);	
   				        DB_L1(" getPacket::Circular buffer, nbytes = %ld\n",nbytes);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tswr  = %ld\n",buf_p->tswr);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tsmax = %ld\n",buf_p->tsmax);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tsbc  = %ld\n",buf_p->tsbc);
   				        DB_L1(" getPacket::Circular buffer, buf_p->bc    = %ld\n",buf_p->bc);
   				        DB_L1(" getPacket::Circular buffer, buf_p->max   = %ld\n",buf_p->max);
		            }
			        else {
			            buf_p->tswr += nbytes;
                    }
                }
                else { // the network is disconnected
			        DB_L00(" getPacket::THE NETWORK IS DISCONNECTED\n");
      		        return(MEDIA_WRN_DATA_FILE);
                }
            } // > NO_CONTROL
	        else 
#endif
            {
	            // get the packet from file
                if ((feof(buf_p->fiP)==0)) {
                    // Read a new buffer from the file
		            nbytes = buf_p->tsmax - buf_p->tswr; 		        
			        if ( nbytes >= BUFFER_SIZE ) {
			            nbytes = fread((buf_p->buf + buf_p->tswr),1,BUFFER_SIZE*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != BUFFER_SIZE*sizeof(UInt8)) {			
   				            DB_L1(" getPacket::End of File detected, nbytes = %ld\n",nbytes);
			            }
      		        }
		            else {
			            nbytes = fread((buf_p->buf + buf_p->tswr),1,nbytes*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != (buf_p->tsmax - buf_p->tswr)*sizeof(UInt8)) {			
   				            DB_L1(" getPacket::End of File detected, nbytes = %ld\n",nbytes);
			            }
			        }
			        // Circular buffer
			        if ( (buf_p->tswr + nbytes) >= buf_p->tsmax ) {
			            if (buf_p->toggle == 0) buf_p->toggle = 1;
			            else if (buf_p->toggle == 1) buf_p->toggle = 0;
			            buf_p->tswr = (buf_p->tswr + nbytes) - buf_p->tsmax;
	                    DB_L1(" getPacket::Circular buffer, buf_p->toggle = %d\n",buf_p->toggle);	
   				        DB_L1(" getPacket::Circular buffer, nbytes = %ld\n",nbytes);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tswr  = %ld\n",buf_p->tswr);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tsmax = %ld\n",buf_p->tsmax);
		            }
			        else {
			            buf_p->tswr += nbytes;
		            } 
    	        } // read from file
    	        else {
		            // we reached the end of the file
   			        DB_L1(" getPacket::End of File detected, buf_p->tswr = %ld\n",buf_p->tswr);
      		        return(MEDIA_EOF);
		        }
            } // end if > NO_CONTROL
	        // To empty a buffer, bc = tswr
	        // To test a buffer is full, bc = max 	
	        buf_p->max = buf_p->tswr;
	    } // The buffer is empty
	    else {
	        DB_L00("WRONG\n");
	    }
	} // dataFm
	else { // inFm or bufNb1
	    // Includes the management of transport streams
	    if ( (buf_p == &fm_p->inFm)&&(fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p->type == VIDEO_TYPE) ) {
	        if (buf_p->bc == buf_p->max) { // the buffer is empty	
	           /* DB_L0(" getPacket::media buffer, buf_p->tswr  = %ld\n",buf_p->tswr);	
   			    DB_L0(" getPacket::media buffer, buf_p->tsmax = %ld\n",buf_p->tsmax);
   			    DB_L0(" getPacket::media buffer, buf_p->bc    = %ld\n",buf_p->bc);
   			    DB_L0(" getPacket::media buffer, buf_p->max   = %ld\n",buf_p->max);*/
	            if (buf_p->max != buf_p->tswr) {
	                DB_L00(" getPacket::media buffer ERROR buf_p->max != buf_p->tswr\n");	
	                DB_L0(" getPacket::media buffer ERROR, buf_p->tswr  = %ld\n",buf_p->tswr);	
   			        DB_L0(" getPacket::media buffer ERROR, buf_p->tsmax = %ld\n",buf_p->tsmax);
   			        DB_L0(" getPacket::media buffer ERROR, buf_p->bc    = %ld\n",buf_p->bc);
   			        DB_L0(" getPacket::media buffer ERROR, buf_p->max   = %ld\n",buf_p->max);
		            return(MEDIA_ERR_TS_ERROR); 
			    }
      		    // Read a new buffer from the media socket
			    if ( (buf_p->tsmax - buf_p->tswr) >= BUFFER_SIZE ) {
		            nbytes = recv(fm_p->serv_p->mediaSocket, (buf_p->buf + buf_p->tswr), BUFFER_SIZE*sizeof(UInt8), 0);
   		            DB_L1(" getPacket::received totally, nbytes = %ld on mediasocket\n",nbytes);
		            if (nbytes <= 0) {			
   		                DB_L1(" getPacket::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
				    if (buf_p->byteCounter == 0.0) {
			            if ((buf_p->buf + buf_p->tswr)[0] == SYNC_BYTE)
			                buf_p->type = TS_TYPE;
		    		    else
	    		            buf_p->type = VIDEO_TYPE;
    		            DB_L2(" getPacket::stream type %d\n",buf_p->type);
				    }
			        // Save the stream locally
		            fwrite((buf_p->buf + buf_p->tswr), nbytes, sizeof(UInt8), fm_p->inFm.fiP);
      		    }
		        else {
		            DB_L10(" getPacket::Should rarely happen on mediasocket\n");
		            nbytes = recv(fm_p->serv_p->mediaSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
   		            DB_L1(" getPacket::received totally, nbytes = %ld on mediasocket\n",nbytes);
				
		            if (nbytes <= 0) {			
   		                DB_L0(" getPacket::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
			        // Save the stream locally		
		            fwrite(p, nbytes, sizeof(UInt8), fm_p->inFm.fiP);
			        // Copy into the input buffer the right amount of bytes
			        if (nbytes > (buf_p->tsmax - buf_p->tswr)) {
			            memcpy((buf_p->buf + buf_p->tswr),&p[0],(buf_p->tsmax - buf_p->tswr));
			            memcpy(buf_p->buf,&p[(buf_p->tsmax - buf_p->tswr)],nbytes-(buf_p->tsmax - buf_p->tswr));
				    } 
				    else {
			            memcpy((buf_p->buf + buf_p->tswr),&p[0],nbytes);
				    }
			    }
			    // Circular buffer
			    if ( (buf_p->tswr + nbytes) >= buf_p->tsmax ) {
			        if (buf_p->toggle == 0) buf_p->toggle = 1;
			        else if (buf_p->toggle == 1) buf_p->toggle = 0;
			        buf_p->tswr = (buf_p->tswr + nbytes) - buf_p->tsmax;
		        }
			    else {
			        buf_p->tswr += nbytes;
                }
		
	            if (buf_p->type == VIDEO_TYPE) {
			    // To empty a buffer, bc = tswr
	            // To test a buffer is full, bc = max 	
		            buf_p->max = buf_p->tswr;
			    }
			    else {
			    	fm_p->seqh_p->seqFlag = TRUE;
	                fm_p->accessType = READ_WRITE;

			    	if (buf_p->bc > buf_p->tsmax) {
                        DB_L0(" getPacket::ERROR buf->bc > buf_p->tsmax %ld\n",buf_p->bc);
      	                return(MEDIA_ERR_TS_ERROR);
	                }
	                // To empty a buffer, bc = tsbc + 4
	                // To test a buffer is full, bc = max
	                buf_p->bc  = buf_p->tsbc;
	                buf_p->max = buf_p->tsbc + TS_PACKET_SIZE;	    
	                if (buf_p->max > buf_p->tsmax) {
    	                DB_L0(" getPacket::ERROR buf_p->max > buf_p->tsmax %ld\n",buf_p->max);
    	                DB_L0(" getPacket::ERROR buf_p->tsbc               %ld\n",buf_p->tsbc);
    	                DB_L0(" getPacket::ERROR nbytes                    %ld\n",nbytes);
      		            return(MEDIA_ERR_TS_ERROR);
		            } 
  	                buf_p->tsbc += TS_PACKET_SIZE; // buffer size?
	                if (buf_p->tsbc >= buf_p->tsmax) {
	                    buf_p->tsbc = 0;
                    }
			    }
	        } // test if buffer is empty
	    } // (buf_p == &fm_p->inFm)&&(fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p->type == VIDEO_TYPE)
	    else if ( (buf_p == &fm_p->inFm)&&(fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p->type == TS_TYPE) ) {
//  	        if ((buf_p->tsbc == buf_p->tswr)||(flag == 1)) { 		// Test if buffer is empty
  	        if (( abs(buf_p->tswr - buf_p->tsbc) < TS_PACKET_SIZE )||(flag == 1)) { 		// Test if buffer is empty
    	        DB_L2(" getPacket::buffer empty, buf_p->tswr = %ld\n",buf_p->tswr);	
	            DB_L2(" getPacket::flag = %d\n",flag);    
      		    // Read a new buffer from the media socket
			    if ( (buf_p->tsmax - buf_p->tswr) >= BUFFER_SIZE ) {
//VISTEST
//		            nbytes = recv(fm_p->serv_p->mediaSocket, (buf_p->buf + buf_p->tswr), BUFFER_SIZE*sizeof(UInt8), 0);
		            printf(" getPacket::VISTEST\n");
			        bufSize = (BUFFER_SIZE/2) + 5;
			        nbytes = fread((buf_p->buf + buf_p->tswr),1,bufSize*sizeof(UInt8),buf_p->fiP);
		            if (nbytes <= 0) {			
   		                DB_L1(" getPacket::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
		            printf(" getPacket::VISTEST::nbytes = %d\n",nbytes);
			        // Save the stream locally
//VISTEST
//		            fwrite((buf_p->buf + buf_p->tswr), nbytes, sizeof(UInt8), fm_p->inFm.fiP);
      		    }
		        else {
//		            DB_L10(" getPacket::Should rarely happen on mediasocket\n");
		            printf(" getPacket::VISTEST::Should rarely happen on mediasocket\n");
			        bufSize = (BUFFER_SIZE/4) + 7;
			        nbytes = fread(&p[0],1,bufSize*sizeof(UInt8),buf_p->fiP);
//		            nbytes = recv(fm_p->serv_p->mediaSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
		            if (nbytes <= 0) {			
   		                DB_L0(" getPacket::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
			        // Save the stream locally		
//VISTEST
//		            fwrite(p, nbytes, sizeof(UInt8), fm_p->inFm.fiP);
			        // Copy into the input buffer the right amount of bytes
			        if (nbytes > (buf_p->tsmax - buf_p->tswr)) {
			            memcpy((buf_p->buf + buf_p->tswr),&p[0],(buf_p->tsmax - buf_p->tswr));
			            memcpy(buf_p->buf,&p[(buf_p->tsmax - buf_p->tswr)],nbytes-(buf_p->tsmax - buf_p->tswr));
				    } 
				    else {
			            memcpy((buf_p->buf + buf_p->tswr),&p[0],nbytes);
				    }
			    }
			    // Circular buffer
			    if ( (buf_p->tswr + nbytes) >= buf_p->tsmax ) {
			        if (buf_p->toggle == 0) buf_p->toggle = 1;
			        else if (buf_p->toggle == 1) buf_p->toggle = 0;
			        buf_p->tswr = (buf_p->tswr + nbytes) - buf_p->tsmax;
		        }
			    else {
			        buf_p->tswr += nbytes;
                }
	        }// buffer is empty
		    if (flag == 0) { // Update also the other pointers
	            if (buf_p->bc > buf_p->tsmax) {
                    DB_L0(" getPacket::ERROR buf->bc > buf_p->tsmax %ld\n",buf_p->bc);
      	            return(MEDIA_ERR_TS_ERROR);
	            }
	            // To empty a buffer, bc = tsbc + 4
	            // To test a buffer is full, bc = max
	            buf_p->bc  = buf_p->tsbc;
	            buf_p->max = buf_p->tsbc + TS_PACKET_SIZE;	    
	            if (buf_p->max > buf_p->tsmax) {
    	            DB_L0(" getPacket::ERROR buf_p->max > buf_p->tsmax %ld\n",buf_p->max);
    	            DB_L0(" getPacket::ERROR buf_p->tsbc               %ld\n",buf_p->tsbc);
    	            DB_L0(" getPacket::ERROR nbytes                    %ld\n",nbytes);
      		        return(MEDIA_ERR_TS_ERROR);
		        } 
  	            buf_p->tsbc += TS_PACKET_SIZE; // buffer size?
	            if (buf_p->tsbc >= buf_p->tsmax) {
	                buf_p->tsbc = 0;
                }
            } // flag ==0
        } // (buf_p == &fm_p->inFm)&&(fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p->type == TS_TYPE)
	    else {
  	        if ((buf_p->tsbc == buf_p->tswr)||(flag == 1)) { 		// Test if buffer is empty
    	        DB_L2(" getPacket::buffer empty, buf_p->tswr = %ld\n",buf_p->tswr);	
	            DB_L2(" getPacket::flag = %d\n",flag);    
    	        if ((feof(buf_p->fiP)==0)) {
      		        // Read a new buffer from the file
		            nbytes = buf_p->tsmax - buf_p->tswr; 		        
			        if ( nbytes >= BUFFER_SIZE ) {
			            nbytes = fread((buf_p->buf + buf_p->tswr),1,BUFFER_SIZE*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != BUFFER_SIZE*sizeof(UInt8)) {			
   				            DB_L1(" getPacket::End of File detected, nbytes = %ld\n",nbytes);
			            }
      		        }
		            else {
			            nbytes = fread((buf_p->buf + buf_p->tswr),1,nbytes*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != (buf_p->tsmax - buf_p->tswr)*sizeof(UInt8)) {			
   				            DB_L1(" getPacket::End of File detected, nbytes = %ld\n",nbytes);
			            }
			        }
			        // Circular buffer
			        if ( (buf_p->tswr + nbytes) >= buf_p->tsmax ) {
			            if (buf_p->toggle == 0) buf_p->toggle = 1;
			            else if (buf_p->toggle == 1) buf_p->toggle = 0;
			            buf_p->tswr = (buf_p->tswr + nbytes) - buf_p->tsmax;
	                    DB_L1(" getPacket::Circular buffer, buf_p->toggle = %d\n",buf_p->toggle);	
   				        DB_L1(" getPacket::Circular buffer, nbytes = %ld\n",nbytes);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tswr  = %ld\n",buf_p->tswr);
   				        DB_L1(" getPacket::Circular buffer, buf_p->tsmax = %ld\n",buf_p->tsmax);
				    }
				    else {
				        buf_p->tswr += nbytes;
		            } 
    	        }
    	        else {
		            // we reached the end of the file
   				    DB_L1(" getPacket::End of File detected, buf_p->tswr = %ld\n",buf_p->tswr);
      		        return(MEDIA_EOF);
		        }
		    } // if buffer is empty
            if (flag == 0) { // Update also the other pointers

	            if (buf_p->bc > buf_p->tsmax) {
                    DB_L0(" getPacket::ERROR buf->bc > buf_p->tsmax %ld\n",buf_p->bc);
      	            return(MEDIA_ERR_TS_ERROR);
	            }

	            if (buf_p->type == VIDEO_TYPE) {
	                // To empty a buffer, bc = tsbc
	                // To test a buffer is full, bc = max
	                buf_p->bc  = buf_p->tsbc;   	
	                buf_p->max = buf_p->tsbc + nbytes;
		
    	            if (buf_p->max > buf_p->tsmax) {
	                    buf_p->max = buf_p->max - buf_p->tsmax;
	                }
	                if (buf_p->max > buf_p->tsmax) {
    	                DB_L0(" getPacket::ERROR buf_p->max > buf_p->tsmax %ld\n",buf_p->max);
    	                DB_L0(" getPacket::ERROR buf_p->tsbc               %ld\n",buf_p->tsbc);
    	                DB_L0(" getPacket::ERROR nbytes                    %ld\n",nbytes);
      		             return(MEDIA_ERR_TS_ERROR);
		            }
	    
  	                buf_p->tsbc += nbytes;
	                if (buf_p->tsbc >= buf_p->tsmax) {
	                    buf_p->tsbc = buf_p->tsbc - buf_p->tsmax;
	                }
                }
                else {	
	                // To empty a buffer, bc = tsbc + 4
	                // To test a buffer is full, bc = max
	                buf_p->bc  = buf_p->tsbc;
	                buf_p->max = buf_p->tsbc + TS_PACKET_SIZE;	    
	                DB_L2(" getPacket::buf_p->bc   = %ld\n",buf_p->bc);
	                DB_L2(" getPacket::buf_p->max  = %ld\n",buf_p->max);
	                if (buf_p->max > buf_p->tsmax) {
    	                DB_L0(" getPacket::ERROR buf_p->max > buf_p->tsmax %ld\n",buf_p->max);
    	                DB_L0(" getPacket::ERROR buf_p->tsbc               %ld\n",buf_p->tsbc);
    	                DB_L0(" getPacket::ERROR nbytes                    %ld\n",nbytes);
      		            return(MEDIA_ERR_TS_ERROR);
		            } 
  	                buf_p->tsbc += TS_PACKET_SIZE; // buffer size?
	                if (buf_p->tsbc >= buf_p->tsmax) {
	                    buf_p->tsbc = 0;
	                }
                }
            } // flag ==0
	    } // FILE_TYPE or bufNb1
	} // dataFm or inFm
    
    if (buf_p->max < 0) {
        DB_L0(" getPacket::ERROR buf_p->max<0 %ld\n",buf_p->max);
      	return(MEDIA_ERR_TS_ERROR);
	}
	if (buf_p->bc > buf_p->tsmax) {
        DB_L0(" getPacket::ERROR buf_p->bc > buf->tsmax %ld\n",buf_p->bc);
        return(MEDIA_ERR_TS_ERROR);
	}
	if (buf_p->bc < 0) {
        DB_L0(" getPacket::ERROR buf_p->bc<0 %ld\n",buf_p->bc);
        return(MEDIA_ERR_TS_ERROR);
	}
	return(err);
}


////////////////////////////////////////////////////////////////////////
// Name    		tsCopy
// Abstract:	allocate memory, initialize a tsHeader structure and
//              add the tsHeader to the file manager list of headers
// In:      	Buffer structure
// Out:     	Buffer structure, position bc and max pointers
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/09/21
////////////////////////////////////////////////////////////////////////
mediaError tsCopy(fileManager_p buf_p, UInt8 *bufts_p) {
  
  	mediaError   err = MEDIA_OK;
    session_p    fm_p;     				    
    tsHeader_p   tshdr_p;
    tsHeader_p   nexthdr_p;

    tshdr_p   = buf_p->tshdr_p;
    nexthdr_p = (tsHeader_p)tshdr_p->nexthdr_p;
	fm_p = (session_p)buf_p->fm_p;


    if (tshdr_p->rdSignal == 1) {
	    DB_L00(" tsCopy::exit()\n");
        return(MEDIA_ERR_TS_HEADER_UNAVAIL);
    }
    
    // The TS header can be used
	memcpy(tshdr_p->hdr_p, bufts_p, TS_PACKET_SIZE);
    DB_L2(" tsCopy::tshdr_p->hdr_p[0] 0x%0x",(tshdr_p->hdr_p[0]));
    DB_L2("%0x",(tshdr_p->hdr_p[1]));
    DB_L2("%0x",(tshdr_p->hdr_p[2]));
    DB_L2("%0x\n",(tshdr_p->hdr_p[3]));

    // Signal the header is now ready to be used
    tshdr_p->rdSignal = 1;
    
    // update the file manager info to point to the next available TS header structure
    if (nexthdr_p->rdSignal == 0) {
        buf_p->tshdr_p = (tsHeader_p)buf_p->tshdr_p->nexthdr_p; 
    }
    else  {
    	// Print the list
//        tsPrint(&fm_p->inFm);
//        tsPrint(&fm_p->outFm);
	    DB_L00(" tsCopy::exit()\n");
        return(MEDIA_ERR_TS_ERROR);
//        tsAdd(buf_p);
//	    DB_L00(" tsCopy::add a header structure\n");
    }
    
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsWrite
// Abstract:	write a TS packet in a file.
//              the payload is provided either through putByte() or through
//              tsRead()
//              the header is read from the input buffer
//              tsWrite maintains the tsbc pointer that points to the
//              current TS header in the input buffer
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/08/07
////////////////////////////////////////////////////////////////////////
mediaError tsWrite(fileManager_p buf_p, UInt8 * buf, Int32 nbytes, FILE *fiP, UInt8 flag){
  
  	mediaError                       err = MEDIA_OK;
static	UInt8                        bufts[TS_PACKET_SIZE];
static  UInt8 initFlag;
    session_p                        fm_p;     
  	UInt8                            startCode = FALSE;  // to TRUE when the start code is detected
	UInt16                           tmHword;
	UInt8                            endFlag = FALSE;
	UInt8                           *bufparse;
	program_association_table_p      pat_p;
	program_map_table_p              pmt_p;

	fm_p   = (session_p)buf_p->fm_p;	
	pat_p  = fm_p->pat_p;
	pmt_p  = fm_p->pmt_p;
	
	startCode = FALSE;

    if (buf_p == &fm_p->dataFm) {
	    buf_p->packetCounter++;
        DB_L1("tsWrite::PACKET NUMBER %ld",(UInt32)buf_p->packetCounter);    
        DB_L1(", flag %d",flag);
        DB_L10(", dataFm \n");
	}
    if (buf_p == &fm_p->outFm) {
	    buf_p->packetCounter++;
        DB_L1("tsWrite::PACKET NUMBER %ld",(UInt32)buf_p->packetCounter);    
        DB_L1(", flag %d",flag);
        DB_L10(", outFm  \n"); 
	}
    if (buf_p == &fm_p->inFm) {
	    fm_p->outFm.packetCounter++;
        DB_L1("tsWrite::PACKET NUMBER %ld",(UInt32)fm_p->outFm.packetCounter);    
        DB_L1(", flag %d",flag);
        DB_L10(", inFm \n");
 	}
/*	
	DB_L0("tsWrite::buf_p->outFm.bc   = %ld\n",buf_p->bc);
	DB_L0("tsWrite::buf_p->outFm.max  = %ld\n",buf_p->max);
	DB_L0("tsWrite::buf_p->outFm.tsbc = %ld\n",buf_p->tsbc);
	DB_L0("tsWrite::buf_p->outFm.tswr = %ld\n",buf_p->tswr);
	DB_L0("tsWrite::&fm_p->inFm.tswr  = %ld\n",fm_p->inFm.tswr);
	DB_L0("tsWrite::&fm_p->inFm.bc    = %ld\n",fm_p->inFm.bc);
	DB_L0("tsWrite::&fm_p->inFm.max   = %ld\n",fm_p->inFm.max);
	DB_L0("tsWrite::&fm_p->inFm.tsbc  = %ld\n",fm_p->inFm.tsbc);
*/
	// Process the TS packet header
	// bufts[0]: 0x47
	// bufts[1]: 
	//    bit7: transport_error_indicator 
	//    bit6: payload_unit_start_indicator 
	//    bit5: transport_priority
	// bufts[1-2]: 13lsbs : PID
	// bufts[3]: 
	//    bit7: \ transport_scrambling_control 
	//    bit6: /
	//    bit5:  adaptation_field_control
	//    bit4:  payload
	//    bit3-0: continuity_counter
	// bufts[4]: if AFC == 1, AF size

    if (flag == 1) {
        if ( nbytes != (TS_PACKET_SIZE-4 )) {
	        DB_L1("tsWrite::PAYLOAD SIZE = nbytes(%ld)\n",nbytes);
		}
	
	    // Write the TS header
        bufts[0] = SYNC_BYTE;
	
	    if ( (pmt_p->video_valid == TRUE)&&(initFlag==0) ) {
	        if ((pmt_p->continuity_indicator-1) == -1)
		        bufts[3] = 0;
			else 
	            bufts[3] = pmt_p->continuity_indicator-1;
//		    buf_p->tsbc = fm_p->inFm.tsbc - TS_PACKET_SIZE;
		    DB_L1("tsWrite::continuity indicator %d\n",bufts[3]);
		    initFlag = 1;
		}
	
    	// Retrieve the PID + payload_unit_start_indicator (bit6)
        bufts[1] = (fm_p->inFm.buf[buf_p->tsbc+1])&0x1F;
        bufts[2] = (fm_p->inFm.buf[buf_p->tsbc+2])&0xFF;
    	
	    // build the adaptation field and the continuity indicator
		// get the adaptation field control from input buffer
	    bufts[3] = ((fm_p->inFm.buf[buf_p->tsbc+3])&0x30) + (bufts[3]+1)%16;
	    

        // check the incoming payload (buf)
		if ((buf[0]==0) && (buf[1]==0) && (buf[2]==1)) {
 	        // Start code detected 
			if ((buf[3] >= VIDEO_STREAM_ID_MIN)&&(buf[3]<= VIDEO_STREAM_ID_MAX)) {
 			     startCode = TRUE;
                 DB_L1("tsWrite::video stream ID 0x%02x\n",buf[3]);
		    }
		}

	    if ( startCode == TRUE ) {
	        // set payload_unit_start_indicator
	        bufts[1] = bufts[1]|0x40;		
		    DB_L10("tsWrite::PAYLOAD UNIT START INDICATOR\n"); 
		}

		// Write the TS header (4 bytes)
	    fwrite(bufts,1,4*sizeof(UInt8),fiP);	    
	    
	    // If adaptation field control bit, write the adaptation field
        if ((fm_p->inFm.buf[buf_p->tsbc+3])&0x20) {
		    DB_L10("tsWrite::ADAPTATION FIELD CONTROL\n"); 
		    // get the adaptation field size
		    DB_L1("tsWrite::ADAPTATION FIELD SIZE %d\n",fm_p->inFm.buf[buf_p->tsbc+4]); 	
	        fwrite(&fm_p->inFm.buf[buf_p->tsbc+4],1,(fm_p->inFm.buf[buf_p->tsbc+4]+1)*sizeof(UInt8),fiP);
	    }
	    // Write the payload
	    fwrite(buf,1,nbytes,fiP); // nbytes should not exceed 188 bytes

 	    if ( ((fm_p->inFm.buf[buf_p->tsbc+3])&0x20)&&((nbytes + 4 + fm_p->inFm.buf[buf_p->tsbc+4] + 1) != TS_PACKET_SIZE) ) {
    	        DB_L0("tsWrite::ERROR TS PACKET SIZE = nbytes(%ld)\n",nbytes);
    	        DB_L0("tsWrite::&buf_p->tshdr_p->hdr_p[3] = 0x%lx\n",(UInt32)&fm_p->inFm.buf[buf_p->tsbc+3]);
    	        DB_L0("tsWrite::buf_p->tshdr_p->hdr_p[4] = 0x%x\n",fm_p->inFm.buf[buf_p->tsbc+4]);
    	        DB_L0("tsWrite::LINE(%d)\n",__LINE__);
		        return(MEDIA_ERR_TS_ERROR);
		}
		else if ( (!(fm_p->inFm.buf[buf_p->tsbc+3]&0x20))&&((nbytes + 4) != TS_PACKET_SIZE) ) {
    	        DB_L0("tsWrite::ERROR TS PACKET SIZE = nbytes+4(%ld)\n",nbytes+4);
    	        DB_L0("tsWrite::LINE(%d)\n",__LINE__);
        }

	    if (buf_p == &fm_p->outFm) {
            DB_L1("tsWrite::bufts[0] 0x%0x",(bufts[0]));
            DB_L1("%0x",(bufts[1]));
            DB_L1("%0x",(bufts[2]));
            DB_L1("%0x\n",(bufts[3]));
	    }
	    if (bufts[1]&0x40) {
            DB_L4("tsWrite::bufts[0] 0x%0x",(bufts[0]));
            DB_L4("%0x",(bufts[1]));
            DB_L4("%0x",(bufts[2]));
            DB_L4("%0x\n",(bufts[3]));
        }
		// Compute the next buffer size from next TS header
	    // Point to the next TS header in the input buffer. 
		// !!! tsbc in the output buffer points to the input buffer start of packet
		// Parse the header to find the next video header in the buffer
    	do { 
    	    buf_p->tsbc += TS_PACKET_SIZE;
	    	if ( buf_p->tsbc >= fm_p->inFm.tsmax ) {
		        buf_p->tsbc = 0;
			    if (buf_p->toggle == 0) buf_p->toggle = 1;
			    else if (buf_p->toggle == 1) buf_p->toggle = 0;
			}
			// if not enough TS headers in the buffer, load some more
			// but don't do it during a permutation!
			// and not when the write pointer just turn around the corner
		    if ( (buf_p->tsbc >= fm_p->inFm.tswr)&&(fm_p->inFm.suspend == FALSE)&&(fm_p->inFm.toggle == buf_p->toggle) ) {
/*                DB_L0("tsWrite::PACKET NUMBER    = %ld\n",(UInt32)buf_p->packetCounter);
                DB_L0("tsWrite::buf_p->toggle    = %ld\n",(UInt32)buf_p->toggle);
	            DB_L0("tsWrite::buf_p->bc        = %ld\n",buf_p->bc);
	            DB_L0("tsWrite::buf_p->max       = %ld\n",buf_p->max);
	            DB_L0("tsWrite::buf_p->tsbc      = %ld\n",buf_p->tsbc);
	            DB_L0("tsWrite::&fm_p->inFm.toggle = %ld\n",fm_p->inFm.toggle);
	            DB_L0("tsWrite::&fm_p->inFm.tswr = %ld\n",fm_p->inFm.tswr);
	            DB_L0("tsWrite::&fm_p->inFm.bc   = %ld\n",fm_p->inFm.bc);
	            DB_L0("tsWrite::&fm_p->inFm.max  = %ld\n",fm_p->inFm.max);
	            DB_L0("tsWrite::&fm_p->inFm.tsbc = %ld\n",fm_p->inFm.tsbc);*/
                MEDIA_ERR(err = getPacket(&fm_p->inFm,1));
		    }
//		    if ( buf_p->tsbc > fm_p->inFm.tsbc ) {
//		        DB_L0(" putByte::buf_p->tsbc %ld > fm_p->inFm.bc\n", buf_p->tsbc);
//			    return(MEDIA_ERR_TS_ERROR);
//		    }
	        bufparse = &fm_p->inFm.buf[buf_p->tsbc];
	        if (bufparse[0] != 0x47) { 
	            DB_L00("tsWrite::ERROR in TS PACKET\n");
                DB_L0("tsWrite::PACKET NUMBER %ld\n",(UInt32)buf_p->packetCounter);
	            DB_L0("tsWrite::buf_p->bc   = %ld\n",buf_p->bc);
	            DB_L0("tsWrite::buf_p->max  = %ld\n",buf_p->max);
	            DB_L0("tsWrite::buf_p->tsbc = %ld\n",buf_p->tsbc);
	            DB_L0("tsWrite::&fm_p->inFm.tswr = %ld\n",fm_p->inFm.tswr);
	            DB_L0("tsWrite::&fm_p->inFm.bc   = %ld\n",fm_p->inFm.bc);
	            DB_L0("tsWrite::&fm_p->inFm.max  = %ld\n",fm_p->inFm.max);
	            DB_L0("tsWrite::&fm_p->inFm.tsbc = %ld\n",fm_p->inFm.tsbc);
                DB_L0("tsWrite::bufparse[0] 0x%x\n",bufparse[0]);
     		   
	            if (buf_p == &fm_p->inFm) DB_L00("tsWrite::inFm\n");
                if (buf_p == &fm_p->outFm) DB_L00("tsWrite::outFm\n");
                if (buf_p == &fm_p->dataFm) DB_L00("tsWrite::dataFm\n");
		        return(MEDIA_ERR_TS_SYNC_BYTE);
		    }
	        tmHword = ( (bufparse[1]<<8) + bufparse[2])& 0x1FFF;
	        switch (tmHword) {
	            case PAT_PID:
	    	        DB_L2("tsWrite::PAT PID 0x%x\n",tmHword);
				    if (pmt_p->video_valid == TRUE) fwrite(&bufparse[0],1,TS_PACKET_SIZE,fiP); // nbytes should not exceed 188 bytes
    	            break;
	            case CAT_PID:
	            	DB_L2("tsWrite::CAT PID 0x%x\n",tmHword);
				    if (pmt_p->video_valid == TRUE) fwrite(&bufparse[0],1,TS_PACKET_SIZE,fiP); // nbytes should not exceed 188 bytes
    	            break;
	            case NULL_PID:
	    	        DB_L2("tsWrite::NULL PID 0x%x\n",tmHword);
				    if (pmt_p->video_valid == TRUE) fwrite(&bufparse[0],1,TS_PACKET_SIZE,fiP); // nbytes should not exceed 188 bytes
	                break;
    	        default: 
    	            if ( ( tmHword >= ANY_PID_MIN ) || ( tmHword <= ANY_PID_MAX ) ) {
		                if (pmt_p->video_valid == TRUE) {
			                if ( tmHword == pat_p->video_PID ) {
 		    	                endFlag = TRUE; 
                            }
		                    else {
	    	                    DB_L2("tsWrite::ANY PID 0x%x\n",tmHword);
				    		    fwrite(&bufparse[0],1,TS_PACKET_SIZE,fiP); // nbytes should not exceed 188 bytes
	    			        }
					    }
		                else {
	    	                DB_L2("tsWrite::ANY PID 0x%x\n",tmHword);
	    			    }
    		        }
		            else {
	    	            DB_L2("tsWrite::RESERVED PID 0x%x\n",tmHword);
		            }
            } // switch
    	} while (endFlag == FALSE);
		
		// Initialize the following buffer with the appropriate size
        if ((fm_p->inFm.buf[buf_p->tsbc+3])&0x20) // if adaptation field
		    buf_p->max = TS_PACKET_SIZE - 4 - fm_p->inFm.buf[buf_p->tsbc+4] - 1;
		else
		    buf_p->max = TS_PACKET_SIZE - 4;
		    
	} // flag == 1
    else {
        if (nbytes != TS_PACKET_SIZE) {
	        DB_L0("tsWrite::exit() on packet number %ld\n",(UInt32)buf_p->packetCounter);
            return(MEDIA_ERR_TS_ERROR);
        }    
	    fwrite(buf,1,nbytes,fiP); // nbytes should not exceed 188 bytes
    }
    
    
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		tsRead
// Abstract:	get a payload from a TS packet. 
// In:      	Buffer structure
// Out:     	Buffer structure, position bc and max pointers
//              Pointer to the video payload
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/09/21
////////////////////////////////////////////////////////////////////////
mediaError tsRead(fileManager_p buf_p) {
  
  	mediaError                  err = MEDIA_OK;
	Int16                       nbytes;
	session_p                   fm_p;
	UInt16                      tmHword;
	UInt8                       endFlag = FALSE;
	UInt8                      *bufts;
	program_association_table_p pat_p;
	program_map_table_p         pmt_p;
	UInt32                      len = 0;
	UInt32                      program_number  = 0;
    UInt32                      stream_type = 0;
    UInt32                      ES_info_length = 0;

	fm_p   = (session_p)buf_p->fm_p;
	pat_p  = fm_p->pat_p;
	pmt_p  = fm_p->pmt_p;
	
    nbytes = TS_PACKET_SIZE;
	do { 
	    buf_p->packetCounter++;
        DB_L1(" tsRead::PACKET NUMBER %ld\n",(UInt32)buf_p->packetCounter);

	    // Read a new TS packet from the file
        if (buf_p == &fm_p->dataFm) MEDIA_DATA_FILE(err = getPacket(buf_p,0));
        if (buf_p == &fm_p->inFm)   MEDIA_ERR(err = getPacket(buf_p,0));
	    if (buf_p->type == VIDEO_TYPE) {
            DB_L10(" tsRead::VIDEO_TYPE\n");
	        return(MEDIA_OK);
		}
/*	
		DB_L0(" tsRead::buf_p->inFm.bc   = %ld\n",buf_p->bc);
	    DB_L0(" tsRead::buf_p->inFm.max  = %ld\n",buf_p->max);
	    DB_L0(" tsRead::buf_p->inFm.tsbc = %ld\n",buf_p->tsbc);
	    DB_L0(" tsRead::buf_p->inFm.tswr = %ld\n",buf_p->tswr);
	    DB_L0(" tsRead::&fm_p->outFm.bc   = %ld\n",fm_p->outFm.bc);
	    DB_L0(" tsRead::&fm_p->outFm.max  = %ld\n",fm_p->outFm.max);
	    DB_L0(" tsRead::&fm_p->outFm.tsbc = %ld\n",fm_p->outFm.tsbc);
	    DB_L0(" tsRead::&fm_p->outFm.tswr = %ld\n",fm_p->outFm.tswr);
*/	
	
	    /* Parse the TS header
	       bufts[0]: 0x47
	       bufts[1-2]: 13lsbs : PID
	       bufts[3]: bits 5&4 adaptation field control
	    */
	    bufts = &buf_p->buf[buf_p->bc];
	    if (bufts[0] != 0x47) {
	        DB_L00(" tsRead::ERROR in TS PACKET\n");
            DB_L0(" tsRead::PACKET NUMBER %ld\n",(UInt32)buf_p->packetCounter);
	        DB_L0(" tsRead::buf_p->inFm.bc   = %ld\n",buf_p->bc);
	        DB_L0(" tsRead::buf_p->inFm.max  = %ld\n",buf_p->max);
	        DB_L0(" tsRead::buf_p->inFm.tsbc = %ld\n",buf_p->tsbc);
	        DB_L0(" tsRead::buf_p->inFm.tswr = %ld\n",buf_p->tswr);
	        DB_L0(" tsRead::&fm_p->outFm.bc   = %ld\n",fm_p->outFm.bc);
	        DB_L0(" tsRead::&fm_p->outFm.max  = %ld\n",fm_p->outFm.max);
	        DB_L0(" tsRead::&fm_p->outFm.tsbc = %ld\n",fm_p->outFm.tsbc);
	        DB_L0(" tsRead::&fm_p->outFm.tswr = %ld\n",fm_p->outFm.tswr);
            if (buf_p == &fm_p->inFm) DB_L00(" tsRead::inFm\n");
            if (buf_p == &fm_p->dataFm) DB_L00(" tsRead::dataFm\n");
		    return(MEDIA_ERR_TS_SYNC_BYTE);
		}
		
	    tmHword = ( (bufts[1]<<8) + bufts[2])& 0x1FFF;
	    
	    switch (tmHword) {
	        case PAT_PID:
	    	    DB_L1(" tsRead::PAT PID 0x%x\n",tmHword);
				if (pmt_p->video_valid == FALSE) {
	            //  table_id
		        pat_p->table_id = bufts[5];
		        DB_L1("     table_id                 %ld\n",pat_p->table_id); 			    
	            //  section_syntax_indicator
		        pat_p->section_syntax_indicator = (bufts[6]&0x80)>>7;
		        DB_L1("     section_syntax_indicator 0x%02lx\n",pat_p->section_syntax_indicator); 			    
	            //  section_length
		        pat_p->section_length = ((bufts[6]&0x0F)<<8) + bufts[7];
		        DB_L1("     section_length           %ld\n",pat_p->section_length); 			    
			    if ( (pat_p->section_length) > (1021) ) return(MEDIA_ERR_TS_ERROR);
	            //  transport_stream_id
		        pat_p->transport_stream_id = ((bufts[8])<<8) + bufts[9];
		        DB_L1("     transport_stream_id      %ld\n",pat_p->transport_stream_id); 			    
	            //  version_number
		        pat_p->version_number = ((bufts[10]&0x3E)>>1);
		        DB_L1("     version_number           %ld\n",pat_p->version_number); 			    
	            //  current_next_indicator
		        pat_p->current_next_indicator = (bufts[10]&0x01);
		        DB_L1("     current_next_indicator   %ld\n",pat_p->current_next_indicator); 			    
	            //  section_number
		        pat_p->section_number = (bufts[11]);
		        DB_L1("     section_number           0x%02lx\n",pat_p->section_number); 			    
	            //  last_section_number
		        pat_p->last_section_number = (bufts[12]);
		        DB_L1("     last_section_number      0x%02lx\n",pat_p->last_section_number);
 				//  program_number
			    program_number  = ((bufts[13])<<8) + bufts[14];
		        DB_L1("     program_number           0x%02lx\n",program_number);
			    if (program_number == 0) {
			        pat_p->network_PID = ((bufts[15]&0x1F)<<8) + bufts[16];
    		        DB_L1("     network_PID              0x%02lx\n",pat_p->network_PID);
				}
                else {
	    		    pat_p->program_map_PID = ((bufts[15]&0x1F)<<8) + bufts[16];
		            DB_L1("     program_map_PID          0x%02lx\n",pat_p->program_map_PID);
				}
			    program_number  = ((bufts[17])<<8) + bufts[18];
			    if (program_number == 0) {
			        pat_p->network_PID = ((bufts[19]&0x1F)<<8) + bufts[20];
    		        DB_L1("     network_PID              0x%02lx\n",pat_p->network_PID);
				}
                else {
	    		    pat_p->program_map_PID = ((bufts[19]&0x1F)<<8) + bufts[20];
		            DB_L1("     program_map_PID          0x%02lx\n",pat_p->program_map_PID);
				}
				pat_p->pat_defined = TRUE;
		        // Copy the whole TS packet to the output
			    MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
//			    if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			    }
	            break;
		    
	        case CAT_PID:
	        	DB_L1(" tsRead::CAT PID 0x%x\n",tmHword);
		        // Copy the whole TS packet to the output
//			    MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			    if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
	            break;
		    
	        case NULL_PID:
	    	    DB_L1(" tsRead::NULL PID 0x%x\n",tmHword);
		        // Copy the whole TS packet to the output
//			    MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			    if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
	            break;
		    
	        default: 
	        if ( ( tmHword >= ANY_PID_MIN ) || ( tmHword <= ANY_PID_MAX ) ) {
		        if (pat_p->pat_defined == TRUE) {
				    if ( tmHword == pat_p->program_map_PID ) {
				        if (pmt_p->video_valid == FALSE) {
            	    	    DB_L1(" PROGRAM MAP PID 0x%x\n",tmHword);
	                        //  table_id
		                    pmt_p->table_id = bufts[5];
		                    DB_L1("     table_id                 %ld\n",pmt_p->table_id); 			    
	                        //  section_syntax_indicator
		                    pmt_p->section_syntax_indicator = (bufts[6]&0x80)>>7;
		                    DB_L1("     section_syntax_indicator 0x%02lx\n",pmt_p->section_syntax_indicator); 			    
	                        //  section_length
		                    pmt_p->section_length = ((bufts[6]&0x0F)<<8) + bufts[7];
		                    DB_L1("     section_length           %ld\n",pmt_p->section_length); 			    
					        if ( (pmt_p->section_length) > (1021) ) return(MEDIA_ERR_TS_ERROR);
	                        //  transport_stream_id
		                    pmt_p->program_number = ((bufts[8])<<8) + bufts[9];
		                    DB_L1("     program_number           %ld\n",pmt_p->program_number); 			    
	                        //  version_number
		                    pmt_p->version_number = ((bufts[10]&0x3E)>>1);
		                    DB_L1("     version_number           %ld\n",pmt_p->version_number); 			    
	                        //  current_next_indicator
		                    pmt_p->current_next_indicator = (bufts[10]&0x01);
		                    DB_L1("     current_next_indicator   %ld\n",pmt_p->current_next_indicator); 			    
	                        //  section_number
		                    pmt_p->section_number = (bufts[11]);
		                    DB_L1("     section_number           0x%02lx\n",pmt_p->section_number); 			    
	                        //  last_section_number
		                    pmt_p->last_section_number = (bufts[12]);
		                    DB_L1("     last_section_number      0x%02lx\n",pmt_p->last_section_number);
	                        //  PCR_PID
		                    pmt_p->PCR_PID = ((bufts[13]&0x1F)<<8) + bufts[14];
		                    DB_L1("     PCR_PID                  0x%02lx\n",pmt_p->PCR_PID);
 				            //  program_info_length
			                pmt_p->program_info_length  = ((bufts[15]&0x0F)<<8) + bufts[16];
				            // stream_type
					        if ( (16+pmt_p->program_info_length) > (TS_PACKET_SIZE-1) ) return(MEDIA_ERR_TS_ERROR);
						    len = 1;
					 		while ((9+pmt_p->program_info_length+len < pmt_p->section_length-4)) {
						        stream_type = bufts[16+pmt_p->program_info_length+len];
		                        DB_L1("     stream_type              0x%02lx\n",stream_type);
					            ES_info_length = ((bufts[16+pmt_p->program_info_length+len+3]&0x0F)<<8) + (bufts[16+pmt_p->program_info_length+len+4]);
		                        DB_L1("     ES_info_length           0x%02lx\n",ES_info_length);
                                if ((stream_type == 2)) {
					    	        pat_p->video_PID = ((bufts[16+pmt_p->program_info_length+len+1]&0x1F)<<8) + bufts[16+pmt_p->program_info_length+len+2];
		                            DB_L1("     pat_p->video_PID         0x%02lx\n", pat_p->video_PID);
				                    pat_p->pmt_defined = TRUE;
						        }
                                if ((stream_type == 3)) {
					    	        pat_p->audio_PID = ((bufts[16+pmt_p->program_info_length+len+1]&0x1F)<<8) + bufts[16+pmt_p->program_info_length+len+2];
		                            DB_L1("     pat_p->audio_PID         0x%02lx\n", pat_p->audio_PID);
						        }
							    len += ES_info_length + 5;
						    }
				        }
	    	            // Copy the whole TS packet to the output
	        	        DB_L1(" tsRead::ANY PID 0x%x\n",tmHword);
//			            MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			            if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
					}
			        else if (pat_p->pmt_defined == TRUE) {
			            if ( tmHword == pat_p->video_PID ) {		
 			                endFlag = TRUE; 
					        if (pmt_p->video_valid == FALSE) {
						        pmt_p->continuity_indicator = bufts[3]&0x0F;
							    // point to the latest TS
							    fm_p->outFm.tsbc = buf_p->bc;
							    if ((fm_p->inFm.buf[buf_p->tsbc+3])&0x20) // if adaptation field
		                            fm_p->outFm.max = TS_PACKET_SIZE - 4 - fm_p->inFm.buf[buf_p->tsbc+4] - 1;
			                    else
		                            fm_p->outFm.max = TS_PACKET_SIZE - 4;

	        	                DB_L1(" fm_p->pmt_p->continuity_indicator %ld\n",pmt_p->continuity_indicator);
							    pmt_p->video_valid = TRUE;
							}
			                // Read the TS payload			
                            if (bufts[3]&0x20) {
		                        DB_L1(" tsRead::ADAPTATION FIELD CONTROL 0x%x\n",(bufts[3]&0x20)); 
				                // get the adaptation field size
		                        DB_L1(" tsRead::ADAPTATION FIELD SIZE %d\n",bufts[4]); 	
	                            // Determine the size of the payload
				                // Provide the payload to the upper level
					            buf_p->bc += (4+bufts[4]+1);
		                        DB_L1(" tsRead::PAYLOAD SIZE %ld\n",buf_p->max-buf_p->bc); 
		                    } 
	    	                else {
				                // Provide the payload to the upper level
					            buf_p->bc += 4;
		                        DB_L1(" tsRead::PAYLOAD SIZE %ld\n",buf_p->max-buf_p->bc); 
					        }
						}
        		        else {
	        	            // Copy the whole TS packet to the output
	            	        DB_L1(" tsRead::ANY PID 0x%x\n",tmHword);
//			                MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			                if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
				        } // if pat_p->video_PID
			        } //  if pmt_p->defined
    		        else {
	    	            // Copy the whole TS packet to the output
	        	        DB_L1(" tsRead::ANY PID 0x%x\n",tmHword);
//			            MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			            if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
				    }
                } //  if pat_p->defined
		        else {
		            // Copy the whole TS packet to the output
	    	        DB_L1(" tsRead::ANY PID 0x%x\n",tmHword);
//			        MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
			        if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
				}
		    }
		    else {
	    	    DB_L1(" tsRead::RESERVED PID 0x%x\n",tmHword);
		        // Copy the whole TS packet to the output
			    if (pmt_p->video_valid == FALSE) MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
//			    MEDIA_ERR(err = tsWrite(buf_p,bufts,nbytes,fm_p->outFm.fiP,0));
		    }
        } // switch
	} while (endFlag == FALSE);
	
	return(err);
}
