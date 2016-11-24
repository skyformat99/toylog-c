#ifndef _TOYLOG_WRITE_H_
#define _TOYLOG_WRITE_H_
#include <stdio.h>

int toylog_write_console(LogOutput * output, int priority, const char * fmt, va_list arg_list);

#endif /* end file */

