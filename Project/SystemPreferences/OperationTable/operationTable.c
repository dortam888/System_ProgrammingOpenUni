#include <stddef.h> 
#include <string.h>

#include "operationTable.h"


struct operations
{
	const char *operation_name;
	unsigned int opcode:4;
	unsigned int funct:4;
	unsigned int src_addressing_method: 4;
	unsigned int dest_addressing_method:4;
};

static const operation_data_t operation_table[] = 
{
	{"mov", 0, NOFUNCT,   IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING, 
	                  	  DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"cmp", 1, NOFUNCT,   IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"add", 2, 10, 		  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						  DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"sub", 2, 11, 		  IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING,
						  DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"lea", 4, NOFUNCT,   DIRECT_ADDRESSING|INDEX_ADDRESSING,
						  DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"clr", 5, 10, 	 	  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"not", 5, 11, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"inc", 5, 12, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"dec", 5, 13, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"jmp", 9, 10, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"bne", 9, 11, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"jsr", 9, 12, 		  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING},
	{"red", 12, NOFUNCT,  NONE, DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"prn", 13, NOFUNCT,  NONE, IMMEDIATE_ADDRESSING|DIRECT_ADDRESSING|INDEX_ADDRESSING|REGISTER_ADDRESSING},
	{"rts", 14, NOFUNCT,  NONE, NONE},
	{"stop", 15, NOFUNCT, NONE, NONE}
};

static const size_t number_of_operations = sizeof(operation_table)/sizeof(*operation_table);

unsigned int GetOpcode(const char *operation_name)
{
	int i = 0;
	
	for (i = 0; i < number_of_operations; ++i)
	{
		if (strcmp(operation_table[i].operation_name, operation_name) == 0)
		{
			return operation_table[i].opcode;
		}
	}
	
	return ERROR;
}

unsigned int GetFunct(const char *operation_name)
{
	int i = 0;
	
	for (i = 0; i < number_of_operations; ++i)
	{
		if (strcmp(operation_table[i].operation_name, operation_name) == 0)
		{
			return operation_table[i].funct;
		}
	}
	
	return ERROR;
}

int IsSrcAdressingMethodLegal(const char *operation_name, enum addressing_methods src_addressing_method)
{
	int i = 0;
		
	for (i = 0; i < number_of_operations; ++i)
	{
		if (strcmp(operation_table[i].operation_name, operation_name) == 0)
		{
			break;
		}
	}
	
	return ((operation_table[i].src_addressing_method & src_addressing_method) == 0);
}

int IsDestAdressingMethodLegal(const char *operation_name, enum addressing_methods dest_addressing_method)
{
	int i = 0;
		
	for (i = 0; i < number_of_operations; ++i)
	{
		if (strcmp(operation_table[i].operation_name, operation_name) == 0)
		{
			break;
		}
	}
	
	return ((operation_table[i].dest_addressing_method & dest_addressing_method) == 0);
}
