/* dummy implementation of strtol */

long strtol(const char* nPtr, char** endPtr, int base)
{
	if (endPtr) {
		*endPtr = (char*)nPtr;
	}
	return 0;
}
