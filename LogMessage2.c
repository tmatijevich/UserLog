/*********************************************************************************
 * File:      LogMessage2.c
 * Author:    Tyler Matijevich
 * Created:   September 26, 2021/11:58 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Declare global variables */
/* UserLogSeverityEnum severityThreshold; */

signed long LogMessage2(enum UserLogSeverityEnum severity, unsigned short code, char *message) {
	
	/* Declare local variables */
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;
	static signed long timeStamp;
	static short cyclicLogCount;
	char asciiMessage[USERLOG_MESSAGE_LENGTH + 1];
	
	/***********************************
	Suppress if below severity threshold
	***********************************/
	if(severity < USERLOG_SEVERITY_SUCCESS || severity > USERLOG_SEVERITY_SUPPRESS)
		return USERLOG_ERROR_SEVERITY;
	else if(severity < severityThreshold) {
		return USERLOG_ERROR_NONE;
	}
	
	/*******************************
	Manage calls per cyclic resource
	*******************************/
	if(timeStamp != AsIOTimeCyclicStart()) 				/* First call this cycle */
		cyclicLogCount = 0; 							/* Reset count */
	else if(cyclicLogCount >= USERLOG_MAX_MESSAGES) { 	/* More than max cycles */
		return USERLOG_ERROR_BUFFERFULL;
	}
	cyclicLogCount++;
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
		return USERLOG_ERROR_NONE;
	}
	else { 							/* Error, or did not complete in one scan */
		fbWrite.Execute = false;
		ArEventLogWrite(&fbWrite); 	/* Reset */
		return USERLOG_ERROR_WRITE;
	}
	
} /* End function */
