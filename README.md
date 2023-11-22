# UserLog [![Made for B&R](https://raw.githubusercontent.com/hilch/BandR-badges/dfd5e264d7d2dd369fd37449605673f779db437d/Made-For-BrAutomation.svg)](https://www.br-automation.com)

UserLog is an [Automation Studio](https://www.br-automation.com/en-us/products/software/automation-software/automation-studio/) library with functions for writing to user logbooks synchronously.  UserLog's simple one-line functions provide a necessary tool for software development and debug.

![2023-11-17_15-06-35](https://github.com/tmatijevich/UserLog/assets/33841634/60dd7762-9ecd-48d2-9329-1b7271fbe130)

**NOTE:** This is not an official library and is supported through the community.  UserLog is provided as-is under the [MIT License](https://mit-license.org/) agreement.  Source code, documentation, and issues are managed through [GitHub](https://github.com/tmatijevich/UserLog).

## Installation

1. [Download](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip) and extract the archive.
2. Import the UserLog package to your Automation Studio project's Logical View using **Existing Library** from the toolbox.

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

## Build

Clone to a reference location.

```bash
git clone https://github.com/tmatijevich/UserLog.git
```

Or, add as a submodule to your project's libraries.

```bash
cd <project_root>
git submodule add https://github.com/tmatijevich/UserLog.git <project_root>/Logical/Libraries/UserLog
```

Then add as an **Existing Library** in Logical View from the Automation Studio toolbox.

### Warnings

![Build warnings 2022-03-31_12 34 35](https://user-images.githubusercontent.com/33841634/161134955-5e71050f-bd1b-49cf-b07c-6408ae3c24ca.png)

In Automation Studio 4.11+,

1. Configuration View
2. Right-click the CPU object and select Properties
3. Navigate to the Build tab
4. Add the following to the **Objects ignored for build warnings 9232 and 9233** field

```
*README*;*LICENSE*;.git;.gitignore;.github
```

Prior to Automation Studio 4.11,

1. Configuration View
2. Right-click the CPU object and select Properties
3. Navigate to the Build tab
4. Add the following to the **Additional build options** field

```
-W 9232 9233
```

## Dependencies

- ArEventLog
- AsBrStr
- sys_lib

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
