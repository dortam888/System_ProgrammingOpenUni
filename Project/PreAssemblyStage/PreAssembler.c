#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "PreAssembler.h"
#include "../utils/data_structures/vector.h"
#include "../utils/file_utils/fileUtils.h"

#define INITIAL_NUMBER_OF_MACROS 10

typedef struct macro_data
{
	char macro_name[MAX_LINE_LEN];
	int start_of_macro;
	int end_of_macro;
} macro_data_t;

typedef enum {LINE_NOT_IN_MACRO, LINE_IN_MACRO, ERROR_IN_MACRO = -1} is_in_macro_t;

static int HandleNotInMacro(const char *line, d_vector_t *macro_table, FILE *input_file, FILE *output_file, const char *delim);
static int HandleInMacro(const char *line, d_vector_t *macro_table, FILE *input_file, FILE *output_file, const char *delim);

macro_table_t *CreateMacroTable(size_t initial_size_of_table)
{
	return DVectorCreate(sizeof(macro_data_t), initial_size_of_table);
}

void FreeMacroTable(macro_table_t *macro_table)
{
	assert(macro_table != NULL);

	DVectorDestroy(macro_table);
}

int PushMacroToTable(macro_table_t *macro_table, void *macro)
{
	assert(macro_table != NULL);

	return DVectorPushBack(macro_table, (macro_data_t *)macro);
}

void *FindMacroInTable(macro_table_t *macro_table, const void *data_to_find, int(*compare_func)(const void *,const void *))
{
	assert(macro_table != NULL);
	assert(compare_func != NULL);

	return DVectorFind(macro_table, data_to_find, compare_func);
}

static void FreeAndCloseAll(macro_table_t *macro_table, FILE *input_file, FILE *output_file)
{
	FreeMacroTable(macro_table);
	fclose(input_file);
	fclose(output_file);
}

enum preassembly_stage_status CreateAmFile(const char *file_name)	
{
	FILE *input_file = NULL;
	FILE *output_file = NULL;
	char input[MAX_FILE_NAME];
	char output[MAX_FILE_NAME];
	char line[MAX_LINE_LEN] = {'\0'};
	const char *delim = " \t\n\v\r\f"; /* whitespaces characters */
	is_in_macro_t is_in_macro = LINE_NOT_IN_MACRO;
	macro_table_t *macro_table = CreateMacroTable(INITIAL_NUMBER_OF_MACROS);
	if (macro_table == NULL)
	{
		printf("OS Error - Couldn't allocate memory to macro_table\n");
		return OS_ERROR;
	}

	PutEndingToFileName(input, file_name, ".as");
	PutEndingToFileName(output, file_name, ".am");
	
	input_file = fopen(input, "r");
	if (input_file == NULL)
	{
		printf("Error - Couldn't find file name %s\n", input);
		return COULDNT_FIND_INPUT_FILE;
	}
	
	output_file = fopen(output, "w");
	if (output_file == NULL)
	{
		printf("OS Error - Couldn't open file %s in this directory.\n Please check permission to this directory\n", output);
		fclose(input_file);
		return OS_ERROR;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		int(*handle_func[])(const char *, macro_table_t*, FILE *, FILE *, const char *) = {HandleNotInMacro, HandleInMacro};
		
		is_in_macro = handle_func[is_in_macro](line, macro_table, input_file, output_file, delim);
		if (is_in_macro == ERROR_IN_MACRO)
		{
			printf("OS Error - Not enough memory in the machine");
			FreeAndCloseAll(macro_table, input_file, output_file);
			remove(output);
			return OS_ERROR;
		}
	}
	
	FreeAndCloseAll(macro_table, input_file, output_file);
	
	return SUCCESS;	
}

static int CreateNewMacro(macro_table_t *macro_table, FILE* file, const char *delim)
{
	macro_data_t new_macro;
	
	strcpy(new_macro.macro_name, strtok(NULL, delim));
	
	new_macro.start_of_macro = ftell(file);
	new_macro.end_of_macro = 0;
	
	return PushMacroToTable(macro_table, &new_macro);
}

static void CopyMacroLinesToAmFile(macro_data_t *macro, FILE *input_file, FILE *output_file)
{		
	int current = ftell(input_file);
	
	fseek(input_file, macro -> start_of_macro, SEEK_SET);
	while (macro -> start_of_macro < macro -> end_of_macro)
	{		
		fprintf(output_file, "%c", fgetc(input_file));
		++(macro->start_of_macro);
	}
	
	fseek(input_file, current, SEEK_SET);
}

static int IsMacroLine(const char *line)
{
	return !strcmp(line, "macro");
}

static int CompareToMacroName(const void *macro_data, const void *line)
{
	return strcmp((const char *)line, ((const macro_data_t *)(macro_data)) -> macro_name);
}

static int HandleNotInMacro(const char *line, macro_table_t *macro_table, FILE *input_file, FILE *output_file, const char *delim)
{
	char copy_line[MAX_LINE_LEN]; /* buffer for copy of the line to not ruin line when calling strtok for copy */
	
	strcpy(copy_line, line); /* copy the line to buffer */
	strtok(copy_line, delim);

	if(IsMacroLine(copy_line))
	{
		if (CreateNewMacro(macro_table, input_file, delim) != 0)
		{
			return ERROR_IN_MACRO;
		}
		
		return LINE_IN_MACRO;
	}

	else
	{
			macro_data_t *is_macro = NULL;
			
			if((is_macro = FindMacroInTable(macro_table, copy_line, CompareToMacroName)) != NULL)
			{
				CopyMacroLinesToAmFile(is_macro, input_file, output_file);	
			}

			else
			{
				fprintf(output_file, "%s", line);
			}
	}
	
	return LINE_NOT_IN_MACRO;
}

static int IsEndOfMacro(const char *line, const char *delim)
{
	char copy_line[MAX_LINE_LEN];
	strcpy(copy_line, line);
	strtok(copy_line, delim);
	
	return !strcmp(copy_line, "endm");	
} 

static int HandleInMacro(const char *line, macro_table_t *macro_table, FILE *input_file, FILE *output_file, const char *delim)
{
	if (IsEndOfMacro(line, delim))
	{
		size_t line_length = strlen(line);
		macro_data_t *data = DVectorGetItemAddress(macro_table, DVectorSize(macro_table) - 1); /* assuming no macro inside a macro */
		data -> end_of_macro = ftell(input_file) - line_length; /* the end of the macro is where the pointer is now (line after endm) minus the length of the line endm 
		                                                               with whitespaces */
		return LINE_NOT_IN_MACRO;
	}

	return LINE_IN_MACRO;
}