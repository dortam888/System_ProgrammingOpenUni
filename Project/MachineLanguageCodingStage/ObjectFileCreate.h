#ifndef OBJ_H
#define OBJ_H

#include "../SymbolTableStage/SymbolTableCreate.h"

enum outfiles_status {SUCCEED_TO_CREATE_FILES, FAILED_TO_CREATE_FILES};

enum outfiles_status CreateObjectFile(symbol_table_t *symbol_table, size_t final_instruction_counter);






#endif /* OBJ_H */
