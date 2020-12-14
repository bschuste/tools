#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ops/custom_defs.h>
#include <tmNxTypes.h>

/*

 tmcc -o b2y.out -B -el -host tmsim bmp2yuvosd.c -IC:\NEXPERIA\TCS22DVP\include
 tmsim b2y.out 

 */
#define BI_RGB          0    // No compression
#define BI_RLE8         1    // RLE8 compression (256 colors)
#define BI_RLE4         2    // RLE4 compression (16 colors)

#ifdef WinNT
#define UCLIPU(x,y) ((x)>(y)?(y):(x))
#endif

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

Int32 bmp_wd = 720, bmp_ht = 480; /* FIXTHIS */

static void bmpBufferFlip(Int32 biWidth, Int32 biHeight, UInt8 *buffer)
{
  UInt8  byte;
  Int32  x, y;

  for (y = 0; y < biHeight/2; y++)
    for (x = 0; x < biWidth; x++) {
      byte = buffer[y*biWidth+x];
      buffer[y*biWidth+x] = buffer[(biHeight-y-1)*biWidth+x];
      buffer[(biHeight-y-1)*biWidth+x] = byte;
    }
}


static void bmpConvert(Char *inname, Char *outbase) {
    Char filename [81];
    UInt8 R, G, B, dummy;
    Int32 scanLineSize, wd, ht, i, j, r, c, rcopy, ccopy;
    Int32 l;
	double RR, GG, BB;
    FILE *in, *YUV, *out;
    bmpFileHeader_t fileHd;
    bmpInfoHeader_t infoHd;
    UInt8 *yBuf_p, *uBuf_p, *vBuf_p, *aBuf_p;
    UInt8 *y, *u, *v, *a;

	printf ("inname %s outbase %s\n", inname, outbase);

    in = fopen (inname, "rb");
    if (in == NULL) {
        printf ("unable to open %s\n", inname);
        return;
    }

	sprintf (filename, "%s.yuv", outbase);
	YUV = fopen (filename, "wb");
	if (YUV == NULL) {
		printf ("Unable to open %s\n", filename);
		return;
	}
	sprintf (filename, "%s.h", outbase);
	out = fopen (filename, "w");
	if (out == NULL) {
		printf ("Unable to open %s\n", filename);
		return;
	}

    fread (&fileHd.bfType, 1, sizeof (fileHd.bfType), in);
    fread (&fileHd.bfSize, 1, sizeof (fileHd.bfSize), in);
    fread (&fileHd.bfReserved1, 1, sizeof (fileHd.bfReserved1), in);
    fread (&fileHd.bfReserved2, 1, sizeof (fileHd.bfReserved2), in);
    fread (&fileHd.bfOffBits, 1, sizeof (fileHd.bfOffBits), in);
    fread (&infoHd.biSize, 1, sizeof (infoHd.biSize), in);
    fread (&infoHd.biWidth, 1, sizeof (infoHd.biWidth), in);
    fread (&infoHd.biHeight, 1, sizeof (infoHd.biHeight), in);
    fread (&infoHd.biPlanes, 1, sizeof (infoHd.biPlanes), in);
    fread (&infoHd.biBitCount, 1, sizeof (infoHd.biBitCount), in);
    fread (&infoHd.biCompression, 1, sizeof (infoHd.biCompression), in);
    fread (&infoHd.biSizeImage, 1, sizeof (infoHd.biSizeImage), in);
    fread (&infoHd.biXPelsPerMeter, 1, sizeof (infoHd.biXPelsPerMeter), in);
    fread (&infoHd.biYPelsPerMeter, 1, sizeof (infoHd.biYPelsPerMeter), in);
    fread (&infoHd.biClrUsed, 1, sizeof (infoHd.biClrUsed), in);
    fread (&infoHd.biClrImportant, 1, sizeof (infoHd.biClrImportant), in);

	printf (" --> File Header   0x%04x\n", fileHd.bfType);
	printf ("     bfSize    0x%08x\n", fileHd.bfSize);
	printf ("     bfOffBits 0x%08x\n", fileHd.bfOffBits);
	printf (" --> Info Header   \n");
	printf ("     biSize    0x%08x\n", infoHd.biSize);
	printf ("     biWidth   0x%08x\n", infoHd.biWidth);
	printf ("     biHeight  0x%08x\n", infoHd.biHeight);
	printf ("     biCompression %d\n", infoHd.biCompression);
	printf ("     biBitCount    %d\n", infoHd.biBitCount);
	printf ("     biSizeImage   %d\n", infoHd.biSizeImage);
	
//    bmnumcolors = (1 << infoHd.biBitCount);
    printf("\nNumber of colors: %d\n", (1 << infoHd.biBitCount));

    scanLineSize = (((int) infoHd.biWidth + 3)) & ~3;
    printf("\nscanLineSize: %d\n", scanLineSize);

	printf ("input  size: ht %ld wd %ld\n", infoHd.biHeight, infoHd.biWidth);

	/*	we should only this much to yuv or have only this much available */	
	rcopy = infoHd.biHeight;
	ccopy = infoHd.biWidth;
	printf ("rcopy %d ccopy %d\n", rcopy, ccopy);

#ifdef WinNT
	yBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	uBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	vBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	aBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
#else
	yBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,0) );
	uBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,1) );
	vBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,2) );
	aBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,3) );
