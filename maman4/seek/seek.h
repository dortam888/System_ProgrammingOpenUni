#ifndef SEEK
#define SEEK

/* 
	Description: 
		Prints the ascii value of all the characters in a given position in all files given to the function. 
	input:
		long char_position - The char position in the file (measured from the start of the file)
		char *file_names[] - Array of file names to print from
		int num_of_files - size of the array of files
	output:
		(none)
	Time Complxity:
		O(n) n is the number of files given to the function
	Space Complexity:
		O(1) No additional space required
*/
void PrintCharNumberInAllFiles(long char_position, char *file_names[], int num_of_files);

#endif /*SEEK*/
