# UserLog Changes

## V0.1.3 (*2020-12-02*)
- Implement severity threshold to filter what's written to logbook
- Fix ReturnValue of CyclicLogBuffer
- Update error enumeration naming

## V0.1.2 (*2020-11-25*)
- Display max number of entries in buffer (`USER_LOG_BUFFER_SIZE`) in the adminstrative logging
- Add changelog

## V0.1.1 (*2020-11-18*)
- Add administrative logging when the buffer is full and subsequently emptied

## V0.1.0 (*2020-11-04*)
- **First release**
- Implement `LogEvent()`, `CyclicLogBuffer()`, and `GetBufferInfo()` functions