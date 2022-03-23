vector = utils/data_structures/vector.h
parse = Parsing/parsing_asm_file.h
file = utils/file_utils/fileUtils.h
gensys = SystemPreferences/GeneralSystemPreferences/system_preferences.h
adrs = SystemPreferences/AddressingMethods/addressing_methods.h
op_table = SystemPreferences/OperationTable/operationTable.h
pre = PreAssemblyStage/PreAssembler.h
sym = SymbolTableStage/SymbolTableCreate.h
maccod = MachineLanguageCodingStage/ObjectFileCreate.h

assembler: vector.o parsing.o file.o sys_pref.o addressing.o operationTable.o PreAssembler.o SymbolTableCreate.o MachineCoding.o WriteBinary.o  
	gcc -ansi -Wall -pedantic *.o main.c -o assembler
	
MachineCoding.o: MachineLanguageCodingStage/ObjectFileCreate.c $(machcod) $(sym) $(file) $(op_table) $(gensys) $(adrs) $(parse)
	gcc -ansi -Wall -pedantic -c MachineLanguageCodingStage/ObjectFileCreate.c -o MachineCoding.o

WriteBinary.o: MachineLanguageCodingStage/WriteBinaryToFile.c  $(sym) $(file) $(op_table) $(gensys) $(adrs) $(parse)
	gcc -ansi -Wall -pedantic -c -DNDBUG MachineLanguageCodingStage/WriteBinaryToFile.c -o WriteBinary.o

SymbolTableCreate.o: SymbolTableStage/SymbolTableCreate.c $(sym) $(file) $(op_table) $(vector) $(gensys) $(adrs) $(parse)
	gcc -ansi -Wall -pedantic -c -DNDBUG SymbolTableStage/SymbolTableCreate.c -o SymbolTableCreate.o
	
PreAssembler.o: PreAssemblyStage/PreAssembler.c $(pre) $(vector) $(file)
	gcc -ansi -Wall -pedantic -c PreAssemblyStage/PreAssembler.c
		
operationTable.o: SystemPreferences/OperationTable/operationTable.c $(op_table) $(adrs)
	gcc -ansi -Wall -pedantic -c SystemPreferences/OperationTable/operationTable.c -o operationTable.o
	
addressing.o: SystemPreferences/AddressingMethods/addressing_methods.c $(adrs) $(parse) $(gensys)
	gcc -ansi -Wall -pedantic -c SystemPreferences/AddressingMethods/addressing_methods.c -o addressing.o

sys_pref.o: SystemPreferences/GeneralSystemPreferences/system_preferences.c $(gensys)
	gcc -ansi -Wall -pedantic -c SystemPreferences/GeneralSystemPreferences/system_preferences.c -o sys_pref.o
	
file.o: utils/file_utils/fileUtils.c $(file)
	gcc -ansi -Wall -pedantic -c utils/file_utils/fileUtils.c -o file.o
	
parsing.o: Parsing/parsing_asm_file.c $(parse)
	gcc -ansi -Wall -pedantic -c Parsing/parsing_asm_file.c -o parsing.o
	
vector.o: utils/data_structures/vector.c $(vector)
	gcc -ansi -Wall -pedantic -c utils/data_structures/vector.c

TestPreAssembler:
	make -C tests/PreAssemblerTest
	
TestSymbolTable:
	make -C tests/SymbolTableTest
	
TestAssembler:
	make -C tests/AseemblerTest

clear:
	rm *.o assembler
