# UserLog [![Made For B&R](https://github.com/hilch/BandR-badges/blob/main/Made-For-BrAutomation.svg)](https://www.br-automation.com)

UserLog is an [Automation Studio](https://www.br-automation.com/en-us/products/software/automation-software/automation-studio/) library with functions for writing to logbooks synchronously.  UserLog's simple one-line functions provide a necessary tool for software development and debug.

**NOTE:** This is not an official library. UserLog is provided as is under the GNU GPL v3.0 license agreement.  Support for this library is provided through the [GitHub Issues List](https://github.com/tmatijevich/UserLog/issues).

## Installation

1. [Download](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip) and extract the archive
2. Import the UserLog package to your Automation Studio project's Logical View using *Existing Library* from the toolbox

## Features

- Synchronously write to user logbooks
	- Timestamp is recorded at the time of the function call
- Parameters
	- Event ID
		- Severity: Debug, Success, Info, Warning, Error, Critical
			- **NOTE:** Debug and Critical are extentions of ArEventLog severities
		- Facility 0..4095
		- Code 0..65535
	- Origin record ID for hierarchy
	- Object name
	- Message
	- Format message with runtime data (%b, %f, %i, %s)
- Automatically read software object name
- Set severity level to suppress messages
- Error messages written to the User logbook
- String truncation to protect overflow
- Create custom logbooks
- Compatible with text system

## Functions

Function | Description
---|---
[UserLogBasic](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L2) | Write to the User logbook synchronously
[UserLogAdvanced](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L10) | Write to the User logbook with runtime data
[UserLogCustom](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L19) | Write to any user logbook synchronously
[UserLogEventText](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L32) | Write to any user logbook through binary-encoded data and event texts
[UserLogCreate](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L43) | Create a custom logbook in an Init subroutine
[UserLogSetSeverityLevel](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L50) | Suppress messages below the input level and return previous level
[UserLogGetSeverity](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L56) | Get ArEventLog severity from an event ID
[UserLogGetFacility](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L62) | Get ArEventLog facility from an event ID
[UserLogGetCode](https://github.com/tmatijevich/UserLog/blob/main/UserLog.fun?ts=4#L68) | Get ArEventLog code from an event ID

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
