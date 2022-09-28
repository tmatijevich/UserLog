# UserLog

UserLog is an Automation Studio library with functions for writing to logbooks synchronously.  UserLog's simple one-line functions provide a necessary tool for software development and debug.    
**NOTE:** This is not an official library. UserLog is provided as is under the GNU GPL v3.0 license agreement.  
[Download the library here](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip).  

## Features

- Write to user or custom logbooks
- Parameters:
	- Event ID
		- Severity (Debug, Success, Info, Warning, Error, Critical)
		- Facility 0..4095
		- Code 0..65535
	- Original record hierarchy (optional)
	- Object name (optional)
	- Message up to 120 characters
	- Format data (%b, %f, %i, %s)
- Automatically retrieve software object name for record data
- Set severity level to suppress messages
- Create custom logbooks

## Functions

- UserLogQuick (Write to user logbook)
	- severity
	- code
	- message
- UserLogFormat (Write to user logbook with runtime data)
	- severity
	- code
	- message
	- args
- UserLogMessage (Write to custom logbook with optional parameters)
	- logbook
	- severity
	- facility
	- code
	- origin
	- object
	- message
	- args
- UserLogSeverity (Set severity level)
	- level 
- UserLogCreate (Create custom logbook - `_INIT` routine only)

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

You may notice build warnings such as "Additional directory/file found ..." from the submodule.

![Build warnings project independent](https://user-images.githubusercontent.com/33841634/133009811-98cf2414-ec89-40d3-a529-34980b59e27f.png)

Add `-W 9232 9233` to your CPU's build properties window under *Additional build options* to suppress.

## Dependencies

- `ArEventLog`
- `AsBrStr`
- `sys_lib`

## Git

- [Install and setup Git](https://tmatijevich.github.io/gfw-tutorial/)
- [Setup gitignore for your repository](https://gist.github.com/tmatijevich/453436f1e6abc62a3d052d9b03f9db58)

## Authors

- [tmatijevich](https://github.com/tmatijevich)
