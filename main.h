/*******************************************************************************
 * File: main.h
 * Created: 2022-08-30
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file main.h is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#ifndef USERLOG_MAIN_H
#define USERLOG_MAIN_H

#ifdef __cplusplus
extern "C" 
{
#endif

/* Headers */
#ifndef USERLOG_HEADER_EXPLICIT
#include <UserLog.h>
#include "IecString/main.h"
#else
#include <ArEventLog.h>
#include <sys_lib.h>
#include <astime.h>
#endif
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Macros */
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

/* Constants */
#ifdef USERLOG_HEADER_EXPLICIT
#define USERLOG_FACILITY 10U
#define USERLOG_MESSAGE_LENGTH 120U
#endif
#define DATA_MESSAGE_SIZE (USERLOG_MESSAGE_LENGTH + 1U)
#define DATA_CODED_SIZE (40U + USERLOG_MESSAGE_LENGTH + 2U)
#define FACILITY_ERROR 330U
#define CODE_ERROR_IDENT 10000U
#define CODE_ERROR_WRITE 10010U
#define CODE_ERROR_CREATE 10020U
#define LOGBOOK_USER_NAME "$arlogusr"

/* Types */
#ifdef USERLOG_HEADER_EXPLICIT
#ifndef USERLOG_FORMAT_TYPE_EXTERNAL
#define USERLOG_FORMAT_INDEX 5U
typedef struct UserLogFormatType
{
    uint8_t b[6];
    double f[6];
    int32_t i[6];
    char s[6][81];
} UserLogFormatType;
#else
#include "../format.h"
#endif
#endif

/* Global variables */
extern UserLogSeverityEnum severity_level;
extern const uint8_t severity_map[];

/* Prototypes */
#ifdef USERLOG_HEADER_EXPLICIT
uint32_t UserLogBasic(int32_t Severity, uint16_t Code, char *Message);
uint32_t UserLogAdvanced(int32_t Severity, uint16_t Code, char *Message, 
                         struct UserLogFormatType *Values);
uint32_t UserLogCustom(char *Logbook, int32_t Severity, uint16_t Facility, 
                       uint16_t Code, ArEventLogRecordIDType Origin, 
                       char *Object, char *Message, 
                       struct UserLogFormatType *Values);
uint32_t UserLogEventText(char *Logbook, int32_t Event, 
                          ArEventLogRecordIDType Origin, char *Object, 
                          char *Message, struct UserLogFormatType *Values);
int32_t UserLogCreate(char *Name, uint32_t Size);
int32_t UserLogSetSeverityLevel(int32_t Level);
uint8_t UserLogGetSeverity(int32_t Event);
uint16_t UserLogGetFacility(int32_t Event);
uint16_t UserLogGetCode(int32_t Event);
#endif

#ifdef __cplusplus
};
#endif

#endif /* USERLOG_MAIN_H */