#endif

    fprintf(out,"#define %s_WIDTH    %d\n",outbase,ccopy);
	/* BS 2003/11/23 to have IO Display Logo the right size */
//    fprintf(out,"#define %s_HEIGHT   %d\n",outbase,rcopy*2); /* who knows why */
    fprintf(out,"#define %s_HEIGHT   %d\n",outbase,rcopy); /* who knows why */
    fprintf(out,"#define %s_STRIDE   %d\n",outbase,ccopy);
    
    fprintf(out,"Int8 %s[4 * %d * %d] = {\n",outbase,ccopy,rcopy);

    y = yBuf_p;
    u = uBuf_p;
    v = vBuf_p;
	for (r = 0; r < rcopy; r++) {
		printf ("\rr = %d", r);
        fseek (in, fileHd.bfOffBits + (long int) scanLineSize * (infoHd.biHeight - r - 1) * 3, SEEK_SET);
		for (c = 0; c < ccopy; c++) {
			fread (&B, 1, 1, in);
			fread (&G, 1, 1, in);
			fread (&R, 1, 1, in);

			/*	normalize to 16.. 235 */
			RR = 219. * R / 255.0 + 16;
			GG = 219. * G / 255.0 + 16;
			BB = 219. * B / 255.0 + 16;

			*y = UCLIPU((0.2991 * RR + 0.5849 * GG + 0.1159 * BB), 255);
			*u = -0.1725 * RR - 0.3373 * GG + 0.5097 * BB + 128.5;
			*v =  0.5097 * RR - 0.4254 * GG - 0.0843 * BB + 128.5;
            y++;
	        if (c%2 == 0) u++;
	        if (c%2 == 0) v++;
		}
	}
	//Flipping only if biHeight is negative 
/*	bmpBufferFlip(ccopy,rcopy,yBuf_p);
	bmpBufferFlip(ccopy/2,rcopy,vBuf_p);
	bmpBufferFlip(ccopy/2,rcopy,uBuf_p); */
	fwrite (yBuf_p, 1, rcopy*ccopy, YUV); /* Y */
    fwrite (uBuf_p, 1, rcopy*ccopy/2, YUV); /* U */
    fwrite (vBuf_p, 1, rcopy*ccopy/2, YUV); /* V */
    y = yBuf_p;
    u = uBuf_p;
    v = vBuf_p;
	/* OSDIMAGE is organized in little endian [U+a, Y, V+a, Y] */                               
	for (r = 0; r < rcopy; r++) {
		for (c = 0; c < ccopy/2; c++) {
		    if (c%(ccopy-1) == 0) fprintf(out,"\n");
	        fprintf(out,"%d, ",(Int8)*u); /* U */
            fprintf(out,"%d, ",(Int8)*y); /* Y */
            y++;
	        fprintf(out,"%d, ",(Int8)*v); /* V */
            fprintf(out,"%d, ",(Int8)*y); /* Y */
            y++;
	        u++;
		    v++;
		}
	}
	
    fprintf(out,"};\n");
    fclose (in);
	fclose (YUV);
    fclose (out);

#ifdef WinNT
    free(yBuf_p);
    free(uBuf_p);
    free(vBuf_p);
    free(aBuf_p);
#else
    _cache_free(yBuf_p);
    _cache_free(uBuf_p);
    _cache_free(vBuf_p);
    _cache_free(aBuf_p);
#endif
} /* save_bmp */


main (int noarg, char *argv []) {
	char ReadName [256];
	char WriteName[256];
	Int32 i;

	for (i=0;i<256;i++) {
		ReadName[i] = '\0';
		WriteName[i] = '\0';
	}
	if (noarg < 3) {
		printf ("Usage: %s <input filename> <output filename base>\n",
			argv [0]);
		return (1);
	}

	strcpy(ReadName,argv[1]);
	strcpy(WriteName,argv[2]);
	bmpConvert(ReadName, WriteName);
	return (0);
}
