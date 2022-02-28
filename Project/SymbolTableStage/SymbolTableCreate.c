#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>

#include "SymbolTableCreate.h"
#include "../utils/file_utils/fileUtils.h"
#include "../utils/data_structures/vector.h"
#include "../SystemPreferences/OperationTable/operationTable.h"
#include "../SystemPreferences/AddressingMethods/addressing_methods.h"
#include "../SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "../Parsing/parsing_asm_file.h"

#define SYMBOL_TABLE_INITIAL_SIZE 10
#define ERROR_IN_SYMBOL_TABLE_CREATION -1

typedef struct symbol_data
{
	char symbol_name[MAX_LINE_LEN];
	size_t address_value;
	size_t base_address;
	symbol_attribute_t attribute;
	unsigned int offset: OFFSET_BITS;
} symbol_data_t;

struct attribute_handlers
{
	const char *attribute_name;
	ssize_t(*handle_func)(symbol_table_t *symbol_table, symbol_data_t *symbol, size_t instruction_counter, size_t *data_counter);
};

static ssize_t HandleLabel(d_vector_t *symbol_table, char *line, const char *delim, size_t line_number, size_t instruction_counter, size_t *data_counter);
static ssize_t HandleNotLabel(d_vector_t *symbol_table, char *line, const char *delim, size_t line_number, size_t instruction_counter, size_t *data_counter);
static ssize_t HandleAttribute(symbol_table_t *symbol_table, symbol_data_t *symbol, char *line, size_t instruction_counter, const char* delim, size_t *data_counter);
static ssize_t HandleCommand(symbol_data_t *symbol, char *line, size_t instruction_counter, const char* delim);
static void FixDataAttributesInTable(symbol_table_t *symbol_table, int instruction_counter);

symbol_table_t* CreateSymbolTable(const char *file_name)
{
	symbol_table_t *symbol_table = DVectorCreate(sizeof(symbol_data_t), SYMBOL_TABLE_INITIAL_SIZE);
	size_t instruction_counter = START_ADDRESS_OF_INSTRUCTION_COUNT;
	size_t data_counter = 0;
	FILE *input_file = NULL;
	char input[MAX_FILE_NAME] = {'\0'};
	char line[MAX_LINE_LEN] = {'\0'};
	size_t line_number = 0;
	const char *delim = ", \n\t";
	
	PutEndingToFileName(input, file_name, ".am");
	
	input_file = fopen(input, "r");
	if (input_file == NULL)
	{
		printf("couldn't open file %s\n", input);
		return NULL;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		ssize_t(*handle_func[])(symbol_table_t *, char *, const char *, size_t, size_t, size_t *) = {HandleNotLabel, HandleLabel};
		
		++line_number;
		strtok(line, " \t\n");
		
		if (IsComment(line))
		{
			continue;
		}
		
		instruction_counter = handle_func[IsLabel(line)](symbol_table, line, delim, line_number, instruction_counter, &data_counter);
		
		if (instruction_counter == ERROR_IN_SYMBOL_TABLE_CREATION)
		{	
			DVectorDestroy(symbol_table);
			fclose(input_file);
			return NULL;
		}
	}
	
	fclose(input_file);
	
	FixDataAttributesInTable(symbol_table, instruction_counter);
	
	return symbol_table;
}

static void InitiateNewSymbol(symbol_data_t *new_symbol, const char *symbol_name, size_t address_value, unsigned int offset, symbol_attribute_t attribute)
{
	strcpy(new_symbol -> symbol_name, symbol_name);
	new_symbol -> address_value = address_value;
	new_symbol -> offset = offset;
	new_symbol -> base_address = address_value - offset;
	new_symbol -> attribute = attribute;
}

static int ComapreSymbolNames(const void *symbol_data, const void *symbol_name)
{
	return strcmp((const char *)symbol_name, ((const symbol_data_t *)symbol_data) -> symbol_name);
}

void *FindSymbolInSymbolTable(symbol_table_t *symbol_table, const char *symbol_name)
{
	return DVectorFind(symbol_table, symbol_name, ComapreSymbolNames);
}

