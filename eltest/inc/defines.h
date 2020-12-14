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
// Creation: 2002/07/03                                                  
// File: defines.h
// History:
// 		Version 1.0                                                       
// 		Version 1.1	    2002/07/11      Brigitte Schuster
//                      Update with descrambler structures
// 		Version 1.2	    2002/07/22      Brigitte Schuster
//                      Update with server's #define
// 		Version 1.3	    2002/07/24      Brigitte Schuster
//                      Update with message structure
//
////////////////////////////////////////////////////////////////////////

#ifndef DEFINES_H
#define DEFINES_H

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

////////////////////////////////////////////////////////////////////////
// Portability management
//
#define UInt8   unsigned char
#define UInt16  unsigned int
#define UInt32  unsigned long
#define Int8    char
#define Int16   int
#define Int32   long
#define Char    char
#define Float   float
#define Double  double

#define	FALSE	0
#define	TRUE	1


////////////////////////////////////////////////////////////////////////
// MPEG-2 codes
//
//#define PACKET_ES_START_CODE_VALUE 			0xE0
#define SEQUENCE_HEADER_CODE_VALUE 			0xB3
#define PICTURE_START_CODE_VALUE 			0x00
#define PICTURE_START_CODE_LENGTH 			4
#define SEQUENCE_END_CODE_VALUE 			0xB7
#define SLICE_START_CODE_VALUE_MIN 			0x01
#define SLICE_START_CODE_VALUE_MAX 			0xAF
#define SEQUENCE_ERROR_CODE_VALUE			0xB4
#define EXTENSION_START_CODE_VALUE			0xB5
#define GROUP_START_CODE_VALUE				0xB8
#define USER_DATA_START_CODE_VALUE 			0xB2
#define MPEG_PROGRAM_END_CODE_VALUE 		0xB9
#define PACK_START_CODE_VALUE 		        0xBA
#define PROGRAM_STREAM_MAP_VALUE            0xBC
#define PRIVATE_STREAM_1_VALUE              0xBD
#define PADDING_STREAM_VALUE                0xBE
#define PRIVATE_STREAM_2_VALUE              0xBF
#define AUDIO_STREAM_ID_MIN                 0xC0
#define AUDIO_STREAM_ID_MAX                 0xDF
#define VIDEO_STREAM_ID_MIN                 0xE0
#define VIDEO_STREAM_ID_MAX                 0xEF
#define ECM_STREAM_ID                       0xF0
#define EMM_STREAM_ID                       0xF1
#define DSMCC_STREAM_ID                     0xF2
#define STREAM_ID_13522                     0xF3
#define TS_STREAM_ID_TYPE_A                 0xF4
#define TS_STREAM_ID_TYPE_B                 0xF5
#define TS_STREAM_ID_TYPE_C                 0xF6
#define TS_STREAM_ID_TYPE_D                 0xF7
#define TS_STREAM_ID_TYPE_E                 0xF8
#define ANCILLARY_STREAM_ID                 0xF9
#define TS_STREAM_ID_RES_MIN                0xFA
#define TS_STREAM_ID_RES_MAX                0xFE
#define PROGRAM_STREAM_ID_DIR               0xFF

#define SYNC_BYTE                           0x47

#define PAT_PID                             0x0000
#define CAT_PID                             0x0001
#define ANY_PID_MIN                         0x0010
#define ANY_PID_MAX                         0x1FFE
#define NULL_PID                            0x1FFF

