
TYPE
	UserLogSeverityEnum : 
		( (*Levels of severity. See 32-bit event ID in Automation Help*)
		USERLOG_SEVERITY_DEBUG := -1, (*Information: Developer information*)
		USERLOG_SEVERITY_SUCCESS, (*Success: Completion*)
		USERLOG_SEVERITY_INFORMATION, (*Information: User information*)
		USERLOG_SEVERITY_WARNING, (*Warning: The application may continue*)
		USERLOG_SEVERITY_ERROR, (*Error: The application in recoverable*)
		USERLOG_SEVERITY_CRITICAL (*Error: The application in unrecoverable*)
		);
	UserLogFormatType : 	STRUCT  (*Data structure to replace format specifier with runtime values*)
		b : ARRAY[0..USERLOG_FORMAT_INDEX]OF BOOL; (*Place boolean (TRUE or FALSE)*)
		f : ARRAY[0..USERLOG_FORMAT_INDEX]OF LREAL; (*Place double*)
		i : ARRAY[0..USERLOG_FORMAT_INDEX]OF DINT; (*Place integer (32-bit signed)*)
		s : ARRAY[0..USERLOG_FORMAT_INDEX]OF STRING[USERLOG_FORMAT_LENGTH]; (*Place string*)
	END_STRUCT;
END_TYPE
