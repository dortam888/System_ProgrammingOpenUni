#ifndef COMPLEX
#define COMPLEX

typedef struct complex* complex;

complex CreateComplex();

complex ReadComplex(complex comp_num, float real, float imaginary);

void PrintComplex(const complex comp_num);

void AddComplex(const complex comp1, const complex comp2);

void SubtractComplex(const complex comp1, const complex comp2);

void MultiplyComplexReal(const complex comp_num, float real);

void MultiplyComplexImag(const complex comp_num, float imag);

void MultiplyComplex(const complex comp1, const complex comp2);

void AbsComplex(const complex comp_num);

void DestroyComplex(complex comp_num);

int Stop();


#endif /* COMPLEX*/
