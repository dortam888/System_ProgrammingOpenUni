#ifndef SYMBOL_H
#define SYMBOL_H

#include "../utils/data_structures/vector.h"

typedef d_vector_t symbol_table_t;
typedef enum symbol_attribute {ENTRY = 0, EXTERN = 1, CODE = 2, DATA = 4, INIT_ATR = -1} symbol_attribute_t;

symbol_table_t* CreateSymbolTable(const char *file_name);

void *FindSymbolInSymbolTable(symbol_table_t *symbol_table, const char *symbol_name);

void PrintSymbolTable(symbol_table_t *symbol_table);	


#endif /*SYMBOL_H*/
