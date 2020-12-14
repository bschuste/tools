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

int main(int argc, char **argv)
{
	FILE	*fpRead;
	char	ReadName[256];
	FILE	*fpWrite;
	char	WriteName[256];
	char	ExtName[256];
	int     i;
	int     nbytes;
    int     NumberOfPackets;
	unsigned char buf[1024];
    int     NumberOfMB = 3;

//	size = 352*288*1.5;
	size = 1024;
	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		ReadName[i] = '\0';
		ExtName[i] = '\0';
	}

	if ( argc < 2 )
	{
//		fprintf(stderr,"%s input_file_name [-s packet_size] number_of_packets\n",argv[0]);
		fprintf(stderr,"%s input_file_name [number_of_MB]\n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(ReadName,argv[1]);
//		options(&argc,&argv);
//		size = atoi(argv[2]);
//		NumberOfPackets = atoi(argv[2]);
	}

	if ( argc == 3 )
	   NumberOfMB = atoi(argv[2]);

	if ((fpRead=fopen(ReadName,"rb"))==NULL)
	{
		printf("error : cannot open %s\n",ReadName);
		exit(-1);
	}

    
	strncpy(WriteName,ReadName,strlen(ReadName)-4);
	strncpy(ExtName,&ReadName[strlen(ReadName)-4],4);

	strcat(WriteName,"_cut");
	strcat(WriteName,ExtName);

	if ((fpWrite=fopen(WriteName,"wb"))==NULL)
	{
		printf("error : cannot open %s\n",WriteName);
		exit(-1);
	}
	printf("size %d bytes\n",size*(1024*NumberOfMB));
    for (i=0;i<(1024*NumberOfMB)-1;i++) 
    {
    	nbytes = fread(&buf[0],1,size*sizeof(unsigned char),fpRead);
    	//printf("nbytes %d\n",nbytes);
    	if (nbytes>0) fwrite(&buf[0],1,size,fpWrite);
    	if (nbytes==0) break;
	}
	fclose(fpWrite);
	fclose(fpRead);

	return(0);
}
