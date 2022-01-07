/*********************************************************************************
 * File:      CreateCustomLogbook.c
 * Author:    Tyler Matijevich
 * Created:   December 5, 2021/11:55 AM 
 *********************************************************************************/ 

#include "UserLogMain.h"

/* _INIT routine ONLY! Create custom logbook in USERROM (if not already existing) */
signed long CreateCustomLogbook(char *name, unsigned long size) {
	
	/********************** 
	Declare local variables
	**********************/
	static ArEventLogGetIdent_typ fbGetIdent;
	static ArEventLogCreate_typ fbCreate;
	signed long arEventLogStatusID;
	
	/****************
	Check for logbook
	****************/
	strncpy(fbGetIdent.Name, name, 10);
	fbGetIdent.Name[10] = '\0';
	fbGetIdent.Execute 	= true;
	ArEventLogGetIdent(&fbGetIdent);
	
	if(fbGetIdent.Done) { 								/* Success */
		fbGetIdent.Execute = false;
		ArEventLogGetIdent(&fbGetIdent); 				/* Reset */
		return 0; 										/* Logbook exists */
	}
	else if(fbGetIdent.StatusID == arEVENTLOG_ERR_LOGBOOK_NOT_FOUND) {
		fbGetIdent.Execute = false;
		ArEventLogGetIdent(&fbGetIdent); 				/* Reset and continue */
	}
	else { 												/* Unknown error, or did not complete in one scan */
		arEventLogStatusID = fbGetIdent.StatusID;
		fbGetIdent.Execute = false;
		ArEventLogGetIdent(&fbGetIdent); 				/* Reset */
		if(arEventLogStatusID != 0) 
			return arEventLogStatusID;
		else
			return -1;
	}
	
	/*************
	Create logbook
	*************/
	fbCreate.Execute 		= true;
	strncpy(fbCreate.Name, name, 10);
	fbCreate.Name[10] 		= '\0';
	fbCreate.Size 			= size;
	fbCreate.Persistence 	= arEVENTLOG_PERSISTENCE_PERSIST;
	fbCreate.Ident 			= 0;
	
	ArEventLogCreate(&fbCreate);
	while(fbCreate.Busy)
		ArEventLogCreate(&fbCreate); 				/* Continue to call, _INIT only! */
	if(fbCreate.Done) 
		return 0; 									/* Success */
	else { 											/* Error */
		arEventLogStatusID 	= fbCreate.StatusID; 	/* Record */
		fbCreate.Execute 	= false;
		ArEventLogCreate(&fbCreate); 				/* Reset */
		if(arEventLogStatusID != 0) 
			return arEventLogStatusID;
		else
			return -1;
	}
	
} /* End function */
