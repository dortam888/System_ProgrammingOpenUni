all: par.o parenthesis.o
	gcc -Wall -pedantic -ansi -o par par.o parenthesis.o
	
par.o: par.c
	gcc -Wall -pedantic -ansi -c par.c

parenthesis.o: parenthesis.c
	gcc -Wall -pedantic -ansi -c parenthesis.c
	
test:
	./test.sh
