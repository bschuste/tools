/***********************************************************************
 **
   Copyright (c) Electropix 2002-2009.
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
   Last update: 2004/12/29
   Modified:
   File: h264parse.c
   History:
 		Version 0.1	 2002/06/29
 **
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "viSportab.h"
#include "viSdefs.h"
#include "viSerror.h"

/*!*********************************************************************
 ** NALU types
 */
#define   NALU_SLICE_START_CODE_VALUE    1
#define   NALU_DPA_START_CODE_VALUE      2
#define   NALU_DPB_START_CODE_VALUE      3
#define   NALU_DPC_START_CODE_VALUE      4
#define   NALU_IDR_START_CODE_VALUE      5
#define   NALU_SEI_START_CODE_VALUE      6
#define   NALU_SPS_START_CODE_VALUE      7
#define   NALU_PPS_START_CODE_VALUE      8
#define   NALU_AUD_START_CODE_VALUE      9
#define   NALU_EOSEQ_START_CODE_VALUE    10
#define   NALU_EOSTREAM_START_CODE_VALUE 11
#define   NALU_FILL_START_CODE_VALUE     12

#define   MAXSPS                         32
#define   MAXPPS                         128

/*!*********************************************************************
 ** Hypothetical Reference Decoder(HRD) Structure
 */
#define   MAX_CBP_CNT   32
typedef struct
{
    UInt32          valid;                                   // Validation flag. ( = 0xdeadbeef/0xaaaababe)
    UInt32          cpb_cnt;                                 // ue(v)
    UInt16          bit_rate_scale;                          // u(4)
    UInt16          cpb_size_scale;                          // u(4)
    UInt32          bit_rate_value[MAX_CBP_CNT];             // ue(v)
    UInt32          cpb_size_value[MAX_CBP_CNT];             // ue(v)
    UInt16          vbr_cbr_flag[MAX_CBP_CNT];               // u(1)
    UInt16          initial_cpb_removal_delay_length_minus1; // u(5)
    UInt16          cpb_removal_delay_length_minus1;         // u(5)
    UInt16          dpb_output_delay_length_minus1;          // u(5)
    UInt16          time_offset_length;                      // u(5)
} hypRefDecoder_t, *hypRefDecoder_p;

/*!*********************************************************************
 ** Video Usability Information (VUI) Structure
 */
typedef struct
{
    UInt32          valid;                                   // Validation flag. ( = 0xdeadbeef/0xaaaababe)
    UInt16          aspect_ratio_info_present_flag; // u(1)
    UInt16          aspect_ratio_idc;               // u(8)
    UInt32          sar_width;                      // u(16)
    UInt32          sar_height;                     // u(16)
    UInt16          overscan_info_present_flag;     // u(1)
    UInt16          overscan_appropriate_flag;      // u(1)
    UInt16          video_signal_type_present_flag; // u(1)
    UInt16          video_format;                   // u(3)
    UInt16          video_full_range_flag;          // u(1)
    UInt16          colour_description_present_flag;// u(1)
    UInt16          colour_primaries;               // u(8)
    UInt16          transfer_characteristics;       // u(8)
    UInt16          matrix_coefficients;            // u(8)
    UInt16          chroma_location_info_present_flag;// u(1)
    UInt32          chroma_location_frame;          // ue(v)
    UInt32          chroma_location_field;          // ue(v)
    UInt16          timing_info_present_flag;       // u(1)
    UInt32          num_units_in_tick;              // u(32)
    UInt32          time_scale;                     // u(32)
    UInt16          fixed_frame_rate_flag;          // u(1)
    UInt16          nal_hrd_parameters_present_flag;// u(1)
    hypRefDecoder_t nal_hrd_parameters;
    UInt16          vcl_hrd_parameters_present_flag;// u(1)
    hypRefDecoder_t vcl_hrd_parameters;
    UInt16          low_delay_hrd_flag;             // u(1)
    UInt16          bitstream_restriction_flag;     // u(1)
    UInt16          motion_vectors_over_pic_boundaries_flag; // u(1)
    UInt32          max_bytes_per_pic_denom;        // ue(v)
    UInt32          max_bits_per_mb_denom;          // ue(v)
    UInt32          log2_max_mv_length_vertical;    // ue(v)
    UInt32          log2_max_mv_length_horizontal;  // ue(v)
    UInt32          max_dec_frame_reordering;       // ue(v)
    UInt32          max_dec_frame_buffering;        // ue(v)
} vidUseInfo_t, *vidUseInfo_p;

/*!*********************************************************************
 ** Picture Parameter Set (PPS) Structure
 */
