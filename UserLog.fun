
FUNCTION LogMessage : DINT (*Add entry to FIFO log buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity, see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code, see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate ASC II data of logbook entry*)
	END_VAR
END_FUNCTION

FUNCTION_BLOCK CyclicLogBuffer (*Write buffered event entries to the User logbook*)
	VAR_INPUT
		severityThreshold : UserLogSeverityEnum; (*Calls to LogMessage() will be suppressed if below the threshold*)
		reset : BOOL; (*Reset the function block if error occurs during operation*)
	END_VAR
	VAR_OUTPUT
		status : UserLogErrorEnum; (*Return operation status*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION GetBufferInfo : DINT (*Get information on the logging buffer. Returns 0.*)
	VAR_INPUT
		bufferInfo : UserLogBufferInfoType; (*Local logging buffer information structure*)
	END_VAR
END_FUNCTION
