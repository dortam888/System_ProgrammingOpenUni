#ifndef FILEUTIL_H
#define FILEUTIL_H

#include <stdio.h>

#define MAX_LINE_LEN 100
#define MAX_FILE_NAME 255

char *PutEndingToFileName(char *dest, const char *file_name, const char *ending);

void FileCat(FILE *dest, const char *src_file_name);

#endif /*FILEUTIL_H*/