#define   MAX_SLICE_GROUPS    8
typedef struct
{
    UInt32          valid;                                   // Validation flag. ( = 0xdeadbeef/0xaaaababe)
    UInt16          pic_parameter_set_id;         // ue(v)
    UInt16          seq_parameter_set_id;         // ue(v)
    UInt16          entropy_coding_mode_flag;     // u(1)
    UInt16          pic_order_present_flag;       // u(1)
    UInt16          num_slice_groups_minus1;      // ue(v)
    UInt16          slice_group_map_type;         // ue(v)
    UInt32          run_length_minus1[MAX_SLICE_GROUPS]; // ue(v)
    UInt32          top_left[MAX_SLICE_GROUPS];   // ue(v)
    UInt32          bottom_right[MAX_SLICE_GROUPS]; // ue(v)
    UInt16          slice_group_change_direction_flag; // u(1)
    UInt16          slice_group_change_rate_minus1; // ue(v)
    UInt32          num_slice_group_map_units_minus1; // ue(v)
    UInt32*         slice_group_id;     // u(v)*...
    UInt16          num_ref_idx_l0_active_minus1; // ue(v)
    UInt16          num_ref_idx_l1_active_minus1; // ue(v)
    UInt16          weighted_pred_flag; // u(1)
    UInt16          weighted_bipred_idc;// u(2)
    Int32           pic_init_qp_minus26;// se(v)
    Int32           pic_init_qs_minus26;// se(v)
    Int32           chroma_qp_index_offset; // se(v)
    UInt16          deblocking_filter_control_present_flag; // u(1)
    UInt16          constrained_intra_pred_flag;            // u(1)
    UInt16          redundant_pic_cnt_present_flag;         // u(1)
    UInt16          vui_pic_parameters_flag;                // u(1)
}picParSet_t, *picParSet_p;


/*!*********************************************************************
 ** Sequence Parameter Set (SPS) Structure
 ** raw byte sequence payload (RBSP): A syntax structure containing an
 ** integer number of bytes that is encapsulated in a NAL unit.
 ** An RBSP is either empty or has the form of a string of data bits
 ** containing syntax elements followed by an RBSP stop bit and followed
 ** by zero or more subsequent bits equal to 0.
 */
#define   MAX_REF_FRAMES_IN_POC_CYCLE   255
typedef struct
{
    UInt32          valid;                                 // validation flag. ( = 0xdeadbeef/0xaaaababe)
    UInt16          profile_idc;                           // u(8)
    UInt16          constrained_set0_flag;                 // u(1)
    UInt16          constrained_set1_flag;                 // u(1)
    UInt16          constrained_set2_flag;                 // u(1)
    UInt16          level_idc;                             // u(8)
    UInt16          seq_parameter_set_id;                  // ue(v)
    UInt32          log2_max_frame_num_minus4;             // ue(v)
    UInt16          pic_order_cnt_type;                    // ue(v)
    UInt32          log2_max_pic_order_cnt_lsb_minus4;     // ue(v)
    UInt16          delta_pic_order_always_zero_flag;      // u(1)
    Int32           offset_for_non_ref_pic;                // se(v)
    Int32           offset_for_top_to_bottom_field;        // se(v)
    UInt32          num_ref_frames_in_pic_order_cnt_cycle; // ue(v)
    Int32           offset_for_ref_frame[MAX_REF_FRAMES_IN_POC_CYCLE];   // se(v)
    UInt16          num_ref_frames;                        // ue(v)
    UInt16          gaps_in_frame_num_value_allowed_flag;  // u(1)
    UInt32          pic_width_in_mbs_minus1;               // ue(v)
    UInt32          pic_height_in_map_units_minus1;        // ue(v)
    UInt16          frame_mbs_only_flag;                   // u(1)
    UInt16          mb_adaptive_frame_field_flag;          // u(1)
    UInt16          direct_8x8_inference_flag;             // u(1)
    UInt16          frame_cropping_flag;                   // u(1)
    UInt32          frame_cropping_rect_left_offset;       // ue(v)
    UInt32          frame_cropping_rect_right_offset;      // ue(v)
    UInt32          frame_cropping_rect_top_offset;        // ue(v)
    UInt32          frame_cropping_rect_bottom_offset;     // ue(v)
    UInt16          vui_parameters_present_flag;           // u(1)
    vidUseInfo_t    vui_seq_parameters;
} seqParSet_t, *seqParSet_p;

/*!*********************************************************************
 ** Structure for managing buffers and associated file
 */
typedef struct {
    FILE*           fp;             /* Current file */
  	UInt8*			buf; 			/* buffer for the io of the file */
  	Int32 			bc;            	/* current index of the payload  */
  	Int32 			max;            /* max index of the buffer (actual memory size of the payload) */
	Int32           byteCounter;    /* count the number of bytes read from the buffer since initialization  */
    Int32           limit;
    UInt32          word;
    UInt32          mask;
    Int32           avail_bits;
} bufManager_t, *bufManager_p;

/***********************************************************************
 ** Debug level printout
 */
#define PROUT             fpWrite

/***********************************************************************
 ** Global definitions
 */
#define SCAN_BUFFER_SIZE  16384
#define SCAN_LIMIT        10 * 1024 * 1024
#define PIC_LIMIT         4
#define MIN_SC_FOUND      100
#define True              1
#define False             0
#define LEVEL1            1
#define LEVEL0            0
#define LOG_MAX_SIZE      4096

/***********************************************************************
 * Module Global Variables
 */
