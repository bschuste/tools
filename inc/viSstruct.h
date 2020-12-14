////////////////////////////////////////////////////////////////////////
//
// Copyright (c) viSystems 2002. 							             
// All rights reserved. Reproduction in whole or in part is prohibited   
// without the written consent of the copyright holder.                  
//                                                                                    						             
// This code and information is provided "as is" without any warranty    
// of any kind, either expressed or implied, including but not limited   
// to the implied warranties of merchantability and/or fitness for any   
// particular purpose.                                                   
// 
// This disclaimer of warranty extends to the user of these programs
// and user's customers, employees, agents, transferees, successors,
// and assigns.
//
// viSystems Research and Development does not represent or warrant that
// the programs furnished hereunder are free of infringement of any
// third-party patents.
//
////////////////////////////////////////////////////////////////////////
//
// Author: Brigitte Schuster
// Last update: 2002/06/29                                                  
// Modified:                                                   
// File: viSstruct.h
// History:
// 		Version 0.1	 2002/06/29
//						
////////////////////////////////////////////////////////////////////////
#ifndef VISSTRUCT_H
#define VISSTRUCT_H

/* Structure with image data */
typedef struct pict_image {
  unsigned char *lum;         /* Luminance plane */
  unsigned char *Cr;          /* Cr plane */
  unsigned char *Cb;          /* Cb plane */
} PictImage;

typedef struct Picture {
	UInt32	w0;		/* width of c0 component */
	UInt32	h0;		/* height of c0 component */
	UInt32	w1;		/* width of c1 component */
	UInt32	h1;		/* height of c1 component */
	UInt32	w2;		/* width of c2 component */
	UInt32	h2;		/* height of c2 component */
	UInt8		*c0;		/* component Y or R */ 
	UInt8		*c1;		/* component U or G */
	UInt8		*c2;		/* component V or B */
} Picture_t, *Picture_p;

/*---------------------*/
/*    Video Buffers    */
/*---------------------*/
typedef  enum {    			  /* picture size */
	PS_SQSIF 	= 1,
	PS_QSIF		= 2,
	PS_SIF		= 3,
	PS_HHR		= 4,
	PS_CCIR		= 5
} PictSize;

typedef struct _videoBuf {    /* video buffer structure */
	Pointer     Y;            /* buffer pointer for Y */
	Pointer     U;            /* buffer pointer for U */
	Pointer     V;            /* buffer pointer for V */
	Int32	    flag;
	PictSize	PS;
} vBuf;


/*---------------------*/
/*    Audio Buffers    */
/*---------------------*/
typedef struct _audioBuf {    /* audio buffer structure */
	Pointer     Buf1;         /* buffer pointer for Buf1 */
	Pointer     Buf2;         /* buffer pointer for Buf2 */
	Int32		flag;
} aBuf;


/*---------------------*/
/*  Bit-Plane Buffers  */
/*---------------------*/
typedef struct _biplaneBuf {  /* Bit plane buffer structure */
	Pointer     B0;           /* buffer pointer for Bit 0 (LSB) */
	Pointer     B1;           /* buffer pointer for Bit 1       */
	Pointer     B2;           /* buffer pointer for Bit 2       */
	Pointer     B3;           /* buffer pointer for Bit 3       */
	Pointer     B4;           /* buffer pointer for Bit 4       */
	Pointer     B5;           /* buffer pointer for Bit 5       */
	Pointer     B6;           /* buffer pointer for Bit 6       */
	Pointer     B7;           /* buffer pointer for Bit 7       */
	Pointer     B8;           /* buffer pointer for Bit 8       */
	Pointer     B9;           /* buffer pointer for Bit 9 (MSB) */
} bpBuf;



/*-----------------------------*/
/*  Compressed Flow Buffers    */
/*-----------------------------*/
typedef struct _compressedBuf {    
	UInt8     *Data;         
} cBuf;

/*-----------------------------*/
/*  Peak Signal to Noise Ratio */
/*-----------------------------*/
typedef struct {    
	Double     Y;         
	Double     U;         
	Double     V;         
} pSNR_t;


/* ------------------------------- */
/* Video Sequence Header structure */
/* ------------------------------- */
typedef struct vid_header {
  UInt16   h_size;
  UInt16   v_size;
  UInt8    aspect_ratio;
  UInt8    frame_rate;
  UInt32   bit_rate;
  UInt16   vbv_buf_size;
  UInt8    const_param_flag;
  UInt8    intra_quant_matrix[8][8];
  UInt8    non_intra_quant_matrix[8][8];
} SeqHeader;

/* ---------------------------- */
/* Sequence extension structure */
/* ---------------------------- */
typedef struct seq_ext {
  UInt8    ext_id;
  UInt8    profile_level;
  UInt8    progressive_seq_flag;
  UInt8    chroma_format;
  UInt8    h_size_ext;
  UInt8    v_size_ext;
  UInt16   bit_rate_ext;
  UInt8    vbv_buf_size_ext;
  UInt8    low_delay;
  UInt8    frame_rate_ext_n;
  UInt8    frame_rate_ext_d;
} SeqExt;

/* -------------------------- */
/* Group Of Picture structure */
/* -------------------------- */
typedef struct GoP {
  bool 	drop_flag;                 /* Flag indicating dropped frame. */
  UInt32 tc_hours;                 /* Hour component of time code.   */
  UInt32 tc_minutes;               /* Minute component of time code. */
  UInt32 tc_seconds;               /* Second component of time code. */
  UInt32 tc_pictures;              /* Picture counter of time code.  */
  bool   closed_gop;               /* Indicates no pred. vectors to
                                            previous group of pictures.    */
  bool   broken_link;              /* B frame unable to be decoded.  */
  UInt8  *ext_data;                /* Extension data.                */
  UInt8  *user_data;               /* User data.                     */
} GoP;

#endif
