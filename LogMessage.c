/*********************************************************************************
 * File:      LogMessage.c
 * Author:    Tyler Matijevich
 * Created:   October 31, 2020/14:42 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Add an event to the logging FIFO buffer to be written to the User logbook. */
signed long LogMessage(enum UserLogSeverityEnum severity, unsigned short code, char *message){

	/* Local variable declaration */
	unsigned short status; 
	
	if(info.full) { /* The entry cannot be added to the buffer */
		info.lostCount++;
		return USERLOG_ERROR_BUFFERFULL;
	}
	
	/* Check for severity in range and within threshold */
	if(severity <= 3) {
		if(severity >= severityThreshold)
			buffer[info.writeIndex].severity = severity;
		else {
			info.suppressedCount++; /* Subceeds threshold, entry suppressed */
			return USERLOG_ERROR_NONE;
		}
	} 
	else { /* Invalid severity */
		info.lostCount++;
		return USERLOG_ERROR_SEVERITY;
	}
	
	/* Copy task name to buffer, return if error */
	status = ST_name(0, buffer[info.writeIndex].task, 0);
	if(status != ERR_OK) {
		info.sysLibStatus = status;
		info.lostCount++;
		return USERLOG_ERROR_TASKNAME;
	}
	
	/* Set the code of the buffer entry */
	buffer[info.writeIndex].code = code;
	
	/* Copy the message to the buffer entry */
	strncpy(buffer[info.writeIndex].message, message, USERLOG_MESSAGE_LENGTH); /* Copy up to MESSAGE_LENGTH characters */
	memset(buffer[info.writeIndex].message + USERLOG_MESSAGE_LENGTH, 0, 1); /* Ensure null terminator if the incoming message exceeds MESSAGE_LENGTH */
	
	/* Increment the write index */
	info.writeIndex = ++info.writeIndex % USERLOG_BUFFER_SIZE;
	
	/* Check if the buffer is now full */
	if(info.writeIndex == info.readIndex) {
		/* Mark the buffer as full, no new entries until it is empty again */
		info.full 	= true;
		promptFull	= true;
	}
	
	return USERLOG_ERROR_NONE;
}
