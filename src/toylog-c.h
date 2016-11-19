#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
#define TOYLOG_PRIORITY_ALL	    (0xffff)

#define MAX_FILE_NAME_LEN 1024

typedef int (*CLOSE_FUN)(FILE *);

typedef struct 
{
    FILE * out;
    CLOSE_FUN close_fun;
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

#define toylog_fatal   (fmt, arg...) \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_FATAL, fmt, ##arg);\
    }while(0)

#define toylog_alert   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_ALERT, fmt, ##arg);\
    }while(0)

#define toylog_crit    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_CRIT, fmt, ##arg);\
    }while(0)

#define toylog_error   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_ERROR, fmt, ##arg);\
    }while(0)

#define toylog_warn    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_WARN, fmt, ##arg);\
    }while(0)

#define toylog_notice  (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_NOTICE, fmt, ##arg);\
    }while(0)

#define toylog_info    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_INFO, fmt, ##arg);\
    }while(0)

#define toylog_debug   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_DEBUG, fmt, ##arg);\
    }while(0)

#define toylog_race    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_RACE, fmt, ##arg);\
    }while(0)

#define toylog_notset  (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_NOTSET, fmt, ##arg);\
    }while(0)

#define toylog_unknown (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_UNKNOWN, fmt, ##arg);\
    }while(0)

const char * toylog_version();
const char ** toylog_help();

int toylog_init_config(const char * config_file);
int toylog_priority_set(int priority);
int toylog_priority_filter(int priority);
int toylog_end();

#endif

