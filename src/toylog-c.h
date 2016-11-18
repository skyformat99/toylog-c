#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#define MULTI_THREAD 

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

int toyLog(const char * fmt, ...);
int toyErr(const char * fmt, ...);
int toyAlt(const char * fmt, ...);
int toyInf(const char * fmt, ...);
int toyTrc(const char * fmt, ...);
int toyDbg(const char * fmt, ...);


        toyLog_write_log(TOYLOG_LEVEL_LOG, fmt, ##arg); \

    do{\
        toyLog_write_log(TOYLOG_LEVEL_ERR, fmt, ##arg); \
    }while(0)

#define toyInf(fmt, arg...) \
    do{\
        toyLog_write_log(TOYLOG_LEVEL_INF, fmt, ##arg); \
    }while(0)

#define toyTrc(fmt, arg...) \
    do{\
        toyLog_write_log(TOYLOG_LEVEL_TRC, fmt, ##arg); \
    }while(0)

#define toyLOG_LEVEL_LOG   (1 << 3)
#define toyLOG_LEVEL_ERR   (1 << 2)
#define toyLOG_LEVEL_INF   (1 << 1)
#define toyLOG_LEVEL_TRC   (1 << 0)
#define toyLOG_LEVEL_ALL   (0x0ff)

#define toyLOG_TAG_TIME    (1 << 8)
#define toyLOG_TAG_DATE    (1 << 9)
#define toyLOG_TAG_PID     (1 << 10)
#define toyLOG_TAG_PPID    (1 << 11)
#define toyLOG_TAG_ALL     (0x0ff00)

#define MAX_FILE_NAME_LEN 1024

typedef struct
{
    char * file_name;
    FILE * file_point;
    int    log_grade;
    int    log_level;   //0x0ffffff 
    int    on_screen;
#ifdef MULTI_THREAD 
    pthread_mutex_t p_Mutex;
#endif
} logbase;

extern logbase * g_Log;
//extern const char g_LogType[][];

int toyLog_get_default_name(char * file_name, int len);
int toyLog_init(const char * file_name);
int toyLog_check_file(logbase * pLog);
int toyLog_update_file(logbase * pLog, const char * file_name);
int toyLog_open_file(logbase * pLog);

int toyLog_write_log(int level, const char * fmt, ...);
int toyLog_write_log_v(int level, const char * fmt, va_list arg_list);
int toyLog_write_file(int level, const char * fmt, va_list arg_screen, FILE * fp);
int toyLog_close_file(logbase * pLog);
int toyLog_end();
int toyLog_destroy();
int toyLog_setlevel(int levels);
int toyLog_dellevel(int level);
int toyLog_addlevel(int level);

int toyLog_write_header(int level, FILE * fp);
int toyLog_write_time  (int level, FILE * fp);
int toyLog_write_pid   (int level, FILE * fp);
int toyLog_write_ppid  (int level, FILE * fp);

#endif

