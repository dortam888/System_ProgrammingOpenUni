#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc */ 
#include <math.h> /* sqrt pow */

#include "complex.h" 

struct complex
{
	float real;
	float imag;
};

complex CreateComplex()
{
	complex new_complex = (complex) malloc(sizeof(*new_complex));
	if (new_complex == NULL)
	{
		return NULL;
	}
	
	new_complex->real = 0;
	new_complex->imag = 0;
	
	return new_complex;
}

void ReadComplex(complex comp_num_to_read, float real, float imag)
{
	comp_num_to_read->real = real;
	comp_num_to_read->imag = imag;
}


void PrintComplex(complex comp_num)
{
	char sign_char = '+';
	int sign = 1;
	
	if (comp_num->imag < 0)
	{
		sign_char = '-';
		sign = -1;
	}
	
	printf("%.2f %c (%.2f)i\n", comp_num->real, sign_char, sign * comp_num->imag);
}

complex AddComplex(complex comp1, complex comp2)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, comp1->real + comp2->real, comp1->imag + comp2->imag);
	
	return new_complex;
}

complex SubtractComplex(complex comp1, complex comp2)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, comp1->real - comp2->real, comp1->imag - comp2->imag);
	
	return new_complex;
}

complex MultiplyComplex(complex comp1, complex comp2)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, comp1->real * comp2->real - comp1->imag * comp2->imag
	                       , comp1->real * comp2->imag + comp1->imag * comp2->real);
	
	return new_complex;
}

complex MultiplyComplexImag(complex comp, float imag)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, -comp->imag * imag, comp->real * imag);
	
	return new_complex;
}

complex MultiplyComplexReal(complex comp, float real)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, comp->real * real, comp->imag * real);
	
	return new_complex;
} 

complex AbsComplex(complex comp)
{
	complex new_complex = CreateComplex();
	
	ReadComplex(new_complex, sqrt(pow(comp->real, 2) + pow(comp->imag, 2)), 0);
	
	return new_complex;
}


void DestroyComplex(complex comp_num)
{
	comp_num->real = 0;
	comp_num->imag = 0;
	
	free(comp_num);
	comp_num = NULL;
}