Int8  dbgLevel;
FILE *fpWrite;
//FILE *fpWrite_bin;
UInt8 scanBuffer[SCAN_BUFFER_SIZE];
//Int   copy = False;
FILE *fprout;
static Int32  g_dbgLevel = 0;


#ifndef VIS_RETAIL
#define DB_L00(a)  if (dbgLevel >  0) fprintf(fprout,(a));
#define DB_L0(a,b) if (dbgLevel >  0) fprintf(fprout,(a),(b));
#define DB_L1(a,b) if (dbgLevel >  1) fprintf(fprout,(a),(b));
#define DB_L10(a)  if (dbgLevel >  1) fprintf(fprout,(a));
#define DB_L2(a,b) if (dbgLevel >  2) fprintf(fprout,(a),(b));
#define DB_L20(a)  if (dbgLevel >  2) fprintf(fprout,(a));
#define DB_L3(a,b) if (dbgLevel >  3) fprintf(fprout,(a),(b));
#define DB_L30(a)  if (dbgLevel >  3) fprintf(fprout,(a));
#define DB_L4(a,b) if (dbgLevel >  4) fprintf(fprout,(a),(b));
#define DB_L40(a)  if (dbgLevel >  4) fprintf(fprout,(a));
#endif


void dbgPrintf (int level, char *format, ...) {
    va_list   va;
    Int32     nb;
    Char      mystring[LOG_MAX_SIZE];

    if (g_dbgLevel > level) {
        va_start(va, format);
        nb = vsprintf(mystring, format, va);
        if (nb > LOG_MAX_SIZE) {
        }
        printf("%s",mystring);
        va_end(va);
    }
}

#define DBGPTF(a,b) dbgPrintf(a,b)


typedef struct
{
    Int32        width;
    Int32        height;
    Int32        nbPictures;
	bufManager_t bufMan;
    seqParSet_t  sps;
    picParSet_t  pps;
} session_t, *session_p;

viSerror elFileInitBufManager(bufManager_p buf_p, UInt8* pu8Buf) {

	buf_p->buf         = pu8Buf;
    buf_p->fp          = Null;
	buf_p->max         = 0;
	buf_p->bc          = buf_p->max; /* buffer is empty */
	buf_p->byteCounter = 0;  /* Is currently an Int32 should we use a Double? */

    return VIS_OK;
}

//! Get next byte from the file using temporary buffer
viSerror elFileInitBuffer(bufManager_p buf_p) {

	buf_p->max = 0;
	buf_p->bc  = buf_p->max; /* buffer is empty */
    buf_p->avail_bits = 0;
    return VIS_OK;
}

//! Get next byte from the file using temporary buffer
viSerror elFileGetByte(bufManager_p buf_p, UInt8 *tmp) {

    viSerror err = VIS_OK;

	if (buf_p->bc >= buf_p->max) { 		// Test if buffer is empty
	    buf_p->bc = 0 ; 		 	// Signal buffer is getting loaded

    	if ((feof(buf_p->fp)==0)) {
      		/* Read a new buffer from the file */
			buf_p->max = fread(buf_p->buf,1,SCAN_BUFFER_SIZE*sizeof(UInt8),buf_p->fp);
      		if (buf_p->max != SCAN_BUFFER_SIZE*sizeof(UInt8)) {
   			    DB_L1(" elFileGetByte::End of File detected, buf_p->max = %ld\n",buf_p->max);
			}
    	}
    	else // we reached the end of the file
      		return VIS_EOF;
  	}
   *tmp = buf_p->buf[buf_p->bc];
#if 0
    if ((copy == True)&(buf_p->limit > buf_p->byteCounter)) {
#ifdef FILE_OUTPUT
        fprintf(fpWrite,"0x%02x",*tmp);
        fprintf(fpWrite,",");
        if (buf_p->byteCounter % 16 == 15) fprintf(fpWrite,"\n");
#endif
        fputc(*tmp,fpWrite_bin);
    }
#endif
  	buf_p->bc++;
	buf_p->byteCounter++;
	return err;
}
//! Get next byte from the file using temporary buffer
viSerror elFileGetWord(bufManager_p buf_p, UInt32 *tmWord) {

    viSerror err = VIS_OK;
    UInt8 tmByte;

    err = elFileGetByte(buf_p, &tmByte);
    *tmWord = tmByte;
    if (err) {
        buf_p->avail_bits = 8;
        return err;
    }
    *tmWord = *tmWord<<8;
    err = elFileGetByte(buf_p, &tmByte);
    *tmWord |= tmByte;
    if (err) {
        buf_p->avail_bits = 16;
        return err;
    }
    *tmWord = *tmWord<<8;
    err = elFileGetByte(buf_p, &tmByte);
    *tmWord |= tmByte;
    if (err) {
        buf_p->avail_bits = 24;
        return err;
    }
    *tmWord = *tmWord<<8;
    err = elFileGetByte(buf_p, &tmByte);
    *tmWord |= tmByte;
    buf_p->avail_bits = 32;

	return err;
}

