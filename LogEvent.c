/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    Tyler Matijevich
 * Created:   October 31, 2020/14:42 
 *********************************************************************************/ 
 
#include <UserLog.h> // Include the automatically generated header file from the library
#include "UserLogMain.h" // Include local header file for internal declarations

extern UserLogBufferEntryType Buffer[USER_LOG_BUFFER_SIZE];
extern UserLogBufferInfoType Info;

/* Add an event to the logging FIFO buffer. Then the buffer will write entries to the user logbook */
UDINT LogEvent(enum UserLogSeverityEnum Severity, UINT Code, plcstring* sMessage){

	// Check that the buffer is not already full
	if(Info.Full) {
		Info.NumEntriesLost++;
		// The event entry cannot be added to the buffer
		return USERLOG_ERROR_BUFFER_FULL;
	}
	
	// Copy over the task name, return if error
	if(ST_name(0, &Buffer[Info.WriteIndex].sTaskName, 0) != 0) {
		return USERLOG_ERROR_TASK_NAME;
	}
	
	// Check that the severity level is within ArEventLog options
	if(Severity <= 3) {
		// Set the severity of the buffer entry
		Buffer[Info.WriteIndex].Severity = Severity;
	} else {
		return USERLOG_ERROR_INVALID_SEVERITY;
	}
	
	// Set the code of the buffer entry
	Buffer[Info.WriteIndex].Code = Code;
	
	// Copy the message string into the buffer
	brsstrcpy((UDINT)&Buffer[Info.WriteIndex].sMessage, (UDINT)sMessage);
	
	// Increment the write index
	Info.WriteIndex = ++Info.WriteIndex % USER_LOG_BUFFER_SIZE;
	
	// Check if the buffer is now full
	if(Info.WriteIndex == Info.ReadIndex) {
		// Mark the buffer as full, no new entries until it is empty again
		Info.Full = true;
	}
	
	return USERLOG_ERROR_NONE;
}