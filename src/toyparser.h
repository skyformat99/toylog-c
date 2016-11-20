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

#endif /* end file */

