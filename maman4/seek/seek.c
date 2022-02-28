#include <stdio.h> /*printf FILE*/
#include <stdlib.h> /*strtol*/

#include "seek.h"

#define START_OF_FILES 2
#define MINIMUM_NUMBER_OF_ARGUMENTS 3
#define DECIMAL_BASE 10

static int IsValidNumberOfArguments(int args);
static void PrintCharNumberInFile(const char *file_name, long char_position);
static long GetCharPosition(const char *number);

int main(int argc, char *argv[])
{
	long char_number = 0;
	
	if (!IsValidNumberOfArguments(argc))
	{
		return 1;
	}
	
	char_number = GetCharPosition(argv[1]);
	if (char_number == 0)
	{
		return 1;
	}
	
	PrintCharNumberInAllFiles(char_number, &argv[START_OF_FILES], argc - START_OF_FILES);	
		
	return 0;
}

/* 
	Description: 
		Checks if number of arguments is valid (there is at least one file to open)
	input:
		int num_of_args - number of arguments entered by the user
	output:
		returns 0 if number of arguments is less than 3 (executable name, a number, and at least one file) and other value otherwise
	Time Complxity:
		O(1)
	Space Complexity:
		O(1)
*/
static int IsValidNumberOfArguments(int num_of_args)
{	
	if(num_of_args < MINIMUM_NUMBER_OF_ARGUMENTS)
	{
		printf("Error - you entered %d arguments. number of arguments must be at least %d\n", num_of_args, MINIMUM_NUMBER_OF_ARGUMENTS);
		return 0;
	}
	
	return 1;
}

/* 
	Description: 
		gets the position of the character to print from arguments
	input:
		const char *number - the number as a string
	output:
		returns the number in case it is a valid. 
		In case the number is not valid or its less than 1 (i.e not legitamate for checking in a file) returns 0
	Time Complxity:
		O(1)
	Space Complexity:
		O(1)
*/
static long GetCharPosition(const char *number)
{
	char *end; /*end of number part*/
	long result = 0;
	
	result = strtol(number, &end, DECIMAL_BASE);
	
	if(*end != '\0')
	{
		printf("%s is not a Valid number argument\n", number);
		return 0;
	}	
	
	if (result < 1)
	{
		printf("Number argument must be greater than 0\n");
		return 0;
	}
	
	return result;
}

/* 
	Description: 
		checks if there are enough characters in file
	input:
		FILE *fp - the file to check
		long char_position - 
	output:
		(none)
	Time Complxity:
		O(n) n is the number of files given to the function
	Space Complexity:
		O(1) No additional space required
*/
static int IsNumberInFile(FILE *fp, long char_position, const char *file_name)
{
	int num_of_chars_in_file = 0;
	
	fseek(fp, -1, SEEK_END);
	num_of_chars_in_file = ftell(fp);
	
	if (char_position > num_of_chars_in_file)
	{
		printf("There are %d characters in file %s. Couldnt print char number %ld\n", num_of_chars_in_file, file_name, char_position);
		return 0;
	}
	
	return 1;
}

/* 
	Description: 
		Prints the ascii value of a character in a given position in a given file
	input:
		long char_position - The char position in the file (measured from the start of the file)
		char *file_name - the file name of the file to print from
	output:
		(none)
	Time Complxity:
		O(n) n is the number of files given to the function
	Space Complexity:
		O(1) No additional space required
*/
static void PrintCharNumberInFile(const char *file_name, long char_position)
{
	FILE *fp = fopen(file_name, "r"); /*File pointer to the file*/
	if (fp == NULL)
	{
		printf("Couldn't open file %s. Either it doesn't exist or you don't have permission to read it\n", file_name);
		return;
	}
		
	if (!IsNumberInFile(fp, char_position, file_name))
	{	
		return;
	}
	
	fseek(fp, char_position - 1, SEEK_SET);
	printf("The Ascii value of char number %ld in file %s is:	%d\n", char_position, file_name, fgetc(fp));
	fclose(fp); /* to let more files to be opened i close the current file after printing the character ASCII */
}

void PrintCharNumberInAllFiles(long char_position, char *file_names[], int num_of_files)
{
	int i = 0;
	
	for (i = 0; i < num_of_files; ++i)
	{
		PrintCharNumberInFile(file_names[i], char_position);
	}
}
