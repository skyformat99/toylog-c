#ifndef _TOYLOG_FILE_H_
#define _TOYLOG_FILE_H_
#include <stdio.h>
#include "toytypes.h"

int get_filename(char *szfilename, int len, LogOutput * output, int index);
int toylog_write_file(LogOutput * output, int priority, const char * fmt, va_list arg_list);
int toylog_file_full(LogOutput * output);
int toylog_move_file(LogOutput * output, int index);
int toylog_adjust_file(LogOutput * output);

#endif