ssize_t HandleLabel(symbol_table_t *symbol_table, char *line, const char *delim, size_t line_number, size_t instruction_counter, size_t *data_counter)
{
	/*variables*/
	char *current_part_of_line = NULL;
	symbol_data_t new_symbol;
	char label_name[MAX_LINE_LEN] = {'\0'};
	
	/*function*/
	GetLabelName(label_name, line);
		
	if (FindSymbolInSymbolTable(symbol_table, label_name) != NULL)
	{
		printf("Error Conflict in symbols. Label name %s defined in line %lu is already defined\n", label_name, line_number);
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
	
	InitiateNewSymbol(&new_symbol, label_name, instruction_counter, instruction_counter % WORD_BITS, INIT_ATR);
	
	current_part_of_line = strtok(NULL, delim);
	instruction_counter = IsAttribute(current_part_of_line)? HandleAttribute(symbol_table, &new_symbol, current_part_of_line, instruction_counter, delim, data_counter) : 
	                                                         HandleCommand(&new_symbol, current_part_of_line, instruction_counter, delim);
	if (DVectorPushBack(symbol_table, &new_symbol) == -1)
	{
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
	
	return instruction_counter;
}

ssize_t HandleNotLabel(symbol_table_t *symbol_table, char *line, const char *delim, size_t line_number, size_t instruction_counter, size_t *data_counter)
{	
	instruction_counter = IsAttribute(line)? HandleAttribute(symbol_table, NULL, line, instruction_counter, delim, data_counter) : 
                                                             HandleCommand(NULL, line, instruction_counter, delim);
                                                             
    return instruction_counter;
}

ssize_t HandleData(symbol_table_t *symbol_table, symbol_data_t *symbol, size_t instruction_counter, size_t *data_counter)
{	
	if (symbol != NULL)
	{
		symbol -> attribute = DATA;
		symbol -> address_value = *data_counter;
	}
			
	while(strtok(NULL, ", \t\n") != NULL)
	{
		++(*data_counter);
	}
	
	return instruction_counter;
}

ssize_t HandleString(symbol_table_t *symbol_table, symbol_data_t *symbol, size_t instruction_counter, size_t *data_counter)
{
	char *current_part_of_line = NULL;
	size_t str_length = 0;
	
	if (symbol != NULL)
	{
		symbol -> attribute = DATA;
		symbol -> address_value = *data_counter;
	}
	
	current_part_of_line = strtok(NULL, " \t\n“\"");
	str_length = strlen(current_part_of_line); 
	
	*data_counter = str_length + 1; /* +1 for ending character '\0' */
	
	return instruction_counter;
}

ssize_t HandleEntry(symbol_table_t *symbol_table, symbol_data_t *symbol, size_t instruction_counter, size_t *data_counter)
{
	if (symbol != NULL)
	{
		printf("warning label before .entry qualifier\n");
	}
	
	return instruction_counter;
}

ssize_t HandleExtern(symbol_table_t *symbol_table, symbol_data_t *symbol, size_t instruction_counter, size_t *data_counter)
{
	char *current_part_of_line = NULL;
	symbol_data_t new_symbol;
	
	if (symbol != NULL)
	{
		printf("warning label before .extern qualifier\n");
	}
	
	current_part_of_line = strtok(NULL, ", \t\n");
	InitiateNewSymbol(&new_symbol, current_part_of_line, 0, 0, EXTERN);
	
	DVectorPushBack(symbol_table, &new_symbol);	
	
	return instruction_counter;
}

ssize_t HandleAttribute(symbol_table_t *symbol_table, symbol_data_t *symbol, char *line, size_t instruction_counter, const char *delim, size_t *data_counter)
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
			return handle[i].handle_func(symbol_table, symbol, instruction_counter, data_counter);
		}
	}
	
	printf("Error - No Such Attribute %s\n", line);
	
	return instruction_counter;
}

ssize_t ParseCommand(char *line, size_t instruction_counter, const char *delim)
{
	enum addressing_methods address_method = NONE;
	
	if(GetOpcode(line) == ERROR)
	{
		printf("Command %s doesn't exist\n", line);
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
		
	while((line = strtok(NULL, delim)) != NULL)
	{
		address_method = GetAddressingMethod(line);
		instruction_counter += GetWordsForAddressMethod(address_method);
	}
	
	return instruction_counter + 1 + (address_method != NONE); /* +1 for opcode word and if there are parameters another word for them else no addiotional word needed*/
}

ssize_t HandleCommand(symbol_data_t *symbol, char *line, size_t instruction_counter, const char *delim)
{
	if(symbol != NULL)
	{
		symbol -> attribute = CODE;
	}
	
	return ParseCommand(line, instruction_counter, delim);
}

static void FixDataAttributesInTable(symbol_table_t *symbol_table, int instruction_counter)
{
	int i = 0;
	
	for (i = 0; i < DVectorSize(symbol_table); ++i)
	{
		symbol_data_t *symbol = DVectorGetItemAddress(symbol_table, i);
		
		if (symbol -> attribute == DATA)
		{
			symbol -> address_value += instruction_counter;
			symbol -> offset = symbol -> address_value % WORD_BITS;
			symbol -> base_address = symbol -> address_value - symbol -> offset;
		}
	}
}

void PrintSymbolTable(symbol_table_t *symbol_table)
{
	int i = 0;
	
	for (i = 0; i < DVectorSize(symbol_table); ++i)
	{
		symbol_data_t *new_symbol = DVectorGetItemAddress(symbol_table, i);
		printf("%5s	%5lu   %5lu	%5d	 %5d\n", new_symbol -> symbol_name, 
		                                  new_symbol -> address_value, 
		                                  new_symbol -> base_address, 
		                                  new_symbol -> attribute, 
		                                  new_symbol -> offset);
	}
}			
