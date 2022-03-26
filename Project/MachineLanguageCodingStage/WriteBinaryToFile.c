#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

#include "WriteBinaryToFile.h"
#include "../Parsing/parsing_asm_file.h"
#include "../SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "../SystemPreferences/OperationTable/operationTable.h"
#include "../SystemPreferences/AddressingMethods/addressing_methods.h"
#include "../utils/file_utils/fileUtils.h"

#define NUMBER_OF_CHARS_IN_GROUP 3
#define BITS_IN_GROUP 4

static int WriteOperandToFile(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, const char *operand, 
                       size_t line_number, size_t *instruction_counter, enum addressing_methods method);

struct addressing_method_coding
{
	char operand[MAX_LINE_LEN];
	enum addressing_methods method;
	unsigned int register_number: BITS_FOR_NUM_OF_REG;
};

void WriteStringToFile(FILE *data_file, size_t address_to_write, const char *str, size_t *data_counter)
{
	while (!IsEndOfString(str))
	{
		WriteNumberToFile(data_file, address_to_write, *str, ABSOLUTE);
		++str;
		++address_to_write;
		++(*data_counter);
	}

	WriteNumberToFile(data_file, address_to_write, *str, ABSOLUTE); /* Write '\0' */
	++(*data_counter);
}

static unsigned int NumForMasking(unsigned int num_of_bits)
{
	unsigned int mask = 0U;
	int i = 0;
	
	for (i = 0; i < num_of_bits; ++i)
	{
		mask = (mask << 1U) | 1U;
	}

	return mask;
}

void WriteNumberToFile(FILE *output_file, size_t address_to_write, unsigned int number_to_write, enum word_attributes word_attribute)
{
	char bit_group = 'A'; /* first bit group */
  char seperator = '-'; /* seperator char between groups of bits */
	int i = 0;
	
	number_to_write &= NumForMasking(BITS_FOR_IMMEDIATE);

	fprintf(output_file, "%04lu\t%c%x%c", address_to_write, bit_group, word_attribute, seperator);	
	++bit_group;
	
	for (i = 0; i < BITS_FOR_IMMEDIATE; i+=BITS_IN_GROUP)
	{
    if (i + BITS_IN_GROUP == BITS_FOR_IMMEDIATE)
    {
      seperator = '\n'; /* change seperator in last iteration to new line */
    }
		fprintf(output_file, "%c%x%c", bit_group, (number_to_write >> (BITS_FOR_IMMEDIATE - BITS_IN_GROUP - i)) & NumForMasking(BITS_IN_GROUP), seperator);
		++bit_group;
	}		
}

void WriteOpcodeToObject(FILE *output_file, size_t *instruction_counter, unsigned int opcode)
{
	unsigned int optext = 1U << opcode; /* turn on the bit in opcode */

	WriteNumberToFile(output_file, *instruction_counter, optext, ABSOLUTE);

	++(*instruction_counter);
}

static unsigned int SecondWordCode(unsigned int funct, struct addressing_method_coding *operand)
{
	if (operand[1].method == 0)
	{
		return funct << (FUNCT_PLACE_IN_WORD) | 
		       operand[0].register_number << (DEST_REGISTER_PLACE_IN_WORD) | 
		       AddressingMethodNumber(operand[0].method) << (DEST_ADDRESSING_METHOD_PLACE_IN_WORD);
	}

	return funct << (FUNCT_PLACE_IN_WORD) | 
				 operand[0].register_number << (SRC_REGISTER_PLACE_IN_WORD) | 
				 AddressingMethodNumber(operand[0].method) << (SRC_ADDRESSING_METHOD_PLACE_IN_WORD) | 
	       operand[1].register_number << (DEST_REGISTER_PLACE_IN_WORD) | 
				 AddressingMethodNumber(operand[1].method) << (DEST_ADDRESSING_METHOD_PLACE_IN_WORD);
}

static void WriteCommandCode(FILE *output_file, size_t *instruction_counter, unsigned int funct, struct addressing_method_coding *operand)
{
	unsigned int num_to_write = SecondWordCode(funct, operand);

	WriteNumberToFile(output_file, *instruction_counter, num_to_write, ABSOLUTE);
	++(*instruction_counter);
}