#define START_CODE 		   					0x000001
#define START_CODE_LENGTH					24	
#define SLICE_START_CODE 		   			0x00000101	
#define SLICE_START_LENGTH		    		32
#define MBKSTUFFING_CODE 		    		15		// 0b 000 0000 1111
#define MBKSTUFFING_LENGTH		    		11
//  Quantizer scale code + extra bit slice 
#define SLICEQSCALE_CODE					2   	// 0b000010	
#define SLICEQSCALE_LENGTH		   			6	
// Macro-block address increment + Macro-block type Intra
#define MACROBLOCK_CODE                     3		// 0b11
#define MACROBLOCK_LENGTH					2
#define ESCAPE_CODE				 			1
#define ESCAPE_LENGTH 			    		6
#define LUMA_DCSIZE_CODE					4
#define LUMA_DCSIZE_LENGTH 					3
#define CHROMA_DCSIZE_CODE					0
#define CHROMA_DCSIZE_LENGTH 				2
#define	RUN_0_CODE							0
#define	RUN_0_LENGTH						6
#define	LEVEL_LENGTH						12
#define T0_EOB_CODE 			    		2		// When Mpeg-2 Table 0
#define T0_EOB_LENGTH 			    		2
#define T1_EOB_CODE 			    		6		// When Mpeg-2 Table 1
#define T1_EOB_LENGTH 			    		4
#define FRAME_PICTURE						3

////////////////////////////////////////////////////////////////////////
// Definition of Default Values
//
#define M_DEFAULT 							1200
#define m_DEFAULT 							10
#define W_DEFAULT 							7
#define D_DEFAULT 							6
#define IN_NAME_DEFAULT						"test.m2v"      // scrambler
#define DESC_IN_NAME_DEFAULT				"media_test.m2v"// descrambler
#define DESC_OUT_NAME_DEFAULT				"dec_test.m2v"  // descrambler
#define DESC_LIST_NAME_DEFAULT              "no_list"
#define DESC_SERV_NAME_DEFAULT              "file"
//#define BUFFER_SIZE 						1024		     // Input and output buffers
#define TS_PACKET_SIZE                      188              // Transport Stream packet size
#define BUFFER_SIZE 						(6*TS_PACKET_SIZE) // Input and output buffers = 6*TS_PACKET_SIZE
#define PICTURE_SIZE 						(800*BUFFER_SIZE)  // GOP size
#define SMALL_BUFFER_SIZE                   256              // client-server commands


////////////////////////////////////////////////////////////////////////
// User Interface
//
#define MEDIA_PROMPT					    DB_L00("Media> ")
#define	MAXLINE                             512


////////////////////////////////////////////////////////////////////////
// Server
//
#define	SERV_TCP_MEDIA_PORT                 6058
#define	SERV_TCP_UI_PORT                    6059
#define	SERV_TCP_DATA_PORT                  6060
#define	SERV_TCP_CONTROL_PORT               6061
//#define	SERV_HOST_ADDR	                    "192.168.0.100" // host address for the server
#define	SERV_HOST_ADDR	                    "192.168.0.44" // host address for the server


#ifndef __TCS__
#ifdef LITTLE_ENDIAN
	#define htonl(addr)     \
	            ( (((unsigned long)(addr) & 0x000000FF)<<24) | \
				  (((unsigned long)(addr) & 0x0000FF00)<<8)  | \
				  (((unsigned long)(addr) & 0x00FF0000)>>8)  | \
				  (((unsigned long)(addr) & 0xFF000000)>>24))
	
	#define ntohl(addr)     htonl(addr)
	
	#define htons(addr)     ( (((unsigned short)(addr) & 0x000000FF)<<8)  | \
				  (((unsigned short)(addr) & 0x0000FF00)>>8))
	
	#define ntohs(addr)     htons(addr)
#else
	#define htonl(a)       ((unsigned long)(a))
	#define ntohl(a)       ((unsigned long)(a))
	#define htons(a)       ((unsigned short)(a))
	#define ntohs(a)       ((unsigned short)(a))
#endif
#endif


