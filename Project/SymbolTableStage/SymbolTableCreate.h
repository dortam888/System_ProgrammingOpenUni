#ifndef SYMBOL_H
#define SYMBOL_H

#define MAX_LABEL_NAME 32

#include "../utils/data_structures/vector.h"

typedef d_vector_t symbol_table_t;
typedef struct symbol_data symbol_data_t;
typedef enum symbol_attribute {ENTRY = 1, EXTERN = 2, CODE = 4, DATA = 8, INIT_ATR = -1} symbol_attribute_t;

symbol_table_t* CreateSymbolTable(const char *file_name, size_t *instruction_counter, size_t *data_counter);

void FreeSymbolTable(symbol_table_t *symbol_table);

symbol_data_t *FindSymbolInSymbolTable(symbol_table_t *symbol_table, const char *symbol_name);

symbol_attribute_t GetAttribute(symbol_data_t *symbol);

unsigned int GetBaseAddress(symbol_data_t *symbol);

unsigned int GetOffset(symbol_data_t *symbol); 


#ifdef NDBUG
void PrintSymbolTable(symbol_table_t *symbol_table);	
#endif

#endif /*SYMBOL_H*/
