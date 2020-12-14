/***********************************************************************
 **
   Copyright (c) Tigress 2004. 							             
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
   Last update: 2004/06/23                                                  
   Modified:                                                   
   File: viSptscheck.c
   History:
 		Version 0.1	 2004/06/23
 **						
 ***********************************************************************/					

#include "viSdefs.h"
#include "tmNxTypes.h"
#include "tmLibappErr.h"

/***********************************************************************
 ** Debug level printout
 */
#define PROUT stderr

/***********************************************************************
 ** Global variables
 */
static UInt8 dbgLevel = 0;

#ifndef VIS_RETAIL
#define DB_L00(a)  if (dbgLevel >  0) fprintf(PROUT,(a));
#define DB_L0(a,b) if (dbgLevel >  0) fprintf(PROUT,(a),(b));
#define DB_L1(a,b) if (dbgLevel >  1) fprintf(PROUT,(a),(b));
#define DB_L10(a)  if (dbgLevel >  1) fprintf(PROUT,(a));
#define DB_L2(a,b) if (dbgLevel >  2) fprintf(PROUT,(a),(b));
#define DB_L20(a)  if (dbgLevel >  2) fprintf(PROUT,(a));
#define DB_L3(a,b) if (dbgLevel >  3) fprintf(PROUT,(a),(b));
#define DB_L30(a)  if (dbgLevel >  3) fprintf(PROUT,(a));
#define DB_L4(a,b) if (dbgLevel >  4) fprintf(PROUT,(a),(b));
#define DB_L40(a)  if (dbgLevel >  4) fprintf(PROUT,(a));
#endif

#define MINUTES 5

/***********************************************************************
 ** MPEG-2 PROGRAM STREAM PACK HEADER STRUCTURE
 */

typedef struct{
	UInt32				SCR_base_msb;
	UInt32 			    SCR_base;
	UInt32 			    SCR_extension;
	double				SCR; // SCR = (double)(SCR_base * 300 + (double)(SCR_base_msb << 31) * 2.0 * 300 + SCR_extension)
	UInt32 			    program_mux_rate;
	UInt32 			    pack_stuffing_length;
} pack_header_t, *pack_header_p;

/***********************************************************************
 ** MPEG-2 PROGRAM STREAM SYSTEM HEADER STRUCTURE
 */
typedef struct{
	UInt32 			    header_length;
	UInt32 			    rate_bound;
	UInt32              audio_bound;
	UInt32 			    CSPS_flag;
	UInt32 			    system_audio_lock_flag;
	UInt32 			    system_video_lock_flag;
	UInt32 			    video_bound;
	UInt32 			    packet_rate_restriction_flag;
	UInt32              stream_id; //FIXTHIS are there several stream ids?
	UInt32              P_STD_buffer_bound_scale;
	UInt32              P_STD_buffer_size_bound;
} system_header_t, *system_header_p;

/***********************************************************************
 ** MPEG-2 PROGRAM STREAM MAP TABLE
 */
typedef struct {
	UInt32          el_PID;       /* elementary stream PID */
    void		   *nextProg_p;   /* Pointer to the next program number in the list */
} program_number_t, *program_number_p;

typedef struct{
	UInt32 			    map_stream_id;
	UInt32 			    program_stream_map_length;
	UInt32 			    current_next_indicator;
	UInt32 			    program_stream_map_version;
	UInt32 			    program_stream_info_length;
	UInt32 			    elementary_stream_map_length;
	UInt32 			    stream_type;
	UInt32 			    elementary_stream_id;
	UInt32 			    elementary_stream_info_length;
	UInt32              CRC_32;
} program_map_table_t, *program_map_table_p;

/***********************************************************************
 ** PES header
 */
typedef struct{
	UInt32 			    stream_id;
	UInt32 			    PES_packet_length;
	UInt32 			    PES_scrambling_control;
	UInt32 			    PES_priority;
	UInt32 			    data_alignment_indicator;
	UInt32 			    copyright;
	UInt32 			    original_or_copy;
	UInt32 			    PTS_DTS_flags;
	UInt32              ESCR_flag;
	UInt32              ES_rate_flag;
	UInt32              DSM_trick_mode_flag;
	UInt32              additional_copy_info_flag;
	UInt32              PES_CRC_flag;
	UInt32              PES_extension_flag;
	UInt32              PES_header_data_length;
	UInt32              PTS_hi;
	UInt32              PTS_lo;
	double				PTS;
	UInt32              DTS_hi;
	UInt32              DTS_lo;
	double				DTS;
	UInt32              ESCR_base;
	UInt32              ESCR_extension;
	UInt32              ES_rate;
	UInt32              trick_mode_control;
	UInt32              field_id;
	UInt32              intra_slice_refresh;
	UInt32              frequency_truncation;
	UInt32              rep_cntrl;
	UInt32              additional_copy_info;
	UInt32              previous_PES_packet_CRC;
	UInt32              PES_private_data_flag;
	UInt32              pack_header_field_flag;
	UInt32              program_packet_sequence_counter_flag;
	UInt32              P_STD_buffer_flag;
	UInt32              PES_extension_flag_2;
	UInt32              PES_private_data;
	UInt32              pack_field_length;
	UInt32              program_packet_sequence_counter;
	UInt32              MPEG1_MPEG2_identifier;
	UInt32              original_stuff_length;
	UInt32              P_STD_buffer_scale;
	UInt32              P_STD_buffer_size;
	UInt32              PES_extension_field_length;
} pes_header_t, *pes_header_p;

typedef tmLibappErr_t viSerror_t;
#define VOB_PES_HEADER_LENGTH                  22 /* PREVIOUS_CRC, DTS and PTS are present, no stuffing */
#define PCM_PES_HEADER_LENGTH                  26 /* includes the substreamid + 6 bytes DVD PCM header + 5//1 byte padding */

viSerror_t parsePES(UInt8 **buf_pp, pes_header_p pesh_p);

viSerror_t parsePACK(UInt8 **buf_pp, pack_header_p pah_p);
