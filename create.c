/*******************************************************************************
 * File: UserLog\create.c
 * Author: Tyler Matijevich
 * Created: 2021-12-05
 ******************************************************************************/

#include "Main.h"

/* Create custom logbook. Only use in _INIT routine */
int32_t UserLogCreate (char *name, uint32_t size)
{
	/* Declare local variables */
	static ArEventLogCreate_typ create_logbook;
	int32_t status;
	UserLogFormatType args;
	
	/* Create */
	string_copy(create_logbook.Name, MIN(USERLOG_LOGBOOK_LENGTH + 1, sizeof(create_logbook.Name)), name);
	create_logbook.Size = size;
	create_logbook.Execute = true;
	do 
		ArEventLogCreate(&create_logbook);
	while (create_logbook.Busy);
	
	status = create_logbook.StatusID;
	
	create_logbook.Execute = false;
	ArEventLogCreate(&create_logbook);
	
	if (status && status != arEVENTLOG_ERR_LOGBOOK_EXISTS)
	{
		args.i[0] = status;
		string_copy(args.s[0], USERLOG_LOGBOOK_LENGTH + 1, name);
		
		UserLogMessage(USERLOG_USER_LOGBOOK, USERLOG_SEVERITY_ERROR, USERLOG_ERROR_FACILITY, USERLOG_CODE_CREATE, 0, NULL, 
						"UserLog: ArEventLog error %i. Could not create logbook %s.", &args);
	}
	
	return status;
}
