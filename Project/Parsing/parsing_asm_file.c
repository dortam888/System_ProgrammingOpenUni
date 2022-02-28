#include <stdlib.h> /* strtol */
#include <ctype.h> /* isalpha */
#include <string.h> /* strlen */

#include "parsing_asm_file.h"

int IsNumber(const char *str)
{
	char *end;
	
	strtol(str, &end, DECIMAL_BASE);
	
	return *end == '\0';
}

int IsComment(const char *str)
{
	return *str == ';';
}

int IsLabel(const char *str)
{	
	return isalpha(*str) && (*(str + strlen(str) - 1) == ':');
}

int IsAttribute(const char *str)
{
	return *str == '.';
}

char *GetLabelName(char *label_name_buffer, const char *str)
{
	size_t label_len = strlen(str) - 1; /* for : in the end of label */
	
	strncpy(label_name_buffer, str, label_len);
	label_name_buffer[label_len] = '\0';
	
	return label_name_buffer;
}