////////////////////////////////////////////////////////////////////////
// Data file status
//
#define	DATA_FILE_OK	                     0x00
#define	DATA_FILE_EOS	                     0x01
#define DATA_FILE_NOT_AVAILABLE              0x02
/*
#define MEDIA_DATA_FILE(a)                                      \
    (a);                                                        \
	if ( err == MEDIA_EOF ) { 						            \
		DB_L10(" End of data stream \n"); 	            \
	    fm_p->serv_p->statusDataFile = DATA_FILE_EOS;           \
		return(MEDIA_EOF);                            \
	}                                                           \
	if ( err == MEDIA_WRN_DATA_FILE ) { 						\
		DB_L10(" Data stream not available \n"); 	    \
	    fm_p->serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE; \
		return(MEDIA_WRN_DATA_FILE);                            \
	}                                                           \
	if ( err & MEDIA_ERR_BASE ) { 						        \
		DB_L1(" Program exited with code 0x%x\n",err); \
		exit(err);                                              \
	}                                                           \
	if ( err & MEDIA_WRN_BASE ) { 						        \
		DB_L1(" Warning code 0x%x\n",err); 	        \
	}
*/
/*
#define MEDIA_DATA_FILE(a)                                      \
    (a);                                                        \
	if ( err == MEDIA_EOF ) { 						            \
		fprintf(PROUT,"End of data stream %s line %d\n",__FILE__,__LINE__); 	\
	    fm_p->serv_p->statusDataFile = DATA_FILE_EOS;           \
		return(MEDIA_EOF);                            \
	}                                                           \
	if ( err == MEDIA_WRN_DATA_FILE ) { 						\
		fprintf(PROUT,"Data stream not available %s line %d\n",__FILE__,__LINE__); 	\
	    fm_p->serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE; \
		return(MEDIA_WRN_DATA_FILE);                            \
	}                                                           \
	if ( err & MEDIA_ERR_BASE ) { 						        \
		fprintf(PROUT," Program exited with code 0x%x\n",err); \
		exit(err);                                              \
	}                                                           \
	if ( err & MEDIA_WRN_BASE ) { 						        \
		fprintf(PROUT," Warning code 0x%x\n",err); 	        \
	}
*/

#define MEDIA_DATA_FILE(a)                                      \
    (a);                                                        \
	if ( err == MEDIA_EOF ) { 						            \
	    fm_p->serv_p->statusDataFile = DATA_FILE_EOS;           \
		return(MEDIA_EOF);                                      \
	}                                                           \
	if ( err == MEDIA_WRN_DATA_FILE ) { 						\
	    fm_p->serv_p->statusDataFile = DATA_FILE_NOT_AVAILABLE; \
		return(MEDIA_WRN_DATA_FILE);                            \
	}                                                           \
	if ( err & MEDIA_ERR_BASE ) { 						        \
		exit(err);                                              \
	}                                                           \
	if ( err & MEDIA_WRN_BASE ) { 						        \
	}



////////////////////////////////////////////////////////////////////////
// Debug level printout
//
#ifdef linux
#define PROUT stderr
#else
#define PROUT stdout
#endif

#ifndef MEDIA_RETAIL
#define DB_L00(a)  if (dbgLevel >= 0) fprintf(PROUT,(a));
#define DB_L0(a,b) if (dbgLevel >= 0) fprintf(PROUT,(a),(b));
#define DB_L1(a,b) if (dbgLevel >= 1) fprintf(PROUT,(a),(b));
#define DB_L10(a)  if (dbgLevel >= 1) fprintf(PROUT,(a));
#define DB_L2(a,b) if (dbgLevel >= 2) fprintf(PROUT,(a),(b));
#define DB_L20(a)  if (dbgLevel >= 2) fprintf(PROUT,(a));
#define DB_L3(a,b) if (dbgLevel >= 3) fprintf(PROUT,(a),(b));
#define DB_L30(a)  if (dbgLevel >= 3) fprintf(PROUT,(a));
#define DB_L4(a,b) if (dbgLevel >= 4) fprintf(PROUT,(a),(b));
#define DB_L40(a)  if (dbgLevel >= 4) fprintf(PROUT,(a));
// For buffer management functions
/*
#define DB_BUF00(a)                            \
    if (dbgLevel >= 0) {                       \
        if (buf_p == &buf_p->fm_p->inFm) {     \
	        fprintf(PROUT,"inFm  ");           \
	    }                                      \
        if (buf_p == &buf_p->fm_p->dataFm) {   \
	        fprintf(PROUT,"dataFm  ");         \
	    }                                      \
        if (buf_p == &buf_p->fm_p->outFm) {    \
	        fprintf(PROUT,"outFm  ");          \
	    }                                      \
	    fprintf(PROUT,(a));                    \
	}                                          
#define DB_BUF0(a,b) if (dbgLevel >= 0) fprintf(PROUT,(a),(b));
#define DB_BUF1(a,b) if (dbgLevel >= 1) fprintf(PROUT,(a),(b));
#define DB_BUF10(a)  if (dbgLevel >= 1) fprintf(PROUT,(a)); */
#endif

