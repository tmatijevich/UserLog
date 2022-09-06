/*******************************************************************************
 * File: UserLog\message.c
 * Author: Tyler Matijevich
 * Created: 2020-10-29
 ******************************************************************************/

#include "main.h"

/* Write to logbook synchronously */
int32_t UserLogMessage(char *logbook, UserLogSeverityEnum severity, uint8_t facility, uint16_t code, ArEventLogRecordIDType origin, char *object, char *message, UserLogFormatArgumentType *args) {
	
	/* Declare local variables */
	ArEventLogGetIdent_typ getLogbookIdent;
	ArEventLogWrite_typ writeToLogbook;
	int32_t status;
	ArEventLogIdentType ident;
	const uint8_t severityMap[] = {
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_WARNING,
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_SUCCESS,
		arEVENTLOG_SEVERITY_INFO
	};
	char asciiMessage[121];
	
	/* Get logbook ident */
	memset(&getLogbookIdent, 0, sizeof(getLogbookIdent));
	stringCopy(getLogbookIdent.Name, logbook, sizeof(getLogbookIdent.Name)); /* Safe copy */
	
	getLogbookIdent.Execute = true;
	ArEventLogGetIdent(&getLogbookIdent);
	
	status = getLogbookIdent.StatusID; /* Store results */
	ident = getLogbookIdent.Ident;
	
	getLogbookIdent.Execute = false;
	ArEventLogGetIdent(&getLogbookIdent);
	
	if(status) return status;
	
	/* Write to logbook */
	memset(&writeToLogbook, 0, sizeof(writeToLogbook));
	writeToLogbook.Ident = ident;
	
	if(severity < USERLOG_SEVERITY_CRITICAL) severity = USERLOG_SEVERITY_CRITICAL;
	else if(severity > USERLOG_SEVERITY_DEBUG) severity = USERLOG_SEVERITY_DEBUG;
	writeToLogbook.EventID = ArEventLogMakeEventID(severityMap[severity], facility, code);
	
	writeToLogbook.OriginRecordID = origin;
	
	memset(asciiMessage, 0, sizeof(asciiMessage));
	if(message == NULL) stringCopy(asciiMessage, "UserLog: No message provided", sizeof(asciiMessage));
	else stringFormat(asciiMessage, message, args, sizeof(asciiMessage));
	writeToLogbook.AddDataSize = strlen(asciiMessage) + 1;
	writeToLogbook.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
	writeToLogbook.AddData = (uint32_t)asciiMessage;
	
	if(object == NULL) ST_name(0, writeToLogbook.ObjectID, 0);
	else if(*object == '\0') ST_name(0, writeToLogbook.ObjectID, 0);
	else stringCopy(writeToLogbook.ObjectID, object, sizeof(writeToLogbook.ObjectID));
	
	writeToLogbook.Execute = true;
	ArEventLogWrite(&writeToLogbook);
	status = writeToLogbook.StatusID;
	writeToLogbook.Execute = false;
	ArEventLogWrite(&writeToLogbook);
	
	return status;
	
} /* End function */
