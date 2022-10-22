# UserLog

UserLog is an Automation Studio library with functions for writing to logbooks synchronously.  UserLog's simple one-line functions provide a necessary tool for software development and debug.    
**NOTE:** This is not an official library. UserLog is provided as is under the GNU GPL v3.0 license agreement.  

## Installation

1. [Download](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip)
2. Extract the archive
3. Import to your Automation Studio project's Logical View using *Existing Library* from the toolbox

## Features

- Synchronously write to user logbooks
	- Timestamp is recorded at the time of the function call
- Parameters
	- Event ID
		- Severity: Debug, Success, Info, Warning, Error, Critical
			- **NOTE:** Debug and Critical are extentions of the existing ArEventLog severities
		- Facility 0..4095
		- Code 0..65535
	- Origin record for hierarchy
	- Object name
	- Message
	- Format message with runtime data (%b, %f, %i, %s)
- Automatically read software object name
- Set severity level and suppress messages
- Error messages written to User logbook
- String truncation to protect overflow
- Create custom logbooks

## Functions

Function | Description
---|---
[UserLogQuick](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L7) | Write to user logbook. Returns record ID if successful, zero otherwise
[UserLogFormat](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L15) | Write to user logbook with runtime data.  Returns record ID if successful, zero otherwise
[UserLogMessage](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L24) | Write to logbook synchronously. Returns record ID if successful, zero otherwise
[UserLogCreate](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L37) | Create custom logbook. Only use in _INIT routine
[UserLogSetSeverityLevel](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L44) | Set severity level. Suppresses messages below level
[UserLogGetSeverity](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L50) | Get ArEventLog severity from event ID
[UserLogGetFacility](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L56) | Get ArEventLog facility from event ID
[UserLogGetCode](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L62) | Get ArEventLog code from event ID

## Clone

Clone to a reference location.

```
git clone https://github.com/tmatijevich/UserLog.git
```

Or, add as a submodule to your project's libraries.

```
cd <project_root>
git submodule add https://github.com/tmatijevich/UserLog.git <project_root>/Logical/Libraries/UserLog
```

Then add as an existing library from the Automation Studio toolbox.

## Build 

Building a project with this package may result in warnings for additional files.  

![Build warnings 2022-03-31_12 34 35](https://user-images.githubusercontent.com/33841634/161134955-5e71050f-bd1b-49cf-b07c-6408ae3c24ca.png)

In Automation Studio 4.11+, it is possible to add specific filters to warnings 9232 and 9233.  Navigate to Configuration View, right-click the PLC object and select properties, chose the Build tab, and add the follow text to the "Objects ignored for build warnings 9232 and 9233" field. The filters are case sensitive.

```
*README*;*LICENSE*;.git;.gitignore;.github
```

Prior to Automation Studio 4.11, it is possible to suppress *all* build warnings regarding additional files by using `-W 9232 9233` in the "Additional build options" field.

## Dependencies

- `ArEventLog`
- `AsBrStr`
- `sys_lib`

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
