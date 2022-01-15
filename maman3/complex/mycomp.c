#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "complex.h"
#include "parsing.h"

#define MAX_COMMAND_LENGTH 81
#define NUMBER_OF_VARIABLES 6

typedef enum status {ENDED_SUCCESSFULLY, ENCOUNTERED_EOF_BEFORE_SESSION_STOPPED} session_status_t;

typedef struct operation
{
	const char *command;
	void(*operation_func)(complex *, const char *, error_t *);
} operation_t;

static session_status_t StartSession(complex variables[]);
static void InitializeVariables(complex *variables);
static void DestroyVariables(complex *variables);

int main()
{
	complex complex_variables[NUMBER_OF_VARIABLES]; /* complex numbers in which the user can put info */
	session_status_t session_status = ENDED_SUCCESSFULLY; /* Status of the session */
	
	InitializeVariables(complex_variables);  /*initialize the variables*/
	session_status = StartSession(complex_variables); /* starting the session with the user */
	DestroyVariables(complex_variables); /*free the variables at the end of the session*/
	
	return session_status;
}

/* 
	Initialize the session complex variables.
	input : complex *variables - array of variables to initialize
	output: (none)
*/
static void InitializeVariables(complex *complex_variables)
{
	int i = 0;
	
	for (i = 0; i < NUMBER_OF_VARIABLES; ++i)
	{
		complex_variables[i] = CreateComplex();
	}	
}

/* 
	Destroy the session complex variables in disk memory.
	input : complex *variables - array of variables to initialize
	output: (none)
*/
static void DestroyVariables(complex *complex_variables)
{
	int i = 0;
	
	for (i = 0; i < NUMBER_OF_VARIABLES; ++i)
	{
		DestroyComplex(complex_variables[i]); 
	}	
}

/* 
	Handle command error in case of undefined command
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: the float parameter in the command or 0 if the parameter is not valid
			in case the parameter is not valid error will be changed according to the mistake
*/
static void HandleCommandError(complex *variables, const char *delim, error_t *error)
{
	(void)variables;
	(void)delim;
	(void)error;
	
	printf("Undefined command name\n");
}

/* 
	Abstract function for ReadComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void ReadComplexAbsract(complex *variables, const char *delim, error_t *error)
{
	complex num_to_read = NULL; /* complex number to read */
	float real = 0; /* real parameter in read */
	float imag = 0; /* imag parameter in read */
	
	num_to_read = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	real = GetFloatParameter(delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
		
	imag = GetFloatParameter(delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if (!TooManyParameters(delim))
	{
		ReadComplex(num_to_read, real, imag);
	}	
}

/* 
	Abstract function for PrintComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void PrintComplexAbstract(complex *variables, const char *delim, error_t *error)
{
	complex num_to_print = GetComplexParameter(variables, delim, error); /* complex number to print */
	
	if (*error != NO_ERROR)
	{
		return;
	}
		
	if (!TooManyParameters(delim))
	{
		PrintComplex(num_to_print);
	}
}

