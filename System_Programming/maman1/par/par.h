#ifndef PAR
#define PAR

#define MAX_LINE_LENGTH 101

typedef enum balance {UNBALANCE = 0, BALANCE = 1} balance_t;

/* 
	Return if a text file is parenthesis balanced 
	input: (void)
	output: Returns BALANCE if text is parenthesis balanced and UNBALACED if the text is parenthesis unbalanced
	Time Complexity: O(n*m) n is the number of lines in file and m is the number of characters in each line can be at most MAX_LINE_LENGTH
	Space Complexity: O(m) for the line and the parenthesis buffer. m is the of characters in each line can be at most MAX_LINE_LENGTH
*/
balance_t IsTextBalance(); 

/* 
	Returns if a single line from a text is balanced 
	input: char line[] - A string that contains the line 
	input: char par_buffer[] - A buffer that contains the open parenthesis in the line
	output: Returns BALANCE if line is parenthesis balanced and UNBALACED if the text is parenthesis unbalanced
	Time Complexity: O(m) m is number of characters in line can be at most MAX_LINE_LENGTH
	Space Complexity: O(1) No extra space is required
*/   
balance_t IsLineBalance(char line[], char par_buffer[]);

/* 
	Print if text is balanced or unbalanced
	input: balance_t is_balance - BALANCE or UNBALANCE message in text
	Time Complexity: O(1)
	Space Complexity: O(1)
*/
void PrintBalanceMessage(balance_t is_balance);

#endif /* PAR */
