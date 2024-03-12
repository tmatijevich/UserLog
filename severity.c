/*******************************************************************************
 * File: severity.c
 * Created: 2021-10-06
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file severity.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Suppress DEBUG messages by default */
UserLogSeverityEnum severity_level = USERLOG_SEVERITY_SUCCESS;

const uint8_t severity_map[] = {
    arEVENTLOG_SEVERITY_INFO, /* USERLOG_SEVERITY_DEBUG */
    arEVENTLOG_SEVERITY_SUCCESS, /* USERLOG_SEVERITY_SUCCESS */
    arEVENTLOG_SEVERITY_INFO, /* USERLOG_SEVERITY_INFORMATION */
    arEVENTLOG_SEVERITY_WARNING, /* USERLOG_SEVERITY_WARNING */
    arEVENTLOG_SEVERITY_ERROR, /* USERLOG_SEVERITY_ERROR */
    arEVENTLOG_SEVERITY_ERROR}; /* USERLOG_SEVERITY_CRITICAL */

/* Suppress messages below the input level and return previous level */
int32_t UserLogSetSeverityLevel(int32_t Level)
{
    /* Store previous global level */
    UserLogSeverityEnum previous = severity_level;

    /* Saturate level */
    Level = MIN(MAX(USERLOG_SEVERITY_DEBUG, Level), USERLOG_SEVERITY_CRITICAL);

    severity_level = Level;
    return previous;
}
