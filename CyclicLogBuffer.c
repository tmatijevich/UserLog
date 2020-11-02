/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich 
 * Created:   October 21, 2020/21:03 
 *********************************************************************************/ 

// <file> - System header files
// "file" - Program header files
#include <UserLog.h>
#include "UserLogMain.h"

/* Global variables */
UserLogBufferEntryType Buffer[USER_LOG_BUFFER_SIZE];
UserLogBufferInfoType Info;
BOOL ErrorResetPrevious = false; // Use this to detect a rising edge on the ErrorReset inputs

/* Write buffered event entries to the user logbook */
void CyclicLogBuffer(struct CyclicLogBuffer* inst) {

	// Declare and initialize static function blocks for logging
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;

	/* Begin the main switch statement */
	switch(Info.State) {
		// INIT
		case USER_LOG_STATE_INIT:
			// Prepare the get logbook ident function block
			brsstrcpy((UDINT)fbGetIdent.Name, (UDINT)&"$arlogusr");
			fbGetIdent.Execute = true;
			
			// Go to the IDENT state
			Info.State = USER_LOG_STATE_IDENT;
		
			break;
			
		// IDENT
		case USER_LOG_STATE_IDENT:
			// Wait for the function block to complete and record the output
			if(fbGetIdent.Done) {
				// Reset the function block execution
				fbGetIdent.Execute = false;
				
				// Record the user logbook ident
				Info.UserLogbookIdent = fbGetIdent.Ident;
				
				// Go to the IDLE state
				Info.State = USER_LOG_STATE_IDLE;
			}
			else if(fbGetIdent.Error) {
				// Report the error
				inst->ReturnValue = USERLOG_ERROR_LOGBOOK_IDENT;
			
				// Go to the ERROR state
				Info.State = USER_LOG_STATE_ERROR;
			}
		
			break;
			
		// IDLE
		case USER_LOG_STATE_IDLE:
			if(Info.ReadIndex != Info.WriteIndex || Info.Full) {
				// Prepare the write logbook event function block
				fbWrite.Ident 			= Info.UserLogbookIdent;
				fbWrite.EventID 		= ArEventLogMakeEventID(Buffer[Info.ReadIndex].Severity, 0, Buffer[Info.ReadIndex].Code);
				fbWrite.OriginRecordID	= 0; // Default 0 if no origin event
				fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
				fbWrite.AddDataSize 	= brsstrlen((UDINT)Buffer[Info.ReadIndex].sMessage) + 1;
				fbWrite.AddData 		= (UDINT)Buffer[Info.ReadIndex].sMessage;
				brsstrcpy((UDINT)fbWrite.ObjectID, (UDINT)Buffer[Info.ReadIndex].sTaskName);
				fbWrite.Execute 		= true;
				
				// Increment the read index
				Info.ReadIndex = ++Info.ReadIndex % USER_LOG_BUFFER_SIZE;
				
				// Go to the WRITE state
				Info.State = USER_LOG_STATE_WRITE;
			}
		
			break;
			
		// WRITE
		case USER_LOG_STATE_WRITE:
			if(fbWrite.Done) {
				// Reset the function block execution
				fbWrite.Execute = false;
				
				// Increment the number of entries logged
				Info.NumEntriesLogged++;
				
				// Check if the indices match
				if(Info.ReadIndex == Info.WriteIndex && Info.Full) {
					// Reset the buffer full status
					Info.Full = false;
				}
				
				// Return to the IDLE state
				Info.State = USER_LOG_STATE_IDLE;
			}
			else if(fbWrite.Error) {
				// Report the error
				inst->ReturnValue = USERLOG_ERROR_WRITE;
			
				// Go to the ERROR state
				Info.State = USER_LOG_STATE_ERROR;
			}
		
			break;
		case USER_LOG_STATE_ERROR:
			if(inst->ErrorReset && !ErrorResetPrevious) {
				// Reset the buffer
				Info.WriteIndex = 0;
				Info.ReadIndex	= 0;
				Info.Full		= 0;
				
				// Reset the function blocks
				fbGetIdent.Execute	= false;
				fbWrite.Execute		= false;
				
				// Return to the INIT state
				Info.State = USER_LOG_STATE_INIT;
			}
		
			break;
			
	}
	
	/* Update all status information */
	ErrorResetPrevious = inst->ErrorReset; // Used in the ERROR state
	
	// Determine the number of entries in the buffer
	if(Info.WriteIndex > Info.ReadIndex) 
		Info.NumEntriesInBuffer = Info.WriteIndex - Info.ReadIndex;
		
	else if(Info.WriteIndex < Info.ReadIndex) 
		Info.NumEntriesInBuffer = (USER_LOG_BUFFER_SIZE - Info.ReadIndex) + Info.WriteIndex;
	
	else
		Info.NumEntriesInBuffer = 0;
	
	/* Call all function blocks */
	ArEventLogGetIdent(&fbGetIdent);
	ArEventLogWrite(&fbWrite);

}
