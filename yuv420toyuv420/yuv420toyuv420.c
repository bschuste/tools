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
   File: yuv422toyuv420.c
   Abstract:	Converts 422 separate files into a 420 concatenated file
  			    Limited to 384x288 4:2:0 only
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


/* Structure with image data */
typedef struct {
  unsigned char *lumCbCr;         /* Luminance + chrominance planes */
} uyvyImage_t, *uyvyImage_p;


Int32 yuv_wd = 720, yuv_ht = 576; 

int yuvtoyuv(yuvImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines, int offset) {
    int i,j;
    int len = (pels >> 6) * 64;
	/* Convert YUV 4:2:0 to YUV 4:2:0
	 */
    printf("yuvtoyuv: lines=%d, pels=%d, len=%d\n", lines, pels, len);
	for (i=0;i<lines;i++) {
        memcpy(pIm2->lum + i*offset, pIm1->lum + i*pels, len);
	}
	for (i=0;i<lines>>1;i++) {
        memcpy(pIm2->Cb + i*(offset>>1),  pIm1->Cb + i*(pels>>1), len>>1);
        memcpy(pIm2->Cr + i*(offset>>1),  pIm1->Cr + i*(pels>>1), len>>1);
    }    
    return(0);
}

int yuv422to420(yuvImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
	int		size;
    int     i,j;

	size = pels * lines;

	pIm2->lum = pIm1->lum;

	/* Convert YUV 4:2:2 to YUV 4:2:0
	 */
	for (i=0;i<lines;i+=2) {
		for (j=0;j<pels/2;j++) {
			*(pIm2->Cb + i/2*pels/2 + j) = (*(pIm1->Cb + i*pels/2 + j) +
				                            *(pIm1->Cb + i*pels/2 + j + pels/2))/2;
			*(pIm2->Cr + i/2*pels/2 + j) = (*(pIm1->Cr + i*pels/2 + j) +
				                            *(pIm1->Cr + i*pels/2 + j + pels/2))/2;
		}
	}
    printf("yuv422to420: i=%d, j=%d\n", i, j);
    /*
    for (j=0;j<pels/2-1;j++) {
	    *(pIm2->Cb + i/2*pels/2 + j) = *(pIm1->Cb + i*pels/2 + j);
		*(pIm2->Cr + i/2*pels/2 + j) = *(pIm1->Cr + i*pels/2 + j);
    } */

	return(0);
}

int freadYUV422Pict(FILE *fpRead, yuvImage_t *pPict, int width, int height) {
    int size, nbytes;

    size = width * height;
    nbytes  = fread(pPict->lum,sizeof(unsigned char),size,  fpRead);
    nbytes += fread(pPict->Cb, sizeof(unsigned char),size/2,fpRead);
    nbytes += fread(pPict->Cr, sizeof(unsigned char),size/2,fpRead);
    printf("freadYUV422Pict: read %d bytes\n",nbytes);
    return(feof(fpRead));
}

int freadUYVY422Pict(FILE *fpRead, uyvyImage_t *pPict, int width, int height) {
    int size, nbytes;

    size = width * height;
    nbytes = fread(pPict->lumCbCr,sizeof(unsigned char), size*2,  fpRead);
    printf("freadUYVY422Pict: read %d bytes\n",nbytes);
    return(feof(fpRead));
}

int freadYUV420Pict(FILE *fpRead, yuvImage_t *pPict, int width, int height) {
    int size, nbytes;

    size = width * height;
    nbytes = fread(pPict->lum,sizeof(unsigned char), size,  fpRead);
    nbytes = fread(pPict->Cb,sizeof(unsigned char), size/4,  fpRead);
    nbytes = fread(pPict->Cr,sizeof(unsigned char), size/4,  fpRead);
    return(0);    
}

int fwriteYUV420Pict(FILE *fpWrite, yuvImage_t *pPict, int width, int height) {
    int size;

    size = width * height;
    fwrite(pPict->lum,sizeof(unsigned char),size,fpWrite);
    fwrite(pPict->Cb,sizeof(unsigned char),size/4,fpWrite);
    fwrite(pPict->Cr,sizeof(unsigned char),size/4,fpWrite);
    return(0);    
}

int fwriteYUV420progPict(FILE *fpWrite, yuvImage_t *pPict, int width, int height) {
    int size;
    int i;

    size = width * height;
    for (i=0;i<height;i+=2) {
        fwrite(pPict->lum + i*width,sizeof(unsigned char),width,fpWrite);
    }
    for (i=0;i<height/2;i+=2) {
        fwrite(pPict->Cb + i*width/2,sizeof(unsigned char),width/2,fpWrite);
    }
    for (i=0;i<height/2;i+=2) {
        fwrite(pPict->Cr + i*width/2,sizeof(unsigned char),width/2,fpWrite);
    }
    return(0);    
}

int fwriteYUV422Pict(FILE *fpWrite, yuvImage_t *pPict, int width, int height) {
    int size;

    size = width * height;
    fwrite(pPict->lum,sizeof(unsigned char),size,fpWrite);
    fwrite(pPict->Cb,sizeof(unsigned char),size/2,fpWrite);
    fwrite(pPict->Cr,sizeof(unsigned char),size/2,fpWrite);
    return(0);    
}

