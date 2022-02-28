#include "PreAssembler.h"
#include "SymbolTableCreate.h"

int main(int argc, char* argv[])
{
	const char *file_name = "ps";
	
	CreateAmFile(file_name);
	PrintSymbolTable(CreateSymbolTable(file_name));
	
	return 0;
}
