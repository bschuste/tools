////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Electropix 2007. 							             
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
// Electropix Research and Development does not represent or warrant that
// the programs furnished hereunder are free of infringement of any
// third-party patents.
//
////////////////////////////////////////////////////////////////////////
//
// Author: Brigitte Schuster
// Last update: 2002/06/29                                                  
// Modified:                                                   
// File: viSdisplay.c
// History:
// 		Version 0.1	 2002/06/29
//						
////////////////////////////////////////////////////////////////////////
/*
 * Example of glDrawPixels
 *
 * Link with the following libraries: opengl32.lib glu32.lib glaux.lib 
 */

//#include "glos.h"
#include <windows.h>
// disable data conversion warnings
//#pragma warning(disable : 4136)     // X86
//#pragma warning(disable : 4309)     // X86


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glaux.h"
#include "types.h"


GLenum doubleBuffer;
GLint  windW, windH;


static void Init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glDisable(GL_DITHER);
    glViewport(0, 0, windW, windH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-0.5, windW+0.5, -0.5, windH+0.5);
    glMatrixMode(GL_MODELVIEW);
}

UInt8  pixs[240][320][3];

int freadPicture(FILE *fpRead, Picture_p yuv_p)
{
	int nbBytes = 0;

    nbBytes = fread(yuv_p->c0, sizeof(UInt8), yuv_p->w0*yuv_p->h0, fpRead);
	printf("nbBytes Y = %d\n",nbBytes);
	if (nbBytes != 320*240) printf("error\n");
    nbBytes = fread(yuv_p->c1, sizeof(UInt8), yuv_p->w1*yuv_p->h1, fpRead);
	printf("nbBytes U = %d\n",nbBytes);
	if (nbBytes != 160*120) printf("error\n");
	memset(yuv_p->c1,128,160*120);
    nbBytes = fread(yuv_p->c2, sizeof(UInt8), yuv_p->w2*yuv_p->h2, fpRead);
	printf("nbBytes V = %d\n",nbBytes);
	if (nbBytes != 160*120) printf("error\n");
	memset(yuv_p->c2,128,160*120);
	
	return(0);

}


int yuvtorgb(Picture_p yuv_p, Picture_p rgb_p) {
  register  UInt16	i,j;
  register	double	R,G,B;
			double	CB,CR;

	UInt8	*y, *u, *v, *r, *g, *b;
	UInt8	*c0, *c1, *c2;

	y = yuv_p->c0;
	u = yuv_p->c1;
	v = yuv_p->c2;

	r = rgb_p->c0;
	g = rgb_p->c1;
	b = rgb_p->c2;
	
	c0 = yuv_p->c0;
	c1 = yuv_p->c1;
	c2 = yuv_p->c2;
	for (i=0;i<8;i++) {
		printf("O1=%x",*c0);
		c0++;
		printf("\n");
	}
	for (i=0;i<8;i++) {
		printf("O1=%x",*c1);
		c1++;
		printf("\n");
	}
	for (i=0;i<8;i++) {
		printf("O1=%x",*c2);
		c2++;
		printf("\n");
	}
//	printf("y = %d\n",*y);
//	printf("u = %d\n",*u);
//	printf("v = %d\n",*v);
	for(j=0 ; j < yuv_p->h0 ; j++)
	{
		for(i=0 ; i < yuv_p->w0 ; i++)
		{
			CB = (double)(0.25*(*u) + 0.25*(*(u + 1))
				        + 0.25*(*(u + yuv_p->w1)) + 0.25*(*(u + yuv_p->w1 + 1)));

			CR = (double)(0.25*(*v) + 0.25*(*(v + 1))
				        + 0.25*(*(v + yuv_p->w2)) + 0.25*(*(v + yuv_p->w2 + 1)));

          	R = ((double)*y + 1.371*(CR-128.0) + 0.000*(CB-128.0));
			R = R>255.0?255:R<0.0?0:R;
			*r = (UInt8)R;

         	G = ((double)*y - 0.698*(CR-128.0) - 0.336*(CB-128.0));
 			G = G>255.0?255:G<0.0?0:G;
 			*g = (UInt8)G;

			B = ((double)*y + 0.000*(CR-128.0) + 1.732*(CB-128.0));
 			B = B>255.0?255:B<0.0?0:B;
			*b = (UInt8)B;
			y++;
			u++;
			v++;
			r++;
			g++;
			b++;
		}
	}
	return(0);
}


