/*******************************************************************************
 * File: UserLog\message.c
 * Author: Tyler Matijevich
 * Created: 2020-10-29
 ******************************************************************************/

#include <UserLog.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint32_t stringCopy(char *destination, char *source, uint32_t size);
static uint32_t stringFormat(char *destination, char *source, UserLogFormatArgumentType *args, uint32_t size);

/* Write to logbook synchronously */
int32_t UserLogMessage(char *logbook, UserLogSeverityEnum severity, uint8_t facility, uint16_t code, ArEventLogRecordIDType origin, char *object, char *message, UserLogFormatArgumentType *args) {
	
	/* Declare local variables */
	ArEventLogGetIdent_typ getLogbookIdent;
	ArEventLogWrite_typ writeToLogbook;
	int32_t status;
	ArEventLogIdentType ident;
	const uint8_t severityMap[] = {
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_ERROR,
		arEVENTLOG_SEVERITY_WARNING,
		arEVENTLOG_SEVERITY_INFO,
		arEVENTLOG_SEVERITY_SUCCESS,
		arEVENTLOG_SEVERITY_INFO
	};
	char asciiMessage[121];
	
	/* Get logbook ident */
	memset(&getLogbookIdent, 0, sizeof(getLogbookIdent));
	stringCopy(getLogbookIdent.Name, logbook, sizeof(getLogbookIdent.Name)); /* Safe copy */
	
	getLogbookIdent.Execute = true;
	ArEventLogGetIdent(&getLogbookIdent);
	
	status = getLogbookIdent.StatusID; /* Store results */
	ident = getLogbookIdent.Ident;
	
	getLogbookIdent.Execute = false;
	ArEventLogGetIdent(&getLogbookIdent);
	
	if(status) return status;
	
	/* Write to logbook */
	memset(&writeToLogbook, 0, sizeof(writeToLogbook));
	writeToLogbook.Ident = ident;
	
	if(severity < USERLOG_SEVERITY_CRITICAL) severity = USERLOG_SEVERITY_CRITICAL;
	else if(severity > USERLOG_SEVERITY_DEBUG) severity = USERLOG_SEVERITY_DEBUG;
	writeToLogbook.EventID = ArEventLogMakeEventID(severityMap[severity], facility, code);
	
	writeToLogbook.OriginRecordID = origin;
	
	memset(asciiMessage, 0, sizeof(asciiMessage));
	stringFormat(asciiMessage, message, args, sizeof(asciiMessage));
	writeToLogbook.AddDataSize = strlen(asciiMessage) + 1;
	writeToLogbook.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
	writeToLogbook.AddData = (uint32_t)asciiMessage;
	
	if(object == NULL) ST_name(0, writeToLogbook.ObjectID, 0);
	else if(*object == '\0') ST_name(0, writeToLogbook.ObjectID, 0);
	else stringCopy(writeToLogbook.ObjectID, object, sizeof(writeToLogbook.ObjectID));
	
	writeToLogbook.Execute = true;
	ArEventLogWrite(&writeToLogbook);
	status = writeToLogbook.StatusID;
	writeToLogbook.Execute = false;
	ArEventLogWrite(&writeToLogbook);
	
	return status;
	
} /* End function */

/* Embedded from IecString library */
/* Format string with runtime data. Returns destination */
uint32_t stringFormat(char *destination, char *source, UserLogFormatArgumentType *args, uint32_t size) {
	
	/* Declare local variables */
	char *src = source, *dst = destination;
	const char sBool[][6] = {"FALSE", "TRUE"}; 	/* Boolean arguments = 5 + null terminator */
	char sNumber[13]; /* Floats: [<+->]1.23456[e<+->12] = 12 + null terminator, Longs: -2147483648 to 2147483647 = 11 + null terminator */
	uint8_t countBool = 0;
	uint8_t countFloat = 0;
	uint8_t countInteger = 0;
	uint8_t countString = 0; 
	uint32_t length, bytesRemaining = size - 1;
	
	/* Verify parameters */
	if(destination == NULL || source == NULL || size == 0) return (uint32_t)destination;
	if(args == NULL) return stringCopy(destination, source, size);
	
	/* Format */
	while(*src != '\0' && bytesRemaining > 0) {
		if(*src != '%') {
			*dst++ = *src++; /* Direct copy */
			bytesRemaining--;
			continue;
		}
		
		*dst = '\0'; /* Temporarily add null terminator to perform concatenation */
		length = 0; /* Set the length to zero if the format specifier is invalid */
		
		switch(*(++src)) {
			case 'b':
				if(countBool <= USERLOG_FORMATARG_INDEX) 
					length = strlen(strncat(dst, sBool[args->b[countBool++]], bytesRemaining));
				break;
			
			 case 'f':
			 	if(countFloat <= USERLOG_FORMATARG_INDEX) {
					brsftoa((float)(args->f[countFloat++]), (uint32_t)sNumber);
					length = strlen(strncat(dst, sNumber, bytesRemaining));
			 	}
			 	break;
			 
			 case 'i':
			 	if(countInteger <= USERLOG_FORMATARG_INDEX) {
					brsitoa(args->i[countInteger++], (uint32_t)sNumber);
					length = strlen(strncat(dst, sNumber, bytesRemaining));
			 	}
			 	break;
			 
			 case 's':
			 	if(countString <= USERLOG_FORMATARG_INDEX) 
					length = strlen(strncat(dst, args->s[countString++], bytesRemaining));
			 	break;
			 
			 case '%':
			 	*dst = '%';
				length = 1;
				break;
		} /* End switch */
		
		dst += length;
		bytesRemaining -= length;
		src++;
		
	} /* End while */
	
	*dst = '\0'; /* Add the null terminator to end the string */
	return (uint32_t)destination;
	
} /* End function */

/* Embedded from IecString library */
/* Copies source to destination up to size (of destination) or source length. Returns destination */
uint32_t stringCopy(char *destination, char *source, uint32_t size) {
	
	/* Declare local variables */
	char *src = source, *dst = destination;
	uint32_t bytesRemaining = size - 1;
	
	/* Verify parameters */
	if(destination == NULL || source == NULL || size == 0) return (uint32_t)destination;
	
	/* Initialize (size = 1) */
	*dst = '\0';
	
	/* Copy */
	while(bytesRemaining) {
		if(*src == '\0') {
			*dst = '\0';
			break;
		}
		*dst++ = *src++;
		bytesRemaining--;
		if(bytesRemaining == 0) *dst = '\0';
	}
	
	return (uint32_t)destination;
	
} /* End function */
