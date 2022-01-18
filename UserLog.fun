
FUNCTION LogMessage : DINT (*Write message (event) to user logbook*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity (verbosity) - mapped to ArEventLog severities, see AS Help*)
		code : UINT; (*Entry code (0 - 65535), see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASCII data up to USERLOG_MESSAGE_LENGTH characters*)
		args : FormatStringArgumentsType; (*Format message arguments %b boolean %i integer %r real %s string*)
	END_VAR
END_FUNCTION

FUNCTION SetVerbosityLevel : DINT (*Set the verbosity level to suppress high verbose messages*)
	VAR_INPUT
		level : UserLogSeverityEnum; (*Suppress messages above verbosity level*)
	END_VAR
END_FUNCTION

FUNCTION GetUserLogInfo : DINT (*Gather information on this library's logging history*)
	VAR_INPUT
		logInfo : UserLogInfoType; (*Information structure*)
	END_VAR
END_FUNCTION

FUNCTION CreateCustomLogbook : DINT (*_INIT routine ONLY! Create custom logbook in USERROM, skip if already exists*)
	VAR_INPUT
		name : STRING[0]; (*Set name of logbook, see ArEventLogCreate for details*)
		size : UDINT; (*Size of logbook in bytes (minimum 4096, commonly 200000), user is responsible for enough memory in USERROM - see user partition size*)
	END_VAR
END_FUNCTION

FUNCTION CustomMessage : DINT (*Write message (event) to custom logbook*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Entry severity (verbosity) - mapped to ArEventLog severities, see AS Help*)
		code : UINT; (*Entry code (0 - 65535), see AS Help for 32-bit event ID*)
		message : STRING[0]; (*Message to populate entry ASCII data up to USERLOG_MESSAGE_LENGTH characters*)
		args : FormatStringArgumentsType; (*Format message arguments %b boolean %i integer %r real %s string*)
		logbook : STRING[0]; (*Name of logbook to write message to*)
		facility : USINT; (*0..15 event ID area, see AS Help for 32-bit event ID*)
	END_VAR
END_FUNCTION
