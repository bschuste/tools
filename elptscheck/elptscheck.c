/***********************************************************************
 **
   Copyright (c) Electropix 2004. 
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
   Last update: 2004/06/23                                                  
   Modified:                                                   
   File: viSptscheck.c
   History:
 		Version 0.1	 2004/06/23
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
#define DB_L00x(a,b,c)  if (dbgLevel >  0) fprintf(PROUT,(a),(b),(c));
#define DB_L000(a,b,c,d)  if (dbgLevel >  0) fprintf(PROUT,(a),(b),(c),(d));
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
	UInt32              ESCR_base_hi;
	UInt32              ESCR_base;
//	double				ESCR_base;
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
	UInt32              stream_id_extension; //BS 2008
} pes_header_t, *pes_header_p;

#ifdef NEW_PARSEPES
viSerror_t parsePES(UInt8 **buf_pp, pes_header_p pesh_p);
#else
viSerror parsePES(UInt8 *buf_p, pes_header_p pesh_p) {
#endif
	
    viSerror err = VIS_OK;
    UInt8    pes_len;
    Int32    i;
#ifdef NEW_PARSEPES
    UInt8   *buf_p;

    buf_p = *buf_pp;
#endif
    
	pesh_p->stream_id = *buf_p;
	DB_L1("     stream_id                 0x%02lx\n",pesh_p->stream_id); 			    		    
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
		DB_L1("     PES_scrambling_control    0x%02lx\n",pesh_p->PES_scrambling_control); 			    
	    pesh_p->PES_priority              = (*buf_p&0x08)>>3;
		DB_L1("     PES_priority              0x%02lx\n",pesh_p->PES_priority); 			    
    	pesh_p->data_alignment_indicator  = (*buf_p&0x04)>>2;
		DB_L1("     data_alignment_indicator  0x%02lx\n",pesh_p->data_alignment_indicator); 			    
	    pesh_p->copyright                 = (*buf_p&0x02)>>1;
		DB_L1("     copyright                 0x%02lx\n",pesh_p->copyright); 			    
	    pesh_p->original_or_copy          = (*buf_p&0x01);
		DB_L1("     original_or_copy          0x%02lx\n",pesh_p->original_or_copy); 			    
	    buf_p++;
	    pesh_p->PTS_DTS_flags             = (*buf_p&0xc0)>>6;
		DB_L1("     PTS_DTS_flags             0x%02lx\n",pesh_p->PTS_DTS_flags); 			    
	    pesh_p->ESCR_flag                 = (*buf_p&0x20)>>5;
		DB_L1("     ESCR_flag                 0x%02lx\n",pesh_p->ESCR_flag); 			    
	    pesh_p->ES_rate_flag              = (*buf_p&0x10)>>4;
		DB_L1("     ES_rate_flag              0x%02lx\n",pesh_p->ES_rate_flag); 			    
	    pesh_p->DSM_trick_mode_flag       = (*buf_p&0x08)>>3;
		DB_L1("     DSM_trick_mode_flag       0x%02lx\n",pesh_p->DSM_trick_mode_flag); 			    
	    pesh_p->additional_copy_info_flag = (*buf_p&0x04)>>2;
		DB_L1("     additional_copy_info_flag 0x%02lx\n",pesh_p->additional_copy_info_flag); 			    
	    pesh_p->PES_CRC_flag              = (*buf_p&0x02)>>1;
		DB_L1("     PES_CRC_flag              0x%02lx\n",pesh_p->PES_CRC_flag); 			    
	    pesh_p->PES_extension_flag        = (*buf_p&0x01);
		DB_L1("     PES_extension_flag        0x%02lx\n",pesh_p->PES_extension_flag); 			    
	    buf_p++;
	    pesh_p->PES_header_data_length    = (*buf_p);
		DB_L1("     PES_header_data_length    0x%02lx\n",pesh_p->PES_header_data_length); 	
	    buf_p++;
#if 0
		if (((*buf_p & 0xF0) != 0x30)&((*buf_p & 0xF0) != 0x20)) {
//			DB_L0("     UNEXPECTED VALUE IN PES HEADER %02x\n",(*buf_p & 0xF0));
		}				    
#endif
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
//            DB_L0("     PTS                                      0x%ld",(pesh_p->PTS_hi>>31)&0x00000001);
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
//            DB_L0("     PTS                                      0x%ld",(pesh_p->PTS_hi>>31)&0x00000001);
//            DB_L0("%04lx\n",(pesh_p->PTS_hi<<1) + pesh_p->PTS_lo);
//            DB_L0("     DTS                                      0x%ld",(pesh_p->DTS_hi>>31)&0x00000001);
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
    		DB_L1("     ESCR_base             0x%04lx\n",pesh_p->ESCR_base); 	
    	    pesh_p->ESCR_extension       = (*buf_p&0x03)>>1;
		    buf_p+=1;	
    	    pesh_p->ESCR_extension      += (*buf_p&0xfe)>>1;
    		DB_L1("     ESCR_extension        0x%04lx\n",pesh_p->ESCR_extension); 	
		    buf_p+=2;	
        }
	    if (pesh_p->ES_rate_flag == 1) {
    	    pesh_p->ES_rate              = ((*buf_p&0x7f)<<15) + ((*(buf_p+1))<<7) + ((*(buf_p+2)&0xfe)>>1);
    		DB_L1("     ES_rate               0x%04lx\n",pesh_p->ES_rate); 	
		    buf_p+=3;	
        }
	    if (pesh_p->DSM_trick_mode_flag == 1) {
        	pesh_p->trick_mode_control       = (*buf_p&0xe0)>>5;
	        if (pesh_p->trick_mode_control == FAST_FORWARD) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L1("     field_id              0x%02lx\n",pesh_p->field_id); 	
	            pesh_p->intra_slice_refresh  = (*buf_p&0x04)>>2;
    		    DB_L1("     intra_slice_refresh   0x%02lx\n",pesh_p->intra_slice_refresh); 	
            	pesh_p->frequency_truncation = (*buf_p&0x03);
    		    DB_L1("     frequency_truncation  0x%02lx\n",pesh_p->frequency_truncation); 	
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == SLOW_MOTION) {
	            pesh_p->rep_cntrl            = (*buf_p&0x1f);
    		    DB_L1("     rep_cntrl             0x%02lx\n",pesh_p->rep_cntrl); 	
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == FREEZE_FRAME) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L1("     field_id              0x%02lx\n",pesh_p->field_id); 	
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == FAST_REVERSE) {
	            pesh_p->field_id             = (*buf_p&0x18)>>3;
    		    DB_L1("     field_id 0x%02lx\n",pesh_p->field_id); 	
	            pesh_p->intra_slice_refresh  = (*buf_p&0x04)>>2;
    		    DB_L1("     intra_slice_refresh   0x%02lx\n",pesh_p->intra_slice_refresh); 	
            	pesh_p->frequency_truncation = (*buf_p&0x03);
    		    DB_L1("     frequency_truncation  0x%02lx\n",pesh_p->frequency_truncation); 	
        	    buf_p++;
    	    }
	        else if (pesh_p->trick_mode_control == SLOW_REVERSE) {
	            pesh_p->rep_cntrl            = (*buf_p&0x1f);
    		    DB_L1("     rep_cntrl                        0x%02lx\n",pesh_p->rep_cntrl); 	
        	    buf_p++;
    	    }
	    }
		if (pesh_p->additional_copy_info_flag == 1) {
	        pesh_p->additional_copy_info     = (*buf_p&0x7f);
    		DB_L1("     additional_copy_info                 0x%02lx\n",pesh_p->additional_copy_info); 	
        	buf_p++;
	    }
		if (pesh_p->PES_CRC_flag == 1) {
        	pesh_p->previous_PES_packet_CRC  = (*buf_p)<<8;
        	buf_p++;
        	pesh_p->previous_PES_packet_CRC += (*buf_p);
    		DB_L1("     previous_PES_packet_CRC            0x%04lx\n",pesh_p->previous_PES_packet_CRC); 	
        	buf_p++;
	    }
		if (pesh_p->PES_extension_flag == 1) {
        	pesh_p->PES_private_data_flag                = (*buf_p&0x80)>>7;
    		DB_L1("     PES_private_data_flag                0x%02lx\n",pesh_p->PES_private_data_flag); 	
	        pesh_p->pack_header_field_flag               = (*buf_p&0x40)>>6;
    		DB_L1("     pack_header_field_flag               0x%02lx\n",pesh_p->pack_header_field_flag); 	
	        pesh_p->program_packet_sequence_counter_flag = (*buf_p&0x20)>>5;
    		DB_L1("     program_packet_sequence_counter_flag 0x%02lx\n",pesh_p->program_packet_sequence_counter_flag); 	
	        pesh_p->P_STD_buffer_flag                    = (*buf_p&0x10)>>4;
    		DB_L1("     P_STD_buffer_flag                    0x%02lx\n",pesh_p->P_STD_buffer_flag); 	
	        pesh_p->PES_extension_flag_2                 = (*buf_p&0x01);
    		DB_L1("     PES_extension_flag_2                 0x%02lx\n",pesh_p->PES_extension_flag_2); 	
        	buf_p++;
		
		    if (pesh_p->PES_private_data_flag == 1) {
		        // TBD:128 bits value
	            pesh_p->PES_private_data                 = (*buf_p);
        		DB_L1("     PES_private_data                 0x%02lx\n",pesh_p->PES_private_data); 	
            	buf_p+=16;
	        }	    
		    if (pesh_p->pack_header_field_flag == 1) {
    	        pesh_p->pack_field_length                = (*buf_p);
        		DB_L1("     pack_field_length                0x%02lx\n",pesh_p->pack_field_length); 	
            	buf_p++;
		        // TBD:pack_header();
	        }
	        if (pesh_p->program_packet_sequence_counter_flag == 1) {
	            pesh_p->program_packet_sequence_counter  = (*buf_p&0x7f);
        		DB_L1("     program_packet_sequence_counter  0x%02lx\n",pesh_p->program_packet_sequence_counter); 	
            	buf_p++;
	            pesh_p->MPEG1_MPEG2_identifier           = (*buf_p&0x40)>>6;;
        		DB_L1("     MPEG1_MPEG2_identifier           0x%02lx\n",pesh_p->MPEG1_MPEG2_identifier); 	
	            pesh_p->original_stuff_length            = (*buf_p&0x3f);
        		DB_L1("     original_stuff_length            0x%02lx\n",pesh_p->original_stuff_length); 	
            	buf_p++;
	        }
	        if (pesh_p->P_STD_buffer_flag == 1) {
	            pesh_p->P_STD_buffer_scale               = (*buf_p&0x20)>>5;
        		DB_L1("     P_STD_buffer_scale               0x%02lx\n",pesh_p->P_STD_buffer_scale); 	
	            pesh_p->P_STD_buffer_size                = (*buf_p&0x1f)<<8;
            	buf_p++;
	            pesh_p->P_STD_buffer_size               += (*buf_p);
        		DB_L1("     P_STD_buffer_size                0x%02lx\n",pesh_p->P_STD_buffer_size); 	
            	buf_p++;
	        }
	        if (pesh_p->PES_extension_flag_2 == 1) {
	            pesh_p->PES_extension_field_length       = (*buf_p&0x7f);
        		DB_L1("     PES_extension_field_length       0x%02lx\n",pesh_p->PES_extension_field_length); 	
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
#ifdef NEW_PARSEPES
    if (buf_p - *buf_pp < pesh_p->PES_header_data_length + 9) {
        DB_L1("   header length   %02d\n", pesh_p->PES_header_data_length + 6); 
    }
    *buf_pp += pesh_p->PES_header_data_length + 6; //buf_p;
#endif
    return(err);
}

#ifdef NEW_PARSEPACK
viSerror_t parsePACK(UInt8 **buf_pp, pack_header_p pah_p);
#else
viSerror parsePACK(UInt8 *buf_p, pack_header_p pah_p) {
#endif

    viSerror err = VIS_OK;
    UInt8    pack_len;
    Int32    i;
#ifdef NEW_PARSEPACK
    UInt8   *buf_p;

    buf_p = *buf_pp;
#endif
   
	pah_p->SCR_base_msb = (*buf_p & 0x20)>>5; //1 bit, should be 0

	pah_p->SCR_base  = (*buf_p & 0x18)<<26; //2 bits
	pah_p->SCR_base |= (*buf_p & 0x03)<<27; //2 bits
	buf_p++;
	pah_p->SCR_base |= (*buf_p)<<19;		//8 bits
	buf_p++;
	pah_p->SCR_base |= (*buf_p & 0xF8)<<11; //5 bits
	pah_p->SCR_base |= (*buf_p & 0x03)<<12; //2 bits
	buf_p++;
	pah_p->SCR_base |= (*buf_p)<<5;		    //8 bits
	buf_p++;
	pah_p->SCR_base |= (*buf_p & 0xF8)>>3;  //5 bits

	buf_p++;
	pah_p->SCR_extension = (*buf_p);
	DB_L1("     SCR_extension             %02x\n",pah_p->SCR_extension); 			    
	buf_p++;
	pah_p->SCR = ( (double)(pah_p->SCR_base) * 300.0 + (double)pah_p->SCR_extension );
	DB_L1("     SCR_base                  %6.2f\n",(double)(pah_p->SCR_base)); 			    
	DB_L1("     SCR                       %6.2f\n",(double)(pah_p->SCR)); 			    

	pah_p->program_mux_rate  = (*buf_p & 0xFF)<<14; //8 bits
	buf_p++;
	pah_p->program_mux_rate |= (*buf_p & 0xFF)<<6;  //8 bits
	buf_p++;
	pah_p->program_mux_rate |= (*buf_p & 0xFC)>>2;  //6 bits
	DB_L1("     program_mux_rate          %d\n",pah_p->program_mux_rate); 			    
	buf_p++;
	if ((*buf_p & 0xF8) != 0xF8) {
		DB_L0("     UNEXPECTED VALUE      %02x\n",(*buf_p & 0xF8));
	}
	pah_p->pack_stuffing_length = (*buf_p & 0x07); //BS 2008 was 0x03
	buf_p++;
	DB_L1("     pack_stuffing_length      %d\n",pah_p->pack_stuffing_length); 			    
    for (i=0;i<pah_p->pack_stuffing_length;i++) {
	    buf_p++;
	}
#ifdef NEW_PARSEPACK
    *buf_pp = buf_p;
#endif
    return(err);
}

viSerror visFileGetNextStartCode(FILE *fpRead, UInt8 *pTmp, UInt32 *nbBytes) {

    viSerror err = VIS_OK;   
  	UInt8 tmp0=1;  		      /* current byte */
  	UInt8 tmp1=1;		      /* previous byte */
  	UInt8 tmp2=1;		      /* previous previous byte */
  	UInt8 startCode = FALSE;  /* to TRUE when the start code is detected */
	UInt32 count = 0;
	Int nbytes;
	  
	*nbBytes = 0;
	while (startCode == FALSE) {
    	tmp2 = tmp1; 
		tmp1 = tmp0;
  		/* Read a byte from the input buffer */
		nbytes = fread(&tmp0,1,sizeof(UInt8),fpRead);
		if (nbytes != sizeof(UInt8)) {
		    return VIS_EOF;
		}
		count++;
		
	    if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			/* Start code detected */
 			startCode = TRUE;
		}
	}
	/* Get the start code value */
	nbytes = fread(pTmp,1,sizeof(UInt8),fpRead);
	if (nbytes != sizeof(UInt8)) {
		return VIS_EOF;
	}
	*nbBytes = count;
	return VIS_OK;	
}

