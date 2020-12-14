/***********************************************************************
 **
   Copyright (c) Tigress 2004. 							             
   All rights reserved. Reproduction in whole or in part is prohibited   
   without the written consent of the copyright holder.                  
                                                                                    						             
   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 
   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Tigress Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **						
 ***********************************************************************					
 **
   Author: Brigitte Schuster
   Last update: 2004/06/23                                                  
   Modified:                                                   
   File: visound.c
   History:
 		Version 0.1	 2004/06/23
 **						
 ***********************************************************************/					

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "viSportab.h"
#include "viSdefs.h"
#include "viSerror.h"

/***********************************************************************
 ** Debug level printout
 */

#define PROUT fpWrite

/***********************************************************************
 ** Global variables
 */
#define PACKET_SIZE     32768

int main(int argc, char **argv)
{
	FILE	*fpRead;
   FILE	*fpWrite;
	char	ReadName[256];
	char	WriteName[256];
	char	TabName[256];
	int     i;
	int     nbytes;
   viSerror err = VIS_OK;
   UInt8 *buffer;
   int fileSize;

	if ( argc < 2 ) {
		printf("%s input_file_name\n",argv[0]);
		exit(1);
	}
	else {
		strcpy(ReadName,argv[1]);
	}

	if ((fpRead=fopen(ReadName,"rb"))==NULL) {
		printf("error : cannot open %s\n",ReadName);
		return(VIS_ERR_OPEN_FILE);
	}
	
	for (i=0;i<256;i++) {
		WriteName[i] = '\0';
		TabName[i]   = '\0';
	}

	strncpy(WriteName,ReadName,strlen(ReadName)-4);
	strcpy(TabName,WriteName);
	strcat(WriteName,".h");
	strcat(TabName,"Tab");
	
	if ((fpWrite=fopen(WriteName,"w"))==NULL) {
		printf("error : cannot open %s\n",WriteName);
		return(VIS_ERR_OPEN_FILE);
	}
      
	/* allocate memory for the structures */
  	if ((buffer =  malloc(sizeof(UInt8) * PACKET_SIZE)) == NULL) return(VIS_ERR_MEM_ALLOC);
   memset(buffer, (sizeof(UInt8) * PACKET_SIZE), 0);
	
	fileSize = 0;
	do { 
       // Read a new buffer from the file
	    nbytes = fread(buffer, 1, PACKET_SIZE*sizeof(UInt8), fpRead);
       fileSize += nbytes;
	} while ((feof(fpRead) == 0));

  fseek (fpRead, 0, SEEK_SET);    
  
  fprintf(fpWrite,"#define %s_WIDTH    %d\n", TabName, fileSize);
  fprintf(fpWrite,"UInt8 %s[%s_WIDTH] = \n", TabName, TabName);
  fprintf(fpWrite,"{\n");

	do { 
       // Read a new buffer from the file
	    nbytes = fread(buffer, 1, PACKET_SIZE*sizeof(UInt8), fpRead);
	    for (i=0; i<nbytes; i++) 
       {
          fprintf(fpWrite," 0x%02X,", buffer[i]);
          if (i%16 == 15) fprintf(fpWrite,"\n");
       }
       if (nbytes != PACKET_SIZE*sizeof(UInt8))
       {
            if (nbytes == 0) {
                // we reached the end of the file
   			    printf(" No error, End of File\n");
		    }
		    else {
  			    printf(" End of File detected, nbytes = %d\n",nbytes);
		    }
	    }
	} while ((feof(fpRead) == 0));
    
   fprintf(fpWrite,"\n};\n");
	fclose(fpRead);
	fclose(fpWrite);
   free(buffer);

	exit(0);
}
