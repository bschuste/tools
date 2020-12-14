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
   File: viSpsnr.c
   Abstract:	Compares two YUV image files using peak signal to noise 
                ratio method
  			    Limited to qcif 4:2:0 only
   Usage: viSpsnr original_file_name input_file_name [number of pictures]
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

/* Structure with image data */
typedef struct {
  unsigned char *lum;         /* Luminance plane */
  unsigned char *Cb;          /* Cb plane */
  unsigned char *Cr;          /* Cr plane */
} PictImage;

/*  Peak Signal to Noise Ratio */
typedef struct {    
	float     Y;         
	float     U;         
	float     V;         
	float     meanY;         
	float     meanU;         
	float     meanV;         
} pictSnr_t, *pictSnr_p;

int freadPict(FILE *fpRead, PictImage *Pict, int width, int height) {
    int size;

    size = width * height;
    fread(Pict->lum,sizeof(unsigned char),size,fpRead);
    fread(Pict->Cb,sizeof(unsigned char),size/4,fpRead);
    fread(Pict->Cr,sizeof(unsigned char),size/4,fpRead);
    return(feof(fpRead));
}

int fwritePict(FILE *fpWrite, PictImage *Pict, int width, int height) {
    int size;

    size = width * height;
    fwrite(Pict->lum,sizeof(unsigned char),size,fpWrite);
    fwrite(Pict->Cb,sizeof(unsigned char),size/4,fpWrite);
    fwrite(Pict->Cr,sizeof(unsigned char),size/4,fpWrite);
    return(0);    
}

int videoPSNR(PictImage *im1, PictImage *im2, int pels, int lines, pictSnr_p snr_p) {
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

int main(int argc, char **argv) {
	FILE      *fpRead1;
	FILE      *fpRead2; 
	char	   ReadName1[256];
	char       ReadName2[256];
    int        pels;
    int        lines;
    int        nbPict;   
    PictImage *inPict1;
    PictImage *inPict2;
    pictSnr_t  snr_t;
    int        targetPict = 9999;

      snr_t.meanY = 0.0;
      snr_t.meanU = 0.0;
      snr_t.meanV = 0.0;
    
	if ( argc < 5 ) {
		printf(" Usage: %s original_file_name input_file_name width height [number of pictures] \n",argv[0]);
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
	
	if ((fpRead2=fopen(ReadName2,"rb"))==NULL) {
		printf("error :cannot open %s\n",ReadName2);
		exit(-1);
	}
	
	if (argc == 6)
	    targetPict = atoi(argv[5]);    
    
    printf(" PSNR computation between two yuv files v1.0\n");
    printf(" Only 4:2:0 pictures are supported\n");
    printf(" send e-mail to info@tigress.fr for additional features\n\n");
	
    pels  = atoi(argv[3]); /* 176; */
    lines = atoi(argv[4]); /* 144; */
    
    inPict1      = (PictImage *)malloc(sizeof(PictImage));
    inPict1->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPict1->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);    
    inPict1->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);
    
    inPict2      = (PictImage *)malloc(sizeof(PictImage));
    inPict2->lum = (unsigned char *)malloc(sizeof(char)*pels*lines);
    inPict2->Cb  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);    
    inPict2->Cr  = (unsigned char *)malloc(sizeof(char)*pels*lines/4);

	nbPict = 0;
/*	memset(&snr_t,0,sizeof(snr_t)); */

	while (freadPict(fpRead1,inPict1,pels,lines) == 0) {
        if (freadPict(fpRead2,inPict2,pels,lines) == 0) {
            videoPSNR(inPict1,inPict2,pels,lines,&snr_t);
	        printf(" PSNR picture n°%04d  Y=%2.2f dB, U=%2.2f dB, V=%2.2f dB\n",nbPict,snr_t.Y,snr_t.U,snr_t.V);
	    }
	    else {
	        /* end of file */
	    }
		nbPict++;
		if (nbPict >= targetPict) break;
	}
      snr_t.meanY = (snr_t.meanY)/(float)nbPict;
      snr_t.meanU = (snr_t.meanU)/(float)nbPict;
      snr_t.meanV = (snr_t.meanV)/(float)nbPict;
	

	printf("+-------------------------------------------------------+\n");
	printf(" PSNR average         Y=%2.2f dB, U=%2.2f dB, V=%2.2f dB\n",snr_t.meanY,snr_t.meanU,snr_t.meanV);
	fclose(fpRead1);
	fclose(fpRead2);
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