int main(int argc, char **argv)
{
    GLenum type;
	FILE	*fpRead;
	char	ReadName[256];
	int		i,j,n;
	int		NumberOfPictures;
	Picture_t	yuv;
	Picture_t	rgb;
	Picture_p   yuv_p;
	Picture_p   rgb_p;
	UInt8	*c0, *c1, *c2;

	yuv_p = &yuv;
	rgb_p = &rgb;

	if ( argc < 7 )
	{
		printf("%s -w <width> -h <height> file_name nb_frames \n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName,argv[5]);
		NumberOfPictures = atoi(argv[6]);
		printf("ReadName = %s\n",argv[5]);
		printf("NumberOfPictures = %d\n",NumberOfPictures);

		if (argv[1][0] == '-')
		{
			if (argv[1][1] == 'w') {
				printf("argv[2] = %s\n",argv[2]);
				yuv_p->w0 = atoi(argv[2]);
				yuv_p->w1 = yuv_p->w0/2;
				yuv_p->w2 = yuv_p->w0/2;
				rgb_p->w0 = yuv_p->w0;
				rgb_p->w1 = rgb_p->w0;
				rgb_p->w2 = rgb_p->w0;
			}
			else if (argv[1][1] == 'h') {
				printf("argv[2] = %s\n",argv[2]);
				yuv_p->h0 = atoi(argv[2]);
				yuv_p->h1 = yuv_p->h0/2;
				yuv_p->h2 = yuv_p->h0/2;
				rgb_p->h0 = yuv_p->h0;
				rgb_p->h1 = rgb_p->h0;
				rgb_p->h2 = rgb_p->h0;
			}
			else
			{
				printf("%s -w <width> -h <height> file_name nb_frames \n",argv[0]);
				exit(1);
			}
		}

		if (argv[3][0] == '-')
		{
			if (argv[3][1] == 'w') {
				yuv_p->w0 = atoi(argv[4]);
				yuv_p->w1 = yuv_p->w0/2;
				yuv_p->w2 = yuv_p->w0/2;
				rgb_p->w0 = yuv_p->w0;
				rgb_p->w1 = rgb_p->w0;
				rgb_p->w2 = rgb_p->w0;
			}
			else if (argv[3][1] == 'h') {
				printf("argv[4] = %s\n",argv[4]);
				yuv_p->h0 = atoi(argv[4]);
				yuv_p->h1 = yuv_p->h0/2;
				yuv_p->h2 = yuv_p->h0/2;
				rgb_p->h0 = yuv_p->h0;
				rgb_p->h1 = rgb_p->h0;
				rgb_p->h2 = rgb_p->h0;
			}
			else
			{
				printf("%s -w <width> -h <height> file_name nb_frames \n",argv[0]);
				exit(1);
			}
		}
		else
		{
			printf("%s -w <width> -h <height> file_name nb_frames \n",argv[0]);

		}
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName);
		exit(-1);
	}

    /* + 1 is for filteting purpose */
	yuv_p->c0 = (UInt8 *)malloc((yuv_p->w0 + 1) * (yuv_p->h0 + 1) * sizeof(UInt8));
	yuv_p->c1 = (UInt8 *)malloc((yuv_p->w1 + 1) * (yuv_p->h1 + 1) * sizeof(UInt8));
	yuv_p->c2 = (UInt8 *)malloc((yuv_p->w2 + 1) * (yuv_p->h2 + 1) * sizeof(UInt8));
	rgb_p->c0 = (UInt8 *)malloc((rgb_p->w0 + 1) * (rgb_p->h0 + 1) * sizeof(UInt8));
	rgb_p->c1 = (UInt8 *)malloc((rgb_p->w1 + 1) * (rgb_p->h1 + 1) * sizeof(UInt8));
	rgb_p->c2 = (UInt8 *)malloc((rgb_p->w2 + 1) * (rgb_p->h2 + 1) * sizeof(UInt8));
	windW = rgb_p->w0;
	windH = rgb_p->h0;

	printf("yuv_p->c0 = 0x%x\n",yuv_p->c0);
	printf("yuv_p->c1 = 0x%x\n",yuv_p->c1);
	printf("yuv_p->c2 = 0x%x\n",yuv_p->c2);
	printf("yuv_p->w0 = %d\n",yuv_p->w0);
	printf("yuv_p->h0 = %d\n",yuv_p->h0);
	printf("yuv_p->w1 = %d\n",yuv_p->w1);
	printf("yuv_p->h1 = %d\n",yuv_p->h1);
	printf("yuv_p->w2 = %d\n",yuv_p->w2);
	printf("yuv_p->h2 = %d\n",yuv_p->h2);
	printf("rgb_p->c0 = 0x%x\n",rgb_p->c0);
	printf("rgb_p->c1 = 0x%x\n",rgb_p->c1);
	printf("rgb_p->c2 = 0x%x\n",rgb_p->c2);
	printf("rgb_p->w0 = %d\n",rgb_p->w0);
	printf("rgb_p->h0 = %d\n",rgb_p->h0);
	printf("rgb_p->w1 = %d\n",rgb_p->w1);
	printf("rgb_p->h1 = %d\n",rgb_p->h1);
	printf("rgb_p->w2 = %d\n",rgb_p->w2);
	printf("rgb_p->h2 = %d\n",rgb_p->h2);

	auxInitPosition(200, 0, windW, windH);
	type  = AUX_RGB;
	type |=  AUX_SINGLE;
	auxInitDisplayMode(type);
	
	if (auxInitWindow("avSdisplay") == GL_FALSE) 
		{
			auxQuit();
		}
	Init();

	printf("windW = %d, windH = %d\n",windW,windH);
	for(n=0; n<NumberOfPictures; n++)
	{
		freadPicture(fpRead, yuv_p);
/*		c0 = yuv_p->c0;
		for (i=0;i<8;i++) {
			printf("O1=%x",*c0);
			c0++;
			printf("\n");
		} */

		yuvtorgb(yuv_p, rgb_p);
/*
		c0 = rgb_p->c0;
		c1 = rgb_p->c1;
		c2 = rgb_p->c2;
		
		for (i=0;i<8;i++) {
			printf("O1=%x",*c0);
			c0++;
			printf("\n");
		}
		for (i=0;i<8;i++) {
			printf("O1=%x",*c1);
			c1++;
			printf("\n");
		}	
		for (i=0;i<8;i++) {
			printf("O1=%x",*c2);
			c2++;
			printf("\n");
		}
		*/
		c0 = rgb_p->c0;
		c1 = rgb_p->c1;
		c2 = rgb_p->c2;
		
		for (j=0; j<windW; j++)
		{
			for (i=0; i<windH; i++)
			{
				/*
				pixs[i][j][0] = *c0;
				pixs[i][j][1] = *c1;
				pixs[i][j][2] = *c2;
				c0++;
				c1++;
				c2++; 
				*/
				pixs[i][j][0] = *(c0 + rgb_p->w0 * (rgb_p->h0-1-i));
				pixs[i][j][1] = *(c1 + rgb_p->w1 * (rgb_p->h1-1-i));
				pixs[i][j][2] = *(c2 + rgb_p->w2 * (rgb_p->h2-1-i));
				c0++;
				c1++;
				c2++; 
			}
		}
		glDrawPixels(windW,windH,GL_RGB,GL_UNSIGNED_BYTE,pixs);
	} 
	free(yuv_p->c0);
	free(yuv_p->c1);
	free(yuv_p->c2);
	free(rgb_p->c0);
	free(rgb_p->c1);
	free(rgb_p->c2);

  }
