/***********************************************************************
 **
   Copyright (c) Electropix 2010.                                        
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
   Last update: 2010/12/19
   Modified:
   File: elbmp.h
   History:
        Version 0.1  2010/12/19
 **
 ***********************************************************************/
#ifndef EL_BMP_H
#define EL_BMP_H

#define BI_RGB          0    // No compression
#define BI_RLE8         1    // RLE8 compression (256 colors)
#define BI_RLE4         2    // RLE4 compression (16 colors)


typedef struct bmpFileHeader {
    unsigned short    bfType;
    unsigned int      bfSize;
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    unsigned int      bfOffBits;
} bmpFileHeader_t, *bmpFileHeader_p;

typedef struct bmpInfoHeader 
{
    unsigned int      biSize;
    unsigned int      biWidth;
    unsigned int      biHeight;
    unsigned short    biPlanes;
    unsigned short    biBitCount;
    unsigned int      biCompression;
    unsigned int      biSizeImage;
    unsigned int      biXPelsPerMeter;
    unsigned int      biYPelsPerMeter;
    unsigned int      biClrUsed;
    unsigned int      biClrImportant;
} bmpInfoHeader_t, *bmpInfoHeader_p;

typedef struct bmpRgbQuad
{
    unsigned char    rgbBlue;
    unsigned char    rgbGreen;
    unsigned char    rgbRed;
    unsigned char    rgbReserved;
} bmpRgbQuad_t, *bmpRgbQuad_p;

#endif  /* EL_BMP_H */
