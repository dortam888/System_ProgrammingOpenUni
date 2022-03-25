#include <stdio.h> /* FILE printf */
#include <string.h> /* strtok */
#include <assert.h> /* assert */

#include "SymbolTableCreate.h" 
#include "../utils/file_utils/fileUtils.h"
#include "../utils/data_structures/vector.h"
#include "../SystemPreferences/OperationTable/operationTable.h"
#include "../SystemPreferences/AddressingMethods/addressing_methods.h"
#include "../SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "../Parsing/parsing_asm_file.h"

#define SYMBOL_TABLE_INITIAL_SIZE 10
#define ERROR_IN_SYMBOL_TABLE_CREATION 0xBADBAD

struct symbol_data
{
	char symbol_name[MAX_LABEL_NAME];
	size_t address_value;
	size_t base_address;
	symbol_attribute_t attribute;
	unsigned int offset: OFFSET_BITS;
};

struct attribute_handlers
{
	const char *attribute_name;
	size_t(*handle_func)(symbol_table_t *, symbol_data_t *,const char *, size_t, 
											 size_t, size_t *, const char *, int *);
};

static size_t HandleLabel(d_vector_t *symbol_table, const char *line, 
                          char *label, const char *delim, size_t line_number, 
													size_t instruction_counter, size_t *data_counter);

static size_t HandleNotLabel(d_vector_t *symbol_table, const char *line, 
                             char *part_line, const char *delim, 
														 size_t line_number, size_t instruction_counter, 
														 size_t *data_counter);

static size_t HandleAttribute(symbol_table_t *symbol_table, symbol_data_t *symbol, 
                              const char *line, char *part, size_t line_number, 
                              size_t instruction_counter, const char* delim, 
															size_t *data_counter, int *status);

static size_t HandleCommand(symbol_data_t *symbol, char *line, size_t line_number, 
                            size_t instruction_counter, const char* delim);

static void FixDataAttributesInTable(symbol_table_t *symbol_table, int instruction_counter);

symbol_attribute_t GetAttribute(symbol_data_t *symbol)
{
	assert(symbol != NULL);

	return symbol -> attribute;
}

unsigned int GetBaseAddress(symbol_data_t *symbol)
{
	assert(symbol != NULL);

	return symbol -> base_address;
}

unsigned int GetOffset(symbol_data_t *symbol)
{
	assert(symbol != NULL);
	
	return symbol -> offset;
}

void FreeSymbolTable(symbol_table_t *symbol_table)
{
	DVectorDestroy(symbol_table);
}

static void *HandleError(symbol_table_t *symbol_table, const char *input_file_name, FILE *input_file)
{
		FreeSymbolTable(symbol_table);
		fclose(input_file);
		remove(input_file_name);
		return NULL;
}

