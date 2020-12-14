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
// Creation: 2002/08/05                                                  
// File: mediadescrambler.c
// History:
// 		Version 0.10	    2002/07/07      Brigitte Schuster
//                          File list support
//                          Picture permutation implemented 
//
// 		Version 0.20	    2002/07/18      Brigitte Schuster
//                          Picture substitution implemented
//
// 		Version 0.30	    2002/07/24      Brigitte Schuster
//                          Server connection for local control
//                          Media file retrieval only
//                          User Interface (IHM)
//
// 		Version 0.40	    2002/07/30      Brigitte Schuster
//                          Server connection for local control
//                          User Interface (IHM) connects to server
//                          Media file and Data file retrieval
//
// 		Version 0.50	    2002/08/05      Brigitte Schuster
//                          User Interface has been put in a separate application
//                          Permutation of pictures is supported 
//
// 		Version 0.60	    2002/08/06      Brigitte Schuster
//                          download of media files while connected over
//                          the network 
//
// 		Version 0.70	    2002/08/09      Brigitte Schuster
//                          control de niveau 2
//
// 		Version 0.80	    2002/09/02      Brigitte Schuster
//                          bug correction listed in 2002/08/22 report
//
// 		Version 0.90	    2002/09/11      Brigitte Schuster
//                          bug correction listed in 2002/09/04 report
//
// 		Version 1.00	    2002/09/16      Brigitte Schuster
//                          bug correction listed in 2002/09/13 report
//
// 		Version 1.10	    2002/09/20      Brigitte Schuster
//                          bug correction listed in 2002/09/13 report
//                          start code emulation feature
//
// 		Version 1.20	    2002/09/30      Brigitte Schuster
//                          support of TS stream in substitution mode
//
// 		Version 1.30	    2002/10/10      Brigitte Schuster
//                          support of TS stream in substitution mode
//                          and permutation mode.
//                          ES connections tested
//
// 		Version 1.40	    2002/10/14      Brigitte Schuster
//                          category A: retrieval of ES is functional
//
// 		Version 1.50	    2002/10/20      Brigitte Schuster
//                          category A: retrieval of ES works better
//
// 		Version 1.60	    2002/11/05      Brigitte Schuster
//                          category A: retrieval of TS is running
//
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Include Files
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
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
#include "externs.h"
#include "error.h"
#include "tsmanager.h"


////////////////////////////////////////////////////////////////////////
// Global Variables
//
UInt32 dbgLevel = 0;


////////////////////////////////////////////////////////////////////////
// Mpeg-2 Video - frame rate code
//
float frameRateCode[16] = { 
    1.0, 24000.0/1001.0, 24.0, 25.0,
    30000.0/1001.0, 30.0, 50.0, 60000.0/1001.0,
    60.0, 1.0, 1.0, 1.0,
     1.0, 1.0, 1.0, 1.0
};

////////////////////////////////////////////////////////////////////////
// User Parameters Functions
//
mediaError checkForIntParam(Int16 argc,
			    Char** argv,
			    Char* paramName,
			    UInt32* param,
			    Int16* i){

  if(strcmp(argv[*i],paramName)==0){
    if (*i+1<argc){
      *i=*i+1;
      if (sscanf(argv[*i],"%ld",param)==0){
		printf("Error: parameter %s is not an integer\n",paramName);
		return MEDIA_ERR_WRONG_PARAMETER;
      }
      else {
      	return MEDIA_PARAMETER_FOUND;
		}
    }
    else{
      printf("Error: parameter %s  missing\n",paramName);
      return MEDIA_ERR_PARAMETER_MISSING;
    }
  }
  return MEDIA_PARAMETER_NOT_FOUND;
}

mediaError checkForStrParam(Int16 argc,
			    Char** argv,
			    Char* paramName,
			    Char* param,
			    Int16* i){

  if(strcmp(argv[*i],paramName)==0){
    if (*i+1<argc){
      *i=*i+1;;
      strcpy(param,argv[*i]);
      return MEDIA_PARAMETER_FOUND;
    }
    else{
      printf("Error: parameter %s  missing\n",paramName);
      return MEDIA_ERR_PARAMETER_MISSING;
    }
  }
  return MEDIA_PARAMETER_NOT_FOUND;   
}


void printUserParameters(session_p fm_p) {

	DB_L10("\nUser parameters\n");
	DB_L1(" Parse Flag = %d\n",fm_p->parseFlag);		
//	DB_L1(" List File Name = %s\n",fm_p->list.name);
	DB_L1(" Input File Name = %s\n",fm_p->inFm.name);
	DB_L1(" Input File Path = %s\n",fm_p->inFilePath);
	DB_L1(" Output File Name = %s\n",fm_p->outFm.name);
	DB_L1(" serviceName = %s\n",fm_p->serv_p->serviceName);
	DB_L1(" controlLevel = %d\n",fm_p->serv_p->controlLevel);
	DB_L1(" T = %d\n",fm_p->shared_p->T);
	DB_L1(" A = 0x%lx\n",fm_p->shared_p->A);
//	DB_L1(" a = %ld\n",fm_p->stream_p->a);
//	DB_L1(" b = %ld\n",fm_p->stream_p->b);
	DB_L1(" Debug Level = %ld\n",dbgLevel);
}

