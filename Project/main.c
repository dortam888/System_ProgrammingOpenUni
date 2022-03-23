#include <stdio.h>

#include "PreAssemblyStage/PreAssembler.h"
#include "SymbolTableStage/SymbolTableCreate.h"
#include "SystemPreferences/GeneralSystemPreferences/system_preferences.h"
#include "MachineLanguageCodingStage/ObjectFileCreate.h"

struct assembler
{
	size_t instruction_counter;
	size_t data_counter;
	symbol_table_t *symbol_table;
};

void UsageInfo();

int main(int argc, char* argv[])
{
	int i = 1;

	if (argc == 1)
	{
		UsageInfo();
	}

	for(; i < argc; ++i)
	{
		struct assembler new_assembler = {START_ADDRESS_OF_INSTRUCTION_COUNT, 0, NULL};
		
		if (CreateAmFile(argv[i]) != SUCCESS)
		{
			return -1;
		}
		new_assembler.symbol_table = CreateSymbolTable(argv[i], &new_assembler.instruction_counter, &new_assembler.data_counter);
		if(new_assembler.symbol_table == NULL)
		{
			return -1;
		}
		CreateOutputFiles(argv[i], new_assembler.symbol_table, new_assembler.instruction_counter);
	}
	
	return 0;
}

void UsageInfo()
{
	printf("To use the assembler enter the command 'assembler' with all one or multiple assembly files with the end .as\n \
	        Available Flags: -E -S\n \
					-E\tStops the assembler after the preprocessing stage\n \
					-S\tStop the assembler after before assembling and creating the code out files");
}