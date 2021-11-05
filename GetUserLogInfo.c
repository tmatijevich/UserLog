/*********************************************************************************
 * File:      GetUserLogInfo.c
 * Author:    Tyler Matijevich
 * Created:   October 8, 2021/17:45 
 *********************************************************************************/ 

#include "UserLogMain.h"
 
/* Gather information on this library's logging history */
signed long GetUserLogInfo(UserLogInfoType *logInfo) {
	
	memcpy(logInfo, &info, sizeof(info));
	return 0;
	
} /* End function */
