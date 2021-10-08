
FUNCTION LogMessage : DINT (*Add entry to user logbook event buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity (0 - Success, 1 - Information, 2 - Warning, 3 - Error), see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code (0 - 65535), see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASC II data up to USERLOG_MESSAGE_LENGTH characters*)
	END_VAR
END_FUNCTION

FUNCTION SetThreshold : DINT (*Suppress user logbook messages below the threshold*)
	VAR_INPUT
		threshold : UserLogSeverityEnum; (*Suppress messages below severity level*)
	END_VAR
END_FUNCTION
