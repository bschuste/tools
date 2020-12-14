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
// Creation: 2002/07/15                                                  
// Modified: Brigitte Schuster                                                  
// File: streamanager.c
// History:
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef __TCS__
#include <netdb.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#include "defines.h"
#include "error.h"
#include "externs.h"
#include "tsmanager.h"

////////////////////////////////////////////////////////////////////////
// Global Variables
//
extern UInt32 dbgLevel;


////////////////////////////////////////////////////////////////////////
// Name    		openFilesDesc
// Abstract:	This function opens the MPEG input file , the MPEG-2
// 				output file and the data file.
// In:      	Session structure.
//				MPEG-2 input file name and directory path
// Out:     	Session structure.
//				MPEG-2 output file name
//				Data file name
//				Input, output MPEG-2 and Data File pointers
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/29
////////////////////////////////////////////////////////////////////////
mediaError openFilesDesc(session_p fm_p) {
  	char rootName1[1024]="";
  	char rootName2[1024]="";
    mediaError err = MEDIA_OK;
	
    
    strncpy(rootName1,fm_p->inFm.name,strlen(fm_p->inFm.name)-4);
    
    // construct the root names and  the output MPEG-2 file name
  	if((strcmp(fm_p->outFm.name,DESC_OUT_NAME_DEFAULT)==0)){
    	strcat(rootName2,rootName1);
	    strcpy(fm_p->outFm.name,"dec_");
    	strcat(fm_p->outFm.name,rootName2);
    	strcat(fm_p->outFm.name,".m2v");
  	}
    DB_L1("FILE:: fm_p->outFm.name %s\n",fm_p->outFm.name);
	     
  	fm_p->outFm.fiP=fopen(fm_p->outFm.name,"wb");
  	if (!fm_p->outFm.fiP)	{
        DB_L0("Error:: cannot open file %s\n",fm_p->outFm.name);
    	return(MEDIA_ERR_OPEN_FILE);
  	}
	    
    // Construct the input MPEG-2 file name
/*  	strcpy(rootName2,fm_p->inFilePath);
    strcat(rootName2,"/");  
  	strcat(rootName2,fm_p->inFm.name);  
  	strcpy(fm_p->inFm.name,rootName2);
	
    DB_L0("FILE:: fm_p->inFm.name %s\n",fm_p->inFm.name); */

    if (fm_p->serv_p->serviceType == FILE_TYPE) {	
        // Construct the input MPEG-2 file name
 	    strcpy(rootName2,fm_p->inFilePath);
        strcat(rootName2,"/");  
  	    strcat(rootName2,fm_p->inFm.name);  
  	    strcpy(fm_p->inFm.name,rootName2);
	
        DB_L1("FILE:: fm_p->inFm.name %s\n",fm_p->inFm.name);
	    fm_p->inFm.fiP=fopen(fm_p->inFm.name,"rb");
  	    if (!fm_p->inFm.fiP){
    	    printf("Error:: cannot open %s\n",fm_p->inFm.name);
    	    return(MEDIA_ERR_OPEN_FILE);
  	    }
        if (fread(fm_p->inFm.buf,1,sizeof(UInt8),fm_p->inFm.fiP) == 0) {
            // we reached the end of the file
            return(MEDIA_EOF);
    	}
	    else {
            if (fm_p->inFm.buf[0] != SYNC_BYTE) {
		        // Is not a TS stream
		        fm_p->inFm.type = VIDEO_TYPE;
    	        DB_L10(" Media file elementary video stream\n");
	        }
	        else {
		        // Is  a TS stream
		        fm_p->inFm.type = TS_TYPE;
    	        DB_L10(" Media file transport stream\n");
	        }
            fseek(fm_p->inFm.fiP,0,SEEK_SET);
	    }
    }
    else {
   	    DB_L10(" Connecting to the Media File server\n");
	    MEDIA_ERR(clientMediaConnect(fm_p));
	    
        DB_L1("FILE:: fm_p->inFm.name %s\n",fm_p->inFm.name);
	    fm_p->inFm.fiP=fopen(fm_p->inFm.name,"wb");
  	    if (!fm_p->inFm.fiP){
    	    printf("Error:: cannot open %s\n",fm_p->inFm.name);
    	    return(MEDIA_ERR_OPEN_FILE);
  	    }
    }    

    if (fm_p->serv_p->controlLevel == NO_CONTROL) {
        // construct the input Data file name
  	    if (strcmp(fm_p->dataFm.name,"")==0){
    	    strncpy(fm_p->dataFm.name,rootName1,strlen(rootName1));
    	    strcat(fm_p->dataFm.name,".isw");
	    }
        if (fm_p->serv_p->serviceType == FILE_TYPE) {	
  	        strcpy(rootName2,fm_p->inFilePath);
            strcat(rootName2,"/");  
  	        strcat(rootName2,fm_p->dataFm.name);  
  	        strcpy(fm_p->dataFm.name,rootName2);
        }
	
	    DB_L1("FILE:: fm_p->dataFm.name %s\n",fm_p->dataFm.name);
   
  	    fm_p->dataFm.fiP=fopen(fm_p->dataFm.name,"rb");
  	    if (!fm_p->dataFm.fiP) {
    	    DB_L0("Warning:: cannot open data file %s\n",fm_p->dataFm.name);
   	        DB_L00("Using default values\n");
  	        strcpy(fm_p->dafh_p->mediaFileName,fm_p->inFm.name); // Descrambler: default input file
            return(MEDIA_WRN_DATA_FILE);
  	    }
        if (fread(fm_p->dataFm.buf,1,sizeof(UInt8),fm_p->dataFm.fiP) == 0) {
            // we reached the end of the file
            return(MEDIA_EOF);
    	}
	    else {
            if (fm_p->dataFm.buf[0] != SYNC_BYTE) {
		        // Is not a TS stream
		        fm_p->dataFm.type = VIDEO_TYPE;
    	        DB_L10(" Data File elementary stream\n");
	        }
	        else {
		        // Is  a TS stream
		        fm_p->dataFm.type = TS_TYPE;
    	        DB_L10(" Data File transport stream\n");
	        }
            fseek(fm_p->dataFm.fiP,0,SEEK_SET);
	    }
    }
    else  if (fm_p->serv_p->controlLevel >= LOCAL_CONTROL) {
         // construct the input Data file name for debug pruposes
/*  	    if (strcmp(fm_p->dataFm.name,"")==0){
    	    strncpy(fm_p->dataFm.name,rootName1,strlen(rootName1));
    	    strcat(fm_p->dataFm.name,".isw");
	    }
	
        DB_L1("FILE:: fm_p->dataFm.name %s\n",fm_p->dataFm.name);
	    fm_p->dataFm.fiP=fopen(fm_p->dataFm.name,"wb");
  	    if (!fm_p->dataFm.fiP){
    	    printf("Error:: cannot open %s\n",fm_p->dataFm.name);
    	    return(MEDIA_ERR_OPEN_FILE);
  	    } */

	    err = clientDataConnect(fm_p);
        if (err != MEDIA_OK) return(err);
    }
     
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		closeFiles
// Abstract:	This function closes the MPEG input file , the MPEG-2
// 				output file and the data file.
// In:      	Session structure.
// Out:     	
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/29
////////////////////////////////////////////////////////////////////////
mediaError closeFiles(session_p fm_p){
	mediaError err = MEDIA_OK;
	
//    if (fm_p->serv_p->serviceType == FILE_TYPE) {	
        if (fm_p->inFm.fiP != NULL) 
		    fclose(fm_p->inFm.fiP);
	    else
		    err = MEDIA_WRN_FILE_NOT_OPEN;
//    }
    		
	if (fm_p->outFm.fiP != NULL) 
		fclose(fm_p->outFm.fiP);
	else
		err = MEDIA_WRN_FILE_NOT_OPEN;

	if (fm_p->serv_p->controlLevel == NO_CONTROL) {	
	    if (fm_p->dataFm.fiP != NULL) 
		    fclose(fm_p->dataFm.fiP);
	    else
		    err = MEDIA_WRN_FILE_NOT_OPEN;
    }
/*	
	if (fm_p->inFm.name != NULL)   free(fm_p->inFm.name);
	if (fm_p->outFm.name != NULL)  free(fm_p->outFm.name);
	if (fm_p->dataFm.name != NULL) free(fm_p->dataFm.name);
	if (fm_p->inFilePath != NULL)  free(fm_p->inFilePath);
	if (fm_p->list.name != NULL)   free(fm_p->list.name);
	if (fm_p->serv_p->serviceName != NULL)   free(fm_p->serv_p->serviceName);
	if (fm_p->dafh_p->mediaFileName != NULL) free(fm_p->dafh_p->mediaFileName);
	if (fm_p->serv_p->serviceType == TCP_TYPE)
	    close(fm_p->serv_p->mediaSocket);
*/	
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		flushBuffers
// Abstract:	flush the MPEG-2 and Data output buffers.
// In:      	Session structure
// Out:     	Session structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/24
////////////////////////////////////////////////////////////////////////
mediaError flushBuffers(session_p fm_p){
  
  	mediaError err = MEDIA_OK;
    fileManager_p buf_p;
    UInt32 bc;
    UInt8  bufts[4];
    
   	DB_L30(" flushBuffers\n");
	buf_p = &fm_p->outFm;
	bc = 0;
	DB_L4("outFm::buf_p->name %s\n",buf_p->name);
	DB_L4("outFm::buf_p->bc %ld\n",buf_p->bc);
	DB_L4("outFm::buf_p->max %ld\n",buf_p->max);
   	if (buf_p->fiP){ // an output file has been opened for the buffer
	
	    if ( fm_p->inFm.type == TS_TYPE ) {
    	    // Write the TS header
            bufts[0] = SYNC_BYTE;

    	    // Retrieve the PID + payload_unit_start_indicator (bit6)
            bufts[1] = (fm_p->inFm.buf[buf_p->tsbc+1])&0x1F;
            bufts[2] = (fm_p->inFm.buf[buf_p->tsbc+2])&0xFF;
    	
	        // build the adaptation field and the continuity indicator
		    // get the adaptation field control from input buffer
	        bufts[3] = ((fm_p->inFm.buf[buf_p->tsbc+3])&0x30);	    

            // check the incoming payload (buf)
		    if ((buf_p->buf[0]==0) && (buf_p->buf[1]==0) && (buf_p->buf[2]==1)) {
 	            // Start code detected 
			    if ((buf_p->buf[3] >= VIDEO_STREAM_ID_MIN)&&(buf_p->buf[3]<= VIDEO_STREAM_ID_MAX)) {
    	            // set payload_unit_start_indicator
	                bufts[1] = bufts[1]|0x40;		
		        }
		    }
 
		    // Write the TS header (4 bytes)
	        fwrite(bufts,1,4*sizeof(UInt8),buf_p->fiP);
	    }
	    
	    if (buf_p->bc > 0) {
	        while (bc <= buf_p->bc-1){
   			    DB_L40(" flushBuffers::write the output buffer content to the output file\n");
      		    fwrite(&buf_p->buf[bc],1,sizeof(UInt8),buf_p->fiP);
  			    bc++;
    	    }
    	}
      	buf_p->bc = 0; // signal empty buffer 
  	}
	else return(MEDIA_ERR_FILE_NOT_OPEN);

    if (fm_p->function == SCRAMBLER) {
	    bc = 0;
        buf_p = &fm_p->dataFm;
	    DB_L4("dataFm::buf_p->name %s\n",buf_p->name);
	    DB_L4("dataFm::buf_p->bc %ld\n",buf_p->bc);
	    DB_L4("dataFm::buf_p->max %ld\n",buf_p->max);
	
   	    if (buf_p->fiP){ // a data file has been opened for the buffer
	        if (buf_p->bc > 0) {
	            while (bc <= buf_p->bc-1){
   			        DB_L40(" flushBuffers::write the data buffer content to the data file\n");
      		        fwrite(&buf_p->buf[bc],1,sizeof(UInt8),buf_p->fiP);
  			        bc++;
    	        }
      	        buf_p->bc = 0; // signal empty buffer
	        }
  	    }
	    else return(MEDIA_ERR_FILE_NOT_OPEN);
    }
    if (fm_p->function == DESCRAMBLER) {
        buf_p = &fm_p->dataFm;
	    buf_p->bc = 0;
	}

	return(err);
}

#ifdef OLD
////////////////////////////////////////////////////////////////////////
// Name    		putByte
// Abstract:	put a byte into a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/19
////////////////////////////////////////////////////////////////////////
mediaError putByte(fileManager_p buf_p, UInt8 aByte){
  
  	mediaError err = MEDIA_OK;
   
   	DB_L4(" putByte::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" putByte::buf_p->max = %ld\n",buf_p->max);
	
   	if(buf_p->fiP){ // an output file has been opened for the buffer
    	if (buf_p->bc >= buf_p->max){
      		// write the output buffer to the output file
   			DB_L40(" putByte::write the output buffer to the file\n");
      		fwrite(buf_p->buf,1,(buf_p->max)*sizeof(UInt8),buf_p->fiP);
      		buf_p->bc=0; // signal empty buffer 
    	}
  	}
	else return(MEDIA_ERR_FILE_NOT_OPEN);
	  
  	buf_p->buf[buf_p->bc] = aByte;
   	DB_L4(" putByte::buf[buf_p->bc] = 0x%x\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	buf_p->byteCounter++;
   	
	return(err);
}
#endif

#ifndef OLD
////////////////////////////////////////////////////////////////////////
// Name    		putByte
// Abstract:	put a byte into a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/19
////////////////////////////////////////////////////////////////////////
mediaError putByte(fileManager_p buf_p, UInt8 aByte){
  
  	mediaError   err = MEDIA_OK;
    session_p    fm_p;
       
	fm_p = buf_p->fm_p;
   	DB_L4(" putByte::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" putByte::buf_p->max = %ld\n",buf_p->max);
	
   	if(buf_p->fiP){ // an output file has been opened for the buffer
		if (fm_p->inFm.type == VIDEO_TYPE){
    	    if (buf_p->bc >= buf_p->max) {
      		    // write the output buffer to the output file
   			    DB_L40(" putByte::write the output buffer to the file\n");
      		    fwrite(buf_p->buf,1,(buf_p->max)*sizeof(UInt8),buf_p->fiP);
      		    buf_p->bc = 0; // signal empty buffer 
    	    }
	    }
	    else { // TS TYPE
	        if (buf_p->tsbc == 0) { // Upon init
                if ((fm_p->inFm.buf[buf_p->tsbc+3])&0x20) // if adaptation field
		            buf_p->max = TS_PACKET_SIZE - 4 - fm_p->inFm.buf[buf_p->tsbc+4] - 1;
			    else
		            buf_p->max = TS_PACKET_SIZE - 4;
		    }
	        if (buf_p->bc >= buf_p->max) {
			    MEDIA_ERR(err = tsWrite(buf_p,buf_p->buf,(buf_p->max)*sizeof(UInt8),buf_p->fiP,1));
      		    buf_p->bc = 0; // signal empty buffer
    	    }
	    }
  	}
	else return(MEDIA_ERR_FILE_NOT_OPEN);
	  
  	buf_p->buf[buf_p->bc] = aByte;
   	DB_L4(" putByte::buf[buf_p->bc] = 0x%x\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	buf_p->byteCounter++;
   	
	return(err);
}
#endif

////////////////////////////////////////////////////////////////////////
// Name    		putByteMem
// Abstract:	put a byte into a buffer and reallocate memory.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/19
////////////////////////////////////////////////////////////////////////
mediaError putByteMem(fileManager_p buf_p, UInt8 aByte){
  
  	mediaError err = MEDIA_OK;
   
   	DB_L4(" putByteMem::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" putByteMem::buf_p->max = %ld\n",buf_p->max);
	
    if (buf_p->bc >= buf_p->max){
      	// reallocate memory
   		DB_L40(" putByteMem::reallocate memory\n");
 		// Check if not 16 bits representation overflow
		if ( buf_p->max + PICTURE_SIZE > 65535 ) return(MEDIA_ERR_MEM_SIZE);
		if ( (buf_p->buf = (UInt8*)realloc(&buf_p->buf[0],sizeof(UInt8)*PICTURE_SIZE))== NULL) return(MEDIA_ERR_MEM_ALLOC);

		buf_p->max += PICTURE_SIZE;
    }
	  
  	buf_p->buf[buf_p->bc] = aByte;
   	DB_L4(" putByteMem::buf[buf_p->bc] = %d\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	buf_p->byteCounter++;
	   	
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		getByteMem
// Abstract:	get a byte from a bi-directional buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/04
////////////////////////////////////////////////////////////////////////
mediaError getByteMem(fileManager_p buf_p, UInt8 *tmp){
  
  	mediaError err = MEDIA_OK;
   	
	if (buf_p->bc >= buf_p->max){ 		// Test if buffer is empty
		return(MEDIA_EOF);
  	} 
   *tmp = buf_p->buf[buf_p->bc];
   	DB_L4(" getByteMem::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" getByteMem::buf_p->max = %ld\n",buf_p->max);
   	DB_L4(" getByteMem::buf[buf_p->bc] = %d\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	buf_p->byteCounter++;

	return(err);
}

#if 0
////////////////////////////////////////////////////////////////////////
// Name    		getByte
// Abstract:	get a byte from a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/19
////////////////////////////////////////////////////////////////////////
mediaError getByte(fileManager_p buf_p, UInt8 *tmp){
  
  	mediaError   err = MEDIA_OK;
	session_p    fm_p;
	Int32        nloop, i, max;
	UInt8*       p;
	
	fm_p = (session_p)buf_p->fm_p;
	if (buf_p->bc >= buf_p->max){ 		// Test if buffer is empty
		if ( (fm_p->serv_p->serviceType == TCP_TYPE) && (buf_p == &fm_p->inFm) ) {
		    DB_L0(" getByte::buf_p->bc  = %ld\n",buf_p->bc);
		    DB_L0(" getByte::buf_p->max = %ld\n",buf_p->max);
	    }
	    buf_p->bc = 0 ; 		 	    // Signal buffer is getting loaded
	    if ( (fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p == &fm_p->inFm) ) { 
	      	// Read a new buffer from the media socket - through a hierachical buffer scheme to support permutation
	        nloop = (fm_p->serv_p->size/2)/BUFFER_SIZE;
		    p = buf_p->buf;
		    buf_p->max = fm_p->serv_p->size;
		    DB_L0(" getByte::nloop on mediasocket, nloop = %ld\n",nloop);
	        for (i=0;i<nloop;i++) {
	            max = recv(fm_p->serv_p->mediaSocket, p, BUFFER_SIZE*sizeof(UInt8), 0);
		        fwrite(p,max,sizeof(UInt8),fm_p->inFm.fiP);
	            if ( max <= 0 ) {
   		            DB_L0(" getByte::End of Stream detected on mediasocket, max = %ld\n",max);
      		        return(MEDIA_EOF);
	            }
                else {
			        buf_p->max += max;
			        p += max;
				}
			}
   		    DB_L1(" getByte:: received, buf_p->max = %ld bytes on mediasocket\n",buf_p->max);
		    if ( buf_p->max <= 0) {			
   		        DB_L0(" getByte::End of Stream detected on mediasocket, buf_p->max = %ld\n",buf_p->max);
      		    return(MEDIA_EOF);
		    }
	    }
	    else if ( (fm_p->serv_p->controlLevel > NO_CONTROL)&&(buf_p == &fm_p->dataFm) ) { 
#if 1
            fd_set sockets;
            struct timeval timeout;
                    
            FD_ZERO( &sockets );
            FD_SET( fm_p->serv_p->dataSocket, &sockets );   /* socket we listen for clients */
            timeout.tv_usec = 0; /* wait for 10ms */
            timeout.tv_sec  = 5;

            if( select( fm_p->serv_p->dataSocket + 1, &sockets, NULL, NULL, &timeout ) < 0 )
            {
                perror("getByte::select problem");
            }
            
            if( FD_ISSET( fm_p->serv_p->dataSocket, &sockets ) ) // if received packets
            {
                buf_p->max = recv(fm_p->serv_p->dataSocket, buf_p->buf, BUFFER_SIZE*sizeof(UInt8), 0);
            }
            else // the network is disconnected
            {
                buf_p->max = 0;
		        close(fm_p->serv_p->dataSocket);
			    DB_L00(" getByte::THE NETWORK IS DISCONNECTED\n");
            }
#else
      	// Read a new buffer from the data socket
	        buf_p->max = recv(fm_p->serv_p->dataSocket, buf_p->buf, BUFFER_SIZE*sizeof(UInt8), 0);
#endif
   		    DB_L1(" getByte:: received totally, buf_p->max = %ld bytes on datasocket \n",buf_p->max);
		    if ( buf_p->max <= 0) {			
   		        DB_L0(" getByte::End of Stream detected on datasocket, buf_p->max = %ld\n",buf_p->max);
      		    return(MEDIA_WRN_DATA_FILE);
		    }
	    }
        else {
    	    if ((feof(buf_p->fiP)==0)){
      		    // Read a new buffer from the file
			    buf_p->max = fread(buf_p->buf,1,BUFFER_SIZE*sizeof(UInt8),buf_p->fiP);
      		    if (buf_p->max != BUFFER_SIZE*sizeof(UInt8)) {			
   				    DB_L1(" getByte::End of File detected, buf_p->max = %ld\n",buf_p->max);
			    }
    	    }
    	    else // we reached the end of the file
	        {
   				DB_L0(" getByte::End of File detected, buf_p->max = %ld\n",buf_p->max);
      		    return(MEDIA_EOF);
		    }
		}
  	}
   *tmp = buf_p->buf[buf_p->bc];
   	DB_L4(" getByte::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" getByte::buf_p->max = %ld\n",buf_p->max);
   	DB_L4(" getByte::buf[buf_p->bc] = %d\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	buf_p->byteCounter++;
	
	return(err);
}
#endif

#ifdef OLD
////////////////////////////////////////////////////////////////////////
// Name    		getByte
// Abstract:	get a byte from a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/19
////////////////////////////////////////////////////////////////////////
mediaError getByte(fileManager_p buf_p, UInt8 *tmp){
  
  	mediaError     err = MEDIA_OK;
	session_p      fm_p;
	Int32          nbytes;
	UInt8          p[BUFFER_SIZE];
    fd_set         sockets;
    struct timeval timeout;
                    	
	fm_p = (session_p)buf_p->fm_p;
	if (buf_p->bc == buf_p->wr) { 		// Test if buffer is empty
	    if ( (fm_p->serv_p->serviceType == TCP_TYPE) && (buf_p == &fm_p->inFm) ) { 
      		// Read a new buffer from the media socket
			if ( (buf_p->max - buf_p->wr) >= BUFFER_SIZE ) {
		        nbytes = recv(fm_p->serv_p->mediaSocket, (buf_p->buf + buf_p->wr), BUFFER_SIZE*sizeof(UInt8), 0);
		        if (nbytes <= 0) {			
   		            DB_L0(" getByte::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		        return(MEDIA_EOF);
			    }
			    // Save the stream locally		
		        fwrite((buf_p->buf + buf_p->wr), nbytes, sizeof(UInt8), fm_p->inFm.fiP);
      		}
		    else {
		        DB_L10(" getByte::Should rarely happen on mediasocket\n");
		        nbytes = recv(fm_p->serv_p->mediaSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
		        if (nbytes <= 0) {			
   		            DB_L0(" getByte::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		        return(MEDIA_EOF);
			    }
			    // Save the stream locally		
		        fwrite(p, nbytes, sizeof(UInt8), fm_p->inFm.fiP);
			    // Copy into the input buffer the right amount of bytes
			    if (nbytes > (buf_p->max - buf_p->wr)) {
			        memcpy((buf_p->buf + buf_p->wr),&p[0],(buf_p->max - buf_p->wr));
			        memcpy(buf_p->buf,&p[(buf_p->max - buf_p->wr)],nbytes-(buf_p->max - buf_p->wr));
				} 
				else {
			        memcpy((buf_p->buf + buf_p->wr),&p[0],nbytes);
				}
			}
			// Circular buffer
			if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			    buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
		    }
			else {
			    buf_p->wr += nbytes;
            }
	    }
	    else if ( (fm_p->serv_p->controlLevel > NO_CONTROL)&&(buf_p == &fm_p->dataFm) ) { 
            FD_ZERO( &sockets );
            FD_SET( fm_p->serv_p->dataSocket, &sockets );   /* socket we listen for clients */
            timeout.tv_usec = 0; 
            timeout.tv_sec  = 1; /* wait for 1s */

            if( select( fm_p->serv_p->dataSocket + 1, &sockets, NULL, NULL, &timeout ) < 0 )
            {
                perror(" getByte::select()");
            }
            
            if( FD_ISSET( fm_p->serv_p->dataSocket, &sockets ) ) // if received packets
            {
			    if ( (buf_p->max - buf_p->wr) >= BUFFER_SIZE ) {
		            nbytes = recv(fm_p->serv_p->dataSocket, (buf_p->buf + buf_p->wr), BUFFER_SIZE*sizeof(UInt8), 0);
   		            DB_L2(" getByte:: received totally, nbytes = %ld bytes on datasocket \n", nbytes);
		            if (nbytes <= 0) {			
   		                DB_L0(" getByte::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_WRN_DATA_FILE);
			        }
      		    }
		        else {
		            nbytes = recv(fm_p->serv_p->dataSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
		            if (nbytes <= 0) {			
   		                DB_L0(" getByte::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_WRN_DATA_FILE);
			        }
			        // Copy into the input buffer the right amount of bytes
			        if (nbytes > (buf_p->max - buf_p->wr)) {
			            memcpy((buf_p->buf + buf_p->wr),&p[0],(buf_p->max - buf_p->wr));
			            memcpy(buf_p->buf,&p[(buf_p->max - buf_p->wr)],nbytes-(buf_p->max - buf_p->wr));
				    } 
				    else {
			            memcpy((buf_p->buf + buf_p->wr),&p[0],nbytes);
				    }
			    }
			    // Circular buffer
			    if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			        buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
		        }
			    else {
			        buf_p->wr += nbytes;
                }
            }
            else // the network is disconnected
            {
//		        close(fm_p->serv_p->dataSocket);
//		        close(fm_p->serv_p->controlSocket);
			    DB_L00(" getByte::THE NETWORK IS DISCONNECTED\n");
      		    return(MEDIA_WRN_DATA_FILE);
            }
	    }
        else {
    	    if ((feof(buf_p->fiP)==0)) {
      		    // Read a new buffer from the file
		        nbytes = buf_p->max - buf_p->wr; 		        
			    if ( nbytes >= BUFFER_SIZE ) {
			        nbytes = fread((buf_p->buf + buf_p->wr),1,BUFFER_SIZE*sizeof(UInt8),buf_p->fiP);
		            if (nbytes != BUFFER_SIZE*sizeof(UInt8)) {			
   				        DB_L0(" getByte::End of File detected, nbytes = %ld\n",nbytes);
			        }
      		    }
		        else {
			        nbytes = fread((buf_p->buf + buf_p->wr),1,nbytes*sizeof(UInt8),buf_p->fiP);
		            if (nbytes != (buf_p->max - buf_p->wr)*sizeof(UInt8)) {			
   				        DB_L0(" getByte::End of File detected, nbytes = %ld\n",nbytes);
			        }
			    }
			    // Circular buffer
			    if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			        buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
				}
				else {
				    buf_p->wr += nbytes;
		        } 
    	    }
    	    else {
		        // we reached the end of the file
   				DB_L0(" getByte::End of File detected, buf_p->wr = %ld\n",buf_p->wr);
      		    return(MEDIA_EOF);
		    }
		}
  	}
   *tmp = buf_p->buf[buf_p->bc];
   	DB_L4(" getByte::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" getByte::buf_p->wr  = %ld\n",buf_p->wr);
   	DB_L4(" getByte::buf_p->max = %ld\n",buf_p->max);
   	DB_L4(" getByte::buf[buf_p->bc] = %d\n",buf_p->buf[buf_p->bc]);
  	buf_p->bc++;
	if (buf_p->bc >= buf_p->max) {
	    buf_p->bc = 0 ;
	}
	buf_p->byteCounter++;
	
	return(err);
}
#else
////////////////////////////////////////////////////////////////////////
// Name    		getByte
// Abstract:	get a byte from a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/08/07
////////////////////////////////////////////////////////////////////////
mediaError getByte(fileManager_p buf_p, UInt8 *tmp){
  
  	mediaError     err = MEDIA_OK;
	session_p      fm_p;
	Int32          nbytes;
                    	
	fm_p = (session_p)buf_p->fm_p;   	    
   	
	if (buf_p->bc == buf_p->max){ 		// Test if buffer is empty
        // Read a new buffer
		if (buf_p->type == TS_TYPE) {
		    if (buf_p == &fm_p->dataFm) {
	            MEDIA_DATA_FILE(err = tsRead(buf_p));
		    }
		    else {
	            MEDIA_ERR(err = tsRead(buf_p));
		    }
		}
		else {
		    if (buf_p == &fm_p->dataFm) {
	            MEDIA_DATA_FILE(err = getPacket(buf_p,0));
		    
		    }
		    else {
		        // set-up the stream type at the beginning
		        if (buf_p->byteCounter == 0.0) {
			        MEDIA_ERR(err = tsRead(buf_p));
			    }
			    else {
	                MEDIA_ERR(err = getPacket(buf_p,0));
		    	}
		    }
		    // buf->max has to be ALWAYS higher than buf_p->bc
		    nbytes = buf_p->max - buf_p->bc;
		    if (nbytes < 0) {
		        nbytes = buf_p->tsmax - buf_p->bc + buf_p->max;
		    }
      	    if ( nbytes < BUFFER_SIZE*sizeof(UInt8)) {
	            // Ce n'est pas forcément la fin de fichier
   		        DB_L1(" getByte:: retrieved nbytes = %ld\n",nbytes);
			}
			else if ( nbytes > BUFFER_SIZE*sizeof(UInt8)) {
   		        DB_L0(" getByte:: nbytes > BUFFER_SIZE %ld\n",nbytes);
			    return(MEDIA_ERR_TS_ERROR);
			}
		}
  	}
   *tmp = buf_p->buf[buf_p->bc];
  	buf_p->bc++;
   	DB_L4(" getByte::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" getByte::buf_p->max = %ld\n",buf_p->max);
   	DB_L4(" getByte::buf[buf_p->bc] = %d\n",buf_p->buf[buf_p->bc]);
	buf_p->byteCounter++;
	if ( ((buf_p->bc >= buf_p->tsmax)&&(buf_p == &fm_p->dataFm)) ||
  	     ((buf_p->bc >= buf_p->tsmax)&&(buf_p == &fm_p->inFm)&&(fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p->type == VIDEO_TYPE)) ){
	    buf_p->bc = 0;
    }

	return(err);
}
#endif

////////////////////////////////////////////////////////////////////////
// Name    		setBufferPointer
// Abstract:	set the bc pointer to a relative value
//				check if underflow
// In:      	File Manager structure
//				bc pointer
// Out:     	bc pointer
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/08
////////////////////////////////////////////////////////////////////////
mediaError setBufferPointer(fileManager_p buf_p, UInt16 tmpBc){

  	mediaError err = MEDIA_OK;

	// set the new pointer value
	if ( buf_p->bc >= tmpBc) 
		buf_p->bc -= tmpBc; 
	else
		err = MEDIA_ERR_BC_UNDERFLOW;

	return(err);
	
}

#if 0
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
    buf_p->suspend = TRUE;
    pos_p->packetCounter = buf_p->packetCounter;
	DB_L1(" getStreamPosition::buffer status bc     %ld\n",buf_p->bc);
	DB_L1(" getStreamPosition::buffer status max    %ld\n",buf_p->max);
	DB_L1(" getStreamPosition::buffer status tswr   %ld\n",buf_p->tswr);
	DB_L1(" getStreamPosition::buffer status tsbc   %ld\n",buf_p->tsbc);
	DB_L1(" getStreamPosition::buffer status tsmax  %ld\n",buf_p->tsmax);
	DB_L1(" getStreamPosition::buffer status PACKET %ld\n",(UInt32)buf_p->packetCounter);
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
    buf_p->max  = pos_p->max;
    buf_p->tsbc = pos_p->tsbc;
    buf_p->suspend = FALSE;
    buf_p->packetCounter = pos_p->packetCounter;
	DB_L1(" setStreamPosition::buffer status bc     %ld\n",buf_p->bc);
	DB_L1(" setStreamPosition::buffer status max    %ld\n",buf_p->max);
	DB_L1(" setStreamPosition::buffer status tswr   %ld\n",buf_p->tswr);
	DB_L1(" setStreamPosition::buffer status tsbc   %ld\n",buf_p->tsbc);
	DB_L1(" setStreamPosition::buffer status tsmax  %ld\n",buf_p->tsmax);
	DB_L1(" setStreamPosition::buffer status PACKET %ld\n",(UInt32)buf_p->packetCounter);

	return(err);
	
}
#endif
#ifdef OLD
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
mediaError getStreamPosition(fileManager_p buf_p, UInt32 *tmp){

  	mediaError err = MEDIA_OK;
	session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;
//	if ( (fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p == &fm_p->inFm) ) {
        DB_L1(" getStreamPosition:: should work %ld\n",buf_p->bc);
	    *tmp = (buf_p->bc);
//	}
//	else { 
//	    *tmp = ftell(buf_p->fiP) -BUFFER_SIZE + (buf_p->bc);
//	}
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
mediaError setStreamPosition(fileManager_p buf_p, UInt32 tmp){

  	mediaError err = MEDIA_OK;
    session_p    fm_p;
	
	fm_p = (session_p)buf_p->fm_p;

//	if ( (fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p == &fm_p->inFm) ) {
        DB_L1(" setStreamPosition:: should work %ld\n",tmp);
	    buf_p->bc = tmp;
//	}
//	else { 
  	    // Position the file
//	    fseek(buf_p->fiP,tmp,0);
	    // Empty the input buffer
//	    buf_p->bc = buf_p->max;
//    }	
	
	return(err);
	
}
#endif

////////////////////////////////////////////////////////////////////////
// Name    		testEndProcess
// Abstract:	terminate the process
// In:      	error code
// Out:     	
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/11
////////////////////////////////////////////////////////////////////////
mediaError testEndProcess(session_p fm_p, mediaError err) {

	if (err == MEDIA_EOF) {
	    if (fm_p->accessType == READ_WRITE) {
            MEDIA_ERR(err=putByte(&fm_p->outFm,0));
            MEDIA_ERR(err=putByte(&fm_p->outFm,0));
            MEDIA_ERR(err=putByte(&fm_p->outFm,1));
            MEDIA_ERR(err=putByte(&fm_p->outFm,SEQUENCE_END_CODE_VALUE));
        }
    
        MEDIA_ERR(err = flushBuffers(fm_p));
        MEDIA_ERR(err = closeFiles(fm_p));
        DB_L00("Program exited with no error\n");
	    return(MEDIA_EOF);
//        exit(0);
    }
	 	
	return(MEDIA_OK);
} 	

////////////////////////////////////////////////////////////////////////
// Name    		getByteInWord
// Abstract:	get a byte from a 32-bits word
//              the position can be either 0,1,2 or 3. 0 is the least
//               significant position
// In:      	32-bits word
//				byte index
// Out:     	8-bits pointer
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/04
////////////////////////////////////////////////////////////////////////
mediaError getByteInWord(UInt32 tmWord, UInt8 tmPos, UInt8 *tmByte_p){

  	mediaError err = MEDIA_OK;
	UInt32 tmW0 = 0;

    switch (tmPos) {
        case 0: tmW0 = tmWord << 24;
			    break;
        case 1: tmW0 = tmWord << 16;
			    break;
        case 2: tmW0 = tmWord << 8;
			    break;
		default : tmW0 = tmWord;
    }
	*tmByte_p = tmW0 >> 24;

	return(err);	
}

////////////////////////////////////////////////////////////////////////
// Name    		putWord
// Abstract:	put a 32-bits word into a buffer.
// In:      	Buffer structure
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/04
////////////////////////////////////////////////////////////////////////
mediaError putWord(fileManager_p buf_p, UInt32 aWord) {
  
  	mediaError err = MEDIA_OK;
	UInt8  aByte = 0;
	UInt8  i;
   
   	DB_L4(" putWord::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L4(" putWord::buf_p->max = %ld\n",buf_p->max);

    for (i=0;i<4;i++) {
	    if ((err=getByteInWord(aWord,3-i,&aByte))!=MEDIA_OK) return err;
	
   	    if(buf_p->fiP){ // an output file has been opened for the buffer
    	    if (buf_p->bc >= buf_p->max){
      		    // write the output buffer to the output file
   			    DB_L40(" putWord::write the output buffer to the file\n");
      		    fwrite(buf_p->buf,1,(buf_p->max)*sizeof(UInt8),buf_p->fiP);
      		    buf_p->bc=0; // signal empty buffer 
    	    }
  	    }
	    else return(MEDIA_ERR_FILE_NOT_OPEN);
	  
  	    buf_p->buf[buf_p->bc] = aByte;
   	    DB_L4(" putWord::buf[buf_p->bc] = 0x%x\n",buf_p->buf[buf_p->bc]);
  	    buf_p->bc++;
   	}
	
	return(err);
}

#if 0
////////////////////////////////////////////////////////////////////////
// Name    		getWord
// Abstract:	get a 32-bits word from a buffer.
//              Suppression of start code emulation when flag == 1 
// In:      	Buffer structure, 32-bits word, flag
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	2002/09/19
// Last update: 2002/07/13
////////////////////////////////////////////////////////////////////////
mediaError getWord(fileManager_p buf_p, UInt32 *aWord_p, UInt8 flag) {
  
  	mediaError err = MEDIA_OK;
	UInt8        aByte = 0;
	UInt8        i;
	session_p    fm_p;
	UInt32       nbytes;
	UInt8        p[BUFFER_SIZE];
    fd_set         sockets;
    struct timeval timeout;

	fm_p = (session_p)buf_p->fm_p;
	
   	DB_L1(" getWord::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L1(" getWord::buf_p->wr  = %ld\n",buf_p->wr);
   	DB_L1(" getWord::buf_p->max = %ld\n",buf_p->max);

    *aWord_p = 0;
    // MSB first on the left side
    // LSB on the right side
    
    for (i=0;i<4;i++) {
	    if (buf_p->bc == buf_p->wr) { 		// Test if buffer is empty
	        if ( (fm_p->serv_p->serviceType == TCP_TYPE)&&(buf_p == &fm_p->inFm) ) { 
      		    // Read a new buffer from the media socket
			    if ( (buf_p->max - buf_p->wr) >= BUFFER_SIZE ) {
		            nbytes = recv(fm_p->serv_p->mediaSocket, (buf_p->buf + buf_p->wr), BUFFER_SIZE*sizeof(UInt8), 0);
		            if (nbytes <= 0) {			
   		                DB_L1(" getWord::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
			        // Save the stream locally		
		            fwrite((buf_p->buf + buf_p->wr), nbytes, sizeof(UInt8), fm_p->inFm.fiP);
      		    }
		        else {
		            DB_L10(" getWord::Should rarely happen on mediasocket\n");
		            nbytes = recv(fm_p->serv_p->mediaSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
		            if (nbytes <= 0) {			
   		                DB_L1(" getWord::End of Stream detected on mediasocket, nbytes = %ld\n",nbytes);
      		            return(MEDIA_EOF);
			        }
			        // Save the stream locally		
		            fwrite(p, nbytes, sizeof(UInt8), fm_p->inFm.fiP);
			        // Copy into the input buffer the right amount of bytes
			        if (nbytes > (buf_p->max - buf_p->wr)) {
			            memcpy((buf_p->buf + buf_p->wr),&p[0],(buf_p->max - buf_p->wr));
			            memcpy(buf_p->buf,&p[(buf_p->max - buf_p->wr)],nbytes-(buf_p->max - buf_p->wr));
				    } 
				    else {
			            memcpy((buf_p->buf + buf_p->wr),&p[0],nbytes);
				    }
			    }
			    // Circular buffer
			    if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			        buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
		        }
			    else {
			        buf_p->wr += nbytes;
                }
	        }
	        else if ( (fm_p->serv_p->controlLevel != NO_CONTROL)&&(buf_p == &fm_p->dataFm) ) { 
                FD_ZERO( &sockets );
                FD_SET( fm_p->serv_p->dataSocket, &sockets );   /* socket we listen for clients */
                timeout.tv_usec = 0; 
                timeout.tv_sec  = 1; /* wait for 1s */

                if( select( fm_p->serv_p->dataSocket + 1, &sockets, NULL, NULL, &timeout ) < 0 )
                {
                    perror( " getWord::select()" );
                }
            
                if( FD_ISSET( fm_p->serv_p->dataSocket, &sockets ) ) // if received packets
                {
			        if ( (buf_p->max - buf_p->wr) >= BUFFER_SIZE ) {
		                nbytes = recv(fm_p->serv_p->dataSocket, (buf_p->buf + buf_p->wr), BUFFER_SIZE*sizeof(UInt8), 0);
   		                DB_L1(" getWord:: received totally, nbytes = %ld bytes on datasocket \n", nbytes);
		                if (nbytes <= 0) {			
   		                    DB_L1(" getWord::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		                return(MEDIA_WRN_DATA_FILE);
			            }
      		        }
		            else {
		                nbytes = recv(fm_p->serv_p->dataSocket, &p[0], BUFFER_SIZE*sizeof(UInt8), 0);
		                if (nbytes <= 0) {			
   		                    DB_L1(" getWord::End of Stream detected on datasocket, nbytes = %ld\n",nbytes);
      		                return(MEDIA_WRN_DATA_FILE);
			            }
			            // Copy into the input buffer the right amount of bytes
			            if (nbytes > (buf_p->max - buf_p->wr)) {
			                memcpy((buf_p->buf + buf_p->wr),&p[0],(buf_p->max - buf_p->wr));
			                memcpy(buf_p->buf,&p[(buf_p->max - buf_p->wr)],nbytes-(buf_p->max - buf_p->wr));
				        } 
				        else {
			                memcpy((buf_p->buf + buf_p->wr),&p[0],nbytes);
				        }
			        }
			        // Circular buffer
			        if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			            buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
		            }
			        else {
			            buf_p->wr += nbytes;
                    }
		        }
                else // the network is disconnected
                {
//		            close(fm_p->serv_p->dataSocket);
//		            close(fm_p->serv_p->controlSocket);
	    		    DB_L10(" getWord::THE NETWORK IS DISCONNECTED\n");
      		        return(MEDIA_WRN_DATA_FILE);
                }
	        }
            else {
		        if ((feof(buf_p->fiP)==0)) {
			        // Read a new buffer from the file
		            nbytes = buf_p->max - buf_p->wr; 		        
			        if ( nbytes >= BUFFER_SIZE ) {
			            nbytes = fread((buf_p->buf + buf_p->wr),1,BUFFER_SIZE*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != BUFFER_SIZE*sizeof(UInt8)) {			
   				            DB_L1(" getWord::End of File detected, nbytes = %ld\n",nbytes);
			            }
      		        }
		            else {
			            nbytes = fread((buf_p->buf + buf_p->wr),1,nbytes*sizeof(UInt8),buf_p->fiP);
		                if (nbytes != (buf_p->max - buf_p->wr)*sizeof(UInt8)) {			
   				            DB_L1(" getWord::End of File detected, nbytes = %ld\n",nbytes);
			            }
			        }
			        // Circular buffer
			        if ( (buf_p->wr + nbytes) >= buf_p->max ) {
			            buf_p->wr = (buf_p->wr + nbytes) - buf_p->max;
				    }
				    else {
				        buf_p->wr += nbytes;
		            }     
    	        }
    	        else // we reached the end of the file
	            {
   				    DB_L10(" getWord::End of File detected\n");
      		        return(MEDIA_EOF);
		        }
  	        }
		}
        aByte = buf_p->buf[buf_p->bc];
	    if (flag==1) {
   	        DB_L4(" getWord::suppression of start code emulation aByte = 0x%x\n",aByte);
		    /* Only the MSB of the first byte of a word needs to be masked */
	        if (i==0) aByte &= 0x7F;
	    }
        *aWord_p = (*aWord_p<<8) + aByte;
	  
   	    DB_L4(" getWord::buf[buf_p->bc] = 0x%x\n",buf_p->buf[buf_p->bc]);
  	    buf_p->bc++;
	    if (buf_p->bc >= buf_p->max) {
	        buf_p->bc = 0 ;
	    }
	    buf_p->byteCounter++;
   	}
	
	return(err);
}

#endif

#if 1
////////////////////////////////////////////////////////////////////////
// Name    		getWord
// Abstract:	get a 32-bits word from a buffer.
//              Suppression of start code emulation when flag == 1 
// In:      	Buffer structure, 32-bits word, flag
// Out:     	Buffer structure
//				error code 
// Author:		Brigitte Schuster
// Modified:	2002/09/19
// Last update: 2002/07/13
////////////////////////////////////////////////////////////////////////
mediaError getWord(fileManager_p buf_p, UInt32 *aWord_p, UInt8 flag) {
  
  	mediaError err = MEDIA_OK;
	UInt8        aByte = 0;
	UInt8        i;
	session_p    fm_p;
 
	fm_p = (session_p)buf_p->fm_p;
	
   	DB_L1(" getWord::buf_p->bc  = %ld\n",buf_p->bc);
   	DB_L1(" getWord::buf_p->max = %ld\n",buf_p->max);

    *aWord_p = 0;
    // MSB first on the left side
    // LSB on the right side
    
    for (i=0;i<4;i++) {
    
        if (buf_p == &fm_p->dataFm) {
	        MEDIA_DATA_FILE(err = getByte(buf_p,&aByte));
		}
        if (buf_p == &fm_p->inFm) {
	        MEDIA_ERR(err = getByte(buf_p,&aByte));
        }
	    if (flag==1) {
   	        DB_L4(" getWord::suppression of start code emulation aByte = 0x%x\n",aByte);
		    /* Only the MSB of the first byte of a word needs to be masked */
	        if (i==0) aByte &= 0x7F;
	    }
        *aWord_p = (*aWord_p<<8) + aByte;
	  
   	    DB_L4(" getWord::buf[buf_p->bc] = 0x%lx\n",*aWord_p);
   	}
	
	return(err);
}

#endif



////////////////////////////////////////////////////////////////////////
// Name    		getVlcCode
// Abstract:	get a variable length code from an input buffer.
//				The buffer structure pointers are updated, it does 
//				copy bytes to the output buffer. By the same time,
//				it reads and parses the input bytes from
//				the input buffer given the total number of bits to read
//				and the number of bits left to read in the current byte.
//				The number of bits left to read is stored in a file manager
//				structure
//				Notice that bits are left-aligned in a byte
// In:      	Session structure
//				Number of bits to read
//				The remaining number of bits in the current byte
// Out:     	Session structure
//				The read VL Code
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/27
////////////////////////////////////////////////////////////////////////
mediaError getVlcCode(UInt32 *number, session_p fm_p, UInt8 readBits) { 	
	// number result: right-aligned VLC
	// readBits: Total amount of bits to read

	fileManager_p buf_p;
	// nbBits: Number of bits to be retrieved from the buffer besides the valid
	// bits available from current byte(curByte)
  	Int16 nbBits; 
	// +---------------------------------------+
	// | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 | current byte
	// +---------------------------------------+
	// <-already read ----><--- vBits --------->
  	Int16 nbBytes=0;	// Number of bytes to read from the input buffer
  	UInt8 j;
	UInt8 tmByte=0;
	UInt8 vBits; 		// Number of remaining bits in the current byte
	UInt8 curByte;
  	mediaError 	err = MEDIA_OK;

  	*number = 0;
	buf_p   = &fm_p->inFm;
	
	vBits   = buf_p->bitsLeft;
	curByte = buf_p->curByte;		
	
	// Initialize the byte
	if (vBits == 0) {
		MEDIA_ERR(err=getByte(&fm_p->inFm, &curByte));
		// Copy the byte read to the output MPEG-2 file			
		if (fm_p->accessType == READ_WRITE) MEDIA_ERR(err=putByte(&fm_p->outFm,curByte));
		if (fm_p->accessType == READ_LOAD) MEDIA_ERR(err=putByteMem(&fm_p->bufNb1,curByte));
		vBits = 8;
	}
  	// Determine the number of bytes to retrieve given the number of bits left to retrieve
	// 
	nbBits  = readBits - vBits;

	DB_L4(" getVlcCode::readBits         = %d\n",readBits);
   	DB_L4(" getVlcCode::vBits            = %d\n",vBits);
   	DB_L4(" getVlcCode::nbBits           = %d\n",nbBits);
   	DB_L4(" getVlcCode::current byte     = 0x%x\n",curByte);	

	if (nbBits <= 0) { // The number of bits to read is less or equal to vBits
    	tmByte = (curByte << (8 - vBits)); // mask bits already read
    	tmByte = (tmByte >> (8 - vBits));
		// Update vBits
		vBits = vBits - readBits; 
    	*number = (UInt32)(tmByte >> vBits);
	}
	else {
		nbBytes = (nbBits/8)+(nbBits%8 ? 1:0);
		if (nbBytes > sizeof(UInt32)) return(MEDIA_ERR_VLC_OVERFLOW);
	
		if (vBits!=0){
    		// we read the last bits of the current byte.
			// The result is right aligned
    		tmByte = (curByte << (8 - vBits)); // mask bits already read
    		tmByte = (tmByte >> (8 - vBits));
    		*number = (UInt32)(tmByte);
  		}
	
   		DB_L4(" getVlcCode::nbBytes          = %d\n",nbBytes);
   		DB_L4(" getVlcCode::*number          = 0x%lx\n",*number);	

		// If nbBytes is 0, we do not increment the index and we do not copy information
		// into the output buffer
  		for (j=1;j<=nbBytes;j++) {					
		    MEDIA_ERR(err=getByte(&fm_p->inFm, &tmByte));
			// Copy the byte read to the output MPEG-2 file			
			if (fm_p->accessType == READ_WRITE) MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
			if (fm_p->accessType == READ_LOAD) MEDIA_ERR(err=putByteMem(&fm_p->bufNb1,tmByte));

			//Update the current byte
			curByte = tmByte;
					
   			DB_L4(" getVlcCode::j                = %d\n",j);
   			DB_L4(" getVlcCode::current byte     = 0x%x\n",tmByte);	

   			if (j != nbBytes) // Get the whole byte
      			*number = (*number<<8) + tmByte;
    		else {
				// Get the number of bits left to read in the current byte
      			vBits = (nbBits-(nbBits/8)*8);  
      			if (vBits == 0) vBits = 8; // the whole byte is valid
      			*number=(*number<<vBits) + (tmByte>>(8-vBits));		
				// Update the number of bits valid in the current byte
				vBits = 8-vBits;
    		}
   			DB_L4(" getVlcCode::vBits       = %d\n",vBits);
		}
	   	DB_L4(" getVlcCode::*number          = 0x%lx\n",*number);
	}

	buf_p->bitsLeft = vBits;
	buf_p->curByte  = curByte;
	
   	return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		putVlcCode
// Abstract:	put a variable length code to an output buffer or a
//				bi-directional
//				Notice that bits are left-aligned in a byte
// In:      	File Manager structure
//				The VL code to write
//				The length of the code to be written
// Out:     	Session structure
//				The read VL Code
//				error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/27
////////////////////////////////////////////////////////////////////////
mediaError putVlcCode(fileManager_p buf_p, UInt32 vlcCode, UInt16 vlcLength) { 	

	mediaError err = MEDIA_OK;
	
    // vBits: Number of remaining bits in the current byte
	// nbBits: Number of bits to be retrieved from the buffer besides the valid
	// bits available from current byte(indexed by bc).
  	Int16 nbBits; 
	// +---------------------------------------+
	// | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 | current byte
	// +---------------------------------------+
	// <-already written ------><--- vBits ---->
  	Int16 nbBytes=0;	// Number of bytes to write to the output buffer
  	UInt8 j;
  	Int16 remainBits=0;
	UInt8 tmByte=0;
	UInt8 vBits, curByte;
	
	vBits   = buf_p->bitsLeft;
	curByte = buf_p->curByte;

  	// Determine the number of bytes to be used given the number of bits left
	// in the current byte
	nbBits  = vlcLength - vBits;
   	DB_L4(" putVlcCode::vBits            = %d\n",vBits);
   	DB_L4(" putVlcCode::nbBits           = %d\n",nbBits);
    DB_L4(" putVlcCode::current byte     = 0x%x\n",curByte);	

	// Initialize the byte
	if (vBits == 8) {
		curByte = 0;		
	}
	if (nbBits <= 0) { // the number of bits to concatenate is fitting into the current byte
		// concatenation
		tmByte = (curByte)|(vlcCode << (vBits - vlcLength));
		if ((vBits - vlcLength) == 0) {
			MEDIA_ERR(err=putByte(buf_p,tmByte));
		   	DB_L4(" putVlcCode::tmByte          = 0x%x\n",tmByte);
			// Initialize the next byte to process
			vBits = 8;
			curByte = 0;
		}
		else {
			curByte = tmByte;
			vBits  = (vBits - vlcLength);
		}
	}
	else {
		// Compute the number of bytes necessary to store the vl code
		nbBytes = (nbBits/8)+(nbBits%8 ? 1:0);
   		DB_L4(" putVlcCode::nbBytes          = %d\n",nbBytes);

		if (nbBytes > 4) return(MEDIA_ERR_VLC_OVERFLOW);
	
		if (vBits!=0){
    		// concatenation with the current byte. The result is right aligned
    		tmByte  = (curByte)|(vlcCode >> (vlcLength - vBits));
   			DB_L4(" putVlcCode::tmByte           = 0x%x\n",tmByte);
			MEDIA_ERR(err=putByte(buf_p,tmByte));
			curByte = 0;
  		}
      	remainBits = nbBits;  

		// If nbBytes is 0, we do not increment the index 
  		for (j=1;j<=nbBytes;j++) {
			// Remove the bits already written
			vlcCode = vlcCode << (sizeof(UInt32)*8 - (remainBits));
			vlcCode = vlcCode >> (sizeof(UInt32)*8 - (remainBits));

			DB_L4(" putVlcCode::j                = %d\n",j);

   			if (j != nbBytes) { // More than 8 bits are remaining in the vlcCode
				tmByte = vlcCode >> ( (vlcLength - vBits) - 8 );
				remainBits = remainBits - 8;
   				DB_L4(" putVlcCode::tmByte           = 0x%x\n",tmByte);
				MEDIA_ERR(err=putByte(buf_p,tmByte));
				// Initialize the next byte to process
				curByte = 0;
			}				
    		else {
				// Get the number of bits left 
				if (remainBits > 8) return(MEDIA_ERR_VLC_OVERFLOW);

				if (remainBits == 8) { // the whole byte is valid
					tmByte = vlcCode;
					MEDIA_ERR(err=putByte(buf_p,tmByte));
					// Initialize the next byte to process
					curByte = 0;
					vBits = 8;
				}
				else {
					vBits = (8-remainBits);
					curByte |= (vlcCode << vBits);   						// left alignment
				}
	   		}
		}
	}
	buf_p->bitsLeft = vBits;
	buf_p->curByte  = curByte;

   	return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		getSequenceParameters
// Abstract:	retrieve the Sequence parameters from the input buffer.
// In:      	Input buffer structure
//				Sequence parameters structure
// Out:     	return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/29
////////////////////////////////////////////////////////////////////////
//       Add bitrate unit and frame rate unit
//  	return MEDIA_ERR_NO_SEQUENCE_HEADER;
mediaError getSequenceParameters(session_p fm_p, seqHeader_p sh_p) {

	mediaError err = MEDIA_OK;				
  	UInt32 number = 0;
	fileManager_p buf_p;
	
	buf_p = &fm_p->inFm;
	buf_p->bitsLeft = 0;
	buf_p->curByte  = 0;

	if ((err=getVlcCode(&number,fm_p,12))!=MEDIA_OK) return err;
	sh_p->hSize = number;
	DB_L1(" horizontal_size = %d\n",sh_p->hSize);
	if ((err=getVlcCode(&number,fm_p,12))!=MEDIA_OK) return err;
	sh_p->vSize = number;
	DB_L1(" vertical_size   = %d\n",sh_p->vSize);
	if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
	sh_p->aspectRatio = number;
	DB_L1(" aspect ratio    = %d\n",sh_p->aspectRatio);
	if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
	sh_p->frameRate = number;
	DB_L1(" frame rate      = %d\n",sh_p->frameRate);
	if ((err=getVlcCode(&number,fm_p,18))!=MEDIA_OK) return err;
	sh_p->bitRate = number;
	DB_L1(" bit rate        = %d\n",sh_p->bitRate);
	if (fm_p->parseFlag == TRUE) {
		DB_L00("Sequence Header Detected\n");
		DB_L0(" horizontal_size = %d\n",sh_p->hSize);
		DB_L0(" vertical_size   = %d\n",sh_p->vSize);
		DB_L0(" aspect ratio    = %d\n",sh_p->aspectRatio);
		DB_L0(" frame rate      = %d\n",sh_p->frameRate);
		DB_L0(" bit rate        = %d\n",sh_p->bitRate);
	}
	return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		getExtension
// Abstract:	retrieve the picture coding or sequence extensions
// In:      	Input buffer structure
//			 	Picture coding extension structure
//				Sequence extension structure
// Out:     	return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/06/27
////////////////////////////////////////////////////////////////////////
mediaError getExtensions(session_p fm_p) {

	UInt32						ID;
  	mediaError 					err = MEDIA_OK;
  	UInt32 						number = 0;
	picture_coding_extension_p 	pce_p;
	sequence_extension_p 		seqex_p;
	fileManager_p 				buf_p;
	
	pce_p = fm_p->pce_p;
	seqex_p = fm_p->seqex_p;
	buf_p = &fm_p->inFm;
	
	buf_p->bitsLeft = 0;
	buf_p->curByte  = 0;

	if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err; 
	ID = number;
		
	switch(ID) {
		case 1 : 		// case Sequence_extension
			DB_L10(" Sequence Extension Detected\n");
			if ((err=getVlcCode(&number,fm_p,8))!=MEDIA_OK) return err;
			seqex_p->profile_and_level = number;		
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			seqex_p->progressive_sequence = number;		
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			seqex_p->chroma_format = number;		
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			seqex_p->horizontal_size_ext = number;		
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			seqex_p->vertical_size_ext = number;		
			if ((err=getVlcCode(&number,fm_p,12))!=MEDIA_OK) return err;
			seqex_p->bit_rate_ext = number;		
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err; // marker bit
			if ((err=getVlcCode(&number,fm_p,8))!=MEDIA_OK) return err;
			seqex_p->vbv_buffer_size_ext = number;		
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			seqex_p->low_delay = number;		
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			seqex_p->frame_rate_ext_n = number;		
			if ((err=getVlcCode(&number,fm_p,5))!=MEDIA_OK) return err;
			seqex_p->frame_rate_ext_d = number;		
			if (fm_p->parseFlag == TRUE) {
			DB_L0(" profile_and_level          = 0x%lx\n",seqex_p->profile_and_level);
			DB_L0(" progressive_sequence       = %ld\n",seqex_p->progressive_sequence);
			DB_L0(" chroma_format              = %ld\n",seqex_p->chroma_format);
			DB_L0(" horizontal_size_ext        = %ld\n",seqex_p->horizontal_size_ext);
			DB_L0(" vertical_size_ext          = %ld\n",seqex_p->vertical_size_ext);
			DB_L0(" bit_rate_ext               = %ld\n",seqex_p->bit_rate_ext);
			DB_L0(" vbv_buffer_size_ext        = %ld\n",seqex_p->vbv_buffer_size_ext);
			DB_L0(" low_delay                  = %ld\n",seqex_p->low_delay);
			DB_L0(" frame_rate_ext_n           = %ld\n",seqex_p->frame_rate_ext_n);
			DB_L0(" frame_rate_ext_d           = %ld\n",seqex_p->frame_rate_ext_d);
			}
			break;
		case 8 :		// case Picture_Coding_Extension
			DB_L10(" Picture Coding Extension Detected\n");
			if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
			pce_p->f_code[0][0] = number;		
			if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
			pce_p->f_code[0][1] = number;
			if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
			pce_p->f_code[1][0] = number;
			if ((err=getVlcCode(&number,fm_p,4))!=MEDIA_OK) return err;
			pce_p->f_code[1][1] = number;
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			pce_p->intra_dc_precision = number;
			if ((err=getVlcCode(&number,fm_p,2))!=MEDIA_OK) return err;
			pce_p->picture_structure = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->top_field_first = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->frame_pred_frame_dct = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->concealment_motion_vectors = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->q_scale_type = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->intra_vlc_format = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->alternate_scan = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->repeat_first_field = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->chroma_420_type = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->progressive_frame = number;
			if ((err=getVlcCode(&number,fm_p,1))!=MEDIA_OK) return err;
			pce_p->composite_display_flag = number;

			DB_L1(" f_code[0][0]               = %ld\n",pce_p->f_code[0][0]);
			DB_L1(" f_code[0][1]               = %ld\n",pce_p->f_code[0][1]);
			DB_L1(" f_code[1][0]               = %ld\n",pce_p->f_code[1][0]);
			DB_L1(" f_code[1][1]               = %ld\n",pce_p->f_code[1][1]);
			DB_L1(" intra_dc_precision         = %ld\n",pce_p->intra_dc_precision);
			DB_L1(" picture_structure          = %ld\n",pce_p->picture_structure);
			DB_L1(" top_field_first            = %ld\n",pce_p->top_field_first);
			DB_L1(" frame_pred_frame_dct       = %ld\n",pce_p->frame_pred_frame_dct);
			DB_L1(" concealment_motion_vectors = %ld\n",pce_p->concealment_motion_vectors);
			DB_L1(" q_scale_type               = %ld\n",pce_p->q_scale_type);
			DB_L1(" intra_vlc_format           = %ld\n",pce_p->intra_vlc_format);
			DB_L1(" alternate_scan             = %ld\n",pce_p->alternate_scan);
			DB_L1(" repeat_first_field         = %ld\n",pce_p->repeat_first_field);
			DB_L1(" chroma_420_type            = %ld\n",pce_p->chroma_420_type);
			DB_L1(" progressive_frame          = %ld\n",pce_p->progressive_frame);
			DB_L1(" composite_display_flag     = %ld\n",pce_p->composite_display_flag);
		
			if (fm_p->parseFlag == TRUE) {
			DB_L2(" f_code[0][0]               = %ld\n",pce_p->f_code[0][0]);
			DB_L2(" f_code[0][1]               = %ld\n",pce_p->f_code[0][1]);
			DB_L2(" f_code[1][0]               = %ld\n",pce_p->f_code[1][0]);
			DB_L2(" f_code[1][1]               = %ld\n",pce_p->f_code[1][1]);
			DB_L2(" intra_dc_precision         = %ld\n",pce_p->intra_dc_precision);
			DB_L2(" picture_structure          = %ld\n",pce_p->picture_structure);
			DB_L2(" top_field_first            = %ld\n",pce_p->top_field_first);
			DB_L2(" frame_pred_frame_dct       = %ld\n",pce_p->frame_pred_frame_dct);
			DB_L2(" concealment_motion_vectors = %ld\n",pce_p->concealment_motion_vectors);
			DB_L2(" q_scale_type               = %ld\n",pce_p->q_scale_type);
			DB_L2(" intra_vlc_format           = %ld\n",pce_p->intra_vlc_format);
			DB_L2(" alternate_scan             = %ld\n",pce_p->alternate_scan);
			DB_L2(" repeat_first_field         = %ld\n",pce_p->repeat_first_field);
			DB_L2(" chroma_420_type            = %ld\n",pce_p->chroma_420_type);
			DB_L2(" progressive_frame          = %ld\n",pce_p->progressive_frame);
			DB_L2(" composite_display_flag     = %ld\n",pce_p->composite_display_flag);
			}
			break;
		default :
			break;
	}

	return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		getDataHeader
// Abstract:	read the header from the data file.
// In:      	Session structure.
// Out:     	return error code 
//				Data file header structure.
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/11
////////////////////////////////////////////////////////////////////////
mediaError getDataHeader(session_p fm_p){

  mediaError err = MEDIA_OK;
  dataFileHeader_p dafh_p;	
  
  dafh_p = fm_p->dafh_p;
    
    
    if (fm_p->serv_p->controlLevel == NO_CONTROL) {
        if (fm_p->dataFm.fiP == NULL) return(MEDIA_ERR_FILE_NOT_OPEN);
  		fscanf(fm_p->dataFm.fiP,"%s\n",dafh_p->mediaFileName);
	}
	// Read in binary
	if ((err=getWord(&fm_p->dataFm,&dafh_p->M,1))!=MEDIA_OK) return err;
	if ((err=getWord(&fm_p->dataFm,&dafh_p->m,1))!=MEDIA_OK) return err;
	if ((err=getWord(&fm_p->dataFm,&dafh_p->W,1))!=MEDIA_OK) return err;
	if ((err=getWord(&fm_p->dataFm,&dafh_p->D,1))!=MEDIA_OK) return err;
	if ((err=getWord(&fm_p->dataFm,&dafh_p->S,1))!=MEDIA_OK) return err;
	if ((err=getByte(&fm_p->dataFm,&dafh_p->X))!=MEDIA_OK) return err;
	if ((err=getByte(&fm_p->dataFm,&dafh_p->parseFlag))!=MEDIA_OK) return err;
	// In the following case, we need to fill the data buffer with only 
	// picture descrambling information coming from the server
	if (fm_p->serv_p->controlLevel == REMOTE_CONTROL) {
	    MEDIA_ERR(err = tsInitInputBuffer(&fm_p->dataFm));
	//    fm_p->dataFm.bc = 0;
	//    fm_p->dataFm.max = 0;
	}
		  
  return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		getPictureType
// Abstract:	test if the Picture is Intra encoded.
// In:      	Files structure.
// Out:     	Picture type
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/29
////////////////////////////////////////////////////////////////////////
mediaError getPictureType(session_p fm_p, picType* pictureType_p) {

	mediaError err = MEDIA_OK;				
  	UInt32 number = 0;
	fileManager_p buf_p;
	
	buf_p = &fm_p->inFm;
	
	buf_p->bitsLeft = 0;
	buf_p->curByte  = 0;
																
	// Retrieve the temporal reference
	if ((err=getVlcCode(&number,fm_p,10))!=MEDIA_OK) return err; 
	DB_L2(" temporal reference = %ld\n",number);
	// Retrieve the picture type
	if ((err=getVlcCode(&number,fm_p,3))!=MEDIA_OK) return err;
	*pictureType_p = (picType)number;
	DB_L1(" picture type       = %d\n",*pictureType_p);
	fm_p->pic_p->pictureType = *pictureType_p;
	
	return(MEDIA_OK);
}

////////////////////////////////////////////////////////////////////////
// Name    		findNextStartCode
// Abstract:	synchronize to the next start code in the stream
// In:      	Session structure.
// Out:     	Position the input buffer to the next start code value
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/11
////////////////////////////////////////////////////////////////////////
mediaError findNextStartCode(session_p fm_p, fileManager_p buf_p, UInt8 *tmByte_p, UInt32 *nbBytes) {
   
  	UInt8 tmp0=1;  		// current byte
  	UInt8 tmp1=1;		// previous byte
  	UInt8 tmp2=1;		// previous previous byte
  	mediaError err = MEDIA_OK;
  	UInt8 startCode = FALSE;  // to TRUE when the start code is detected
	UInt32 count = 0;
	  
	*nbBytes = 0;
	while (startCode == FALSE) {
    	tmp2 = tmp1; 
		tmp1 = tmp0;
  		// Read a byte from the input buffer
		MEDIA_ERR(err=getByte(&fm_p->inFm, &tmp0));
		count++;
		
		// Write a byte to the output buffer
		if (fm_p->accessType == READ_WRITE)
		    MEDIA_ERR(err=putByte(&fm_p->outFm,tmp0));
		if (fm_p->accessType == READ_LOAD)
		    MEDIA_ERR(err=putByteMem(buf_p,tmp0));
		if (fm_p->storeBufNb2 == TRUE) 
            MEDIA_ERR(err=putByteMem(&fm_p->bufNb2,tmp0));

	    if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			// Start code detected 
 			startCode = TRUE;
		}
	}
	MEDIA_ERR(err=getByte(&fm_p->inFm, tmByte_p));
	if (fm_p->accessType == READ_WRITE)
	    MEDIA_ERR(err=putByte(&fm_p->outFm, *tmByte_p));
	if (fm_p->accessType == READ_LOAD)
	    MEDIA_ERR(err=putByteMem(buf_p, *tmByte_p));
	if (fm_p->storeBufNb2 == TRUE) 
        MEDIA_ERR(err=putByteMem(&fm_p->bufNb2,*tmByte_p));
	count++;
	*nbBytes = count;
	return(err);
}


////////////////////////////////////////////////////////////////////////
// Name    		findNextStartCodeMem
// Abstract:	synchronize to the next start code in the stream
// In:      	Session structure.
// Out:     	Position the input buffer to the next start code value
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/11
////////////////////////////////////////////////////////////////////////
mediaError findNextStartCodeMem(session_p fm_p, fileManager_p buf_p, UInt8 *tmByte_p, UInt32 *nbBytes) {
   
  	UInt8 tmp0=1;  		// current byte
  	UInt8 tmp1=1;		// previous byte
  	UInt8 tmp2=1;		// previous previous byte
  	mediaError err = MEDIA_OK;
  	UInt8 startCode = FALSE;  // to TRUE when the start code is detected
	UInt32 count = 0;
	  
	*nbBytes = 0;
	while (startCode == FALSE) {
    	tmp2 = tmp1; 
		tmp1 = tmp0;
  		// Read a byte from the input buffer
		MEDIA_ERR(err=getByteMem(buf_p, &tmp0));
		MEDIA_ERR(err=putByte(&fm_p->outFm, tmp0));
		count++;
		
    	if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			// Start code detected 
 			startCode = TRUE;
		}
	}
	MEDIA_ERR(err=getByteMem(buf_p, tmByte_p));
	MEDIA_ERR(err=putByte(&fm_p->outFm, *tmByte_p));
	count++;
	*nbBytes = count;
	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		storePicture
// Abstract:	store the saved picture into the MPEG-2 output file
//				empties bufferNb1
// In:      	Session structure.
// Out:     	Store into the output file
//				return error code
//				buffer bufNb1 bc = 0
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/31
////////////////////////////////////////////////////////////////////////
mediaError storePicture(session_p fm_p) {
  	mediaError 	err = MEDIA_OK;
	UInt32 i;
	
	DB_L1("    Enter store picture %ld\n", fm_p->bufNb1.bc);
		
  	// Write the saved picture to the output file.
	DB_L2("    Enter store picture, picture size is %ld\n", fm_p->bufNb1.picSize);
	fm_p->bufNb1.bc = 0;
	
	for (i=0 ; i<fm_p->bufNb1.picSize ; i++){
    	MEDIA_ERR(err=putByte(&fm_p->outFm,fm_p->bufNb1.buf[i]));
  	}
	DB_L1("    Exit store picture %ld\n", fm_p->bufNb1.bc);

	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		permutePicture
// Abstract:	permute the current Picture with another one in the 
//			    stream
// In:      	Session structure.
// Out:     	
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/05/31
////////////////////////////////////////////////////////////////////////
mediaError permutePicture(session_p fm_p)	{
  
 	mediaError 	err = MEDIA_OK;
	streamPosition_t posEnd; 
	Int16		picNbDiff;
  	UInt32		picSize = 0;	// this size is the number of bytes after the first slice start code
	UInt32		nbBytes = 0;
	UInt8		tmByte  = 255;
	picType		pictureType;
	UInt16		i = 0;
		
  	DB_L1("    ## PERMUTATION of picture %ld ",fm_p->pic_p->xpro);
   	DB_L1("with %ld \n",fm_p->pic_p->xp2);

	DB_L3("    Enter permutation bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
  
	// Empty the permute 1 buffer and use it as temporary storage
	fm_p->bufNb1.picSize = 2; // count temporal ref & picture type

	// Read the input file until the next picture and store it into the temporary buffer
	// Store all the original slices into the temporary buffer
	if (fm_p->inFm.type == VIDEO_TYPE) {
    	while (tmByte != PICTURE_START_CODE_VALUE) {
	    	fm_p->accessType = READ_LOAD;
		    MEDIA_ERR(err=findNextStartCode(fm_p,&fm_p->bufNb1,&tmByte,&nbBytes));
		    fm_p->bufNb1.picSize += nbBytes;
	    }
	}
	else {
	    while ( (tmByte < VIDEO_STREAM_ID_MIN)||(tmByte > VIDEO_STREAM_ID_MAX) ) {
	    	fm_p->accessType = READ_LOAD;
		    MEDIA_ERR(err=findNextStartCode(fm_p,&fm_p->bufNb1,&tmByte,&nbBytes));
		    fm_p->bufNb1.picSize += nbBytes;
	    }
	}
	
	DB_L3("    After load bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);

	fm_p->bufNb1.picSize -= PICTURE_START_CODE_LENGTH; // decrease start code size in bytes
	
	DB_L3("    picture size is %ld\n", fm_p->bufNb1.picSize);	

	MEDIA_ERR(err=getStreamPosition(&fm_p->inFm,&posEnd));
    fm_p->inFm.suspend = TRUE;
    posEnd.packetCounter = fm_p->inFm.packetCounter;
	
	picNbDiff = fm_p->pic_p->xp2 - fm_p->pic_p->xpro;

    fm_p->accessType = READ_ONLY;

	// Load temporal ref and picture type into bufNb1
	MEDIA_ERR(err = getPictureType(fm_p,&pictureType));
	if (pictureType == I_PICTURE_TYPE)
			picNbDiff--;

	// Synchronize to the next Intra Picture given by xp2
	while (picNbDiff > 0) {
		// Check if Intra Picture
		picSize = 0;
		tmByte = 1;
 		while (tmByte != PICTURE_START_CODE_VALUE) {
			fm_p->accessType = READ_ONLY;
			    
			err=findNextStartCode(fm_p,&fm_p->dataFm,&tmByte,&nbBytes);
		    if (err == MEDIA_EOF) {	    	
		    	MEDIA_ERR(err = flushBuffers(fm_p));
	            MEDIA_ERR(err = closeFiles(fm_p));
//            	DB_L0("ICI::Program exited with no error %d\n",fm_p->accessType);
              	exit(MEDIA_OK);
	        }

			picSize += nbBytes;
 			
			if (tmByte == PICTURE_START_CODE_VALUE) {
				fm_p->accessType = READ_LOAD; // Load the picture type into the bufNb1 buffer
				MEDIA_ERR(err = getPictureType(fm_p,&pictureType));
				if (pictureType == I_PICTURE_TYPE)
					picNbDiff--;
				 // if not yet at the end of the loop, the temporal ref and picture type should be removed from bufNb1
				if (picNbDiff != 0)
					setBufferPointer(&fm_p->bufNb1, 2);
			}
			else if (tmByte == SEQUENCE_END_CODE_VALUE) {
		    	MEDIA_ERR(err = flushBuffers(fm_p));
	            MEDIA_ERR(err = closeFiles(fm_p));
            	DB_L00("Program exited with no error\n");
              	exit(MEDIA_OK);
			}
		}
	}

	// Store the temporal ref and the picture type (13 bits) into the MPEG-2 output file
	DB_L3("    After picture 2 detection bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
	
	// point to the temporal ref in the MPEG-2 output buffer
	setBufferPointer(&fm_p->bufNb1, 2);

// Attention la sauvegarde du start code value 
	for (i=0;i<2;i++) {
		MEDIA_ERR(err = getByteMem(&fm_p->bufNb1, &tmByte));
    	MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    }
	// rewind -> point before the picture start code
	setBufferPointer(&fm_p->bufNb1, 6);
	
	// Read the picture to permute from file and copy into the MPEG-2 output buffer
	fm_p->accessType = READ_WRITE;
	tmByte = 1;
	if (fm_p->inFm.type == VIDEO_TYPE) {
	    while (tmByte != PICTURE_START_CODE_VALUE) {
		    MEDIA_ERR(err = findNextStartCode(fm_p,&fm_p->dataFm,&tmByte,&nbBytes));
 	    }
	}
	else {
	    while ( (tmByte < VIDEO_STREAM_ID_MIN)||(tmByte > VIDEO_STREAM_ID_MAX) ) {
		    MEDIA_ERR(err = findNextStartCode(fm_p,&fm_p->dataFm,&tmByte,&nbBytes));
 	    }
    }

	// Rewind the input file to the marked position (end of picture 1)
	MEDIA_ERR(err = setStreamPosition(&fm_p->inFm,&posEnd));
    fm_p->inFm.suspend = FALSE;
    fm_p->inFm.packetCounter = posEnd.packetCounter;
	
	DB_L1("    End of permutation bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
	DB_L1("    End of permutation inFm buffer status bc    %ld\n",fm_p->inFm.bc);
	DB_L1("    End of permutation inFm buffer status max   %ld\n",fm_p->inFm.max);
	DB_L1("    End of permutation inFm buffer status tswr  %ld\n",fm_p->inFm.tswr);
	DB_L1("    End of permutation inFm buffer status tsbc  %ld\n",fm_p->inFm.tsbc);
	DB_L1("    End of permutation inFm buffer status tsmax %ld\n",fm_p->inFm.tsmax);
  	DB_L10("    ## END OF PERMUTATION\n");
	
  	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		getNextIntraPicturetoProcess
// Abstract:	retrieve the next Intra picture number to process 
//			    from the data file
// In:      	Session structure.
// Out:     	
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/14
////////////////////////////////////////////////////////////////////////
mediaError getNextIntraPicturetoProcess(session_p fm_p)	{
  
 	mediaError 	err = MEDIA_OK;
	UInt32      startCode = 0;			   
  	UInt8 tmp0=1;  		// current byte
  	UInt8 tmp1=1;		// previous byte
  	UInt8 tmp2=1;		// previous previous byte
    UInt8 tmByte;
    UInt8 goodCode = FALSE;
  
//   	if ( ( (fm_p->dataFm.fiP)||(fm_p->serv_p->controlLevel != NO_CONTROL) )&&(fm_p->serv_p->statusDataFile == DATA_FILE_OK) ) { // an input file has been opened for the buffer
   	if ( ( (fm_p->dataFm.fiP)||(fm_p->serv_p->controlLevel != NO_CONTROL) )&&(fm_p->serv_p->statusDataFile != DATA_FILE_NOT_AVAILABLE) ) { // an input file has been opened for the buffer
        // If level 2 control, send the status message to the server

	    if ( (fm_p->serv_p->controlLevel == REMOTE_CONTROL)&&(fm_p->serv_p->statusDataFile != DATA_FILE_EOS) ) {
	        MEDIA_ERR(err=dataSendMessage(fm_p, MSG_STATUS,fm_p->serv_p->sndbuf_p, fm_p->serv_p->controlSocket));
	    }
	    // i.e the latest Intra picture number fm_p->pic_p->xc	    
	    	    
		MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&startCode,0));
		if (startCode != (0x00000100|USER_DATA_START_CODE_VALUE)) {
		    DB_L0(" No user data start code, code = 0x%lx\n",startCode);
		    if (fm_p->serv_p->controlLevel == NO_CONTROL) return(MEDIA_ERR_USER_CODE);
			while (tmByte != USER_DATA_START_CODE_VALUE) {
			   goodCode = FALSE;
 	           while (goodCode == FALSE) {
    	           tmp2 = tmp1; 
		           tmp1 = tmp0;
  		           // Read a byte from the data buffer
		           err = getByte(&fm_p->dataFm, &tmp0);
			       if (err == MEDIA_WRN_DATA_FILE)
			           // go out of the loop, the server has been flushed
			           goodCode = TRUE;
	               if((tmp1==0) && (tmp2==0) && (tmp0==1))
 			           // Start code detected 
 			           goodCode = TRUE;
				}
			    if (err == MEDIA_WRN_DATA_FILE) {
			        // go out of the loop, the server has been flushed
			        tmByte = USER_DATA_START_CODE_VALUE;
		            MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&startCode,0));
			    }
				else {
				    MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&tmByte));
				}
		    }
		}
		
		MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xpro,1));
	 	while (fm_p->pic_p->xpro < fm_p->pic_p->xc) {
	        DB_L0(" Next picture to process %ld is too old\n",fm_p->pic_p->xpro);
		    if ( (fm_p->serv_p->controlLevel == REMOTE_CONTROL)&&(fm_p->serv_p->statusDataFile != DATA_FILE_EOS) ) {
			    while (tmByte != USER_DATA_START_CODE_VALUE) {
			        goodCode = FALSE;
 	                while (goodCode == FALSE) {
    	                tmp2 = tmp1; 
		                tmp1 = tmp0;
  		                // Read a byte from the data buffer
		                MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm, &tmp0));
	                    if((tmp1==0) && (tmp2==0) && (tmp0==1))
 			                // Start code detected 
 			                goodCode = TRUE;
					}
                    MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&tmByte));
		        }
		        MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xpro,1));
            }
	        else {
		        fm_p->pic_p->xpro = fm_p->pic_p->xc-1;
			    return(MEDIA_ERR_NEXT_PICTURE);
			}
		}
        MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&fm_p->pic_p->opType));
	 	if (fm_p->pic_p->opType == PERMUTATION) {
	        MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xp2,1));
		}
		else {
			if (fm_p->pic_p->opType == SUBSTITUTION) {
//					fm_p->substitutionStarted = TRUE;
		    }
			else {
	                DB_L0(" Wrong opType = %d\n",fm_p->pic_p->opType);
	                DB_L0(" dataFm bc = %ld\n",fm_p->dataFm.bc);			        
	                DB_L0(" dataFm max = %ld\n",fm_p->dataFm.max);			        
	                DB_L0(" dataFm byteCounter = %f\n",fm_p->dataFm.byteCounter);			        
			        return(MEDIA_ERR_OP_TYPE);
			}
		}
  	}
	else return(MEDIA_WRN_DATA_FILE);
    
	DB_L1("    xc     = %ld\n",fm_p->pic_p->xc);
	DB_L1("    xpro   = %ld\n",fm_p->pic_p->xpro);
	DB_L1("    opType = %d\n",fm_p->pic_p->opType);
	DB_L1("    startCode = %ld\n",startCode);
	if (fm_p->pic_p->opType == PERMUTATION) DB_L1("    xp2    = %ld\n",fm_p->pic_p->xp2);
	DB_L1(" dataFm bc = %ld\n",fm_p->dataFm.bc);			        
	DB_L1(" dataFm max = %ld\n",fm_p->dataFm.max);			        
	DB_L1(" dataFm byteCounter = %f\n",fm_p->dataFm.byteCounter);			        
    		
  	return(err);
}

////////////////////////////////////////////////////////////////////////
// Name    		getNextIntraSliceData
// Abstract:	retrieve the slice from the data file 
// In:      	Session structure.
// Out:     	slice data in bufNb1
//				return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/07/14
////////////////////////////////////////////////////////////////////////
mediaError getNextIntraSliceData(session_p fm_p, UInt8 tmByte)	{
  
	UInt16 	 	i;
  	mediaError 	err = MEDIA_OK;
  	UInt32		sliceSize = 0; 			// original slice size
	UInt32		actualSliceSize = 0;    // slice size as stored in the scrambled file

	DB_L1("    ## SUBSTITUTION tmByte %d\n", tmByte);
  
   	if ( ( (fm_p->dataFm.fiP)||(fm_p->serv_p->controlLevel != NO_CONTROL) )&&(fm_p->serv_p->statusDataFile == DATA_FILE_OK) ) { // an input file has been opened for the buffer
		    // Read in binary
		    if (tmByte != SLICE_START_CODE_VALUE_MIN ) {
			    MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xpro,1));
		        MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&fm_p->pic_p->opType));
			}
			MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&sliceSize,1));
  	}
	else return(MEDIA_WRN_DATA_FILE);
 
	DB_L1("    ## SUBSTITUTION of slice in picture %ld\n", fm_p->pic_p->xpro);

	// Read the original slices from the datafile and put it into the output file
   	for (i=0;i<sliceSize;i++) {
	    MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm, &tmByte));
		MEDIA_ERR(err = putByte(&fm_p->outFm, tmByte));
   	}
	
	// Remove sub slice from the input file
	MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&actualSliceSize,1));
	
   	for (i=0;i<actualSliceSize;i++) {
		MEDIA_ERR(err = getByte(&fm_p->inFm, &tmByte)); 
   	}

	DB_L1("    sliceSize       = %ld\n",sliceSize);
	DB_L1("    actualSliceSize = %ld\n",actualSliceSize);
  	return(err);
}

#if 0
////////////////////////////////////////////////////////////////////////
// Name    		flushNextPictureData
// Abstract:	remove the slice data from the data file 
// In:      	Session structure.
// Out:     	return error code 
// Author:		Brigitte Schuster
// Modified:	
// Last update: 2002/09/18
////////////////////////////////////////////////////////////////////////
mediaError flushNextPictureData(session_p fm_p)	{
  
	UInt16 	 	i;
  	mediaError 	err = MEDIA_OK;
  	UInt32		sliceSize = 0; 			// original slice size
	UInt32		actualSliceSize = 0;    // slice size as stored in the scrambled file
	UInt8       tmByte;
	UInt32      startCode;

    MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&fm_p->pic_p->opType));
	if (fm_p->pic_p->opType == PERMUTATION) {
       MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xp2,1));
	}
	else if (fm_p->pic_p->opType == SUBSTITUTION) {
	    // Read in binary
	    for (i=0; i<0x1E; i++) {
		    MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&sliceSize,1));

	        // Read the original slices from the datafile
   	        for (i=0;i<sliceSize;i++) {
	            MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm, &tmByte));
   	        }
		
	        // Remove sub slice from the input file
	        MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&actualSliceSize,1));
	        MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&fm_p->pic_p->xpro,1));
	        MEDIA_DATA_FILE(err = getByte(&fm_p->dataFm,&fm_p->pic_p->opType));
		}
	}
	else {
        return(MEDIA_ERR_OP_TYPE);
	}
	
	MEDIA_DATA_FILE(err = getWord(&fm_p->dataFm,&startCode,0));
	if (startCode != (0x00000100|USER_DATA_START_CODE_VALUE)) {
        DB_L0(" No user data start code, code = 0x%lx\n",startCode);
		return(MEDIA_ERR_USER_CODE);
	}

  	return(err);
}
#endif

