/***********************************************************************
 **
   Copyright (c) Electropix 2010.                                        
   All rights reserved. Reproduction in whole or in part is prohibited   
   without the written consent of the copyright holder.                  
                                                                         
   This code and information is provided "as is" without any warranty    
   of any kind, either expressed or implied, including but not limited   
   to the implied warranties of merchantability and/or fitness for any   
   particular purpose.                                                   
 
   This disclaimer of warranty extends to the user of these programs
   and user's customers, employees, agents, transferees, successors,
   and assigns.

   Electropix Research and Development does not represent or warrant that
   the programs furnished hereunder are free of infringement of any
   third-party patents.
 **
 ***********************************************************************
 **
   Author: Brigitte Schuster
   Last update: 2002/06/29
   Modified:
   File: elvideo.h
   History:
        Version 0.1  2002/06/29
        
        Version 0.1  2010/12/19
            Added type to Picture structure
 **
 ***********************************************************************/
#ifndef EL_AUDIO_H
#define EL_AUDIO_H

/*-----------------------------------------------------------------------------
 *  Audio Buffers
 *-----------------------------------------------------------------------------
 */
typedef struct _audioBuf {    /* audio buffer structure */
    Pointer     Buf1;         /* buffer pointer for Buf1 */
    Pointer     Buf2;         /* buffer pointer for Buf2 */
    Int32       flag;
} aBuf;

#endif /* EL_AUDIO_H */
