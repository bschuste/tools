////////////////////////////////////////////////////////////////////////
//
// Copyright (c) viSystems 2002. 							             
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
// Author: Jerome Gribling
// Last update: 2002/07/15                                                  
// Modified:                                                   
// File: viScomp.c
// History:
// 		Version 0.1	 2002/07/15
//						
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
	FILE	*fpRead1;
	FILE    *fpRead2; 
	char	ReadName1[256];
	char    ReadName2[256];
	
	int n;
      int diff=0;
	int nbytes;
	unsigned char byte1, byte2;
	

	if ( argc < 2 )
	{
		printf("Usage: %s input_file_name \n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName1,argv[1]);
		strcpy(ReadName2,argv[2]);
	}

	if ((fpRead1=fopen(ReadName1,"rb"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName1);
		exit(-1);
	}
	
	if ((fpRead2=fopen(ReadName2,"rb"))==NULL)
	{
		printf("error :cannot open %s\n",ReadName2);
		exit(-1);
	}

	while ( (feof(fpRead1) == 0) && (feof(fpRead2) == 0) ) {
		nbytes = fread(&byte1,1,1,fpRead1);
		nbytes = fread(&byte2,1,1,fpRead2);
		if(byte1!=byte2) {
			printf("File %s differs from file %s\n",ReadName1,ReadName2);
			diff = 1;
		    printf("ftell(fpRead1)=0x%02lx ftell(fpRead2)=0x%02lx\n",ftell(fpRead1),ftell(fpRead2));
		    printf("ftell(fpRead1)=%ld ftell(fpRead2)=%ld\n",ftell(fpRead1),ftell(fpRead2));
			fseek(fpRead1,-4,SEEK_CUR);
			fseek(fpRead2,-4,SEEK_CUR);
			for(n=1;n<=8;n++) {
			nbytes = fread(&byte1,1,1,fpRead1);
			nbytes = fread(&byte2,1,1,fpRead2);		
				printf("byte1=0x%02x ",byte1);
				printf("byte2=0x%02x\n",byte2);
			}
//			exit(0);
		}
	}
	if ((feof(fpRead1) == 1) && (feof(fpRead2) == 1) && (diff == 0))  
		printf("File %s is IDENTICAL to file %s\n",ReadName1,ReadName2);
	else
		printf("File %s is DIFFERENT from file %s\n",ReadName1,ReadName2);
	return(0);
}
