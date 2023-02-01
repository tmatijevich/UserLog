/*******************************************************************************
 * File: UserLog\severity.c
 * Author: Tyler Matijevich
 * Created: 2021-10-06
 ******************************************************************************/

#include "Main.h"

/* Ignore DEBUG messages by default */
UserLogSeverityEnum severity_level = USERLOG_SEVERITY_SUCCESS;

/* Set severity level. Suppresses messages below level */
int32_t UserLogSetSeverityLevel (int32_t level)
{
	/* Saturate level */
	if (level < USERLOG_SEVERITY_DEBUG) 
		level = USERLOG_SEVERITY_DEBUG;
	else if (level > USERLOG_SEVERITY_CRITICAL) 
		level = USERLOG_SEVERITY_CRITICAL;
	
	return severity_level = level;
}

/* Get ArEventLog severity from event ID */
uint8_t UserLogGetSeverity (int32_t event) 
{
	/* Return bit 31-30. See 32-bit event ID in ArEventLog help */
	return (uint8_t)(event >> 30 & 0x3);
}

/* Get ArEventLog facility from event ID */
uint16_t UserLogGetFacility (int32_t event) 
{
	/* Return bit 27-16. See 32-bit event ID in ArEventLog help */
	return (uint16_t)(event >> 16 & 0xFFF);
}

/* Get ArEventLog code from event ID */
uint16_t UserLogGetCode (int32_t event) 
{
	/* Return bit 15-0. See 32-bit event ID in ArEventLog help */
	return (uint16_t)(event & 0xFFFF);
}
