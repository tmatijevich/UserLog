/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich
 * Created:   October 21, 2020/21:31 
 *********************************************************************************/ 

#ifndef __USER_LOG_MAIN_H__
#define __USER_LOG_MAIN_H__

// Include true false keywords
#include <stdbool.h>

// Defined in the ArEventLogWrite() function
#define OBJECT_ID_STR_LEN 36

typedef struct UserLogBufferEntryType {
	enum UserLogSeverityEnum Severity;
	UINT Code;
	char sMessage[USER_LOG_MESSAGE_LENGTH];
	char sTaskName[OBJECT_ID_STR_LEN];
} UserLogBufferEntryType;

#endif