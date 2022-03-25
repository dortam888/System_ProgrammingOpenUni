#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "WriteBinaryToFile.h"
#include "ObjectFileCreate.h"
#include "../utils/file_utils/fileUtils.h"
#include "../Parsing/parsing_asm_file.h"
#include "../SymbolTableStage/SymbolTableCreate.h"
#include "../SystemPreferences/OperationTable/operationTable.h"
#include "../SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "../SystemPreferences/AddressingMethods/addressing_methods.h"

static enum outfiles_status HandleCommand(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, char *line, 
                                          size_t line_number, size_t *instruction_counter, const char *delim);
static enum outfiles_status HandleAttribute(FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, char *attribute, size_t line_number, 
                                            size_t *instruction_counter, size_t final_instruction_counter, size_t *data_counter, const char *delim);

struct attribute_handlers
{
	const char *attribute_name;
	enum outfiles_status(*handle_func)(FILE *, FILE *, symbol_table_t *, const char *, size_t, size_t, size_t *, const char *);
};

struct files
{
	FILE *input_file;
	FILE *object_file;
	FILE *data_file;
	FILE *entry_file;
	FILE *external_file;
};

static enum outfiles_status OpenFiles(const char *file_name, struct files *files)
{
	char input[MAX_FILE_NAME] = {'\0'};
	char object[MAX_FILE_NAME] = {'\0'};
	char enteries[MAX_FILE_NAME] = {'\0'};
	char externals[MAX_FILE_NAME] = {'\0'};

	PutEndingToFileName(input, file_name, ".am");
	PutEndingToFileName(object, file_name, ".ob");
	PutEndingToFileName(enteries, file_name, ".ent");
	PutEndingToFileName(externals, file_name, ".ext");
	
	files->input_file = fopen(input, "r");
	if (files->input_file == NULL)
	{
		printf("couldn't open file %s\n", input);
		return FAILED_TO_CREATE_FILES;
	}
	
	files->object_file = fopen(object, "w");
	if (files->object_file == NULL)
	{
		printf("couldn't open file %s\n", object);
		return FAILED_TO_CREATE_FILES;
	}
	
	files->entry_file = fopen(enteries, "w");
	if(files->entry_file == NULL)
	{
		printf("couldn't open file %s\n", enteries);
		return FAILED_TO_CREATE_FILES;
	}

	files->external_file = fopen(externals, "w");
	if(files->external_file == NULL)
	{
		printf("couldn't open file %s\n", externals);
		return FAILED_TO_CREATE_FILES;
	}

	files->data_file = fopen("data_file", "w");
	if (files->data_file == NULL)
	{
		return FAILED_TO_CREATE_FILES;
	}

	return SUCCEED_TO_CREATE_FILES;
}

static void CloseAndFreeAll(struct files *files, symbol_table_t *symbol_table)
{
	fclose(files->object_file);
	fclose(files->input_file);
	fclose(files->entry_file);
	fclose(files->external_file);
	FreeSymbolTable(symbol_table);
}

static void RemoveOutputFiles(const char *file_name)
{
	char input[MAX_FILE_NAME] = {'\0'};
	char object[MAX_FILE_NAME] = {'\0'};
	char enteries[MAX_FILE_NAME] = {'\0'};
	char externals[MAX_FILE_NAME] = {'\0'};

	PutEndingToFileName(input, file_name, ".am");
	PutEndingToFileName(object, file_name, ".ob");
	PutEndingToFileName(enteries, file_name, ".ent");
	PutEndingToFileName(externals, file_name, ".ext");

	remove(input);
	remove(object);
	remove(enteries);
	remove(externals);
}

enum outfiles_status CreateOutputFiles(const char *file_name, symbol_table_t *symbol_table, size_t final_instruction_counter, size_t final_data_counter)
{
	enum outfiles_status assembler_status = SUCCEED_TO_CREATE_FILES;
	size_t data_counter = 0;
	size_t instruction_counter = START_ADDRESS_OF_INSTRUCTION_COUNT;
	struct files file = {NULL, NULL, NULL, NULL, NULL};
	char line[MAX_LINE_LEN] = {'\0'};
	size_t line_number = 0;
	const char *delim = " ,\t\n\v\r\f";

	if(OpenFiles(file_name, &file) != SUCCEED_TO_CREATE_FILES)
	{
		return FAILED_TO_CREATE_FILES;
	}
	fprintf(file.object_file, "\t%zu\t%zu\n", final_instruction_counter - START_ADDRESS_OF_INSTRUCTION_COUNT, final_data_counter);
	
