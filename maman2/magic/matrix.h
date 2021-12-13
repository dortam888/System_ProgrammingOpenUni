#ifndef MATRIX
#define MATRIX

/*This is an API for a square matrix of defined size N X N (predefined by the user here).
  numbers in the matrix are predefined in base MATRIX_BASE (Binary, Octal, Decimal, Hexa..)*/

#define N 20
#define MATRIX_BASE 10

typedef int matrix_t[N][N];
typedef enum errors {OK, INVALID_INPUT, NOT_ENOUGH_INPUTS, TOO_MANY_INPUTS} error_t;

/*
	Input numbers from stdin to a given matrix.
	input: matrix_t matrix - a matrix created by the user
  	input: error_t *errno - error indicator 
  	output: In case of success returns the user the matrix. otherwise returns NULL and changes errno according to errors section
  	errors: INVALID_INPUT - In case of Invalid number (including floating point numbers)
  		  	NOT_ENOUGH_INPUTS - In case there are not enough inputs in stdin to fill the given matrix
  		  	TOO_MANY_INPUTS - In case there are still inputs on stdin after the matrix was filled
  	time complexity: O(n) n is the size of the matrix (i.e N X N)
  	space complexity: O(1) 
*/
int** InputMatrix(matrix_t matrix, error_t *errno);

/*
	Prints a given matrix
	input: const matrix_t matrix - the matrix to print
	output: (none)
	time complexity: O(n) n is the size of the matrix (i.e N X N)
	space complexity: O(1)
*/
void PrintMatrix(const matrix_t matrix);

/*
	Returns the sum of a given row in a given matrix
	input: const matrix_t matrix - the given matrix
	input: int row_number - the row number to sum upon
	output: the sum of row number "row_number"
	time_complexity: O(n) n is the number of columns in the matrix (i.e N)
	space complexity: O(1)
*/
int SumOfRow(const matrix_t matrix, int row_number);

/*
	Returns the sum of a given column in a given matrix
	input: const matrix_t matrix - the given matrix
	input: int column_number - the column number to sum upon
	output: the sum of column number "column_number"
	time_complexity: O(n) n is the number of rows in the matrix (i.e N)
	space complexity: O(1)
*/
int SumOfColumn(const matrix_t matrix, int column_number);

/*
	Check if a matrix is a standard Magic Square 
	(a magic square is a matrix in which the sum of all rows, columns and diagonals are the same and it contains the numbers from 1 to N X N included)
	read more in: https://en.wikipedia.org/wiki/Magic_square
	input: const matrix_t matrix the matrix to check if magic square
	output: return 0 if its not a magic square and other value otherwise
	time_complexity: O(n) n is the size of the matrix (i.e N X N)
	space complexity: O(n) n is the size of the matrix (i.e N X N)
*/
int IsMagicSquare(const matrix_t matrix);

#endif /* MATRIX */
