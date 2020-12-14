////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Electropix 2002. 							             
// All rights reserved. Reproduction in whole or in part is prohibited   
// without the written consent of the copyright holder.                  
//                                                                        
// This code and information is provided "as is" without any warranty    
// of any kind, either expressed or implied, including but not limited   
// to the implied warranties of merchantability and/or fitness for any   
// particular purpose.                                                   
// 
// This disclaimer of warranty extends to the user of these programs
// and user's customers, employees, agents, transferees, successors,
// and assigns.
//
// viSystems Research and Development does not represent or warrant that
// the programs furnished hereunder are free of infringement of any
// third-party patents.
//					
////////////////////////////////////////////////////////////////////////
//             
// Author: Brigitte Schuster
// Last update: 2002/06/29                                                  
// Modified:                                                   
// File: eldump.c
// History:
// 		Version 0.1	 2002/06/29
//						
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	FILE	*fpRead;
	char	ReadName[256];
	int i;
	int nbytes;
	unsigned char buf[8];

	if ( argc < 2 )
	{
		printf("%s input_file_name \n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName,argv[1]);
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName);
		exit(-1);
	}

	while (feof(fpRead) == 0) {
		nbytes = fread(&buf[0],1,8,fpRead);
		for (i=0;i<nbytes;i++)
			printf("%02x ",buf[i]);
		printf("\n");
	}
	return(0);
}
