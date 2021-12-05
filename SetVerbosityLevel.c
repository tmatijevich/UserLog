/*********************************************************************************
 * File:      SetVerbosityLevel.c
 * Author:    Tyler Matijevich
 * Created:   October 6, 2021/17:20 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Set the verbosity level to suppress high verbose messages */
signed long SetVerbosityLevel(UserLogSeverityEnum level) {
	
	if(level > USERLOG_SEVERITY_DEBUG)
		return -1;
		
	verbosityLevel = level; /* Set global verbosity level */
	return 0;
	
} /* End function */
