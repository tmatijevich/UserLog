# UserLog

[![Made for B&R](https://raw.githubusercontent.com/hilch/BandR-badges/dfd5e264d7d2dd369fd37449605673f779db437d/Made-For-BrAutomation.svg)](https://www.br-automation.com)
![GitHub License](https://img.shields.io/github/license/tmatijevich/UserLog)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/tmatijevich/UserLog/total)
[![GitHub issues](https://img.shields.io/github/issues-raw/tmatijevich/UserLog)](https://github.com/tmatijevich/UserLog/issues)

UserLog is an [Automation Studio](https://www.br-automation.com/en-us/products/software/automation-software/automation-studio/) library for writing to user logbooks synchronously.  
UserLog's simple one-line functions provide an essential **diagnostic tool** for software development.

**NOTE:** This is not an official library and is supported through the community.  UserLog is provided as-is under the [MIT License](https://mit-license.org/) agreement.  Source code, documentation, and issues are managed through [GitHub](https://github.com/tmatijevich/UserLog).

![2023-11-17_15-06-35](https://github.com/tmatijevich/UserLog/assets/33841634/60dd7762-9ecd-48d2-9329-1b7271fbe130)

# Features

- :repeat: Write to logbooks synchronously
- :pencil2: Fully customizable with message, code, and runtime data parameters
- :warning: Suppress messages with adjustable severity level
- :orange_book: Create custom logbooks
- :clipboard: Compatible with text system

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

# Install

1. [Download](https://github.com/tmatijevich/UserLog/releases/latest/download/UserLog.zip) the binaries and extract the archive
2. Add to the project as **existing library** from the toolbox in logical view

# Build

- Clone or download source files
- Add to the project as **existing library**
    - Option to add as **reference**

Alternatively, add as submodule to an existing project repository.

```
cd <repository>
git submodule add https://github.com/tmatijevich/UserLog.git <project>/Logical/Libraries/UserLog
```

Then add to project as **existing library**.

# Unit Tests

See the [UserLogProject](https://github.com/tmatijevich/UserLogProject) for unit tests of all functions.

# Documentation

Once installed, the help file can be viewed using the `F1` function key.

See the [UserLogHelp](https://github.com/tmatijevich/UserLogHelp) for the documentation source code.

### Dependencies

- [ArEventLog](https://help.br-automation.com/#/en/4/libraries%2Fareventlog%2Fareventlog.html)
- [sys_lib](https://help.br-automation.com/#/en/4/libraries%2Fsys_lib%2Fsys_lib.html)
- [astime](https://help.br-automation.com/#/en/4/libraries%2Fastime%2Fastime.html)

### Authors

- Tyler Matijevich ([@tmatijevich](https://github.com/tmatijevich))
