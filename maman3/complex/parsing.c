#include <stdio.h> /* printf */
#include <ctype.h> /* isspace */
#include <stdlib.h> /* atof */
#include <string.h> /* strtok */

#include "parsing.h"


/* 
	Skips white spaces in text
	input : char *text - the text
	output: position in text after whitespaces
*/
static char *SkipWhiteSpaces(char *text)
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
static char *CheckForIllegalComma(char *line)
{
	while(!isspace(*line))
	{
		if(*line == ',')
		{
			printf("Illegal Comma\n");
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
int CheckCommandCommasErrors(char *line)
{
	line = SkipWhiteSpaces(line);
	
	line = CheckForIllegalComma(line);
	
	if (line == NULL)
	{
		return 0;
	}
	
	line = SkipWhiteSpaces(line);
	
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
			printf("Multiple consecutive commas\n");
			return 0;
		}
		
		if (*line != ',' && *line != '\0' && comma_flag == 0)
		{
			printf("Missing Comma\n");
			return 0;
		}
		
		if (*line == '\0' && comma_flag == 1)
		{
			printf("Extraneous text after end of command\n");
			return 0;
		}
	}
	
	return 1;
}

/* 
	Check if input letter for complex variable is legal.
	input : char c
	output: 0 if not one of the letters A,B,C,D,E,F other value otherwise
*/
static int IsLegalComplexParameter(char c)
{
	return (c >= 'A') && (c <= 'F');
}

/* 
	Check if input complex parameter is valid.
	input : char *input - the input parameter
			error_t *error - indicator for input error
	output: 0 parameter is not valid 1 otherwise
			if parameter is not valid error will be changed accordingly
*/
static int IsValidComplex(char *input, error_t *error)
{
	int input_length = strlen(input);; /*the string input length*/
	
	if (input_length > 1 || !IsLegalComplexParameter(*input)) /*complex input length should be letter A-F*/
	{
		printf("Undefined Complex Variable\n");
		*error = INVALID_PARAMETER;
		return 0;	
	}
	
	return 1;
}

/* 
	Get complex parameter from stdin input.
	input : complex *variables - array of variables to initialize
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: the complex parameter in the command or NULL if the parameter is not valid
			in case the parameter is not valid error will be changed according to the mistake
*/
complex GetComplexParameter(complex *variables, const char *delim, error_t *error)
{
	char *parameter = strtok(NULL, delim);
	
	if(parameter == NULL)
	{
		printf("Missing Parameter\n");
		*error = MISSING_PARAMETER;
		return NULL;
	}
	
	if(IsValidComplex(parameter, error))
	{
		return variables[*parameter - 'A'];
	}		
	
	return NULL;
}

/* 
	checks if the float parameter is valid.
	input : const char *parameter - the parameter that was gotten
			error_t *error - indicator for input error
	output: the float parameter in the command or 0 if the parameter is not valid
			in case the parameter is not valid error will be changed according to the mistake
*/
static float IsValidFloat(const char *parameter, error_t *error)
{
	int sign = 1; /*sign of the input number*/ 
	
	if(*parameter == '-') /* if -change sign */
	{
		sign = -1;
		++parameter;
	}
	
	if((!isdigit(*parameter)))
	{
		printf("Invalid Parameter - Not a number\n");
		*error = INVALID_PARAMETER;
		return 0;
	}
	
	return atof(parameter) * sign;
}

/* 
	Get float parameter from stdin input.
	input : const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: the float parameter in the command or 0 if the parameter is not valid
			in case the parameter is not valid error will be changed according to the mistake
*/
float GetFloatParameter(const char *delim, error_t *error)
{
	char *parameter = strtok(NULL, delim);
	
	if(parameter == NULL)
	{
		printf("Missing Parameter\n");
		*error = MISSING_PARAMETER;
		return 0;
	}
	
	return IsValidFloat(parameter, error);	
}

/* 
	Handle command error in case of undefined command
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: the float parameter in the command or 0 if the parameter is not valid
			in case the parameter is not valid error will be changed according to the mistake
*/
int TooManyParameters(const char *delim)
{
	if (strtok(NULL, delim) != NULL)
	{
		printf("Extraneous text after end of command\n");
		return 1;
	}
	
	return 0;
}
