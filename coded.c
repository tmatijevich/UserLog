/*******************************************************************************
 * File: coded.c
 * Created: 2023-02-03
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file coded.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Write to any user logbook through binary-encoded data and event texts */
ArEventLogRecordIDType UserLogEventText(char *Logbook, int32_t Event, 
                                        ArEventLogRecordIDType Origin, 
                                        char *Object, char *Message, 
                                        UserLogFormatType *Values)
{
    /* Suppress message */
    if (UserLogGetSeverity(Event) < severity_level) return 0;

    /* Get logbook identifier */
    ArEventLogGetIdent_typ get_ident = {{0}};
    IecStringCopy(get_ident.Name, sizeof(get_ident.Name), Logbook);
    get_ident.Execute = true;
    ArEventLogGetIdent(&get_ident);

    /* Check for error */
    static uint8_t error;
    UserLogFormatType log_values = {{0}};
    if (get_ident.StatusID) 
    {
        /* Block infinite recursion */
        if (error) return 0;
        
        /* Log error */
        log_values.i[0] = get_ident.StatusID;
        IecStringCopy(log_values.s[0], sizeof(log_values.s[0]), Logbook);
        log_values.i[1] = Event;
        UserLogCustom(LOGBOOK_USER_NAME, USERLOG_SEVERITY_ERROR,
                      FACILITY_ERROR, CODE_ERROR_IDENT, 0, NULL,
                      "ArEventLog error %i writing to logbook \"%s\" "
                      "with event ID %i using UserLog", &log_values);
        error = true;
        return 0;
    }

    /* Write to identified logbook */
    ArEventLogWrite_typ write = {0};
    write.Ident = get_ident.Ident;

    /* Event ID */
    write.EventID = Event;

    /* Additional coded data */
    char data[DATA_CODED_SIZE] = {0};
    char string_data[DATA_MESSAGE_SIZE] = {0};
    ArEventLogAddDataInit((uint32_t)data, sizeof(data), 
                          arEVENTLOG_ADDFORMAT_CODED);
    IecStringFormat(string_data, sizeof(string_data), Message, Values);
    ArEventLogAddDataString((uint32_t)data, sizeof(data),
                            (uint32_t)string_data);
    write.AddDataSize = 0;
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_CODED;
    write.AddData = (uint32_t)data;

    /* Object name */
    if (!Object || !*Object)
        ST_name(0, write.ObjectID, 0);
    else
        IecStringCopy(write.ObjectID, sizeof(write.ObjectID), Object);

    /* Write to logbook */
    write.Execute = true;
    ArEventLogWrite(&write);

    /* Check for error */
    if (write.StatusID)
    {
        /* Block infinite recursion */
        if (error) return 0;

        /* Log error */
        log_values.i[0] = write.StatusID;
        IecStringCopy(log_values.s[0], sizeof(log_values.s[0]), Logbook);
        log_values.i[1] = Event;
        UserLogCustom(LOGBOOK_USER_NAME, USERLOG_SEVERITY_ERROR,
                      FACILITY_ERROR, CODE_ERROR_WRITE, 0, NULL,
                      "ArEventLog error %i writing to logbook \"%s\" "
                      "with event ID %i using UserLog", &log_values);
        error = true;
        return 0;
    }

    /* No error, share record */
    error = false;
    return write.RecordID;
}
