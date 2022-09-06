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

/* Prototypes */
uint32_t stringCopy(char *destination, char *source, uint32_t size);
uint32_t stringFormat(char *destination, char *source, UserLogFormatArgumentType *args, uint32_t size);

#ifdef __cplusplus
};
#endif

#endif
