#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elportab.h>

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

Int32 bmp_wd = 800, bmp_ht = 480; /* FIXTHIS */

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
    UInt8 A=0;
    UInt8 R, G, B, dummy;
    Int32 scanLineSize, wd, ht, i, j, r, c, rcopy, ccopy;
    Int32 l;
	double RR, GG, BB;
    FILE *in, *RGB;
    bmpFileHeader_t fileHd;
    bmpInfoHeader_t infoHd;
    UInt8 *gBuf_p, *bBuf_p, *rBuf_p, *aBuf_p;
    UInt8 *rp, *gp, *bp, *ap;

	printf ("inname %s outbase %s\n", inname, outbase);

    in = fopen (inname, "rb");
    if (in == NULL) {
        printf ("unable to open %s\n", inname);
        return;
    }

	sprintf (filename, "%s.argb", outbase);
	RGB = fopen (filename, "wb");
	if (RGB == NULL) {
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

	/*	we should only this much to RGB or have only this much available */	
	rcopy = infoHd.biHeight;
	ccopy = infoHd.biWidth;
	printf ("rcopy (height) %d ccopy (width) %d\n", rcopy, ccopy);

#ifdef __TCS__
	gBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,0) );
	bBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,1) );
	rBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,2) );
	aBuf_p = (UInt8 *)(_cache_malloc(rcopy*ccopy,3) );
#else
	gBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	bBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	rBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
	aBuf_p = (UInt8 *)(malloc(rcopy*ccopy) );
#endif

    gp = gBuf_p;
    bp = bBuf_p;
    rp = rBuf_p;
	for (r = 0; r < rcopy; r++) {
		printf ("\rr = %d", r);
        fseek (in, fileHd.bfOffBits + (long int) scanLineSize * (infoHd.biHeight - r - 1) * 3, SEEK_SET);
		for (c = 0; c < ccopy; c++) {
			fread (&B, 1, 1, in);
			fwrite(&B, 1, 1, RGB);
			fread (&G, 1, 1, in);
			fwrite(&G, 1, 1, RGB);
			fread (&R, 1, 1, in);
			fwrite(&R, 1, 1, RGB);
			fwrite(&A, 1, 1, RGB);
			*rp = R;
			*gp = G;
			*bp = B;
            rp++;
	        gp++;
	        bp++;
		}
	}
//    fwrite (gBuf_p, 1, rcopy*ccopy, RGB); /* G */
//    fwrite (bBuf_p, 1, rcopy*ccopy, RGB); /* B */
//    fwrite (rBuf_p, 1, rcopy*ccopy, RGB); /* R */
	
    fclose (in);
	fclose (RGB);

#ifdef __TCS__
    _cache_free(gBuf_p);
    _cache_free(bBuf_p);
    _cache_free(rBuf_p);
    _cache_free(aBuf_p);
#else
    free(gBuf_p);
    free(bBuf_p);
    free(rBuf_p);
    free(aBuf_p);
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
