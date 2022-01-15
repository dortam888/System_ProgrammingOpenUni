complex: complex.o mycomp.o parsing.o
	gcc -ansi -pedantic -Wall complex.o mycomp.o parsing.o -o complex -lm
	
complex.o: complex.c complex.h
	gcc -ansi -pedantic -Wall -c complex.c -lm
	
mycomp.o: mycomp.c
	gcc -ansi -pedantic -Wall -c mycomp.c 
	
parsing.o: parsing.c parsing.h
	gcc -ansi -pedantic -Wall -c parsing.c
