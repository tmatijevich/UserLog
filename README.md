# UserLog

UserLog is an Automation Studio library with functions to buffer log messages written to the user logbook. A one-line function call records a new message. A single function block instance manages all function calls, writing all messages recorded within the buffer to the user logbook.

[Download the library here](https://github/com/tmatijevich/UserLog/releases/latest/download/UserLog.zip).

The buffer is scalable and can be sized by the user. When the buffer is filled, the user is notified with an entry in the logbook. Once emptied, the user is notified again. The message passed to the log function is written to the ASCII Data column of the system logger.

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

#### LogMessage()

```C 
typedef enum UserLogSeverityEnum
{   USERLOG_SEVERITY_SUCCESS = 0,
    USERLOG_SEVERITY_INFORMATION = 1,
    USERLOG_SEVERITY_WARNING = 2,
    USERLOG_SEVERITY_ERROR = 3
} UserLogSeverityEnum;

/* Add entry to user logbook event buffer */
signed long LogMessage(enum UserLogSeverityEnum severity, unsigned short code, char *message);
```

### Usage

#### IEC Structured Text

```
/* Write a new message */
LogMessage(severity := USERLOG_SEVERITY_SUCCESS, code := 1234, message := 'Hello World!');

/* Declare fbCyclicLogBuffer and continuously call at the desired rate */
fbCyclicLogBuffer();
```

#### ANSI C 
```C 
/* Write a new message */
LogMessage(USERLOG_SEVERITY_SUCCESS, 1234, "Hello World!");

/* Declare fbCyclicLogBuffer and continuously call at the desired rate */
CyclicLogBuffer(&fbCyclicLogBuffer);
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
    fbCyclicLogBuffer : CyclicLogBuffer;
END_VAR
```

Setup a loop to log `NumOfMessages` messages. Then call `fbCyclicLogBuffer`.

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

fbCyclicLogBuffer();
```

In Automation Studio, connect to the target, and open the logger. The result looks like this.

![2021-09-26_11 35 14](https://user-images.githubusercontent.com/33841634/134816278-53b1bc4c-2bcf-475b-8978-12f787a16dca.png)

By default, `USERLOG_BUFFER_SIZE` is 10.

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

### Recommended

- [`IecString`](https://github.com/tmatijevich/IecString)

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
