#ifndef _TOYTYPES_H_
#define _TOYTYPES_H_
#include "toybitmap.h"

//#define _TOY_MULTI_PTHREAD 
#ifdef _TOY_MULTI_PTHREAD
#include <pthread.h>
#endif

#define TOYLOG_PRIORITY_FATAL   (1 << 0)
#define TOYLOG_PRIORITY_ALERT   (1 << 1)
#define TOYLOG_PRIORITY_CRIT    (1 << 2)
#define TOYLOG_PRIORITY_ERROR   (1 << 3)
#define TOYLOG_PRIORITY_WARN    (1 << 4)
#define TOYLOG_PRIORITY_NOTICE  (1 << 5)
#define TOYLOG_PRIORITY_INFO    (1 << 6)
#define TOYLOG_PRIORITY_DEBUG   (1 << 7)
#define TOYLOG_PRIORITY_RACE    (1 << 8)
#define TOYLOG_PRIORITY_NOTSET  (1 << 9)
#define TOYLOG_PRIORITY_UNKNOWN (1 << 10)

#define MAX_FILE_NAME_LEN 1024

#define LOG_TYPE_FILE   0
#define LOG_TYPE_CONCLE 1

typedef struct
{
    const char * config;
    ToyBitmap map;
}LogMessage;

#define PATTERN_DATA    1
#define PATTERN_MESG    2
typedef struct 
{
    int type;
    const char * file_name;
    FILE * fp;
    int  color;
    ToyBitmap *map;
}LogLayout;

typedef int (*CLOSE_FUN)(FILE *);
typedef struct 
{
    const char *log_file_name;
    int         log_type;
    FILE       *out;
    CLOSE_FUN   close_fun;
#ifdef _TOY_MULTI_PTHREAD 
    pthread_mutex_t mutex;
#endif
} LogOutput;

typedef struct
{
    const char * file_name;
    FILE * file_point;
    int    priority;
#ifdef _TOY_MULTI_PTHREAD 
    pthread_mutex_t p_Mutex;
#endif
} LogBody;

#endif /* end file */

