#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/elportab.h"
#include "../inc/elbmp.h"
#include "../inc/elvideo.h"

/* unused */
#if 0
static void rgbPlanesHflip(Picture_p rgb_p)
{
    UInt8  byte;
    Int32  i, j;

    for(j=0; j < rgb_p->h0>>1; j++)
    {
        for(i=0 ; i < rgb_p->w0 ; i++)
        {
            byte = rgb_p->c0[j*rgb_p->w0 + i];
            rgb_p->c0[j*rgb_p->w0 + i] = rgb_p->c0[(rgb_p->h0-j-1)*rgb_p->w0 + i];
            rgb_p->c0[(rgb_p->h0-j-1)*rgb_p->w0 + i] = byte;
        }
    }
    for(j=0; j < rgb_p->h1>>1; j++)
    {
        for(i=0 ; i < rgb_p->w1 ; i++)
        {
            byte = rgb_p->c1[j*rgb_p->w1 + i];
            rgb_p->c1[j*rgb_p->w1 + i] = rgb_p->c1[(rgb_p->h1-j-1)*rgb_p->w1 + i];
            rgb_p->c1[(rgb_p->h1-j-1)*rgb_p->w1 + i] = byte;
        }
    }
    for(j=0; j < rgb_p->h2>>1; j++)
    {
        for(i=0 ; i < rgb_p->w2 ; i++)
        {
            byte = rgb_p->c2[j*rgb_p->w2 + i];
            rgb_p->c2[j*rgb_p->w2 + i] = rgb_p->c2[(rgb_p->h2-j-1)*rgb_p->w2 + i];
            rgb_p->c2[(rgb_p->h2-j-1)*rgb_p->w2 + i] = byte;
        }
    }
}
#endif

