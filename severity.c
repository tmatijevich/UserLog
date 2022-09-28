/*******************************************************************************
 * File: UserLog\severity.c
 * Author: Tyler Matijevich
 * Created: 2021-10-06
 ******************************************************************************/

#include "Main.h"

/* Ignore DEBUG messages by default */
UserLogSeverityEnum severity_level = USERLOG_SEVERITY_SUCCESS;

/* Set severity level. Suppresses messages below level */
uint32_t UserLogSeverity (int32_t level)
{
	/* Saturate level */
	if (level < USERLOG_SEVERITY_DEBUG) 
		level = USERLOG_SEVERITY_DEBUG;
	else if (level > USERLOG_SEVERITY_CRITICAL) 
		level = USERLOG_SEVERITY_CRITICAL;
	
	return severity_level = level;
}
