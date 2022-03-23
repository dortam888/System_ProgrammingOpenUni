#ifndef SYS_PREF_H
#define SYS_PREF_H

#define MEMORY_SIZE_IN_WORDS 8192
#define START_ADDRESS_OF_INSTRUCTION_COUNT 100
#define WORD_SIZE_IN_REGISTER 20
#define BITS_FOR_NUM_OF_REG 4 /* usually number of registers is a power of 2. this constant is for how many bits can represent that number */
#define BITS_FOR_ADDRESSING_METHOD 2 /* there are 4 addressing methods 0,1,2,3 */
#define WORD_BITS 16 /* For Address Offset */
#define OFFSET_BITS 4
#define BITS_FOR_CODE_ATTRIBUTE 4 /* 0 A R E */

#define BITS_FOR_IMMEDIATE WORD_SIZE_IN_REGISTER - BITS_FOR_CODE_ATTRIBUTE
#define FUNCT_PLACE_IN_WORD WORD_BITS - BITS_FOR_CODE_ATTRIBUTE
#define SRC_REGISTER_PLACE_IN_WORD FUNCT_PLACE_IN_WORD - BITS_FOR_NUM_OF_REG
#define SRC_ADDRESSING_METHOD_PLACE_IN_WORD SRC_REGISTER_PLACE_IN_WORD - BITS_FOR_ADDRESSING_METHOD
#define DEST_REGISTER_PLACE_IN_WORD SRC_ADDRESSING_METHOD_PLACE_IN_WORD - BITS_FOR_NUM_OF_REG
#define DEST_ADDRESSING_METHOD_PLACE_IN_WORD DEST_REGISTER_PLACE_IN_WORD - BITS_FOR_ADDRESSING_METHOD

#define MAX_REG_NAME_LEN 4

enum word_attributes {ABSOLUTE = 4, RELATIVE = 2, EXTERNAL = 1};

int IsValidRegister(const char *reg);

int IsValidRegisterForIndex(const char *reg);

unsigned int GetRegNumber(const char *reg);

int IsSavedWord(const char *str);

#endif /* SYS_PREF_H */
