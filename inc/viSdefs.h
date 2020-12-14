/***********************************************************************
 **
   Copyright (c) Tigress 2002. 2003. 2004
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
   Author: Brigitte Schuster
   Last update: 2002/06/29
   Modified:
   File: videfs.c
   History:
 		Version 0.1	 2002/06/29
 **
 ***********************************************************************/
#ifndef VISDEFS_H
#define VISDEFS_H

/***********************************************************************
 ** Video Picture Sizes
 */
#define visWIDTH_QCIF_UV	   88
#define visHEIGHT_PAL_QCIF_UV  72
#define visHEIGHT_NTSC_QCIF_UV 60
#define visWIDTH_QCIF	      176
#define visHEIGHT_PAL_QCIF	  144
#define visHEIGHT_NTSC_QCIF	  120
#define visWIDTH_CIF	      352
#define visWIDTH_CCIR		  704
#define visHEIGHT_PAL_CIF	  288
#define visHEIGHT_NTSC_CIF	  240


/***********************************************************************
 ** TriMedia Data Size
 */
#define CACHE_LINE_SIZE        64

/***********************************************************************
 ** TriMedia Video Out
 */
#define voHEIGHT_PAL		  288
#define voHEIGHT_NTSC		  240
#define voWIDTH_CIF		      352
#define voWIDTH_CCIR		  704
#define voSCANWIDTH_CIF		  384	/*  ceil(voWIDTH_CIF/64)*64  */
#define voSCANWIDTH_CCIR	  704	/*  ceil(viWIDTH_CCIR/64)*64 */

/***********************************************************************
 ** MPEG-4 SYSTEM CODES
 */
#define VIDEO_OBJECT_START_CODE_MIN         0x00
#define VIDEO_OBJECT_START_CODE_MAX         0x1F
#define VIDEO_OBJECT_LAYER_START_CODE_MIN   0x20
#define VIDEO_OBJECT_LAYER_START_CODE_MAX   0x2F
#define VO_SEQUENCE_HEADER_CODE_VALUE 		0xB0
#define VO_SEQUENCE_END_CODE_VALUE 		    0xB1
#define USER_DATA_START_CODE_VALUE 			0xB2
#define GROUP_OF_VOP_START_CODE_VALUE 		0xB3
#define VISUAL_OBJECT_START_CODE_VALUE 		0xB5
#define VOP_START_CODE_VALUE 		        0xB6
#define MPEG_PROGRAM_END_CODE_VALUE 		0xB9
#define PACK_START_CODE_VALUE 		        0xBA
#define SYSTEM_HEADER_START_CODE_VALUE 		0xBB
#define PROGRAM_STREAM_MAP_VALUE            0xBC
#define PRIVATE_STREAM_1_VALUE              0xBD
#define PADDING_STREAM_VALUE                0xBE
#define PRIVATE_STREAM_2_VALUE              0xBF
#define AUDIO_STREAM_ID_MIN                 0xC0
#define AUDIO_STREAM_ID_MAX                 0xDF
#define PACKET_ES_START_CODE_VALUE 			0xE0
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
#define TS_PACKET_SIZE                      188
#define VOB_PACKET_SIZE                     2048

/***********************************************************************
 ** MPEG-2 SYSTEM TRICK MODES
 */
#define FAST_FORWARD                        0
#define SLOW_MOTION                         1
#define FREEZE_FRAME                        2
#define FAST_REVERSE                        3
#define SLOW_REVERSE                        4

/***********************************************************************
 ** MPEG-2 VIDEO CODES
 */
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
#define START_CODE 		   					0x000001
#define START_CODE_LENGTH					24
#define SLICE_START_CODE 		   			0x00000101
#define SLICE_START_LENGTH		    		32
#define MBKSTUFFING_CODE 		    		15		/* 0b 000 0000 1111 */
#define MBKSTUFFING_LENGTH		    		11
/*  Quantizer scale code + extra bit slice */
#define SLICEQSCALE_CODE					2   	/* 0b000010 */
#define SLICEQSCALE_LENGTH		   			6
/* Macro-block address increment + Macro-block type Intra */
#define MACROBLOCK_CODE                     3		/* 0b11 */
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
#define T0_EOB_CODE 			    		2		/* When Mpeg-2 Table 0 */
#define T0_EOB_LENGTH 			    		2
#define T1_EOB_CODE 			    		6		/* When Mpeg-2 Table 1 */
#define T1_EOB_LENGTH 			    		4
#define FRAME_PICTURE						3

#define TS_VIDEO_TYPE                       2
#define TS_AUDIO_TYPE						3
#ifndef TS_PRIVATE_DATA_TYPE
#define TS_PRIVATE_DATA_TYPE				18
#endif

typedef enum mpgColourPrimaries {
    FORBIDDEN_VALUE                 = 0x00,
    COLOUR_PRIMARIES_BT709          = 0x01,
    COLOUR_PRIMARIES_NON_SPECIFIED  = 0x02,
    COLOUR_PRIMARIES_RESERVED       = 0x03,
    COLOUR_PRIMARIES_BT470M         = 0x04,
    COLOUR_PRIMARIES_BT470BG        = 0x05,
    COLOUR_PRIMARIES_BT170M         = 0x06,
    COLOUR_PRIMARIES_BT240M         = 0x07
} mpgColourPrimaries_t;



#endif
