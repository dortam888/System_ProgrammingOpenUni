#ifndef PARSING
#define PARSING

#include "complex.h"

typedef enum error  {NO_ERROR, INVALID_COMMAND, INVALID_PARAMETER, MISSING_PARAMETER} error_t;

int CheckCommandCommasErrors(char *line);

int TooManyParameters(const char *delim);

complex GetComplexParameter(complex *variables, const char *delim, error_t *error);

float GetFloatParameter(const char *delim, error_t *error);

#endif /* PARSING */
