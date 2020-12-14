/***********************************************************************
 **
   Copyright (c) Tigress 2002. All rights reserved. 
   This source code and any compilation or derivative thereof is the sole
   property of Tigress SARL and is provided pursuant to a Software
   License Agreement.  This code is the proprietary information   
   of Tigress SARL and is confidential in nature.  Its use and dissemination 
   by any party other than Tigress SARL is strictly limited by the
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

   Tigress Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **						
 ***********************************************************************					
 **
   File: dibpsnr.c
   Abstract:	Compares two YUV image files using peak signal to noise 
                ratio method
  			    Limited to qcif 4:2:0 only
   Usage: dibpsnr original_file_name input_file_name width height [number of pictures]
                number of pictures is optional, default is 9999
   Out:     	prints the psnr values per image out
   Author: Brigitte Schuster
   Last update: 2002/06/29                                                  
   Modified:                                                   
   History:
 		Version 0.1	 2002/11/29
 **						
 ***********************************************************************/					
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



#define DIBHEADER_SIZE 54


/*  Peak Signal to Noise Ratio */
typedef struct {    
	float     Y;         
	float     U;         
	float     V;         
	float     meanY;         
	float     meanU;         
	float     meanV;         
} pictSnr_t, *pictSnr_p;

#ifdef __TCS__
#include <tmNxTypes.h>
#else
#define UInt16 unsigned short
#define UInt32 unsigned int
#define Char   char
#define Int32  int
#endif

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

typedef struct bmpFileHeader {
        UInt16    bfType;
        UInt32    bfSize;
        UInt16    bfReserved1;
        UInt16    bfReserved2;
        UInt32    bfOffBits;
} bmpFileHeader_t, *bmpFileHeader_p;

typedef struct bmpInfoHeader {
        UInt32    biSize;
        UInt32    biWidth;
        UInt32    biHeight;
        UInt16    biPlanes;
        UInt16    biBitCount;
        UInt32    biCompression;
        UInt32    biSizeImage;
        UInt32    biXPelsPerMeter;
        UInt32    biYPelsPerMeter;
        UInt32    biClrUsed;
        UInt32    biClrImportant;
} bmpInfoHeader_t, *bmpInfoHeader_p;

typedef struct _RGBQUAD {
    Char    rgbBlue;
    Char    rgbGreen;
    Char    rgbRed;
    Char    rgbReserved;
} RGBQUAD;

Int32 bmp_wd = 384, bmp_ht = 288; /* FIXTHIS */

int freadHeader(FILE *fpRead, bmpFileHeader_p pBmpHeader, bmpInfoHeader_p pInfoHeader) {

    fread (&pBmpHeader->bfType, sizeof (pBmpHeader->bfType), 1, fpRead);
    fread (&pBmpHeader->bfSize, 1, sizeof (pBmpHeader->bfSize), fpRead);
    fread (&pBmpHeader->bfReserved1, 1, sizeof (pBmpHeader->bfReserved1), fpRead);
    fread (&pBmpHeader->bfReserved2, 1, sizeof (pBmpHeader->bfReserved2), fpRead);
    fread (&pBmpHeader->bfOffBits, 1, sizeof (pBmpHeader->bfOffBits), fpRead);
    fread (&pInfoHeader->biSize, 1, sizeof (pInfoHeader->biSize), fpRead);
    fread (&pInfoHeader->biWidth, 1, sizeof (pInfoHeader->biWidth), fpRead);
    fread (&pInfoHeader->biHeight, 1, sizeof (pInfoHeader->biHeight), fpRead);
    fread (&pInfoHeader->biPlanes, 1, sizeof (pInfoHeader->biPlanes), fpRead);
    fread (&pInfoHeader->biBitCount, 1, sizeof (pInfoHeader->biBitCount), fpRead);
    fread (&pInfoHeader->biCompression, 1, sizeof (pInfoHeader->biCompression), fpRead);
    fread (&pInfoHeader->biSizeImage, 1, sizeof (pInfoHeader->biSizeImage), fpRead);
    fread (&pInfoHeader->biXPelsPerMeter, 1, sizeof (pInfoHeader->biXPelsPerMeter), fpRead);
    fread (&pInfoHeader->biYPelsPerMeter, 1, sizeof (pInfoHeader->biYPelsPerMeter), fpRead);
    fread (&pInfoHeader->biClrUsed, 1, sizeof (pInfoHeader->biClrUsed), fpRead);
    fread (&pInfoHeader->biClrImportant, 1, sizeof (pInfoHeader->biClrImportant), fpRead);

	return(feof(fpRead));
}

int freadRGBPict(FILE *fpRead, rgbImage_t *pPict, int pels, int lines, int offset) {

    int i, j, n, size;
	int scanLineSize = 0;

    size = pels * lines;

    scanLineSize = (((int) pels + 3)) & ~3;
/*	printf("scanLineSize = %d\n",scanLineSize);
	printf("pels = %d\n",pels);
	printf("lines = %d\n",lines);
	printf("offset = %d\n",offset); */

	for (i=0; i<lines; i++) {
		fseek (fpRead, offset + (long int) scanLineSize * (lines - i - 1) * 3, SEEK_SET);
		for (j=0;j<pels;j++) {
			fread((pPict->b + i*pels + j), sizeof(unsigned char), 1, fpRead);
			fread((pPict->g + i*pels + j), sizeof(unsigned char), 1, fpRead);
			fread((pPict->r + i*pels + j), sizeof(unsigned char), 1, fpRead);
		}
	}
	n = fseek (fpRead, offset + (long int) scanLineSize * (lines - 1) * 3, SEEK_SET);
    return(feof(fpRead));
}

