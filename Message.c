/*******************************************************************************
 * File: UserLog\Message.c
 * Author: Tyler Matijevich
 * Created: 2020-10-29
 ******************************************************************************/

#include "Main.h"

/* Write to logbook synchronously. Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogMessage(char *Logbook, int32_t Severity, uint16_t Facility, uint16_t Code, 
										ArEventLogRecordIDType Origin, char *Object, char *Message, UserLogFormatType *Args) {
	
	/* Declare local variables */
	/* Make function block instances static to avoid memset initialization */
	static ArEventLogGetIdent_typ GetLogbookIdent;
	static ArEventLogWrite_typ WriteToLogbook;
	int32_t Status;
	ArEventLogIdentType Ident;
	static uint8_t Error;
	UserLogFormatType LocalArgs;
	const uint8_t SeverityMap[] = {
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_SUCCESS,
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_WARNING,
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_ERROR
	};
	char AsciiMessage[USERLOG_MESSAGE_LENGTH + 1];
	ArEventLogRecordIDType Result;
	
	/* Saturate severity */
	if(Severity < USERLOG_SEVERITY_DEBUG)
		Severity = USERLOG_SEVERITY_DEBUG;
	else if(Severity > USERLOG_SEVERITY_CRITICAL)
		Severity = USERLOG_SEVERITY_CRITICAL;
	
	/* Suppress */
	if(Severity < SeverityLevel)
		return 0;
	
	/* Call ArEventLog get ident */
	StringCopy(GetLogbookIdent.Name, sizeof(GetLogbookIdent.Name), Logbook);
	GetLogbookIdent.Execute = true;
	ArEventLogGetIdent(&GetLogbookIdent);
	
	/* Store results */
	Status = GetLogbookIdent.StatusID;
	Ident = GetLogbookIdent.Ident;
	
	/* Reset execution */
	GetLogbookIdent.Execute = false;
	ArEventLogGetIdent(&GetLogbookIdent);
	
	/* Error check */
	if(Status) {
		/* Check for repeat recursion */
		if(Error)
			return 0;
		
		/* Set error */
		Error = true;
		
		/* Report error */
		LocalArgs.i[0] = Status;
		StringCopy(LocalArgs.s[0], USERLOG_LOGBOOK_LENGTH + 1, Logbook);
		LocalArgs.i[1] = Code;
		StringCopy(LocalArgs.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, Message);
		UserLogMessage(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_IDENT, 0, NULL, 
						"UserLog: ArEventLog error %i. (Logbook \"%s\", code %i, message \"%s\")", &LocalArgs);
		
		return 0;
	}
	
	/* Write to logbook */
	WriteToLogbook.Ident = Ident;
		
	/* Get event ID */
	WriteToLogbook.EventID = ArEventLogMakeEventID(SeverityMap[Severity + 1], Facility, Code);
	
	/* Origin record */
	WriteToLogbook.OriginRecordID = Origin;
	
	/* Add ascii message */
	if(Message == NULL)
		StringCopy(AsciiMessage, sizeof(AsciiMessage), "UserLog: No message provided");
	else
		StringFormat(AsciiMessage, sizeof(AsciiMessage), Message, Args);
	
	WriteToLogbook.AddDataSize = strlen(AsciiMessage) + 1;
	WriteToLogbook.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
	WriteToLogbook.AddData = (uint32_t)AsciiMessage;
	
	/* Add object name */
	if(Object == NULL)
		ST_name(0, WriteToLogbook.ObjectID, 0);
	else if(*Object == '\0')
		ST_name(0, WriteToLogbook.ObjectID, 0);
	else
		StringCopy(WriteToLogbook.ObjectID, sizeof(WriteToLogbook.ObjectID), Object);
	
	/* Write */
	WriteToLogbook.Execute = true;
	ArEventLogWrite(&WriteToLogbook);
	Status = WriteToLogbook.StatusID;
	Result = WriteToLogbook.RecordID;
	WriteToLogbook.Execute = false;
	ArEventLogWrite(&WriteToLogbook);
	
	if(Status) {
		/* Check for repeat recursion */
		if(Error)
			return 0;
		
		/* Set error */
		Error = true;
		
		/* Report error */
		LocalArgs.i[0] = Status;
		StringCopy(LocalArgs.s[0], USERLOG_LOGBOOK_LENGTH + 1, Logbook);
		LocalArgs.i[1] = Code;
		StringCopy(LocalArgs.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, Message);
		UserLogMessage(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_WRITE, 0, NULL, 
						"UserLog: ArEventLog error %i. (Logbook \"%s\", code %i, message \"%s\")", &LocalArgs);
		
		return 0;
	}
	
	Error = false;
	return Result;
}

/* Write to user logbook. Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogQuick(int32_t Severity, uint16_t Code, char *Message) {
	return UserLogMessage(USERLOG_USER_LOGBOOK, Severity, USERLOG_FACILITY, Code, 0, NULL, Message, NULL);
}

/* Write to user logbook with runtime data.  Returns record ID if successful, zero otherwise */
ArEventLogRecordIDType UserLogFormat(int32_t Severity, uint16_t Code, char *Message, UserLogFormatType *Args) {
	return UserLogMessage(USERLOG_USER_LOGBOOK, Severity, USERLOG_FACILITY, Code, 0, NULL, Message, Args);
}