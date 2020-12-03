
TYPE
	UserLogErrorEnum : 
		( (*Errors that may occur during UserLog library function and function block operations*)
		USERLOG_ERROR_NONE := 0, (*No error has occured since the last system restart or error reset command*)
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
	UserLogCyclicStateEnum : 
		( (*States for the cyclic log buffer function block*)
		USER_LOG_STATE_INIT, (*Initialize the user logbook get identity function block*)
		USER_LOG_STATE_IDENT, (*Handle the completion of the get logbook identity function block*)
		USER_LOG_STATE_IDLE, (*Wait for the write index to exceed the read index, or pop all entries from the buffer if flagged as full*)
		USER_LOG_STATE_ADMIN, (*Write administrative messages when the buffer is full, then subsequently emptied.*)
		USER_LOG_STATE_WRITE, (*Handle the completion of the logbook write function block*)
		USER_LOG_STATE_ERROR := 255 (*An error has occured during the cyclic log buffer operation and requires and error reset*)
		);
	UserLogBufferInfoType : 	STRUCT  (*Information structure declaration for the UserLog library's FIFO event entry buffer*)
		WriteIndex : USINT; (*Next index the LogEvent function will add to*)
		ReadIndex : USINT; (*Next index the CyclicLogBuffer will read from*)
		Full : BOOL; (*The FIFO event entry buffer has been fulled. This status will stay true until it is emptied. All calls to LogEvent will result in a lost event.*)
		State : UserLogCyclicStateEnum; (*State of the CyclicLogBuffer function block state machine*)
		UserLogbookIdent : ArEventLogIdentType; (*Store the Ident address of the user logbook from ArEventLogGetIdent function block*)
		NumEntriesInBuffer : USINT; (*This value counts the number of entries currently in the buffer*)
		NumEntriesLogged : UDINT; (*This value increments every time a buffered event entry is written to the user logbook*)
		NumEntriesLost : UDINT; (*This value increments every time LogEvent function is called but results in an error (Buffer full, Task name error, Invalid severity)*)
		NumEntriesIgnored : UDINT; (*Number of calls to LogEvent() ignored because the severity was below the SeverityThreshold of CyclicLogBuffer()*)
	END_STRUCT;
END_TYPE