viSerror elFileGetNextStartCode(bufManager_p buf_p, UInt8 *pTmp, UInt32 *nbBytes) {

    viSerror err = VIS_OK;
  	UInt8 tmp0=1;  		      /* current byte */
  	UInt8 tmp1=1;		      /* previous byte */
  	UInt8 tmp2=1;		      /* previous previous byte */
  	UInt8 startCode = FALSE;  /* to TRUE when the start code is detected */
	UInt32 count = 0;

	*nbBytes = 0;
	while (startCode == FALSE) {
    	tmp2 = tmp1;
		tmp1 = tmp0;
  		/* Read a byte from the input buffer */
		err = elFileGetByte(buf_p, &tmp0);
		if (err == VIS_EOF) {
		    return err;
		}
		count++;

	    if((tmp1==0) && (tmp2==0) && (tmp0==1)) {
 			/* Start code detected */
 			startCode = TRUE;
		}
	}
	err = elFileGetByte(buf_p, pTmp); /* Get the start code value */
	*nbBytes = count;
	return err;
}

viSerror elFileGetStreamPosition(bufManager_p buf_p, UInt32 *tmp){

    viSerror err = VIS_OK;

	*tmp = ftell(buf_p->fp) - SCAN_BUFFER_SIZE + (buf_p->bc);
	return err;
}

#if 0
init_bits() {
  word = *p32_buf;
  avail_bits = 32;
  mask = 0xffffffff;
}
#endif

viSerror read_bits(bufManager_p buf_p, Int32 a, UInt32 *val32) {
    viSerror err = VIS_OK;
    Int32 ra, rb;
    UInt32 tmp;

  ra = buf_p->avail_bits - a;
  rb = a - buf_p->avail_bits;
  DB_L1("ra = %ld\n",ra);
  DB_L1("rb = %ld\n",rb);
  if (buf_p->avail_bits == a) {
    *val32 = buf_p->word;
    err = elFileGetWord(buf_p,&buf_p->word);
    buf_p->avail_bits = 32;
    buf_p->mask = 0xffffffff;
  }
  else if (ra > 0) {
    DB_L1("buf_p->word = 0x%08lx\n",buf_p->word);
    DB_L1("buf_p->mask = 0x%08lx\n",buf_p->mask);
    DB_L1("buf_p->avail_bits = %ld\n",buf_p->avail_bits);
    *val32 = buf_p->word >> ra;
    DB_L1("*val32 = 0x%08lx\n",*val32);
    buf_p->mask = buf_p->mask >> a;
    buf_p->word = buf_p->word & buf_p->mask;
    buf_p->avail_bits -= a;
  }
  else {
    *val32 = buf_p->word << rb;
    err = elFileGetWord(buf_p,&tmp);
    buf_p->mask = 0xffffffff >> rb;
    *val32 |= tmp >> (32-rb);
    buf_p->word = tmp & buf_p->mask;
    buf_p->avail_bits -= rb;
  }
  return err;
}
#if 0
    /* reference code
    leadingZeroBits = -1;
    for (b=0;!b;leadingZeroBits++) {
        b = read_bits(1);
    }
    codeNum = (1<<leadingZeroBits) - 1 + read_bits(leadingZeroBits); */
    /* word:
     +----+----+----+----+---+---+---+---+---+---+---+---+---+---+
     | 31 | 30 | 29 | .. | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     +----+----+----+----+---+---+---+---+---+---+---+---+---+---+
                         <--- bits ------------------------------>
     initialize word with 0 and available bits with 0
     available bits: 10 (bit0 to bit9)
     want x bits: if (x<=available bits) word>>(available bits-x)
     */
int read_exp() {
  UInt32 test;
  Int32 len;

  test = 1 << (avail_bits - 1);
  len = 0;
  if (word&test) {
    val32 = 0;
    avail_bits--;
    if (!avail_bits) {
        word = *p32_bits;
        mask = 0xffffffff;
        avail_bits = 32;
    }
    else {
        mask = mask>>1;
        word = word&(~test);
    }
  }
  else {
    while !(word&test) {
      word = word&(~test);
      test = test >> 1;
      mask = mask >> 1;
      len++;
      if (!test) {
        word = *p32_bits;
        test = 0x80000000;
        mask = 0xffffffff;
      }
    }
    word  = word&(~test);
    val32 = (word>>(32-len)) - 1;
  }
  return val32;
}
#endif


