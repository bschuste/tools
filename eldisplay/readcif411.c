
/*******************************************************************************
*
*	Filename:	readcif411.c
*
*	Date:		03/12/98
*
*	Description:	Read a file.cif
*
*******************************************************************************/

/*******************************************************************************
*	CIF-format reading
*	CIF FILE FORMAT  => Y[288][352]
*						U[144][176]
*						V[144][176]
*
*******************************************************************************/

#include <stdio.h>
#include "types.h"




int readcif411(
		FILE 	*fpRead,
struct 	cif411 	*cif_int
)
{
	/*
	register int i,j;
	for(j=0 ; j<NLCIF ; j++)
		for(i=0 ; i<NPCIF ; i++)
		{
          		cif_int->y[j][i] = getc(fpRead);
		}

	for(j=0 ; j<NLCIF/2 ; j++)
		for(i=0 ; i<NPCIF/2 ; i++)
		{
          		cif_int->u[j][i] = getc(fpRead); 
		}

	for(j=0 ; j<NLCIF/2 ; j++)
		for(i=0 ; i<NPCIF/2 ; i++)
		{
          		cif_int->v[j][i] = getc(fpRead); 
		}
	*/
    	fread(cif_int->y,sizeof(unsigned char),NLCIF*NPCIF,fpRead);
//	fread(cif_int->u,sizeof(unsigned char),NLCIF*NPCIF/4,fpRead);
//	fread(cif_int->v,sizeof(unsigned char),NLCIF*NPCIF/4,fpRead);
	
	return(0);

}

