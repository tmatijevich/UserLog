# UserLog

UserLog is an Automation Studio library with functions to quickly log messages to the user logbook.  
**NOTE:** This is not an official library. UserLog is provided as is under the GNU GPL v3.0 license agreement.  
[Download the library here](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip). 

#### Features

- Write to the user logbook with information:
	- Severity (Critical, Error, Warning, Information, Success, Debug)
	- Code (0 - 65535)
	- Formatted message
- Set the verbosity level (Default: Success) to suppress messages
- Retrieve library's logging history
- Create and write to custom logbooks (user responsible for enough memory)

#### Functions

- [LogMessage](#logmessage)
- [SetVerbosityLevel](#setverbositylevel)
- [GetUserLogInfo](#getuserloginfo)
- [CreateCustomLogbook](#createcustomlogbook)
- [CustomMessage](#custommessage)

## Clone

Clone to a reference location

```
git clone https://github.com/tmatijevich/UserLog.git
```

Or, add as a submodule to your project's libraries

```
cd <project_root>
git submodule add https://github.com/tmatijevich/UserLog.git <apj_root>/Logical/Libraries/UserLog
```

Then add as an existing library with the Automation Studio toolbox.

## Functions

### [LogMessage](https://github.com/tmatijevich/UserLog/blob/main/LogMessage.c?ts=4)

```C 
typedef enum UserLogSeverityEnum
{   USERLOG_SEVERITY_CRITICAL = 0,
    USERLOG_SEVERITY_ERROR = 1,
    USERLOG_SEVERITY_WARNING = 2,
    USERLOG_SEVERITY_INFORMATION = 3,
    USERLOG_SEVERITY_SUCCESS = 4,
    USERLOG_SEVERITY_DEBUG = 5
} UserLogSeverityEnum;

/* Write message (event) to user logbook */
long LogMessage(UserLogSeverityEnum severity, unsigned short code, char *message);

/* Write formatted message to user logbook */
long LogFormatMessage(UserLogSeverityEnum severity, unsigned short code, char *message, FormatStringArgumentsType *args);
```

#### Usage

##### IEC Structured Text

``` 
(* Write a new message *)
LogMessage(severity := USERLOG_SEVERITY_SUCCESS, code := 1234, message := 'Hello World!');

(* Or *)
MyArgs.i[0] := 4;
LogFormatMessage(severity := USERLOG_SEVERITY_ERROR, code := 5678, message := 'Error in state %i', args := MyArgs);
```

##### ANSI C 
```C
/* Write a new message */
LogMessage(USERLOG_SEVERITY_SUCCESS, 1234, "Hello World!");

/* Or */
formatArgs.i[0] = 4;
LogFormatMessage(USERLOG_SEVERITY_ERROR, 5678, "Error in state %i", &formatArgs);
```

### [SetVerbosityLevel](https://github.com/tmatijevich/UserLog/blob/main/SetVerbosityLevel.c?ts=4)

```C
/* Set the verbosity level to suppress high verbose messages */
long SetVebosityLevel(UserLogSeverityEnum level);
```

### [GetUserLogInfo](https://github.com/tmatijevich/UserLog/blob/main/GetUserLogInfo.c?ts=4)

```C
/* Gather information on this library's logging history */
long GetUserLogInfo(UserLogInfoType *logInfo);
```

Example

```
LogInfo
  loggedCount        128
  lostCount            8
  suppressedCount    551
```

### [CreateCustomLogbook](https://github.com/tmatijevich/UserLog/blob/main/CreateCustomLogbook.c?ts=4)

**IMPORTANT: For use in _INIT routine only!** The function asynchronously creates the new logbook (if it does not already exist), this can cause cycle time violations in _CYCLIC routines.

```C
/* _INIT routine ONLY! Create custom logbook in USERROM (if not already existing) */
long CreateCustomLogbook(char *name, unsigned long size);
```

### [CustomMessage](https://github.com/tmatijevich/UserLog/blob/main/LogMessage.c?ts=4)

Same as [LogMessage](#logmessage) with additional parameters for logbook name and facility number. Use the facility number (0-15) to differentiate the code area of available event IDs.

```C
/* Write message (event) to custom logbook */
long CustomMessage(UserLogSeverityEnum severity, unsigned short code, char *message, char *logbook, unsigned char facility);

/* Write formatted message to customer logbook */
long CustomFormatMessage(UserLogSeverityEnum severity, unsigned short code, char *message, FormatStringArgumentsType *args, char *logbook, unsigned char facility);
```

### Sample

Declare local variables.

```
VAR
    Run : BOOL;
    FormatArgs : FormatStringArgumentsType;
    MySeverity : UserLogSeverityEnum;
    MyCode : UINT;
    Index : USINT;
    NumOfMessages : USINT := 24;
    LogInfo : UserLogInfoType;
    MyVerbosityLevel : UserLogSeverityEnum := USERLOG_SEVERITY_DEBUG;
END_VAR
```

Write a series of messages to the user logbook.

```
// Write a burst of events to the user logbook
IF Run THEN
    Run := FALSE;
    FOR Index := 1 TO NumOfMessages DO
        // Setup
        SetVebosityLevel(MyVerbosityLevel);
        MySeverity := (Index - 1) MOD 6;
        MyCode := 1000 + Index;
        // Format
        FormatArgs.i[0] := Index;
        FormatArgs.i[1] := NumOfMessages;
        CASE MySeverity OF
            USERLOG_SEVERITY_CRITICAL:      FormatArgs.s[0] := 'Critical';
            USERLOG_SEVERITY_ERROR:         FormatArgs.s[0] := 'Error';
            USERLOG_SEVERITY_WARNING:       FormatArgs.s[0] := 'Warning';
            USERLOG_SEVERITY_INFORMATION:   FormatArgs.s[0] := 'Infomration';
            USERLOG_SEVERITY_SUCCESS:       FormatArgs.s[0] := 'Success';
            USERLOG_SEVERITY_DEBUG:         FormatArgs.s[0] := 'Debug';
        END_CASE
        // Log
        LogFormatMessage(MySeverity, MyCode, 'Event: %i of %i Severity: %s', FormatArgs);
    END_FOR
END_IF

GetUserLogInfo(LogInfo);
```

In Automation Studio, connect to the target and open the logger. The result looks like this.

![2021-11-06_18 59 52](https://user-images.githubusercontent.com/33841634/140626835-f11d83f0-fa00-4d29-ac9a-8e0c019fc800.png)

By default, `USERLOG_MESSAGE_MAXCOUNT` is 20.

## Build

You may notice build warnings such as "Additional directory/file found ..." from the IecString submodule.

![Build warnings project independent](https://user-images.githubusercontent.com/33841634/133009811-98cf2414-ec89-40d3-a529-34980b59e27f.png)

Add `-W 9232 9233` to your CPU's build properties window under *Additional build options* to suppress.

## Dependencies

- `ArEventLog`
- `AsBrStr`
- `sys_lib`
- `AsIOTime`
- [`IecString`](https://github.com/tmatijevich/IecString#readme) >= 0.3.0

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