viSerror  elFileGetExpGolomb(bufManager_p buf_p, UInt32 *val32) {

  viSerror err = VIS_OK;
  UInt32 test;
  Int32 len, tmp;

  DB_L10("elFileGetExpGolomb\n");
  if (buf_p->avail_bits == 0) {
    err = elFileGetWord(buf_p,&buf_p->word);
    buf_p->mask = 0xffffffff;
//    buf_p->avail_bits = 32;
  }
  test = 1 << (buf_p->avail_bits - 1);
  len = 0;
  DB_L1("test = 0x%08lx\n",test);
  DB_L1("buf_p->word = 0x%08lx\n",buf_p->word);
  if (buf_p->word&test) {
    *val32 = 0;
    buf_p->avail_bits--;
    if (!buf_p->avail_bits) {
        err = elFileGetWord(buf_p,&buf_p->word);
        buf_p->mask = 0xffffffff;
        buf_p->avail_bits = 32;
    }
    else {
        buf_p->mask = buf_p->mask>>1;
        buf_p->word = buf_p->word&(buf_p->mask);
    }
  }
  else {
    while (!(buf_p->word&test)) {
      DB_L1("buf_p->mask = 0x%08lx\n",buf_p->mask);
      DB_L1("buf_p->word = 0x%08lx\n",buf_p->word);
      DB_L1("test        = 0x%08lx\n",test);
      test = test >> 1;
      buf_p->mask = buf_p->mask>>1;
      len++;
      buf_p->avail_bits--;
      DB_L1("len = %ld\n",len);
      DB_L1("buf_p->avail_bits = %ld\n",buf_p->avail_bits);
      if (!buf_p->avail_bits) {
        err = elFileGetWord(buf_p,&buf_p->word);
        test = 0x80000000;
        buf_p->mask = 0xffffffff;
      }
    }
//    buf_p->word  = buf_p->word&(~test);
    DB_L1("buf_p->word = 0x%08lx\n",buf_p->word);
    DB_L1("buf_p->mask = 0x%08lx\n",buf_p->mask);
    DB_L1("test        = 0x%08lx\n",test);
    if (buf_p->avail_bits-(len+1) < 0) {
        DB_L1("((len+1)-buf_p->avail_bits) = %ld\n",((len+1)-buf_p->avail_bits));
        tmp = (len+1)-buf_p->avail_bits;
        *val32 = buf_p->word<<(tmp);
        err = elFileGetWord(buf_p,&buf_p->word);
        DB_L1("buf_p->word = 0x%08x\n",buf_p->word);
        *val32|= buf_p->word>>(32-tmp);
        *val32 -= 1;
        buf_p->mask = 0xffffffff>>tmp;
        buf_p->avail_bits -= tmp;
        DB_L1("buf_p->mask = 0x%08x\n",buf_p->mask);
        DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
        buf_p->word = buf_p->word&(buf_p->mask);
    }
    else {
        *val32 = (buf_p->word>>(buf_p->avail_bits-(len+1)))-1;
        buf_p->mask = buf_p->mask>>(len+1);
        buf_p->avail_bits -= (len+1);
        buf_p->word = buf_p->word&(buf_p->mask);
        DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
        DB_L1("len               = %d\n",len);
    }
  }
  DB_L10("Exiting elFileGetExpGolomb...................\n");
  return err;
}

viSerror  elFileGetSignedExpGolomb(bufManager_p buf_p, UInt32 *val32) {

  viSerror err = VIS_OK;
  UInt32 test, k;
  Int32 len, tmp;

  DB_L10("elFileGetSignedExpGolomb\n");
  DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
  if (buf_p->avail_bits == 0) {
    err = elFileGetWord(buf_p,&buf_p->word);
    buf_p->mask = 0xffffffff;
//    buf_p->avail_bits = 32;
  }
  test = 1 << (buf_p->avail_bits - 1);
  len = 0;
  DB_L1("test = 0x%08x\n",test);
  DB_L1("buf_p->word = 0x%08x\n",buf_p->word);
  if (buf_p->word&test) {
    *val32 = 0;
    buf_p->avail_bits--;
    if (!buf_p->avail_bits) {
        err = elFileGetWord(buf_p,&buf_p->word);
        buf_p->mask = 0xffffffff;
        buf_p->avail_bits = 32;
    }
    else {
        buf_p->mask = buf_p->mask>>1;
        buf_p->word = buf_p->word&(buf_p->mask);
    }
  }
  else {
    while (!(buf_p->word&test)) {
      DB_L1("buf_p->mask = 0x%08x\n",buf_p->mask);
      DB_L1("buf_p->word = 0x%08x\n",buf_p->word);
      DB_L1("test        = 0x%08x\n",test);
      test = test >> 1;
      buf_p->mask = buf_p->mask>>1;
      len++;
      buf_p->avail_bits--;
      DB_L1("len = %d\n",len);
      DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
      if (!buf_p->avail_bits) {
        err = elFileGetWord(buf_p,&buf_p->word);
        test = 0x80000000;
        buf_p->mask = 0xffffffff;
      }
    }
//    buf_p->word  = buf_p->word&(~test);
    DB_L1("buf_p->word = 0x%08x\n",buf_p->word);
    DB_L1("buf_p->mask = 0x%08x\n",buf_p->mask);
    DB_L1("test        = 0x%08x\n",test);
    if (buf_p->avail_bits-(len+1) < 0) {
        DB_L1("((len+1)-buf_p->avail_bits) = %d\n",((len+1)-buf_p->avail_bits));
        tmp = (len+1)-buf_p->avail_bits;
        k = buf_p->word<<(tmp);
        err = elFileGetWord(buf_p,&buf_p->word);
        DB_L1("buf_p->word = 0x%08x\n",buf_p->word);
        k|= buf_p->word>>(32-tmp);
        k -= 1;
        if (k&1)
            *val32 = (k+1)>>2;
        else
            *val32 = -(k+1)>>2;
        buf_p->mask = 0xffffffff>>tmp;
        buf_p->avail_bits -= tmp;
        DB_L1("buf_p->mask = 0x%08x\n",buf_p->mask);
        DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
        buf_p->word = buf_p->word&(buf_p->mask);
    }
    else {
        k = (buf_p->word>>(buf_p->avail_bits-(len+1)));
        if (k&1)
            *val32 = (k+1)>>2;
        else
            *val32 = -(k+1)>>2;
        buf_p->mask = buf_p->mask>>(len+1);
        buf_p->avail_bits -= (len+1);
        buf_p->word = buf_p->word&(buf_p->mask);
    }
    DB_L1("buf_p->avail_bits = %d\n",buf_p->avail_bits);
    DB_L1("len               = %d\n",len);
  }
  DB_L10("Exiting elFileGetSignedExpGolomb...................\n");
  return err;
}

