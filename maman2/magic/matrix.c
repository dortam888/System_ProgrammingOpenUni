#include <stdio.h>
#include <ctype.h>
#include "matrix.h"

static int CharToInt(char c);
static int GetInt(error_t *err);
static void PrintErrorMessage(error_t err);

/*Prints matrix see matrix.h for details*/
void PrintMatrix(const matrix_t matrix)
{
	int i = 0;
	int j = 0;
	
	printf("\nThe Matrix Is:\n");
	
	for (i = 0; i < N; ++i)
	{
		for(j = 0; j < N; ++j)
		{
			printf("%d	", matrix[i][j]);
		}
		
		printf("\n");
	}
}

static int IsInputAfterMatrix(error_t *err)
{
	return GetInt(err) != 0 || *err != NOT_ENOUGH_INPUTS; /*expect to not get any number (also not zero) in that case GetInt will return the error
															NOT_ENOUGH_INPUTS*/
}

/*Input numbers to given matrix see matrix.h for details*/
int** InputMatrix(matrix_t matrix, error_t *err)
{
	int i = 0;
	int j = 0;
	
	printf("Please enter a matrix of size %d X %d (%d integer values separated by whitespace)\n", N, N, N*N);
	
	for (i = 0; i < N; ++i)
	{
		for(j = 0; j < N; ++j)
		{
			matrix[i][j] = GetInt(err);
			
			if(*err != OK) /* if there is an error in input return NULL*/
			{
				PrintErrorMessage(*err);
				return NULL;
			}
		}
	}
	
	if(IsInputAfterMatrix(err)) /* if there are inputs after the matrix filled return NULL */
	{	
		*err = TOO_MANY_INPUTS;
		PrintErrorMessage(*err);
		return NULL;
	}
	else
	{
		*err = OK;
	}
	
	return (int **)matrix;
}

/* sum the row for more details see matrix.h*/
int SumOfRow(const matrix_t matrix, int row)
{
	int i = 0;
	int sum_of_row = 0;
	
	for (i = 0; i < N; ++i)
	{
		sum_of_row += matrix[row][i];
	}
	
	return sum_of_row;
}

/* sum the column for more details see matrix.h*/
int SumOfColumn(const matrix_t matrix, int column)
{
	int i = 0;
	int sum_of_column = 0;
	
	for (i = 0; i < N; ++i)
	{
		sum_of_column += matrix[i][column];
	}
	
	return sum_of_column;
}

/*Checks if sign wasnt changed*/
static int IsSignNotChanged(int sign)
{
	return sign == 1;
}

/*Gets a string that represent an integer and returns it.
  input: error_t *err - error indicator see errors
  output: the integer that was get until a whitespace or an Invalid char is read. In case an Invalid char read change err to right value
  errors: INVALID_INPUT - In case an invalid char was entered
  		  NOT_ENOUGH_INPUTS - In case no number was read 
  */  
static int GetInt(error_t *err)
{
	char current_char = '\0'; /*current char*/
	int num = 0; /*number to return*/
	int sign = 1; /*number sign*/
	int is_number_read = 0; /*if no number was read remains 0 and otherwise become 1*/
	
	while ((current_char = getchar()) != EOF)
	{
		if (isspace(current_char))
		{
			if(is_number_read) /*differ between whitespace before number that are ignored and after number to seperate between numbers*/
			{
				break;
			}
		}
		
		else if (!is_number_read && IsSignNotChanged(sign) && current_char == '-') /*If no number was read and sign wasnt changed and we got the minus sign*/
		{
			sign = -1;
		}
		
		else if (!isdigit(current_char))
		{
			*err = INVALID_INPUT;
			return num;
		}
		
		else
		{
			num = num*MATRIX_BASE + CharToInt(current_char); /*Multiply the number that was gotten until now by the base and add the current digit that was read*/
			is_number_read = 1; /*A number was read*/
		}
	}	
	
	if(!is_number_read) 
	{
		*err = NOT_ENOUGH_INPUTS;
	}
	else
	{
		*err = OK;
	}
	
	return num * sign;
}

/* char digit to int value*/
static int CharToInt(char c)
{
	return (int)(c - '0');
}

/*Prints error message for the error occured*/
static void PrintErrorMessage(error_t err)
{
	switch(err)
	{
		case NOT_ENOUGH_INPUTS:
			printf("\nNot Enough Inputs Were Entered\n");
			break;
		case TOO_MANY_INPUTS:
			printf("\nToo Many Inputs Were Entered\n");
			break;
		case INVALID_INPUT:
			printf("\nInvalid Input Was Entered\n");
			break;
		default:
			printf("\nUnexpected Error\n");
	}
}

/*returns magic constant for matrix dimension. see more: https://en.wikipedia.org/wiki/Magic_constant */
static int MagicConstant(int matrix_dimension)
{
	return (matrix_dimension * 
		   (matrix_dimension * matrix_dimension + 1)) >> 1;
}

/*checks if the sum of diagonals is equal and returns it for the magic square check
  input: const matrix_t matrix - the matrix to check sum of diagonals
  output: the sum of the diagonalsif equal and -1 if not (if sum of diagonals is -1 its not a magic square)
  */
static int SumOfDiagonals(const matrix_t matrix)
{
	int i = 0;
	int sum_of_main_diagonal = 0;
	int sum_of_second_diagonal = 0;
	
	for (i = 0; i < N; ++i)
	{
		sum_of_main_diagonal += matrix[i][i];
		sum_of_second_diagonal += matrix[i][N - 1 - i];
	}
	
	return (sum_of_main_diagonal == sum_of_second_diagonal)?
		    sum_of_main_diagonal : -1;
}

/*is number in range for magic square 1 - N^2 */
static int isInRange(int number)
{
	return (number > 0) && (number < N*N + 1);
}

/* Checks if all the numbers from 1 to N^2 appears exactly once.
   the algorithm is building a look up table size N^2 and increase the value by 1 if */
static int AreAllNumbersInRangeAppearsOnce(const matrix_t matrix)
{
	int numbers_lut[N*N] = {0};
	int i = 0;
	int j = 0;
	int is_in_range = 1;
	
	for (i = 0; i < N; ++i)
	{
		for (j = 0; j < N; ++j)
		{
			if (isInRange(matrix[i][j]))
			{
				++numbers_lut[matrix[i][j] - 1];
				if (numbers_lut[matrix[i][j] - 1] > 1)
				{
					is_in_range = 0;
					break;
				}
			}
			else
			{
				is_in_range = 0;
				break;
			}
		}
	}
	
	return is_in_range;	
}

/* returns if the matrix is a magic square for more details see matrix.h*/
int IsMagicSquare(const matrix_t matrix)
{
	int i = 0;
	const int sum = MagicConstant(N);
	
	if (!AreAllNumbersInRangeAppearsOnce(matrix))
	{
		return 0;
	}
	for (i = 0; i < N; ++i)
	{
		if (SumOfRow(matrix, i) != sum || SumOfColumn(matrix, i) != sum)
		{
			return 0;
		} 
	}
	
	return SumOfDiagonals(matrix) == sum;
}
