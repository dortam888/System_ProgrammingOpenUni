#ifndef PREASSEMBLER
#define PREASSEMBLER

#include "../utils/data_structures/vector.h"

typedef d_vector_t macro_table_t;

enum preassembly_stage_status {SUCCESS, COULDNT_FIND_INPUT_FILE, OS_ERROR};

macro_table_t *CreateMacroTable(size_t initial_size_of_table);

void FreeMacroTable(macro_table_t *macro_table);

int PushMacroToTable(macro_table_t *macro_table, void *macro);

void *FindMacroInTable(macro_table_t *macro_table, const void *data_to_find, int(*compare_func)(const void *,const void *));

enum preassembly_stage_status CreateAmFile(const char *file_name);

#endif /*PREASSEMBLER*/
