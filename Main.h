/*******************************************************************************
 * File: UserLog\main.h
 * Author: Tyler Matijevich
 * Created: 2022-08-30
 ******************************************************************************/

#ifndef USERLOG_MAIN_H
#define USERLOG_MAIN_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Headers */
#include <UserLog.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Macros */
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

/* Constants */
#define USERLOG_MESSAGE_LENGTH 120U
#define USERLOG_LOGBOOK_LENGTH MIN(10U, USERLOG_FORMAT_LENGTH)
#define USERLOG_MESSAGE_PREVIEW_LENGTH MIN(20U, USERLOG_FORMAT_LENGTH)
#define USERLOG_ERROR_FACILITY 3U
#define USERLOG_CODE_IDENT 10000U
#define USERLOG_CODE_WRITE 10010U
#define USERLOG_CODE_CREATE 10020U
#define USERLOG_USER_LOGBOOK "$arlogusr"

/* Prototypes */
void string_copy (char *destination, uint32_t size, char *source);
void string_format (char *destination, uint32_t size, char *source, UserLogFormatType *args);

/* Global variables */
extern UserLogSeverityEnum severity_level;

#ifdef __cplusplus
};
#endif

#endif
