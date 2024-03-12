/*******************************************************************************
 * File: create.c
 * Created: 2021-12-05
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file create.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Create a custom logbook in an Init subroutine */
int32_t UserLogCreate(char *Name, uint32_t Size)
{
    /* Create */
    ArEventLogCreate_typ create = {{0}};
    IecStringCopy(create.Name, sizeof(create.Name), Name);
    create.Size = Size;
    create.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
    create.Execute = true;
    do
        ArEventLogCreate(&create);
    while (create.Busy);
    
    if (create.StatusID && create.StatusID != arEVENTLOG_ERR_LOGBOOK_EXISTS)
    {
        /* Log error */
        UserLogFormatType log_values = {{0}};
        log_values.i[0] = create.StatusID;
        IecStringCopy(log_values.s[0], sizeof(log_values.s[0]), Name);
        UserLogCustom(LOGBOOK_USER_NAME, USERLOG_SEVERITY_ERROR, 
                      FACILITY_ERROR, CODE_ERROR_CREATE, 0, NULL, 
                      "ArEventLog error %i attempting to create logbook "
                      "\"%s\" with UserLog", &log_values);
    }

    return create.StatusID;
}
