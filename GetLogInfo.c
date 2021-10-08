/*********************************************************************************
 * File:      GetLogInfo.c
 * Author:    Tyler Matijevich
 * Created:   October 8, 2021/17:45 
 *********************************************************************************/ 

#include "UserLogMain.h"
 
/* Get logging history and error information */
signed long GetLogInfo(UserLogInfoType *logInfo) {
	memcpy(logInfo, &info, sizeof(info));
	return 0;
} /* End function */
