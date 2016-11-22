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

#define LOG_TYPE_CONCLE 0
#define LOG_TYPE_FILE   1
#define LOG_TYPE_SOCKET 2
#define LOG_TYPE_EMAIL  3
#define LOG_TYPE_DB     4
#define LOG_TYPE_WEB    5


typedef struct
{
    const char * config;
    ToyBitmap map;
}LogMessage;

#define _MSG_TYPE_d     0x100
#define __MSG_TYPE_y    0x101
#define __MSG_TYPE_Y    0x102
#define __MSG_TYPE_m    0x103
#define __MSG_TYPE_b    0x104
#define __MSG_TYPE_d    0x105
#define __MSG_TYPE_u    0x106
#define __MSG_TYPE_a    0x107
#define __MSG_TYPE_H    0x108
#define __MSG_TYPE_I    0x109
#define __MSG_TYPE_M    0x10a
#define __MSG_TYPE_s    0x10b
#define __MSG_TYPE_l    0x10c

#define _MSG_TYPE_m     0x200
#define _MSG_TYPE_p     0x300
#define _MSG_TYPE_r     0x400
#define _MSG_TYPE_CHAR  0x500

typedef struct _LogLayout
{
    int layout_type;
    char * msg;
}LogLayout;

typedef struct 
{
    int  log_type;
    char *engine;
    char *protocol;
    char *log_file;
    FILE *out;
    char * layout;
    LogLayout * fomartted_layout;
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

