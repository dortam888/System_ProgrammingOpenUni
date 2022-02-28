vector = utils/data_structures/vector.h
parse = Parsing/parsing_asm_file.h
file = utils/file_utils/fileUtils.h
gensys = SystemPreferences/GeneralSystemPreferences/system_preferences.h
adrs = SystemPreferences/AddressingMethods/address_methods.h
op_table = SystemPreferences/OperationTable/operationTable.h
pre = PreAssemblyStage/PreAssembler.h
sym = SymbolTableStage/SymbolTableCreate.h
maccod = MachineLanguageCodingStage/OutputFileCreate.h

Assembler: PreAssembler.o vector.o file.o SymbolTableCreate.o operationTable.o SymbolTableTest.c
	gcc -ansi -Wall -pedantic *.o main.c -o SymbolTableTest

MachineLanguageCode.o: MachineLanguageCodingStage/OutputFileCreate.c $(maccod) $(sym) $(file) $(op_table) $(gensys) $(adrs) $(parse)
	gcc -ansi -Wall -pedantic -c MachineLanguageCodingStage/OutputFileCreate.c	

SymbolTableCreate.o: SymbolTableStage/SymbolTableCreate.c $(sym) $(file) $(op_table) $(vector) $(gensys) $(adrs) $(parse)
	gcc -ansi -Wall -pedantic -c SymbolTableStage/SymbolTableCreate.c -o SymbolTableCreate.o
	
PreAssembler.o: PreAssemblyStage/PreAssembler.c $(pre) $(vector) $(file)
	gcc -ansi -Wall -pedantic -c PreAssemblyStage/PreAssembler.c
		
operationTable.o: SystemPreferences/OperationTable/operationTable.c $(op_table) $(adrs)
	gcc -ansi -Wall -pedantic -c SystemPreferences/OperationTable/operationTable.c -o operationTable.o
	
addressing.o: SystemPreferences/AddressingMethods/address_methods.c $(adrs) $(parse) $(gensys)
	gcc -ansi -Wall -pedantic -c SystemPreferences/AddressingMethods/address_methods.c -o addressing.o

sys_pref.o: SystemPreferences/GeneralSystemPreferences/system_preferences.c $(gensys)
	gcc -ansi -Wall -pedantic -c SystemPreferences/GeneralSystemPreferences/system_preferences.c -o sys_pref.o
	
file.o: utils/file_utils/fileUtils.c $(file)
	gcc -ansi -Wall -pedantic -c utils/file_utils/fileUtils.c -o file.o
	
parsing.o: Parsing/parsing_asm_file.c $(parse)
	gcc -ansi -Wall -pedantic -c Parsing/parsing_asm_file.c -o parsing.o
	
vector.o: utils/data_structures/vector.c $(vector)
	gcc -ansi -Wall -pedantic -c utils/data_structures/vector.c

TestPreAssembler:
	make -c tests/PreAssemblerTest
	
TestSymbolTable:
	make -c tests/SymbolTableTest
	
TestAssembler:
	make -c tests/AseemblerTest

clear:
	rm *.o Aseembler