void printDataHeader(session_p fm_p) {

	DB_L00("\nData file header\n");
	
//	DB_L0(" Media File Name = %s\n",fm_p->dafh_p->mediaFileName);
	DB_L0(" M = %ld\n",fm_p->dafh_p->M);
	DB_L0(" m = %ld\n",fm_p->dafh_p->m);
	DB_L0(" W = %ld\n",fm_p->dafh_p->W);
	DB_L0(" D = %ld\n",fm_p->dafh_p->D);
	DB_L0(" S = %ld\n",fm_p->dafh_p->S);
	DB_L0(" X = %d\n",fm_p->dafh_p->X);
	DB_L0(" Parse Flag = %d\n",fm_p->dafh_p->parseFlag);		
}


mediaError printSessionParameters(session_p fm_p) {

	DB_L00("\nInternal session parameters\n");

	DB_L0(" skipPicture         = %d\n",fm_p->skipPicture);	        // in process of a permutation	
	DB_L0(" substitutionStarted = %d\n",fm_p->substitutionStarted); // in process of a substitution
	DB_L0(" accessType          = %d\n",fm_p->accessType);          // flow control
	DB_L0(" storeBufNb2         = %d\n",fm_p->storeBufNb2);         // in process to store I-1 in BufNb2
	
	DB_L0(" dataFm.byteCounter  = %f\n",fm_p->dataFm.byteCounter);  // data stream byte counter
	DB_L0(" dataFm.bc           = %ld\n",fm_p->dataFm.bc);           // data stream byte pointer
	DB_L0(" dataFm.max          = %ld\n",fm_p->dataFm.max);          // data stream max pointer
	DB_L0(" dataFm.bitsLeft     = %d\n",fm_p->dataFm.bitsLeft);     // data stream bits left in a byte
	DB_L0(" dataFm.curByte      = %d\n",fm_p->dataFm.curByte);      // data stream curByte being processed
	DB_L0(" dataFm.nbCoeffs     = %d\n",fm_p->dataFm.nbCoeffs);     // data stream
	DB_L0(" dataFm.picSize      = %ld\n",fm_p->dataFm.picSize);     // data stream
	DB_L0(" dataFm.fiP          = 0x%lx\n",(UInt32)fm_p->dataFm.fiP);          // data stream file pointer
	DB_L0(" dataFm.name         = %s\n",fm_p->dataFm.name);         // data stream name
	
	DB_L0(" inFm.byteCounter    = %f\n",fm_p->inFm.byteCounter);    // input stream byte counter
	DB_L0(" inFm.bc             = %ld\n",fm_p->inFm.bc);             // input stream byte pointer
	DB_L0(" inFm.max            = %ld\n",fm_p->inFm.max);            // input stream max pointer
	DB_L0(" inFm.bitsLeft       = %d\n",fm_p->inFm.bitsLeft);       // input stream bits left in a byte
	DB_L0(" inFm.curByte        = %d\n",fm_p->inFm.curByte);        // input stream curByte being processed
	DB_L0(" inFm.nbCoeffs       = %d\n",fm_p->inFm.nbCoeffs);       // input stream
	DB_L0(" inFm.picSize        = %ld\n",fm_p->inFm.picSize);       // input stream
	DB_L0(" inFm.fiP            = 0x%lx\n",(UInt32)fm_p->inFm.fiP);            // input stream file pointer
	DB_L0(" inFm.name           = %s\n",fm_p->inFm.name);           // input stream name
	
	DB_L0(" outFm.byteCounter   = %f\n",fm_p->outFm.byteCounter);   // output stream byte counter
	DB_L0(" outFm.bc            = %ld\n",fm_p->outFm.bc);            // output stream byte pointer
	DB_L0(" outFm.max           = %ld\n",fm_p->outFm.max);           // output stream max pointer
	DB_L0(" outFm.bitsLeft      = %d\n",fm_p->outFm.bitsLeft);      // output stream bits left in a byte
	DB_L0(" outFm.curByte       = %d\n",fm_p->outFm.curByte);       // output stream curByte being processed
	DB_L0(" outFm.nbCoeffs      = %d\n",fm_p->outFm.nbCoeffs);      // output stream
	DB_L0(" outFm.picSize       = %ld\n",fm_p->outFm.picSize);      // output stream
	DB_L0(" outFm.fiP           = 0x%lx\n",(UInt32)fm_p->outFm.fiP);           // output stream file pointer
	DB_L0(" outFm.name          = %s\n",fm_p->outFm.name);          // output stream name
	
 	DB_L0(" xc                  = %ld\n",fm_p->pic_p->xc); 			// current picture being read
 	DB_L0(" xpro                = %ld\n",fm_p->pic_p->xpro); 		// next Intra picture to process
 	DB_L0(" xs                  = %ld\n",fm_p->pic_p->xs); 			// should be 0
 	DB_L0(" xp                  = %ld\n",fm_p->pic_p->xp); 			// should be 0
 	DB_L0(" xp2                 = %ld\n",fm_p->pic_p->xp2); 		// second picture in a permutation operation
 	DB_L0(" opType              = %d\n",fm_p->pic_p->opType); 		// current operation
 	DB_L0(" pictureType         = %d\n",fm_p->pic_p->pictureType);  // current picture type

 	DB_L0(" serviceType         = %d\n",fm_p->serv_p->serviceType);  // current service type
 	DB_L0(" controlLevel        = %d\n",fm_p->serv_p->controlLevel);  // current controllLevel
 	DB_L0(" statusDataFile      = %d\n",fm_p->serv_p->statusDataFile);  // data file stream status

	return(MEDIA_OK);
}

