
FUNCTION LogMessage : DINT (*Add entry to FIFO log buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity, see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code, see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate ASC II data of logbook entry*)
	END_VAR
END_FUNCTION

FUNCTION_BLOCK CyclicLogBuffer (*Write buffered entries to the user logbook*)
	VAR_INPUT
		ErrorReset : BOOL; (*Reset the function block in case of an error during operation*)
		SeverityThreshold : UserLogSeverityEnum; (*Only LogMessage() calls with a severity level equal to or greater than this threshold will be written to the User logbook*)
	END_VAR
	VAR_OUTPUT
		ReturnValue : UserLogErrorEnum; (*Return status value for the user*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION GetBufferInfo : DINT (*Function to obtain the information structure describing the log event buffer. Always returns 0*)
	VAR_IN_OUT
		LogBufferInfo : UserLogBufferInfoType; (*Reference pointer to a local structure to copy the information to*)
	END_VAR
END_FUNCTION
