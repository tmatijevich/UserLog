/*******************************************************************************
 * File: UserLog\String.c
 * Author: Tyler Matijevich
 * Created: 2022-08-30
 ******************************************************************************/

#include "Main.h"

/* Format string with runtime data */
void StringFormat(char *Destination, uint32_t Size, char *Source, UserLogFormatType *Args) {
	
	/* Declare local variables */
	/* Boolean arguments = 5 + null terminator */
	const char Boolean[][6] = {"FALSE", "TRUE"};
	
	/* Floats: [<+->]1.23456[e<+->12] = 12 + null terminator */
	/* Longs: -2147483648 to 2147483647 = 11 + null terminator */
	char Number[13]; 
	
	uint8_t CountBool = 0;
	uint8_t CountFloat = 0;
	uint8_t CountInteger = 0;
	uint8_t CountString = 0;
	uint32_t Length, BytesRemaining = Size - 1;
	
	/* Verify parameters, attempt to copy if failed */
	if(Destination == NULL || Source == NULL || Args == NULL || Size == 0)
		return StringCopy(Destination, Size, Source);
	
	/* Format */
	while(*Source != '\0' && BytesRemaining > 0) {
		
		if(*Source != '%') {
			/* Direct copy */
			*Destination++ = *Source++;
			BytesRemaining--;
			continue;
		}
		
		/* Temporarily add null terminator to perform concatenation */
		*Destination = '\0';
		
		/* Set the length to zero if the format specifier is invalid */
		Length = 0;
		
		switch(*(++Source)) {
			/* Use strncat to concatenate the formatted value up to BytesRemaining */
			/* strlen(strncat(Destination, ...)) returns the length of characters appended because Destination began as null */
			case 'b':
				if(CountBool <= USERLOG_FORMAT_INDEX)
					Length = strlen(strncat(Destination, Boolean[Args->b[CountBool++]], BytesRemaining));
				break;
			
			 case 'f':
			 	if(CountFloat <= USERLOG_FORMAT_INDEX) {
					brsftoa((float)(Args->f[CountFloat++]), (uint32_t)Number);
					Length = strlen(strncat(Destination, Number, BytesRemaining));
			 	}
			 	break;
			 
			 case 'i':
			 	if(CountInteger <= USERLOG_FORMAT_INDEX) {
					brsitoa(Args->i[CountInteger++], (uint32_t)Number);
					Length = strlen(strncat(Destination, Number, BytesRemaining));
			 	}
			 	break;
			 
			 case 's':
			 	if(CountString <= USERLOG_FORMAT_INDEX)
					Length = strlen(strncat(Destination, Args->s[CountString++], BytesRemaining));
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

/* Copies source to destination up to size (of destination) or source length */
void StringCopy(char *Destination, uint32_t Size, char *Source) {
	
	/* Declare local variables */
	uint32_t Length;
	
	/* Verify parameters */
	if(Destination == NULL || Source == NULL || Size == 0)
		return;
	
	/* Check if destination's start overwrites source */
	Length = strlen(Source);
	if(Source <= Destination && Destination <= Source + Length)
		return;
	
	/* Check if Destination's end overwrites source, assumming truncation */
	if(Length >= Size - 1 && Source - Length <= Destination && Destination <= Source)
		return;
	
	/* Copy, decrement first for size - 1 characters remaining */
	while(--Size && *Source != '\0')
		*Destination++ = *Source++;
	
	/* Complete with null terminator */
	*Destination = '\0';
}
