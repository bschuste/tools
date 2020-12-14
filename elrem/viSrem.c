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
// File: viSoffset.c
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
	char	ReadName1[256];
	FILE	*fpWrite;
	char	WriteName[256];
	char	ExtName[256];
	int     i;
	int     nbytes, size, off;
	unsigned char buf[1024*1024];


	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		ReadName1[i] = '\0';
		ExtName[i] = '\0';
	}

	if ( argc < 3 )
	{
		fprintf(stderr,"%s number_of_offset_bytes input_file_name\n",argv[0]);
		exit(1);
	}
	else
	{
		off = atoi(argv[1]);
		strcpy(ReadName1,argv[2]);
	}

	if ((fpRead1=fopen(ReadName1,"rb"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName1);
		exit(-1);
	}
    
	strncpy(WriteName,ReadName1,strlen(ReadName1)-3);
	strcat(WriteName,".epd");

	if ((fpWrite=fopen(WriteName,"wb"))==NULL)
	{
		printf("error : cannot open %s\n",WriteName);
		exit(-1);
	}

	// write offset bytes
	for (i=0;i<off;i++) {
		buf[i] = 0;
	}
	fwrite(&buf[0],1,off,fpWrite);

	//copy first file
	size = 0;
	while ( (feof(fpRead1) == 0) ) {
		nbytes = fread(&buf[0],1,1,fpRead1);
	    fwrite(&buf[0],1,nbytes,fpWrite);
		size += nbytes;
	}
	fprintf(stderr,"size %d\n",size);
	fclose(fpWrite);
	fclose(fpRead1);

	return(0);
}
