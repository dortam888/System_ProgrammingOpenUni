main_dir = ../../
vector = $(main_dir)utils/data_structures/vector.h
parse = $(main_dir)Parsing/parsing_asm_file.h
file = $(main_dir)utils/file_utils/fileUtils.h
gensys = $(main_dir)SystemPreferences/GeneralSystemPreferences/system_preferences.h
adrs = $(main_dir)SystemPreferences/AddressingMethods/addressing_methods.h
op_table = $(main_dir)SystemPreferences/OperationTable/operationTable.h
pre = $(main_dir)PreAssemblyStage/PreAssembler.h
sym = $(main_dir)SymbolTableStage/SymbolTableCreate.h

TestSymbolTable: vector.o parsing.o file.o sys_pref.o addressing.o operationTable.o PreAssembler.o  SymbolTableCreate.o  SymbolTableTest.c
	gcc -g -ansi -Wall -pedantic *.o SymbolTableTest.c -o TestSymbolTable

SymbolTableCreate.o: $(main_dir)SymbolTableStage/SymbolTableCreate.c $(sym) $(file) $(op_table) $(vector) $(gensys) $(adrs) $(parse)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)SymbolTableStage/SymbolTableCreate.c -o SymbolTableCreate.o
	
PreAssembler.o: $(main_dir)PreAssemblyStage/PreAssembler.c $(pre) $(vector) $(file)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)PreAssemblyStage/PreAssembler.c
		
operationTable.o: $(main_dir)SystemPreferences/OperationTable/operationTable.c $(op_table) $(adrs)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)SystemPreferences/OperationTable/operationTable.c -o operationTable.o
	
addressing.o: $(main_dir)SystemPreferences/AddressingMethods/addressing_methods.c $(adrs) $(parse) $(gensys)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)SystemPreferences/AddressingMethods/addressing_methods.c -o addressing.o

sys_pref.o: $(main_dir)SystemPreferences/GeneralSystemPreferences/system_preferences.c $(gensys)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)SystemPreferences/GeneralSystemPreferences/system_preferences.c -o sys_pref.o
	
file.o: $(main_dir)utils/file_utils/fileUtils.c $(file)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)utils/file_utils/fileUtils.c -o file.o
	
parsing.o: $(main_dir)Parsing/parsing_asm_file.c $(parse)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)Parsing/parsing_asm_file.c -o parsing.o
	
vector.o: $(main_dir)utils/data_structures/vector.c $(vector)
	gcc -g -ansi -Wall -pedantic -c $(main_dir)utils/data_structures/vector.c
	
clear:
	rm *.o TestSymbolTable
