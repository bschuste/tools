/***********************************************************************
 **
   Copyright (c) Electropix 2002.
   All rights reserved. Reproduction in whole or in part is prohibited
   without the written consent of the copyright holder.

   This code and information is provided "as is" without any warranty
   of any kind, either expressed or implied, including but not limited
   to the implied warranties of merchantability and/or fitness for any
   particular purpose.

   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Electropix Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **
 ***********************************************************************
 **
   Author: Brigitte Schuster
   Last update: 2002/06/29
   Modified:
   File: viSparse.c
   History:
 		Version 0.1	 2002/06/29
 **
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "viSportab.h"
#include "viSdefs.h"
/*#include "viSstruct.h"*/
#include "viSerror.h"

/***********************************************************************
 ** Debug level printout
 */

#define PROUT fpWrite

/***********************************************************************
 ** Global variables
 */
UInt8     dbgLevel;
FILE	 *fpWrite;

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

typedef struct {
	UInt32          el_PID;       /* elementary stream PID */
    Int32           continuity_counter;
} stream_t, *stream_p;

/***********************************************************************
 ** TS Program Number structure
 */
typedef struct {
	UInt32          el_PID;       /* elementary stream PID */
    void		   *nextProg_p;   /* Pointer to the next program number in the list */
} program_number_t, *program_number_p;

/***********************************************************************
 ** TS PAT structure
 */
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
	UInt32              klv_PID;         //for military applications
	UInt32              pat_defined;
	UInt32              pmt_defined;
	program_number_p    prog_p;
} program_association_table_t, *program_association_table_p;

/***********************************************************************
 ** TS PMT structure
 */
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
} program_map_table_t, *program_map_table_p;

/***********************************************************************
 ** TS adaptation field
 */
typedef struct{
	UInt32 			    adaptation_field_length;
	UInt32 			    discontinuity_indicator;
	UInt32              random_access_indicator;
	UInt32 			    elementary_stream_priority_indicator;
	UInt32 			    PCR_flag;
	UInt32 			    OPCR_flag;
	UInt32 			    splicing_point_flag;
	UInt32 			    transport_private_data_flag;
	UInt32 			    adaptation_field_extension_flag;
	UInt32              program_clock_reference_base1;
	UInt32              program_clock_reference_base0;
	UInt32              program_clock_reference_extension;
	UInt32              original_program_clock_reference_base1;
	UInt32              original_program_clock_reference_base0;
	UInt32              original_program_clock_reference_extension;
	UInt32              splice_countdown;
	UInt32              transport_private_data_length;
	UInt32              adaptation_field_extension_length;
	UInt32              ltw_flag;
	UInt32              piecewise_rate_flag;
	UInt32              seamless_splice_flag;
	UInt32              ltw_valid_flag;
	UInt32              ltw_offset;
	UInt32              piecewise_rate;
	UInt32              splice_type;
	UInt32              DTS_next_AU_hi;
	UInt32              DTS_next_AU_lo;
} adaptation_field_t, *adaptation_field_p;

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


viSerror parsePES(UInt8 *buf_p, pes_header_p pesh_p) {

    viSerror err = VIS_OK;
    UInt8    pes_len;
    Int32    i;

	pesh_p->stream_id = *buf_p;
	DB_L0("     stream_id                                  0x%02lx\n",pesh_p->stream_id);
	buf_p++;
	pesh_p->PES_packet_length = (*buf_p)<<8;
	buf_p++;
	pesh_p->PES_packet_length += *buf_p;
	buf_p++;
	pes_len = pesh_p->PES_packet_length;

	if ( (pesh_p->stream_id != PROGRAM_STREAM_MAP_VALUE)
	  && (pesh_p->stream_id != PADDING_STREAM_VALUE)
	  && (pesh_p->stream_id != PRIVATE_STREAM_2_VALUE)
	  && (pesh_p->stream_id != ECM_STREAM_ID)
	  && (pesh_p->stream_id != EMM_STREAM_ID)
	  && (pesh_p->stream_id != PROGRAM_STREAM_ID_DIR)
	  && (pesh_p->stream_id != DSMCC_STREAM_ID)
	  && (pesh_p->stream_id != TS_STREAM_ID_TYPE_E)
	 ) {
	    pesh_p->PES_scrambling_control    = (*buf_p&0x30)>>4;
		DB_L0("     PES_scrambling_control                     0x%02lx\n",pesh_p->PES_scrambling_control);
	    pesh_p->PES_priority              = (*buf_p&0x08)>>3;
		DB_L0("     PES_priority                               0x%02lx\n",pesh_p->PES_priority);
    	pesh_p->data_alignment_indicator  = (*buf_p&0x04)>>2;
		DB_L0("     data_alignment_indicator                   0x%02lx\n",pesh_p->data_alignment_indicator);
	    pesh_p->copyright                 = (*buf_p&0x02)>>1;
		DB_L0("     copyright                                  0x%02lx\n",pesh_p->copyright);
	    pesh_p->original_or_copy          = (*buf_p&0x01);
		DB_L0("     original_or_copy                           0x%02lx\n",pesh_p->original_or_copy);
	    buf_p++;
	    pesh_p->PTS_DTS_flags             = (*buf_p&0xc0)>>6;
		DB_L0("     PTS_DTS_flags                              0x%02lx\n",pesh_p->PTS_DTS_flags);
	    pesh_p->ESCR_flag                 = (*buf_p&0x20)>>5;
		DB_L0("     ESCR_flag                                  0x%02lx\n",pesh_p->ESCR_flag);
	    pesh_p->ES_rate_flag              = (*buf_p&0x10)>>4;
		DB_L0("     ES_rate_flag                               0x%02lx\n",pesh_p->ES_rate_flag);
	    pesh_p->DSM_trick_mode_flag       = (*buf_p&0x08)>>3;
		DB_L0("     DSM_trick_mode_flag                        0x%02lx\n",pesh_p->DSM_trick_mode_flag);
	    pesh_p->additional_copy_info_flag = (*buf_p&0x04)>>2;
		DB_L0("     additional_copy_info_flag                  0x%02lx\n",pesh_p->additional_copy_info_flag);
	    pesh_p->PES_CRC_flag              = (*buf_p&0x02)>>1;
		DB_L0("     PES_CRC_flag                               0x%02lx\n",pesh_p->PES_CRC_flag);
	    pesh_p->PES_extension_flag        = (*buf_p&0x01);
		DB_L0("     PES_extension_flag                         0x%02lx\n",pesh_p->PES_extension_flag);
	    buf_p++;
	    pesh_p->PES_header_data_length    = (*buf_p);
		DB_L0("     PES_header_data_length                     0x%02lx\n",pesh_p->PES_header_data_length);
	    buf_p++;

	    if (pesh_p->PTS_DTS_flags == 2) {
	        pesh_p->PTS_hi   = (*buf_p & 0x08)>>3;
	        pesh_p->PTS_lo   = (*buf_p & 0x06)<<29;
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)<<22);
			buf_p++;
			pesh_p->PTS_lo  |= (((*buf_p)&0xfe)<<14);
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)<<7);
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)&0xfe)>>1;
			pesh_p->PTS = ((double)pesh_p->PTS_hi + (double)pesh_p->PTS_lo);
	        DB_L1("     PTS                       %6.2f\n",pesh_p->PTS);
		    buf_p++;
