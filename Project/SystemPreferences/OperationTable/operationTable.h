#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../AddressingMethods/addressing_methods.h"

#define NOFUNCT 0
#define ERROR 0xBADBAD

typedef struct operations operation_data_t;

unsigned int GetOpcode(const char *operation_name);

unsigned int GetFunct(const char *operation_name);

int IsSrcAdressingMethodLegal(const char *operation_name, enum addressing_methods src_addressing_method);

int IsDestAdressingMethodLegal(const char *operation_name, enum addressing_methods dest_addressing_method);

#endif /* OPERATIONS_H */
