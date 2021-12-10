#include <stdio.h> /* printf, fgets */
#include <ctype.h> /* isspace */
#include "par.h" /* IsTextBalance, IsLineBalance, PrintBalanceMessage*/

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

static int IsBufferEmpty(char buffer[], int buffer_index);
static int EndOfLine(char eol);
static int SpecialCase(char line[]);
static int IsClosedParenthesis(char parenthesis);
static int IsRightTypeOfCloseParenthsis(char parenthesis, char par_buffer[], 
                                        int buffer_index);
static int IsOpenParenthesis(char parenthesis);
static int IsString(char string);
static int IsComment(char ch1, char ch2);
static int LastIndexOfString(char line[], int index);
static int LastIndexOfComment(char line[], int index, char par_buffer[], 
                              int par_index);
static int EndOfComment(char ch1, char ch2);
static int EndOfString(char ch);

void PrintBalanceMessage(balance_t is_balance)
{
	printf("Summary Message:\n");
	is_balance? 
	printf(GREEN "Congratulations! Your Text Is Parenthesis Balanced\n" RESET) :
	printf(RED "Oh No Your Text Is Imbalanced\n" RESET);
}

/* 
	Algorithm: Check for a special case lines of curly parenthesis with white spaces only and count them
			   if counter is less than 0 than text is unbalanced.
			   Also checks if every line in text is balanced if a line is not balanced and not a special case
			   Than the text is not balanced
*/
balance_t IsTextBalance()
{
	char line[MAX_LINE_LENGTH]; /* Buffer for one line in text used to contain a line */
	char par_buffer[MAX_LINE_LENGTH];/* Buffer for open parenthesis in text used to hold open parethesis types and compare
										Them to close parenthesis types*/
	balance_t is_balance = BALANCE; /* saves if the text is balanced or unbalanced */
	int special_case_counter = 0; /* counts the special cases +1 for { -1 for }*/
	
	while (fgets(line, MAX_LINE_LENGTH, stdin))
	{
		int special_case = SpecialCase(line);
		balance_t is_line_balance = IsLineBalance(line, par_buffer);
		
		if (special_case != 0)
		{
			special_case_counter += special_case;
		}
		
		else if (!is_line_balance)
		{
			is_balance = UNBALANCE;
		}
		
		if (special_case_counter < 0)
		{
			is_balance = UNBALANCE;
		}
	}
	
	return (special_case_counter == 0)? is_balance : UNBALANCE;
}


/* 
	Check if line is parenthesis balanced
	Algorithm:
		Check each char in line until it gets to EndOfLine:
		Case 1: if the char is open parenthesis it puts it in a buffer (used as a stack).
		Case 2: if the char is closing parenthesis it checks in the buffer if the open parenthesis is of the right type.../
		otherwise it prints the line and prints unbalanced message and return UNBALACE.
		Case 3: if char is a start of a string (i.e ") it goes into the function that handles strings and continue to run on the line after the string has ended.
		Case 4: if char is a start of a comment (i.e / *) it goes into the function that handles comments and continue to run on the line or the new line after the comment has ended.
		After reaching EndOfLine:
		if par_buffer is not empty the line is unbalanced (there are open parenthesis that doesn't close) and it prints unbalanced message and return UNBALACE
		otherwise the line is balanced and it prints balanced message and return BALANCE
*/
balance_t IsLineBalance(char line[], char par_buffer[])
{
	int line_index = 0;
	int par_index = 0;
	balance_t balanced_after_comment = BALANCE;
	
	for (line_index = 0; !EndOfLine(line[line_index]); ++line_index)
	{
		char current_char = line[line_index];
		
		if (IsClosedParenthesis(current_char))
		{
			if (IsRightTypeOfCloseParenthsis(current_char, par_buffer, 
			                                 par_index - 1))
			{
				--par_index;
			}
			else
			{
				for (; !EndOfLine(line[line_index]); ++line_index)
				{
					putchar(line[line_index]);
				}
				printf(RED "	Line Is Imbalanced\n" RESET);
				return UNBALANCE;
			}
		}
		else if (IsOpenParenthesis(current_char))
		{
			par_buffer[par_index] = current_char;
			++par_index;
		}
		else if (IsString(current_char))
		{
			putchar(current_char);
			line_index = LastIndexOfString(line, line_index + 1);
		}
		else if(IsComment(current_char, line[line_index + 1]))
		{
			line_index = LastIndexOfComment(line, line_index, par_buffer, par_index);
			
			if (par_buffer[par_index - 1] == '\0')
			{
				balanced_after_comment = UNBALANCE;
			}
		}
		
		putchar(line[line_index]);
	}
	
	if (!IsBufferEmpty(par_buffer, par_index))
	{
		printf(RED "	Line Is Imbalanced\n" RESET);
		return UNBALANCE;
	}
	else
	{
		printf(GREEN "	Line Is Balanced\n" RESET);	
		return balanced_after_comment;
	}
}

