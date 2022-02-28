#include "../../SymbolTableStage/SymbolTableCreate.h"

int main(int argc, char *argv[])
{
	symbol_table_t *sym_tab = NULL;
	const char *file_name = "testfiles/ps";
	
	sym_tab = CreateSymbolTable(file_name);
	PrintSymbolTable(sym_tab);
	
	return 0;
}
