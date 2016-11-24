#ifndef _TOYLOG_CONSOLE_H_
#define _TOYLOG_CONSOLE_H_
#include "toytypes.h"

int toylog_write_console(LogOutput * output, int priority, const char * fmt, va_list arg_list);

#endif /* end file */