symbol_table_t* CreateSymbolTable(const char *file_name, size_t *instruction_counter, size_t *data_counter)
{
	FILE *input_file = NULL;
	char input[MAX_FILE_NAME] = {'\0'};
	char line[MAX_LINE_LEN] = {'\0'};
	size_t line_number = 0;
	const char *delim = ", \n\t\v\r\f";
	symbol_table_t *symbol_table = NULL;
	int symbol_table_creation_status = 0;
	
	assert(file_name != NULL);
	assert(instruction_counter != NULL);
	assert(data_counter != NULL);

	symbol_table = DVectorCreate(sizeof(symbol_data_t), SYMBOL_TABLE_INITIAL_SIZE);
	if (symbol_table == NULL)
	{
		printf("OS Error - Couldn't Allocate Symbol Table\n");
		return NULL;
	}
	
	PutEndingToFileName(input, file_name, ".am");
	
	input_file = fopen(input, "r");
	if (input_file == NULL)
	{
		printf("couldn't open file %s\n", input);
		FreeSymbolTable(symbol_table);
		return NULL;
	}
	
	while(fgets(line, MAX_LINE_LEN, input_file))
	{
		size_t(*handle_func[])(symbol_table_t *, const char *, char *, const char *, size_t, size_t, size_t *) = {HandleNotLabel, HandleLabel};
		char *part_of_line = NULL;
		char copy_line[MAX_LINE_LEN] = {'\0'};

		strcpy(copy_line, line); 
		++line_number;
		part_of_line = strtok(line, delim);

		if (part_of_line == NULL || IsComment(part_of_line))
		{
			continue;
		}
		
		if(!IsSentenceCorrect(copy_line))
		{
			printf("in line %zu\n", line_number);
			continue;
		}
		
		*instruction_counter = handle_func[IsLabel(part_of_line)](symbol_table, copy_line, part_of_line, delim, line_number, *instruction_counter, data_counter);
		
		if (*instruction_counter == ERROR_IN_SYMBOL_TABLE_CREATION)
		{	
			symbol_table_creation_status = 1;
		}
	}

	if(symbol_table_creation_status != 0)
	{
		return HandleError(symbol_table, input, input_file);
	}

	if (*instruction_counter + *data_counter > MEMORY_SIZE_IN_WORDS)
	{
		printf("Error - assembly code is exceeding memory limitation\n");
		return HandleError(symbol_table, input, input_file);
	}
	
	fclose(input_file);
	FixDataAttributesInTable(symbol_table, *instruction_counter);
	
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

symbol_data_t *FindSymbolInSymbolTable(symbol_table_t *symbol_table, const char *symbol_name)
{
	return DVectorFind(symbol_table, symbol_name, ComapreSymbolNames);
}

static int IsSymbolLegal(symbol_table_t *symbol_table, const char *symbol_name, size_t line_number)
{
	if(IsSavedWord(symbol_name))
	{
		printf("Error - in line %zu Label name %s is a system saved name and can't be used as a symbol\n", line_number, symbol_name);
		return 0;
	}

	if (FindSymbolInSymbolTable(symbol_table, symbol_name) != NULL)
	{
		printf("Error - Conflict in symbols. Label name %s defined in line %zu is already defined\n", symbol_name, line_number);
		return 0;
	}

	return 1;
}

static size_t HandleLabel(symbol_table_t *symbol_table, const char *line, char *label, const char *delim, 
                   size_t line_number, size_t instruction_counter, size_t *data_counter)
{
	char *current_part_of_line = NULL;
	symbol_data_t new_symbol;
	char label_name[MAX_LABEL_NAME] = {'\0'};
	int label_status = 0;

	assert(strlen(label) < MAX_LABEL_NAME);
	
	GetLabelName(label_name, label);
	
	if(!IsSymbolLegal(symbol_table, label_name, line_number))
	{
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
	
	InitiateNewSymbol(&new_symbol, label_name, instruction_counter, instruction_counter % WORD_BITS, INIT_ATR);
	
	current_part_of_line = strtok(NULL, delim);
	instruction_counter = IsAttribute(current_part_of_line)? 
	                      HandleAttribute(symbol_table, &new_symbol, line, current_part_of_line, line_number, 
												                instruction_counter, delim, data_counter, &label_status) : 
	                      HandleCommand(&new_symbol, current_part_of_line, line_number, instruction_counter, delim);

	if (label_status == 0 && DVectorPushBack(symbol_table, &new_symbol) != 0)
	{
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
	
	return instruction_counter;
}

static size_t HandleNotLabel(symbol_table_t *symbol_table, const char *line, char *part_of_line, const char *delim, 
                             size_t line_number, size_t instruction_counter, size_t *data_counter)
{	
	instruction_counter = IsAttribute(part_of_line)? HandleAttribute(symbol_table, NULL, line, part_of_line, line_number, 
	                                                                 instruction_counter, delim, data_counter, 0) : 
                                                   HandleCommand(NULL, part_of_line, line_number, instruction_counter, delim);
                                                             
  return instruction_counter;
}

static size_t SymHandleData(symbol_table_t *symbol_table, symbol_data_t *symbol, const char *line, size_t line_number, 
                            size_t instruction_counter, size_t *data_counter, const char *delim, int *status)
{	
	if (symbol != NULL)
	{
		symbol -> attribute = DATA;
		symbol -> address_value = *data_counter;
	}
			
	while(strtok(NULL, delim) != NULL)
	{
		++(*data_counter);
	}
	
	return instruction_counter;
}

static size_t SymHandleString(symbol_table_t *symbol_table, symbol_data_t *symbol, const char *line, size_t line_number, 
                              size_t instruction_counter, size_t *data_counter, const char *delim, int *status)
{  
	if (symbol != NULL)
	{
		symbol -> attribute = DATA;
		symbol -> address_value = *data_counter;
	}
	
	while (!IsStartOfString(*line))
	{
		++line; /* move to next letter until start of string */
	}

	++line; /* skip " which is start of string */

	while(!IsEndOfString(line))
	{
		if (*line == '\\')
		{
			++line; /* backslash is special sign and i skip it */
		}
		++(*data_counter); /* updata data counter until end of string */
		++line;
	}

	++(*data_counter); /* for \0 */

	return instruction_counter;
}

static size_t SymHandleEntry(symbol_table_t *symbol_table, symbol_data_t *symbol, const char *line, size_t line_number, 
                             size_t instruction_counter, size_t *data_counter, const char *delim, int *status)
{
	if (symbol != NULL)
	{
		printf("Warning - in line %zu label %s is before .entry qualifier. the label will not be added to symbol_table\n", line_number, symbol -> symbol_name);
		*status = 1; /* don't push the label to symbol table */
	}
	
	return instruction_counter;
}

static size_t SymHandleExtern(symbol_table_t *symbol_table, symbol_data_t *symbol, const char *line, size_t line_number, 
                              size_t instruction_counter, size_t *data_counter, const char *delim, int *status)
{
	char *symbol_name = NULL;
	symbol_data_t new_symbol;
	
	if (symbol != NULL)
	{
		printf("Warning - in line %zu label %s is before .extern qualifier. the label will not be added to symbol_table\n", line_number, symbol -> symbol_name);
		*status = 1; /* don't push the label to symbol table */
	}
	
	symbol_name = strtok(NULL, delim);

	if(!IsSymbolLegal(symbol_table, symbol_name, line_number))
	{
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}

	InitiateNewSymbol(&new_symbol, symbol_name, 0, 0, EXTERN);
	DVectorPushBack(symbol_table, &new_symbol);	
	
	return instruction_counter;
}

static size_t HandleAttribute(symbol_table_t *symbol_table, symbol_data_t *symbol, const char *line, char *attribute, size_t line_number, 
                              size_t instruction_counter, const char *delim, size_t *data_counter, int *status)
{
	struct attribute_handlers handle[] = {
																				{".data", SymHandleData},
																				{".string", SymHandleString},
																				{".entry", SymHandleEntry},
																				{".extern", SymHandleExtern}
	                                     };
	int i = 0;
	
	for(i = 0; i < sizeof(handle) / sizeof(*handle); ++i)
	{
		if(strcmp(attribute, handle[i].attribute_name) == 0)
		{
			return handle[i].handle_func(symbol_table, symbol, line, line_number, instruction_counter, data_counter, delim, status);
		}
	}
	
	printf("Error - In line %zu No Such Attribute %s\n", line_number, attribute);
	
	return instruction_counter;
}

static size_t ParseCommand(char *line, size_t line_number, size_t instruction_counter, const char *delim)
{
	enum addressing_methods address_method = NONE;

	assert(line != NULL);

	if(GetCommand(line) == ERROR)
	{
		printf("Error - In line %zu Command %s doesn't exist\n", line_number, line);
		return ERROR_IN_SYMBOL_TABLE_CREATION;
	}
		
	while((line = strtok(NULL, delim)) != NULL)
	{
		address_method = GetAddressingMethod(line);
		instruction_counter += GetWordsForAddressMethod(address_method);
	}
	
	return instruction_counter + 1 + (address_method != NONE); /* +1 for opcode word and if there are parameters another word for them else no addiotional word needed*/
}

static size_t HandleCommand(symbol_data_t *symbol, char *line, size_t line_number, size_t instruction_counter, const char *delim)
{
	if(symbol != NULL)
	{
		symbol -> attribute = CODE;
	}
	
	return ParseCommand(line, line_number, instruction_counter, delim);
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

#ifdef NDBUG
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
#endif
