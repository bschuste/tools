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


static unsigned char
get_dc_scaler(unsigned char quant,
			  unsigned char lum)
{
	if (quant < 5)
		return 8;

	if (quant < 25 && !lum)
		return (quant + 13) / 2;

	if (quant < 9)
		return 2 * quant;

	if (quant < 25)
		return quant + 8;

	if (lum)
		return 2 * quant - 16;
	else
		return quant - 6;
}

static unsigned char
get_dc_scaler_lum(unsigned char quant)
{
	if (quant < 5)
		return 8;

	if (quant < 9)
		return 2 * quant;

	if (quant < 25)
		return quant + 8;

	return 2 * quant - 16;
}

static unsigned char
get_dc_scaler_chrom(unsigned char quant)
{
	if (quant < 5)
		return 8;

	if (quant < 25 )
		return (quant + 13) / 2;

	return quant - 6;
}

uint8_t
get_fcode(uint16_t sr)
{
	if (sr <= 16)
		return 1;

	else if (sr <= 32)
		return 2;

	else if (sr <= 64)
		return 3;

	else if (sr <= 128)
		return 4;

	else if (sr <= 256)
		return 5;

	else if (sr <= 512)
		return 6;

	else if (sr <= 1024)
		return 7;

	else
		return 0;
}

int main(int argc, char **argv)
{
	FILE	*fpWrite;
	char	WriteName[256];
	int     i;
    viSerror err = VIS_OK;   
    unsigned char q[32] = {0};
    unsigned long lq[32] = {0};

	dbgLevel = 0;
	if ( argc < 2 ) {
		printf("%s output_file_name\n",argv[0]);
		exit(1);
	}
	else {
		strcpy(WriteName,argv[1]);
	}

	if ((fpWrite=fopen(WriteName,"w"))==NULL) {
		printf("error : cannot open %s\n",WriteName);
		return(VIS_ERR_OPEN_FILE);
	}
//    fprintf(fpWrite,"#pragma TCS_align=4\n");
//    fprintf(fpWrite,"static const uint8_t qtab_lum[32] = {\n");
    fprintf(fpWrite,"static const uint32_t qtab_lum[32] = {\n");
    for (i=0;i<32;i++) {
        q[i] = get_dc_scaler_lum(i);
        fprintf(fpWrite,"%d, ",q[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");

//    fprintf(fpWrite,"#pragma TCS_align=4\n");
//    fprintf(fpWrite,"static const uint8_t qtab_chrom[32] = {\n");
    fprintf(fpWrite,"static const uint32_t qtab_chrom[32] = {\n");
    for (i=0;i<32;i++) {
        q[i] = get_dc_scaler_chrom(i);
        fprintf(fpWrite,"%d, ",q[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");

    fprintf(fpWrite,"static const uint32_t div_lum[32] = {\n");
    for (i=0;i<32;i++) {
        lq[i] = ((4294967296)/get_dc_scaler_lum(i)) + 1;
        fprintf(fpWrite,"%u, ",lq[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");

    fprintf(fpWrite,"static const uint32_t div_chrom[32] = {\n");
    for (i=0;i<32;i++) {
        lq[i] = ((4294967296)/get_dc_scaler_chrom(i)) + 1;
        fprintf(fpWrite,"%u, ",lq[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");

    fprintf(fpWrite,"static const uint32_t inv_q[32] = {\n");
    for (i=0;i<32;i++) {
        if (i==0) 
            lq[i] = ((4294967296)) + 1;
        else
            lq[i] = ((4294967296)/(i)) + 1;
        fprintf(fpWrite,"%u, ",lq[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");

    fprintf(fpWrite,"#pragma TCS_align=4\n");
    fprintf(fpWrite,"static const uint8_t fcode[32] = {\n");
    for (i=0;i<32;i++) {
        fcode[i] = get_fcode(i);
        fprintf(fpWrite,"%u, ",fcode[i]);
        if (i%8 == 7) fprintf(fpWrite,"\n");
    }
    fprintf(fpWrite,"};\n\n");
    fclose(fpWrite);

	return(0);
}
