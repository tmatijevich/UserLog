/*********************************************************************************
 * File:      GetBufferInfo.c
 * Author:    Tyler Matijevich 
 * Created:   November 1, 2020/18:03 
 *********************************************************************************/ 

#include <UserLog.h> // Include the automatically generated header file from the library
#include "UserLogMain.h" // Include local header file for internal declarations

extern UserLogBufferInfoType Info;

/*  */
DINT GetBufferInfo(struct UserLogBufferInfoType* LogBufferInfo) {
	brsmemcpy((UDINT)LogBufferInfo, (UDINT)&Info, sizeof(Info));
	return 0;
}
