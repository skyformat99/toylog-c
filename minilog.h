
#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#define MULTI_THREAD 

#define miniLog(fmt, arg...) \
    do{\
        miniLog_write_log(MINILOG_LEVEL_LOG, fmt, ##arg); \
    }while(0)

#define miniErr(fmt, arg...) \
    do{\
        miniLog_write_log(MINILOG_LEVEL_ERR, fmt, ##arg); \
    }while(0)

#define miniInf(fmt, arg...) \
    do{\
        miniLog_write_log(MINILOG_LEVEL_INF, fmt, ##arg); \
    }while(0)

#define miniTrc(fmt, arg...) \
    do{\
        miniLog_write_log(MINILOG_LEVEL_TRC, fmt, ##arg); \
    }while(0)

#define MINILOG_LEVEL_LOG   (1 << 3)
#define MINILOG_LEVEL_ERR   (1 << 2)
#define MINILOG_LEVEL_INF   (1 << 1)
#define MINILOG_LEVEL_TRC   (1 << 0)
#define MINILOG_LEVEL_ALL   (0x0ff)

#define MINILOG_TAG_TIME    (1 << 8)
#define MINILOG_TAG_DATE    (1 << 9)
#define MINILOG_TAG_PID     (1 << 10)
#define MINILOG_TAG_PPID    (1 << 11)
#define MINILOG_TAG_ALL     (0x0ff00)

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

int miniLog_init(const char * file_name);
int miniLog_update_file(logbase * pLog, const char * file_name);
int miniLog_open_file(logbase * pLog);
int miniLog_write_log(int level, const char * fmt, ...);
int miniLog_write_log_v(int level, const char * fmt, va_list arg_list);
int miniLog_write_file(int level, const char * fmt, va_list arg_screen, FILE * fp);
int miniLog_close_file(logbase * pLog);
int miniLog_end();
int miniLog_destroy();
int miniLog_setlevel(int levels);
int miniLog_dellevel(int level);
int miniLog_addlevel(int level);

int miniLog_write_header(int level, FILE * fp);
int miniLog_write_time  (int level, FILE * fp);
int miniLog_write_pid   (int level, FILE * fp);
int miniLog_write_ppid  (int level, FILE * fp);

#endif