/* 
	Abstract function for AddComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void AddComplexAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for substract Complex with complex operation*/
	complex first_parameter = NULL; /* First parameter for the function */
	complex second_parameter = NULL; /* Second parameter for the function */
	
	first_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	second_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if(!TooManyParameters(delim))
	{
		result = AddComplex(first_parameter, second_parameter);	 
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Abstract function for SubComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void SubtractComplexAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for substract Complex with complex operation*/
	complex first_parameter = NULL; /* First parameter for the function */
	complex second_parameter = NULL; /* Second parameter for the function */
	
	first_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	second_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if(!TooManyParameters(delim))
	{
		result = SubtractComplex(first_parameter, second_parameter);	
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Abstract function for MultiplyComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void MultiplyComplexAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for Multiply Complex with complex operation*/
	complex first_parameter = NULL; /* First parameter for the function */
	complex second_parameter = NULL; /* Second parameter for the function */
	
	first_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	second_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if (!TooManyParameters(delim))
	{	
		result = MultiplyComplex(first_parameter, second_parameter);	
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Abstract function for MultiplyComplexImag that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void MultiplyComplexImagAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for Multiply Complex with imag operation*/
	complex first_parameter = NULL; /* First parameter for the function */
	float second_parameter = 0; /* Second parameter for the function */
	
	first_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	second_parameter = GetFloatParameter(delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if(!TooManyParameters(delim))
	{	
		result = MultiplyComplexImag(first_parameter, second_parameter);	
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Abstract function for MultiplyComplexReal that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void MultiplyComplexRealAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for Multiply Complex with real operation*/
	complex first_parameter = NULL; /* First parameter for the function */
	float second_parameter= 0; /* Second parameter for the function */
	
	first_parameter = GetComplexParameter(variables, delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	second_parameter = GetFloatParameter(delim, error);
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if(!TooManyParameters(delim))
	{
		result = MultiplyComplexImag(first_parameter, second_parameter);
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Abstract function for AbsComplex that gets the parameters from stdin
	input : complex *variables - array of complex variables
			const char *delim - the delimeters in which we separated the command line
			error_t *error - indicator for input error
	output: (none)
*/
static void AbsComplexAbstract(complex *variables, const char *delim, error_t *error)
{
	complex result = NULL; /*result for abs operation*/
	complex parameter = NULL; /*the parameter from stdin*/
	
	parameter = GetComplexParameter(variables, delim, error);
	
	if (*error != NO_ERROR)
	{
		return;
	}
	
	if (!TooManyParameters(delim))
	{
		result = AbsComplex(parameter);
		PrintComplex(result);
		DestroyComplex(result);
	}
}

/* 
	Find the function index from array of operation
	input : operation_t array_of_operations - array of operations to choose from
			const char *command - command name from stdin
	output: index in array of operation that match the command name
*/
static int FindFunction(operation_t array_of_operations[], const char *command)
{
	int i = 0;
		
	for (i = 0; array_of_operations[i].command != NULL; ++i)
	{
		if(strcmp(array_of_operations[i].command, command) == 0)
		{
			break;
		}
	}
	
	return i;	
}

/* 
	Print End Of session message
	input : session_status_t status - the status that the session ended in
	output: the session ending status
*/
static session_status_t PrintEndSessionMessage(session_status_t status)
{
	status? printf("Error: Session stopped before stop was reached\n") : printf("Session ended successfully\n");
	return status;	
}

/* 
	Starting a session with the user
	input : complex *variables - array of complex variables
	output: session ending status if session ended with stop return session ENDED_SUCCESSFULLY.
			if encountered EOF before stop retuns ENCOUNTERED_EOF_BEFORE_SESSION_STOPPED 
*/
static session_status_t StartSession(complex variables[])
{
	session_status_t status = ENCOUNTERED_EOF_BEFORE_SESSION_STOPPED; /* session status in case of illegal end of session */ 	
	operation_t array_of_operations[] = {{"read_comp", ReadComplexAbsract},  /* array of operation and there expected command */
										 {"print_comp", PrintComplexAbstract},
										 {"add_comp",  AddComplexAbstract},
										 {"sub_comp", SubtractComplexAbstract},
										 {"mult_comp_real", MultiplyComplexRealAbstract},
										 {"mult_comp_img", MultiplyComplexImagAbstract},
										 {"mult_comp_comp", MultiplyComplexAbstract},
										 {"abs_comp", AbsComplexAbstract},
										 {NULL, HandleCommandError}
										 };
	
	while (1)
	{
		int i = 0; /* index in array of operation*/
		char line[MAX_COMMAND_LENGTH] = {'\0'}; /*buffer for command line in stdin*/
		char *command = NULL; /*pointer for command name*/
		const char *delim = ", 	\n"; /*the delimeters which separated the command*/
		error_t error = NO_ERROR; /*error indicator*/
		
		printf("Enter A Command >  ");

		if (fgets(line, MAX_COMMAND_LENGTH, stdin) == NULL)
		{
			break;
		}
		
		printf("%s", line);
		
		if (!CheckCommandCommasErrors(line)) /*parse command for error in commas order*/
		{
			continue;
		}
		
		command = strtok(line, delim); /*separate command name*/
		
		if (strcmp(command, "stop") == 0)
		{
			if(strtok(NULL, delim) == NULL) /*if stop encountered and no other parameters included end session*/
			{
				status = ENDED_SUCCESSFULLY;
				break;
			}
			else
			{
				printf("Extraneous text after end of command\n");
				continue;
			}
		}
		
		i = FindFunction(array_of_operations, command); /*Find function to execute from array_of_operations*/
		array_of_operations[i].operation_func(variables, delim, &error); /*execute correct operation*/	
	}
	
	return PrintEndSessionMessage(status);
}