int main(int argc, char **argv)
{
	FILE	*fpRead;
	char	ReadName[256];
	char	WriteName[256];
	int     i;
	int     nbytes;
	UInt8   tmByte = 0;
	UInt16  tmHword = 0;
	UInt32  tmWord = 0;
  	UInt8   startCode = FALSE;  /* to TRUE when the start code is detected */
	UInt8   tmp0 = 1,tmp1 = 1,tmp2 = 1;
	UInt8   bufvob[VOB_PACKET_SIZE];
	UInt32  packetCounter = 0.0; 
	UInt32  program_number  = 0;	
    UInt32  stream_type = 0;
    UInt32  ES_info_length = 0;
    UInt32  len = 0;
    UInt32  continuity_counter = 0;
	pes_header_p    pes_p;
	pack_header_p   pah_p;
	system_header_p syh_p; 
    viSerror err = VIS_OK;   
	double audioPTS, videoPTS;
	Bool noSystemPacket = False;

	dbgLevel = 2;
    audioPTS = 0.0;
	videoPTS = 0.0;

	if ( argc < 2 ) {
		printf("%s input_file_name\n",argv[0]);
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
	
	fprintf(stderr," Checking file header\n");
	if (visFileGetNextStartCode(fpRead, &tmByte, &nbytes) != VIS_OK) {
   	    printf(" Not a Program Stream\n");
	    exit(-1);
	}
	// Check for pack_start_code 
    if (tmByte != PACK_START_CODE_VALUE) 
    {
   	 printf(" Not a Program Stream but could be a PES\n");
	    noSystemPacket = True;
	    exit(-1);
	}
    
	fclose(fpRead);
	if ((fpRead=fopen(ReadName,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName);
		return(VIS_ERR_OPEN_FILE);
	}
    
//    fseek(fpRead,-4,SEEK_SET); //rewind start code offset
//    fseek(fpRead,0,SEEK_SET);
      
	/* allocate memory for the structures */
  	if ((pah_p = (pack_header_p) malloc(sizeof(pack_header_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(pah_p,sizeof(pack_header_t),0);
  	if ((syh_p = (system_header_p) malloc(sizeof(system_header_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(syh_p,sizeof(system_header_t),0);
  	if ((pes_p = (pes_header_p) malloc(sizeof(pes_header_t))) == NULL) return(VIS_ERR_MEM_ALLOC);
    memset(pes_p,sizeof(pes_header_t),0);
    
//    dbgLevel = 1;
	fprintf(stderr," Checking file content\n");

	do { 

	    DB_L1(" PACKET NUMBER %ld \n",(UInt32)packetCounter);
        // Read a new buffer from the file
	    nbytes = fread(bufvob,1,VOB_PACKET_SIZE*sizeof(UInt8),fpRead);
        if (nbytes != VOB_PACKET_SIZE*sizeof(UInt8)) {
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

	    
	    if (bufvob[3] != PACK_START_CODE_VALUE) {
   	        printf(" ERROR not a PACK, 0x%02x\n",bufvob[3]);
   	        printf(" Resynchronizing ");
		    i = 1;
			while ((startCode == FALSE)&&(i<VOB_PACKET_SIZE-1)) {
    			tmp2 = tmp1; 
				tmp1 = tmp0;
				tmp0 = bufvob[i];
			    if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 					/* Start code detected */
 					startCode = TRUE;
				}
				i++;
			}
   	        if ( i<(VOB_PACKET_SIZE-1)) {
		        DB_L0(" Done after %d bytes\n",i);
			}
			else {
   	            DB_L00(" not found \n ");
				continue;
		    }
		}
	        
	    // Parse the pack header
		err = parsePACK(&bufvob[4], pah_p);

		
		// Retrieve the next start code
	    tmWord = (bufvob[14]<<24)|(bufvob[15]<<16)|(bufvob[16]<<8)|(bufvob[17]);
	    for (i=0;i<32;i++)
	    {
	    	DB_L1(" 0x%02x", bufvob[i]);
	    	if (i%4==3) DB_L10("\n");
	    }
	    // Identify and parse the packet
	    
	    switch (tmWord) {
	        case (0x00000100 | SYSTEM_HEADER_START_CODE_VALUE):
	    	    DB_L1("     SYSTEM HEADER START CODE 0x%x\n",tmWord);
				//Do nothing
		    	break;
	        case (0x00000100 | VIDEO_STREAM_ID_MIN):
	    	    DB_L10("     VIDEO PACKET\n");
				err = parsePES(&bufvob[17], pes_p);
				videoPTS = pes_p->PTS;
        		if (videoPTS < pah_p->SCR_base) {
		        	DB_L0(" VIDEO is BEFORE SCR, PACKET %ld\n", packetCounter);
		        }
        		if (videoPTS < (double)pah_p->SCR_base + 10000.0) {
DB_L000(" VIDEO is NEAR SCR, PACKET %ld, PTS = %8.1f, SCR = %8.1f\n", packetCounter,videoPTS,(double)pah_p->SCR_base);
		        }
                if (pah_p->SCR_base <= 10000) {
//                    DB_L00x("VIDEO: SCR is NULL %ld, PACKET %ld\n", pah_p->SCR_base, packetCounter);
    DB_L000(" VIDEO: PACKET %ld, PTS = %8.1f, SCR = %8.1f\n", packetCounter,videoPTS,(double)pah_p->SCR_base);
                }
		    	break;
	        case (0x00000100 | PRIVATE_STREAM_1_VALUE):
	    	    DB_L10("     PRIVATE STREAM 1 PACKET\n");
				err = parsePES(&bufvob[17], pes_p);
				audioPTS = pes_p->PTS;
        		if (audioPTS < (double)pah_p->SCR_base) {
DB_L000(" AUDIO is BEFORE SCR, PACKET %ld, PTS = %8.1f, SCR = %8.1f\n", packetCounter,audioPTS,(double)pah_p->SCR_base);
		        }        
		    	break;
	        case (0x00000100 | AUDIO_STREAM_ID_MIN):
	    	    DB_L10("     AUDIO STREAM PACKET\n");
				err = parsePES(&bufvob[17], pes_p);
				audioPTS = pes_p->PTS;
        		if (audioPTS < (double)pah_p->SCR_base) {
DB_L000(" AUDIO is BEFORE SCR, PACKET %ld, PTS = %8.1f, SCR = %8.1f\n", packetCounter,audioPTS,(double)pah_p->SCR_base);
		        }        
                if (pah_p->SCR_base <= 10000) {
                    DB_L00x("AUDIO: SCR is NULL %ld, PACKET %ld\n", pah_p->SCR_base, packetCounter);
                }
		    	break;
			default: 
				if ( (tmWord & 0xFFFFFF00) == 0x00000100 ) {
		    	    DB_L0("     UNEXPECTED START CODE 0x%x \n", (tmWord&0xFF));
				}
				else {
		    	    DB_L0("     UNEXPECTED WORD 0x%x exiting...\n", tmWord);
					exit(-1);
				}
        } // switch
	    packetCounter++;
		if ((videoPTS > audioPTS + 10000)||(videoPTS < audioPTS - 10000)) {
//			DB_L0(" VIDEO and AUDIO ARE OUT OF SYNC, PACKET %ld\n", packetCounter);
		}
	} while ((feof(fpRead) == 0)); //&(packetCounter<(105000*MINUTES))); //1mn
    
	fclose(fpRead);
	fclose(fpWrite);

	return(0);
}
