/*******************************************************************************
 * File: UserLog\Severity.c
 * Author: Tyler Matijevich
 * Created: 2021-10-06
 ******************************************************************************/

#include "Main.h"

/* Ignore DEBUG messages by default */
UserLogSeverityEnum SeverityLevel = USERLOG_SEVERITY_SUCCESS;

/* Set severity level. Suppresses messages below level */
int32_t UserLogSetSeverityLevel(int32_t Level) {
	/* Saturate Level */
	if (Level < USERLOG_SEVERITY_DEBUG) 
		Level = USERLOG_SEVERITY_DEBUG;
	else if (Level > USERLOG_SEVERITY_CRITICAL) 
		Level = USERLOG_SEVERITY_CRITICAL;
	
	return SeverityLevel = Level;
}

/* Get ArEventLog severity from event ID */
uint8_t UserLogGetSeverity(int32_t Event) {
	/* Return bit 31-30. See 32-bit event ID in ArEventLog help */
	return (uint8_t)(Event >> 30 & 0x3);
}

/* Get ArEventLog facility from event ID */
uint16_t UserLogGetFacility(int32_t Event) {
	/* Return bit 27-16. See 32-bit event ID in ArEventLog help */
	return (uint16_t)(Event >> 16 & 0xFFF);
}

/* Get ArEventLog code from event ID */
uint16_t UserLogGetCode(int32_t Event) {
	/* Return bit 15-0. See 32-bit event ID in ArEventLog help */
	return (uint16_t)(Event & 0xFFFF);
}
