/***********************************************************************
 **
   Author: Brigitte Schuster
   Last update: 2010/02/15
   Modified:                                                   
   File: elstn.c
   History:
   		Version 0.1                                          2010/02/15
 **
 ***********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned int generic[16]= 
{
    0x00000,
    0x10100,
    0x10410,
    0x11110,
    0x12244,
    0x12492,
    0x1292A,
    0x0AAAA,
    0x15555,
    0x0D6D5,
    0x0DD6D,
    0x0DDBB,
    0x0EEEF,
    0x0FBEF,
    0x0FEFF,
    0x1FFFF
};


int main(int argc, char **argv)
{
	FILE *fpWrite;
	char  WriteName[256];
	int   i,j;

	if ( argc < 2 )
	{
		printf("%s output_file_name \n",argv[0]);
		exit(1);
	}
	else
	{
		strcpy(WriteName,argv[1]);
	}

	if ((fpWrite=fopen(WriteName,"w"))==NULL)
	{
		printf("error : cannot open %s\n",WriteName);
		exit(-1);
	}
#define INDEX 16
    //TABLE 0
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=16;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");
	
    //TABLE 1   >>1
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=15;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"%d",(generic[i]>>16)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 2   >>2
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=14;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=15;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 3   >>3
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=13;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=14;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 4   >>4
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=12;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=13;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 5   >>5
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=11;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=12;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 6   >>6
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=10;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=11;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 7   >>7
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=9;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=10;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 8   >>8
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=8;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=9;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 9   >>9
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=7;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=8;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 10   >>10
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=6;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=7;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 11   >>11
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=5;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=6;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 12  >>12
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=4;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=5;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 13   >>13
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=3;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=4;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 14   >>14
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=2;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=3;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 15   >>15
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=1;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=2;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

    //TABLE 16   >>16
	fprintf(fpWrite,"      case Gray0(5 downto 2) is\n");
	for (i=0;i<INDEX;i++) 
    {
        fprintf(fpWrite,"        when \"%d%d%d%d\" => iD0 <= \"",(i>>3)&1,(i>>2)&1,(i>>1)&1,(i)&1);
        for (j=0;j>=0;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        for (j=16;j>=1;j--)
            fprintf(fpWrite,"%d",(generic[i]>>j)&1);
        fprintf(fpWrite,"\"; -- %d\n", i);
    }
	fprintf(fpWrite,"      end case;\n");
	fprintf(fpWrite,"\n\n");

	fclose(fpWrite);
	return(0);
}
