#ifndef _TOYLOG_DB_H_
#define _TOYLOG_DB_H_

#define ENGINE_SQLITE   1
#define ENGINE_MYSQL    2

typedef struct {
    int db_engine_type;
    void * conn;
}LogDB;

int toylog_write_to_db(LogOutput * output, int priority, const char * fmt, va_list arg_list);
int toylog_open_db(LogOutput * output);
int toylog_close_db(LogOutput * output);

#endif 

