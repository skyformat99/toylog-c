#ifndef _TOYLOG_DB_H_
#define _TOYLOG_DB_H_

int toylog_write_db(LogOutput * output, int priority, const char * fmt, va_list arg_list);

#endif 

