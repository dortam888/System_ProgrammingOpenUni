#include "../../SymbolTableStage/SymbolTableCreate.h"

int main(int argc, char *argv[])
{
	symbol_table_t *sym_tab = NULL;
	const char *file_name = "testfiles/ps";
	unsigned long instruction_counter = 100;
	unsigned long data_counter = 0;
	
	sym_tab = CreateSymbolTable(file_name, &instruction_counter, &data_counter);
	PrintSymbolTable(sym_tab);
	
	FreeSymbolTable(sym_tab);
	
	return 0;
}
