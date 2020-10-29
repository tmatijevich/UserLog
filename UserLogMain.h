/*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
 * Author:    matijevict 
 * Created:   October 21, 2020/21:31 
 *********************************************************************************/ 

 #ifndef __USER_LOG_MAIN_H__
 #define __USER_LOG_MAIN_H__
 
 // Defined in the ArEventLogWrite() function
 #define OBJECT_ID_STR_LEN 36
 
 // Reference global structures
 
 typedef UserLogFifoBufferEntryType {
 	BOOL Valid;
	DINT EventID;
	STRING ObjectID[OBJECT_ID_STR_LEN];
	STRING Message[USER_LOG_MESSAGE_SIZE];
 } UserLogFifoBufferEntryType;
 
 #endif