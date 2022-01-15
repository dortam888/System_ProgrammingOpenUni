#ifndef COMPLEX
#define COMPLEX

typedef struct complex* complex;

/*
	Constructor for a complex variable initialize as 0+0i
	input: (none)
	output: a new complex type variable or NULL in case of failure
*/
complex CreateComplex();

/*
	Read real and imag to a given complex number and change it accordingly
	input: complex comp_num - the complex to be changed 
		   float real - the real part to be given to comp_num
		   float imag - the imag part to be given to comp_num
	output: (none)
*/
void ReadComplex(complex comp_num, float real, float imag);

/*
	Prints a complex number in a format: real + (imag) i or real - (imag)i
	input: complex comp_num - the complex to be printed 
	output: (none)
*/
void PrintComplex(complex comp_num);

/*
	Adding complex number to another one
	input: complex comp1 - the first complex number in operation
		   complex comp2 - the second complex number in operation
	output: the result of Add
*/
complex AddComplex(complex comp1, complex comp2);

/*
	Substracting complex number from another one
	input: complex comp1 - the first complex number in operation
		   complex comp2 - the second complex number in operation
	output: the result of substract
*/
complex SubtractComplex(complex comp1, complex comp2);

/*
	Multiply complex number with another one
	input: complex comp_num - the complex number in operation
		   complex real - the real number number in operation
	output: the result of Multiplication
*/
complex MultiplyComplexReal(complex comp_num, float real);

/*
	Multiply complex number with another one
	input: complex comp1 - the complex number in operation
		   complex comp2 - the imag number in operation
	output: the result of Multiplication
*/
complex MultiplyComplexImag(complex comp_num, float imag);

/*
	Multiply complex number with another one
	input: complex comp1 - the first complex number in operation
		   complex comp2 - the second complex number in operation
	output: the result of Multiplication
*/
complex MultiplyComplex(complex comp1, complex comp2);

/*
	Giving the size of a complex number i.e sqrt(real ^ 2 + imag ^ 2)
	input: complex comp_num - the complex number for the operation
	output: the size of comp_num
*/
complex AbsComplex(complex comp_num);

/*
	Destructor for a complex number
	input: complex comp_num - the complex to be freed
	output: (none)
*/
void DestroyComplex(complex comp_num);

#endif /* COMPLEX*/
