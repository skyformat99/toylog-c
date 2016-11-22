#ifndef _PARSER_H_
#define _PARSER_H_
#include "toybitmap.h"
#include "toytypes.h"

int parse_file  (LogBody * log, const char * file);
int parse_memory(LogBody * log, const char * mem);
int parse_lines(LogBody * log, char * const* line_list);
int read_file(char ** line_list, const char * file_name);
int split_memory(char *** line_list, const char *mem);
void free_line_list(char *** line_list);

int get_first_not_of(const char * p, int len, const char * sub);
void free_mem(void * p);
void *malloc_mem(size_t size);
char *toy_chrdup(char c);
char *toy_strndup(const char *s, size_t n);
int get_char_count(const char * mem, int len, const char * ptn);

#endif /* end file */