static void Convert2bmp(char *inname, char *outname, int width, int height) 
{
    FILE *in, *out;
    
    bmpFileHeader_t fileHd;
    bmpInfoHeader_t infoHd;
    Picture_t       rgb;
    int scanLineSize, r, c;


    in = fopen (inname, "rb");
    if (in == NULL) {
        printf ("unable to open %s\n", inname);
        return;
    }

    out = fopen (outname, "wb");
    if (out == NULL) {
        printf ("Unable to open %s\n", outname);
        return;
    }

    infoHd.biSize           = 40;
    infoHd.biWidth          = width;
    infoHd.biHeight         = height;
    infoHd.biPlanes         = 1;
    infoHd.biBitCount       = 24;
    infoHd.biCompression    = BI_RGB;
    infoHd.biSizeImage      = 3 * width * height;
    infoHd.biXPelsPerMeter  = 2824;
    infoHd.biYPelsPerMeter  = 2834;
    infoHd.biClrUsed        = 0;
    infoHd.biClrImportant   = 0;
    
    fileHd.bfType       = 0x4d42;
    fileHd.bfReserved1  = 0;
    fileHd.bfReserved2  = 0;
    fileHd.bfOffBits    = sizeof(bmpFileHeader_t) + sizeof(bmpInfoHeader_t);//56;
    fileHd.bfSize       = infoHd.biSizeImage + fileHd.bfOffBits;

    fwrite (&fileHd.bfType, 1, sizeof (fileHd.bfType), out);
    fwrite (&fileHd.bfSize, 1, sizeof (fileHd.bfSize), out);
    fwrite (&fileHd.bfReserved1, 1, sizeof (fileHd.bfReserved1), out);
    fwrite (&fileHd.bfReserved2, 1, sizeof (fileHd.bfReserved2), out);
    fwrite (&fileHd.bfOffBits, 1, sizeof (fileHd.bfOffBits), out);
    fwrite (&infoHd.biSize, 1, sizeof (infoHd.biSize), out);
    fwrite (&infoHd.biWidth, 1, sizeof (infoHd.biWidth), out);
    fwrite (&infoHd.biHeight, 1, sizeof (infoHd.biHeight), out);
    fwrite (&infoHd.biPlanes, 1, sizeof (infoHd.biPlanes), out);
    fwrite (&infoHd.biBitCount, 1, sizeof (infoHd.biBitCount), out);
    fwrite (&infoHd.biCompression, 1, sizeof (infoHd.biCompression), out);
    fwrite (&infoHd.biSizeImage, 1, sizeof (infoHd.biSizeImage), out);
    fwrite (&infoHd.biXPelsPerMeter, 1, sizeof (infoHd.biXPelsPerMeter), out);
    fwrite (&infoHd.biYPelsPerMeter, 1, sizeof (infoHd.biYPelsPerMeter), out);
    fwrite (&infoHd.biClrUsed, 1, sizeof (infoHd.biClrUsed), out);
    fwrite (&infoHd.biClrImportant, 1, sizeof (infoHd.biClrImportant), out);

    printf (" --> File Header, %d\n", sizeof(bmpFileHeader_t));
    printf (" --> bfType           0x%04x\n", fileHd.bfType);
    printf ("     bfSize           %d\n", fileHd.bfSize);
    printf ("     bfReserved1      %d\n", fileHd.bfReserved1);
    printf ("     bfReserved2      %d\n", fileHd.bfReserved2);
    printf ("     bfOffBits        %d\n", fileHd.bfOffBits);
    fwrite (&fileHd, 1, sizeof(bmpFileHeader_t), out);

    printf (" --> Info Header, %d\n", sizeof(bmpInfoHeader_t));
    printf ("     biSize           %d\n", infoHd.biSize);
    printf ("     biWidth          %d\n", infoHd.biWidth);
    printf ("     biHeight         %d\n", infoHd.biHeight);
    printf ("     biPlanes         %d\n", infoHd.biPlanes);
    printf ("     biBitCount       %d\n", infoHd.biBitCount);
    printf ("     biCompression    %d\n", infoHd.biCompression);
    printf ("     biSizeImage      %d\n", infoHd.biSizeImage);
    printf ("     biXPelsPerMeter  %d\n", infoHd.biXPelsPerMeter);
    printf ("     biYPelsPerMeter  %d\n", infoHd.biYPelsPerMeter);
    printf ("     biClrUsed        %d\n", infoHd.biClrUsed);
    printf ("     biClrImportant   %d\n", infoHd.biClrImportant);
    fwrite (&infoHd, 1, sizeof(bmpInfoHeader_t), out);
    
    printf("\nNumber of colors: %d\n", (1 << infoHd.biBitCount));

    scanLineSize = (((int) infoHd.biWidth + 3)) & ~3;
    printf("\nscanLineSize: %d\n", scanLineSize);

    printf ("output  size: ht %d wd %d\n", infoHd.biHeight, infoHd.biWidth);
    
    /* Define type as RGBA */
    rgb.type = PT_RGBA;
    /* R component */
    rgb.w0 = width;
    rgb.h0 = height;
    /* G component */
    rgb.w1 = width;
    rgb.h1 = height;
    /* B component */
    rgb.w2 = width;
    rgb.h2 = height;
    /* A component */
    rgb.w3 = width;
    rgb.h3 = height;
    
    rgb.c0 = (UInt8 *)malloc((rgb.w0) * (rgb.h0) * sizeof(UInt8));
    rgb.c1 = (UInt8 *)malloc((rgb.w1) * (rgb.h1) * sizeof(UInt8));
    rgb.c2 = (UInt8 *)malloc((rgb.w2) * (rgb.h2) * sizeof(UInt8));
    rgb.c3 = (UInt8 *)malloc((rgb.w3) * (rgb.h3) * sizeof(UInt8));

    int nb;
    for (r = 0; r < height; r++) 
    {
        for (c = 0; c < width; c++) 
        {
            nb = fread (&rgb.c2[r*rgb.w2 + c], sizeof(UInt8), 1, in); /* B */
            nb = fread (&rgb.c1[r*rgb.w1 + c], sizeof(UInt8), 1, in); /* G */
            nb = fread (&rgb.c0[r*rgb.w0 + c], sizeof(UInt8), 1, in); /* R */
            nb = fread (&rgb.c3[r*rgb.w3 + c], sizeof(UInt8), 1, in); /* A */
        }
    }
    fprintf(stderr, " Buffer flip done\n");
    
    for (r = 0; r < height; r++) 
    {
        fseek (out, fileHd.bfOffBits + (long int) scanLineSize * (infoHd.biHeight - r - 1) * 3, SEEK_SET);
        for (c = 0; c < width; c++) 
        {
            fwrite (&rgb.c2[r*rgb.w2 + c], sizeof(UInt8), 1, out);  //B
            fwrite (&rgb.c1[r*rgb.w1 + c], sizeof(UInt8), 1, out);  //G
            fwrite (&rgb.c0[r*rgb.w0 + c], sizeof(UInt8), 1, out);  //R
        }
    }

    fclose (in);
    fclose (out);

    free(rgb.c0);
    free(rgb.c1);
    free(rgb.c2);
    free(rgb.c3);
    
} /* save_bmp */



int main (int noarg, char *argv []) {
    char ReadName [256];
    char WriteName[256];
    int i;
    int width, height;

    for (i=0;i<256;i++) {
        ReadName[i] = '\0';
        WriteName[i] = '\0';
    }
    if (noarg < 4) {
        printf ("Usage: %s <input filename> <width> <height>\n",
            argv [0]);
        return (1);
    }

    strcpy(ReadName,argv[1]);
    strncpy(WriteName, ReadName, strlen(ReadName)-5);
    strcat(WriteName, ".bmp");
    
    width  = atoi(argv[2]);
    height = atoi(argv[3]);

    Convert2bmp(ReadName, WriteName, width, height);

    return (0);
}
