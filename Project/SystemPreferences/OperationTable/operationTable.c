#include <stddef.h> 
#include <string.h>
#include <assert.h>

#include "operationTable.h"


struct operations
{
	const char *operation_name;
	unsigned int opcode: 4;
	unsigned int funct: 4;
	unsigned int num_of_operations: 2;
	unsigned int src_addressing_method: 4;
	unsigned int dest_addressing_method: 4;
};

static const operation_data_t operation_table[] = 
{
	{"mov", 0, NOFUNCT,   2,  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING, 
	                  	      DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"cmp", 1, NOFUNCT,   2,  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						                IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"add", 2, 10, 		    2,  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						                DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"sub", 2, 11, 		    2,  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						                DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"lea", 4, NOFUNCT,   2,  DIRECT_ADDRESSING|INDEX_ADDRESSING,
						                DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"clr", 5, 10, 	 	    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"not", 5, 11, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"inc", 5, 12, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"dec", 5, 13, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"jmp", 9, 10, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"bne", 9, 11, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"jsr", 9, 12, 		    1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"red", 12, NOFUNCT,  1,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"prn", 13, NOFUNCT,  1,  NONE, IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"rts", 14, NOFUNCT,  0,  NONE, NONE},
	{"stop", 15, NOFUNCT, 0,  NONE, NONE}
};

static const size_t number_of_operations = sizeof(operation_table)/sizeof(*operation_table);

int GetCommand(const char *operation_name)
{
	int i = 0;
	
	assert(operation_name != NULL);

	for (i = 0; i < number_of_operations; ++i)
	{
		if (strcmp(operation_table[i].operation_name, operation_name) == 0)
		{
			return i;
		}
	}
	
	return ERROR;
}

const char *GetCommandName(int command)
{
	return operation_table[command].operation_name;
}

unsigned int GetOpcode(int command)
{
	assert(command >= 0);
	assert(command < number_of_operations);

	return operation_table[command].opcode;
}

unsigned int GetFunct(int command)
{
	assert(command >= 0);
	assert(command < number_of_operations);
	
	return operation_table[command].funct;
}

int IsSrcAdressingMethodLegal(int command, enum addressing_methods src_addressing_method)
{
	assert(command >= 0);
	assert(command < number_of_operations);
	
	return ((operation_table[command].src_addressing_method & src_addressing_method) != 0);
}

int IsDestAdressingMethodLegal(int command, enum addressing_methods dest_addressing_method)
{
	assert(command >= 0);
	assert(command < number_of_operations);
	
	return ((operation_table[command].dest_addressing_method & dest_addressing_method) != 0);
}

unsigned int GetNumOfOperands(int command)
{
	assert(command >= 0);
	assert(command < number_of_operations);

	return operation_table[command].num_of_operations;
}