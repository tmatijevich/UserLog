/*********************************************************************************
 * File:      CyclicLogBuffer.c
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
BOOL WriteAdminMessage = false; // When the buffer is full, write an admin message. Once emptied, write another admin message.
enum UserLogSeverityEnum SeverityThreshold = USER_LOG_SEVERITY_INFORMATION; // Default to 0

/* Write buffered event entries to the user logbook */
void CyclicLogBuffer(struct CyclicLogBuffer* inst) {

	// Declare and initialize static function blocks for logging
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;
	
	// Declare a static variable to detect a rising edge on the ErrorReset input
	static BOOL ErrorResetPrevious = false;
	
	// Assign the global severity threshold
	SeverityThreshold = inst->SeverityThreshold;
	
	// Store admin message in a local variable
	STRING sAdminMessage[80];
	STRING sMaxEntries[12];

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
				inst->ReturnValue = USER_LOG_ERROR_LOGBOOK_IDENT;
			
				// Go to the ERROR state
				Info.State = USER_LOG_STATE_ERROR;
			}
		
			break;
			
		// IDLE
		case USER_LOG_STATE_IDLE:
			// Write an admin message when the buffer is filled
			if(WriteAdminMessage)
				Info.State = USER_LOG_STATE_ADMIN;
			
			// Wait for new event message (write index incremented in LogMessage)
			else if(Info.ReadIndex != Info.WriteIndex || Info.Full) {
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
			
		// ADMIN
		case USER_LOG_STATE_ADMIN:
			// Prepare the write logbook event function block for an administrative message
			fbWrite.Ident			= Info.UserLogbookIdent;
			fbWrite.OriginRecordID	= 0; // Default 0 if no origin event
			if(WriteAdminMessage) {
				fbWrite.EventID = ArEventLogMakeEventID(USER_LOG_SEVERITY_WARNING, 1, 0); // Use a different facility
				brsstrcpy((UDINT)sAdminMessage, (UDINT)&"Log buffer full. No new events until emptied. Max entries: "); // 59
				brsitoa(USER_LOG_BUFFER_SIZE, (UDINT)sMaxEntries); // Max 12
				brsstrcat((UDINT)sAdminMessage, (UDINT)sMaxEntries); // Concatinate to record the user's buffer size
			}
			else {
				fbWrite.EventID = ArEventLogMakeEventID(USER_LOG_SEVERITY_INFORMATION, 1, 0); // Use a different facility
				brsstrcpy((UDINT)sAdminMessage, (UDINT)&"Log buffer emptied. Accepting new events.");
			}
			fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
			fbWrite.AddDataSize 	= brsstrlen((UDINT)sAdminMessage) + 1;
			fbWrite.AddData 		= (UDINT)sAdminMessage;
			brsstrcpy((UDINT)fbWrite.ObjectID, (UDINT)&"Admin");
			fbWrite.Execute 		= true;
			
			// Go to WRITE
			Info.State = USER_LOG_STATE_WRITE;
			
			break;
			
		// WRITE
		case USER_LOG_STATE_WRITE:
			if(fbWrite.Done) {
				// Reset the function block execution
				fbWrite.Execute = false;
				
				// Increment the number of entries logged
				Info.NumEntriesLogged++;
				
				// Check for the admin flag
				if(Info.Full && WriteAdminMessage){
					// Reset the flag and go to IDLE
					WriteAdminMessage 	= false;
					Info.State			= USER_LOG_STATE_IDLE;
				}
				// Check if the indices match
				else if(Info.ReadIndex == Info.WriteIndex && Info.Full) {
					// Reset the buffer full status and go to ADMIN
					Info.Full 	= false;
					Info.State 	= USER_LOG_STATE_ADMIN;
				}
				else {
					// Return to the IDLE state
					Info.State = USER_LOG_STATE_IDLE;
				}
				
			}
			else if(fbWrite.Error) {
				// Report the error
				inst->ReturnValue = USER_LOG_ERROR_WRITE;
			
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
				
				// Reset the return value
				inst->ReturnValue = USER_LOG_ERROR_NONE;
				
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
	
	else { // Indicies are equal
		// The buffer is full and we are not executing except when writing the first admin message
		if(Info.Full && (!fbWrite.Execute ^ WriteAdminMessage))
			Info.NumEntriesInBuffer = USER_LOG_BUFFER_SIZE;
		else
			Info.NumEntriesInBuffer = 0;
	}
	
	/* Call all function blocks */
	ArEventLogGetIdent(&fbGetIdent);
	ArEventLogWrite(&fbWrite);

}
