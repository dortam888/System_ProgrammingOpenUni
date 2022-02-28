#include <string.h>
#include <stddef.h>

#include "system_preferences.h"


struct registers
{
	const char *reg_name;
	unsigned int reg_num:BITS_FOR_NUM_OF_REG;
	unsigned int is_index:1; 
};

static const struct registers REGS[] = {	/*reg_name		reg_num			is_index*/
											{"r0",  		0,				0},
											{"r1",  		1,				0},
											{"r2",  		2,				0},
											{"r3",  		3,				0},
											{"r4",  		4,				0},
											{"r5",  		5,				0},
											{"r6",  		6,				0},
											{"r7",  		7,				0},
											{"r8",  		8,				0},
											{"r9",  		9,				0},
											{"r10", 		10,				1},
											{"r11", 		11,				1},
											{"r12", 		12,				1},
											{"r13", 		13,				1},
											{"r14", 		14,				1},
											{"r15", 		15,				1}
									   };
									   
static const size_t num_of_regs = sizeof(REGS) / sizeof(*REGS);
									   
int IsValidRegister(const char *reg)
{
	int i = 0;
	
	for (i = 0; i < num_of_regs; ++i)
	{
		if (strcmp(reg, REGS[i].reg_name) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}

int IsValidRegisterForIndex(const char *reg)
{
	int i = 0;
	
	for(i = 0; i < num_of_regs; ++i)
	{
		if (strcmp(reg, REGS[i].reg_name) == 0)
		{
			break;
		}
	}
	
	return REGS[i].is_index;
}

/*int IsSavedWord(const char *str)
{
	size_t num_of_commands = sizeof(SAVED_COMMANDS)/sizeof(*SAVED_COMMANDS);
	size_t num_of_registers = sizeof(SAVED_REGISTERS)/sizeof(*SAVED_REGISTERS);
	size_t num_of_general_saved_words = sizeof(SAVED_GENERALS)/sizeof(*SAVED_GENERALS);
	int i = 0;
	
	for (i = 0; i < num_of_commands; ++i)
	{
		if(strcmp(str, SAVED_COMMANDS[i]) == 0)
		{
			return 1;
		}
	}
	
	for (i = 0; i < num_of_registers; ++i)
	{
		if(strcmp(str, SAVED_REGISTERS[i]) == 0)
		{
			return 1;
		}
	}
	
	for (i = 0; i < num_of_general_saved_words; ++i)
	{
		if(strcmp(str, SAVED_GENERALS[i]) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}*/
