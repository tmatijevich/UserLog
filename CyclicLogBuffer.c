/*********************************************************************************
 * File:      CyclicLogBuffer.c
 * Author:    Tyler Matijevich 
 * Created:   October 21, 2020/21:03 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Declare global variables */
UserLogBufferEntryType buffer[USERLOG_BUFFER_SIZE];
UserLogBufferInfoType info;
UserLogSeverityEnum severityThreshold;
unsigned char promptFull;
unsigned char promptEmpty;

/* Write buffered event entries to the User logbook */
void CyclicLogBuffer(struct CyclicLogBuffer *inst) {

	/* Local variables */
	/* Declare and initialize static function blocks for logging */
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;
	
	static unsigned char previousReset = false; /* Detect rising edge on error reset */
	static char adminMessage[81]; /* Store admin message */
	static char bufferSizeText[12]; /* Store buffer size text in admin message */
	
	/* Assign global threshold, read by LogMessage() */
	severityThreshold = inst->severityThreshold;

	/*********************
	 Operation
	*********************/
	switch(info.state) {
		/* IDENT */
		case USERLOG_STATE_IDENT:
			/* Execute ArEventLogGetIdent() for the User logbook */
			strcpy(fbGetIdent.Name, "$arlogusr"); /* Up to 256 characters */
			fbGetIdent.Execute = true;
			
			/* Handle get ident response */
			if(fbGetIdent.Done) {
				fbGetIdent.Execute 	= false; 
				info.ident 			= fbGetIdent.Ident; /* Record User logbook ident */
				info.state 			= USERLOG_STATE_IDLE; /* Get ready to accept messages */
			}
			else if(fbGetIdent.Error) {
				fbGetIdent.Execute 		= false;
				inst->status 			= USERLOG_ERROR_IDENT; /* Return error code */
				info.arEventLogStatusID = fbGetIdent.StatusID; /* Record status id */
				info.full 				= true; /* Prevent more entries to buffer */
				info.state 				= USERLOG_STATE_ERROR; /* Send to error state */
			}
		
			break;
			
		/* IDLE */
		case USERLOG_STATE_IDLE:
			/* Write an admin message as soon as the buffer is filled */
			if(promptFull)
				info.state = USERLOG_STATE_ADMIN;
			
			/* Wait for new message, writeIndex incremented in LogMessage()
			   If the buffer has just been filled, readIndex == writeIndex */
			else if(info.readIndex != info.writeIndex || info.full) {
				/* Prepare and execute ArEventLogWrite() */
				fbWrite.Ident 			= info.ident;
				fbWrite.EventID 		= ArEventLogMakeEventID(buffer[info.readIndex].severity, 0, buffer[info.readIndex].code); /* Force facility 0 for simplicity */
				fbWrite.OriginRecordID	= 0; /* Force no origin event for simplicity */
				fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT; /* ASC II data */
				fbWrite.AddDataSize 	= USERLOG_MESSAGE_LENGTH + 1;
				fbWrite.AddData 		= (unsigned long)buffer[info.readIndex].message;
				strcpy(fbWrite.ObjectID, buffer[info.readIndex].task);
				fbWrite.Execute 		= true;
				
				/* Update read index */
				info.readIndex = ++info.readIndex % USERLOG_BUFFER_SIZE;
				
				info.state = USERLOG_STATE_WRITE;
			}
		
			break;
			
		/* ADMIN */
		case USERLOG_STATE_ADMIN:
			/* Prepare and execute ArEventLogWrite() for an administrative message */
			fbWrite.Ident			= info.ident;
			fbWrite.OriginRecordID	= 0; /* Force no origin event for simplicity */
			if(promptFull) {
				fbWrite.EventID = ArEventLogMakeEventID(USERLOG_SEVERITY_WARNING, 1, 0); /* Use facility 1 */
				strcpy(adminMessage, "Log buffer full. No new events until emptied. Max entries: "); /* 59 characters */
				brsitoa(USERLOG_BUFFER_SIZE, (unsigned long)bufferSizeText); /* Max 11 characters */
				strcat(adminMessage, bufferSizeText);
			}
			else { /* promptEmpty */
				fbWrite.EventID = ArEventLogMakeEventID(USERLOG_SEVERITY_INFORMATION, 1, 0); /* Use facility 1 */
				strcpy(adminMessage, "Log buffer emptied. Accepting new events.");
			}
			fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
			fbWrite.AddDataSize 	= strlen(adminMessage) + 1;
			fbWrite.AddData 		= (unsigned long)adminMessage;
			strcpy(fbWrite.ObjectID, "Admin");
			fbWrite.Execute 		= true;
			
			info.state = USERLOG_STATE_WRITE;
			
			break;
			
		/* WRITE */
		case USERLOG_STATE_WRITE:
			if(fbWrite.Done) {
				fbWrite.Execute = false;
				info.loggedCount++; 
				
				/* Check for full prompt */
				if(info.full && promptFull) {
					promptFull = false;
					info.state = USERLOG_STATE_IDLE;
				}
				/* Check for empty prompt */
				else if(info.full && promptEmpty) {
					promptEmpty = false;
					info.full 	= false; /* Reset buffer full status for new messages */
					info.state 	= USERLOG_STATE_IDLE;
				}
				/* Check if buffer has emptied */
				else if(info.readIndex == info.writeIndex && info.full) {
					promptEmpty = true;
					info.state = USERLOG_STATE_ADMIN;
				}
				else { /* Normal message */
					info.state = USERLOG_STATE_IDLE;
				}
				
			}
			else if(fbWrite.Error) {
				fbWrite.Execute 		= false;
				inst->status 			= USERLOG_ERROR_WRITE;
				info.arEventLogStatusID = fbWrite.StatusID;
				info.full 				= true; /* Prevent more entries to buffer */
				info.state 				= USERLOG_STATE_ERROR;
			}
		
			break;
			
		/* ERROR */
		case USERLOG_STATE_ERROR:
			if(inst->reset && !previousReset) {
				/* Reset the buffer */
				info.writeIndex = 0;
				info.readIndex 	= 0;
				info.full 		= 0;
				
				/* Reset and go to INIT */
				inst->status 	= USERLOG_ERROR_NONE;
				info.state 		= USERLOG_STATE_IDENT;
			}
		
			break;
	}
	
	previousReset = inst->reset; /* Update for next scan */
	
	/* Count the entries in the buffer */
	if(info.writeIndex > info.readIndex) info.bufferCount = info.writeIndex - info.readIndex;
	else if(info.writeIndex < info.readIndex) info.bufferCount = (USERLOG_BUFFER_SIZE - info.readIndex) - info.writeIndex;
	else {
		if(info.full && !promptEmpty) info.bufferCount = USERLOG_BUFFER_SIZE;
		else info.bufferCount = 0;
	}
	
	/* Call function blocks */
	ArEventLogGetIdent(&fbGetIdent);
	ArEventLogWrite(&fbWrite);

} /* End function block */
