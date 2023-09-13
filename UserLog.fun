
FUNCTION UserLogBasic : UDINT (*Write to the User logbook synchronously*)
	VAR_INPUT
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Code : UINT; (*0..65535*)
		Message : STRING[0]; (*Event description*)
	END_VAR
END_FUNCTION

FUNCTION UserLogAdvanced : UDINT (*Write to the User logbook with runtime data*)
	VAR_INPUT
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Code : UINT; (*0..65535*)
		Message : STRING[0]; (*Event description with optional format specifiers*)
		Values : UserLogFormatType; (*Format values to replace specifiers (%b, %f, %i, %s)*)
	END_VAR
END_FUNCTION

FUNCTION UserLogCustom : UDINT (*Write to any user logbook synchronously*)
	VAR_INPUT
		Logbook : STRING[0]; (*Name of logbook*)
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Facility : UINT; (*0..4095*)
		Code : UINT; (*0..65535*)
		Origin : ArEventLogRecordIDType; (*(Optional) Origin record ID*)
		Object : STRING[0]; (*(Optional) Object name*)
		Message : STRING[0]; (*Event description with optional format specifiers*)
		Values : UserLogFormatType; (*Format values to replace specifiers (%b, %f, %i, %s)*)
	END_VAR
END_FUNCTION

FUNCTION UserLogEventText : UDINT (*Write to any user logbook through binary-encoded data and event texts*)
	VAR_INPUT
		Logbook : STRING[0]; (*Name of logbook*)
		Event : DINT; (*Event ID (match Text ID)*)
		Origin : ArEventLogRecordIDType; (*(Optional) Origin record ID*)
		Object : STRING[0]; (*(Optional) Object name*)
		Message : STRING[0]; (*Event description with optional format specifiers*)
		Values : UserLogFormatType; (*Format values to replace specifiers (%b, %f, %i, %s)*)
	END_VAR
END_FUNCTION

FUNCTION UserLogCreate : DINT (*Create a custom logbook in an Init subroutine*)
	VAR_INPUT
		Name : STRING[0]; (*Name of logbook limited to 10 characters*)
		Size : UDINT; (*Size of logbook in bytes (minimum 4096)*)
	END_VAR
END_FUNCTION

FUNCTION UserLogSetSeverityLevel : DINT (*Suppress messages below the input level and return previous level*)
	VAR_INPUT
		Level : DINT; (*Severity level threshold*)
	END_VAR
END_FUNCTION

FUNCTION UserLogGetSeverity : USINT (*Get ArEventLog severity from an event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION

FUNCTION UserLogGetFacility : UINT (*Get ArEventLog facility from an event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION

FUNCTION UserLogGetCode : UINT (*Get ArEventLog code from an event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION
