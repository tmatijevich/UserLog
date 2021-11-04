
FUNCTION LogMessage : DINT (*Add entry to user logbook event buffer*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity (0 - Success, 1 - Information, 2 - Warning, 3 - Error), see AS Help for 32-bit event ID*)
		code : UINT; (*Entry code (0 - 65535), see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASC II data up to USERLOG_MESSAGE_LENGTH characters*)
	END_VAR
END_FUNCTION

FUNCTION SetVebosityLevel : DINT (*Set level of verbosity*)
	VAR_INPUT
		level : UserLogSeverityEnum; (*Suppress messages above verbosity level*)
	END_VAR
END_FUNCTION

FUNCTION GetLogInfo : DINT (*Gather information on logging history*)
	VAR_INPUT
		logInfo : UserLogInfoType;
	END_VAR
END_FUNCTION