int uyvytoyuv(uyvyImage_t *pIm1, yuvImage_t *pIm2, int pels, int lines) {
    int i,j;

	/* Convert UYVY 4:2:2 to YUV 4:2:2
	 */
    printf("uyvytoyuv: lines=%d, pels =%d\n", lines, pels);
	for (i=0;i<lines;i++) {
		for (j=0;j<pels*2;j+=4) {
			*(pIm2->lum + i*pels + j/2)     = *(pIm1->lumCbCr + i*pels*2 + j + 1);
			*(pIm2->lum + i*pels + j/2 + 1) = *(pIm1->lumCbCr + i*pels*2 + j + 3);
			*(pIm2->Cb  + i*pels/2 + j/4)   = *(pIm1->lumCbCr + i*pels*2 + j);
			*(pIm2->Cr  + i*pels/2 + j/4)   = *(pIm1->lumCbCr + i*pels*2 + j + 2);
		}
	}
    printf("uyvytoyuv: i=%d, j=%d\n", i, j);
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
	FILE        *fpRead;
	FILE        *fpWrite;
	char	     ReadName[256];
	char	     WriteName[256];
    int          pels;
    int          lines;
    int          offset = 768;
    int          nbPict;   
	yuvImage_t  *inPictYUV;
	yuvImage_t  *ouPictYUV;
    int          targetPict = 9999;
    char	     fileNum[7] = {'\0'};        /* file number 4 digits + trailer */
	char        *infileName;
	unsigned int i;
	size_t	     len;


	if ( argc < 3 ) {
		printf(" Usage: %s input_file_name(yuv420) output_file_name(yuv420) [number of pictures] \n",argv[0]);
        printf(" Convert 4:2:0 concatenated file w %d into a 4:2:0 concatenated file w 704 offset 768\n", yuv_wd);
        printf(" Only %dx%d pictures are supported\n", yuv_wd, yuv_ht);
        printf(" Example: yuv420toyuv420 flower_garden.yuv flower_garden_offset.yuv \n");
        printf(" send e-mail to info@tigress.fr for additional features\n");
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
	len = strlen(ReadName); 

	if ((fpWrite=fopen(WriteName,"wb"))==NULL) {
		printf("error :cannot open %s\n",WriteName);
		exit(-1);
	}
	
	if (argc == 4)
	    targetPict = atoi(argv[3]);    
    
    printf(" Convert 4:2:0 concatenated file w %d into a 4:2:0 concatenated file w 704 offset 768\n", yuv_wd);
    printf(" Only %dx%d pictures are supported\n", yuv_wd, yuv_ht);
    printf(" send e-mail to info@tigress.fr for additional features\n\n");
	
    pels  = yuv_wd; /* atoi(argv[3]); */
    lines = yuv_ht; /* atoi(argv[4]); */

    inPictYUV      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    inPictYUV->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPictYUV->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);
    inPictYUV->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);

    ouPictYUV      = (yuvImage_t *)malloc(sizeof(yuvImage_t));
    ouPictYUV->lum = (unsigned char *)malloc(sizeof(char)*pels*768);
    ouPictYUV->Cb  = (unsigned char *)malloc(sizeof(char)*pels*768/4);
    ouPictYUV->Cr  = (unsigned char *)malloc(sizeof(char)*pels*768/4);

	nbPict = 0;
    infileName = (char *) malloc(len*sizeof(size_t));
	for (i=0;i<len;i++) infileName[i] = '\0';
	strcpy(&infileName[0], ReadName);
/*    fprintf(stderr,"infileName %s\n",infileName); */

	while (1) {
        int val;

		val = freadYUV420Pict(fpRead, inPictYUV, pels, lines);
        yuvtoyuv(inPictYUV, ouPictYUV, pels, lines, 768);
        fwriteYUV420Pict(fpWrite, ouPictYUV, pels, 768);
        if (val == 0) {
           /* end of file */

		nbPict++;
		if (nbPict >= targetPict) break;
		fclose(fpRead);
		ntoa(nbPict,&fileNum[0]);
		for (i=0;i<len;i++) infileName[i] = '\0';			
		strcpy(&infileName[0], ReadName);
		infileName[len-4] = '\0';
		strcat(&infileName[0],&fileNum[0]);
//		strcat(&infileName[0],".dib");
/*		fprintf (stderr, "infileName %s\n", infileName); */
			
		fpRead = fopen(infileName, "rb");
		if (fpRead == NULL) {
			fprintf(stderr, "Error opening input file %s\n", infileName);
    		break;
		}
	    } /* end of file */
	}
	if (fpRead) fclose(fpRead);
    printf("fpWrite = 0x%x\n",fpWrite);
	if (fpWrite) fclose(fpWrite);

	free(inPictYUV->lum);
	free(inPictYUV->Cb);
	free(inPictYUV->Cr);
	free(inPictYUV);
    free(ouPictYUV->lum);
	free(ouPictYUV->Cb);
	free(ouPictYUV->Cr);
	free(ouPictYUV);

	return(0);
}



