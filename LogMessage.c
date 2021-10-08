/*********************************************************************************
 * File:      LogMessage.c
 * Author:    Tyler Matijevich
 * Created:   September 26, 2021/11:58 
 *********************************************************************************/ 

#include "UserLogMain.h"

static signed long LogAdminMessage(ArEventLogIdentType userLogbookIdent);

/* Declare global variables */
UserLogSeverityEnum severityThreshold;

signed long LogMessage(enum UserLogSeverityEnum severity, unsigned short code, char *message) {
	
	/********************** 
	Declare local variables
	**********************/
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;
	static signed long timeStamp;
	static short cyclicLogCount;
	char asciiMessage[USERLOG_MESSAGE_LENGTH + 1];
	
	/***********************************
	Suppress if below severity threshold
	***********************************/
	if(severity < USERLOG_SEVERITY_SUCCESS || severity > USERLOG_SEVERITY_ERROR)
		return USERLOG_ERROR_SEVERITY;
	else if(severity < severityThreshold) {
		return USERLOG_ERROR_NONE;
	}
	
	/*******************************
	Manage calls per cyclic resource
	*******************************/
	if(timeStamp != AsIOTimeCyclicStart()) 				/* First call this cycle */
		cyclicLogCount = 0; 							/* Reset count */
	if(cyclicLogCount >= USERLOG_MAX_MESSAGES) { 		/* More than max cycles */
		return USERLOG_ERROR_BUFFERFULL;
	}
	else cyclicLogCount++; 								/* Ensure the counter doesn't overrun */
	timeStamp = AsIOTimeCyclicStart();
	
	/**********************
	Find user logbook ident 
	**********************/
	if(fbWrite.Ident == 0) {
		strcpy(fbGetIdent.Name, "$arlogusr");
		fbGetIdent.Execute = true;
		ArEventLogGetIdent(&fbGetIdent);
		
		if(fbGetIdent.Done) { 					/* Success */
			fbWrite.Ident = fbGetIdent.Ident;
			fbGetIdent.Execute = false;
			ArEventLogGetIdent(&fbGetIdent); 	/* Reset */
		}
		else { 									/* Error, or did not complete in one scan */
			fbGetIdent.Execute = false;
			ArEventLogGetIdent(&fbGetIdent); 	/* Reset */
			return USERLOG_ERROR_IDENT;
		}
	}
	
	/**************************
	Get name of cyclic resource
	**************************/
	if(ST_name(0, fbWrite.ObjectID, 0) != ERR_OK)
		strcpy(fbWrite.ObjectID, "Unknown");
	
	/***********************
	Write message to logbook
	***********************/
	/* fbWrite.Ident */ 												/* Ident already assigned */
	fbWrite.EventID 		= ArEventLogMakeEventID(severity, 0, code); /* See AH 32-bit event ID. Use facility 0 for simplicity */
	fbWrite.OriginRecordID 	= 0; 										/* No origin record for simplicity */
	fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT; 				/* ASCII data */
	strncpy(asciiMessage, message, USERLOG_MESSAGE_LENGTH); 			/* Copy up to MESSAGE_LENGTH characters */
	asciiMessage[USERLOG_MESSAGE_LENGTH] = '\0'; 						/* Ensure null terminator if the incoming message exceeds MESSAGE_LENGTH */
	fbWrite.AddDataSize 	= strlen(asciiMessage) + 1; 
	fbWrite.AddData 		= (unsigned long)asciiMessage;
	/* fbWrite.ObjectID */												/* ObjectID already assigned */
	fbWrite.TimeStamp 		= 0; 										/* Null, handled by AR */
	fbWrite.Execute 		= true;
	ArEventLogWrite(&fbWrite);
	
	if(fbWrite.Done) { 				/* Success */
		fbWrite.Execute = false;
		ArEventLogWrite(&fbWrite); 	/* Reset */
		if(cyclicLogCount == USERLOG_MAX_MESSAGES)
			LogAdminMessage(fbWrite.Ident);
		return USERLOG_ERROR_NONE;
	}
	else { 							/* Error, or did not complete in one scan */
		fbWrite.Execute = false;
		ArEventLogWrite(&fbWrite); 	/* Reset */
		return USERLOG_ERROR_WRITE;
	}
	
} /* End function */


/* Log an administrative message when the max messages have been reached per cycle */
signed long LogAdminMessage(ArEventLogIdentType userLogbookIdent) {
	
	/********************** 
	Declare local variables
	**********************/
	static struct ArEventLogWrite fbWrite;
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
	fbWrite.Execute = false;
	ArEventLogWrite(&fbWrite);
	
	return USERLOG_ERROR_NONE;
	
} /* End function */