////////////////////////////////////////////////////////////////////////
// Structures Definition
//

// TS header structure
typedef struct {
	UInt8          *hdr_p;          // TS header for video
	UInt32          id;             // TS header number
	UInt8           rdSignal;       // tswrite() to signal the header has been read
    void		   *nexthdr_p;      // Pointer to the next TS header in the list
} tsHeader_t, *tsHeader_p;

// TS packet structure
typedef struct {
	UInt8          *pack_p;         // TS packet pointer
	UInt32          packCounter;    // TS packet number
    void		   *nextPacket_p;   // Pointer to the next TS packet in the list
} tsPacket_t, *tsPacket_p;

// Structure for TS PMT parameters retrieved from the MPEG-2 system stream
typedef struct{
	UInt32 			    table_id;
	UInt32 			    section_syntax_indicator;
	UInt32 			    section_length;
	UInt32 			    program_number;
	UInt32 			    version_number;
	UInt32 			    current_next_indicator;
	UInt32 			    section_number;
	UInt32 			    last_section_number;
	UInt32              PCR_PID;
	UInt32              program_info_length;
	UInt32              continuity_indicator; // initial value of the video packet
	                                          // continuity indicator
	UInt32				video_valid;
} program_map_table_t, *program_map_table_p;

// Structure for TS PAT parameters retrieved from the MPEG-2 system stream
typedef struct{
	UInt32 			    table_id;
	UInt32 			    section_syntax_indicator;
	UInt32 			    section_length;
	UInt32 			    transport_stream_id;
	UInt32 			    version_number;
	UInt32 			    current_next_indicator;
	UInt32 			    section_number;
	UInt32 			    last_section_number;
	UInt32              network_PID;
	UInt32              program_map_PID;
	UInt32              video_PID;
	UInt32              audio_PID;
	UInt32              pat_defined;
	UInt32              pmt_defined;
} program_association_table_t, *program_association_table_p;

// Structure for sequence header parameters retrieved from the MPEG-2 file
typedef struct {
  	UInt16 	hSize;          // horizontal size value of a picture
  	UInt16 	vSize;          // vertical size value of a picture
  	UInt16 	aspectRatio;    // aspect ratio of a picture
  	UInt16 	frameRate;      // frame rate code
  	UInt16 	bitRate;        // bit rate value
	UInt8	seqFlag;		// Sequence Header detected
} seqHeader_t, *seqHeader_p;  

// Picture coding Extension Structure
typedef struct{
	UInt32 			f_code[2][2];
	UInt32 			intra_dc_precision;
	UInt32 			picture_structure;
	UInt32 			top_field_first;
	UInt32 			frame_pred_frame_dct;
	UInt32			concealment_motion_vectors;
	UInt32			q_scale_type;
	UInt32 			intra_vlc_format;
	UInt32			alternate_scan;
	UInt32 			repeat_first_field;
	UInt32 			chroma_420_type;
	UInt32 			progressive_frame;
	UInt32 			composite_display_flag;
	UInt32 			v_axis;
	UInt32 			field_sequence;
	UInt32 			sub_carrier;
	UInt32 			burst_amplitude;
	UInt32 			sub_carrier_phase;
} picture_coding_extension_t, *picture_coding_extension_p;

