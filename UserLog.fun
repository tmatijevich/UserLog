
FUNCTION LogMessage : DINT (*Add entry to user logbook event buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity, see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code, see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASC II data*)
	END_VAR
END_FUNCTION

FUNCTION_BLOCK CyclicLogBuffer (*Write buffered event entries to user logbook*)
	VAR_INPUT
		SeverityThreshold : UserLogSeverityEnum; (*New entries are suppressed if below threshold*)
		ErrorReset : BOOL; (*Reset if error occurs*)
	END_VAR
	VAR_OUTPUT
		ErrorID : UserLogErrorEnum; (*Operation status*)
	END_VAR
END_FUNCTION_BLOCK

FUNCTION GetBufferInfo : DINT (*Get logging buffer information, returns 0*)
	VAR_INPUT
		bufferInfo : UserLogBufferInfoType; (*Local information structure*)
	END_VAR
END_FUNCTION

FUNCTION LogMessage2 : DINT (*Add entry to user logbook event buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity, see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code, see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASC II data*)
	END_VAR
END_FUNCTION
