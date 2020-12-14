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
// File: viScut.c
// History:
// 		Version 0.1	 2002/06/29
// 		Version 0.2	 2004/02/08 add options parsing
//						
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int size;

static int getval(argv)
char *argv[];
{
  int val;

  if (sscanf(argv[1]+2,"%d",&val)!=1)
    return 0;

  while (isdigit(argv[1][2]))
    argv[1]++;

  return val;
}
/* option processing */
static void options(int *argcp,char **argv[])
{
  while (*argcp>1 && (*argv)[1][0]=='-')
  {
    while ((*argv)[1][1])
    {
      switch (toupper((*argv)[1][1]))
      {
      case 'S':
        size = getval(*argv);
		fprintf(stderr,"options: size %d\n",size);
        break;
/*      case 'T':
        trace = 1; 
        break;
      default: */
        fprintf(stderr,"undefined option -%c ignored\n",(*argv)[1][1]);
      }

      (*argv)[1]++;
    }
    (*argv)++;
    (*argcp)--;
  }
}

#if 0
static void asc2bin(Char *pString, Int num) {
    /* pString should at least be 3 caracters wide */
	pString[0] = '0'; pString[1] = '0';
	
	pString[1] = 48 + num%10; /*ascii conversion */
	if (num>9) {
	    pString[0] = 48 + (num%100 - (pString[1]-48))/10;
	}
	if (num>99) {
    	pString[0] = '0'; pString[1] = '0';
	}
}
#endif


int main(int argc, char **argv)
{
	FILE	*fpRead;
	char	ReadName[256];
	FILE	*fpWrite;
	char	WriteName[256];
	char	ExtName[256];
	int     i;
	int     nbytes;
	         char bufin[1024];
	unsigned char bufou[1024];
    char asc;
    unsigned char bin;

	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		ReadName[i] = '\0';
    	ExtName[i] = '\0';
	}
    bufin[1] = '\0';
    strcpy(ExtName,".bin");

	if ( argc < 2 )
	{
//		fprintf(stderr,"%s input_file_name [-s packet_size] number_of_packets\n",argv[0]);
		fprintf(stderr,"%s input_file_name.asc\n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName,argv[1]);
//		options(&argc,&argv);
//		size = atoi(argv[2]);
//		NumberOfPackets = atoi(argv[2]);
	}


	if ((fpRead=fopen(ReadName,"r"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName);
		exit(-1);
	}

    
	strncpy(WriteName,ReadName,strlen(ReadName)-4);
//	strncpy(ExtName,&ReadName[strlen(ReadName)-4],4);
	strcat(WriteName,ExtName);

	if ((fpWrite=fopen(WriteName,"wb"))==NULL)
	{
		printf("error : cannot open %s\n",WriteName);
		exit(-1);
	}
    nbytes = 0;
    do {
    	nbytes += fread(&bufin[0],1,sizeof(char),fpRead);
//        bufou[0] = atoi(&bufin[0]);
        bufou[0] = (unsigned char)bufin[0];
//        printf("ou = %x, in = %c\n",bufou[0],bufin[0]);
    	fwrite(&bufou[0],1,sizeof(unsigned char),fpWrite);
    } while (feof(fpRead) == 0);
	printf("file size %d\n",nbytes);
//    fseek(fpRead,0,SEEK_SET);

	fclose(fpWrite);
	fclose(fpRead);

	return(0);
}