// Sequence Extension Structure
typedef struct{
	UInt32 			profile_and_level;
	UInt32 			progressive_sequence;		
	UInt32 			chroma_format;		// chrominance format either 1(4:2:0), 2(4:2:2) or 3(4:4:4)
	UInt32 			horizontal_size_ext;		
	UInt32 			vertical_size_ext;		
	UInt32 			bit_rate_ext;		
	UInt32 			vbv_buffer_size_ext;		
	UInt32 			low_delay;		
	UInt32 			frame_rate_ext_n;		
	UInt32 			frame_rate_ext_d;		
} sequence_extension_t, *sequence_extension_p;

// Structure for the data file header
typedef struct{
    char   *mediaFileName;   // useful for the descrambler, name of the 
	                         //  original mpeg-2 file
  	UInt32 M; 		         // mean value of the distance between two key 
                             //  frames (i.e. Intra ) to be replaced with a 
                             //  randomly generated picture.
  	UInt32 m; 		         // variation range of M.
  	UInt32 W; 		         // mean value of the distance between two key 
                             // frames (i.e. Intra) to be permuted with another 
                             // key frame.
  	UInt32 D;  		         // distance between two Intra frames to be swapped.
  	Int32  S;                // control the substitution operation
  	UInt8  X;                // disables the permutation operation
  	UInt8  parseFlag;        // useful for the descrambler, parseFlag of the 
	                         //  original mpeg-2 file
} dataFileHeader_t, *dataFileHeader_p;

// Structure and data definition to control the picture processing.
enum opTypeList {
	SUBSTITUTION 		= 0,								
	PERMUTATION 		= 1,
	NO_OPERATION 		= 2
};
typedef enum opTypeList opType;

// Structure and data definition to control the io buffers processing.
enum accessTypeList {
	READ_ONLY 			= 0,	// read content - parse only							
	READ_WRITE 			= 1,    // load content and write into a file
	READ_LOAD 			= 2     // load content into memory
};
typedef enum accessTypeList accessType;

// Picture type definition 
enum pictureTypeList {
	I_PICTURE_TYPE 		= 1,
	P_PICTURE_TYPE 		= 2,
	B_PICTURE_TYPE 		= 3,
	D_PICTURE_TYPE 		= 4
};
typedef enum pictureTypeList picType;

// Service type definition 
enum serviceTypeList {
	TCP_TYPE 		    = 0,
	FILE_TYPE 		    = 1
//	UDP_TYPE 		    = 0,
};
typedef enum serviceTypeList serviceType;

// Control level type definition 
enum controlTypeList {
	NO_CONTROL 		    = 0,
	LOCAL_CONTROL 		= 1,
	REMOTE_CONTROL 	    = 2
};
typedef enum controlTypeList controlType;

// Function type definition 
enum functionTypeList {
	SCRAMBLER 		    = 0,
	DESCRAMBLER 		= 1,
	SERVER              = 2
};
typedef enum functionTypeList functionType;

// Message type definition : 256 codes possible
enum messageTypeList {
	MSG_UI_START 		= 0,  // User Interface start command
	MSG_STATUS 		    = 1,
	MSG_GET_LIST        = 2,
	MSG_DATA_HEADER     = 3,
	MSG_DATA_CONTENT    = 4,
	MSG_FILES_LIST      = 5,  
	MSG_UI_FILE_NAME    = 6,  // User Interface file name
	MSG_UI_QUIT         = 7,  // User Interface quit command
	MSG_UI_CATEGORY     = 8,  // User Interface file category
	MSG_UI_RIGHTS       = 9   // User Interface file access rights
};
typedef enum messageTypeList messageType;

// Server status definition 
enum statusList {
	IDLE 		    = 0,
	START 		    = 1,
	CONNECTED 		= 2,
	QUIT 		    = 3
};
typedef enum statusList Status;

// Stream type definition 
enum streamTypeList {
	VIDEO_TYPE 	    = 0,
	TS_TYPE         = 1
};
typedef enum streamTypeList streamType;

