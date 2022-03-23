#include <string.h>
#include <ctype.h>

#include "addressing_methods.h"
#include "../GeneralSystemPreferences/system_preferences.h"
#include "../../Parsing/parsing_asm_file.h"

#define MAX_REG_NAME_LEN 4

struct address_method
{
	int(*addressing_func)(const char *);
	enum addressing_methods method;
};

char *StringAddMethod(enum addressing_methods addressing_mod)
{
	switch(addressing_mod)
	{
		case IMMEDIATE_ADDRESSING:
			return "immediate";
		case DIRECT_ADDRESSING:
			return "direct";
		case INDEX_ADDRESSING:
			return "index";
		case REGISTER_ADDRESSING:
			return "register";
		default:
			return NULL;
	}
	
	return NULL;
}

static int IsImmediateAddressingMethod(const char *method)
{
	return (method != NULL) && (*method == '#') && (IsNumber(method + 1));
}

static int IsRegisterAddressingMethod(const char *method)
{
	return (method != NULL) && IsValidRegister(method);
}

static int IsDirectAddressingMethod(const char *method)
{
	int is_direct = 1;
	
	if(method == NULL)
	{
		return 0;
	}
	
	if(!isalpha(*method))
	{
		is_direct = 0;
	}
	
	for(; is_direct != 0 && *method != '\0'; ++method)
	{
		if(!isalnum(*method))
		{
			is_direct = 0;
		}
	}
	
	return is_direct;
}

static int CreateRegName(char *reg_name_buffer, const char *str)
{
	size_t reg_name_len = strlen(str) - 1;
	strncpy(reg_name_buffer, str, reg_name_len);
	reg_name_buffer[reg_name_len] = '\0';
	
	return str[reg_name_len] == ']';
} 

static int IsIndexAddressingMethod(const char *method)
{
	int is_index = 1;
	char reg_name[MAX_REG_NAME_LEN] = {'\0'};
	
	if(method == NULL)
	{
		return 0;
	}
	
	if(!isalpha(*method))
	{
		is_index = 0;
	}
	
	for(; is_index != 0 && *method != '\0'; ++method)
	{
		if(*method == '[')
		{
			break;
		}
			
		else if(!isalnum(*method))
		{
			is_index = 0;
		}
	}
	
	if (!CreateRegName(reg_name, ++method) || !IsValidRegisterForIndex(reg_name))
	{
		is_index = 0;
	}
	
	return is_index;
}

unsigned int AddressingMethodNumber(enum addressing_methods address_method)
{
	switch(address_method)
	{
		case NONE:
			return 0;
		case IMMEDIATE_ADDRESSING:
			return 0;
		case REGISTER_ADDRESSING:
			return 3;
		case DIRECT_ADDRESSING:
			return 1;
		case INDEX_ADDRESSING:
			return 2;
		default:
			return -1;
	}
	
	return -1;
}

enum addressing_methods GetAddressingMethod(const char *str)
{
	struct address_method address_methods[] = {{IsImmediateAddressingMethod, IMMEDIATE_ADDRESSING},
	                                           {IsRegisterAddressingMethod, REGISTER_ADDRESSING},
	                                           {IsDirectAddressingMethod, DIRECT_ADDRESSING},
	                                           {IsIndexAddressingMethod, INDEX_ADDRESSING}};
	int i = 0;
	size_t address_methods_len = sizeof(address_methods) / sizeof(*address_methods);
	
	for (i = 0; i < address_methods_len; ++i)
	{
		if(address_methods[i].addressing_func(str))
		{
			break;
		}
	}
	
	return (i != address_methods_len)? address_methods[i].method : INVALID;
}

size_t GetWordsForAddressMethod(enum addressing_methods address_method)
{
	switch(address_method)
	{
		case NONE:
			return 0;
		case IMMEDIATE_ADDRESSING:
			return 1;
		case REGISTER_ADDRESSING:
			return 0;
		case DIRECT_ADDRESSING:
			return 2;
		case INDEX_ADDRESSING:
			return 2;
		default:
			return -1;
	}
	
	return -1;
}

static unsigned int IndexGetRegister(const char *str)
{
	char reg[MAX_REG_NAME_LEN] = {'\0'};

	while (*str != '[')
	{
		++str;
	}

	CreateRegName(reg, str);

	return GetRegNumber(reg);
}

unsigned int GetRegisterNumber(const char *str, enum addressing_methods addressing_method)
{
	switch(addressing_method)
	{
		case NONE:
			return 0;
		case IMMEDIATE_ADDRESSING:
			return 0;
		case REGISTER_ADDRESSING:
			return GetRegNumber(str);
		case DIRECT_ADDRESSING:
			return 0;
		case INDEX_ADDRESSING:
			return IndexGetRegister(str);
		default:
			return -1;
	}
}