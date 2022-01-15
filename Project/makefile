assembler: PreAssembler.o main.o vector.o
	gcc -ansi -Wall -pedantic PreAssembler.o vector.o main.o -o assembler

debug: PreAssembler.c main.c vector.c
	gcc -g -ansi -Wall -pedantic PreAssembler.c vector.c main.c
	
PreAssembler.o: PreAssembler.c PreAssembler.h vector.h
	gcc -ansi -Wall -pedantic -c PreAssembler.c
	
main.o: main.c PreAssembler.h
	gcc -ansi -Wall -pedantic -c main.c
	
vector.o: vector.c vector.h
	gcc -ansi -Wall -pedantic -c vector.c
