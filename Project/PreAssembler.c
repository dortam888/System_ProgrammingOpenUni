#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "PreAssembler.h"
#include "vector.h"

#define MAX_LINE_LEN 100
#define MAX_FILE_NAME 10
#define INITIAL_NUMBER_OF_MACROS 10

typedef struct macro_data
{
	char macro_name[MAX_LINE_LEN];
	FILE *start_of_macro;
	FILE *end_of_macro;
} macro_data_t;

typedef enum {LINE_NOT_IN_MACRO, LINE_IN_MACRO} is_in_macro_t;

static int HandleNotInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file);
static int HandleInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file);

static int OpenFiles(FILE **input, FILE **output, const char *input_name, const char *output_name)
{
	return (*input = fopen(input_name, "r")) && 
	       (*output = fopen(output_name, "w"));
}

int CreateAmFile(const char *file_name)	
{
	FILE *input_file = NULL;
	FILE *output_file = NULL;
	char input [MAX_FILE_NAME];
	char output [MAX_FILE_NAME];
	char line[MAX_LINE_LEN] = {'\0'};
	is_in_macro_t is_in_macro = LINE_NOT_IN_MACRO;
	d_vector_t *macro_vector = DVectorCreate(sizeof(macro_data_t), INITIAL_NUMBER_OF_MACROS);

	strcat(strcpy(input, file_name), ".as");
	strcat(strcpy(output, file_name), ".am");
	
	if(!OpenFiles(&input_file, &output_file, input, output))
	{
		printf("couldn't open necessary files\n");
		return -1;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		int(*handle_func[2])(const char *, d_vector_t*, FILE *, FILE *) = {HandleNotInMacro, HandleInMacro};
		
		is_in_macro = handle_func[is_in_macro](line, macro_vector, input_file, output_file);
	}
	
	return 0;	
}

static int CreateNewMacro(d_vector_t *macro_vector, FILE* file)
{
	macro_data_t *new_macro = malloc(sizeof(*new_macro)); 
	if (new_macro == NULL)
	{
		return 1;
	}
	
	strcpy(new_macro -> macro_name, strtok(NULL, " \t\n"));
	
	new_macro -> start_of_macro = file;
	
	DVectorPushBack(macro_vector, new_macro);
	
	return 0;
}

static void CopyMacroLinesToAmFile(macro_data_t *macro, FILE *output_file)
{	
	while (macro -> start_of_macro < macro -> end_of_macro)
	{
		fprintf(output_file, "%c", getc(macro->start_of_macro));
	}
}

static int IsMacroLine(const char *line)
{
	return !strcmp(line, "macro");
}

static macro_data_t *FindMacroNameInVector(d_vector_t *vector, char *line)
{
	int i = 0;
	
	for (i = 0; i < DVectorSize(vector); ++i)
	{
		macro_data_t *data = DVectorGetItemAddress(vector, i);
		if (strcmp(data -> macro_name, line) == 0)
		{
			return data;
		}
	}
	
	return NULL;
}

static int HandleNotInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file)
{
	char copy_line[MAX_LINE_LEN];
	strcpy(copy_line, line);

	strtok(copy_line, " \t\n");

	if(IsMacroLine(copy_line))
	{
		CreateNewMacro(macro_vector, input_file);
		return LINE_IN_MACRO;
	}
	else
	{
		macro_data_t *is_macro = FindMacroNameInVector(macro_vector, copy_line);
		
		if(is_macro)
		{
			CopyMacroLinesToAmFile(is_macro, output_file);	
		}
		else
		{
			fprintf(output_file, "%s", line);
		}
	}
	
	return LINE_NOT_IN_MACRO;
}

static int EndOfMacro(const char *line)
{
	char copy_line[MAX_LINE_LEN];
	strcpy(copy_line, line);
	strtok(copy_line, " \t\n");
	
	return !strcmp(copy_line, "endm");	
} 

static int HandleInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file)
{
	if (EndOfMacro(line))
	{
		macro_data_t *data = DVectorGetItemAddress(macro_vector, DVectorSize(macro_vector) - 1);
		data -> end_of_macro = input_file;
		return LINE_NOT_IN_MACRO;
	}
	
	return LINE_IN_MACRO;
}
