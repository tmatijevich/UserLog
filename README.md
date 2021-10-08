# UserLog

UserLog is an Automation Studio library with functions to quickly log messages to the user logbook.

**Feaures**:

- Write to the user logbook by providing
	- Severity (Success, Information, Warning, Error)
	- Code (0 - 65535)
	- Message text
- Set the severity threshold to suppress messages 
- Retreive information on the logging history

[Download the library here](https://github/com/tmatijevich/UserLog/releases/latest/download/UserLog.zip).

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

#### [LogMessage](https://github.com/tmatijevich/UserLog/blob/main/LogMessage.c?ts=4)()

```C 
typedef enum UserLogSeverityEnum
{   USERLOG_SEVERITY_SUCCESS = 0,
    USERLOG_SEVERITY_INFORMATION = 1,
    USERLOG_SEVERITY_WARNING = 2,
    USERLOG_SEVERITY_ERROR = 3
} UserLogSeverityEnum;

/* Write message to user logbook */
signed long LogMessage(enum UserLogSeverityEnum severity, unsigned short code, char *message);
```

### Usage

#### IEC Structured Text

``` 
(* Write a new message *)
LogMessage(severity := USERLOG_SEVERITY_SUCCESS, code := 1234, message := 'Hello World!');
```

#### ANSI C 
```C
/* Write a new message */
LogMessage(USERLOG_SEVERITY_SUCCESS, 1234, "Hello World!");
```

#### Log Info

Retrieve logging information. 
```
(* Get logging information during runtime *)
GetLogInfo(LogInfo);
```

Example
```
LogInfo
  loggedCount        128
  lostCount            8
  suppressedCount    551
  arEventLogStatusID   0
```

### Sample

Write a series of messages to the user logbook in Structured Text with the help of the [IecString library](https://github.com/tmatijevich/IecString).

Declare local variables.

```
VAR
    Run : BOOL;
    FormatArgs : FormatStringArgumentsType;
    MyMessage : STRING[80];
    MySeverity : UserLogSeverityEnum;
    MyCode : UINT;
    Index : USINT;
    NumOfMessages : USINT := 12;
    LogInfo : UserLogInfoType;
    SeverityThreshold : UserLogSeverityEnum;
END_VAR
```

Setup a loop to log `NumOfMessages` messages.

```
IF Run THEN
    Run := FALSE;
    FOR Index := 1 TO NumOfMessages DO
        FormatArgs.i[0] := Index;
        FormatArgs.i[1] := NumOfMessages;
        IecFormatString(MyMessage, SIZEOF(MyMessage), 'Event %i of %i', FormatArgs);
        
        MySeverity := (Index - 1) MOD 4;
        MyCode := 1000 + Index;
        LogMessage(MySeverity, MyCode, MyMessage);
    END_FOR
END_IF

SetThreshold(SeverityThreshold);
GetLogInfo(LogInfo);
```

In Automation Studio, connect to the target, and open the logger. The result looks like this.

![UserLog sample 2021-10-07_09 49 18](https://user-images.githubusercontent.com/33841634/136633532-f9bfb8b7-e399-4919-ac75-0d778910f490.png)

By default, `USERLOG_MAX_MESSAGES` is 10.

## Automation Studio

Version 4.9.3.144 SP

### Build

You may notice build warnings such as "Additional directory/file found ..." from the IecString submodule.

![Build warnings project independent](https://user-images.githubusercontent.com/33841634/133009811-98cf2414-ec89-40d3-a529-34980b59e27f.png)

Add `-W 9232 9233` to your CPU's build properties window under *Additional build options* to suppress.

## Dependencies

- `ArEventLog`
- `AsBrStr`
- `sys_lib`
- `AsIOTime`

### Recommended

- [`IecString`](https://github.com/tmatijevich/IecString)

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