viSerror elFileSetStreamPosition(bufManager_p buf_p, UInt32 tmp){

    viSerror err = VIS_OK;

  	/* Position the file */
	fseek(buf_p->fp,tmp,SEEK_SET);
	/* Empty the input buffer */
	buf_p->bc = buf_p->max;
	return err;
}

viSerror parseSPS(bufManager_p buf_p, seqParSet_p sps_p) {

    viSerror err = VIS_OK;
    UInt8    tmByte;
    UInt32   tmWord;
    Int      i;

    err = elFileGetByte(buf_p, &tmByte);
    sps_p->profile_idc = tmByte;
    DB_L0("     profile_idc                           %d\n",sps_p->profile_idc);
    //66 baseline, 77 main, 88 extended profile
    err = elFileGetByte(buf_p, &tmByte);
    sps_p->constrained_set0_flag = tmByte&0x80;
    if (sps_p->constrained_set0_flag) {
        DBGPTF(LEVEL0,("     constrained_set0_flag                 Yes\n"));
    }
    else {
        DBGPTF(LEVEL0,("     constrained_set0_flag                 No\n"));
    }
    sps_p->constrained_set1_flag = tmByte&0x40;
    if (sps_p->constrained_set1_flag) {
        DBGPTF(LEVEL0,("     constrained_set1_flag                 Yes\n"));
    }
    else {
        DBGPTF(LEVEL0,("     constrained_set1_flag                 No\n"));
    }
	sps_p->constrained_set2_flag = tmByte&0x20;
    if (sps_p->constrained_set2_flag) {
        DBGPTF(LEVEL0,("     constrained_set2_flag                 Yes\n"));
    }
    else {
        DBGPTF(LEVEL0,("     constrained_set2_flag                 No\n"));
    }
    /* skip reserved zero 5bits */
    err = elFileGetByte(buf_p, &tmByte);
    sps_p->level_idc = tmByte;
	DB_L0("     level_idc                             %d\n",sps_p->level_idc);
    /* exponential golomb code */
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->seq_parameter_set_id = tmWord;
    DB_L0("     seq_parameter_set_id                  %d\n",sps_p->seq_parameter_set_id);
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->log2_max_frame_num_minus4 = tmWord;
    DB_L0("     log2_max_frame_num_minus4             %d\n",sps_p->log2_max_frame_num_minus4);
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->pic_order_cnt_type = tmWord;
    DB_L0("     pic_order_cnt_type                    %d\n",sps_p->pic_order_cnt_type);
    if (sps_p->pic_order_cnt_type == 0) {
        err = elFileGetExpGolomb(buf_p, &tmWord);
        sps_p->log2_max_pic_order_cnt_lsb_minus4 = tmWord;
    DB_L0("     log2_max_pic_order_cnt_lsb_minus4     %d\n",sps_p->log2_max_pic_order_cnt_lsb_minus4);
    }
    else if (sps_p->pic_order_cnt_type == 1) {
        err = read_bits(buf_p, 1, &tmWord);
        sps_p->delta_pic_order_always_zero_flag = tmWord;      // u(1)
        DB_L0("     delta_pic_order_always_zero_flag      %d\n",sps_p->delta_pic_order_always_zero_flag);
        err = elFileGetSignedExpGolomb(buf_p, &tmWord);
        sps_p->offset_for_non_ref_pic = tmWord;                // se(v)
        DB_L0("     offset_for_non_ref_pic                %d\n",sps_p->offset_for_non_ref_pic);
        err = elFileGetSignedExpGolomb(buf_p, &tmWord);
        sps_p->offset_for_top_to_bottom_field = tmWord;        // se(v)
        DB_L0("     offset_for_top_to_bottom_field        %d\n",sps_p->offset_for_top_to_bottom_field);
        err = elFileGetExpGolomb(buf_p, &tmWord);
        sps_p->num_ref_frames_in_pic_order_cnt_cycle = tmWord; // ue(v)
        DB_L0("     num_ref_frames_in_pic_order_cnt_cycle %d\n",sps_p->num_ref_frames_in_pic_order_cnt_cycle);
        for (i=0;i<sps_p->num_ref_frames_in_pic_order_cnt_cycle;i++) {
            err = elFileGetSignedExpGolomb(buf_p, &tmWord);
            sps_p->offset_for_ref_frame[i] = tmWord;
            DB_L0("     offset_for_ref_frame[%d]               ",i);
            DB_L0("%d\n",sps_p->offset_for_ref_frame[i]);
        }
    }
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->num_ref_frames = tmWord;
    DB_L0("     num_ref_frames                        %d\n",sps_p->num_ref_frames);
    err = read_bits(buf_p, 1, &tmWord);
    sps_p->gaps_in_frame_num_value_allowed_flag = tmWord;
    DB_L0("     gaps_in_frame_num_value_allowed_flag  %d\n",sps_p->gaps_in_frame_num_value_allowed_flag);
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->pic_width_in_mbs_minus1 = tmWord;
    DB_L0("     pic_width_in_mbs_minus1               %d\n",sps_p->pic_width_in_mbs_minus1);
    DB_L0("     picture width                         %d\n",(sps_p->pic_width_in_mbs_minus1<<4)+16);
    err = elFileGetExpGolomb(buf_p, &tmWord);
    sps_p->pic_height_in_map_units_minus1 = tmWord;
    DB_L0("     pic_height_in_map_units_minus1        %d\n",sps_p->pic_height_in_map_units_minus1);
    err = read_bits(buf_p, 1, &tmWord);
    sps_p->frame_mbs_only_flag = tmWord;      // u(1)
    DB_L0("     frame_mbs_only_flag                   %d\n",sps_p->frame_mbs_only_flag);
    DB_L0("     picture height                        %d\n",((2-sps_p->frame_mbs_only_flag)*(sps_p->pic_height_in_map_units_minus1+1)<<4));

    /* faster version
	sps_p->seq_parameter_set_id = read_exp();                  // ue(v)
	sps_p->log2_max_frame_num_minus4 = read_exp();             // ue(v)
	sps_p->pic_order_cnt_type = read_exp();                    // ue(v)
	sps_p->log2_max_pic_order_cnt_lsb_minus4 = read_exp();     // ue(v)
    */
	return err;
}

