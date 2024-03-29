/*******************************************************************************
 * File: custom.c
 * Created: 2020-10-29
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file custom.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Write to any user logbook synchronously */
ArEventLogRecordIDType UserLogCustom(char *Logbook, int32_t Severity, 
                                     uint16_t Facility, uint16_t Code, 
                                     ArEventLogRecordIDType Origin, 
                                     char *Object, char *Message, UserLogFormatType *Values)
{
    /* Saturate severity */
    Severity = MIN(MAX(USERLOG_SEVERITY_DEBUG, Severity),
                       USERLOG_SEVERITY_CRITICAL);

    /* Suppress message */
    if (Severity < severity_level) return 0;

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
        log_values.i[1] = Code;
        UserLogCustom(LOGBOOK_USER_NAME, USERLOG_SEVERITY_ERROR,
                      FACILITY_ERROR, CODE_ERROR_IDENT, 0, NULL,
                      "ArEventLog error %i writing to logbook \"%s\" "
                      "with code %i using UserLog", &log_values);
        error = true;
        return 0;
    }

    /* Write to identified logbook */
    ArEventLogWrite_typ write = {0};
    write.Ident = get_ident.Ident;

    /* Event ID */
    write.EventID = ArEventLogMakeEventID(severity_map[Severity + 1], 
                                          Facility, Code);

    /* Origin record */  
    write.OriginRecordID = Origin;

    /* Additional string data */
    char data[DATA_MESSAGE_SIZE] = {0};
    IecStringFormat(data, sizeof(data), Message, Values);
    write.AddDataSize = strlen(data) + 1;
    write.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
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
        log_values.i[1] = Code;
        UserLogCustom(LOGBOOK_USER_NAME, USERLOG_SEVERITY_ERROR,
                      FACILITY_ERROR, CODE_ERROR_WRITE, 0, NULL,
                      "ArEventLog error %i writing to logbook \"%s\" "
                      "with code %i using UserLog", &log_values);
        error = true;
        return 0;
    }

    /* No error, share record */
    error = false;
    return write.RecordID;
}

/* Write to the User logbook synchronously */
ArEventLogRecordIDType UserLogBasic(int32_t Severity, uint16_t Code, 
                                    char *Message)
{
    return UserLogCustom(LOGBOOK_USER_NAME, Severity, USERLOG_FACILITY, Code, 
                         0, NULL, Message, NULL);
}

/* Write to the User logbook with runtime data */
ArEventLogRecordIDType UserLogAdvanced(int32_t Severity, uint16_t Code, char 
                                       *Message, UserLogFormatType *Values)
{
    return UserLogCustom(LOGBOOK_USER_NAME, Severity, USERLOG_FACILITY, Code, 
                         0, NULL, Message, Values);
}
