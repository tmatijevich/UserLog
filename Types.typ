
TYPE
	UserLogErrorEnum : 
		(
		USERLOG_ERROR_NONE := 0,
		USERLOG_ERROR_INVALID_SEVERITY := 101, (*The severity is not an option when generating the EventID (<= 3)*)
		USERLOG_ERROR_TASK_NAME, (*The task name function from sys_lib has returned an error*)
		USERLOG_ERROR_LOGBOOK_IDENT, (*The function block to retrieve the user logbook ident address has returned an error*)
		USERLOG_ERROR_WRITE, (*The function block to write an entry to the user logbook has returned an error*)
		USERLOG_ERROR_BUFFER_FULL (*The log buffer is full and no further events can be written until the buffer is emptied*)
		);
	UserLogSeverityEnum : 
		( (*Levels of severity, see AS Help 32-bit event ID*)
		USER_LOG_SEVERITY_SUCCESS := 0, (*The logbook entry marks success operation completion*)
		USER_LOG_SEVERITY_INFORMATION := 1, (*An informative entry useful to the user*)
		USER_LOG_SEVERITY_WARNING := 2, (*An issue has occured during operation but is not critical*)
		USER_LOG_SEVERITY_ERROR := 3 (*An error has occured during operation*)
		);
	UserLogBufferInfoType : 	STRUCT 
		WriteIndex : USINT;
		ReadIndex : USINT;
		Full : BOOL;
		State : USINT;
		UserLogbookIdent : ArEventLogIdentType;
		NumEntriesInBuffer : USINT;
		NumEntriesLogged : USINT;
		NumEntriesLost : USINT;
	END_STRUCT;
END_TYPE