////////////////////////////////////////////////////////////////////////
//
// GENERAL STRUCTURES
//

// Structure to track counting all the stream position elements
typedef struct {
	Int32			groupCounter;
	Int32			IPictureCounter;
	Double			pictureCounter;           // Counts the number of pictures (all type, I,P,B) since the beginning
	Int32           a;                        // Lower picture boundary
	Int32           b;                        // Upper picture boundary
	Int32			sliceCounter;
	Int32			macroblockCounter;
	Int32			blockCounter;
} streamManager_t, *streamManager_p;

// Stream position structure
typedef struct {
	Int32           bc;             // current index of the payload
	Int32           max;            // max index of the buffer (actual memory size of the payload)
	Int32           tsbc;           // current index of the input buffer
	Double          packetCounter;  // number of TS packets processed
} streamPosition_t, *streamPosition_p;

// Structure for managing buffers and files
typedef struct {
  	Char* 			name; 			// name of the file attached to the buffer
  	FILE* 			fiP;  			// pointer to the binary file
  	UInt8*			buf; 			// buffer for the io of the file
  	Int32 			bc;            	// current index of the payload 
  	Int32 			max;            // max index of the buffer (actual memory size of the payload)
	Int32 			tsbc;           // current index of the input buffer 
  	Int32 			tsmax;          // max index of the payload buffer (memory size allocated for the buffer in bytes)
	Int32           tswr;           // write pointer of the buffer
	streamType      type;           // type of media file
	UInt8           suspend;        // suspend tsWrite while reading TS packets
	UInt8           toggle;         // the write pointer turned around
    tsHeader_p      tshdr_p;        // current TS header structure
  	UInt16 			bitsLeft;		// amount of free bits left in a byte
  	Int32 			errBytes;		// amount of bytes left to generate to reach the target number of bytes for the picture
  	UInt8 			curByte;		// current byte being processed
  	UInt16 			nbCoeffs;		// amount of vlc coefficient per block to generate
  	UInt32 		    picSize; 		// actual size in bytes of the resident picture
	Double          byteCounter;    // count the number of bytes read from the buffer since program start or wraparound
	Double          packetCounter;  // count the number of TS packets processed
    void            *fm_p;          // pointer to the parent session manager
} fileManager_t, *fileManager_p;

////////////////////////////////////////////////////////////////////////
//
// USER INTERFACE SPECIFIC STRUCTURES
//

// Structure for managing the user display
typedef struct {
    Char**              disFiles;          // list of files to display
    Char*               disCategory0;      // category A to display
    Char*               disCategory1;      // category B to display
    Char*               disCategory2;      // category C to display
    Char*               disCategory3;      // user display
    Char*               disCategory4;      // user display
    Char*               disCategory5;      // user display
    Char*               disCategory6;      // user display
    Char*               disCategory7;      // user display
    void               *uiman_p;           // pointer to the parent UI session manager
} userStringManager_t, *userStringManager_p;

// Structure for managing buffers and files
typedef struct {
  	Char* 			    inFileName; 	    // name of the file attached to the pointer
  	FILE* 			    inf_p;  	        // pointer to the binary file
  	Char* 			    dataFileName; 	    // name of the file attached to the pointer
  	FILE* 			    dataf_p;  		    // pointer to the binary file
    UInt8               rights;             // access rights
//    Char**              streamFiles;        // list of files to retrieve from server
    Char*               category;           // category of content retrieval (A, B or C)
    void               *uiman_p;            // pointer to the parent UI session manager
} userFileManager_t, *userFileManager_p;

