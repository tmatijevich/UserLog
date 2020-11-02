/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich 
 * Created:   October 21, 2020/21:03 
 *********************************************************************************/ 

// <file> - System header files
// "file" - Program header files
#include <UserLog.h>
#include "UserLogMain.h"

struct UserLogBufferEntryType Buffer[USER_LOG_BUFFER_SIZE];
struct UserLogBufferInfoType Info;

/*  */
void CyclicUserLogBuffer(struct CyclicUserLogBuffer* inst) {

	// Declare and initialize static function blocks for logging
	static struct ArEventLogGetIdent fbGetIdent;
	static struct ArEventLogWrite fbWrite;

	/* Begin the main switch statement */
	switch(Info.State) {
		// INIT
		case 0:
			// Prepare the get logbook ident function block
			brsstrcpy((UDINT)&fbGetIdent.Name, (UDINT)&"$arlogusr");
			fbGetIdent.Execute = true;
			
			// Go to the IDENT state
			Info.State = 10;
		
			break;
			
		// IDENT
		case 10:
			// Wait for the function block to complete and record the output
			if(fbGetIdent.Done) {
				// Reset the function block execution
				fbGetIdent.Execute = false;
				
				// Record the user logbook ident
				Info.UserLogbookIdent = fbGetIdent.Ident;
				
				// Go to the IDLE state
				Info.State = 20;
			}
			else if(fbGetIdent.Error) {
				// Go to the ERROR state
				Info.State = 255;
			}
		
			break;
			
		// IDLE
		case 20:
			if(Info.ReadIndex != Info.WriteIndex || Info.Full) {
				// Prepare the write logbook event function block
				fbWrite.Ident 			= Info.UserLogbookIdent;
				fbWrite.EventID 		= ArEventLogMakeEventID(Buffer[Info.ReadIndex].Severity, 0, Buffer[Info.ReadIndex].Code);
				fbWrite.OriginRecordID	= 0; // Default 0 if no origin event
				fbWrite.AddDataFormat 	= arEVENTLOG_ADDFORMAT_TEXT;
				fbWrite.AddDataSize 	= brsstrlen((UDINT)&Buffer[Info.ReadIndex].sMessage) + 1;
				fbWrite.AddData 		= (UDINT)&Buffer[Info.ReadIndex].sMessage;
				brsstrcpy((UDINT)&fbWrite.ObjectID, (UDINT)&Buffer[Info.ReadIndex].sTaskName);
				fbWrite.Execute 		= true;
				
				// Increment the read index
				Info.ReadIndex = ++Info.ReadIndex % USER_LOG_BUFFER_SIZE;
				
				// Go to the WRITE state
				Info.State = 30;
			}
		
			break;
			
		// WRITE
		case 30:
			if(fbWrite.Done) {
				// Reset the function block execution
				fbWrite.Execute = false;
				
				// Check if the indices match
				if(Info.ReadIndex == Info.WriteIndex && Info.Full) {
					// Reset the buffer full status
					Info.Full = false;
				}
				
				// Return to the IDLE state
				Info.State = 20;
			}
			else if(fbWrite.Error) {
				// Go to the ERROR state
				Info.State = 255;
			}
		
			break;
		case 255:
			break;
			
	}
	
	/* Call all function blocks */
	ArEventLogGetIdent((UDINT)&fbGetIdent);
	ArEventLogWrite((UDINT)&fbWrite);

}
