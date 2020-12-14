/***********************************************************************
 **
   Copyright (c) Tigress 2002.
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
   Last update: 2002/06/29
   Modified:
   File: viSparse.c
   History:
 		Version 0.1	 2002/06/29
 **
 ***********************************************************************/

#ifndef VISERROR_H
#define VISERROR_H


#define VIS_ERR_BASE  	0x00010000
#define VIS_WRN_BASE  	0x00020000
#define VIS_INF_BASE  	0x00040000
#define VIS_ALL  		0x00070000

#define VIS_OUT         stderr

#define VIS_ERR(a) \
    (a); \
	if ( err == VIS_EOF ) { \
		fprintf(VIS_OUT," End of file %s line %d\n",__FILE__,__LINE__); \
		return(err); \
	} \
	if ( err & VIS_ERR_BASE ) { \
		fprintf(VIS_OUT," Program exited with code 0x%x, file %s, line %d\n",err,__FILE__,__LINE__); \
		exit(err); \
	} \
	if ( err & VIS_WRN_BASE ) { \
	}

enum viSerrorList {
	VIS_OK 					    = 0x00,					// everything is OK
	VIS_ERR_RESERVED 		    = VIS_ERR_BASE | 0x01,  // reserved for important things
	VIS_ERR_MEM_ALLOC 		    = VIS_ERR_BASE | 0x02,  // not enough memory calling malloc
	VIS_ERR_MEM_SIZE 			= VIS_ERR_BASE | 0x03,  // memory allocation results in size > BUFFER_SIZE bytes
	VIS_ERR_FILE_NOT_OPEN 	    = VIS_ERR_BASE | 0x04,  // attempt to read/write while the file is not open
	VIS_ERR_OPEN_FILE 		    = VIS_ERR_BASE | 0x05,  // error opening file, might not exist
	VIS_ERR_WRONG_PARAMETER 	= VIS_ERR_BASE | 0x06,  // user parameter is not recognized
	VIS_ERR_PARAMETER_MISSING   = VIS_ERR_BASE | 0x07,  // some user parameter is missing and should be defined
	VIS_ERR_TS                  = VIS_ERR_BASE | 0x08,  // error in TS parser
	VIS_PARAMETER_NOT_FOUND	    = VIS_WRN_BASE | 0x01,  // user parameter not defined
	VIS_PARAMETER_FOUND		    = VIS_WRN_BASE | 0x02,  // user parameter defined
	VIS_EOF					    = VIS_WRN_BASE | 0x04,  // end of file indicator
	VIS_VLC_OVERFLOW 		    = VIS_WRN_BASE | 0x08,  // overflow detected
	VIS_ERR_NOT_TS_HEADER       = VIS_WRN_BASE | 0x09,  // not a TS header
};

typedef enum viSerrorList viSerror;
typedef enum viSerrorList viSerror_t;

extern void generateError(viSerror err, int eofErr);

/* error options */
#define X(x)	#x, sizeof(#x)-1, x

struct {
	char	*text;
	int		len;
	int		value;
} options [] =
{
	{ X(VIS_OK) },
	{ X(VIS_ERR_MEM_ALLOC) },
	{ X(VIS_EOF) },
	{ NULL, 0 }
};
#endif

