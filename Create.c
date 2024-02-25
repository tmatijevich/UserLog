/*******************************************************************************
 * File: Create.c
 * Created: 2021-12-05
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file Create.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Create a custom logbook in an Init subroutine */
int32_t UserLogCreate(char *Name, uint32_t Size) {
	
	/* Local variables */
	static ArEventLogCreate_typ CreateLogbook;
	int32_t Status;
	UserLogFormatType Arguments;
	
	/* Create */
	IecStringCopy(CreateLogbook.Name, MIN(USERLOG_LOGBOOK_LENGTH + 1, sizeof(CreateLogbook.Name)), Name);
	CreateLogbook.Size = Size;
	CreateLogbook.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
	CreateLogbook.Execute = true;
	do
		ArEventLogCreate(&CreateLogbook);
	while(CreateLogbook.Busy);
	
	Status = CreateLogbook.StatusID;
	
	CreateLogbook.Execute = false;
	ArEventLogCreate(&CreateLogbook);
	
	if(Status && Status != arEVENTLOG_ERR_LOGBOOK_EXISTS) {
		Arguments.i[0] = Status;
		IecStringCopy(Arguments.s[0], USERLOG_LOGBOOK_LENGTH + 1, Name);
		UserLogCustom(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_CREATE, 0, NULL, 
			"UserLog: ArEventLog error %i. Could not create logbook %s.", &Arguments);
	}
	
	return Status;
}
