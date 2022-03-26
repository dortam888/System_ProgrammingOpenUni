#include <stdio.h> /* printf */
#include <stdlib.h> /* strtol */
#include <ctype.h> /* isalpha */
#include <string.h> /* strlen */
#include <assert.h> /* assert */

#include "parsing_asm_file.h"

struct parse_attributes
{
	const char *attribute;
	int(*handling_func)(const char *line);
};

int IsNumber(const char *str)
{
	char *end;

	assert(str != NULL);
	strtol(str, &end, DECIMAL_BASE);
	
	return *end == '\0';
}

int IsComment(const char *str)
{
	assert(str != NULL);
	return *str == ';';
}

int IsLabel(const char *str)
{	
	assert(str != NULL);
	return isalpha(*str) && (*(str + strlen(str) - 1) == ':');
}

int IsAttribute(const char *str)
{
	assert(str != NULL);
	return *str == '.';
}

char *GetLabelName(char *label_name_buffer, const char *str)
{
	size_t label_len = 0; 
	
	assert(str != NULL);
	assert(label_name_buffer != NULL);

	label_len = strlen(str) - 1; /* -1 to remove  : in the end of label */
	strncpy(label_name_buffer, str, label_len);
	label_name_buffer[label_len] = '\0';
	
	return label_name_buffer;
}

char *GetLabelFromIndexOperand(char *label, const char *operand)
{
	char *start_of_label = label;
	
	while(*operand != '[')
	{
		*label = *operand;/* for : in the end of label */
		++label;
		++operand;
	}
	
	return start_of_label;
}

/* 
	Skips white spaces in text
	input : char *text - the text
	output: position in text after whitespaces
*/
static const char *SkipWhiteSpaces(const char *text)
{
	while(isspace(*text))
	{
		++text;
	}
	
	return text;
}

/* 
	Check if there is a comma in the command name
	input : input : char *line - the line of command
	output: the current position in the input
*/
static const char *CheckForIllegalComma(const char *line)
{
	while(!isspace(*line))
	{
		if(*line == ',')
		{
			printf("Illegal Comma ");
			return NULL;
		}
		++line;
	}
	
	return line;
}

/* 
	Check for comma errors
	input : char *line - the line of command
	output: 0 if there is a mistake in the input other value otherwise
*/
static int ParsingHandleCommand(const char *line, int status)
{
	line = SkipWhiteSpaces(line);

	if(status)
	{
		line = CheckForIllegalComma(line);
		
		if (line == NULL)
		{
			return 0;
		}
		
		line = SkipWhiteSpaces(line);
	}

	while (*line != '\0')
	{
		int comma_flag = 0;
		
		while(!isspace(*line))
		{
			if(*line == ',')
			{
				comma_flag = 1;
				++line;
				break;
			}
			++line;
		}
		
		line = SkipWhiteSpaces(line);
		
		if(*line == ',' && comma_flag == 1)
		{
			printf("Multiple consecutive commas ");
			return 0;
		}
		
		if (*line != ',' && *line != '\0' && comma_flag == 0)
		{
			printf("Missing Comma ");
			return 0;
		}
		
		if (*line == '\0' && comma_flag == 1)
		{
			printf("Extraneous text after end of command ");
			return 0;
		}
	}
	
	return 1;
}

static int ParsingHandleData(const char *line)
{
	assert(line != NULL);
	
	while(*line != '\0')
	{
		char *end;
		++line;
		line = SkipWhiteSpaces(line);
		if(*line == '\0')
		{
			printf("Error - Extraneous text after data attribute ");
			return 0;
		}

		strtol(line, &end, DECIMAL_BASE);
		line = SkipWhiteSpaces(end);
		if (*line != ',' && *line != '\0')
		{
			printf("Error - invalid expression in data attribute ");
			return 0;
		}
	}

	return 1;
}

int IsStartOfString(char c)
{
	return c == '\"';
}

int IsEndOfString(const char *str)
{
	return (*str == '\"' && ((*(str - 1) == '\\' && *(str - 2) == '\\') || (*str - 1) != '\\'));
}

static int ParsingHandleString(const char *line)
{
	line = SkipWhiteSpaces(line);

	if(!IsStartOfString(*line))
	{
		printf("Error - invalid string attribute ");
		return 0;
	}

	++line;

	while(!(IsEndOfString(line)))
	{
		++line;
	}

	line = SkipWhiteSpaces(++line);
	if(*line != '\0')
	{
		printf("Error - Extraneous text after string ");
		return 0;
	}

	return 1;
}

static int ParsingHandleEntryOrExtern(const char *line)
{
	line = SkipWhiteSpaces(line);

	while(!isspace(*line))
	{
		++line;
	}

	line = SkipWhiteSpaces(line);

	if(*line != '\0')
	{
		printf("Error - Extraneous text after entry or extern ");
		return 0;
	}

	return 1;
}

static int ParsingHandleAttribute(const char *line)
{	
	struct parse_attributes attributes[] = {{".data", ParsingHandleData}, {".string", ParsingHandleString}, 
	                                         {".entry", ParsingHandleEntryOrExtern}, {".extern", ParsingHandleEntryOrExtern}};
	int i = 0;

	for(i = 0; i < sizeof(attributes)/sizeof(*attributes); ++i)
	{
		size_t attribute_length = strlen(attributes[i].attribute);

		if(strncmp(line, attributes[i].attribute, attribute_length) == 0)
		{
			line += attribute_length;
			return attributes[i].handling_func(line);
		}
	}

	return -1;
}

int IsSentenceCorrect(const char *line)
{
	int label_status = 0;

	line = SkipWhiteSpaces(line);

	if(*line == '\0')
	{
		return 1;
	}

	if (IsAttribute(line))
	{
		return ParsingHandleAttribute(line);
	}

	line = CheckForIllegalComma(line);
	if (line == NULL)
	{
		return 0;
	}

	if(*(line - 1) == ':')
	{
		label_status = 1;
	}

	line = SkipWhiteSpaces(line);

	if(*line == '\0')
	{
		printf("Error - Empty Label\n");
		return 0;
	}

	if (IsAttribute(line))
	{
		return ParsingHandleAttribute(line);
	}

	return ParsingHandleCommand(line, label_status);
}