//            DB_L0("%04lx\n",(pesh_p->PTS_hi<<1) + pesh_p->PTS_lo);
        }
	    if (pesh_p->PTS_DTS_flags == 3) {
	        pesh_p->PTS_hi   = (*buf_p&0x08)>>3;
	        pesh_p->PTS_lo   = (*buf_p&0x06)<<29;
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)<<22);
			buf_p++;
			pesh_p->PTS_lo  |= ((*(buf_p)&0xfe)<<14);
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)<<7);
			buf_p++;
	        pesh_p->PTS_lo  |= ((*buf_p)&0xfe)>>1;
 			pesh_p->PTS = ((double)pesh_p->PTS_hi + (double)pesh_p->PTS_lo);
			buf_p++;
	        pesh_p->DTS_hi   = (*buf_p&0x08)>>3;
	        pesh_p->DTS_lo   = (*buf_p&0x06)<<29;
    	    buf_p++;
	        pesh_p->DTS_lo  |= ((*buf_p)<<22);
			buf_p++;
			pesh_p->DTS_lo  |= ((*(buf_p)&0xfe)<<14);
			buf_p++;
	        pesh_p->DTS_lo  |= ((*buf_p)<<7);
			buf_p++;
	        pesh_p->DTS_lo  |= ((*buf_p)&0xfe)>>1;
			pesh_p->DTS = ((double)pesh_p->DTS_hi + (double)pesh_p->DTS_lo);
   			buf_p++;
	        DB_L1("     PTS                       %6.2f\n",pesh_p->PTS);
	        DB_L1("     DTS                       %6.2f\n",pesh_p->DTS);
//            DB_L0("%04lx\n",(pesh_p->DTS_hi<<1) + pesh_p->DTS_lo);
        }
	    if (pesh_p->ESCR_flag == 1) {
    	    pesh_p->ESCR_base            = (*buf_p&0x38)<<26;
    	    pesh_p->ESCR_base            = (*buf_p&0x03)<<27;
    	    pesh_p->ESCR_base           += (((*(buf_p+1)))<<16);
    	    pesh_p->ESCR_base           += (((*(buf_p+2))&0xf8)<<8);
    	    pesh_p->ESCR_base           += (((*(buf_p+2))&0x03)<<9);
    	    pesh_p->ESCR_base           += (((*(buf_p+3))));
		    buf_p+=4;
    	    pesh_p->ESCR_base           += (*buf_p&0xf8)>>3;
    		DB_L0("     ESCR_base                                  0x%04lx\n",pesh_p->ESCR_base);
    	    pesh_p->ESCR_extension       = (*buf_p&0x03)>>1;
		    buf_p+=1;
    	    pesh_p->ESCR_extension      += (*buf_p&0xfe)>>1;
    		DB_L0("     ESCR_extension                             0x%04lx\n",pesh_p->ESCR_extension);
		    buf_p+=2;
        }
	    if (pesh_p->ES_rate_flag == 1) {
    	    pesh_p->ES_rate              = ((*buf_p&0x7f)<<15) + ((*(buf_p+1))<<7) + ((*(buf_p+2)&0xfe)>>1);
    		DB_L0("     ES_rate                                    0x%04lx\n",pesh_p->ES_rate);
		    buf_p+=3;
        }
	    if (pesh_p->DSM_trick_mode_flag == 1) {
        	pesh_p->trick_mode_control       = (*buf_p&0xe0)>>5;
	        if (pesh_p->trick_mode_control == FAST_FORWARD) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L0("     field_id                                   0x%02lx\n",pesh_p->field_id);
	            pesh_p->intra_slice_refresh  = (*buf_p&0x04)>>2;
    		    DB_L0("     intra_slice_refresh                        0x%02lx\n",pesh_p->intra_slice_refresh);
            	pesh_p->frequency_truncation = (*buf_p&0x03);
    		    DB_L0("     frequency_truncation                       0x%02lx\n",pesh_p->frequency_truncation);
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == SLOW_MOTION) {
	            pesh_p->rep_cntrl            = (*buf_p&0x1f);
    		    DB_L0("     rep_cntrl                                  0x%02lx\n",pesh_p->rep_cntrl);
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == FREEZE_FRAME) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L0("     field_id                                   0x%02lx\n",pesh_p->field_id);
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == FAST_REVERSE) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L0("     field_id                                   0x%02lx\n",pesh_p->field_id);
	            pesh_p->intra_slice_refresh  = (*buf_p&0x04)>>2;
    		    DB_L0("     intra_slice_refresh                        0x%02lx\n",pesh_p->intra_slice_refresh);
            	pesh_p->frequency_truncation = (*buf_p&0x03);
    		    DB_L0("     frequency_truncation                       0x%02lx\n",pesh_p->frequency_truncation);
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == SLOW_REVERSE) {
	            pesh_p->rep_cntrl            = (*buf_p&0x1f);
    		    DB_L0("     rep_cntrl                                  0x%02lx\n",pesh_p->rep_cntrl);
        	    buf_p++;
    	    }
	    }
		if (pesh_p->additional_copy_info_flag == 1) {
	        pesh_p->additional_copy_info     = (*buf_p&0x7f);
    		DB_L0("     additional_copy_info                           0x%02lx\n",pesh_p->additional_copy_info);
        	buf_p++;
	    }
		if (pesh_p->PES_CRC_flag == 1) {
        	pesh_p->previous_PES_packet_CRC  = (*buf_p)<<8;
        	buf_p++;
        	pesh_p->previous_PES_packet_CRC += (*buf_p);
    		DB_L0("     previous_PES_packet_CRC                        0x%02lx\n",pesh_p->previous_PES_packet_CRC);
        	buf_p++;
	    }
		if (pesh_p->PES_extension_flag == 1) {
        	pesh_p->PES_private_data_flag                = (*buf_p&0x80)>>7;
    		DB_L0("     PES_private_data_flag                          0x%02lx\n",pesh_p->PES_private_data_flag);
	        pesh_p->pack_header_field_flag               = (*buf_p&0x40)>>6;
    		DB_L0("     pack_header_field_flag                         0x%02lx\n",pesh_p->pack_header_field_flag);
	        pesh_p->program_packet_sequence_counter_flag = (*buf_p&0x20)>>5;
    		DB_L0("     program_packet_sequence_counter_flag           0x%02lx\n",pesh_p->program_packet_sequence_counter_flag);
	        pesh_p->P_STD_buffer_flag                    = (*buf_p&0x10)>>4;
    		DB_L0("     P_STD_buffer_flag                              0x%02lx\n",pesh_p->P_STD_buffer_flag);
	        pesh_p->PES_extension_flag_2                 = (*buf_p&0x01);
    		DB_L0("     PES_extension_flag_2                           0x%02lx\n",pesh_p->PES_extension_flag_2);
        	buf_p++;

		    if (pesh_p->PES_private_data_flag == 1) {
		        // TBD:128 bits value
	            pesh_p->PES_private_data                 = (*buf_p);
        		DB_L0("     PES_private_data                           0x%02lx\n",pesh_p->PES_private_data);
            	buf_p+=16;
	        }
		    if (pesh_p->pack_header_field_flag == 1) {
    	        pesh_p->pack_field_length                = (*buf_p);
        		DB_L0("     pack_field_length                          0x%02lx\n",pesh_p->pack_field_length);
            	buf_p++;
		        // TBD:pack_header();
	        }
	        if (pesh_p->program_packet_sequence_counter_flag == 1) {
	            pesh_p->program_packet_sequence_counter  = (*buf_p&0x7f);
        		DB_L0("     program_packet_sequence_counter            0x%02lx\n",pesh_p->program_packet_sequence_counter);
            	buf_p++;
	            pesh_p->MPEG1_MPEG2_identifier           = (*buf_p&0x40)>>6;;
        		DB_L0("     MPEG1_MPEG2_identifier                     0x%02lx\n",pesh_p->MPEG1_MPEG2_identifier);
	            pesh_p->original_stuff_length            = (*buf_p&0x3f);
        		DB_L0("     original_stuff_length                      0x%02lx\n",pesh_p->original_stuff_length);
            	buf_p++;
	        }
	        if (pesh_p->P_STD_buffer_flag == 1) {
	            pesh_p->P_STD_buffer_scale               = (*buf_p&0x20)>>5;
        		DB_L0("     P_STD_buffer_scale                         0x%02lx\n",pesh_p->P_STD_buffer_scale);
	            pesh_p->P_STD_buffer_size                = (*buf_p&0x1f)<<8;
            	buf_p++;
	            pesh_p->P_STD_buffer_size               += (*buf_p);
        		DB_L0("     P_STD_buffer_size                          0x%02lx\n",pesh_p->P_STD_buffer_size);
            	buf_p++;
	        }
	        if (pesh_p->PES_extension_flag_2 == 1) {
	            pesh_p->PES_extension_field_length       = (*buf_p&0x7f);
        		DB_L0("     PES_extension_field_length                 0x%02lx\n",pesh_p->PES_extension_field_length);
            	buf_p++;
		        for (i=0;i<pesh_p->PES_extension_field_length;i++) {
                	buf_p++;
			    }
	        }
	    }
    }
    else if ( (pesh_p->stream_id != PROGRAM_STREAM_MAP_VALUE)
	       && (pesh_p->stream_id != PRIVATE_STREAM_2_VALUE)
	       && (pesh_p->stream_id != ECM_STREAM_ID)
	       && (pesh_p->stream_id != EMM_STREAM_ID)
	       && (pesh_p->stream_id != PROGRAM_STREAM_ID_DIR)
	       && (pesh_p->stream_id != DSMCC_STREAM_ID)
	       && (pesh_p->stream_id != TS_STREAM_ID_TYPE_E) ) {
        for (i=0;i<pesh_p->PES_packet_length;i++) {
	        buf_p++;
	    }
	}
    else if (pesh_p->stream_id != PADDING_STREAM_VALUE) {
        for (i=0;i<pesh_p->PES_packet_length;i++) {
	        buf_p++;
	    }
    }
    return(err);
}

