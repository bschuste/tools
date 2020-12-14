
/*******************************************************************************
*
*	Filename:	types.h
*
*	Date:		04/10/95
*
*	Description:	structures and constants
*
*******************************************************************************/

#include <stdio.h>


/*//////////////////////////////////////////////////////////////////////
// Portability management
/*/
#define UInt8   unsigned char
#define UInt16  unsigned int
#define UInt32  unsigned long
#define Int8    char
#define Int16   int
#define Int32   long

#define	FALSE	0
#define	TRUE	1

/* Structure with image data */
typedef struct pict_image {
  unsigned char *lum;        // Luminance plane
  unsigned char *Cr;         // Cr plane
  unsigned char *Cb;         // Cb plane
} PictImage;

typedef struct Picture {
	UInt32	w0;		/* width of c0 component */
	UInt32	h0;		/* height of c0 component */
	UInt32	w1;		/* width of c1 component */
	UInt32	h1;		/* height of c1 component */
	UInt32	w2;		/* width of c2 component */
	UInt32	h2;		/* height of c2 component */
	UInt8	*c0;	/* component Y or R */ 
	UInt8	*c1;	/* component U or G */
	UInt8	*c2;	/* component V or B */
} Picture_t, *Picture_p;
