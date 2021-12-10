#include <stdio.h>
#include <string.h>

#include "complex.h"

#define MAX_COMMAND_LENGTH 15

struct operation
{
	
}

int main()
{
	complex a = CreateComplex();
	complex b = CreateComplex();
	complex c = CreateComplex();
	complex d = CreateComplex();
	complex e = CreateComplex();
	complex f = CreateComplex();

	StartSession(a,b,c,d,e,f);
	
	return 0;
}

void StartSession(complex a, complex b, complex c,
				  complex d, complex e, complex f)
{
	char command[MAX_COMMAND_LENGTH] = {'\0'};
	return_type_t action = ERROR;
	
	while (strcmp(command, "stop") != 0)
	{
		printf("Enter A Command >  ");
		InputCommand(command);
		
	}
}
