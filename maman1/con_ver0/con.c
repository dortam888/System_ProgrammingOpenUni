#include <stdio.h> /* printf, fgets */
#include "contract.h" /* contract */

#define MAX_LENGTH 81

int main()
{
	char str_input[MAX_LENGTH] = {0};
	char str_output[MAX_LENGTH] = {0};
	
	printf("Please input a string of with max length of 80 charachters: \n");
	fgets(str_input, MAX_LENGTH, stdin);
	
	contract(str_input, str_output);
	
	printf("The original string was: %s\n The new shortcuted string is: %s\n", str_input, str_output);
	
	return 0;
}
