////////////////////////////////////////////////////////////////////////
//             
// Author: Brigitte Schuster
// Last update: 2024/04/01                                                  
// Modified:                                                   
// File: elmtisp.c
// History:
// 		Version 0.1	 2024/04/01
//						
////////////////////////////////////////////////////////////////////////
//
// See linux-mtk/Documentation/userspace-api/media/v4l/pixfmt-mtisp-sbggr10.rst

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define COLOR_BAND_SIZE_IN_PIXELS (1920/8)
#define COLOR_BAND_SIZE_IN_BYTES ((COLOR_BAND_SIZE_IN_PIXELS/4) * 5)
#define NUM_OF_FRAMES 10
#define NUM_OF_COLOR_BANDS 8
#define NUM_OF_BYTES_PER_SET 10 // B G B G  or G R G R
#define WIDTH_IN_PIXELS (1920)
#define WIDTH_IN_BYTES (1920/4*5)
#define HEIGHT_IN_PIXELS (1080)

int color_band(FILE *fpr, int line)
{
	int i, j, k;
	int nbytes;
	uint8_t r1, r2, g1, g2, b1, b2;
	unsigned char buf[COLOR_BAND_SIZE_IN_BYTES];

	fseek(fpr, 0L, SEEK_SET);
	printf("Color band size in bytes %d with two lines %d\n", line/8, COLOR_BAND_SIZE_IN_BYTES);

// White - Yellow -  Cyan  -  Green - Magenta - vRed  -  Blue -  Black
//ffffff - ffff00 - 00ffff - 00ff00 - ff00ff - ff0000 - 0000ff - 000000

	for (k=0; k<NUM_OF_COLOR_BANDS; k++) { // first and second lines
		if (k==0) printf("--- White ---\n");
		if (k==1) printf("--- Yellow ---\n");
		if (k==2) printf("--- Cyan ---\n");
		if (k==3) printf("--- Green ---\n");
		if (k==4) printf("--- Magenta ---\n");
		if (k==5) printf("--- Red ---\n");
		if (k==6) printf("--- Blue ---\n");
		if (k==7) printf("--- Black ---\n");

		for (j=0;j<COLOR_BAND_SIZE_IN_BYTES/5;j++) {
			fseek(fpr, k*COLOR_BAND_SIZE_IN_BYTES + j*5, SEEK_SET);
			nbytes = fread(&buf[0],1, 5, fpr);
			printf("%d: ", j);
			for (i=0;i<nbytes;i++) {
				printf("%02x ",buf[i]);
			}
			b1 = ((buf[1]&0x03) << 6) | (buf[0] >> 2);
			g1 = (buf[1] >> 4) | ((buf[2]&0x0f) << 4);
			b2 = ((buf[2]&0xc0) >> 6) | ((buf[3]&0x3f) << 2);
			g2 = (buf[4]);
			printf("  %d %d %d %d\n", b1, g1, b2, g2);
			fseek(fpr, k*COLOR_BAND_SIZE_IN_BYTES + j*5 + 2400, SEEK_SET);

			nbytes = fread(&buf[0],1, 5, fpr);
			printf("%d: ", j);
			for (i=0;i<nbytes;i++) {
				printf("%02x ",buf[i]);
			}
			g1 = ((buf[1]&0x03) << 6) | (buf[0] >> 2);
			r1 = (buf[1] >> 4) | ((buf[2]&0x0f) << 4);
			g2 = ((buf[2]&0xc0) >> 6) | ((buf[3]&0x3f) << 2);
			r2 = (buf[4]);
			printf("  %d %d %d %d\n", g1, r1, g2, r2);

		}

		printf("\n");
		printf("\n");
	}
	return(0);
}

void convert10bit_to_8(uint8_t *p, uint8_t *o) {
	o[0] = ((p[1]&0x03) << 6) | (p[0] >> 2);   //g1 or b1
	o[1] =  (p[1] >> 4) | ((p[2]&0x0f) << 4);   //r1 or g1
	o[2] = ((p[2]&0xc0) >> 6) | ((p[3]&0x3f) << 2); //g2 or b2
	o[3] =   p[4]; // r2 or g2
}

int convert_image(FILE *fpr, FILE *fpw) {
	int err = 0;
	uint8_t bayer10[WIDTH_IN_BYTES];
	uint8_t bayer8[WIDTH_IN_PIXELS];
	int i, w, h;
	int nbytes;
	uint8_t *p, *o;
	
	for (h=0; h<HEIGHT_IN_PIXELS; h++) {
		i = 0; // start of the line
		nbytes = fread(&bayer10[0], 1, WIDTH_IN_BYTES, fpr);
		if (nbytes != WIDTH_IN_BYTES) {
			printf("ERROR: not enough bytes in file, read %d bytes", nbytes);
		}
		for (w=0; w<nbytes; w+=5) {
			p = &bayer10[w];
			o = &bayer8[i];
			i += 4;
			convert10bit_to_8(p, o);
		}
		nbytes = fwrite(&bayer8[0], 1, WIDTH_IN_PIXELS, fpw);

		if (nbytes != WIDTH_IN_PIXELS) {
			printf("ERROR: writing bytes in file, wrote %d bytes", nbytes);
		}
	}

	return err;
}


int main(int argc, char **argv)
{
	FILE *fpRead, *fpWrite;
	char ReadName[256] = "\0";
	char WriteName[256] = "\0";
	int sz, line, number_of_images;
	int i;

	if ( argc < 2 )
	{
		printf("%s input_file_name \n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName,argv[1]);
	}
	strncpy(WriteName, argv[1], strlen(ReadName)-4);


	if ((fpRead=fopen(ReadName,"rb"))==NULL)
	{
		printf("error : cannot open %s\n", ReadName);
		exit(-1);
	}
	strcat(WriteName, "-bggr8.raw");

	if ((fpWrite=fopen(WriteName,"wb"))==NULL)
	{
		printf("error : cannot open %s\n", WriteName);
		exit(-1);
	}

	fseek(fpRead, 0L, SEEK_END);
	sz = ftell(fpRead);
	printf("File size in bytes %d\n", sz);
	fseek(fpRead, 0L, SEEK_SET);
	line = sz/(1080 * NUM_OF_FRAMES);
	printf("Line size in bytes %d\n", line);
	printf("Line size in pixels %d\n", (line * 4)/5);
	number_of_images = sz / (WIDTH_IN_BYTES * HEIGHT_IN_PIXELS);
	printf("number_of_images %d\n", number_of_images);
	for (i=0;i<number_of_images;i++) {
		convert_image(fpRead, fpWrite);
	}

	return (0);
}