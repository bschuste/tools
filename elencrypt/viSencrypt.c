/***********************************************************************
 **
   Copyright (c) Tigress SARL 2003. 							             
   All rights reserved. Reproduction in whole or in part is prohibited   
   without the written consent of the copyright holder.                  
 
   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 **						
  ***********************************************************************/	
 /*!
 ***********************************************************************
 **
   \file 
       pvrEsn.c
   \brief
       Serial Number processing function
   \date
       Last update: 2004/01/31
   \author
       Brigitte Schuster
   \todo                                               
 **						
 ***********************************************************************/	
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>
#include "viSportab.h"
#include "viSdefs.h"
#include "viSerror.h"

#define PROUT stderr
//#define PROUT fpWrite


#define DB_L00(a)  if (dbgLevel >  0) fprintf(PROUT,(a));
#define DB_L0(a,b) if (dbgLevel >  0) fprintf(PROUT,(a),(b));
#define DB_L1(a,b) if (dbgLevel >  1) fprintf(PROUT,(a),(b));
#define DB_L10(a)  if (dbgLevel >  1) fprintf(PROUT,(a));
#define DB_L2(a,b) if (dbgLevel >  2) fprintf(PROUT,(a),(b));
#define DB_L20(a)  if (dbgLevel >  2) fprintf(PROUT,(a));
#define DB_L3(a,b) if (dbgLevel >  3) fprintf(PROUT,(a),(b));
#define DB_L30(a)  if (dbgLevel >  3) fprintf(PROUT,(a));
#define DB_L4(a,b) if (dbgLevel >  4) fprintf(PROUT,(a),(b));
#define DB_L40(a)  if (dbgLevel >  4) fprintf(PROUT,(a));

/***********************************************************************
 * Module Global Variables
 */

Int8  dbgLevel;
FILE *fpWrite;

#define ROOT_ESN "/HARDISK/BIN/ESN.TXT"
#define PVR_ENCRYPT_KEY 0x00FFAA55

/***********************************************************************
 * Functions
 */

//! Read the encrypted serial number from the eeprom and provide the 4-bytes value
viSerror pvrEsnDecrypt(UInt32 edata, UInt32 *odata) {

	viSerror err = VIS_OK;

	*odata = edata ^ PVR_ENCRYPT_KEY;
	return err;

}
//! Encrypt the given 4-bytes value with the key and store the result in the eeprom
viSerror pvrEsnEncrypt(UInt32 odata, UInt32 *edata) {

	viSerror err = VIS_OK;

	*edata = odata ^ PVR_ENCRYPT_KEY;
	return err;
}

viSerror pvrEsnUpdateEeprom()
{
	viSerror err = VIS_OK;
	FILE *fpRead;

	/* check if files exist */
    fpRead = fopen(ROOT_ESN, "r");
    if (!fpRead) {
		printf("file %s does not exist\n",ROOT_ESN);
        return VIS_ERR_OPEN_FILE;
    }
    else {
		/* check if eeprom has a serial number */
		/* if yes, then check if file has the magic number */
		/* if no, check if file has the other magic number */

		/* encrypt the Serial Number */
		
		/* write then encrypted value into the eeprom */
	}
	
	return err;
}



int main(int argc, char **argv)
{
	UInt32 i;
	UInt32 decrypted;
	UInt32 encrypted;
	UInt32 errNum = 0;

    dbgLevel = 1;
    i = 0xFFFF;
	do {
		pvrEsnEncrypt(i, &encrypted);
		pvrEsnEncrypt(encrypted, &decrypted);
		if (i != decrypted) errNum++;
		i--;
	}
	while (i > 0);
	i = 0;
	pvrEsnEncrypt(i, &encrypted);
	pvrEsnDecrypt(encrypted, &decrypted);
	if (i != decrypted) errNum++;

	printf("errNum = %ld\n",errNum);

	return(0);
}
