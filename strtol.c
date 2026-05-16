#include <ctype.h>
#include <errno.h>
#include <stddef.h>

int checkSign(const unsigned char* current_char_ptr, int* sign);
int validateBase(int base);

long int strtol(const char* nPtr, char** endPtr, int base)
{
	const unsigned char *current_char_ptr = (const unsigned char*)nPtr;
	
	while (isspace(*current_char_ptr))
	{
		current_char_ptr++;
	}

	int sign = 1;
	if (checkSign(current_char_ptr, &sign))
	{
		current_char_ptr++;
	}
	
	if (!validateBase(base))
	{
		errno = EINVAL;
		if (endPtr != NULL)
		{
			*endPtr = (char*)current_char_ptr;
		}
		return 0;
	}

	


	return 0;
}

int checkSign(const unsigned char* current_char_ptr, int* sign)
{
	if (*current_char_ptr == '-')
	{
		*sign = -1;
		return 1;
	}
	else if (*current_char_ptr == '+')
	{
		return 1;
	}
	return 0;
}


int validateBase(int base)
{
	if (base == 0 || (base >= 2 && base <= 36))
	{
		return 1;
	}
	else
	{	
		return 0;
	}
}