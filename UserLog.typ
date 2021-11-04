
TYPE
	UserLogErrorEnum : 
		( (*Error codes returned by all functions and function blocks*)
		USERLOG_ERROR_NONE := 0, (*Successful operation*)
		USERLOG_ERROR_MAX := 10, (*Log buffer is already full. No further entries will be written until the buffer is emptied.*)
		USERLOG_ERROR_LEVEL, (*Invalid severity level*)
		USERLOG_ERROR_IDENT, (*Unable to retreive user logbook ident. See info.arEventLogErrorID*)
		USERLOG_ERROR_WRITE (*Unable to write to user logbook. See info.arEventLogErrorID*)
		);
	UserLogSeverityEnum : 
		( (*Levels of severity, see AS Help 32-bit event ID*)
		USERLOG_SEVERITY_CRITICAL := 0, (*Error: The application in unrecoverable*)
		USERLOG_SEVERITY_ERROR := 1, (*Error: The application in recoverable*)
		USERLOG_SEVERITY_WARNING := 2, (*Warning: The application may continue*)
		USERLOG_SEVERITY_INFORMATION := 3, (*Information: User information*)
		USERLOG_SEVERITY_SUCCESS := 4, (*Success: Completion*)
		USERLOG_SEVERITY_DEBUG := 5 (*Information: Developer information*)
		);
	UserLogCyclicStateEnum : 
		( (*States for the cyclic log buffer function block*)
		USERLOG_STATE_IDENT := 0, (*Execute ArEventLogGetIdent()*)
		USERLOG_STATE_IDLE, (*Wait for new entry in buffer*)
		USERLOG_STATE_ADMIN, (*Log administrative message*)
		USERLOG_STATE_WRITE, (*Complete ArEventLogWrite() call*)
		USERLOG_STATE_ERROR := 255 (*An error has occured during the cyclic log buffer operation and requires and error reset*)
		);
	UserLogInfoType : 	STRUCT  (*Logging history and error information*)
		loggedCount : UDINT; (*Entries successfully written to the ser logbook*)
		lostCount : UDINT; (*Entries lost due to ArEventLog error, invalid severity, or messages exceeding max per cycle*)
		suppressedCount : UDINT; (*Entries suppressed due to severity subceeding the threshold*)
		arEventLogStatusID : DINT; (*ErrorID from ArEventLogGetIdent() or ArEventLogWrite()*)
	END_STRUCT;
END_TYPE
