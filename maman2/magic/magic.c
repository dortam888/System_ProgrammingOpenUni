#include <stdio.h>

#include "matrix.h"

int main()
{
	matrix_t matrix;
	error_t err = OK;
	
	if (InputMatrix(matrix, &err) == NULL)
	{
		return err;
	}
		
	PrintMatrix((const int (*)[])matrix);
	
	IsMagicSquare((const int (*)[])matrix)? printf("The Matrix Is Magic Square\n") : 
											printf("The Matrix Is Not Magic Square\n");
	
	return 0;
}
