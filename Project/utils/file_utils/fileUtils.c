#include <string.h>

#include "fileUtils.h"

char *PutEndingToFileName(char *dest, const char *file_name, const char *ending)
{
	strcat(strcpy(dest, file_name), ending);
	
	return dest;
}
