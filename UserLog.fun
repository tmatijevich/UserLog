
FUNCTION UserLogBasic : UDINT (*Write to the User logbook*)
	VAR_INPUT
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Code : UINT; (*0..65535 unique code*)
		Message : STRING[0]; (*ASCII data message*)
	END_VAR
END_FUNCTION

FUNCTION UserLogAdvanced : UDINT (*Write to the User logbook with runtime data*)
	VAR_INPUT
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Code : UINT; (*0..65535 unique code*)
		Message : STRING[0]; (*ASCII data message*)
		Arguments : UserLogFormatType; (*Format arguments*)
	END_VAR
END_FUNCTION

FUNCTION UserLogCustom : UDINT (*Write to any user logbook synchronously*)
	VAR_INPUT
		Logbook : STRING[0]; (*Name of logbook*)
		Severity : DINT; (*Use UserLogSeverityEnum or arEVENTLOG_SEVERITY constants*)
		Facility : UINT; (*0..15 application area 16..4095 3rd-party device area*)
		Code : UINT; (*0..65535 unique code*)
		Origin : ArEventLogRecordIDType; (*(Optional) Origin record ID*)
		Object : STRING[0]; (*(Optional) Object name*)
		Message : STRING[0]; (*ASCII data message*)
		Arguments : UserLogFormatType; (*Format arguments*)
	END_VAR
END_FUNCTION

FUNCTION UserLogCreate : DINT (*Create custom logbook in INIT*)
	VAR_INPUT
		Name : STRING[0]; (*Name of logbook limited to 10 characters*)
		Size : UDINT; (*Size of logbook in bytes (minimum 4096)*)
	END_VAR
END_FUNCTION

FUNCTION UserLogSetSeverityLevel : DINT (*Suppress messages below level*)
	VAR_INPUT
		Level : DINT;
	END_VAR
END_FUNCTION

FUNCTION UserLogGetSeverity : USINT (*Get ArEventLog severity from event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION

FUNCTION UserLogGetFacility : UINT (*Get ArEventLog facility from event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION

FUNCTION UserLogGetCode : UINT (*Get ArEventLog code from event ID*)
	VAR_INPUT
		Event : DINT; (*Event ID*)
	END_VAR
END_FUNCTION
