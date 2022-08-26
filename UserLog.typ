(*******************************************************************************
 * File: UserLog.typ
 * Author: Tyler Matijevich
 * Created: 2020-10-29
*******************************************************************************)

TYPE
	UserLogSeverityEnum : 
		( (*Levels of severity, see AS Help 32-bit event ID*)
		USERLOG_SEVERITY_CRITICAL, (*Error: The application in unrecoverable*)
		USERLOG_SEVERITY_ERROR, (*Error: The application in recoverable*)
		USERLOG_SEVERITY_WARNING, (*Warning: The application may continue*)
		USERLOG_SEVERITY_INFORMATION, (*Information: User information*)
		USERLOG_SEVERITY_SUCCESS, (*Success: Completion*)
		USERLOG_SEVERITY_DEBUG (*Information: Developer information*)
		);
	UserLogFormatArgumentType : 	STRUCT  (*Argument structure for runtime data*)
		b : ARRAY[0..USERLOG_FORMATARG_INDEX]OF BOOL; (*Place boolean (TRUE or FALSE)*)
		f : ARRAY[0..USERLOG_FORMATARG_INDEX]OF LREAL; (*Place floating point (double casted float)*)
		i : ARRAY[0..USERLOG_FORMATARG_INDEX]OF DINT; (*Place integer (32-bit signed)*)
		s : ARRAY[0..USERLOG_FORMATARG_INDEX]OF STRING[USERLOG_FORMATARG_STRLEN]; (*Place string*)
	END_STRUCT;
END_TYPE