int rgb2yuv(rgbImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
	int		size;
    int     i,j;
	int		Y,CB,CR;

	/* Convert RGB 4:4:4 to YUV 4:4:4
	 */
	size = pels * lines;

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
	return(0);
}

int yuv444to420(yuvImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
	int		size;
    int     i,j;

	size = pels * lines;

	pIm2->lum = pIm1->lum;

	/* Convert YUV 4:4:4 to YUV 4:2:0
	 */
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
	for (j=0;j<pels-2;j+=2) {
		*(pIm2->Cb + (lines/2-1)*pels/2 + j/2) = (*(pIm1->Cb + i*pels + j) +
										          *(pIm1->Cb + i*pels + j  + 1))/2;
		*(pIm2->Cr + (lines/2-1)*pels/2 + j/2) = (*(pIm1->Cr + i*pels + j) +
										          *(pIm1->Cr + i*pels + j  + 1))/2;
	}
	*(pIm2->Cb + (lines/2-1)*pels/2 + pels/2 -1) = *(pIm1->Cb + i*pels + pels-1);
	*(pIm2->Cr + (lines/2-1)*pels/2 + pels/2 -1) = *(pIm1->Cr + i*pels + pels-1);

	return(0);
}

int freadPict(FILE *fpRead, yuvImage_t *pPict, int width, int height) {
    int size;

    size = width * height;
    fread(pPict->lum,sizeof(unsigned char),size,fpRead);
    fread(pPict->Cb,sizeof(unsigned char),size/4,fpRead);
    fread(pPict->Cr,sizeof(unsigned char),size/4,fpRead);
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


int videoPSNR(yuvImage_t *im1, yuvImage_t *im2, int pels, int lines, pictSnr_p snr_p) {
  	double	   mse, diff;
	int		   size;
    int        i,j;

	/* ------------------- */
	/* Process Y component */
	/* ------------------- */
	size = pels * lines;
	mse  = 0.0;

	for (i=0;i<lines;i++) {
		for (j=0;j<pels;j++) {
		    diff = (double)((*(im1->lum + i*pels + j))-(*(im2->lum + i*pels + j)));
      		mse  += diff*diff;
		}
	}
    if (mse != 0.0) {
    	snr_p->Y = (float)10*log10((65025.0 * size)/mse);
    }
    else snr_p->Y = (float)99.99;

	/* ------------------- */
	/* Process U component */
	/* ------------------- */
	size = (pels * lines) / 4;
	mse  = 0.0;

	for (i=0;i<lines/2;i++) {
		for (j=0;j<pels/2;j++) {
		    diff = (double)((*(im1->Cb + i*(pels/2) + j))-(*(im2->Cb + i*(pels/2) + j)));
      		mse  += diff*diff;
		}
	}
    if (mse != 0.0) {
	    snr_p->U = (float)10*log10((65025.0 * size)/mse);
    }
    else {
        snr_p->U = (float)99.99;
    }
    
	/* ------------------- */
	/* Process V component */
	/* ------------------- */
	size = (pels * lines) / 4;
	mse  = 0.0;

	for (i=0;i<lines/2;i++) {
		for (j=0;j<pels/2;j++) {
		    diff = (double)((*((im1->Cr) + i*(pels/2) + j))-(*((im2->Cr) + i*(pels/2) + j)));
      		mse  += diff*diff;
		}
	}
    if (mse) {
    	snr_p->V = (float)10*log10((65025.0 * size)/mse);
    }
    else {
        snr_p->V = (float)99.99;
    }

      snr_p->meanY += (snr_p->Y);
      snr_p->meanU += (snr_p->U);
      snr_p->meanV += (snr_p->V);

	return(0);
}

static int ntoa(int num, char *fileNum) {

    
	fileNum[0] = '0'; fileNum[1] = '0'; fileNum[2] = '0'; fileNum[3] = '0';
	
	/* All the following is related to ascii conversion */
	fileNum[3] = 48 + num%10; /*ascii conversion */
	
	if (num>9) {
	    fileNum[2] = 48 + (num%100 - (fileNum[3]-48))/10;
	}
	if (num>99) {
	    fileNum[1] = 48 + (num%1000 - (fileNum[2]-48) - (fileNum[3]-48))/100;
	}
	if (num>999) {
	    fileNum[0] = 48 + (num%10000 - (fileNum[1]-48) - (fileNum[2]-48) - (fileNum[3]-48))/1000;
	}
	return 0;
}

int main(int argc, char **argv) {
	FILE      *fpRead1;
	FILE      *fpRead2;
	FILE      *fpWrite;
	char	   ReadName1[256];
	char       ReadName2[256];
    int        pels;
    int        lines;
    int        nbPict;   
    rgbImage_t  *inPictRGB;
	yuvImage_t  *inPictYUV;
    bmpFileHeader_t fileHd;
    bmpInfoHeader_t infoHd;
    yuvImage_t *inPict1;
    yuvImage_t *inPict2;
    pictSnr_t  snr_t;
    int        targetPict = 9999;
    char	   fileNum[7] = {'\0'};        /* file number 4 digits + trailer */
	char      *infileName;
	unsigned int i;
	size_t	     len;

    snr_t.meanY = 0.0;
    snr_t.meanU = 0.0;
    snr_t.meanV = 0.0;
    
	if ( argc < 3 ) {
		printf(" Usage: %s original_file_name(dib) input_file_name(yuv) [number of pictures] \n",argv[0]);
        printf(" PSNR computation between two yuv files v1.0\n");
        printf(" Only 4:2:0 pictures are supported\n");
        printf(" send e-mail to info@tigress.fr for additional features\n");
		exit(1);
	}
	else {
		strcpy(ReadName1,argv[1]);
		strcpy(ReadName2,argv[2]);
	}

	if ((fpRead1=fopen(ReadName1,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName1);
		exit(-1);
	}
	len = strlen(ReadName1);

	if ((fpRead2=fopen(ReadName2,"rb"))==NULL) {
		printf("error :cannot open %s\n",ReadName2);
		exit(-1);
	}
	
	if (argc == 4)
	    targetPict = atoi(argv[3]);    
    
    printf(" PSNR computation between two yuv files v1.0\n");
    printf(" Only 4:2:0 pictures are supported\n");
    printf(" send e-mail to info@tigress.fr for additional features\n\n");
	
    pels  = 384; /* atoi(argv[3]); */
    lines = 288; /* atoi(argv[4]); */
    
	freadHeader(fpRead1, &fileHd, &infoHd);
	pels  = infoHd.biWidth;    
	lines = abs(infoHd.biHeight);

	inPictRGB      = (rgbImage_t *)malloc(sizeof(rgbImage_t));
    inPictRGB->r   = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPictRGB->g   = (unsigned char *)malloc(sizeof(char)*pels*lines);    
    inPictRGB->b   = (unsigned char *)malloc(sizeof(char)*pels*lines);

    inPictYUV      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPictYUV->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPictYUV->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines);    
    inPictYUV->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines);

    inPict1      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPict1->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPict1->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);    
    inPict1->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);
    
    inPict2      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPict2->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPict2->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);    
    inPict2->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);

	nbPict = 0;
    infileName = (char *) malloc(len*sizeof(size_t));
	for (i=0;i<len;i++) infileName[i] = '\0';
	strcpy(&infileName[0], ReadName1);
