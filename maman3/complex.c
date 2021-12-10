#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"

struct complex
{
	float real;
	float imag;
};

complex CreateComplex()
{
	complex new_complex = (complex) malloc(sizeof(struct complex));
	
	new_complex->real = 0;
	new_complex->imag = 0;
	
	return new_complex;
}

complex ReadComplex(complex comp_num, float real, float imag)
{
	comp_num->real = real;
	comp_num->imag = imag;
	
	return comp_num;
}

void PrintComplex(const complex comp_num)
{
	char sign_char = '+';
	int sign = 1;
	
	if (comp_num->imag < 0)
	{
		sign_char = '-';
		sign = -1;
	}
	
	printf("%f %c (%f)i", comp_num->real, sign_char, sign * comp_num->imag);
}

void AddComplex(const complex comp1, const complex comp2)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = comp1->real + comp2->real;
	new_imag = comp1->imag + comp2->imag;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);
}

void SubtractComplex(const complex comp1, const complex comp2)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = comp1->real - comp2->real;
	new_imag = comp1->imag - comp2->imag;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);
}

void MultiplyComplex(const complex comp1, const complex comp2)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = comp1->real * comp2->real - comp1->imag * comp2->imag;
	new_imag = comp1->real * comp2->imag + comp1->imag * comp2->real;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);	
}

void MultiplyComplexReal(const complex comp_num, float real)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = comp_num->real * real;
	new_imag = comp_num->imag * real;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);
}

void MultiplyComplexImag(const complex comp_num, float imag)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = - comp_num->imag * imag;
	new_imag = comp_num->real * imag;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);
}

void AbsComplex(const complex comp_num)
{
	complex new_complex = NULL;
	float new_real = 0;
	float new_imag = 0;
	
	new_complex = CreateComplex();
	
	new_real = sqrt(pow(comp_num->real, 2) + pow(comp_num->imag, 2));
	new_imag = 0;
	
	new_complex = ReadComplex(new_complex, new_real, new_imag);
	
	PrintComplex(new_complex);
	
	DestroyComplex(new_complex);
}

void DestroyComplex(complex comp_num)
{
	comp_num->real = 0;
	comp_num->imag = 0;
	
	free(comp_num);
	comp_num = NULL;
}

