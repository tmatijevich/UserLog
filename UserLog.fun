(*******************************************************************************
 * File: UserLog.fun
 * Author: Tyler Matijevich
 * Created: 2020-10-29
*******************************************************************************)

FUNCTION UserLogMessage : DINT (*Write to logbook synchronously*)
	VAR_INPUT
		logbook : STRING[0]; (*Name of logbook*)
		severity : UserLogSeverityEnum; (*See conversion table between UserLog severitiy and ArEventLog severity*)
		facility : UINT; (*0..15 application area 16..4095 3rd-party device area*)
		code : UINT; (*0..65535 unique event ID code*)
		origin : ArEventLogRecordIDType; (*Optional ID of origin record*)
		object : STRING[0]; (*Optional object name*)
		message : STRING[0]; (*ASCII data message*)
		args : UserLogFormatArgumentType; (*Format arguments*)
	END_VAR
END_FUNCTION
