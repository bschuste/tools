////////////////////////////////////////////////////////////////////////
// Copyright (c) Medialive 2002. 							             
// All rights reserved. Reproduction in whole or in part is prohibited   
// without the written consent of the copyright holder.                  
//                                                                       
// This software has been developed by Tigress under contract    
//  n° 2002-07-15.                     			
// This code and information is provided "as is" without any warranty    
// of any kind, either expressed or implied, including but not limited   
// to the implied warranties of merchantability and/or fitness for any   
// particular purpose.                                                   
//                                                                       
// Author: Brigitte Schuster                                 
// Creation: 2002/09/28                                                  
// File: tsmanager.h
// History:
// 		Version 1.0                                                       
////////////////////////////////////////////////////////////////////////

#ifndef TSMANAGER_H
#define TSMANAGER_H

#include "defines.h"
		   
extern mediaError tsListOpen(session_p fm_p);
extern mediaError tsRead(fileManager_p buf_p);
extern mediaError tsWrite(fileManager_p buf_p, UInt8 * buf, Int32 nbytes, FILE *fiP, UInt8 flag);
extern mediaError getPacket(fileManager_p buf_p, UInt8 flag);
extern mediaError getStreamPosition(fileManager_p buf_p, streamPosition_p pos_p);
extern mediaError setStreamPosition(fileManager_p buf_p, streamPosition_p pos_p);
extern mediaError tsInitInputBuffer(fileManager_p buf_p);
extern mediaError tsResetInputBuffer(fileManager_p buf_p);
#endif
