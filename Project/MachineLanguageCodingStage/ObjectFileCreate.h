#ifndef OBJ_H
#define OBJ_H

#include "../SymbolTableStage/SymbolTableCreate.h"

enum outfiles_status {SUCCEED_TO_CREATE_FILES, FAILED_TO_CREATE_FILES};

enum outfiles_status CreateOutputFiles(const char *file_name, symbol_table_t *symbol_table, size_t final_instruction_counter, size_t final_data_counter);


#endif /* OBJ_H */
