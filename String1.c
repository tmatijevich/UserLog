/*******************************************************************************
 * File: UserLog\string.c
 * Author: Tyler Matijevich
 * Created: 2022-08-30
 ******************************************************************************/

#include "main.h"

/* Format string with runtime data */
void string_format (char *destination, uint32_t size, char *source, UserLogFormatType *args)
{	
	/* Declare local variables */
	/* Boolean arguments = 5 + null terminator */
	const char boolean[][6] = {"FALSE", "TRUE"}; 	
	
	/* Floats: [<+->]1.23456[e<+->12] = 12 + null terminator */
	/* Longs: -2147483648 to 2147483647 = 11 + null terminator */
	char number[13]; 
	
	uint8_t count_bool = 0;
	uint8_t count_float = 0;
	uint8_t count_integer = 0;
	uint8_t count_string = 0;
	uint32_t length, bytes_remaining = size - 1;
	
	/* Verify parameters, attempt to copy if failed */
	if (destination == NULL || source == NULL || args == NULL || size == 0) 
		return string_copy(destination, size, source);
	
	/* Format */
	while (*source != '\0' && bytes_remaining > 0)
	{
		if (*source != '%')
		{
			/* Direct copy */
			*destination++ = *source++;
			bytes_remaining--;
			continue;
		}
		
		/* Temporarily add null terminator to perform concatenation */
		*destination = '\0';
		
		/* Set the length to zero if the format specifier is invalid */
		length = 0;
		
		switch (*(++source))
		{
			/* Use strncat to concatenate the formatted value up to bytes_remaining */
			/* strlen(strncat(destination, ...)) returns the length of characters appended because destination began as null */
			case 'b':
				if (count_bool <= USERLOG_FORMAT_INDEX) 
					length = strlen(strncat(destination, boolean[args->b[count_bool++]], bytes_remaining));
				break;
			
			 case 'f':
			 	if (count_float <= USERLOG_FORMAT_INDEX)
				{
					brsftoa((float)(args->f[count_float++]), (uint32_t)number);
					length = strlen(strncat(destination, number, bytes_remaining));
			 	}
			 	break;
			 
			 case 'i':
			 	if (count_integer <= USERLOG_FORMAT_INDEX)
				{
					brsitoa(args->i[count_integer++], (uint32_t)number);
					length = strlen(strncat(destination, number, bytes_remaining));
			 	}
			 	break;
			 
			 case 's':
			 	if (count_string <= USERLOG_FORMAT_INDEX) 
					length = strlen(strncat(destination, args->s[count_string++], bytes_remaining));
			 	break;
			 
			 case '%':
			 	*destination = '%';
				length = 1;
				break;
		}
		
		destination += length;
		bytes_remaining -= length;
		source++;
	}
	
	/* Complete with null terminator */
	*destination = '\0';
}

/* Copies source to destination up to size (of destination) or source length */
void string_copy (char *destination, uint32_t size, char *source)
{	
	/* Declare local variables */
	uint32_t length;
	
	/* Verify parameters */
	if (destination == NULL || source == NULL || size == 0) 
		return;
	
	/* Check if destination's start overwrites source */
	length = strlen(source);
	if (source <= destination && destination <= source + length) 
		return;
	
	/* Check if destination's end overwrites source, assumming truncation */
	if (length >= size - 1 && source - length <= destination && destination <= source) 
		return;
	
	/* Copy, decrement first for size - 1 characters remaining */
	while (--size && *source != '\0')
		*destination++ = *source++;
	
	/* Complete with null terminator */
	*destination = '\0';
}
