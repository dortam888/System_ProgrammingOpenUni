
#include <string.h>

#include "fileUtils.h"

char *PutEndingToFileName(char *dest, const char *file_name, const char *ending)
{
	strcat(strcpy(dest, file_name), ending);
	
	return dest;
}


void FileCat(FILE *dest, const char *src_file_name)
{
	FILE *src = NULL;
	char letter_in_src_file = '\0';

	src = fopen(src_file_name, "r");

	while ((letter_in_src_file = fgetc(src)) != EOF)
	{
		fputc(letter_in_src_file, dest);
	}

	fclose(src);
}