viSerror elFileScan(session_p pSes)
{
    viSerror err = VIS_OK;
  	UInt8 tmByte = 0;  		/* current byte */
    UInt32 nbBytes;
    Int32  nbPictures;
    bufManager_p pbufMan;
    UInt8 forbidden_bit, nal_ref_idc;

    pbufMan = &pSes->bufMan;

	/* Search a limited amount of bytes
	while ((pbufMan->byteCounter < SCAN_LIMIT)&&(nbStartCodes < MIN_SC_FOUND)) { */
    nbPictures = 0;
	while ((nbPictures < pSes->nbPictures)&(err != VIS_EOF)) {
#if 0
        if (copy == True) {
            dbgLevel = 0;
        }
#endif
        err = elFileGetNextStartCode(pbufMan, &tmByte, &nbBytes);
        DB_L1("elFileScan: nbPictures   %d\n", nbPictures);
        forbidden_bit = (tmByte & 0x80);
        nal_ref_idc   = (tmByte & 0x60);
        /* nal_ref_idc:
            0x00: non reference picture
            0x20, 0x40, 0x60: used as reference picture
         */
	    // Identify and parse the packet
	    switch (tmByte & 0x1f) {
	        case (NALU_SLICE_START_CODE_VALUE):
	    	    DB_L0("     NALU SLICE HEADER START CODE          0x%x\n",tmByte);
                nbPictures++;
		    	break;
	        case (NALU_DPA_START_CODE_VALUE):
	    	    DB_L0("     NALU DPA HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_DPB_START_CODE_VALUE):
	    	    DB_L0("     NALU DPB HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_DPC_START_CODE_VALUE):
	    	    DB_L0("     NALU DPC HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_IDR_START_CODE_VALUE):
	    	    DB_L0("     NALU IDR HEADER START CODE            0x%x\n",tmByte);
                nbPictures++;
		    	break;
	        case (NALU_SEI_START_CODE_VALUE):
	    	    DB_L0("     NALU SEI HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_SPS_START_CODE_VALUE):
	    	    DB_L0("     NALU SPS HEADER START CODE            0x%x\n",tmByte);
                parseSPS(pbufMan, &pSes->sps);
                pSes->width  = (pSes->sps.pic_width_in_mbs_minus1<<4)+16;
                pSes->height = (2-pSes->sps.frame_mbs_only_flag)*((pSes->sps.pic_height_in_map_units_minus1+1)<<4);
                break;
	        case (NALU_PPS_START_CODE_VALUE):
	    	    DB_L0("     NALU PPS HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_AUD_START_CODE_VALUE):
	    	    DB_L0("     NALU AUD HEADER START CODE            0x%x\n",tmByte);
		    	break;
	        case (NALU_EOSEQ_START_CODE_VALUE):
	    	    DB_L0("     NALU EOSEQ HEADER START CODE          0x%x\n",tmByte);
		    	break;
	        case (NALU_EOSTREAM_START_CODE_VALUE):
	    	    DB_L0("     NALU EOSTREAM HEADER START CODE       0x%x\n",tmByte);
		    	break;
	        case (NALU_FILL_START_CODE_VALUE):
	    	    DB_L0("     NALU FILL HEADER START CODE           0x%x\n",tmByte);
		    	break;
			default:
				if (tmByte == 0) {
		    	    DB_L0("     UNEXPECTED START CODE                 0x%x \n", tmByte);
				}
				else {
		    	    DB_L0("     UNEXPECTED BYTE                       0x%x \n", tmByte);
				}
        }// switch
    } /* while */

//    DB_L1("elFileScan: nbPictures  %ld\n", nbPictures-1);

    return err;
}

int main(int argc, char **argv)
{
	FILE	   *fpRead;
	char	    WriteName_bin[256];
	char	    ReadName[256];
	char	    WriteName[256];
	int         i;
    session_t   ses;

    dbgLevel = 1;

	if ( argc < 2 ) {
		printf("%s input_file_name [number_of_pictures - default is 3]\n",argv[0]);
        printf(" example: %s corvette.264 5\n",argv[0]);
        printf(" version 0.9\n");
        printf(" send an e-mail to brigitte@electropix.com for additional features\n\n");
		exit(1);
	}
	else {
		strcpy(ReadName,argv[1]);
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName);
		return(VIS_ERR_OPEN_FILE);
	}
    ses.width       = 176;
    ses.height      = 144;
    ses.nbPictures  = PIC_LIMIT;

    if ( argc == 3 ) {
        ses.nbPictures  = atoi(argv[2]);
        printf(" number of pictures %d\n",ses.nbPictures);
        ses.nbPictures  += 1;
    }
	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		WriteName_bin[i] = '\0';
	}

	strncpy(WriteName,ReadName,strlen(ReadName)-4);
	strncpy(WriteName_bin,ReadName,strlen(ReadName)-4);
	strcat(WriteName,".c");
	strcat(WriteName_bin,".bin");

#ifdef FILE_OUTPUT
	if ((fpWrite=fopen(WriteName,"wb"))==NULL) {
		printf("error : cannot open %s\n",WriteName);
		return(VIS_ERR_OPEN_FILE);
	}
#endif
#if 0
	if ((fpWrite_bin=fopen(WriteName_bin,"wb"))==NULL) {
		printf("error : cannot open %s\n",WriteName_bin);
		return(VIS_ERR_OPEN_FILE);
	}
#endif

	elFileInitBufManager(&ses.bufMan,&scanBuffer[0]);

	/* check if files exist */
    ses.bufMan.fp = fopen(ReadName, "rb");
    if (!ses.bufMan.fp) {
		printf("error : cannot open %s\n",ReadName);
        return VIS_ERR_OPEN_FILE;
    }
    else {
#if 0
        while ((feof(ses.bufMan.fp) == 0)) {
	        nbytes += fread(&tmByte,1,sizeof(UInt8),ses.bufMan.fp);
        }
#endif
        fseek(ses.bufMan.fp,0,SEEK_SET);
		elFileInitBuffer(&ses.bufMan);
        fprout = stderr;
		elFileScan(&ses);

        ses.bufMan.limit = ses.bufMan.byteCounter-4;
#ifdef FILE_OUTPUT
        fprintf(fpWrite,"\n#define SIM_PICNUM %ld\n",ses.nbPictures-1);
        fprintf(fpWrite,"#define SIM_WIDTH  %ld\n",ses.width);
        fprintf(fpWrite,"#define SIM_HEIGHT %ld\n",ses.height);
        fprintf(fpWrite,"#define SIM_SIZE %ld\n",ses.bufMan.limit);
    	fprintf(fpWrite,"         int  simHeight = SIM_HEIGHT;\n");
    	fprintf(fpWrite,"         int  simWidth  = SIM_WIDTH;\n");
    	fprintf(fpWrite,"         int  simPicnum = SIM_PICNUM;\n");
    	fprintf(fpWrite,"         int  simSize   = SIM_SIZE;\n");
    	fprintf(fpWrite,"unsigned char outputCompBuffer[SIM_WIDTH * SIM_HEIGHT * SIM_PICNUM * 2 + 4096] = {0xDE,0xAD,0xBE,0xEF} ;\n");
    	fprintf(fpWrite,"unsigned char simBuffer[SIM_SIZE] = {\n");
#endif
        fseek(ses.bufMan.fp,0,SEEK_SET);
		elFileInitBuffer(&ses.bufMan);
//        copy = True;
        ses.bufMan.byteCounter = 0;
#ifdef FILE_OUTPUT
        fprout = fpWrite;
#endif
        elFileScan(&ses);
#ifdef FILE_OUTPUT
        fprintf(fpWrite," };\n");
#endif
	}

    fclose(ses.bufMan.fp);
#ifdef FILE_OUTPUT
    fclose(fpWrite);
#endif
//    fclose(fpWrite_bin);
    return (0);
}
