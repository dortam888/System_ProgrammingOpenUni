#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>

#include "../SymbolTableStage/SymbolTableCreate.h"
#include "../utils/file_utils/fileUtils.h"
#include "../OperationTable/operationTable.h"

struct attribute_handlers
{
	const char *attribute_name;
	ssize_t(*handle_func)(FILE *, symbol_table_t *, size_t, size_t, size_t *);
};


enum outfiles_status CreateObjectFile(symbol_table_t *symbol_table, size_t final_instruction_counter)
{
	size_t data_counter = 0;
	size_t instruction_counter = START_ADDRESS_OF_INSTRUCTION_COUNT;
	FILE *input_file = NULL;
	FILE *object_file = NULL;
	FILE *data_file = NULL;
	char input[MAX_FILE_NAME] = {'\0'};
	char object[MAX_FILE_NAME] = {'\0'};
	char line[MAX_LINE_LEN] = {'\0'};
	size_t line_number = 0;
	const char *delim = " ,\t\n";
	
	PutEndingToFileName(input, file_name, ".am");
	PutEndingToFileName(object, file_name, ".ob");
	
	input_file = fopen(input, "r");
	if (input_file == NULL)
	{
		printf("couldn't open file %s\n", input);
		return FAILED_TO_CREATE_FILES;
	}
	
	object_file = fopen(object, "w");
	if (object_file == NULL)
	{
		printf("couldn't open file %s\n", object);
		return FAILED_TO_CREATE_FILES;
	}
	
	data_file = fopen("data_file", "w");
	if (data_file == NULL)
	{
		return FAILED_TO_CREATE_FILES;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		++line_number;
		strtok(line, delim);
		
		if (IsComment(line))
		{
			continue;
		}
		
		if (IsLabel(line))
		{
			strtok(NULL, delim);
		}
		
		instruction_counter = IsAttribute(line)? HandleAttribute(data_file, symbol_table, line, instruction_counter, final_instruction_counter, &data_counter) : 
                                                 HandleCommand(object_file, symbol_table, line, instruction_counter);		
	}
	
	return SUCCEED_TO_CREATE_FILES;
}

ssize_t HandleData(FILE *data_file, symbol_table_t *symbol_table, size_t line_number, size_t address_to_write, size_t *data_counter)
{
	char *current_number = NULL;
	
	while((current_number = strtok(NULL, ", \t\n")) != NULL)
	{
		if(!IsNumber(current_number))
		{
			printf("Error - .data in line %lu got Unrecognized number %s\n", line_number, current_number);
			return -1;
		}
		
		WriteNumberToFile(data_file, address_to_write, strtol(current_number));
		
		++(*data_counter);
	}
	
	return 0;
}

ssize_t HandleString (FILE *data_file, symbol_table_t *symbol_table, size_t line_number, size_t address_to_write, size_t *data_counter)
{
	char *current_part_of_line = NULL;
	size_t str_length = 0;
	
	current_part_of_line = strtok(NULL, " \t\n“\"");
	str_length = strlen(current_part_of_line); 
	
	WriteStringToFile(current_part_of_line);
	
	*data_counter = str_length + 1; /* +1 for ending character '\0' */
	
	return 0;
}

ssize_t HandleEntry (FILE *data_file, symbol_table_t *symbol_table, size_t line_number, size_t address_to_write, size_t *data_counter)
{
	char *entry_symbol = strtok(NULL, " \t\n");
	
	if (FindSymbolInSymbolTable(symbol_table, entry_symbol) == NULL)
	{
		printf("Error - in line %lu symbol %s is not found in the symbol table\n", line_number, entry_symbol);
		return -1;
	}
	
	return 0;
}

ssize_t HandleExtern (FILE *data_file, symbol_table_t *symbol_table, size_t line_number, size_t address_to_write, size_t *data_counter)
{
	return 0;
}

static ssize_t HandleAttribute(FILE *data_file, symbol_table_t *symbol_table, char *line, size_t line_number, size_t instruction_counter, 
							   size_t final_instruction_counter, size_t *data_counter)
{
	struct attribute_handlers handle[] = {
											 {".data", HandleData},
					                         {".string", HandleString},
					                         {".entry", HandleEntry},
					                         {".extern", HandleExtern}
	                                     };
	int i = 0;
	
	for(i = 0; i < sizeof(handle) / sizeof(*handle); ++i)
	{
		if(strcmp(line, handle[i].attribute_name) == 0)
		{
			handle[i].handle_func(data_file, symbol_table, line_number, final_instruction_counter + *data_counter, data_counter);
			break;
		}
	}
	
	return instruction_counter;
}