// Structure to set-up the service enviromment
typedef struct {
	Char*	            listName;		     // list of files file name
  	FILE* 			    listf_p;  	         // pointer to the list of files file
  	Char* 			    inFilePath;          // directory path where the list of files file is stored
  	FILE* 			    curFile;  	         // pointer to the downloaded media file
  	userFileManager_t 	uiFm;  		         // 
    Char*               ipAddress;           // IP address of the server resident machine
    UInt8*              sndbuf_p;            // send buffer pointer
    UInt8*              rcvbuf_p;            // receive buffer pointer    
    UInt16              mediaport;           // port for mpeg-2 stream download
    Int16               mediaSocket;         // socket for mpeg-2 stream download
    struct sockaddr_in *mediaServAddr_p;     // server address structure pointer
    UInt16              port;                // port for user commands 
    Int16               socket;              // socket for user commands
    struct sockaddr_in *servAddr_p;          // server address structure pointer
    Status              state;               // user interface status
    Int16               streamNb;            // nb of available files
    Int16               curStream;           // current stream index in the list of files
    Char**              streamFiles;         // list of files to retrieve from server
//    Char**              disFiles;            // list of files to display
//    Char**              disCategory;         // category to display
    userStringManager_p uistr_p;
    UInt8               ifLinux;
    Char*               streamCategory;
} userManager_t, *userManager_p;

////////////////////////////////////////////////////////////////////////
//
// DESCRAMBLER SPECIFIC STRUCTURES
//

// Structure to control the display
typedef struct {
  	UInt8  		        T; 			         // TRUE: enable transfer to an add-on board
  	UInt32  		    A; 			         // physical memory base address of the add-on board
} sharedMemManager_t, *sharedMemManager_p;


// Structure to set-up the service enviromment
typedef struct {
  	serviceType         serviceType;         // matches the serviceName user parameter
	controlType         controlLevel;        // level of control between the server and the descrambler
	Char               *serviceName;         // user parameter udp/file
  	UInt32 			    size;                // size of the media port receive buffer.
    UInt8              *sndbuf_p;            // send buffer pointer
    UInt8              *rcvbuf_p;            // receive buffer pointer
    UInt8               statusDataFile;      // data file availability
    UInt8               statusConnection;    // data connection availability
    UInt16              controlport;         // port for data stream control
    Int16               controlSocket;       // socket for data stream control
    struct sockaddr_in *controlServAddr_p;   // server address structure pointer
    UInt16              dataport;            // port for data stream retrieval
    Int16               dataSocket;          // socket for data stream retrieval
    struct sockaddr_in *dataServAddr_p;      // server address structure pointer
    UInt16              mediaport;           // port for mpeg-2 stream retrieval
    Int16               mediaSocket;         // socket for mpeg-2 stream retrieval
    struct sockaddr_in *mediaServAddr_p;     // server address structure pointer
    userManager_p       uiman_p;
    UInt32              bufferSize;          // size of the input buffer
} serviceManager_t, *serviceManager_p;


////////////////////////////////////////////////////////////////////////
//
// SERVER SPECIFIC STRUCTURES
//

// Structure for the server to manage the client
typedef struct {
    Int16               dataStatus;          // data stream status on client side
    Int16               controlSocket;       // socket for control data stream retrieval
    struct sockaddr_in *controlClientAddr_p; // server address structure pointer
    Int16               dataSocket;          // socket for data stream retrieval
    struct sockaddr_in *dataClientAddr_p;    // server address structure pointer
    Int16               mediaSocket;         // socket for mpeg-2 stream
    struct sockaddr_in *mediaClientAddr_p;   // server address structure pointer
    Int16               uiSocket;            // socket for the client user interface
    struct sockaddr_in *uiClientAddr_p;      // server address structure pointer
} clientManager_t, *clientManager_p;