viSerror parseAF(UInt8 *buf_p, adaptation_field_p af_p) {

    viSerror err = VIS_OK;
    UInt8    af_len;
    Int32    i;

	af_p->adaptation_field_length = *buf_p;
	af_len = af_p->adaptation_field_length;
	buf_p++;
	if (af_p->adaptation_field_length > 0) {
	    af_p->discontinuity_indicator              = (*buf_p&0x80)>>7;
		DB_L0("     discontinuity_indicator                    0x%02lx\n",af_p->discontinuity_indicator);
	    af_p->random_access_indicator              = (*buf_p&0x40)>>6;
		DB_L0("     random_access_indicator                    0x%02lx\n",af_p->random_access_indicator);
	    af_p->elementary_stream_priority_indicator = (*buf_p&0x20)>>5;
		DB_L0("     elementary_stream_priority_indicator       0x%02lx\n",af_p->elementary_stream_priority_indicator);
	    af_p->PCR_flag                             = (*buf_p&0x10)>>4;
		DB_L0("     PCR_flag                                   0x%02lx\n",af_p->PCR_flag);
	    af_p->OPCR_flag                            = (*buf_p&0x08)>>3;
		DB_L0("     OPCR_flag                                  0x%02lx\n",af_p->OPCR_flag);
	    af_p->splicing_point_flag                  = (*buf_p&0x04)>>2;
		DB_L0("     splicing_point_flag                        0x%02lx\n",af_p->splicing_point_flag);
	    af_p->transport_private_data_flag          = (*buf_p&0x02)>>1;
		DB_L0("     transport_private_data_flag                0x%02lx\n",af_p->transport_private_data_flag);
	    af_p->adaptation_field_extension_flag      = (*buf_p&0x01);
		DB_L0("     adaptation_field_extension_flag            0x%02lx\n",af_p->adaptation_field_extension_flag);
    	buf_p++;
        af_len--;

	    if (af_p->PCR_flag == 1) {
	        af_p->program_clock_reference_base1     = ((*buf_p)<<24) + ((*(buf_p+1))<<16) + ((*(buf_p+2))<<8) + (*(buf_p+3));
	        buf_p+=4;
            af_len-=4;
	        af_p->program_clock_reference_base0     = (*buf_p&0x80)>>7;
    	    DB_L0("     program_clock_reference_base1              0x%ld",(af_p->program_clock_reference_base1>>31)&0x00000001);
	        DB_L0("%04lx\n",(af_p->program_clock_reference_base1<<1) + af_p->program_clock_reference_base0);
	        af_p->program_clock_reference_extension = (*buf_p&0x01)<<8;
        	buf_p++;
            af_len--;
	        af_p->program_clock_reference_extension+= *buf_p;
    	    DB_L0("     program_clock_reference_extension          0x%04lx\n",af_p->program_clock_reference_extension);
        	buf_p++;
            af_len--;
		}
	    if (af_p->OPCR_flag == 1) {
	        af_p->original_program_clock_reference_base1     = ((*buf_p)<<24) + ((*(buf_p+1))<<16) + ((*(buf_p+2))<<8) + (*(buf_p+3));
	        buf_p+=4;
            af_len-=4;
	        af_p->original_program_clock_reference_base0     = (*buf_p&0x80)>>7;
            DB_L0("     original_program_clock_reference_base      0x%ld",(af_p->original_program_clock_reference_base1>>31)&0x00000001);
            DB_L0("%04lx\n",(af_p->original_program_clock_reference_base1<<1) + af_p->original_program_clock_reference_base0);
	        af_p->original_program_clock_reference_extension = (*buf_p&0x01)<<8;
        	buf_p++;
            af_len--;
	        af_p->original_program_clock_reference_extension+= *buf_p;
    	    DB_L0("     original_program_clock_reference_extension 0x%04lx\n",af_p->original_program_clock_reference_extension);
        	buf_p++;
            af_len--;
		}
    	if (af_p->splicing_point_flag ==1) {
	        af_p->splice_countdown = *buf_p;
    		DB_L0("     splice_countdown                           0x%02lx\n",af_p->splice_countdown);
        	buf_p++;
            af_len--;
	    }
    	if (af_p->transport_private_data_flag ==1) {
	        af_p->transport_private_data_length = *buf_p;
    		DB_L0("     transport_private_data_length              0x%02lx\n",af_p->transport_private_data_length);
        	buf_p++;
            af_len--;
	        for (i=0;i<af_p->transport_private_data_length;i++) {
            	buf_p++;
                af_len--;
		    }
	    }
    	if (af_p->adaptation_field_extension_flag == 1) {
	        af_p->adaptation_field_extension_length = *buf_p;
    		DB_L0("     adaptation_field_extension_length          0x%02lx\n",af_p->adaptation_field_extension_length);
        	buf_p++;
            af_len--;
	        af_p->ltw_flag             = (*buf_p&0x80)>>7;
    		DB_L0("     ltw_flag                                   0x%02lx\n",af_p->ltw_flag);
	        af_p->piecewise_rate_flag  = (*buf_p&0x40)>>6;
    		DB_L0("     piecewise_rate_flag                        0x%02lx\n",af_p->piecewise_rate_flag);
	        af_p->seamless_splice_flag = (*buf_p&0x20)>>5;
    		DB_L0("     seamless_splice_flag                       0x%02lx\n",af_p->seamless_splice_flag);
        	buf_p++;
            af_len--;
        	if (af_p->ltw_flag == 1) {
    	        af_p->ltw_valid_flag   = (*buf_p&0x80)>>7;
    		    DB_L0("     ltw_valid_flag                             0x%02lx\n",af_p->ltw_valid_flag);
            	buf_p++;
                af_len--;
	            af_p->ltw_offset       = (*buf_p&0x7f);
            	buf_p++;
                af_len--;
	            af_p->ltw_offset      += (*buf_p)<<8;
    		    DB_L0("     ltw_offset                                 0x%02lx\n",af_p->ltw_offset);
    		    DB_L0("     af_len                                     %d\n",af_len);
            	buf_p++;
                af_len--;
		    }
        	if (af_p->piecewise_rate_flag == 1) {
	            af_p->piecewise_rate   = (((*buf_p)&0x3f)<<16) + ((*(buf_p+1))<<8) + (*(buf_p+2));
    		    DB_L0("     piecewise_rate                             0x%04lx\n",af_p->piecewise_rate);
	            buf_p+=3;
                af_len-=3;
		    }
        	if (af_p->seamless_splice_flag == 1) {
	            af_p->splice_type      = (*buf_p&0xf0)>>3;
        		DB_L0("     splice_type                                0x%02lx\n",af_p->splice_type);
	            af_p->DTS_next_AU_hi   = (*buf_p&0x0e)<<28;
            	buf_p++;
                af_len--;
	            af_p->DTS_next_AU_hi  += ((*buf_p)<<21) + (((*(buf_p+1))&0xfe)<<13);
	            af_p->DTS_next_AU_hi  +=  (*(buf_p+2))<<5;
	            af_p->DTS_next_AU_hi  += ((*(buf_p+3))&0xfc)>>2;
	            af_p->DTS_next_AU_lo  += ((*(buf_p+3))&0x02)>>1;
            	buf_p+=4;
                af_len-=4;
                DB_L0("     DTS_next_AU                                0x%ld",(af_p->DTS_next_AU_hi>>31)&0x00000001);
                DB_L0("%04lx\n",(af_p->DTS_next_AU_hi<<1) + af_p->DTS_next_AU_lo);
		    }
	    }
	}
    return(err);
}

