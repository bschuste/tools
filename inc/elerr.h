/***********************************************************************
 **
   Copyright (c) Electropix 2009.                                        
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
   Last update: 2011/01/27
   Modified:
   File: elerr.h
   History:
        Version 0.1  2009/10/08
 **
 ***********************************************************************/
#ifndef EL_ERR_H
#define EL_ERR_H


typedef enum elErrorCode 
{
    EL_OK                           = 0,
    EL_ERR_MALLOC,
    EL_ERR_BAD_PARAMETER,
    EL_ERR_PTHREAD,
    EL_ERR_SIP_CLIENT_SOCKET,
    EL_ERR_COMP_UNIQUE_START,
    
    EL_ERR_MAX
} elErrorCode_t;



#endif
