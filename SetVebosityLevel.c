/*********************************************************************************
 * File:      SetVerbosityLevel.c
 * Author:    Tyler Matijevich
 * Created:   October 6, 2021/17:20 
 *********************************************************************************/ 

#include "UserLogMain.h"

signed long SetVebosityLevel(UserLogSeverityEnum level) {
	
	if(level > USERLOG_SEVERITY_DEBUG)
		return USERLOG_ERROR_LEVEL;
		
	verbosityLevel = level; /* Set global verbosity level */
	return USERLOG_ERROR_NONE;
	
} /* End function */