int WriteFunctAndAddressingInformationToObject(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, size_t *instruction_counter, 
                                               int command, size_t line_number, const char *delim)
{
	int i = 0;
	unsigned int funct = GetFunct(command);
	unsigned int num_of_operands = GetNumOfOperands(command);
	struct addressing_method_coding command_second_word[] = {{{'\0'}, NONE, 0}, {{'\0'}, NONE, 0}};

	for (i = 0; i < num_of_operands; ++i)
	{
		char *operand = NULL;

		if((operand = strtok(NULL, delim)) == NULL)
		{
			printf("Error - In line %u command %s expects %u parameters but gets only %d\n", (unsigned int)line_number, GetCommandName(command), num_of_operands, i);
			return -1;
		}
		
		command_second_word[i].method = GetAddressingMethod(operand); /*TODO check addressing method is legal for the command*/
		command_second_word[i].register_number = GetRegisterNumber(operand, command_second_word[i].method);
		strcpy(command_second_word[i].operand, operand);
	}

	if(strtok(NULL, delim) != NULL)
	{
		printf("Error - In line %u command %s expects %d operands but gets more\n", (unsigned int)line_number, GetCommandName(command), num_of_operands);
		return -1;
	}

	if(num_of_operands != 0)
	{
		WriteCommandCode(object_file, instruction_counter, funct, command_second_word);
		WriteOperandToFile(object_file, external_file, symbol_table, command_second_word[0].operand, line_number, instruction_counter, command_second_word[0].method);
		WriteOperandToFile(object_file, external_file, symbol_table, command_second_word[1].operand, line_number, instruction_counter, command_second_word[1].method);
	}

	return 0;
}

static int HandleImmediateAddressing(FILE *object_file, const char *operand, size_t *instruction_counter)
{
	WriteNumberToFile(object_file, ++(*instruction_counter), strtol(++operand, NULL, DECIMAL_BASE), ABSOLUTE);

	return 0;
}

static void WriteSymbolInExternalFile(FILE *externals_file, const char *operand, size_t instruction_counter)
{
	fprintf(externals_file, "%s\t%s\t%u\n", "BASE", operand, (unsigned int)instruction_counter);
	fprintf(externals_file, "%s\t%s\t%u\n", "OFFSET", operand, (unsigned int)(instruction_counter + 1));
}

static int HandleDirectAddressing(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, const char *operand, 
                                  size_t line_number, size_t *instruction_counter)
{
	enum word_attributes attribute = ABSOLUTE;	
	symbol_data_t *symbol = NULL;
	if ((symbol = FindSymbolInSymbolTable(symbol_table, operand)) == NULL)
	{
		printf("Error - In line %u symbol %s is undefined\n", (unsigned int)line_number, operand);
		return -1;
	}

	attribute = (GetAttribute(symbol) == EXTERN)? EXTERNAL : RELATIVE;
	if(attribute == EXTERNAL)
	{
		WriteSymbolInExternalFile(external_file, operand, *instruction_counter);
	}

	WriteNumberToFile(object_file, (*instruction_counter)++, GetBaseAddress(symbol), attribute);
	WriteNumberToFile(object_file, (*instruction_counter)++, GetOffset(symbol), attribute);
	
	return 0;
}

int HandleIndexAddressing(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, const char *operand, 
                          size_t line_number, size_t *instruction_counter)
{
	char label[MAX_LINE_LEN] = {'\0'};
	GetLabelFromIndexOperand(label, operand);

	return HandleDirectAddressing(object_file, external_file, symbol_table, label, line_number, instruction_counter);
}

static int WriteOperandToFile(FILE *object_file, FILE *external_file, symbol_table_t *symbol_table, const char *operand, 
                       size_t line_number, size_t *instruction_counter, enum addressing_methods method)
{
	int writing_status = 0;

	switch(method)
	{
		case IMMEDIATE_ADDRESSING:
			writing_status = HandleImmediateAddressing(object_file, operand, instruction_counter);
			break;
		case DIRECT_ADDRESSING:
			writing_status = HandleDirectAddressing(object_file, external_file, symbol_table, operand, line_number, instruction_counter);
			break;
		case INDEX_ADDRESSING:
			writing_status = HandleIndexAddressing(object_file, external_file, symbol_table, operand, line_number, instruction_counter);
			break;
		case REGISTER_ADDRESSING:
			return 0;
		case NONE:
			return 0;
		default:
			return -1;
	}

	return writing_status;
}