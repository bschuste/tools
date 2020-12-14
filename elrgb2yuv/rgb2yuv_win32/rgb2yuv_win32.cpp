// rgb2yuv_win32.cpp : Defines the entry point for the console application.
//


/***********************************************************************
 **
   Copyright (c) Electropix 2002. All rights reserved. 
   This source code and any compilation or derivative thereof is the sole
   property of Electropix SARL and is provided pursuant to a Software
   License Agreement.  This code is the proprietary information   
   of Electropix SARL and is confidential in nature.  Its use and dissemination 
   by any party other than Electropix SARL is strictly limited by the
   confidential information provisions of the Agreement referenced above.            

   Reproduction in whole or in part is prohibited without the written 
   consent of the copyright holder.
                                                                                    						             
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
   File: viSrgb2yuv420.c
   Abstract:	Matrix the RGB 4:4:4 components into YUV 4:2:0 components
   Usage: viSrgb2yuv420 input_file_name output_file_name [number of pictures]
                number of pictures is optional, default is 9999
   Out:     	generate the YUV4:2:0 file
   Author: Brigitte Schuster
   Last update: 2003/09/15                                                  
   Modified:                                                   
   History:
 		Version 0.1	 2003/09/15
 **						
 ***********************************************************************/					
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdafx.h"

/* Structure with image data */
typedef struct {
  unsigned char *lum;         /* Luminance plane */
  unsigned char *Cb;          /* Cb plane */
  unsigned char *Cr;          /* Cr plane */
} yuvImage_t, *yuvImage_p;

/* Structure with RGB image data */
typedef struct {
  unsigned char *r;           /* Red plane */
  unsigned char *g;           /* Green plane */
  unsigned char *b;           /* Blue plane */
} rgbImage_t, *rgbImage_p;


int freadRGBPict(FILE *fpRead, rgbImage_t *pPict, int width, int height) {
    int i,size;

    size = width * height;
	for (i=0;i<size;i++) {
		fread(pPict->r + i, sizeof(unsigned char),1,fpRead);
		fread(pPict->g + i, sizeof(unsigned char),1,fpRead);
		fread(pPict->b + i, sizeof(unsigned char),1,fpRead);
	}
    return(feof(fpRead));
}

int fwriteYUVPict(FILE *fpWrite, yuvImage_t *pPict, int width, int height) {
    int size;

    size = width * height;
    fwrite(pPict->lum,sizeof(unsigned char),size,fpWrite);
    fwrite(pPict->Cb,sizeof(unsigned char),size/4,fpWrite);
    fwrite(pPict->Cr,sizeof(unsigned char),size/4,fpWrite);
    return(0);    
}

int rgb2yuv(rgbImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
	int		size;
    int     i,j;
	int		Y,CB,CR;

	/* Convert RGB 4:4:4 to YUV 4:4:4
	 */
	size = pels * lines;

#if 0
	for (i=0;i<lines;i++) {
		for (j=0;j<pels;j++) {
			Y  = (int)( 0.299*(*(pIm1->r + i*pels + j)) + 0.587*(*(pIm1->g + i*pels + j)) + 0.114*(*(pIm1->b + i*pels + j)));
			if (Y>235) Y=235;
			if (Y<16)  Y=16;
			*(pIm2->lum + i*pels + j) = (unsigned char)Y;
			*(pIm2->Cb  + i*pels + j) = 12;
			*(pIm2->Cr  + i*pels + j) = 45;
		}
	}
#else
	for (i=0;i<lines;i++) {
		for (j=0;j<pels;j++) {
			Y  = (int)( 0.299*(*(pIm1->r + i*pels + j)) + 0.587*(*(pIm1->g + i*pels + j)) + 0.114*(*(pIm1->b + i*pels + j)));
			CB = (int)(-0.169*(*(pIm1->r + i*pels + j)) - 0.331*(*(pIm1->g + i*pels + j)) + 0.500*(*(pIm1->b + i*pels + j)));
			CR = (int)( 0.500*(*(pIm1->r + i*pels + j)) - 0.419*(*(pIm1->g + i*pels + j)) - 0.081*(*(pIm1->b + i*pels + j)));
			if (Y>235) Y=235;
			if (Y<16)  Y=16;
			CB = CB+128;
			if (CB>240) CB=240;
			if (CB<16)  CB=16;		    
			CR = CR+128;
			if (CR>240) CR=240;
			if (CR<16)  CR=16;
			*(pIm2->lum + i*pels + j) = (unsigned char)Y;
			*(pIm2->Cb  + i*pels + j) = (unsigned char)CB;
			*(pIm2->Cr  + i*pels + j) = (unsigned char)CR;
		}
	}
#endif
	return(0);
}

