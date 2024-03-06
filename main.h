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
#include <UserLog.h>
#include "IecString/main.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Macros */
#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#define MIN(x,y) (((x) < (y)) ? (x) : (y))

/* Constants */
#define DATA_MESSAGE_SIZE (USERLOG_MESSAGE_LENGTH + 1U)
#define DATA_CODED_SIZE (40U + USERLOG_MESSAGE_LENGTH + 2U)
#define FACILITY_ERROR 330U
#define CODE_ERROR_IDENT 10000U
#define CODE_ERROR_WRITE 10010U
#define CODE_ERROR_CREATE 10020U
#define LOGBOOK_USER_NAME "$arlogusr"

/* Global variables */
extern UserLogSeverityEnum severity_level;
extern const uint8_t severity_map[];

#ifdef __cplusplus
};
#endif

#endif /* USERLOG_MAIN_H */
