#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef enum {LINE_NOT_IN_MACRO, LINE_IN_MACRO} is_in_macro_t;

static int HandleNotInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file);
static int HandleInMacro(const char *line, d_vector_t *macro_vector, FILE *input_file, FILE *output_file);

static int OpenFiles(FILE **input, FILE **output, const char *input_name, const char *output_name)
{
	return (*input = fopen(input_name, "r")) && 
	       (*output = fopen(output_name, "w"));
}

static void FreeAndCloseAll(d_vector_t *macro_vector, FILE *input_file, FILE *output_file)
{
	DVectorDestroy(macro_vector);
	fclose(input_file);
	fclose(output_file);
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
	if (macro_vector == NULL)
	{
		printf("Couldn't allocate memory");
		return -1;
	}

	PutEndingToFileName(input, file_name, ".as");
	PutEndingToFileName(output, file_name, ".am");
	
	if(!OpenFiles(&input_file, &output_file, input, output))
	{
		printf("couldn't open necessary files\n");
		return -1;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		int(*handle_func[])(const char *, d_vector_t*, FILE *, FILE *) = {HandleNotInMacro, HandleInMacro};
		
		is_in_macro = handle_func[is_in_macro](line, macro_vector, input_file, output_file);
	}
	
	FreeAndCloseAll(macro_vector, input_file, output_file);
	
	return 0;	
}

static int CreateNewMacro(d_vector_t *macro_vector, FILE* file)
{
	macro_data_t new_macro;
	
	strcpy(new_macro.macro_name, strtok(NULL, " \t\n"));
	
	new_macro.start_of_macro = ftell(file);
	new_macro.end_of_macro = 0;
	
	DVectorPushBack(macro_vector, &new_macro);
	
	return 0;
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

static int CompareToMacroName(const void *line, const void *macro_data)
{
	return strcmp((const char *)line, ((const macro_data_t *)(macro_data)) -> macro_name);
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
		char *next_part_in_line = copy_line;
		macro_data_t *is_macro = NULL;
		
		while(next_part_in_line != NULL)
		{
			is_macro = DVectorFind(macro_vector, next_part_in_line, CompareToMacroName);
			
			if(is_macro)
			{
				CopyMacroLinesToAmFile(is_macro, input_file, output_file);	
			}
			else
			{
				fprintf(output_file, "%s ", next_part_in_line);
			}
			
			next_part_in_line = strtok(NULL, " \t\n");
		}
		
		fprintf(output_file, "%c", '\n');

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
		size_t line_length = strlen(line);
		macro_data_t *data = DVectorGetItemAddress(macro_vector, DVectorSize(macro_vector) - 1); /*assuming no macro inside a macro*/
		data -> end_of_macro = ftell(input_file) - line_length - 1; /* the end of the macro is where the pointer is now (line after endm) minus the length of the line endm 
		                                                               with whitespaces and -1 for dropping the new line before (implementation choice)*/
		return LINE_NOT_IN_MACRO;
	}
	
	return LINE_IN_MACRO;
}
