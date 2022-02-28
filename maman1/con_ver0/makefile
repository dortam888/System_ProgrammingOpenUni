all: con.o contract.o
	gcc -Wall -pedantic -ansi -o con con.o contract.o
	
con.o: con.c
	gcc -Wall -pedantic -ansi -c con.c
	
contract.o: contract.c
	gcc -Wall -pedantic -ansi -c contract.c
	
test: con_test.c contract.c
	gcc -Wall -pedantic -ansi -o test con_test.c contract.c
