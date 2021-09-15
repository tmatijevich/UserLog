/*********************************************************************************
 * File:      GetBufferInfo.c
 * Author:    Tyler Matijevich 
 * Created:   November 1, 2020/18:03 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* Get information on the logging buffer. Returns 0. */
signed long GetBufferInfo(struct UserLogBufferInfoType *bufferInfo) {
	memcpy(bufferInfo, &info, sizeof(info));
	return 0;
}
