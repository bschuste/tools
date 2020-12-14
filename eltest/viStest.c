/***********************************************************************
 **
   Copyright (c) Tigress 2002. 							             
   All rights reserved. Reproduction in whole or in part is prohibited   
   without the written consent of the copyright holder.                  
                                                                                    						             
   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 
   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Tigress Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **						
 ***********************************************************************					
 **
   File: viStest.c
   Abstract:	
   Usage: viSpsnr original_file_name input_file_name [number of pictures]
                number of pictures is optional, default is 9999
   Out:     	prints the psnr values per image out
   Author: Brigitte Schuster
   Last update: 2002/06/29                                                  
   Modified:                                                   
   History:
 		Version 0.1	 2002/06/29
 **						
 ***********************************************************************/		
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#include "error.h"

UInt32 dbgLevel;

mediaError initSession(session_p fm_p) {
	
	memset(fm_p->pat_p,sizeof(program_association_table_t),0);
    memset(fm_p->pmt_p,sizeof(program_map_table_t),0);
	
	fm_p->pmt_p->video_valid = FALSE;
	fm_p->pat_p->pat_defined = FALSE;
	fm_p->pat_p->pmt_defined = FALSE;
	
	memset(&fm_p->inFm,sizeof(fileManager_t),0);
	memset(&fm_p->dataFm,sizeof(fileManager_t),0);
	memset(&fm_p->outFm,sizeof(fileManager_t),0);

  	// allocate memory for the file names
  	if ((fm_p->inFm.name   = (Char*)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	if ((fm_p->outFm.name  = (Char*)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	if ((fm_p->dataFm.name = (Char*)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	strcpy(fm_p->inFm.name,DESC_IN_NAME_DEFAULT);
  	strcpy(fm_p->outFm.name,DESC_OUT_NAME_DEFAULT);
    
    fm_p->inFm.fm_p  = fm_p;
  	fm_p->outFm.fm_p = fm_p;
  	fm_p->dataFm.fm_p= fm_p;
  	fm_p->bufNb1.fm_p= fm_p;
  	fm_p->bufNb2.fm_p= fm_p;

	fm_p->inFm.bc    = 0;
  	fm_p->outFm.bc   = 0;
  	fm_p->dataFm.bc  = 0;
  	fm_p->bufNb1.bc  = 0;
  	fm_p->bufNb2.bc  = 0;

	fm_p->inFm.tsbc    = 0;
  	fm_p->outFm.tsbc   = 0;
  	fm_p->dataFm.tsbc  = 0;
  	fm_p->bufNb1.tsbc  = 0;
  	fm_p->bufNb2.tsbc  = 0;
	
  	fm_p->inFm.max     = 0;	                // the input buffer is empty when bc == max
  	fm_p->outFm.max    = TS_PACKET_SIZE-4;	// the output buffer is full when bc == max
  	fm_p->dataFm.max   = 0;	                // the input buffer is empty when bc == max
	fm_p->bufNb1.max   = PICTURE_SIZE;	    // store the first picture in a permutation
  	fm_p->bufNb2.max   = PICTURE_SIZE;	    // store the I-1 in a substitution
	
  	fm_p->inFm.tsmax   = 2*BUFFER_SIZE;	    // the input buffer is empty when tsbc == tswr
  	fm_p->outFm.tsmax  = TS_PACKET_SIZE-4;	// the output buffer is full when tsbc == tsmax
  	fm_p->dataFm.tsmax = 2*BUFFER_SIZE;	    // the input buffer is empty when tsbc == tswr
	fm_p->bufNb1.tsmax = PICTURE_SIZE;	    // store the first picture in a permutation
  	fm_p->bufNb2.tsmax = PICTURE_SIZE;	    // store the I-1 in a substitution

	fm_p->inFm.suspend       = FALSE;
	fm_p->outFm.suspend      = FALSE;
	fm_p->dataFm.suspend     = FALSE;
	fm_p->bufNb1.suspend     = FALSE;
	fm_p->bufNb2.suspend     = FALSE;

	fm_p->inFm.toggle       = 0;
	fm_p->outFm.toggle      = 0;
	fm_p->dataFm.toggle     = 0;
	fm_p->bufNb1.toggle     = 0;
	fm_p->bufNb2.toggle     = 0;
	
	fm_p->inFm.tswr    = 0;
  	fm_p->outFm.tswr   = 0;
  	fm_p->dataFm.tswr  = 0;
  	fm_p->bufNb1.tswr  = 0;
  	fm_p->bufNb2.tswr  = 0;
	
	fm_p->inFm.byteCounter   = 0;
	fm_p->outFm.byteCounter  = 0;
	fm_p->dataFm.byteCounter = 0;
	fm_p->bufNb1.byteCounter = 0;
	fm_p->bufNb2.byteCounter = 0;
	fm_p->storeBufNb2        = FALSE;
	
	fm_p->inFm.packetCounter   = 0;
	fm_p->outFm.packetCounter  = 0;
	fm_p->dataFm.packetCounter = 0;

	fm_p->inFm.picSize       = 0;
	fm_p->outFm.picSize      = 0;
	fm_p->dataFm.picSize     = 0;
	fm_p->bufNb1.picSize     = 0;
	fm_p->bufNb2.picSize     = 0;

	fm_p->inFm.nbCoeffs      = 0;
	fm_p->outFm.nbCoeffs     = 0;
	fm_p->dataFm.nbCoeffs    = 0;
	fm_p->bufNb1.nbCoeffs    = 0;
	fm_p->bufNb2.nbCoeffs    = 0;
	
	fm_p->inFm.bitsLeft      = 0;
	fm_p->outFm.bitsLeft     = 0;
	fm_p->dataFm.bitsLeft    = 0;
	fm_p->bufNb1.bitsLeft    = 0;
	fm_p->bufNb2.bitsLeft    = 0;
	
	fm_p->inFm.type          = VIDEO_TYPE;
	fm_p->outFm.type         = VIDEO_TYPE;
	fm_p->dataFm.type        = VIDEO_TYPE;
	fm_p->bufNb1.type        = VIDEO_TYPE;
	fm_p->bufNb2.type        = VIDEO_TYPE;

	fm_p->accessType         = READ_WRITE;

  	// allocate buffer memory
  	if ((fm_p->inFm.buf   = (UInt8*) malloc(sizeof(UInt8)*2*BUFFER_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	if ((fm_p->outFm.buf  = (UInt8*) malloc(sizeof(UInt8)*BUFFER_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	if ((fm_p->dataFm.buf = (UInt8*) malloc(sizeof(UInt8)*2*BUFFER_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	if ((fm_p->bufNb1.buf = (UInt8*) malloc(sizeof(UInt8)*PICTURE_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	if ((fm_p->bufNb2.buf = (UInt8*) malloc(sizeof(UInt8)*PICTURE_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	
  	if ((fm_p->inFilePath = (Char *)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	strcpy(fm_p->inFilePath,".");
	
	fm_p->parseFlag   = FALSE;
	fm_p->skipPicture = FALSE;
	
	// Sequence header initialization
	fm_p->seqh_p->hSize        	= 704;
	fm_p->seqh_p->vSize        	= 480;
	fm_p->seqh_p->frameRate    	= 3;
	fm_p->seqh_p->bitRate 	 	= 25000;
	fm_p->seqh_p->seqFlag 	 	= FALSE; 		// No Sequence Header Detected
	
	// Picture management initialization
	fm_p->pic_p->xpro 			= 1;			// First intra picture to process
	fm_p->pic_p->xc				= 1;
	fm_p->pic_p->xs				= 0;			// is substitution
	fm_p->pic_p->xp				= 0;
	fm_p->pic_p->xp2			= 0;
	fm_p->pic_p->opType 		= SUBSTITUTION;
	fm_p->pic_p->pictureType 	= I_PICTURE_TYPE;

	// Initialize with default parameters
  	if ((fm_p->dafh_p->mediaFileName = (Char *)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	strcpy(fm_p->dafh_p->mediaFileName,IN_NAME_DEFAULT); // Descrambler: default input file
	fm_p->dafh_p->M 			= M_DEFAULT;
  	fm_p->dafh_p->m 			= m_DEFAULT;
  	fm_p->dafh_p->W 			= W_DEFAULT;
  	fm_p->dafh_p->D 			= D_DEFAULT;
  	fm_p->dafh_p->X 			= TRUE;		    // Descrambler:  Permutation disabled
  	fm_p->dafh_p->S 			= 0;			// Descrambler:  Substitution disabled
  	fm_p->dafh_p->parseFlag	    = FALSE;		// Descrambler: media file parse status

	// Initialize picture coding extensions
	fm_p->pce_p->f_code[0][0] 				= 15;		
	fm_p->pce_p->f_code[0][1] 				= 15;
	fm_p->pce_p->f_code[1][0] 				= 15;
	fm_p->pce_p->f_code[1][1] 				= 15;
	fm_p->pce_p->intra_dc_precision 		= 0;
	fm_p->pce_p->picture_structure 			= 3;
	fm_p->pce_p->top_field_first 			= 0;
	fm_p->pce_p->frame_pred_frame_dct 		= 0;
	fm_p->pce_p->concealment_motion_vectors = 0;
	fm_p->pce_p->q_scale_type 				= 0;
	fm_p->pce_p->intra_vlc_format 			= 1; // 0: DCT table 0, 1: DCT table 1
	fm_p->pce_p->alternate_scan 			= 0;
	fm_p->pce_p->repeat_first_field 		= 0;
	fm_p->pce_p->chroma_420_type 			= 1;
	fm_p->pce_p->progressive_frame 			= 0;
	fm_p->pce_p->composite_display_flag 	= 0;

	// Initialize sequence extensions
	fm_p->seqex_p->profile_and_level		= 68;	//Main Profile - Main Level
	fm_p->seqex_p->progressive_sequence		= 0;    // Interlaced
	fm_p->seqex_p->chroma_format			= 1;	// 4:2:0		
	fm_p->seqex_p->horizontal_size_ext		= 0;		
	fm_p->seqex_p->vertical_size_ext		= 0;		
	fm_p->seqex_p->bit_rate_ext				= 0;		
	fm_p->seqex_p->vbv_buffer_size_ext		= 0;		
	fm_p->seqex_p->low_delay				= 0;		
	fm_p->seqex_p->frame_rate_ext_n			= 0;		
	fm_p->seqex_p->frame_rate_ext_d			= 0;		

	// Initialize picture counter structures
	fm_p->stream_p->pictureCounter          = 0.0;
	fm_p->stream_p->b                       = -1;
	fm_p->stream_p->a                       = -1;

	// Initialize service structures
  	if ((fm_p->serv_p->serviceName = (Char *)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	strcpy(fm_p->serv_p->serviceName,DESC_SERV_NAME_DEFAULT);
	fm_p->serv_p->serviceType               = FILE_TYPE;
	fm_p->serv_p->controlLevel              = NO_CONTROL;
	fm_p->serv_p->statusDataFile            = DATA_FILE_NOT_AVAILABLE;
	
	bzero((char *) fm_p->serv_p->mediaServAddr_p, sizeof(struct sockaddr_in));
    fm_p->serv_p->mediaServAddr_p->sin_family      = AF_INET;
    fm_p->serv_p->mediaServAddr_p->sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    fm_p->serv_p->mediaServAddr_p->sin_port        = htons(SERV_TCP_MEDIA_PORT);
	
	bzero((char *) fm_p->serv_p->dataServAddr_p, sizeof(struct sockaddr_in));
    fm_p->serv_p->dataServAddr_p->sin_family      = AF_INET;
    fm_p->serv_p->dataServAddr_p->sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    fm_p->serv_p->dataServAddr_p->sin_port        = htons(SERV_TCP_DATA_PORT);
	
	bzero((char *) fm_p->serv_p->controlServAddr_p, sizeof(struct sockaddr_in));
    fm_p->serv_p->controlServAddr_p->sin_family      = AF_INET;
    fm_p->serv_p->controlServAddr_p->sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    fm_p->serv_p->controlServAddr_p->sin_port        = htons(SERV_TCP_CONTROL_PORT);

    // Message buffers for the data file retrieval
	if ((fm_p->serv_p->sndbuf_p = (UInt8*) malloc(sizeof(UInt8)*SMALL_BUFFER_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
	if ((fm_p->serv_p->rcvbuf_p = (UInt8*) malloc(sizeof(UInt8)*SMALL_BUFFER_SIZE)) == NULL) return(MEDIA_ERR_MEM_ALLOC);

	// Initialize shared memory transfer structures
	fm_p->shared_p->T                       = FALSE;
	fm_p->shared_p->A                       = 0xf0000000;
	
	// Initialize the function
	fm_p->function                          = DESCRAMBLER;

    // Initialize the data file bitrate
    fm_p->bitRate                           = 0;
    
  return MEDIA_OK;
}

int main(int argc, char **argv)
{
	char	         ReadName[256];
	char	         WriteName[256];
	int              i;
    int              NumberOfPackets;
//	unsigned char    buf[1024*1024];
	unsigned char    tmByte;
	mediaError 			        err = MEDIA_OK;     // Error code
  	dataFileHeader_t  	        dafh;         		// data file header parameters structure
  	seqHeader_t 		        seqh;         	    // sequence header parameters structure
	session_t			        fm;                 // session management structure.
  	session_p 			        fm_p=&fm;         
	picManager_t 		        pic;           	    // picture numbers structure
	streamManager_t             stream;             // stream properties structure
	serviceManager_t            serv;               // service structure
	sharedMemManager_t          shared;             // shared memory with the external card
	picture_coding_extension_t	pce;			    // picture coding extension
	sequence_extension_t        seqex;				// sequence extension
    program_association_table_t pat;
    program_map_table_t         pmt;
    struct sockaddr_in          mediaServAddr;
    struct sockaddr_in          dataServAddr;
    struct sockaddr_in          controlServAddr;
    
	    
  	// Initialize the session environment
  	// Allocate the internal input and output data buffers
	fm_p->seqh_p   = &seqh;
	fm_p->pic_p    = &pic;
	fm_p->dafh_p   = &dafh;
	fm_p->pce_p    = &pce;
	fm_p->seqex_p  = &seqex;
	fm_p->stream_p = &stream;
	fm_p->shared_p = &shared;
	fm_p->serv_p   = &serv;
	fm_p->serv_p->mediaServAddr_p   = &mediaServAddr;
	fm_p->serv_p->dataServAddr_p    = &dataServAddr;
	fm_p->serv_p->controlServAddr_p = &controlServAddr;
    fm_p->pmt_p    = &pmt;
    fm_p->pat_p    = &pat;
  
  	MEDIA_ERR(err = initSession(fm_p));	

	if ( argc < 2 ) {
		printf("%s input_file_name\n",argv[0]);
		exit(1);
	}
	else {
		strcpy(ReadName,argv[1]);
//		NumberOfPackets = atoi(argv[2]);
	}

	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
	}
    
	strncpy(WriteName,ReadName,strlen(ReadName)-4);
	strcat(WriteName,".m2o");
	strcpy(fm_p->inFm.name,ReadName);
	strcpy(fm_p->outFm.name,WriteName);

	if ((fm_p->outFm.fiP=fopen(fm_p->outFm.name,"wb"))==NULL) {
		printf("error : cannot open %s\n",fm_p->outFm.name);
		exit(-1);
	}
	if ((fm_p->inFm.fiP=fopen(fm_p->inFm.name,"rb"))==NULL) {
		printf("error : cannot open %s\n",fm_p->inFm.name);
		exit(-1);
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
        fm_p->serv_p->serviceType = TCP_TYPE;
        
	    while ((err = getByte(&fm_p->inFm,&tmByte)) != MEDIA_EOF) {
	        if (err == MEDIA_EOF) {
		        printf(" viStest::fm_p->inFm.bc   = %ld\n",fm_p->inFm.bc);
	            printf(" viStest::fm_p->inFm.max  = %ld\n",fm_p->inFm.max);
	            printf(" viStest::fm_p->inFm.tsbc = %ld\n",fm_p->inFm.tsbc);
	            printf(" viStest::fm_p->inFm.tswr = %ld\n",fm_p->inFm.tswr);
	            printf(" viStest::fm_p->outFm.bc   = %ld\n",fm_p->outFm.bc);
	            printf(" viStest::fm_p->outFm.max  = %ld\n",fm_p->outFm.max);
	            printf(" viStest::fm_p->outFm.tsbc = %ld\n",fm_p->outFm.tsbc);
	            printf(" viStest::fm_p->outFm.tswr = %ld\n",fm_p->outFm.tswr);
		    }
            MEDIA_ERR(err = putByte(&fm_p->outFm,tmByte));
		}
    if (fm_p->inFm.type == VIDEO_TYPE)
        printf(" viStest::fm_p->inFm.type = VIDEO_TYPE\n");
    if (fm_p->inFm.type == TS_TYPE)
        printf(" viStest::fm_p->inFm.type = TS_TYPE\n");
    if (fm_p->outFm.type == VIDEO_TYPE)
        printf(" viStest::fm_p->outFm.type = VIDEO_TYPE\n");
    if (fm_p->outFm.type == TS_TYPE)
        printf(" viStest::fm_p->outFm.type = TS_TYPE\n");

    dbgLevel = 4;
	MEDIA_ERR(err = flushBuffers(fm_p));
    if (fm_p->inFm.name   != NULL) free(fm_p->inFm.name);
	if (fm_p->outFm.name  != NULL) free(fm_p->outFm.name);
	if (fm_p->dataFm.name != NULL) free(fm_p->dataFm.name);
	if (fm_p->inFilePath  != NULL) free(fm_p->inFilePath);
	if (fm_p->serv_p->serviceName   != NULL) free(fm_p->serv_p->serviceName);
	if (fm_p->dafh_p->mediaFileName != NULL) free(fm_p->dafh_p->mediaFileName);
	if (fm_p->serv_p->serviceType == TCP_TYPE)
	    close(fm_p->serv_p->mediaSocket);
	if (fm_p->serv_p->controlLevel != NO_CONTROL)
	    close(fm_p->serv_p->dataSocket);
	if (fm_p->serv_p->controlLevel == REMOTE_CONTROL)
	    close(fm_p->serv_p->controlSocket);

	if (fm_p->inFm.buf   != NULL) free(fm_p->inFm.buf);
	if (fm_p->outFm.buf  != NULL) free(fm_p->outFm.buf);
	if (fm_p->dataFm.buf != NULL) free(fm_p->dataFm.buf);
	if (fm_p->bufNb1.buf != NULL) free(fm_p->bufNb1.buf);	
	if (fm_p->bufNb2.buf != NULL) free(fm_p->bufNb2.buf);
	
  	return MEDIA_OK;
}
