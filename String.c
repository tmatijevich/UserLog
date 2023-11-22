/*******************************************************************************
 * File: String.c
 * Created: 2022-08-30
 * 
 * Authors: 
 *   Tyler Matijevich
 * 
 * License:
 *   This file String.c is part of the UserLog project 
 *   released under the MIT license agreement.
 ******************************************************************************/

#include "Main.h"

/* Format string with runtime data */
void StringFormat(char *Destination, uint32_t Size, char *Source, UserLogFormatType *Values) {
	
	/* Local variables */
	const char Boolean[][6] = {"FALSE", "TRUE"};
	
	/* Floats: [<+->]1.23456[e<+->12] = 12 + null terminator */
	/* Longs: -2147483648 to 2147483647 = 11 + null terminator */
	char Number[13]; 
	
	uint8_t CountBool = 0, CountFloat = 0, CountInteger = 0, CountString = 0;
	uint32_t Length, BytesRemaining = Size - 1;
	
	/* Verify parameters, attempt to copy if failed */
	if(Destination == NULL || Size == 0 || Source == NULL || Values == NULL)
		return StringCopy(Destination, Size, Source);
	
	/* Format */
	while(*Source != '\0' && BytesRemaining) {
		
		if(*Source != '%') {
			/* Direct copy */
			*Destination++ = *Source++;
			BytesRemaining--;
			continue;
		}
		
		/* Temporarily add null terminator for concatenation */
		*Destination = '\0';
		
		/* Reset length if invalid match */
		Length = 0;
		
		/* Match */
		switch(*(++Source)) {
			/* Use strncat to concatenate the formatted value up to BytesRemaining */
			/* strlen(strncat(Destination, ...)) returns the length of characters appended because Destination began as null */
			case 'b':
				if(CountBool <= USERLOG_FORMAT_INDEX)
					Length = strlen(strncat(Destination, Boolean[Values->b[CountBool++]], BytesRemaining));
				break;
			
			 case 'f':
			 	if(CountFloat <= USERLOG_FORMAT_INDEX) {
					brsftoa((float)(Values->f[CountFloat++]), (uint32_t)Number);
					Length = strlen(strncat(Destination, Number, BytesRemaining));
			 	}
			 	break;
			 
			 case 'i':
			 	if(CountInteger <= USERLOG_FORMAT_INDEX) {
					brsitoa(Values->i[CountInteger++], (uint32_t)Number);
					Length = strlen(strncat(Destination, Number, BytesRemaining));
			 	}
			 	break;
			 
			 case 's':
			 	if(CountString <= USERLOG_FORMAT_INDEX)
					Length = strlen(strncat(Destination, Values->s[CountString++], BytesRemaining));
			 	break;
			 
			 case '%':
			 	*Destination = '%';
				Length = 1;
				break;
		}
		
		Destination += Length;
		BytesRemaining -= Length;
		Source++;
	}
	
	/* Complete with null terminator */
	*Destination = '\0';
}

/* Copy source to destination up to size of destination or source length */
void StringCopy(char *Destination, uint32_t Size, char *Source) {
	
	/* Local variables */
	uint32_t Length;
	
	/* Verify parameters */
	if(Destination == NULL || Source == NULL || Size == 0)
		return;
	
	/* Does destination's start overlap source's string length including null terminator */
	Length = strlen(Source);
	if(Source <= Destination && Destination <= Source + Length)
		return;
		
	/* Does destination's end overlap source's string length */
	if(Source <= Destination + Size - 1 && Destination + Size - 1 <= Source + Length)
		return;
	
	/* Copy */
	/* Decrement first, loop up to size - 1 */
	while(--Size && *Source != '\0')
		*Destination++ = *Source++;
	
	/* Complete with null terminator */
	*Destination = '\0';
}