int yuv444to420(yuvImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
	int		size;
    int     i,j;

	size = pels * lines;

	/* Convert YUV 4:4:4 to YUV 4:2:0
	 */

#if 1
	for (i=0;i<lines-2;i+=2) {
		for (j=0;j<pels-2;j+=2) {
			*(pIm2->Cb + i/2*pels/2 + j/2) = (*(pIm1->Cb + i*pels + j) +
				                              *(pIm1->Cb + i*pels + j + pels) +
										      *(pIm1->Cb + i*pels + j + 1) +
										      *(pIm1->Cb + i*pels + j + pels + 1))/4;
			*(pIm2->Cr + i/2*pels/2 + j/2) = (*(pIm1->Cr + i*pels + j) +
				                              *(pIm1->Cr + i*pels + j + pels) +
										      *(pIm1->Cr + i*pels + j + 1) +
										      *(pIm1->Cr + i*pels + j + pels + 1))/4;
		}
		*(pIm2->Cb + i/2*pels/2 + pels/2 -1) = (*(pIm1->Cb + i*pels + pels-1) +
						 			            *(pIm1->Cb + i*pels + pels-1 + pels))/2;
		*(pIm2->Cr + i/2*pels/2 + pels/2 -1) = (*(pIm1->Cr + i*pels + pels-1) +
						 			            *(pIm1->Cr + i*pels + pels-1 + pels))/2;
	}
	*(pIm2->Cb + (lines/2-1)*pels/2 + pels/2 -1) = (*(pIm1->Cb + i*pels + pels-1) +
						 	    	                *(pIm1->Cb + i*pels + pels-1 + pels))/2;
	*(pIm2->Cr + (lines/2-1)*pels/2 + pels/2 -1) = (*(pIm1->Cr + i*pels + pels-1) +
						 		                    *(pIm1->Cr + i*pels + pels-1 + pels))/2;
#else
#if 1
	for (i=0;i<lines/2-1;i++) {
		for (j=0;j<pels/2-1;j++) {
			*(pIm2->Cb + i*pels/2 + j) = 128;
			*(pIm2->Cr + i*pels/2 + j) = 128;
		}
		*(pIm2->Cb + i*pels/2 + pels/2 -1) = 128;
		*(pIm2->Cr + i*pels/2 + pels/2 -1) = 128;
	}
	*(pIm2->Cb + (lines/2-1)*pels/2 + pels/2 -1) = 128;
	*(pIm2->Cr + (lines/2-1)*pels/2 + pels/2 -1) = 128;
#endif
#endif
	return(0);
}
//int main(int argc, char **argv) 
int _tmain(int argc, _TCHAR* argv[])
{
	FILE      *fpRead;
	FILE      *fpWrite;
	char	   ReadName[256];
	char	   WriteName[256];
    int        pels;
    int        lines;
    int        nbPict;   
    rgbImage_t *inPictRGB;
    yuvImage_t *inPictYUV;
    yuvImage_t *inPictYUV2;
    int        targetPict = 9999;
    
	if ( argc < 3 ) {
		printf(" Usage: %s input_file_name output_file_name [number of pictures] \n",argv[0]);
		printf(" Convert RGB 4:4:4 to YUV 4:2:0 v1.0\n");
        printf(" Only 640x480 pictures are supported\n");
        printf(" Number of pictures is optional. Reads until end of file by default\n");
        printf(" send e-mail to brigitte@electropix.com for additional features\n");
		exit(1);
	}
	else {
		strcpy(ReadName,argv[1]);
		strcpy(WriteName,argv[2]);
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName);
		exit(-1);
	}
	
	if ((fpWrite=fopen(WriteName,"wb"))==NULL) {
		printf("error :cannot open %s\n",WriteName);
		exit(-1);
	}
	
	if (argc == 4)
	    targetPict = atoi(argv[3]);    

    printf(" Convert RGB 4:4:4 to YUV 4:2:0 v1.0\n");
    printf(" Only 640x480 pictures are supported\n");
    printf(" send e-mail to brigitte@electropix.com for additional features\n\n");
	
    pels  = 640;
    lines = 480;
    
    inPictRGB      = (rgbImage_t *)malloc(sizeof(rgbImage_t));
    inPictRGB->r   = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPictRGB->g   = (unsigned char *)malloc(sizeof(char)*pels*lines);    
    inPictRGB->b   = (unsigned char *)malloc(sizeof(char)*pels*lines);
    

    inPictYUV      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPictYUV->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPictYUV->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines);    
    inPictYUV->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines);

	inPictYUV2      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPictYUV2->lum = inPictYUV->lum;
    inPictYUV2->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);    
    inPictYUV2->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);

	nbPict = 0;


	while (freadRGBPict(fpRead, inPictRGB, pels, lines) == 0) {
        rgb2yuv(inPictRGB, inPictYUV, pels, lines);
        yuv444to420(inPictYUV, inPictYUV2, pels, lines);
		fwriteYUVPict(fpWrite, inPictYUV2, pels, lines);
		nbPict++;
		if (nbPict >= targetPict) break;
	}
	printf("--> RGB to YUV4:2:0 conversion successfully processed %d pictures\n",nbPict);
	fclose(fpRead);
	fclose(fpWrite);
	free(inPictRGB->r);
	free(inPictRGB->g);
	free(inPictRGB->b);
	free(inPictRGB);
	free(inPictYUV->lum);
	free(inPictYUV->Cb);
	free(inPictYUV->Cr);
	free(inPictYUV);

	return(0);

}

