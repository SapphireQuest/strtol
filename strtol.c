#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <limits.h>

int checkSign(const unsigned char* current_char_ptr, int* sign);
int validateBase(int base);
int detectBase(const unsigned char* ptr);
int getDigitValue(unsigned char current_char, int base);
int addDigitToResult(long int* result, int digit_value, int sign, int base);

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
			*endPtr = (char*)nPtr;
		}
		return 0;
	}

	if (base == 0)
	{
		base = detectBase(current_char_ptr);
	}	

	if (base == 16 && (*current_char_ptr == '0' && (*(current_char_ptr + 1) == 'x' || *(current_char_ptr + 1) == 'X')))
	{
		if (getDigitValue(*(current_char_ptr + 2), base) != -1)
		{
			current_char_ptr += 2;
		}
	}
	
	long int result = 0;
	int digit_value;
	int digitsfound = 0;
	int overflow = 0;
	while (*current_char_ptr)
	{
		digit_value = getDigitValue(*current_char_ptr, base);
		if (digit_value == -1 || digit_value >= base)
		{
			break;
		}
		if (!overflow)
		{
			if (!addDigitToResult(&result, digit_value, sign, base))
			{
				overflow = 1;
			}
		}
		
		digitsfound = 1;
		current_char_ptr++;

	}	

	if (endPtr != NULL)
	{
		if (!digitsfound)
		{
			*endPtr = (char*)nPtr;
		}
		 else
		{
			*endPtr = (char*)current_char_ptr;
		}
	}
	return result;
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


int detectBase(const unsigned char* ptr)
{
	if (*ptr == '0')
	{
		if (*(ptr + 1) == 'x' || *(ptr + 1) == 'X')
		{
			return 16;
		}
		else
		{
			return 8;
		}
	}
	return 10;
}

int getDigitValue(unsigned char current_char, int base)
{
	if (isdigit(current_char))
	{
		int digit_value = current_char - '0';
		if (digit_value < base)
		{
			return digit_value;
		}
	}
	else if (isalpha(current_char))
	{
		int digit_value = toupper(current_char) - 'A' + 10;
		if (digit_value < base)
		{
			return digit_value;
		}
	}
	return -1;
}

int addDigitToResult(long int* result, int digit_value, int sign, int base)
{
	if (sign == 1)
	{
		if (*result > (LONG_MAX - digit_value) / base)
		{
			errno = ERANGE;
			*result = LONG_MAX;
			return 0;
		}
	}
	else
	{
		if (*result < (LONG_MIN + digit_value) / base)
		{
			errno = ERANGE;
			*result = LONG_MIN;
			return 0;
		}
	}
	*result = *result * base + sign * digit_value;
	return 1;
}