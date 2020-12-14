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
// Creation: 2002/05/09                                                  
// File: externs.h
// History:
// 		Version 1.0                                                       
////////////////////////////////////////////////////////////////////////

#ifndef EXTERNS_H
#define EXTERNS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "defines.h"
#include "error.h"
		   
extern mediaError openFiles(session_p fm_p);
extern mediaError closeFiles(session_p fm_p);
extern void       printUserParameters(session_p fm_p);
//extern mediaError printSequenceParameters(session_p fm_p);
extern mediaError printSessionParameters(session_p fm_p);
extern mediaError getSequenceParameters(session_p fm_p, seqHeader_p sh_p);
extern mediaError getNextIntraPicturetoProcess(session_p fm_p);
extern mediaError openFilesDesc(session_p fm_p);
extern mediaError writeDataHeader(session_p fm_p, dataFileHeader_p dafh_p); 
extern void       printDataHeader(session_p fm_p);
extern mediaError getDataHeader(session_p fm_p);
extern mediaError readDataHeader(session_p fm_p);
extern int        dataSendMessage(session_p fm_p, messageType msgt, UInt8 *sndbuf_p, Int16 socket);
extern int        clientDataConnect(session_p fm_p);
extern int        clientMediaConnect(session_p fm_p);
extern mediaError descIhm(session_p fm_p);
extern mediaError mediaDescrambler(session_p fm_p);
extern mediaError startList(userManager_p uiman_p);
extern mediaError substituteSlice(session_p fm_p);
extern mediaError getNextIntraSliceData(session_p fm_p, UInt8 tmByte);
extern mediaError permutePicture(session_p fm_p);
extern mediaError getPicManagerIntraNb(picManager_p pic_p, dataFileHeader_p dafh_p);
extern mediaError findNextStartCode(session_p fm_p, fileManager_p buf_p, UInt8 *tmByte_p, UInt32 *nbBytes);
extern mediaError getByte(fileManager_p buf_p, UInt8 *tmp);
extern mediaError getByteMem(fileManager_p buf_p, UInt8 *tmp);
extern mediaError putByte(fileManager_p buf_p, UInt8 aByte);
extern mediaError putByteMem(fileManager_p buf_p, UInt8 aByte);
extern mediaError getPictureType(session_p fm_p, picType *pictureType);
extern mediaError flushBuffers(session_p fm_p);
extern mediaError getVlcCode(UInt32 *number, session_p fm_p, UInt8 readBits);
extern mediaError putVlcCode(fileManager_p buf_p, UInt32 vlcCode, UInt16 vlcLength);
extern mediaError getExtensions(session_p fm_p);
extern mediaError storePicture(session_p fm_p);
extern mediaError setBufferPointer(fileManager_p buf_p, UInt16 tmpBc);
extern mediaError testEndProcess(session_p fm_p, mediaError err);
extern void       ExceptFunction();
extern mediaError tsRead(fileManager_p buf_p);
extern mediaError tsWrite(fileManager_p buf_p, UInt8 * buf, Int32 nbytes, FILE *fiP, UInt8 flag);
extern mediaError getPacket(fileManager_p buf_p, UInt8 flag);
extern mediaError tsListOpen(session_p fm_p);
#endif