	while(fgets(line, MAX_LINE_LEN, file.input_file))
	{
		char *current_part_of_line = NULL;
		char copy_line[MAX_LINE_LEN] = {'\0'};
		
		strcpy(copy_line, line);
		++line_number;
		current_part_of_line = strtok(line, delim);
		
		if (current_part_of_line == NULL || IsComment(current_part_of_line))
		{
			continue;
		}
		
		if (IsLabel(current_part_of_line))
		{
			current_part_of_line = strtok(NULL, delim);
		}
		
		assembler_status = IsAttribute(current_part_of_line)? 
		HandleAttribute(file.data_file, file.entry_file, symbol_table, copy_line, current_part_of_line, line_number, 
		                &instruction_counter, final_instruction_counter, &data_counter, delim) : 
    HandleCommand(file.object_file, file.external_file, symbol_table, current_part_of_line, line_number, &instruction_counter, delim);
	}
	
	fclose(file.data_file);
	FileCat(file.object_file, "data_file");
	remove("data_file");

	CloseAndFreeAll(&file, symbol_table);

	if(assembler_status != SUCCEED_TO_CREATE_FILES)
	{
		RemoveOutputFiles(file_name);
	}	

	return SUCCEED_TO_CREATE_FILES;
}

static enum outfiles_status HandleData(FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, size_t line_number, 
                                       size_t address_to_write, size_t *data_counter, const char *delim)
{
	char *current_number = NULL;
	
	while((current_number = strtok(NULL, delim)) != NULL)
	{
		if(!IsNumber(current_number))
		{
			printf("Error - .data in line %zu got Unrecognized number %s\nline: %s", line_number, current_number, line);
			return FAILED_TO_CREATE_FILES;
		}
		
		WriteNumberToFile(data_file, address_to_write, strtol(current_number, NULL, DECIMAL_BASE), ABSOLUTE);
		
		++(*data_counter);
		++address_to_write;
	}
	
	return SUCCEED_TO_CREATE_FILES;
}

static enum outfiles_status HandleString (FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, size_t line_number, 
                                          size_t address_to_write, size_t *data_counter, const char *delim)
{
	while (!IsStartOfString(*line))
	{
		++line; /* move to next letter until start of string */
	}

	++line; /* skip " which is start of string */
	
	WriteStringToFile(data_file, address_to_write, line, data_counter);
	
	return SUCCEED_TO_CREATE_FILES;
}

static void WriteSymbolInEntryFile(FILE *entry_file, symbol_data_t *symbol, const char *symbol_name)
{
	fprintf(entry_file, "%s, %u, %u\n", symbol_name, GetBaseAddress(symbol), GetOffset(symbol));
}

static enum outfiles_status HandleEntry (FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, size_t line_number, 
                                         size_t address_to_write, size_t *data_counter, const char *delim)
{
	char *entry_symbol = strtok(NULL, delim);
	symbol_data_t *symbol = NULL;
	
	if ((symbol = FindSymbolInSymbolTable(symbol_table, entry_symbol)) == NULL)
	{
		printf("Error - in line %zu symbol %s is not found in the symbol table\n", line_number, entry_symbol);
		return FAILED_TO_CREATE_FILES;
	}

	if(GetAttribute(symbol) == EXTERN)
	{
		printf("Error - in line %zu symbol %s has both entry and extern attributes", line_number, entry_symbol);
		return FAILED_TO_CREATE_FILES;
	}

	WriteSymbolInEntryFile(entry_file, symbol, entry_symbol);
	
	return SUCCEED_TO_CREATE_FILES;
}

static enum outfiles_status HandleExtern (FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, size_t line_number, 
                                          size_t address_to_write, size_t *data_counter, const char *delim)
{
	return SUCCEED_TO_CREATE_FILES;
}

static enum outfiles_status HandleAttribute(FILE *data_file, FILE *entry_file, symbol_table_t *symbol_table, const char *line, char *attribute, size_t line_number, 
																		        size_t *instruction_counter, size_t final_instruction_counter, size_t *data_counter, const char *delim)
{
	enum outfiles_status handle_attribute_status = SUCCEED_TO_CREATE_FILES;
	struct attribute_handlers handle[] = {
											 						 {".data", HandleData},
					                         {".string", HandleString},
					                         {".entry", HandleEntry},
					                         {".extern", HandleExtern}
	                                     };
	int i = 0;
	
	for(i = 0; i < sizeof(handle) / sizeof(*handle); ++i)
	{
		if(strcmp(attribute, handle[i].attribute_name) == 0)
		{
			handle_attribute_status = handle[i].handle_func(data_file, entry_file, symbol_table, line, line_number, 
			                                                final_instruction_counter + *data_counter, data_counter, delim);
			break;
		}
	}
	
	return handle_attribute_status;
}

enum outfiles_status HandleCommand(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, char *line, size_t line_number, 
                                   size_t *instruction_counter, const char *delim)
{
	enum outfiles_status handle_command_status = SUCCEED_TO_CREATE_FILES;
	int command = GetCommand(line);
	unsigned int opcode = GetOpcode(command);

	WriteOpcodeToObject(object_file, instruction_counter, opcode);

	handle_command_status = WriteFunctAndAddressingInformationToObject(object_file, external_file, symbol_table, instruction_counter, command, line_number, delim);

	return handle_command_status;
}
