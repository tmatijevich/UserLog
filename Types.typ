
TYPE
	UserLogFifoBufferInfoType : 	STRUCT  (*Information about the log buffer writing entries to the user logbook*)
		Active : BOOL;
		State : USINT;
		LogbookIdent : ArEventLogIdentType;
		FifoBufferEntrySize : UDINT;
		FifoBufferSize : UDINT;
		AddEntryIndex : USINT;
		RemoveEntryIndex : USINT;
		NumEntriesInBuffer : USINT;
		NumEntriesLogged : UDINT;
		NumEntriesLost : UDINT;
	END_STRUCT;
END_TYPE
