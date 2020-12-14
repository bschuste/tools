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
// Author: Brigitte Schuster, Tigress                                  
// Creation: 2002/05/03                                                  
// File: error.h
// History:
// 		Version 1.0                                                       
// 		Version 1.1	    2002/07/11      Brigitte Schuster
//                      Update with warning flags
//
////////////////////////////////////////////////////////////////////////

#ifndef ERROR_H
#define ERROR_H

#define MEDIA_ERR_BASE  0x00010000
#define MEDIA_WRN_BASE  0x00020000

#define MEDIA_ERR(a) \
    (a); \
	if ( err == MEDIA_EOF ) { 						            \
		fprintf(PROUT,"End of file, file %s, line %d\n",__FILE__,__LINE__); 	                    \
		return(err); \
	} \
	if ( err & MEDIA_ERR_BASE ) { 						        \
		fprintf(PROUT,"Program exited with code 0x%x\n",err); 	\
		exit(err); \
	} \
	if ( err & MEDIA_WRN_BASE ) { 						        \
	}

enum mediaErrorList {
	MEDIA_OK 					= 0x00,					 // everything is OK
	MEDIA_ERR_RESERVED  		= MEDIA_ERR_BASE | 0x01, // reserved for important things
	MEDIA_ERR_MEM_ALLOC 		= MEDIA_ERR_BASE | 0x02, // not enough memory calling malloc
	MEDIA_ERR_OPEN_FILE 		= MEDIA_ERR_BASE | 0x03, // error opening file, might not exist
	MEDIA_ERR_CHROMA_FORMAT 	= MEDIA_ERR_BASE | 0x04, // erroneous chroma format in stream 
	MEDIA_ERR_NO_SEQUENCE_HEADER= MEDIA_ERR_BASE | 0x05, // no sequence header found in stream
	MEDIA_END_OF_SEQUENCE 		= MEDIA_ERR_BASE | 0x06, // end of sequence detection
	MEDIA_ERR_VLC_OVERFLOW      = MEDIA_ERR_BASE | 0x07, // the size of the VLC to read > 32bits
	MEDIA_ERR_WRONG_PARAMETER 	= MEDIA_ERR_BASE | 0x08, // user parameter is not recognized
	MEDIA_ERR_PARAMETER_MISSING = MEDIA_ERR_BASE | 0x09, // some user parameter is missing and should be defined
	MEDIA_ERR_FILE_NOT_OPEN 	= MEDIA_ERR_BASE | 0x0A, // attempt to read/write while the file is not open
	MEDIA_ERR_VLC_CODE 			= MEDIA_ERR_BASE | 0x0B, // the generated vlc code <1 or >1027
	MEDIA_ERR_MEM_SIZE 			= MEDIA_ERR_BASE | 0x0C, // memory allocation results in size > 65525 bytes
	MEDIA_ERR_BC_UNDERFLOW 		= MEDIA_ERR_BASE | 0x0D, // set of buffer pointer out of memory
	MEDIA_ERR_OP_TYPE   		= MEDIA_ERR_BASE | 0x0E, // wrong operation type in data file
	MEDIA_ERR_NEXT_PICTURE      = MEDIA_ERR_BASE | 0x0F, // next picture to process (xpro) is erroneous
	MEDIA_ERR_MSG_TYPE          = MEDIA_ERR_BASE | 0x10, // the message type is erroneous
	MEDIA_ERR_MSG_LENGTH        = MEDIA_ERR_BASE | 0x11, // the message length is erroneous
	MEDIA_ERR_MSG_RCV           = MEDIA_ERR_BASE | 0x12, // the message is erroneous
	MEDIA_ERR_CONNECT           = MEDIA_ERR_BASE | 0x13, // IP connection declined
	MEDIA_ERR_SEND              = MEDIA_ERR_BASE | 0x14, // IP send message error
	MEDIA_ERR_SOCKET            = MEDIA_ERR_BASE | 0x15, // open socket error
	MEDIA_ERR_RECV              = MEDIA_ERR_BASE | 0x16, // IP recv message error
	MEDIA_ERR_USER_CODE         = MEDIA_ERR_BASE | 0x17, // user data start code cannot be found in datafile
	MEDIA_ERR_TS_SYNC_BYTE      = MEDIA_ERR_BASE | 0x18, // the input TS packet does not start with 0x47
	MEDIA_ERR_TS_HEADER_UNAVAIL = MEDIA_ERR_BASE | 0x19, // there is no more TS header storage available for tsCopy
	MEDIA_ERR_TS_ERROR          = MEDIA_ERR_BASE | 0x1A, // global TS error
	MEDIA_PARAMETER_NOT_FOUND	= MEDIA_WRN_BASE | 0x01, // user parameter not defined
	MEDIA_PARAMETER_FOUND		= MEDIA_WRN_BASE | 0x02, // user parameter defined
	MEDIA_INTRA					= MEDIA_WRN_BASE | 0x03, // picture type is Intra
	MEDIA_EOF					= MEDIA_WRN_BASE | 0x04, // end of file indicator
	MEDIA_WRN_FILE_NOT_OPEN		= MEDIA_WRN_BASE | 0x05, // when the file has not been opened
	MEDIA_WRN_DATA_FILE 		= MEDIA_WRN_BASE | 0x06  // when the data file can not be found
};

typedef enum mediaErrorList mediaError;
 
extern void generateError(mediaError err, int eofErr);

#endif
