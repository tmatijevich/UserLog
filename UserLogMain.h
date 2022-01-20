/*********************************************************************************
 * File:      UserLogMain.h
 * Author:    Tyler Matijevich
 * Created:   October 21, 2020/21:31 
 *********************************************************************************/ 

#ifndef USER_LOG_MAIN_H
#define USER_LOG_MAIN_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include <UserLog.h>
#include <string.h>
#include <stdbool.h>

/* Reference with a promise these variables will be declared in a source file */
extern UserLogInfoType info;
extern UserLogSeverityEnum verbosityLevel;
extern unsigned char severityMap[];

#ifdef __cplusplus
};
#endif

#endif /* USER_LOG_MAIN_H */
