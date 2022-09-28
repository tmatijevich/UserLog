(*******************************************************************************
 * File: UserLog.fun
 * Author: Tyler Matijevich
 * Created: 2020-10-29
*******************************************************************************)

FUNCTION UserLogQuick : UDINT (*Write to user logbook. Returns record ID if successful, zero otherwise*)
	VAR_INPUT
		severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		code : UINT; (*0..65535 unique code*)
		message : STRING[0]; (*ASCII data message*)
	END_VAR
END_FUNCTION

FUNCTION UserLogFormat : UDINT (*Write to user logbook with runtime data.  Returns record ID if successful, zero otherwise*)
	VAR_INPUT
		severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		code : UINT; (*0..65535 unique code*)
		message : STRING[0]; (*ASCII data message*)
		args : UserLogFormatType; (*Format arguments*)
	END_VAR
END_FUNCTION

FUNCTION UserLogMessage : UDINT (*Write to logbook synchronously. Returns record ID if successful, zero otherwise*)
	VAR_INPUT
		logbook : STRING[0]; (*Name of logbook*)
		severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		facility : UINT; (*0..15 application area 16..4095 3rd-party device area*)
		code : UINT; (*0..65535 unique code*)
		origin : ArEventLogRecordIDType; (*(Optional) Origin record ID*)
		object : STRING[0]; (*(Optional) Object name*)
		message : STRING[0]; (*ASCII data message*)
		args : UserLogFormatType; (*Format arguments*)
	END_VAR
END_FUNCTION

FUNCTION UserLogSeverity : DINT (*Set severity level. Suppresses messages below level*)
	VAR_INPUT
		level : DINT;
	END_VAR
END_FUNCTION

FUNCTION UserLogCreate : DINT (*Create custom logbook. Only use in _INIT routine*)
	VAR_INPUT
		name : STRING[0]; (*Name of logbook limited to 10 characters*)
		size : UDINT; (*Size of logbook in bytes (minimum 4096)*)
	END_VAR
END_FUNCTION
