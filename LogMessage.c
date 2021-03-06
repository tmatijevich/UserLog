/*********************************************************************************
 * File:      LogMessage.c
 * Author:    Tyler Matijevich
 * Created:   2020-10-29
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Function prototypes */
static long LogAdminMessage(ArEventLogIdentType userLogbookIdent);

/* Declare global variables */
UserLogSeverityEnum verbosityLevel = USERLOG_SEVERITY_SUCCESS; /* All but debug */
UserLogInfoType info;
unsigned char severityMap[] = {
  arEVENTLOG_SEVERITY_ERROR,   /* 0: USERLOG_SEVERITY_CRITICAL */
  arEVENTLOG_SEVERITY_ERROR,   /* 1: USERLOG_SEVERITY_ERROR */ 
  arEVENTLOG_SEVERITY_WARNING, /* 2: USERLOG_SEVERITY_WARNING */ 
  arEVENTLOG_SEVERITY_INFO,    /* 3: USERLOG_SEVERITY_INFORMATION */ 
  arEVENTLOG_SEVERITY_SUCCESS, /* 4: USERLOG_SEVERITY_SUCCESS */ 
  arEVENTLOG_SEVERITY_INFO,    /* 5: USERLOG_SEVERITY_DEBUG */
}; 

/* Write message (event) to user logbook */
long LogMessage(UserLogSeverityEnum severity, unsigned short code, char *message) {
	return CustomFormatMessage(severity, code, message, NULL, "$arlogusr", 0);
}

/* Write formatted message to user logbook */
long LogFormatMessage(UserLogSeverityEnum severity, unsigned short code, char *message, FormatStringArgumentsType *args) {
	return CustomFormatMessage(severity, code, message, args, "$arlogusr", 0);
}

/* Write message (event) to custom logbook */
long CustomMessage(UserLogSeverityEnum severity, unsigned short code, char *message, char *logbook, unsigned char facility) {
	return CustomFormatMessage(severity, code, message, NULL, logbook, facility);
}

/* Write formatted message to customer logbook */
long CustomFormatMessage(UserLogSeverityEnum severity, unsigned short code, char *message, FormatStringArgumentsType *args, char *logbook, unsigned char facility) {
	
	/********************** 
	Declare local variables
	**********************/
	static ArEventLogGetIdent_typ fbGetIdent;
	static ArEventLogWrite_typ fbWrite;
	static long timeStamp;
	static short cyclicLogCount;
	char asciiMessage[USERLOG_STRLEN_MESSAGE + 1];
	long arEventLogStatusID;
	
	/******************
	Guard null pointers
	******************/
	if(message == NULL || logbook == NULL)
		return -1;
	
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
	if(cyclicLogCount >= USERLOG_MESSAGE_MAXCOUNT) { 	/* More than max cycles */
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
		strcpy(fbWrite.ObjectID, "Unknown"); 			/* Up to 36 characters */
	
	/***********************
	Write message to logbook
	***********************/
	/* fbWrite.Ident */ 																	/* Ident already assigned */
	fbWrite.EventID 		= ArEventLogMakeEventID(severityMap[severity], facility, code); /* See AH 32-bit event ID. Use facility 0 for simplicity */
	fbWrite.OriginRecordID 	= 0; 															/* No origin record for simplicity */
	fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT; 									/* ASCII data */
	if(args == NULL) {
		strncpy(asciiMessage, message, USERLOG_STRLEN_MESSAGE); 							/* Copy message */
		asciiMessage[USERLOG_STRLEN_MESSAGE] = '\0';
	}
	else
		IecFormatString(asciiMessage, sizeof(asciiMessage), message, args); 				/* Format message */
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
		if(cyclicLogCount == USERLOG_MESSAGE_MAXCOUNT)
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
long LogAdminMessage(ArEventLogIdentType userLogbookIdent) {
	
	/********************** 
	Declare local variables
	**********************/
	static ArEventLogWrite_typ fbWrite;
	char asciiMessage[USERLOG_STRLEN_MESSAGE + 1];
	FormatStringArgumentsType args;
	
	/**************
	Prepare message
	**************/
	args.i[0] = USERLOG_MESSAGE_MAXCOUNT;
	IecFormatString(asciiMessage, sizeof(asciiMessage), "Max numbers of messages, %i, reached since start of cycle", &args);
	
	/***********************
	Write message to logbook
	***********************/
	fbWrite.Ident 			= userLogbookIdent;
	fbWrite.EventID 		= ArEventLogMakeEventID(USERLOG_SEVERITY_WARNING, 1, 0);
	fbWrite.OriginRecordID 	= 0;
	fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
	fbWrite.AddDataSize 	= strlen(asciiMessage) + 1;
	fbWrite.AddData 		= (unsigned long)asciiMessage;
	strcpy(fbWrite.ObjectID, "Admin"); /* Up to 36 characters */
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
