#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "../AddressingMethods/addressing_methods.h"

#define NOFUNCT 0
#define ERROR 0xBADBAD

typedef struct operations operation_data_t;

int GetCommand(const char *operation_name);

const char *GetCommandName(int command);

unsigned int GetOpcode(int command);

unsigned int GetFunct(int command);

int IsSrcAdressingMethodLegal(int command, enum addressing_methods src_addressing_method);

int IsDestAdressingMethodLegal(int command, enum addressing_methods dest_addressing_method);

unsigned int GetNumOfOperands(int command);

#endif /* OPERATIONS_H */
