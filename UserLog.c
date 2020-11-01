/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich 
 * Created:   October 21, 2020/21:03 
 *********************************************************************************/ 

// <file> - System header files
// "file" - Program header files
#include <UserLog.h>
#include "UserLogMain.h"

static struct UserLogBufferEntryType Buffer[USER_LOG_BUFFER_SIZE];

//// Declare static global structures to ensure allocated memory
//static struct UserLogFifoBufferInfoType BufferInfo;
//
//// Retrieve the log buffer information stored in this library's global variables
//UDINT GetLogBufferInfo(struct UserLogFifoBufferInfoType* pLogBufferInfo) {
//	// Access the log buffer information with the VAR_IN_OUT pointer
//	*pLogBufferInfo = BufferInfo;
//	
//	// No necessary return value, return zero
//	return 0;
//}
