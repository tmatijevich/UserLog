(*******************************************************************************
 * File: UserLog.fun
 * Author: Tyler Matijevich
 * Created: 2020-10-29
*******************************************************************************)

FUNCTION LogMessage : DINT (*Write message (event) to user logbook*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Severity (verbosity), see map and AH for 32bit event ID*)
		code : UINT; (*Code, see AH for 32bit event ID*)
		message : STRING[0]; (*Additional (ASCII) data of logbook record*)
		args : FormatStringArgumentsType; (*Format message arguments %b bool %i dint %r lreal %s string*)
	END_VAR
END_FUNCTION

FUNCTION SetVerbosityLevel : DINT (*Set level to suppress high verbose messages*)
	VAR_INPUT
		level : UserLogSeverityEnum; (*Suppress messages above verbosity level*)
	END_VAR
END_FUNCTION

FUNCTION GetUserLogInfo : DINT (*Read library's logging history*)
	VAR_INPUT
		logInfo : UserLogInfoType; (*Information structure*)
	END_VAR
END_FUNCTION

FUNCTION CreateCustomLogbook : DINT (*_INIT routine ONLY! Create custom logbook in USERROM, skip if already exists*)
	VAR_INPUT
		name : STRING[0]; (*Name of logbook*)
		size : UDINT; (*Size of logbook (minimum 4096, commonly 200000), user is responsible for memory - see user partition size*)
	END_VAR
END_FUNCTION

FUNCTION CustomMessage : DINT (*Write message (event) to custom logbook*)
	VAR_INPUT
		severity : UserLogSeverityEnum; (*Severity (verbosity), see map and AH for 32bit event ID*)
		code : UINT; (*Code, see AH for 32bit event ID*)
		message : STRING[0]; (*Additional (ASCII) data of logbook record*)
		args : FormatStringArgumentsType; (*Format message arguments %b bool %i dint %r lreal %s string*)
		logbook : STRING[0]; (*Name of logbook*)
		facility : USINT; (*0..15 area, see AH for 32bit event ID*)
	END_VAR
END_FUNCTION
