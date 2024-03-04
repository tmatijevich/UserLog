/*******************************************************************************
 * File: Message.c
 * Created: 2020-10-29
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file Message.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Common private function to call ArEventLogWrite synchronously */
static ArEventLogRecordIDType WriteMessage(char *Logbook, int32_t Severity, uint16_t Facility, uint16_t Code, 
	ArEventLogRecordIDType Origin, char *Object, char *Message, UserLogFormatType *Values, uint8_t Coded) {
	
	/* Local variables */
	/* Make function block instances static to avoid memset initialization */
	static ArEventLogGetIdent_typ GetLogbookIdent;
	static ArEventLogWrite_typ WriteToLogbook;
	int32_t Status;
	ArEventLogIdentType Ident;
	static uint8_t Error;
	UserLogFormatType LocalArguments;
	const uint8_t SeverityMap[] = {
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_SUCCESS,
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_WARNING,
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_ERROR
	};
	char FormattedMessage[USERLOG_MESSAGE_LENGTH + 1];
	ArEventLogRecordIDType Result;
	uint8_t CodedData[USERLOG_CODED_DATA_LENGTH];
	
	/* Saturate severity */
	if(Severity < USERLOG_SEVERITY_DEBUG)
		Severity = USERLOG_SEVERITY_DEBUG;
	else if(Severity > USERLOG_SEVERITY_CRITICAL)
		Severity = USERLOG_SEVERITY_CRITICAL;
	
	/* Suppress */
	if(Severity < SeverityLevel)
		return 0;
	
	/* Call ArEventLogGetIdent and store results */
	IecStringCopy(GetLogbookIdent.Name, sizeof(GetLogbookIdent.Name), Logbook);
	GetLogbookIdent.Execute = true;
	ArEventLogGetIdent(&GetLogbookIdent);
	Status = GetLogbookIdent.StatusID;
	Ident = GetLogbookIdent.Ident;
	GetLogbookIdent.Execute = false;
	ArEventLogGetIdent(&GetLogbookIdent);
	
	/* Error check */
	if(Status) {
		/* Block recursion */
		if(Error)
			return 0;
		Error = true;
		
		/* Log error */
		LocalArguments.i[0] = Status;
		IecStringCopy(LocalArguments.s[0], USERLOG_LOGBOOK_LENGTH + 1, Logbook);
		LocalArguments.i[1] = Code;
		IecStringCopy(LocalArguments.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, Message);
		UserLogCustom(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_IDENT, 0, NULL, 
			"UserLog: ArEventLog error %i. (Logbook \"%s\", code %i, message \"%s\")", &LocalArguments);
		
		return 0;
	}
	
	/* Write to logbook */
	WriteToLogbook.Ident = Ident;
		
	/* Get event ID */
	WriteToLogbook.EventID = ArEventLogMakeEventID(SeverityMap[Severity + 1], Facility, Code);
	
	/* Origin record */
	WriteToLogbook.OriginRecordID = Origin;
	
	/* Format and set message */
	if(Message == NULL)
		IecStringCopy(FormattedMessage, sizeof(FormattedMessage), "UserLog: No message provided");
	else
		IecStringFormat(FormattedMessage, sizeof(FormattedMessage), Message, Values);
	
	if(Coded) {
		ArEventLogAddDataInit((uint32_t)CodedData, sizeof(CodedData), arEVENTLOG_ADDFORMAT_CODED);
		ArEventLogAddDataString((uint32_t)CodedData, sizeof(CodedData), (uint32_t)FormattedMessage);
		WriteToLogbook.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
		WriteToLogbook.AddData = (uint32_t)CodedData;
		WriteToLogbook.AddDataSize = 0;
	}
	else {
		WriteToLogbook.AddDataSize = strlen(FormattedMessage) + 1;
		WriteToLogbook.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
		WriteToLogbook.AddData = (uint32_t)FormattedMessage;
	}
	
	/* Add object name */
	if(Object == NULL)
		ST_name(0, WriteToLogbook.ObjectID, 0);
	else if(*Object == '\0')
		ST_name(0, WriteToLogbook.ObjectID, 0);
	else
		IecStringCopy(WriteToLogbook.ObjectID, sizeof(WriteToLogbook.ObjectID), Object);
	
	/* Write */
	WriteToLogbook.Execute = true;
	ArEventLogWrite(&WriteToLogbook);
	Status = WriteToLogbook.StatusID;
	Result = WriteToLogbook.RecordID;
	WriteToLogbook.Execute = false;
	ArEventLogWrite(&WriteToLogbook);
	
	/* Error check */
	if(Status) {
		/* Block recursion */
		if(Error)
			return 0;
		Error = true;
		
		/* Log error */
		LocalArguments.i[0] = Status;
		IecStringCopy(LocalArguments.s[0], USERLOG_LOGBOOK_LENGTH + 1, Logbook);
		LocalArguments.i[1] = Code;
		IecStringCopy(LocalArguments.s[1], USERLOG_MESSAGE_PREVIEW_LENGTH + 1, Message);
		UserLogCustom(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_WRITE, 0, NULL, 
			"UserLog: ArEventLog error %i. (Logbook \"%s\", code %i, message \"%s\")", &LocalArguments);
		
		return 0;
	}
	
	Error = false;
	return Result;
}

/* Write to any user logbook through binary-encoded data and event texts */
ArEventLogRecordIDType UserLogEventText(char *Logbook, int32_t Event, ArEventLogRecordIDType Origin, 
	char *Object, char *Message, UserLogFormatType *Values) {
	return WriteMessage(Logbook, UserLogGetSeverity(Event), UserLogGetFacility(Event), UserLogGetCode(Event), Origin, Object, Message, Values, true);
}
