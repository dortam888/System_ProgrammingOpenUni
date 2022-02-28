#include <ctype.h> /* alnum */

#include "contract.h" /* contract */

#define MAX_LENGTH 81

static int IsCharPartOfSequence(char current_char, int previous_char);
static int InsertShortcutToNewString(char s2[], int j, int seq_counter, char previous);

void contract(char s1[], char s2[])
{
	int i = 0; /* index for input string */
	int j = 0; /* index for output string */
	char previous = '\0'; /* previous char in string */
	int seq_counter = 0; /* counter for sequence length (one char is 0 seq length)*/
	
	for(i = 0; s1[i] != '\0'; ++i)
	{
		if (IsCharPartOfSequence(s1[i], previous))
		{
			++seq_counter; /* raise sequence counter and skip*/
		}
			
		else /* if not part of sequence */
		{
			j = InsertShortcutToNewString(s2, j, seq_counter, previous);
			
			/*Always add current char to output*/
			s2[j] = s1[i];
		    ++j;		
			seq_counter = 0; /* Initializing sequence count */
		}
							
		previous = s1[i]; /* Update previous */
	}
	
	j = InsertShortcutToNewString(s2, j, seq_counter, previous); /* Handle sequence that ended in the last of the string */
	s2[j] = '\0'; /*Adding null byte to the end of the out string */
}

static int IsCharPartOfSequence(char current_char, int previous_char)
{
	return (isalnum(current_char) && isalnum(previous_char) && (current_char - previous_char == 1)); 
}

static int InsertShortcutToNewString(char s2[], int j, int seq_counter, char previous)
{

	if (seq_counter > 1) /* if sequence is greater than 1 (3 or more letters) insert the shortcuted string */
	{
		s2[j] = '-'; /* add shortcut */
		s2[j + 1] = previous; /* add the last char in sequence */								
		j += 2; 
	}
	
	else if (seq_counter == 1) /* if a sequence of 2 chars than write the skipped char */
	{
		s2[j] = previous;				
		++j;
	}
	
	return j;
}