// Session structure for the server 
typedef struct {
    UInt16              controlport;         // port for the data stream retrieval
    Int16               controlSocket;       // socket for data stream retrieval
    struct sockaddr_in *controlServAddr_p;   // server address structure pointer
    UInt16              dataport;            // port for the data stream retrieval
    Int16               dataSocket;          // socket for data stream retrieval
    struct sockaddr_in *dataServAddr_p;      // server address structure pointer
    UInt16              mediaport;           // port for the mpeg-2 stream
    Int16               mediaSocket;         // socket for mpeg-2 stream
    struct sockaddr_in *mediaServAddr_p;     // server listening address structure pointer
    UInt16              uiport;              // port for the client user interface
    Int16               uiSocket;            // socket for the client user interface
    struct sockaddr_in *uiServAddr_p;        // server address structure pointer
  	Char* 			    inFilePath;          // directory path where the input MPEG-2 file is stored
    UInt32              bufferSize;          // size of the media stream buffer
	controlType         controlLevel;        // level of control between the server and the descrambler (tests)
    clientManager_p     client_p;            // client structure
  	fileManager_t 	    inFm;  		         // file manager for the input mpeg-2 file
  	fileManager_t 	    dataFm;  		     // file manager for the input data file
	fileManager_t	    bufNb1;		         // temporary storage
	accessType		    accessType;	         // type of operation to process on buffers (read and write, read and load, read only)
    UInt8               rights;              // access rights
    Char*               category;            // category of content retrieval (A, B or C)
    Status              state;               // current server state
    UInt32              serverPicNumber;     // latest Intra picture retrieved from data file
    UInt32              clientPicNumber;     // client current Intra picture number
    Char*               ipAddress;           // IP address of the server resident machine
    struct hostent     *hostname_p;
    UInt8               processDataFileRunning;
} serverManager_t, *serverManager_p;


////////////////////////////////////////////////////////////////////////
//
// SCRAMBLER AND DESCRAMBLER SPECIFIC STRUCTURES
//

// Structure describing the whole session 
typedef struct{
  	UInt32  		xc; 			// current picture being read
  	UInt32  		xpro; 			// number of the next intra picture process
  	UInt32  		xs; 			// number of the next intra picture to substitute
  	UInt32  		xp; 			// number of the next intra picture to permute 
  	UInt32  		xp2; 			// number of the intra picture to permute with xs
  	opType  		opType;         // operation type: No operation, substitution, permutation
	picType         pictureType;    // picture type of the current picture: Intra, predictive, bi-directional
} picManager_t, *picManager_p;

// Structure describing the whole session 
typedef struct{
  	fileManager_t 	 inFm;  		// file manager for the input file
  	fileManager_t 	 outFm;			// file manager for the output file
  	fileManager_t 	 dataFm;		// file manager for the data file
	fileManager_t	 bufNb1;		// temporary storage of the first picture in a permutation
	fileManager_t	 bufNb2;		// temporary storage of the previous picture
	UInt8            storeBufNb2;   // flag to control the load of the previous picture into bufNb2 (case S -1)
	accessType		 accessType;	// type of operation to process on buffers (read and write, read and load, read only)
    UInt8			 lastStartCode; // Last start code after a slice substitution
    UInt8			 substitutionStarted; // if TRUE, the substitution process has started, the start code is retrieved
	UInt8 			 parseFlag;		// if TRUE, only parse the input stream, do not process
	UInt8			 skipPicture;	// if TRUE, picture needs to be skipped in a permutation operation.
	UInt8            hiSliceValue;  // highest slice number value during a session
  	Char* 			 inFilePath;    // directory path where the input MPEG-2 file is stored
    Double           bitRate;       // average bitrate (bps) of the data file
	functionType     function;      // type of function operated by the software (SCRAMBLER or DESCRAMBLER)
	picManager_p	 pic_p;			// picture Manager pointer
	dataFileHeader_p dafh_p;		// data File header pointer
	streamManager_p  stream_p;      // stream manager structure pointer
	sharedMemManager_p shared_p;    // shared memory transfer options structure pointer
	serviceManager_p serv_p;        // service structure pointer
	seqHeader_p		 seqh_p;		// sequence header pointer
	picture_coding_extension_p pce_p; // picture coding extension structure pointer
	sequence_extension_p seqex_p; 	// sequence extension structure pointer
	program_map_table_p pmt_p;      
    program_association_table_p  pat_p;
} session_t, * session_p;


#endif
