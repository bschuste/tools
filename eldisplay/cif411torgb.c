
/*******************************************************************************
*
*	Filename:	cif411torgb.c
*
*	Date:		04/10/95
*
*	Description:	Conversion rgb format
*
*******************************************************************************/


#include <stdio.h>
#include "types.h"


int cif411torgb(
struct 	cif411 	*cif_int,
struct	rgb		*rgb_int
)
{
  register  int		i,j;
  register	double	R,G,B;
			double	CB,CR;


	for(j=0 ; j<NLCIF ; j++)
		for(i=0 ; i<NPCIF ; i++)
		{
			CB = (double)(0.25*cif_int->u[j/2][i/2]   + 0.25*cif_int->u[j/2][i/2+1]
				        + 0.25*cif_int->u[j/2+1][i/2] + 0.25*cif_int->u[j/2+1][i/2+1]);
			CR = (double)(0.25*cif_int->v[j/2][i/2]   + 0.25*cif_int->v[j/2][i/2+1]
				        + 0.25*cif_int->v[j/2+1][i/2] + 0.25*cif_int->v[j/2+1][i/2+1]);

          	R = ((double)cif_int->y[j][i] + 1.371*(CR-128.0) + 0.000*(CB-128.0));
			R = R>255.0?255:R<0.0?0:R;
			rgb_int->r[j][i] = (unsigned char)R;

         	G = ((double)cif_int->y[j][i] - 0.698*(CR-128.0) - 0.336*(CB-128.0));
 			G = G>255.0?255:G<0.0?0:G;
 			rgb_int->g[j][i] = (unsigned char)G;

			B = ((double)cif_int->y[j][i] + 0.000*(CR-128.0) + 1.732*(CB-128.0));
 			B = B>255.0?255:B<0.0?0:B;
			rgb_int->b[j][i] = (unsigned char)B;

		}


	
	
	return(0);

}
