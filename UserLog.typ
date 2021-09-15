
TYPE
	UserLogErrorEnum : 
		( (*Error codes returned by all functions and function blocks*)
		USERLOG_ERROR_NONE := 0, (*Successful operation*)
		USERLOG_ERROR_BUFFERFULL := 10, (*Log buffer is already full. No further entries will be written until the buffer is emptied.*)
		USERLOG_ERROR_TASKNAME, (*Error from sys_lib ST_name() call. See info.sysLibStatus*)
		USERLOG_ERROR_SEVERITY, (*Invalid severity*)
		USERLOG_ERROR_IDENT, (*Unable to retreive user logbook ident. See info.arEventLogErrorID*)
		USERLOG_ERROR_WRITE (*Unable to write to user logbook. See info.arEventLogErrorID*)
		);
	UserLogSeverityEnum : 
		( (*Levels of severity, see AS Help 32-bit event ID*)
		USERLOG_SEVERITY_SUCCESS := 0, (*Success*)
		USERLOG_SEVERITY_INFORMATION := 1, (*Information*)
		USERLOG_SEVERITY_WARNING := 2, (*Warning*)
		USERLOG_SEVERITY_ERROR := 3 (*Error*)
		);
	UserLogCyclicStateEnum : 
		( (*States for the cyclic log buffer function block*)
		USERLOG_STATE_IDENT := 0, (*Execute ArEventLogGetIdent()*)
		USERLOG_STATE_IDLE, (*Wait for new entry in buffer*)
		USERLOG_STATE_ADMIN, (*Log administrative message*)
		USERLOG_STATE_WRITE, (*Complete ArEventLogWrite() call*)
		USERLOG_STATE_ERROR := 255 (*An error has occured during the cyclic log buffer operation and requires and error reset*)
		);
	UserLogBufferInfoType : 	STRUCT  (*Information structure declaration for the UserLog library's FIFO event entry buffer*)
		ident : ArEventLogIdentType; (*Ident of the User logbook*)
		writeIndex : USINT; (*Next index in log buffer for LogMessage()*)
		readIndex : USINT; (*Next index in log buffer for ArEventLogWrite()*)
		state : UserLogCyclicStateEnum; (*State of the CyclicLogBuffer function block state machine*)
		full : BOOL; (*The FIFO log buffer is full. This remains true until emptied. All calls to LogMessage() are lost until emptied.*)
		bufferCount : USINT; (*Entries currently in log buffer*)
		loggedCount : UDINT; (*Entries successfully written to the User logbook by the log buffer*)
		lostCount : UDINT; (*Entries lost due to log buffer full, or error from task name or severity.*)
		suppressedCount : UDINT; (*Entries suppressed due to severity subceeding the threshold*)
		sysLibStatus : UINT; (*Status from ST_name() call*)
		arEventLogErrorID : DINT; (*ErrorID from ArEventLogGetIdent() or ArEventLogWrite()*)
	END_STRUCT;
END_TYPE