#if 0
viSerror parseTS(UInt8 *bufts, stream_p st_p) {

    viSerror err = VIS_OK;
    Int32    i;

	// Parse the TS header
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
    if (bufts[0] != SYNC_BYTE) {
   	    DB_L0(" ERROR not a TS packet, 0x%02x\n",bufts[0]);
        return(VIS_ERR_NOT_TS_HEADER) ;
    }
    //  transport_error_indicator
	DB_L0("     transport_error_indicator                  0x%02x\n",bufts[1]&0x80);
	// payload_unit_start_indicator
	DB_L0("     payload_unit_start_indicator               0x%02x\n",bufts[1]&0x40);
	// transport_priority
	DB_L0("     transport_priority                         0x%02x\n",bufts[1]&0x20);
	// transport_scrambling_control
	DB_L0("     transport_scrambling_control               0x%02x\n",bufts[3]&0xC0);
	// adaptation_field_control
    DB_L0("     adaptation_field_control                   0x%02x\n",(bufts[3]&0x20));
    if (bufts[3]&0x20) // get the adaptation field size
	    DB_L0("     adaptation_field_size                      %d\n",bufts[4]);
    // payload
	DB_L0("     payload                                    0x%02x\n",bufts[3]&0x10);
	// continuity_counter
	DB_L0("     continuity_counter                         0x%02x\n",bufts[3]&0x0F);
    if (st_p->continuity_counter != bufts[3]&0x0F)
	    printf("     continuity error in VIDEO STREAM        %d\n",abs(st_p->continuity_counter - bufts[3]&0x0F));
	st_p->continuity_counter = bufts[3]&0x0F;

    if (bufts[3]&0x20)  // parse the adaptation field
        parseAF(&bufts[4], af_p);

	if (bufts[1]&0x40) { // parse the PES header
        if (bufts[3]&0x20) { // parse the adaptation field
		    parsePES(&bufts[bufts[4]+1+3+4], pes_p);
		}
	    else {
		    parsePES(&bufts[7], pes_p);
		}
    }

    // Read the TS payload
    // Read the PES header
	// bufts[4,5,6]: 0x00 00 01
	// bufts[7]: stream id
	// bufts[8-9]: PES packet length
	// bufts[10,11,12]: PES header

    if ( (bufts[4]==0)&&(bufts[5]==0)&&(bufts[6]==1) ) {
        DB_L0("         Stream ID 0x%x\n",bufts[7]);
        DB_L0("         bufts[8]  0x%x\n",bufts[8]);
        DB_L0("         bufts[9]  0x%x\n",bufts[9]);
        DB_L0("         PES packet length %d bytes\n",((bufts[8]<<8) + bufts[9]));
	}

	for (i=4;i<TS_PACKET_SIZE-4;i++) {
        tmp2 = tmp1;
		tmp1 = tmp0;
		tmp0 = bufts[i];
	    if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 		    // Start code detected
			if ( (bufts[i+1] >= VIDEO_STREAM_ID_MIN)&&(bufts[i+1] <= VIDEO_STREAM_ID_MAX) )
                DB_L0("         PES_START_CODE found, byte = %d\n",i+1);
		}
    }
    return(err);
}
#endif

