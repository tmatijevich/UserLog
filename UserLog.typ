(*******************************************************************************
 * File: UserLog.typ
 * Author: Tyler Matijevich
 * Created: 2020-10-29
*******************************************************************************)

TYPE
	UserLogSeverityEnum : 
		( (*Levels of severity, see AS Help 32-bit event ID*)
		USERLOG_SEVERITY_CRITICAL := 0, (*Error: The application in unrecoverable*)
		USERLOG_SEVERITY_ERROR := 1, (*Error: The application in recoverable*)
		USERLOG_SEVERITY_WARNING := 2, (*Warning: The application may continue*)
		USERLOG_SEVERITY_INFORMATION := 3, (*Information: User information*)
		USERLOG_SEVERITY_SUCCESS := 4, (*Success: Completion*)
		USERLOG_SEVERITY_DEBUG := 5 (*Information: Developer information*)
		);
	UserLogInfoType : 	STRUCT  (*Logging history and error information*)
		loggedCount : UDINT; (*Entries successfully written to the User logbook*)
		lostCount : UDINT; (*Entries lost due to ArEventLog error or messages exceeding max per cycle*)
		suppressedCount : UDINT; (*Entries suppressed due to verbosity level*)
	END_STRUCT;
END_TYPE
