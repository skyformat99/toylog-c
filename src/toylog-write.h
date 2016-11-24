#ifndef _TOYLOG_WRITE_H_
#define _TOYLOG_WRITE_H_

int toylog_write(LogOutput * output, int priority, const char * fmt, va_list arg_list);

#endif /* end file */

