/*******************************************************************************
 * File: UserLog\message.c
 * Author: Tyler Matijevich
 * Created: 2020-10-29
 ******************************************************************************/

#include "main.h"

/* Write to logbook synchronously. Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogMessage (char *logbook, int32_t severity, uint16_t facility, uint16_t code, 
										ArEventLogRecordIDType origin, char *object, char *message, UserLogFormatType *args)
{
	/* Declare local variables */
	/* Make function block instances static to avoid memset initialization */
	static ArEventLogGetIdent_typ get_logbook_ident;
	static ArEventLogWrite_typ write_to_logbook;
	int32_t status;
	ArEventLogIdentType ident;
	static uint8_t error;
	UserLogFormatType local_args;
	const uint8_t severity_map[] = {
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_SUCCESS,
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_WARNING,
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_ERROR
	};
	char ascii_message[USERLOG_MESSAGE_LENGTH + 1];
	ArEventLogRecordIDType result;
	
	/* Saturate severity */
	if (severity < USERLOG_SEVERITY_DEBUG) 
		severity = USERLOG_SEVERITY_DEBUG;
	else if (severity > USERLOG_SEVERITY_CRITICAL) 
		severity = USERLOG_SEVERITY_CRITICAL;
	
	/* Suppress */
	if (severity < severity_level) 
		return 0;
	
	/* Call ArEventLog get ident */
	string_copy(get_logbook_ident.Name, sizeof(get_logbook_ident.Name), logbook);
	get_logbook_ident.Execute = true;
	ArEventLogGetIdent(&get_logbook_ident);
	
	/* Store results */
	status = get_logbook_ident.StatusID;
	ident = get_logbook_ident.Ident;
	
	/* Reset execution */
	get_logbook_ident.Execute = false;
	ArEventLogGetIdent(&get_logbook_ident);
	
	/* Error check */
	if (status)
	{
		/* Check for repeat recursion */
		if (error) return 0;
		
		local_args.i[0] = status;
		string_copy(local_args.s[0], USERLOG_LOGBOOK_LENGTH + 1, logbook);
		local_args.i[1] = code;
		string_copy(local_args.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, message);
		UserLogMessage(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_FACILITY, USERLOG_CODE_IDENT, 0, NULL, 
						"UserLog: ArEventLog error %i. (logbook \"%s\", code %i, message \"%s\")", &local_args);
		
		/* Set error */
		error = true;
		return 0;
	}
	
	/* Write to logbook */
	write_to_logbook.Ident = ident;
		
	/* Get event ID */
	write_to_logbook.EventID = ArEventLogMakeEventID(severity_map[severity + 1], facility, code);
	
	/* Origin record */
	write_to_logbook.OriginRecordID = origin;
	
	/* Add ascii message */
	if (message == NULL) 
		string_copy(ascii_message, sizeof(ascii_message), "UserLog: No message provided");
	else 
		string_format(ascii_message, sizeof(ascii_message), message, args);
	
	write_to_logbook.AddDataSize = strlen(ascii_message) + 1;
	write_to_logbook.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
	write_to_logbook.AddData = (uint32_t)ascii_message;
	
	/* Add object name */
	if (object == NULL) 
		ST_name(0, write_to_logbook.ObjectID, 0);
	else if (*object == '\0') 
		ST_name(0, write_to_logbook.ObjectID, 0);
	else 
		string_copy(write_to_logbook.ObjectID, sizeof(write_to_logbook.ObjectID), object);
	
	/* Write */
	write_to_logbook.Execute = true;
	ArEventLogWrite(&write_to_logbook);
	status = write_to_logbook.StatusID;
	result = write_to_logbook.RecordID;
	write_to_logbook.Execute = false;
	ArEventLogWrite(&write_to_logbook);
	
	if (status)
	{
		/* Check for repeat recursion */
		if (error) return 0;
		
		local_args.i[0] = status;
		string_copy(local_args.s[0], USERLOG_LOGBOOK_LENGTH + 1, logbook);
		local_args.i[1] = code;
		string_copy(local_args.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, message);
		UserLogMessage(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_FACILITY, USERLOG_CODE_WRITE, 0, NULL, 
						"UserLog: ArEventLog error %i. (logbook \"%s\", code %i, message \"%s\")", &local_args);
		
		/* Set error */
		error = true;
		return 0;
	}
	
	error = false;
	return result;
}

/* Write to user logbook. Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogQuick (int32_t severity, uint16_t code, char *message)
{
	return UserLogMessage(USERLOG_USER_LOGBOOK, severity, USERLOG_QUICK_FACILITY, code, 0, NULL, message, NULL);
}

/* Write to user logbook with runtime data.  Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogFormat (int32_t severity, uint16_t code, char *message, UserLogFormatType *args)
{
	return UserLogMessage(USERLOG_USER_LOGBOOK, severity, USERLOG_QUICK_FACILITY, code, 0, NULL, message, args);
}

