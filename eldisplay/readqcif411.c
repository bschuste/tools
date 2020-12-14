
/*******************************************************************************
*
*	Filename:	readqcif411.c
*
*	Date:		04/10/95
*
*	Description:	Read a file.qcif
*
*******************************************************************************/

/*******************************************************************************
*	QCIF-format reading
*	QCIF FILE FORMAT  => Y[144][176]
*			     U[72][88]
*			     V[72][88]
*
*******************************************************************************/

#include <stdio.h>
#include "types.h"

int readqcif411(
		FILE 		*fpRead,
struct 	qcif411 	*qcif_int
)
{
	/*
	register int i,j;

	for(j=0 ; j<NLQCIF ; j++)
		for(i=0 ; i<NPQCIF ; i++)
          		qcif_int->y[j][i] = getc(fpRead);

	for(j=0 ; j<NLQCIF/2 ; j++)
		for(i=0 ; i<NPQCIF/2 ; i++)
          		qcif_int->u[j][2*i] = getc(fpRead); 

	for(j=0 ; j<NLQCIF/2 ; j++)
		for(i=0 ; i<NPQCIF/2 ; i++)
          		qcif_int->v[j][2*i+1] = getc(fpRead); 
	*/	

    	fread(qcif_int->y,sizeof(unsigned char),NLQCIF*NPQCIF,fpRead);
	fread(qcif_int->u,sizeof(unsigned char),NLQCIF*NPQCIF/4,fpRead);
	fread(qcif_int->v,sizeof(unsigned char),NLQCIF*NPQCIF/4,fpRead);
	
	return(0);
}

