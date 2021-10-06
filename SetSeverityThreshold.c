/*********************************************************************************
 * File:      B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich
 * Created:   October 6, 2021/17:20 
 *********************************************************************************/ 

#include "UserLogMain.h"

signed long SetSeverityThreshold(enum UserLogSeverityEnum threshold) {
	
	if(threshold < USERLOG_SEVERITY_SUCCESS || threshold > USERLOG_SEVERITY_SUPPRESS)
		return USERLOG_ERROR_SEVERITY;
		
	severityThreshold = threshold; /* Set the global threshold */
	return USERLOG_ERROR_NONE;
	
} /* End function */
