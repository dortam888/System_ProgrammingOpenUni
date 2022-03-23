#include "../../SymbolTableStage/SymbolTableCreate.h"
#include "../../MachineLanguageCodingStage/ObjectFileCreate.h"

int main(int argc, char *argv[])
{
	symbol_table_t *sym_tab = NULL;
	const char *file_name = "testfiles/test_normal";
	size_t instruction_counter = 100;
	size_t data_counter = 0;
	
	sym_tab = CreateSymbolTable(file_name, &instruction_counter, &data_counter);
	
	CreateOutputFiles(file_name, sym_tab, instruction_counter);
	
	return 0;
}