int main(int argc, char **argv)
{
	FILE	*fpRead;
	char	ReadName[256];
	char	WriteName[256];
	int     i;
	int     nbytes;
	UInt8   tmByte = 0;
	UInt8   tmp0 = 1,tmp1 = 1,tmp2 = 1;
	UInt8   bufts[TS_PACKET_SIZE];
	Double  packetCounter = 0;
	UInt16  tmHword;
	program_association_table_p pat_p;
	program_map_table_p pmt_p;
	pes_header_p pes_p;
	adaptation_field_p af_p;
	UInt32  program_number  = 0;
/*    program_number_p prog_p;
    UInt32 elementary_PID = 0;
	UInt32    program_map_PID = 0;
    program_number_p nextProg_p; */
    stream_p stream_vid_p, stream_aud_p, stream_klv_p;
    UInt32 stream_type = 0;
    UInt32 ES_info_length = 0;
    UInt32 len = 0;
    Int32  continuity_counter = 0;

    dbgLevel = 0;

	if ( argc < 2 ) {
		printf("%s input_file_name\n",argv[0]);
    printf(" send e-mail to info@electropix.com for additional features or for a bug report\n");
		exit(1);
	}
	else {
		strcpy(ReadName,argv[1]);
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName);
		return(VIS_ERR_OPEN_FILE);
	}

	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
	}

	strncpy(WriteName,ReadName,strlen(ReadName)-4);
	strcat(WriteName,".txt");
	if ((fpWrite=fopen(WriteName,"wb"))==NULL) {
		printf("error : cannot open %s\n",WriteName);
		return(VIS_ERR_OPEN_FILE);
	}

	nbytes = fread(&tmByte,1,sizeof(UInt8),fpRead);
    if (tmByte != SYNC_BYTE) {
   	    printf(" Not a transport stream\n");
	       exit(-1);
	}
    fseek(fpRead,0,SEEK_SET);

	/* allocate memory for the structures */
  	if ((pat_p = (program_association_table_p) malloc(sizeof(program_association_table_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(pat_p,sizeof(program_association_table_t),0);
  	if ((pmt_p = (program_map_table_p) malloc(sizeof(program_map_table_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(pmt_p,sizeof(program_map_table_t),0);
  	if ((pes_p = (pes_header_p) malloc(sizeof(pes_header_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(pes_p,sizeof(pes_header_t),0);
  	if ((af_p = (adaptation_field_p) malloc(sizeof(adaptation_field_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(af_p,sizeof(adaptation_field_t),0);

	/* allocate memory for three streams */
  	if ((stream_vid_p = (stream_p) malloc(sizeof(stream_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(stream_vid_p,sizeof(stream_t),0);
  	if ((stream_aud_p = (stream_p) malloc(sizeof(stream_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(stream_aud_p,sizeof(stream_t),0);
  	if ((stream_klv_p = (stream_p) malloc(sizeof(stream_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(stream_klv_p,sizeof(stream_t),0);


    /*
	pat_p->table_id = 0;
	pat_p->section_syntax_indicator;
	pat_p->section_length;
	pat_p->transport_stream_id;
	pat_p->version_number;
	pat_p->current_next_indicator;
	pat_p->section_number;
	pat_p->last_section_number;
	pat_p->network_PID;
	pat_p->program_map_PID;
	pat_p->video_PID;
	pat_p->audio_PID;
	pat_p->pat_defined;
	pat_p->pms_defined;
	*/

    dbgLevel = 1;

	do {
	    packetCounter++;
	    DB_L0(" PACKET NUMBER %ld ",(UInt32)packetCounter);
        // Read a new buffer from the file
	    nbytes = fread(bufts,1,TS_PACKET_SIZE*sizeof(UInt8),fpRead);
        if (nbytes != TS_PACKET_SIZE*sizeof(UInt8)) {
            if (nbytes == 0) {
                // we reached the end of the file
   			    printf(" No error, End of File\n");
		        exit(0);
		    }
		    else {
   			    printf(" End of File detected, nbytes = %d\n",nbytes);
		        exit(0);
		    }
	    }
	    // Parse the TS header
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

	    if (bufts[0] != SYNC_BYTE) {
   	        DB_L0(" ERROR not a TS packet, 0x%02x\n",bufts[0]);
   	        DB_L00(" Resynchronizing ");
		    i = 1;
		    while ((bufts[i] != SYNC_BYTE)&&(i<TS_PACKET_SIZE-1)) i++;
   	        if ( i<(TS_PACKET_SIZE-1)) {
		        DB_L0(" Done after %d bytes\n",i);
			}
			else {
   	            DB_L00(" not found \n ");
		    }
		}

	    /* PID */
	    tmHword = ( (bufts[1]<<8) + bufts[2])& 0x1FFF;

	    switch (tmHword) {
	        case PAT_PID:
	    	    DB_L0(" PAT PID 0x%x\n",tmHword);
	            //  table_id
		        pat_p->table_id = bufts[5];
		        DB_L0("     table_id                                   %ld\n",pat_p->table_id);
	            //  section_syntax_indicator
		        pat_p->section_syntax_indicator = (bufts[6]&0x80)>>7;
		        DB_L0("     section_syntax_indicator                   0x%02lx\n",pat_p->section_syntax_indicator);
	            //  section_length
		        pat_p->section_length = ((bufts[6]&0x0F)<<8) + bufts[7];
		        DB_L0("     section_length                             %ld\n",pat_p->section_length);
			      if ( (pat_p->section_length) > (1021) ) 
		            DB_L00("     WARNING: section_length is bigger than 1021 in PAT\n");
            //return(VIS_ERR_TS);
	            //  transport_stream_id
		        pat_p->transport_stream_id = ((bufts[8])<<8) + bufts[9];
		        DB_L0("     transport_stream_id                        %ld\n",pat_p->transport_stream_id);
	            //  version_number
		        pat_p->version_number = ((bufts[10]&0x3E)>>1);
		        DB_L0("     version_number                             %ld\n",pat_p->version_number);
	            //  current_next_indicator
		        pat_p->current_next_indicator = (bufts[10]&0x01);
		        DB_L0("     current_next_indicator                     %ld\n",pat_p->current_next_indicator);
	            //  section_number
		        pat_p->section_number = (bufts[11]);
		        DB_L0("     section_number                             0x%02lx\n",pat_p->section_number);
	            //  last_section_number
		        pat_p->last_section_number = (bufts[12]);
		        DB_L0("     last_section_number                        0x%02lx\n",pat_p->last_section_number);
 				    //  program_number
			      program_number  = ((bufts[13])<<8) + bufts[14];
		        DB_L0("     program_number                             0x%02lx\n",program_number);
			    if (program_number == 0) {
			        pat_p->network_PID = ((bufts[15]&0x1F)<<8) + bufts[16];
    		        DB_L0("     network_PID                                0x%02lx\n",pat_p->network_PID);
				}
                else {
	    		    pat_p->program_map_PID = ((bufts[15]&0x1F)<<8) + bufts[16];
		            DB_L0("     program_map_PID                            0x%02lx\n",pat_p->program_map_PID);
				}
			    program_number  = ((bufts[17])<<8) + bufts[18];
             //Following is the CRC
             /*
			    if (program_number == 0) {
			        pat_p->network_PID = ((bufts[19]&0x1F)<<8) + bufts[20];
    		        DB_L0("     network_PID              0x%02lx\n",pat_p->network_PID);
				}
                else {
	    		    pat_p->program_map_PID = ((bufts[19]&0x1F)<<8) + bufts[20];
		            DB_L0("     program_map_PID          0x%02lx\n",pat_p->program_map_PID);
				} */
				pat_p->pat_defined = TRUE;
/*
  	            if ((pat_p->prog_p = (program_number_p) malloc(sizeof(program_number_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
                memset(pat_p->prog_p,sizeof(program_number_t),0);
		        pat_p->prog_p->nextProg_p = NULL;
		        prog_p = pat_p->prog_p;

			    for (i=0;i<=pat_p->last_section_number;i++)	{
			        program_number  = ((bufts[4*i+13])<<8) + bufts[4*i+14];
			        PID = ((bufts[4*i+15]&0x1F)<<8) + bufts[4*i+16];
  	                if ((nextProg_p = (program_number_p) malloc(sizeof(program_number_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
                    memset(nextProg_p,sizeof(program_number_t),0);
		            nextProg_p->nextProg_p = NULL;
				    prog_p->nextProg_p = (void *)nextProg_p;
			        prog_p = (program_number_p)prog_p->nextProg_p;
			    }
*/
		    	break;
	        case CAT_PID:
	        	DB_L0(" CAT PID 0x%x\n",tmHword);
	            break;
	        case NULL_PID:
	    	    DB_L0(" NULL PID 0x%x\n",tmHword);
	            break;
	        default:
	        if ( ( tmHword >= ANY_PID_MIN ) || ( tmHword <= ANY_PID_MAX ) ) {
	    	    DB_L0(" ANY PID 0x%x \n",tmHword);
		        if (pat_p->pat_defined == TRUE) {
				    if ( tmHword == pat_p->program_map_PID ) {
            	    	    DB_L0(" PROGRAM MAP PID 0x%x\n",tmHword);
	                        //  table_id
		                    pmt_p->table_id = bufts[5];
		                    DB_L0("     table_id                                   %ld\n",pmt_p->table_id);
	                        //  section_syntax_indicator
		                    pmt_p->section_syntax_indicator = (bufts[6]&0x80)>>7;
		                    DB_L0("     section_syntax_indicator                   0x%02lx\n",pmt_p->section_syntax_indicator);
	                        //  section_length
		                    pmt_p->section_length = ((bufts[6]&0x0F)<<8) + bufts[7];
		                    DB_L0("     section_length                             %ld\n",pmt_p->section_length);
		                    if ( (pmt_p->section_length) > (1021) ) 
					                  DB_L00("     WARNING: section_length is bigger than 1021 in PMT\n");
	                       //  transport_stream_id
		                    pmt_p->program_number = ((bufts[8])<<8) + bufts[9];
		                    DB_L0("     program_number                             %ld\n",pmt_p->program_number);
	                        //  version_number
		                    pmt_p->version_number = ((bufts[10]&0x3E)>>1);
		                    DB_L0("     version_number                             %ld\n",pmt_p->version_number);
	                        //  current_next_indicator
		                    pmt_p->current_next_indicator = (bufts[10]&0x01);
		                    DB_L0("     current_next_indicator                     %ld\n",pmt_p->current_next_indicator);
	                        //  section_number
		                    pmt_p->section_number = (bufts[11]);
		                    DB_L0("     section_number                             0x%02lx\n",pmt_p->section_number);
	                        //  last_section_number
		                    pmt_p->last_section_number = (bufts[12]);
		                    DB_L0("     last_section_number                        0x%02lx\n",pmt_p->last_section_number);
	                        //  PCR_PID
		                    pmt_p->PCR_PID = ((bufts[13]&0x1F)<<8) + bufts[14];
		                    DB_L0("     PCR_PID                                    0x%02lx\n",pmt_p->PCR_PID);
 				            //  program_info_length
			                pmt_p->program_info_length  = ((bufts[15]&0x0F)<<8) + bufts[16];
				            // stream_type
					        if ( (16+pmt_p->program_info_length) > (TS_PACKET_SIZE-1) ) return(VIS_ERR_TS);
						    len = 1;
					 		while ((9+pmt_p->program_info_length+len < pmt_p->section_length-4)) {
						        stream_type = bufts[16+pmt_p->program_info_length+len];
		                        DB_L0("     stream_type                                0x%02lx\n",stream_type);
					            ES_info_length = ((bufts[16+pmt_p->program_info_length+len+3]&0x0F)<<8) + (bufts[16+pmt_p->program_info_length+len+4]);
                                if ((stream_type == TS_VIDEO_TYPE)) {
					    	        pat_p->video_PID = ((bufts[16+pmt_p->program_info_length+len+1]&0x1F)<<8) + bufts[16+pmt_p->program_info_length+len+2];
		                            DB_L0("     pat_p->video_PID                           0x%02lx\n", pat_p->video_PID);
				                    pat_p->pmt_defined = TRUE;
						        }
                                if ((stream_type == TS_AUDIO_TYPE)) {
					    	        pat_p->audio_PID = ((bufts[16+pmt_p->program_info_length+len+1]&0x1F)<<8) + bufts[16+pmt_p->program_info_length+len+2];
		                            DB_L0("     pat_p->audio_PID                           0x%02lx\n", pat_p->audio_PID);
						        }
                                if ((stream_type == TS_PRIVATE_DATA_TYPE)) {
					    	        pat_p->klv_PID = ((bufts[16+pmt_p->program_info_length+len+1]&0x1F)<<8) + bufts[16+pmt_p->program_info_length+len+2];
		                            DB_L0("     pat_p->klv_PID                             0x%02lx\n", pat_p->klv_PID);
						        }
		                        DB_L0("     ES_info_length                             0x%02lx\n",ES_info_length);
                                if (ES_info_length != 0) {
                                    int n;
                                    n = 0;
                                    while (n<ES_info_length) {
		                                DB_L0("       descriptor                               0x%02lx\n",(bufts[16+pmt_p->program_info_length+len+5+n]));
                                        n++;
                                    }
                                }
							    len += ES_info_length + 5;
						    }
					}
			        if (pat_p->pmt_defined == TRUE) {
			            if ( tmHword == pat_p->video_PID ) {
	    	                DB_L00(" VIDEO STREAM PID\n");
	                        //  transport_error_indicator
		                    DB_L0("     transport_error_indicator                  0x%02x\n",bufts[1]&0x80);
	                        // payload_unit_start_indicator
		                    DB_L0("     payload_unit_start_indicator               0x%02x\n",bufts[1]&0x40);
	                        // transport_priority
		                    DB_L0("     transport_priority                         0x%02x\n",bufts[1]&0x20);
		                    // transport_scrambling_control
		                    DB_L0("     transport_scrambling_control               0x%02x\n",bufts[3]&0xC0);
		                    // adaptation_field_control
                            DB_L0("     adaptation_field_control                   0x%02x\n",(bufts[3]&0x20));
                            if (bufts[3]&0x20) // get the adaptation field size
		                        DB_L0("     adaptation_field_size                      %d\n",bufts[4]);
		                    // payload
		                    DB_L0("     payload                                    0x%02x\n",bufts[3]&0x10);
		                    // continuity_counter
		                    DB_L0("     continuity_counter                         0x%02x\n",bufts[3]&0x0F);
                            if (stream_vid_p->continuity_counter != (bufts[3]&0x0F))
                            {
                            	DB_L0("     continuity error in VIDEO STREAM        %d\n",abs(stream_vid_p->continuity_counter - (bufts[3]&0x0F)));
                            	stream_vid_p->continuity_counter = bufts[3]&0x0F;
                            }
				    		stream_vid_p->continuity_counter = (stream_vid_p->continuity_counter + 1)%16;

    			            if (bufts[3]&0x20)  // parse the adaptation field
    					            parseAF(&bufts[4], af_p);

						    if (bufts[1]&0x40) { // parse the PES header
    			                if (bufts[3]&0x20) { // parse the adaptation field
						            parsePES(&bufts[bufts[4]+1+3+4], pes_p);
		    					}
	    						else {
						            parsePES(&bufts[7], pes_p);
		    					}
							}

    			            // Read the TS payload
                            // Read the PES header
	                        // bufts[4,5,6]: 0x00 00 01
	                        // bufts[7]: stream id
	                        // bufts[8-9]: PES packet length
	                        // bufts[10,11,12]: PES header

                            if ( (bufts[4]==0)&&(bufts[5]==0)&&(bufts[6]==1) ) {
                                DB_L0("         Stream ID 0x%x\n",bufts[7]);
                                DB_L0("         bufts[8]  0x%x\n",bufts[8]);
                                DB_L0("         bufts[9]  0x%x\n",bufts[9]);
                                DB_L0("         PES packet length %d bytes\n",((bufts[8]<<8) + bufts[9]));
		                    }

			       	        for (i=4;i<TS_PACKET_SIZE-4;i++) {
            	                tmp2 = tmp1;
		                        tmp1 = tmp0;
		                        tmp0 = bufts[i];
	                            if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			                        // Start code detected
			                        if ( (bufts[i+1] >= VIDEO_STREAM_ID_MIN)&&(bufts[i+1] <= VIDEO_STREAM_ID_MAX) )
                                        DB_L0("         PES_START_CODE found, byte = %d\n",i+1);
		                        }
		                    }
				        } // if video
				        else if ( tmHword == pat_p->audio_PID ) {
	    	                DB_L0(" AUDIO STREAM PID 0x%x\n",tmHword);
        	                // payload_unit_start_indicator
		                    DB_L0("     payload_unit_start_indicator               0x%02x\n",bufts[1]&0x40);
			               	for (i=4;i<TS_PACKET_SIZE-4;i++) {
            	                tmp2 = tmp1;
		                        tmp1 = tmp0;
		                        tmp0 = bufts[i];
	                            if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			                        // Start code detected
			                        if ( (bufts[i+1] >= AUDIO_STREAM_ID_MIN)&&(bufts[i+1] <= AUDIO_STREAM_ID_MAX) )
                                        DB_L0("         PES_START_CODE found, byte = %d\n",i+1);
                                }
					        }
						} // if audio
				        else if ( tmHword == pat_p->klv_PID ) {
	    	                DB_L0(" KLV STREAM PID 0x%x\n",tmHword);
	                        //  transport_error_indicator
		                    DB_L0("     transport_error_indicator                  0x%02x\n",bufts[1]&0x80);
	                        // payload_unit_start_indicator
		                    DB_L0("     payload_unit_start_indicator               0x%02x\n",bufts[1]&0x40);
	                        // transport_priority
		                    DB_L0("     transport_priority                         0x%02x\n",bufts[1]&0x20);
		                    // transport_scrambling_control
		                    DB_L0("     transport_scrambling_control               0x%02x\n",bufts[3]&0xC0);
		                    // adaptation_field_control
                            DB_L0("     adaptation_field_control                   0x%02x\n",(bufts[3]&0x20));
                            if (bufts[3]&0x20) // get the adaptation field size
		                        DB_L0("     adaptation_field_size                      %d\n",bufts[4]);
		                    // payload
		                    DB_L0("     payload                                    0x%02x\n",bufts[3]&0x10);
		                    // continuity_counter
		                    DB_L0("     continuity_counter                       0x%02x\n",bufts[3]&0x0F);

    			            if (bufts[3]&0x20)  // parse the adaptation field
    					            parseAF(&bufts[4], af_p);

						    if (bufts[1]&0x40) { // parse the PES header
    			                if (bufts[3]&0x20) { // parse the adaptation field
						            parsePES(&bufts[bufts[4]+1+3], pes_p);
		    					}
	    						else {
						            parsePES(&bufts[7], pes_p);
		    					}
							}

    			            // Read the TS payload
                            // Read the PES header
	                        // bufts[4,5,6]: 0x00 00 01
	                        // bufts[7]: stream id
	                        // bufts[8-9]: PES packet length
	                        // bufts[10,11,12]: PES header

                            if ( (bufts[4]==0)&&(bufts[5]==0)&&(bufts[6]==1) ) {
                                DB_L0("         Stream ID 0x%x\n",bufts[7]);
                                DB_L0("         bufts[8]  0x%x\n",bufts[8]);
                                DB_L0("         bufts[9]  0x%x\n",bufts[9]);
                                DB_L0("         PES packet length %d bytes\n",((bufts[8]<<8) + bufts[9]));
		                    }
			               	for (i=4;i<TS_PACKET_SIZE-4;i++) {
            	                tmp2 = tmp1;
		                        tmp1 = tmp0;
		                        tmp0 = bufts[i];
	                            if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			                        // Start code detected
//			                        if ( (bufts[i+1] >= AUDIO_STREAM_ID_MIN)&&(bufts[i+1] <= AUDIO_STREAM_ID_MAX) )
                                        DB_L0("         PES_START_CODE found, byte = %d\n",i+1);
                                }
					        }
						} // if KLV
					} // if pmt_defined
		            else {
 	    	            DB_L00(" UNIDENTIFIED STREAM PID\n");
				    }
                } // if pat_defined
		        else {
 	    	        DB_L00(" UNIDENTIFIED STREAM PID\n");
				}
		    }
		    else {
	    	    DB_L0(" RESERVED PID 0x%x\n",tmHword);
		    }
        } // switch
	} while (feof(fpRead) == 0);

    // Verification stage for continuity counter
    dbgLevel = 1;
    packetCounter = 0;
    fseek(fpRead,0,SEEK_SET);

	do {
	    packetCounter++;
	    DB_L0(" PACKET NUMBER %ld ",(UInt32)packetCounter);
	    printf("Do loop\n");
        // Read a new buffer from the file
	    nbytes = fread(bufts,1,TS_PACKET_SIZE*sizeof(UInt8),fpRead);
        if (nbytes != TS_PACKET_SIZE*sizeof(UInt8)) {
            if (nbytes == 0) {
                // we reached the end of the file
   			    printf(" No error, End of File\n");
		        exit(0);
		    }
		    else {
   			    printf(" End of File detected, nbytes = %d\n",nbytes);
		        exit(0);
		    }
	    }

	    if (bufts[0] != SYNC_BYTE) {
   	        printf(" ERROR not a TS packet, 0x%02x\n",bufts[0]);
   	        printf(" Resynchronizing ");
		    i = 1;
		    while ((bufts[i] != SYNC_BYTE)&&(i<TS_PACKET_SIZE-1)) i++;
   	        if ( i<(TS_PACKET_SIZE-1)) {
		        DB_L0(" Done after %d bytes\n",i);
			}
			else {
   	            DB_L00(" not found \n ");
		    }
		}

	    /* PID */
	    tmHword = ( (bufts[1]<<8) + bufts[2])& 0x1FFF;

	    switch (tmHword) {
	        case PAT_PID:
	    	    DB_L0(" PAT PID 0x%x\n",tmHword);
		    	break;
	        case CAT_PID:
	        	DB_L0(" CAT PID 0x%x\n",tmHword);
	            break;
	        case NULL_PID:
	    	    DB_L0(" NULL PID 0x%x\n",tmHword);
	            break;
	        default:
	        if ( ( tmHword >= ANY_PID_MIN ) || ( tmHword <= ANY_PID_MAX ) ) {
	    	    DB_L0(" ANY PID 0x%x \n",tmHword);
		        if (pat_p->pat_defined == TRUE) {
				    if ( tmHword == pat_p->program_map_PID ) {
            	    	    DB_L0(" PROGRAM MAP PID 0x%x\n",tmHword);
					}
			        if (pat_p->pmt_defined == TRUE) {
			            if ( tmHword == pat_p->video_PID ) {
	    	                DB_L00(" VIDEO STREAM PID\n");
	                        //  transport_error_indicator
		                    DB_L0("     transport_error_indicator    0x%02x\n",bufts[1]&0x80);
	                        // payload_unit_start_indicator
		                    DB_L0("     payload_unit_start_indicator 0x%02x\n",bufts[1]&0x40);
	                        // transport_priority
		                    DB_L0("     transport_priority           0x%02x\n",bufts[1]&0x20);
		                    // transport_scrambling_control
		                    DB_L0("     transport_scrambling_control 0x%02x\n",bufts[3]&0xC0);
		                    // adaptation_field_control
                            DB_L0("     adaptation_field_control     0x%02x\n",(bufts[3]&0x20));
                            if (bufts[3]&0x20) // get the adaptation field size
		                    DB_L0("     adaptation_field_size        %d\n",bufts[4]);
		                    // payload
		                    DB_L0("     payload                      0x%02x\n",bufts[3]&0x10);
		                    // continuity_counter
		                    DB_L0("     continuity_counter           0x%02x\n",bufts[3]&0x0F);
			                if (bufts[3]&0x0F != continuity_counter) {
					            DB_L0("     continuity_counter error %d\n", continuity_counter);
						    }
                            continuity_counter = (continuity_counter + 1)%16;

			                if (bufts[3]&0x20) // parse the adaptation field
					            parseAF(&bufts[4], af_p);

						    if (bufts[1]&0x40) // parse the PES header
						        parsePES(&bufts[4], pes_p);

				        } // if video
				        else if ( tmHword == pat_p->audio_PID ) {
	    	                DB_L0(" AUDIO STREAM PID 0x%x\n",tmHword);
						} // if audio
					} // if pmt_defined
		            else {
 	    	                DB_L00(" UNIDENTIFIED STREAM PID\n");
				    }
                } // if pat_defined
		        else {
 	    	        DB_L00(" UNIDENTIFIED STREAM PID\n");
				}
		    }
		    else {
	    	    DB_L0(" RESERVED PID 0x%x\n",tmHword);
		    }
        } // switch
	} while (feof(fpRead) == 0);



	return(0);
}
