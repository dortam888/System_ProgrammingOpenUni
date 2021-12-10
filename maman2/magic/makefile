all: magic.o matrix.o
	gcc -Wall -pedantic -ansi magic.o matrix.o -o magic

magic.o: magic.c 
	gcc -Wall -pedantic -ansi -c magic.c
	
matrix.o: matrix.c matrix.h
	gcc -Wall -pedantic -ansi -c matrix.c