mediaError getUserParameters(Int16 argc, Char** argv, session_p fm_p){
 
  Int16 i;
  mediaError err;
  Char ipAddress[SMALL_BUFFER_SIZE];
  Int32    port;
  
  
  for (i=0;i<SMALL_BUFFER_SIZE;i++) ipAddress[i]='\0';
  strcpy(ipAddress,SERV_HOST_ADDR);
  port = SERV_TCP_MEDIA_PORT;

  for (i=1;i<argc;i++){

    if (strcmp(argv[i],"-h")==0){
      DB_L00("Valid arguments are\n");
      DB_L00(" -h help\n");
      DB_L00(" -v software version\n");
      DB_L00(" -p parse the stream, start codes are printed at debug level 1\n");
      DB_L00(" -d [0,1,2,3,4] debug level, default is 0\n");
//      DB_L00(" -l <listfile>\n");
      DB_L00(" -i <inputfile>, default name is test.m2v\n");
      DB_L00(" -o <outfile>, default name is dec_<inpufile>.m2v\n");
      DB_L00(" -path <path>, default path is .\n");
      DB_L00(" -service [tcp, file] service requested to retrieve the scrambled content, default is file\n");
      DB_L00(" -c [0,1,2] control level, default is 0\n");
//      DB_L00(" -T enable the transfer of the descrambled stream to the PC-hosted card\n");
//      DB_L00(" -A [XXXXXXXX] physical address ot the PC_hosted card memory, default is 0xf0000000\n");
      exit(MEDIA_OK);
    }

    if (strcmp(argv[i],"-v")==0){
        DB_L00("\n mediadescrambler version 1.6, 2002/11/05\n\n");
        exit(MEDIA_OK);
    }

    if (strcmp(argv[i],"-p")==0) {
    	fm_p->parseFlag = TRUE;
      	continue;
	}

    if ((err=checkForIntParam(argc,argv,"-d",&(dbgLevel),&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;  

    if ((err=checkForStrParam(argc,argv,"-i",fm_p->inFm.name,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;		

    if ((err=checkForStrParam(argc,argv,"-ip",ipAddress,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;		

    if ((err=checkForStrParam(argc,argv,"-o",fm_p->outFm.name,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;		

    if ((err=checkForStrParam(argc,argv,"-path",fm_p->inFilePath,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;		

    if ((err=checkForStrParam(argc,argv,"-service",fm_p->serv_p->serviceName,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;		

    if ((err=checkForIntParam(argc,argv,"-c",&(fm_p->serv_p->controlLevel),&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;  

    if (strcmp(argv[i],"-T")==0) { 
    	fm_p->shared_p->T = TRUE;
      	continue;
    }

    if ((err=checkForIntParam(argc,argv,"-A",&(fm_p->shared_p->A),&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;  

    if ((err=checkForIntParam(argc,argv,"-b",&(fm_p->stream_p->b),&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;

    if ((err=checkForIntParam(argc,argv,"-port",&port,&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;

    if ((err=checkForIntParam(argc,argv,"-a",&(fm_p->stream_p->a),&i)) == MEDIA_PARAMETER_FOUND)
      continue;
    else if (err!=MEDIA_PARAMETER_NOT_FOUND)
      return err;
  }
  
  if ( strcmp(fm_p->serv_p->serviceName,"tcp") == 0) {
    fm_p->serv_p->serviceType = TCP_TYPE;
  }
  else { 
    fm_p->serv_p->serviceType  = FILE_TYPE;
  }
  
  fm_p->serv_p->mediaServAddr_p->sin_addr.s_addr = inet_addr(ipAddress);
  fm_p->serv_p->mediaServAddr_p->sin_port        = htons(port);
  fm_p->serv_p->dataServAddr_p->sin_addr.s_addr  = inet_addr(ipAddress);
  fm_p->serv_p->dataServAddr_p->sin_port         = htons(port+2);
  fm_p->serv_p->controlServAddr_p->sin_addr.s_addr = inet_addr(ipAddress);
  fm_p->serv_p->controlServAddr_p->sin_port        = htons(port+3);

  return MEDIA_OK;
}

////////////////////////////////////////////////////////////////////////
// Session Environment Initialization
// Initialize all structures
//
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
//  	if ((fm_p->list.name   = (Char *)malloc(sizeof(Char)*1024)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
  	strcpy(fm_p->inFm.name,DESC_IN_NAME_DEFAULT);
  	strcpy(fm_p->outFm.name,DESC_OUT_NAME_DEFAULT);
//  	strcpy(fm_p->list.name,DESC_LIST_NAME_DEFAULT);
    // List has no buffer to be used, so the related structure fields are not initialized
//    fm_p->list.fiP   = NULL;
    
    
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

	
	fm_p->inFm.type     = VIDEO_TYPE;
	fm_p->outFm.type    = VIDEO_TYPE;
	fm_p->dataFm.type   = VIDEO_TYPE;
	fm_p->bufNb1.type   = VIDEO_TYPE;
	fm_p->bufNb2.type   = VIDEO_TYPE;

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


////////////////////////////////////////////////////////////////////////
// Mediadescrambler
//
Int16 main(Int16 argc, Char *argv[]) {

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
    struct sockaddr_in          mediaServAddr;
    struct sockaddr_in          dataServAddr;
    struct sockaddr_in          controlServAddr;
    program_association_table_t pat;
    program_map_table_t         pmt;
    
    
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
	fm_p->serv_p->mediaServAddr_p = &mediaServAddr;
	fm_p->serv_p->dataServAddr_p  = &dataServAddr;
	fm_p->serv_p->controlServAddr_p  = &controlServAddr;
    fm_p->pmt_p    = &pmt;
    fm_p->pat_p    = &pat;
  
  	MEDIA_ERR(err = initSession(fm_p));
	
    // Create circular list of TS headers
//    tsListOpen(fm_p);

  	// Get the user parameters
  	MEDIA_ERR(err = getUserParameters(argc,argv,fm_p));
	   
  	// Start the descrambling process
   	MEDIA_ERR(err = mediaDescrambler(fm_p));

    if (fm_p->inFm.name != NULL)   free(fm_p->inFm.name);
	if (fm_p->outFm.name != NULL)  free(fm_p->outFm.name);
	if (fm_p->dataFm.name != NULL) free(fm_p->dataFm.name);
	if (fm_p->inFilePath != NULL)  free(fm_p->inFilePath);
	if (fm_p->serv_p->serviceName != NULL)   free(fm_p->serv_p->serviceName);
	if (fm_p->dafh_p->mediaFileName != NULL) free(fm_p->dafh_p->mediaFileName);
	if (fm_p->serv_p->serviceType == TCP_TYPE)
	    close(fm_p->serv_p->mediaSocket);
	if (fm_p->serv_p->controlLevel != NO_CONTROL)
	    close(fm_p->serv_p->dataSocket);
	if (fm_p->serv_p->controlLevel == REMOTE_CONTROL)
	    close(fm_p->serv_p->controlSocket);

	if (fm_p->inFm.buf != NULL)   free(fm_p->inFm.buf);
	if (fm_p->outFm.buf != NULL)  free(fm_p->outFm.buf);
	if (fm_p->dataFm.buf != NULL) free(fm_p->dataFm.buf);
	if (fm_p->bufNb1.buf != NULL) free(fm_p->bufNb1.buf);	
	if (fm_p->bufNb2.buf != NULL) free(fm_p->bufNb2.buf);

  	return 0;

}

void  ExceptFunction()
{
	printf(" Okay, okay, I quit.....\n");
    exit(MEDIA_OK);
}

mediaError mediaDescrambler(session_p fm_p) {

  	dataFileHeader_p  	dafh_p;  	
  	seqHeader_p 		seqh_p;   
  	picManager_p 		pic_p;
  	UInt8 				endOfSequence = FALSE; 	// detection of the end of sequence
	UInt8				tmByte;
	picType				pictureType;
	UInt32				nbBytes;
	UInt16				i = 0;
  	mediaError 			err = MEDIA_OK; // Error code
    UInt32              tmSize = 0;

            
	seqh_p = fm_p->seqh_p;
	pic_p  = fm_p->pic_p;
	dafh_p = fm_p->dafh_p;

	// Set up the input and output names.
   	MEDIA_ERR(err = openFilesDesc(fm_p));
	
	if (err == MEDIA_WRN_DATA_FILE) {
	    fm_p->serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE;
	    DB_L00("Data File not available\n");
	}
	else { 
	    fm_p->serv_p->statusDataFile = DATA_FILE_OK;
    }	
	err = MEDIA_OK;

	// Print out the user parameters
  	printUserParameters(fm_p);

	//Read the header from the data_file
	if ((fm_p->serv_p->statusDataFile != DATA_FILE_NOT_AVAILABLE))
	MEDIA_ERR(err = getDataHeader(fm_p));
	
	if (dbgLevel > 0) printDataHeader(fm_p);
	
	// Compute the buffer size of the input media file to be a multiple of a TS packet
	if (fm_p->inFm.type == VIDEO_TYPE) {
    	fm_p->serv_p->size = PICTURE_SIZE*((Int32)fm_p->dafh_p->D + 2);
	}
	else {
    	fm_p->serv_p->size = PICTURE_SIZE*((Int32)fm_p->dafh_p->D + 2);
	}
	
	if ((fm_p->inFm.buf = (UInt8*) realloc(fm_p->inFm.buf, sizeof(UInt8)*fm_p->serv_p->size)) == NULL) return(MEDIA_ERR_MEM_ALLOC);
//	fm_p->inFm.max = fm_p->serv_p->size;
	fm_p->inFm.tsmax = fm_p->serv_p->size;
	DB_L1(" New input memory size: %ld\n", fm_p->inFm.tsmax);
		
    // Get the number of the next Intra picture to process and the type of operation
    MEDIA_ERR(err = getNextIntraPicturetoProcess(fm_p));

    if (fm_p->inFm.type == TS_TYPE) {
	    fm_p->accessType = READ_WRITE;
	    seqh_p->seqFlag = TRUE;
	}
    else {
	    fm_p->accessType = READ_ONLY;
	}

    fm_p->skipPicture   = FALSE;
	fm_p->lastStartCode = PICTURE_START_CODE_VALUE;
	fm_p->substitutionStarted = FALSE;
 	
 	
	while (endOfSequence == FALSE){
	    signal(SIGINT,ExceptFunction);

		err = findNextStartCode(fm_p,&fm_p->bufNb1,&tmByte,&nbBytes);
		// Exit issues
//		MEDIA_ERR(err = findNextStartCode(fm_p,&fm_p->bufNb1,&tmByte,&nbBytes));

        // Test if end of file reached without end of sequence
	    err = testEndProcess(fm_p,err);
		if (err == MEDIA_EOF) return(MEDIA_OK);

        // To synchronize to the first sequence header
 		if (tmByte == SEQUENCE_HEADER_CODE_VALUE) {
		    if (seqh_p->seqFlag == FALSE) {
		           MEDIA_ERR(err=putByte(&fm_p->outFm,0));
                   MEDIA_ERR(err=putByte(&fm_p->outFm,0));
                   MEDIA_ERR(err=putByte(&fm_p->outFm,1));
                   MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
				   fm_p->accessType = READ_WRITE;
            }
			seqh_p->seqFlag = TRUE;	// Sequence Header detected
		}

 	    if (fm_p->seqh_p->seqFlag == TRUE) {
		    switch (tmByte) {
			case TS_STREAM_ID_TYPE_E: 
				DB_L1(" PES TS Start Code    0x%x\n",tmByte);
				break;
			case SEQUENCE_HEADER_CODE_VALUE: 
				seqh_p->seqFlag = TRUE;	// Sequence Header detected
 				MEDIA_ERR(err = getSequenceParameters(fm_p,seqh_p));
				// Double buffer size computation
				tmSize = ( (seqh_p->hSize * seqh_p->vSize)/3 + 1 )*(fm_p->dafh_p->D + 2);
				DB_L1(" Double buffer size estimation %ld\n",tmSize);		
				DB_L1(" Actual buffer size            %ld\n",fm_p->serv_p->size);		
				DB_L1(" Sequence Header Code 0x%x\n",tmByte);		
				break;
			case SEQUENCE_ERROR_CODE_VALUE: 
				DB_L1(" Sequence Error Code  0x%x\n",tmByte);				
				break;
			case EXTENSION_START_CODE_VALUE: 
				DB_L1(" Extension Start Code 0x%x\n",tmByte);
				MEDIA_ERR(err = getExtensions(fm_p));		
				break;
			case GROUP_START_CODE_VALUE: 
				DB_L1(" Group Start Code     0x%x\n",tmByte);				
				break;
			case USER_DATA_START_CODE_VALUE: 
				DB_L1(" User Data Start Code 0x%x\n",tmByte);				
				break;
			case SEQUENCE_END_CODE_VALUE: 
				DB_L1(" Sequence End Code    0x%x\n",tmByte);				
				endOfSequence = TRUE;
				break;
			case PICTURE_START_CODE_VALUE: 
				DB_L1(" Picture Start Code   0x%x\n",tmByte);
				
				if (fm_p->substitutionStarted == TRUE) {
				    fm_p->substitutionStarted = FALSE;
					      // Get the number of the next Intra picture to process and the type of operation
                           err = getNextIntraPicturetoProcess(fm_p);
			               if (fm_p->serv_p->statusDataFile == DATA_FILE_EOS) {
				               // Try the retrieval process
	                           DB_L10(" getNextIntraPicturetoProcess, DATA_FILE_EOS\n");
//				               err = MEDIA_OK;
				           }
			    }

		        // Count the number of picture since the beginning
			    fm_p->stream_p->pictureCounter++;
				DB_L1(" Current Picture Number   %3.1f\n",fm_p->stream_p->pictureCounter);

                // Display the bitrate
		        if ( fm_p->seqex_p->progressive_sequence == 0 ) { // Interlaced sequence
				    if ( (UInt32)(fm_p->stream_p->pictureCounter)%(UInt32)(2.0*frameRateCode[fm_p->seqh_p->frameRate])==0 ) {
    		            fm_p->bitRate = ((fm_p->dataFm.byteCounter)*8.0)/fm_p->stream_p->pictureCounter;
#ifdef linux
                        if (dbgLevel == 0) system("clear");
#endif        
		                DB_L0(" Data  file bit rate (Kbits/s) = %5.1f\n",(float)(frameRateCode[fm_p->seqh_p->frameRate])*(fm_p->bitRate/1000.0) );
		                DB_L0(" Media file bit rate (Kbits/s) = %5.1f\n",((frameRateCode[fm_p->seqh_p->frameRate])*(fm_p->inFm.byteCounter*8.0)/((1000.0)*fm_p->stream_p->pictureCounter) ));
                    }
		        }
        		else { // Progressive sequence
					if ( (UInt32)(fm_p->stream_p->pictureCounter)%(UInt32)(frameRateCode[fm_p->seqh_p->frameRate])==0 ) {
    		            fm_p->bitRate = (frameRateCode[fm_p->seqh_p->frameRate])*((fm_p->dataFm.byteCounter*8.0)/fm_p->stream_p->pictureCounter);
#ifdef linux
                        if (dbgLevel == 0) system("clear");
#endif        
		                DB_L0(" Data  file bit rate (Kbits/s) = %5.1f\n",(fm_p->bitRate/1000.0) );
		                DB_L0(" Media file bit rate (Kbits/s) = %5.1f\n",( ((frameRateCode[fm_p->seqh_p->frameRate]))*(fm_p->inFm.byteCounter*8.0)/((1000.0)*fm_p->stream_p->pictureCounter) ));
                    }
		        }
		
				if (((UInt32)fm_p->stream_p->pictureCounter > fm_p->stream_p->b)&&(fm_p->stream_p->b!=-1)) {
	    		    if (fm_p->accessType == READ_WRITE) {
                        MEDIA_ERR(err=putByte(&fm_p->outFm,0));
			            MEDIA_ERR(err=putByte(&fm_p->outFm,0));
                        MEDIA_ERR(err=putByte(&fm_p->outFm,1));
				        MEDIA_ERR(err=putByte(&fm_p->outFm,SEQUENCE_END_CODE_VALUE));
                    }
				    DB_L0(" NUMBER OF PICTURES PROCESSED %3.1f\n",fm_p->stream_p->pictureCounter-1);
				    return(MEDIA_OK);
				}
				
				// Check if Intra Picture, always load the temporal ref and picture type into bufNb1
				if (fm_p->parseFlag == FALSE) fm_p->accessType = READ_LOAD;

				MEDIA_ERR(err = getPictureType(fm_p,&pictureType));
				 
				if (fm_p->skipPicture != TRUE)
					 fm_p->accessType = READ_WRITE;
					 
				if (fm_p->parseFlag == FALSE) {
	
					if (fm_p->pic_p->pictureType == I_PICTURE_TYPE) {
		       	       
			            DB_L1("    xp2   = %ld\n",pic_p->xp2);
			            DB_L1("    xc    = %ld\n",pic_p->xc);
			            DB_L1("    xpro  = %ld\n",pic_p->xpro);
				   
#ifndef __TCS__
				        if ((fm_p->serv_p->controlLevel == REMOTE_CONTROL)&&(fm_p->serv_p->statusDataFile == DATA_FILE_NOT_AVAILABLE)) {
			                fd_set sockets;
                            struct timeval timeout;
 
                            FD_ZERO(&sockets); 
                            FD_SET(fm_p->serv_p->dataSocket,&sockets);
                            timeout.tv_usec = 10000;
                            timeout.tv_sec  = 0;
//	                        MEDIA_ERR(err = clientDataConnect(fm_p));
//                            if (connect(fm_p->serv_p->controlSocket, (struct sockaddr *)fm_p->serv_p->controlServAddr_p, sizeof(struct sockaddr_in)) < 0) {
//                                perror("connect()");                            }
//	                        if (errno==EISCONN) fm_p->serv_p->statusDataFile = DATA_FILE_OK;      
                            if ( select(fm_p->serv_p->dataSocket + 1,  &sockets, NULL, NULL, &timeout) < 0 ) {
                                    perror("select()");
                            }
                            if (FD_ISSET(fm_p->serv_p->dataSocket, &sockets)) {
                                DB_L1("serveur connected tsbc = %ld",fm_p->dataFm.tsbc);
                                DB_L1(" tswr=%ld\n",fm_p->dataFm.tswr);
				                // reset the dataFm input buffer
//                                fm_p->dataFm.bc = fm_p->dataFm.tswr;
				                MEDIA_ERR(err=tsResetInputBuffer(&fm_p->dataFm));
                                fm_p->substitutionStarted = FALSE;
                                fm_p->serv_p->statusDataFile = DATA_FILE_OK;
                            }
                            else {
                                fm_p->serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE;
                                DB_L1("serveur DISCONNECTED pic number: xc =%ld\n",pic_p->xc);
                            }
				        }
#endif                            
			    									
					    if ((pic_p->xc == pic_p->xpro)&&(pic_p->opType == SUBSTITUTION)&&(dafh_p->S != 0)) {
							fm_p->substitutionStarted = TRUE;
							DB_L1(" fm_p->substitutionStarted %d\n",fm_p->substitutionStarted);
						}
      					
						// Store saved picture as the second picture place of a permutation operation
					    if ((pic_p->xc == pic_p->xp2)&&(dafh_p->X == FALSE)) {
							// remove the temporal ref and picture type from buf Nb1
							setBufferPointer(&fm_p->bufNb1, 2);
							DB_L3("    Enter store bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
					  		MEDIA_ERR(err=storePicture(fm_p)); // copy bufNb1 to the MPEG-2 output buffer
							// skip the current picture, i.e. read from file without writing
							DB_L3("    After store bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
							fm_p->accessType  = READ_LOAD;
							fm_p->skipPicture = TRUE;
      					}
      					else if ((pic_p->xc == pic_p->xpro)&&(pic_p->opType == PERMUTATION)&&(dafh_p->X == FALSE)) {
							MEDIA_ERR(err=permutePicture(fm_p));
							fm_p->accessType = READ_WRITE;
      					}
						else {
							DB_L3("    Intra No store No permutation bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);							
							// transfer the picture start code from bufNb1 into the outfile buffer	
							if (fm_p->skipPicture == TRUE) {
								DB_L2("    fm_p->skipPicture     0x%x\n",fm_p->skipPicture);
								setBufferPointer(&fm_p->bufNb1, 6);
								for (i=0;i<6;i++) {
									MEDIA_ERR(err=getByteMem(&fm_p->bufNb1, &tmByte));
    								MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    							}
								setBufferPointer(&fm_p->bufNb1, 6);

								// flush the skipped picture from bufNb1
								fm_p->bufNb1.bc   = 0;
								fm_p->accessType  = READ_WRITE;
								fm_p->skipPicture = FALSE;
							}
							else {
								// transfer the temporal ref and picture type from bufNb1 into the MPEG-2 output buffer	
								setBufferPointer(&fm_p->bufNb1, 2);
								for (i=0;i<2;i++) {
									MEDIA_ERR(err=getByteMem(&fm_p->bufNb1, &tmByte));
    								MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    							}
								// remove the temporal ref and picture type from temporary buffer
								setBufferPointer(&fm_p->bufNb1, 2);
							}
						}

					    // When substitution operation finished
	  				    if ((pic_p->xc > pic_p->xpro)&&(pic_p->opType == SUBSTITUTION)&&(dafh_p->S != 0)) {
					      // Get the number of the next Intra picture to process and the type of operation
                           err = getNextIntraPicturetoProcess(fm_p);
			               if (fm_p->serv_p->statusDataFile == DATA_FILE_EOS) {
				               // Try the retrieval process
//				               err = MEDIA_OK;
	                           DB_L10(" getNextIntraPicturetoProcess, DATA_FILE_EOS\n");
				           }
                        }
					    // When permutation operation finished
//	  				    else if ((pic_p->xc == pic_p->xp2)&&(pic_p->opType == PERMUTATION)&&(dafh_p->X == FALSE)) {
                        // To support level 2 control
	  				    else if ((pic_p->xc >= pic_p->xp2)&&(pic_p->opType == PERMUTATION)&&(dafh_p->X == FALSE)) {
					      // Get the number of the next Intra picture to process and the type of operation
                           err = getNextIntraPicturetoProcess(fm_p);
			               if (fm_p->serv_p->statusDataFile == DATA_FILE_EOS) {
				               // Try the retrieval process
//				               err = MEDIA_OK;
	                           DB_L10(" getNextIntraPicturetoProcess, DATA_FILE_EOS\n");
				           }
                        }
						
						if ((pic_p->xc == pic_p->xpro)&&(fm_p->pic_p->opType == SUBSTITUTION)) {
					        fm_p->substitutionStarted = TRUE;
	                        DB_L10(" fm_p->substitutionStarted = TRUE\n");
				        }

						pic_p->xc++; // increment current picture number
					}	
					else {
						// transfer the picture start code, temporal ref and picture type from bufNb1 into the MPEG-2 output buffer
						DB_L3("    Inter bufNb1 buffer status bc %ld\n",fm_p->bufNb1.bc);
						if (fm_p->skipPicture == TRUE) {
							DB_L2("    fm_p->skipPicture     0x%x\n",fm_p->skipPicture);
							setBufferPointer(&fm_p->bufNb1, 6);
							for (i=0;i<6;i++) {
								MEDIA_ERR(err=getByteMem(&fm_p->bufNb1, &tmByte));
    							MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    						}
							setBufferPointer(&fm_p->bufNb1, 6);

							// flush the skipped picture from bufNb1
							fm_p->bufNb1.bc   = 0;
							fm_p->accessType  = READ_WRITE;
							fm_p->skipPicture = FALSE;
						}
						else {
							// transfer the temporal ref and picture type from bufNb1 into the MPEG-2 output buffer	
							setBufferPointer(&fm_p->bufNb1, 2);
							for (i=0;i<2;i++) {
								MEDIA_ERR(err=getByteMem(&fm_p->bufNb1, &tmByte));
    							MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    						}
							// remove the temporal ref and picture type from temporary buffer
							setBufferPointer(&fm_p->bufNb1, 2);
						}
					}				
				}
				break;
			default:
				if ((tmByte >= VIDEO_STREAM_ID_MIN)&&(tmByte <= VIDEO_STREAM_ID_MAX)) {
				    // transfer the pes start code from bufNb1 into the MPEG-2 output buffer
					DB_L1(" PES Video Start Code 0x%x\n",tmByte);
						if (fm_p->skipPicture == TRUE) {
							DB_L2("    fm_p->skipPicture     0x%x\n",fm_p->skipPicture);
							setBufferPointer(&fm_p->bufNb1, 4);
							for (i=0;i<4;i++) {
								MEDIA_ERR(err=getByteMem(&fm_p->bufNb1, &tmByte));
    							MEDIA_ERR(err=putByte(&fm_p->outFm,tmByte));
    						}
							setBufferPointer(&fm_p->bufNb1, 4);

							// flush the skipped picture from bufNb1
							fm_p->bufNb1.bc   = 0;
							fm_p->accessType  = READ_WRITE;
							fm_p->skipPicture = FALSE;
						}
                }
			    else if ((tmByte >= AUDIO_STREAM_ID_MIN)&&(tmByte <= AUDIO_STREAM_ID_MAX)) {
    				DB_L1(" PES Audio Start Code 0x%x\n",tmByte);
			    }
				else if ((tmByte >= SLICE_START_CODE_VALUE_MIN)&&(tmByte <= SLICE_START_CODE_VALUE_MAX)) {
					DB_L1(" Slice Start Code     0x%x\n",tmByte);

					if (fm_p->skipPicture == TRUE) 
						DB_L2("    fm_p->skipPicture     0x%x\n",fm_p->skipPicture);
					if (fm_p->parseFlag == FALSE) {
						if (fm_p->substitutionStarted == TRUE) {
							if (tmByte == SLICE_START_CODE_VALUE_MIN) fm_p->bufNb2.bc = 0;
//							MEDIA_ERR(err = getNextIntraSliceData(fm_p,tmByte));
							err = getNextIntraSliceData(fm_p,tmByte);
							if (tmByte == PICTURE_START_CODE_VALUE) {
								DB_L1(" Picture Start Code   0x%x\n",tmByte);
                                // Count the number of picture since the beginning
			                    fm_p->stream_p->pictureCounter++;
				                DB_L1(" Current Picture Number   %3.3f\n",fm_p->stream_p->pictureCounter);
				                if ((fm_p->stream_p->pictureCounter > fm_p->stream_p->b)&&(fm_p->stream_p->b!=-1)) {
	    		                    if (fm_p->accessType == READ_WRITE) {
                                       MEDIA_ERR(err=putByte(&fm_p->outFm,0));
			                           MEDIA_ERR(err=putByte(&fm_p->outFm,0));
                                       MEDIA_ERR(err=putByte(&fm_p->outFm,1));
				                       MEDIA_ERR(err=putByte(&fm_p->outFm,SEQUENCE_END_CODE_VALUE));
                                    }
				                    DB_L0(" NUMBER OF PICTURES PROCESSED %3.3f\n",fm_p->stream_p->pictureCounter-1);
				                    return(MEDIA_OK);
				                }
				
			                	MEDIA_ERR(err = getPictureType(fm_p,&pictureType));
						    }
						}
					}								
				} 
				else {
					DB_L1(" Unidentified start code value 0x%x\n",tmByte);				
				}
		} // end of switch
	}
    else { //sequence header
		switch (tmByte) {
			case VIDEO_STREAM_ID_MIN: 
				DB_L1(" PES Video Start Code 0x%x\n",tmByte);
				break;
			case AUDIO_STREAM_ID_MIN: 
				DB_L1(" PES Audio Start Code 0x%x\n",tmByte);
				break;
			case TS_STREAM_ID_TYPE_E: 
				DB_L1(" PES TS Start Code    0x%x\n",tmByte);
				break;
			case SEQUENCE_HEADER_CODE_VALUE: 
				seqh_p->seqFlag = TRUE;	// Sequence Header detected
 				MEDIA_ERR(err = getSequenceParameters(fm_p,seqh_p));
				DB_L1(" Sequence Header Code 0x%x\n",tmByte);		
				break;
			case SEQUENCE_ERROR_CODE_VALUE: 
				DB_L1(" Sequence Error Code  0x%x\n",tmByte);				
				break;
			case EXTENSION_START_CODE_VALUE: 
				DB_L1(" Extension Start Code 0x%x\n",tmByte);
				break;
			case GROUP_START_CODE_VALUE: 
				DB_L1(" Group Start Code     0x%x\n",tmByte);				
				break;
			case USER_DATA_START_CODE_VALUE: 
				DB_L1(" User Data Start Code 0x%x\n",tmByte);				
				break;
			case SEQUENCE_END_CODE_VALUE: 
				DB_L1(" Sequence End Code    0x%x\n",tmByte);				
				endOfSequence = TRUE;
				break;
			case PICTURE_START_CODE_VALUE: 
				DB_L1(" Picture Start Code   0x%x\n",tmByte);
				break;
			default:
				if ((tmByte >= SLICE_START_CODE_VALUE_MIN)&&(tmByte <= SLICE_START_CODE_VALUE_MAX)) {
					DB_L1(" Slice Start Code     0x%x\n",tmByte);
				} 
				else {
					DB_L1(" Unidentified start code value 0x%x\n",tmByte);				
				}
		} // end of switch
    } // Sequence header story
	} // While

	MEDIA_ERR(err = flushBuffers(fm_p));	

	DB_L00("Program exited with no error\n");
  	return MEDIA_OK;

}
