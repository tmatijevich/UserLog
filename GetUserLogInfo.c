/*********************************************************************************
 * File:      GetUserLogInfo.c
 * Author:    Tyler Matijevich
 * Created:   2020-11-01
 *********************************************************************************/ 

#include "UserLogMain.h"
 
/* Gather information on this library's logging history */
long GetUserLogInfo(UserLogInfoType *logInfo) {
	
	if(logInfo == NULL)
		return -1;
	
	memcpy(logInfo, &info, sizeof(info));
	return 0;
	
} /* End function */