/*    fprintf(stderr,"infileName %s\n",infileName); */

	while (1) {
		freadRGBPict(fpRead1, inPictRGB, pels, lines, fileHd.bfOffBits);
        rgb2yuv(inPictRGB, inPictYUV, pels, lines);
        yuv444to420(inPictYUV, inPict1, pels, lines);
        if (freadPict(fpRead2, inPict2, pels, lines) == 0) {
            videoPSNR((yuvImage_t *)inPict1, inPict2, pels, lines, &snr_t);
	        printf(" PSNR picture n %04d  Y=%2.2f dB, U=%2.2f dB, V=%2.2f dB\n",nbPict,snr_t.Y,snr_t.U,snr_t.V);
	    }
	    else {
			break;
	        /* end of file */
	    }
		nbPict++;
		if (nbPict >= targetPict) break;
		fclose(fpRead1);
		ntoa(nbPict,&fileNum[0]);
		for (i=0;i<len;i++) infileName[i] = '\0';			
		strcpy(&infileName[0], ReadName1);
		infileName[len-8] = '\0';
		strcat(&infileName[0],&fileNum[0]);
		strcat(&infileName[0],".dib");
/*		fprintf (stderr, "infileName %s\n", infileName); */
			
		fpRead1 = fopen(infileName, "rb");
		if (fpRead1 == NULL) {
			fprintf(stderr, "Error opening input file %s\n", infileName);
    		break;
		}
	}

    snr_t.meanY = (snr_t.meanY)/(float)nbPict;
    snr_t.meanU = (snr_t.meanU)/(float)nbPict;
    snr_t.meanV = (snr_t.meanV)/(float)nbPict;
	
	printf("+-------------------------------------------------------+\n");
	printf(" PSNR average         Y=%2.2f dB, U=%2.2f dB, V=%2.2f dB\n",snr_t.meanY,snr_t.meanU,snr_t.meanV);
	if (fpRead1) fclose(fpRead1);
	fclose(fpRead2);
	free(inPictRGB->r);
	free(inPictRGB->g);
	free(inPictRGB->b);
	free(inPictRGB);
	free(inPict1->lum);
	free(inPict1->Cb);
	free(inPict1->Cr);
	free(inPict1);
	free(inPict2->lum);
	free(inPict2->Cb);
	free(inPict2->Cr);
	free(inPict2);

	return(0);
}



