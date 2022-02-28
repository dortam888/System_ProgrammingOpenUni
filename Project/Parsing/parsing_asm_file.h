#ifndef PARSING_H
#define PARSING_H

#define BINARY_BASE 2
#define OCTA_BASE 8
#define DECIMAL_BASE 10
#define HEX_BASE 16

int IsNumber(const char* number);

int IsComment(const char *line);

int IsLabel(const char *line);

int IsAttribute(const char *line);

char *GetLabelName(char *label_name_buffer, const char *str);


#endif /* PARSING_H */
