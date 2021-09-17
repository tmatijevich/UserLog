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

/* Write buffered event entries to user logbook */
void CyclicLogBuffer(struct CyclicLogBuffer *inst) {

	/* Local variables */
	/* Declare and initialize static function blocks for logging */
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite[USERLOG_BUFFER_SIZE];
	static struct ArEventLogWrite fbWriteAdmin;
	
	static unsigned char previousReset = false; /* Detect rising edge on error reset */
	static char adminMessage[81]; /* Store admin message */
	static char bufferSizeText[12]; /* Store buffer size text in admin message */
	unsigned char i, nextWrite;
	
	/* Assign global threshold, read by LogMessage() */
	severityThreshold = inst->SeverityThreshold;

	/*********************
	 Operation
	*********************/
	switch(info.state) {
		/* IDENT */
		case USERLOG_STATE_IDENT:
			/* Execute ArEventLogGetIdent() for the user logbook */
			strcpy(fbGetIdent.Name, "$arlogusr"); /* Up to 256 characters */
			fbGetIdent.Execute = true;
			ArEventLogGetIdent(&fbGetIdent);
			
			/* Handle get ident response */
			if(fbGetIdent.Done) {
				fbGetIdent.Execute 	= false; 
				info.ident 			= fbGetIdent.Ident; /* Record user logbook ident */
				info.state 			= USERLOG_STATE_IDLE; /* Get ready to accept entries */
			}
			else if(fbGetIdent.Error) {
				fbGetIdent.Execute 		= false;
				inst->ErrorID 			= USERLOG_ERROR_IDENT; /* Return error code */
				info.arEventLogStatusID = fbGetIdent.StatusID; /* Record status id */
				info.full 				= true; /* Prevent more entries to buffer */
				info.state 				= USERLOG_STATE_ERROR; /* Send to error state */
			}
		
			break;
			
		/* IDLE */
		case USERLOG_STATE_IDLE:
			for(i = 0; i < USERLOG_BUFFER_SIZE; i++) {
				/* Write an admin message as soon as the buffer is filled */
				if(promptFull) {
					if(!fbWriteAdmin.Execute) {
						fbWriteAdmin.Ident 					= info.ident;
						fbWriteAdmin.OriginRecordID 		= 0; /* Force no origin event for simplicity */
						fbWriteAdmin.EventID 				= ArEventLogMakeEventID(USERLOG_SEVERITY_WARNING, 1, 0); /* Use facility 1 */
						strcpy(adminMessage, "Log buffer full. No new events until emptied. Max entries: "); /* 59 characters */
						brsitoa(USERLOG_BUFFER_SIZE, (unsigned long)bufferSizeText); /* Max 11 characters */
						strcat(adminMessage, bufferSizeText);
						fbWriteAdmin.AddDataFormat 			= arEVENTLOG_ADDFORMAT_TEXT;
						fbWriteAdmin.AddDataSize 			= strlen(adminMessage) + 1;
						fbWriteAdmin.AddData 				= (unsigned long)adminMessage;
						strcpy(fbWriteAdmin.ObjectID, "Admin");
						fbWriteAdmin.Execute 				= true;
					}
					break; /* Loop */
				}
				
				/* Write another admin message after the buffer has been emptied */
				else if(promptEmpty) {
					if(!fbWriteAdmin.Execute) {
						fbWriteAdmin.Ident 					= info.ident;
						fbWriteAdmin.OriginRecordID 		= 0; /* Force no origin event for simplicity */
						fbWriteAdmin.EventID 				= ArEventLogMakeEventID(USERLOG_SEVERITY_WARNING, 1, 0); /* Use facility 1 */
						strcpy(adminMessage, "Log buffer emptied. Accepting new events.");
						fbWriteAdmin.AddDataFormat 			= arEVENTLOG_ADDFORMAT_TEXT;
						fbWriteAdmin.AddDataSize 			= strlen(adminMessage) + 1;
						fbWriteAdmin.AddData 				= (unsigned long)adminMessage;
						strcpy(fbWriteAdmin.ObjectID, "Admin");
						fbWriteAdmin.Execute 				= true;
					}
					break; /* Loop */
				}
				
				/* Wait for new message, writeIndex incremented in LogMessage()
				   If the buffer has just been filled, readIndex == writeIndex */
				else if((info.readIndex != info.writeIndex || info.full) && !fbWrite[nextWrite].Execute) {
					/* Prepare and execute the next ArEventLogWrite() */
					fbWrite[nextWrite].Ident 			= info.ident;
					fbWrite[nextWrite].EventID 			= ArEventLogMakeEventID(buffer[info.readIndex].severity, 0, buffer[info.readIndex].code); /* Force facility 0 for simplicity */
					fbWrite[nextWrite].OriginRecordID	= 0; /* Force no origin event for simplicity */
					fbWrite[nextWrite].AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT; /* ASCII data */
					fbWrite[nextWrite].AddDataSize 		= USERLOG_MESSAGE_LENGTH + 1;
					fbWrite[nextWrite].AddData 			= (unsigned long)buffer[info.readIndex].message;
					strcpy(fbWrite[nextWrite].ObjectID, buffer[info.readIndex].task);
					fbWrite[nextWrite].Execute 			= true;
					
					/* Update indices */
					info.readIndex = ++info.readIndex % USERLOG_BUFFER_SIZE;
					nextWrite == ++nextWrite % USERLOG_BUFFER_SIZE;
					
					/* Check if emptied */
					if(info.readIndex == info.writeIndex && info.bufferCount)
						promptEmpty = true;
				}
				else break; /* Loop */
			} /* End loop */
			break;
			
		/* ERROR */
		case USERLOG_STATE_ERROR:
			if(inst->ErrorReset && !previousReset) {
				/* Reset the buffer */
				info.writeIndex = 0;
				info.readIndex 	= 0;
				info.full 		= 0;
				
				/* Clear executes */
				for(i = 0; i < USERLOG_BUFFER_SIZE; i++) fbWrite[i].Execute = false;
				fbWriteAdmin.Execute = false;
				
				/* Reset and go to INIT */
				inst->ErrorID 	= USERLOG_ERROR_NONE;
				info.state 		= USERLOG_STATE_IDENT;
			}
			
			break;
	} /* End switch */
	
	/* Run administrative write */
	ArEventLogWrite(&fbWriteAdmin);
	if(fbWriteAdmin.Done) {
		fbWriteAdmin.Execute 	= false;
		ArEventLogWrite(&fbWriteAdmin); /* Re-run to reset */
		promptFull 				= false;
		if(promptEmpty) info.full = false;
		promptEmpty 			= false;
		info.loggedCount++;
	}
	else if(fbWriteAdmin.Error) {
		fbWriteAdmin.Execute 	= false;
		inst->ErrorID 			= USERLOG_ERROR_WRITE;
		info.arEventLogStatusID = fbWriteAdmin.StatusID;
		info.full 				= true; /* Prevent more entries to buffer */
		info.state 				= USERLOG_STATE_ERROR;
	}
	
	/* Run user writes */
	for(i = 0; i < USERLOG_BUFFER_SIZE; i++) {
		ArEventLogWrite(&fbWrite[i]);
		if(fbWrite[i].Done) {
			fbWrite[i].Execute = false;
			ArEventLogWrite(&fbWrite[i]); /* Re-run to reset */
			info.loggedCount++;
		}
		else if(fbWrite[i].Error) {
			fbWrite[i].Execute 		= false;
			inst->ErrorID 			= USERLOG_ERROR_WRITE;
			info.arEventLogStatusID = fbWrite[i].StatusID;
			info.full 				= true; /* Prevent more entries to buffer */
			info.state 				= USERLOG_STATE_ERROR;
			/* Continue checking other write blocks */
		}
	}
	
	previousReset = inst->ErrorReset; /* Update for next scan */
	
	/* Count the entries in the buffer */
	if(info.writeIndex > info.readIndex) info.bufferCount = info.writeIndex - info.readIndex;
	else if(info.writeIndex < info.readIndex) info.bufferCount = (USERLOG_BUFFER_SIZE - info.readIndex) + info.writeIndex;
	else {
		if(info.full) info.bufferCount = USERLOG_BUFFER_SIZE;
		else info.bufferCount = 0;
	}

} /* End function block */