/* Returns if the parenthesis buffer is empty 
   input: char buffer[] the parenthesis buffer
   input: int buffer_index the current index in buffer
   output: returns 0 if buffer is not empty and other value otherwise
*/
static int IsBufferEmpty(char buffer[], int buffer_index)
{
	return (buffer_index == 0) || buffer[buffer_index - 1] == '\0';
}

/* 
	Returns if the character is in the end of a line
	input: char eol - character to check if end of line
	output: returns 0 if not end of line and other value otherwise
*/
static int EndOfLine(char eol)
{
	return eol == '\n' || eol == EOF;
}

/*
	Check for special case and returns which special case it is
	input: char line[]: The line to check if its a special case.
	output: returns 1 in case of open parenthesis special case (i.e { ) return -1 
	        if closed parenthesis of special case (i.e } ) returns 0 otherwise
*/
static int SpecialCase(char line[])
{
	int special_case = 0;
	int index = 0;
	
	for (index = 0; !EndOfLine(line[index]); ++index)
	{
		if (!isspace(line[index]))
		{
			if (line[index] == '{')
			{
				special_case = 1;
			}
			else if (line[index] == '}')
			{
				special_case = -1;
			}
			else
			{
				return 0;
			}
		}
	}
	
	return special_case;
}

/* 
	Returns if the character a closing parenthesis
	input: char parenthesis - character to check if its closed parenthesis
	output: returns 0 if not a closed parenthesis of any type and other value otherwise
*/
static int IsClosedParenthesis(char parenthesis)
{
	return (parenthesis == ')') || (parenthesis == '}') || (parenthesis == ']');
}

/* 
	Returns if the character a closing parenthesis
	input: char parenthesis - character to check if its closed parenthesis
	output: returns 0 if not a closed parenthesis of any type and other value otherwise
*/
static int IsRightTypeOfCloseParenthsis(char parenthesis, char par_buffer[], 
                                        int buffer_index)
{
	if (buffer_index < 0)
	{
		return 0;
	}
	
	switch (parenthesis)
	{
		case ')':
			return par_buffer[buffer_index] == '(';
		case '}':
			return par_buffer[buffer_index] == '{';
		case ']':
			return par_buffer[buffer_index] == '[';
		default:
			printf("Fatal Error doesn't suppose to get here please debug");
	}
	
	return 1;
}

/* 
	Returns if the character an opening parenthesis
	input: char parenthesis - character to check if its open parenthesis
	output: returns 0 if not an opening parenthesis of any type and other value otherwise
*/
static int IsOpenParenthesis(char parenthesis)
{
	return (parenthesis == '(' || parenthesis == '{' || parenthesis == '[');
}

/* 
	Returns if the character start of a string
	input: char string - character to check if its a string 
	output: returns 0 if not a string and other value otherwise
*/
static int IsString(char string)
{
	return string == '"';
}

/* 
	Returns if 2 following characters are a beginning of a comment
	input: char ch1, ch2 - 2 char inputs to check if they are opening of a comment
	output: returns 0 if not an opening parenthesis of any type and other value otherwise
*/
static int IsComment(char ch1, char ch2)
{
	return (ch1 == '/') && (ch2 == '*');
}

/* 
	Returns the last index of a string
	input: char line[] - the line that contains the string
	input: int index - the index of the start of the string
	output: returns the index in the line in which the string ends
*/
static int LastIndexOfString(char line[], int index)
{
	for (; !EndOfString(line[index]); ++index)
	{
		putchar(line[index]);
	}
	
	return index;
}

/* 
	Returns the last index of a comment
	input: char line[] - the line that contains the comment
	input: int index - the index of the start of the comment
	output: returns the index in the line in which the comment ends
*/
static int LastIndexOfComment(char line[], int index, char par_buffer[], int par_index)
{
	for (; !EndOfComment(line[index], line[index + 1]); ++index)
	{
		if (line[index] == '\n')
		{
			if(IsBufferEmpty(par_buffer, par_index))
			{
				printf(GREEN "	Line Is Balanced\n" RESET);
			} 
			else 
			{
				printf(RED "	Line Is Imbalanced\n" RESET);
				par_buffer[par_index - 1] = '\0'; /* remove last parenthesis from check */
				par_index = 0; /* initialize par_index */
			}
			                                      
			index = -1; /* Initialize to 0 expecting increasing by loop*/
			fgets(line, MAX_LINE_LENGTH, stdin);
		}
		
		putchar(line[index]);
	}
	
	return index;
}

/* 
	Returns if 2 following characters are an end of a comment
	input: char ch1, ch2 - 2 char inputs to check if they are end of a comment
	output: returns 0 if not an end of a comment and other value otherwise
*/
static int EndOfComment(char ch1, char ch2)
{
	return ch1 == '*' && ch2 == '/';
}

/* 
	Returns if the character is an end of a string
	input: char ch - character to check if its the end of the string
	output: returns 0 if not end of string and other value otherwise
*/
static int EndOfString(char ch)
{
	return ch == '"';
}

