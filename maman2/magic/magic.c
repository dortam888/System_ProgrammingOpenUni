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
		
	PrintMatrix((const int(*)[])matrix);
	
	IsMagicSquare((const int (*)[])matrix);
	
	return 0;
}
