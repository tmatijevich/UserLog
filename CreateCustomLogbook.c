/*********************************************************************************
 * File:      CreateCustomLogbook.c
 * Author:    Tyler Matijevich
 * Created:   December 5, 2021/11:55 AM 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* _INIT routine ONLY! Create custom logbook in USERROM (if not already existing) */
long CreateCustomLogbook(char *name, unsigned long size) {
	
	/********************** 
	Declare local variables
	**********************/
	ArEventLogCreate_typ fbCreate;
	
	/******************
	Guard null pointers
	******************/
	if(name == NULL)
		return -1;
	
	/***********
	Clear memory
	***********/
	memset(&fbCreate, 0, sizeof(fbCreate));
	
	/*************
	Create logbook
	*************/
	fbCreate.Execute 		= true;
	strncpy(fbCreate.Name, name, 10);
	fbCreate.Name[10] 		= '\0';
	fbCreate.Size 			= size;
	fbCreate.Persistence 	= arEVENTLOG_PERSISTENCE_PERSIST;
	fbCreate.Info 			= 0;
	
	ArEventLogCreate(&fbCreate);
	while(fbCreate.Busy)
		ArEventLogCreate(&fbCreate); /* Continue to call, _INIT only! */
	
	return fbCreate.StatusID;
	
} /* End function */
