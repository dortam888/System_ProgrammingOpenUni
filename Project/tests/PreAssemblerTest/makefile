pre = ../../PreAssemblyStage
ds = ../../utils/data_structures
f = ../../utils/file_utils

TestPreAssembler: PreAssemblerTest.c PreAssembler.o vector.o file.o
	gcc -g -ansi -Wall -pedantic PreAssemblerTest.c PreAssembler.o vector.o file.o -o TestPreAssembler

PreAssembler.o: $(pre)/PreAssembler.c $(pre)/PreAssembler.h $(ds)/vector.h $(f)/fileUtils.h
	gcc -g -ansi -Wall -pedantic -c $(pre)/PreAssembler.c
	
vector.o: $(ds)/vector.c $(ds)/vector.h
	gcc -g -ansi -Wall -pedantic -c $(ds)/vector.c
	
file.o: $(f)/fileUtils.h $(f)/fileUtils.c
	gcc -g -ansi -Wall -pedantic -c $(f)/fileUtils.c -o file.o
	
clear: 
	rm *.o TestPreAssembler
