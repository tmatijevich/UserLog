/*******************************************************************************
 * File: Main.h
 * Created: 2022-08-30
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file Main.h is part of the UserLog project 
 *   released under the MIT license agreement.
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
#define USERLOG_LOGBOOK_LENGTH MIN(10U, USERLOG_FORMAT_LENGTH)
#define USERLOG_MESSAGE_PREVIEW_LENGTH MIN(20U, USERLOG_FORMAT_LENGTH)
#define USERLOG_CODED_DATA_LENGTH (40U + USERLOG_MESSAGE_LENGTH + 2U)
#define USERLOG_ERROR_FACILITY 330U
#define USERLOG_CODE_IDENT 10000U
#define USERLOG_CODE_WRITE 10010U
#define USERLOG_CODE_CREATE 10020U
#define USERLOG_USER_LOGBOOK "$arlogusr"

/* Prototypes */
void StringCopy(char *Destination, uint32_t Size, char *Source);
void StringFormat(char *Sestination, uint32_t Size, char *Source, UserLogFormatType *Values);

/* Global variables */
extern UserLogSeverityEnum SeverityLevel;

#ifdef __cplusplus
};
#endif

#endif
