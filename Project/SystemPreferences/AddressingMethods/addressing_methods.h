#ifndef ADDRESSING_H
#define ADDRESSING_H

#include <stddef.h>

enum addressing_methods {IMMEDIATE_ADDRESSING = 1, DIRECT_ADDRESSING = 2, INDEX_ADDRESSING = 4, REGISTER_ADDRESSING = 8, NONE = 0, INVALID = -1};

char *StringAddMethod(enum addressing_methods method);

enum addressing_methods GetAddressingMethod(const char *str);

unsigned int AddressingMethodNumber(enum addressing_methods address_method);

size_t GetWordsForAddressMethod(enum addressing_methods address_method);

unsigned int GetRegisterNumber(const char *str, enum addressing_methods addressing_method);

#endif /* ADDRESSING_H */
