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

char *GetLabelFromIndexOperand(char *label, const char *operand);

int IsSentenceCorrect(const char *line);

int IsStartOfString(char c);

int IsEndOfString(char c, char c_prev);

#endif /* PARSING_H */
