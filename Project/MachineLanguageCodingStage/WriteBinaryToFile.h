#ifndef BINARY_H
#define BINARY_H

#include <stdio.h>
#include "../SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "../SymbolTableStage/SymbolTableCreate.h"

void WriteStringToFile(FILE *output_file, size_t address_to_write, const char *str, size_t *data_counter);

void WriteNumberToFile(FILE *output_file, size_t address_to_write, unsigned int number_to_write, enum word_attributes word_attribute);

void WriteOpcodeToObject(FILE *output_file, size_t *instruction_counter, unsigned int opcode);

int WriteFunctAndAddressingInformationToObject(FILE *output_file, FILE *external_file, symbol_table_t *symbol_table, size_t *instruction_counter, 
                                               int command, size_t line_number, const char *delim);


#endif /* BINARY_H */