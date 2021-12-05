/*********************************************************************************
 * File:      LogMessage.c
 * Author:    Tyler Matijevich
 * Created:   September 26, 2021/11:58 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Function prototypes */
static signed long LogAdminMessage(ArEventLogIdentType userLogbookIdent);

/* Declare global variables */
UserLogSeverityEnum verbosityLevel = USERLOG_SEVERITY_SUCCESS; /* All but debug */
UserLogInfoType info;
unsigned char severityMap[] = {3, 3, 2, 1, 0, 1}; /* 0 - Success, 1 - Information, 2 - Warning, 3 - Error */

/* Write message (event) to user logbook */
signed long LogMessage(UserLogSeverityEnum severity, unsigned short code, char *message) {
	return CustomMessage(severity, code, message, "$arlogusr", 0);
}

/* Write message (event) to custom logbook */
signed long CustomMessage(UserLogSeverityEnum severity, unsigned short code, char *message, char *logbook, unsigned char facility) {
	
	/********************** 
	Declare local variables
	**********************/
	static ArEventLogGetIdent_typ fbGetIdent;
	static ArEventLogWrite_typ fbWrite;
	static signed long timeStamp;
	static short cyclicLogCount;
	char asciiMessage[USERLOG_MESSAGE_LENGTH + 1];
	signed long arEventLogStatusID;
	
	/***********************************
	Suppress if below severity threshold
	***********************************/
	if(severity > verbosityLevel) { 					/* From SetVerbosityLevel, never greater than USERLOG_SEVERITY_DEBUG */
		info.suppressedCount++;
		return 0;
	}
	
	/*******************************
	Manage calls per cyclic resource
	*******************************/
	if(timeStamp != AsIOTimeCyclicStart()) 				/* First call this cycle */
		cyclicLogCount = 0; 							/* Reset count */
	if(cyclicLogCount >= USERLOG_MAX_MESSAGES) { 		/* More than max cycles */
		info.lostCount++;
		return 0;
	}
	else cyclicLogCount++; 								/* Ensure the counter doesn't overrun */
	timeStamp = AsIOTimeCyclicStart();
	
	/**********************
	Find user logbook ident 
	**********************/
	strncpy(fbGetIdent.Name, logbook, 10);
	fbGetIdent.Name[10] = '\0';
	fbGetIdent.Execute 	= true;
	ArEventLogGetIdent(&fbGetIdent);
	
	if(fbGetIdent.Done) { 								/* Success */
		fbWrite.Ident = fbGetIdent.Ident;
		fbGetIdent.Execute = false;
		ArEventLogGetIdent(&fbGetIdent); 				/* Reset */
	}
	else { 												/* Error, or did not complete in one scan */
		info.lostCount++;
		arEventLogStatusID = fbGetIdent.StatusID;
		fbGetIdent.Execute = false;
		ArEventLogGetIdent(&fbGetIdent); 				/* Reset */
		if(arEventLogStatusID != 0) 
			return arEventLogStatusID;
		else
			return -1;
	}
	
	/**************************
	Get name of cyclic resource
	**************************/
	if(ST_name(0, fbWrite.ObjectID, 0) != ERR_OK)
		strcpy(fbWrite.ObjectID, "Unknown");
	
	/***********************
	Write message to logbook
	***********************/
	/* fbWrite.Ident */ 																	/* Ident already assigned */
	fbWrite.EventID 		= ArEventLogMakeEventID(severityMap[severity], facility, code); /* See AH 32-bit event ID. Use facility 0 for simplicity */
	fbWrite.OriginRecordID 	= 0; 															/* No origin record for simplicity */
	fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT; 									/* ASCII data */
	strncpy(asciiMessage, message, USERLOG_MESSAGE_LENGTH); 								/* Copy up to MESSAGE_LENGTH characters */
	asciiMessage[USERLOG_MESSAGE_LENGTH] = '\0'; 											/* Ensure null terminator if the incoming message exceeds MESSAGE_LENGTH */
	fbWrite.AddDataSize 	= strlen(asciiMessage) + 1; 
	fbWrite.AddData 		= (unsigned long)asciiMessage;
	/* fbWrite.ObjectID */																	/* ObjectID already assigned */
	fbWrite.TimeStamp 		= 0; 															/* Null, handled by AR */
	fbWrite.Execute 		= true;
	ArEventLogWrite(&fbWrite);
	
	if(fbWrite.Done) { 									/* Success */
		info.loggedCount++;
		fbWrite.Execute = false;
		ArEventLogWrite(&fbWrite); 						/* Reset */
		if(cyclicLogCount == USERLOG_MAX_MESSAGES)
			LogAdminMessage(fbWrite.Ident);
		return 0;
	}
	else { 												/* Error, or did not complete in one scan */
		info.lostCount++;
		arEventLogStatusID 		= fbGetIdent.StatusID;
		fbWrite.Execute 		= false;
		ArEventLogWrite(&fbWrite); 						/* Reset */
		if(arEventLogStatusID != 0)
			return arEventLogStatusID;
		else
			return -1;
	}
	
} /* End function */


/* Log an administrative message when the max messages have been reached per cycle */
signed long LogAdminMessage(ArEventLogIdentType userLogbookIdent) {
	
	/********************** 
	Declare local variables
	**********************/
	static ArEventLogWrite_typ fbWrite;
	char asciiMessage[USERLOG_MESSAGE_LENGTH + 1];
	char bufferSizeText[12];
	
	/**************
	Prepare message
	**************/
	strcpy(asciiMessage, "Max number of messages ");
	brsitoa(USERLOG_MAX_MESSAGES, (unsigned long)bufferSizeText);
	strcat(asciiMessage, bufferSizeText);
	strcat(asciiMessage, " reached, since start of cycle");
	
	/***********************
	Write message to logbook
	***********************/
	fbWrite.Ident 			= userLogbookIdent;
	fbWrite.EventID 		= ArEventLogMakeEventID(USERLOG_SEVERITY_WARNING, 1, 0);
	fbWrite.OriginRecordID 	= 0;
	fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
	fbWrite.AddDataSize 	= strlen(asciiMessage) + 1;
	fbWrite.AddData 		= (unsigned long)asciiMessage;
	strcpy(fbWrite.ObjectID, "Admin");
	fbWrite.TimeStamp 		= 0;
	fbWrite.Execute 		= true;
	ArEventLogWrite(&fbWrite);
	
	/*********
	Reset
	*********/
	if(fbWrite.Done) info.loggedCount++;
	fbWrite.Execute = false;
	ArEventLogWrite(&fbWrite);
	
	return 0;
	
} /* End function */
