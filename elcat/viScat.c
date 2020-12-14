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
// Author: Brigitte Schuster
// Last update: 2004/02/08                                                  
// Modified:                                                   
// File: viScat.c
// History:
// 		Version 0.1	 2004/02/08 
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
	FILE	*fpWrite;
	char	WriteName[256];
	char	ExtName[256];
	int     i;
	int     nbytes, size;
	unsigned char buf[1024*1024];


	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		ReadName1[i] = '\0';
		ReadName2[i] = '\0';
		ExtName[i] = '\0';
	}

	if ( argc < 3 )
	{
		fprintf(stderr,"%s input_file_name_1 input_file_name_2\n",argv[0]);
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
	if (strcmp(ReadName1,ReadName2) != 0) {
		if ((fpRead2=fopen(ReadName2,"rb"))==NULL)
		{
			printf("error : cannot open %s\n",ReadName2);
			exit(-1);
		}
	}
    
	strncpy(WriteName,ReadName1,strlen(ReadName1)-4);
	strncpy(ExtName,&ReadName1[strlen(ReadName1)-4],4);

	strcat(WriteName,"_cat");
	strcat(WriteName,ExtName);

	if ((fpWrite=fopen(WriteName,"wb"))==NULL)
	{
		printf("error : cannot open %s\n",WriteName);
		exit(-1);
	}
	size = 0;
//	fstat(fpRead2);
	//copy first file
	while ( (feof(fpRead1) == 0) ) {
		nbytes = fread(&buf[0],1,1,fpRead1);
	    fwrite(&buf[0],1,nbytes,fpWrite);
		size += nbytes;
	}
	fprintf(stderr,"size %d\n",size);
	if (strcmp(ReadName1,ReadName2) != 0) {
		//copy second file
		while ( (feof(fpRead2) == 0) ) {
			nbytes = fread(&buf[0],1,1,fpRead2);
			fwrite(&buf[0],1,nbytes,fpWrite);
		}
		fclose(fpRead2);
	}
	else {
		fclose(fpRead1);
		fpRead1=fopen(ReadName1,"rb");
		size = 0;
		while ( (feof(fpRead1) == 0) ) {
			nbytes = fread(&buf[0],1,1,fpRead1);
			fwrite(&buf[0],1,nbytes,fpWrite);
			size += nbytes;
		}
		fprintf(stderr,"size %d\n",size);
	}

	fclose(fpWrite);
	fclose(fpRead1);


	return(0);
}
