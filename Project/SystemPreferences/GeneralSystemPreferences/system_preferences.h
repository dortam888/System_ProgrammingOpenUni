#ifndef SYS_PREF_H
#define SYS_PREF_H

#define WORD_SIZE_IN_REGISTER 20
#define BITS_FOR_NUM_OF_REG 4 /*usually number of registers is a power of 2. this constant is for how many bits can represent that number*/
#define START_ADDRESS_OF_INSTRUCTION_COUNT 100
#define WORD_BITS 16 /* For Address Offset */
#define OFFSET_BITS 4
#define MAX_REG_NAME_LEN 4

/*
static const char *SAVED_COMMANDS[] = {"mov", "cmp", "add", "sub", "lea", "clr", "not", "inc", "dec", "jmp", "bne", "jsr", "red", "prn", "rts", "stop"};

static const char *SAVED_REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

static const char *SAVED_GENERALS[] = {"macro", "endm"};
*/

int IsValidRegister(const char *reg);

int IsValidRegisterForIndex(const char *reg);

int IsSavedWord(const char *str);

#endif /* SYS_PREF_H */
