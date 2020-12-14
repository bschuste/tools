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
// File: viScycles.h
// History:
// 		Version 0.1	 2002/06/29
//						
////////////////////////////////////////////////////////////////////////

#ifndef VISCYCLES_H
#define VISCYCLES_H

#include <tm1/tmProcessor.h>
#include <ops/custom_defs.h>

#define PROFNB	8						/* Number of functions to profile */
		/*
		0 -- Whole decoder 
		1 -- Motion compensation 
		2 -- Texture decoding + IDCT + VLD + dequantification
		3 -- IDCT
		4 -- Global Motion Compensation
		5 -- Quarter Pel
		6 -- Available
		7 -- Deblocking
		*/
#define PROFDEC  0
#define PROFMCOM 1
#define PROFTEXT 2
#define PROFIDCT 3
#define PROFGMC  4
#define PROFQPEL 5
#define PROFXXXX 6
#define PROFDEBK 7

unsigned long	tmProfLo0[PROFNB];
unsigned long	tmProfLo1[PROFNB];
unsigned long	tmProfHi0[PROFNB];
unsigned long	tmProfHi1[PROFNB];
		 float	tmProfTotal[PROFNB];
		 FILE  *tmProfFile;
		 long	tmProfDiff;
unsigned long	tmProfFps;
unsigned long	tmProfPeakValue;
unsigned long	tmProfFrameNber;
 		 char   tmProfFrameType[8];
unsigned long	tmProfBytes;
		 char  *tmProfFunction[PROFNB]; /* Function name table */
		 int	profile;				/* profiling flag */


#define PRFOUT	   tmProfFile
#define PRF0(a)    if (tmProfFile) fprintf(PRFOUT,"%s, ",(a))
#define PRF1(a,b)  if (tmProfFile) fprintf(PRFOUT,"%s %03d, ",(a),(int)(b))
#define PRFB(a,b)  if (tmProfFile) fprintf(PRFOUT,"%s %05d, ",(a),(int)(b))
#define PRFF(a)	   if (tmProfFile) fprintf(PRFOUT,"%03d, ",(int)(a))
#define PRFN	   if (tmProfFile) fprintf(PRFOUT,"\n")

#define	GETCYCLES0(a) { \
		 tmProfLo0[(a)]  = cycles(); }

#define	GETCYCLES1(a) { \
	tmProfLo1[(a)]  = cycles();   \
	if (tmProfLo1[(a)]<tmProfLo0[(a)]) tmProfDiff = (0xffffffff-tmProfLo0[(a)]+tmProfLo1[(a)]); \
	else tmProfDiff = (tmProfLo1[(a)]-tmProfLo0[(a)]); \
		 tmProfTotal[(a)] += (float)tmProfDiff; }
	

#define	INITPROFILE { \
	tmProfDiff = 0;   \
	tmProfPeakValue = 0; \
	for (i=0;i<PROFNB;i++) tmProfTotal[i] = 0.0; \
	tmProfFunction[0]="dec";  \
	tmProfFunction[1]="mcom"; \
	tmProfFunction[2]="text"; \
	tmProfFunction[3]="vld";  \
	tmProfFunction[4]="gmc";  \
	tmProfFunction[5]="qpel"; \
	tmProfFunction[6]="xxxx"; \
	tmProfFunction[7]="debk"; \
	tmProfFile = fopen(strcat(file_name,".cyc"), "w"); \
	tmProfFrameNber = 0; \
	tmProfFps = fps; }

#define	INITCYCLES(a) { \
		 tmProfTotal[(a)] = 0; }

#define	STARTCYCLES {  \
		GETCYCLES0(0); \
		INITCYCLES(1); \
		INITCYCLES(2); \
		INITCYCLES(3); \
		INITCYCLES(4); \
		INITCYCLES(5); \
		INITCYCLES(6); \
		INITCYCLES(7); \
		PRFF(tmProfFrameNber+1); }

#define	STOPCYCLES {   \
		GETCYCLES1(0); \
		if (tmProfPeakValue<tmProfDiff) tmProfPeakValue=tmProfDiff; \
		PRF0(tmProfFrameType); \
		PRFB(("Kbps"), ((float) tmProfBytes * 8 * tmProfFps / 1.0e3));     \
		PRF1((tmProfFunction[0]), ((float) tmProfDiff * tmProfFps / 1.0e6));     \
		PRF1((tmProfFunction[1]), ((float) tmProfTotal[1] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[2]), ((float) tmProfTotal[2] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[3]), ((float) tmProfTotal[3] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[4]), ((float) tmProfTotal[4] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[5]), ((float) tmProfTotal[5] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[6]), ((float) tmProfTotal[6] * tmProfFps / 1.0e6)); \
		PRF1((tmProfFunction[7]), ((float) tmProfTotal[7] * tmProfFps / 1.0e6)); \
		PRFN; \
		tmProfFrameNber++;  }

#define	PRINTCYCLES { \
		procGetCapabilities(&cpuCap); \
		printf("cpuClockFrequency: %03.3f MHz\n",(float)cpuCap->cpuClockFrequency/1.0e6);              \
		printf("Total number of cycles: %f\n",tmProfTotal[0]);                                         \
		printf("Total number of frames: %d\n",tmProfFrameNber);                                        \
		printf("Frame size w=%d, h=%d\n",img_width,img_height);                                        \
		printf("Peak MHz = %f\n", ((float) tmProfPeakValue * tmProfFps / 1.0e6));                      \
		printf("Average MHz = %f\n", tmProfTotal[0] * tmProfFps / (1.0e6 * (float) tmProfFrameNber));  \
		printf("Actual in seconds: %02.3f\n",tmProfTotal[0]/(float)cpuCap->cpuClockFrequency);         \
		printf("Target in seconds: %02.3f\n",(float) (tmProfFrameNber+1)/tmProfFps);                   \
		fprintf(tmProfFile,"cpuClockFrequency: %03.3f MHz\n",(float)cpuCap->cpuClockFrequency/1.0e6);  \
		fprintf(tmProfFile,"Total number of cycles: %f\n",tmProfTotal[0]);                             \
		fprintf(tmProfFile,"Total number of frames: %d\n",tmProfFrameNber);                            \
		fprintf(tmProfFile,"Frame size w=%d, h=%d\n",img_width,img_height);                            \
		fprintf(tmProfFile,"Peak MHz = %f\n", ((float) tmProfPeakValue * tmProfFps / 1.0e6));          \
		fprintf(tmProfFile,"Average MHz = %f\n", tmProfTotal[0] * tmProfFps / (1.0e6 * (float) tmProfFrameNber)); \
		fprintf(tmProfFile,"Actual in seconds: %02.3f\n",tmProfTotal[0]/(float)cpuCap->cpuClockFrequency);        \
		fprintf(tmProfFile,"Target in seconds: %02.3f\n",(float) (tmProfFrameNber+1)/tmProfFps);  }

#endif /* CYCLES_H */
