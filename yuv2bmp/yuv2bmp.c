#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/elportab.h"
#include "../inc/elbmp.h"
#include "../inc/elvideo.h"


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

static int yuv420torgb(Picture_p yuv_p, Picture_p rgb_p) 
{
  register  UInt16  i,j;
  register  double  R,G,B;
            double  CB,CR;

    UInt8   *y, *u, *v, *r, *g, *b;
    UInt8   *c0, *c1, *c2;

    y = yuv_p->c0;
    u = yuv_p->c1;
    v = yuv_p->c2;

    r = rgb_p->c0;
    g = rgb_p->c1;
    b = rgb_p->c2;
    
    c0 = yuv_p->c0;
    c1 = yuv_p->c1;
    c2 = yuv_p->c2;
    printf("O0= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c0);
        c0++;
    }
    printf("\nO1= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c1);
        c1++;
    }
    printf("\nO2= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c2);
        c2++;
    }
    printf("\n");
    
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
static int yuv422torgb(Picture_p yuv_p, Picture_p rgb_p) 
{
  register  UInt16  i,j;
  register  double  R,G,B;
            double  CB,CR;

    UInt8   *y, *u, *v, *r, *g, *b;
    UInt8   *c0, *c1, *c2;

    y = yuv_p->c0;
    u = yuv_p->c1;
    v = yuv_p->c2;

    r = rgb_p->c0;
    g = rgb_p->c1;
    b = rgb_p->c2;
    
    c0 = yuv_p->c0;
    c1 = yuv_p->c1;
    c2 = yuv_p->c2;
    printf("O0= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c0);
        c0++;
    }
    printf("\nO1= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c1);
        c1++;
    }
    printf("\nO2= ");
    for (i=0;i<8;i++) {
        printf("%x ",*c2);
        c2++;
    }
    printf("\n");
    
    for(j=0 ; j < yuv_p->h0 ; j++)
    {
        for(i=0 ; i < yuv_p->w0 ; i++)
        {
            if (i&1)
            {
                CB = (double)(0.25*(*u) + 0.75*(*(u + 1)));
                CR = (double)(0.25*(*v) + 0.75*(*(v + 1)));
            }
            else
            {
                CB = (double)(0.75*(*u) + 0.25*(*(u + 1)));
                CR = (double)(0.75*(*v) + 0.25*(*(v + 1)));
            }
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
            
            if (i&1)
            {
                u++;
                v++;
            }
            r++;
            g++;
            b++;
        }
    }
    return(0);
}

static void Convert2bmp(char *inname, char *outname, int width, int height) 
{
    FILE *in, *out;
    
    bmpFileHeader_t fileHd;
    bmpInfoHeader_t infoHd;
    Picture_t       yuv;
    Picture_t       rgb;
    int scanLineSize, r, c;
    unsigned char *y, *u, *v;


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

    /*  we should only this much to yuv or have only this much available  
    rcopy = infoHd.biHeight;
    ccopy = infoHd.biWidth;
     */

    /* Define type by default as YUV 422 */
    yuv.type = PT_YUV422;
    /* Y component */
    yuv.w0 = width;
    yuv.h0 = height;
    /* U component */
    yuv.w1 = width>>1;
    yuv.h1 = height;
    /* V component */
    yuv.w2 = width>>1;
    yuv.h2 = height;
    yuv.c0 = (UInt8 *)malloc((yuv.w0) * (yuv.h0) * sizeof(UInt8));
    /* + 1 to support upscale filter boundaries */
    yuv.c1 = (UInt8 *)malloc((yuv.w1 + 1) * (yuv.h1) * sizeof(UInt8));
    /* + 1 to support upscale filter boundaries */
    yuv.c2 = (UInt8 *)malloc((yuv.w2 + 1) * (yuv.h2) * sizeof(UInt8));
    
    /* Unused */
    yuv.c3 = NULL; 
    yuv.w3 = 0;
    yuv.h3 = 0;

    y = yuv.c0;
    u = yuv.c1;
    v = yuv.c2;
    
    /* Define type as RGB */
    rgb.type = PT_RGB;
    /* R component */
    rgb.w0 = width;
    rgb.h0 = height;
    /* G component */
    rgb.w1 = width;
    rgb.h1 = height;
    /* B component */
    rgb.w2 = width;
    rgb.h2 = height;
    
    rgb.c0 = (UInt8 *)malloc((rgb.w0) * (rgb.h0) * sizeof(UInt8));
    rgb.c1 = (UInt8 *)malloc((rgb.w1) * (rgb.h1) * sizeof(UInt8));
    rgb.c2 = (UInt8 *)malloc((rgb.w2) * (rgb.h2) * sizeof(UInt8));

    /* Unused */
    rgb.c3 = NULL; 
    rgb.w3 = 0;
    rgb.h3 = 0;

    int nb;
    nb = fread (yuv.c0, sizeof(UInt8), yuv.w0 * yuv.h0, in); /* Y */
    nb = fread (yuv.c1, sizeof(UInt8), yuv.w1 * yuv.h1, in); /* U */
    nb = fread (yuv.c2, sizeof(UInt8), yuv.w2 * yuv.h2, in); /* V */

    fprintf(stderr, " reading YUV file done\n");
    if (yuv.type == PT_YUV422)
    {
        yuv422torgb (&yuv, &rgb);
    }
    else
    {
        fprintf(stderr, " This type of conversion is not supported, only from YUV422\n");
        goto done;
    }
    fprintf(stderr, " YUV to RGB conversion done\n");

//    rgbPlanesHflip(&rgb);
    fprintf(stderr, " Buffer flip done\n");
    
    for (r = 0; r < height; r++) {
        fseek (out, fileHd.bfOffBits + (long int) scanLineSize * (infoHd.biHeight - r - 1) * 3, SEEK_SET);
        for (c = 0; c < width; c++) 
        {
            fwrite (&rgb.c2[r*rgb.w2 + c], 1, 1, out);  //B
            fwrite (&rgb.c1[r*rgb.w1 + c], 1, 1, out);  //G
            fwrite (&rgb.c0[r*rgb.w0 + c], 1, 1, out);  //R
        }
    }

done:
    fclose (in);
    fclose (out);

    free(yuv.c0);
    free(yuv.c1);
    free(yuv.c2);
    free(rgb.c0);
    free(rgb.c1);
    free(rgb.c2);
    
} /* save_bmp */


int main (int noarg, char *argv []) 
{
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
    strncpy(WriteName, ReadName, strlen(ReadName)-4);
    strcat(WriteName, ".bmp");
    
    width  = atoi(argv[2]);
    height = atoi(argv[3]);

    Convert2bmp(ReadName, WriteName, width, height);
    return (0);
    
}
