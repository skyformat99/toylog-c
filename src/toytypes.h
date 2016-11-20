#ifndef _TOYTYPES_H_
#define _TOYTYPES_H_
#include <stdio.h>
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

typedef struct 
{
    int  log_type;
    char *log_file;
    FILE *out;
    char * layout;
    int  priority;
    int  color;
#ifdef _TOY_MULTI_PTHREAD 
    pthread_mutex_t file_mutex;
#endif
} LogOutput;

typedef struct
{
    LogOutput ** output_list;
#ifdef _TOY_MULTI_PTHREAD 
    pthread_mutex_t body_mutex;
#endif
} LogBody;

int toylog_convert_priority(const char * priority);
void show_logoutput(LogOutput * log);

#endif /* end file */

