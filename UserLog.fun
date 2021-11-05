
FUNCTION LogMessage : DINT (*Add entry to user logbook event buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity (verbosity) - mapped to ArEventLog severities, see AS Help*)
		code : UINT; (*Entry code (0 - 65535), see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASC II data up to USERLOG_MESSAGE_LENGTH characters*)
	END_VAR
END_FUNCTION

FUNCTION SetVebosityLevel : DINT (*Set the verbosity level to suppress high verbose messages*)
	VAR_INPUT
		level : UserLogSeverityEnum; (*Suppress messages above verbosity level*)
	END_VAR
END_FUNCTION

FUNCTION GetUserLogInfo : DINT (*Gather information on this library's logging history*)
	VAR_INPUT
		logInfo : UserLogInfoType; (*Information structure*)
	END_VAR
END_FUNCTION
