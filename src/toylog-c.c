#include <unistd.h>
#include "toytypes.h"
#include "toylog-c.h"
#include "toylog-file.h"
#include "toylog-write.h"

LogBody * _g_Log = NULL;
const char * _g_version = "0.1.1";
const char * _g_help_info[] = 
{
    "config patternlayout : ",
    "%d - the date",
    "    %y   last two digits of year (00..99)",
    "    %Y   year",
    "    %m   month (01..12)",
    "    %b   locale's abbreviated month name (e.g., Jan)",
    "    %d   day of month (e.g., 01)",
    "    %u   day of week (1..7); 1 is Monday",
    "    %a   locale's abbreviated weekday name (e.g., Sun)",
    "    %H   hour (00..23)",
    "    %I   hour (01..12)",
    "    %M   minute (00..59)",
    "    %S   second (00..60)",
    "    %l   milliseconds",
    "%m - the message",
    "%p - the priority",
    "%r - milliseconds since this layout was created",
    "",
    NULL
};

/* **********************
 * API functions
 * date : 2016-11-19
 * *********************/
const char * toylog_version()
{
    return _g_version;
}

const char ** toylog_help()
{
    return _g_help_info;
}

int toylog_open_file(LogOutput * output) {
    if(NULL == output) {
        return -1;
    }
    gettimeofday(& output -> start, NULL);
    switch(output -> log_type) {
        case LOG_TYPE_CONCLE :
            output -> out = stdout;
            break;
        case LOG_TYPE_FILE :
            if(NULL != output -> log_file) {
                char szbuf[MAX_FILE_NAME_LEN] = {0};
                get_filename(szbuf, sizeof(szbuf), output, 0);
                TOYDBG("open file : [%s]", szbuf);
                output -> out = fopen(szbuf, "a");
            }
            break;
        default :
            break;
    }

    return 0;
}

int toylog_open_files (LogBody * log) {
    if(NULL == log) {
        return -1;
    }
    int i = 0;
    for(i = 0; i < log -> list_count; i++) {
        toylog_open_file(log -> output_list[i]);
    }

    return 0;
}

int toylog_init(const char * config_file)
{
    if(NULL == config_file) {
        return -1;
    }
    if(access(config_file, R_OK) != 0) {
        return -1;
    }
    _g_Log = (LogBody *)malloc_mem(sizeof(LogBody));
    if(NULL == _g_Log) {
        return -1;
    }
    pthread_mutex_init(&_g_Log -> body_mutex, NULL);// = PTHREAD_MUTEX_INITIALIZER;
    int ret = 0;
    pthread_mutex_lock(& _g_Log -> body_mutex);
    ret = parse_file(_g_Log, config_file);
    if(0 == ret) {
        ret = toylog_open_files(_g_Log);
    }
    pthread_mutex_unlock(& _g_Log -> body_mutex);

    return ret;
}

int toylog_priority_set(int priority, const char * toyfile){
    if(NULL == _g_Log || NULL == _g_Log -> output_list) {
        TOYDBG("_g_Log or it's output_list is empty");
        return -1;
    }
    int i = 0;
    for(i = 0; i < _g_Log -> list_count; i++) {
        if(NULL == toyfile) {
            _g_Log -> output_list[i] -> priority = priority;
            continue;
        } else if(strcasecmp(toyfile, _g_Log -> output_list[i] -> log_file) == 0) {
            _g_Log -> output_list[i] -> priority = priority;
            break;
        }
    }

    return 0;
}

int toylog_end() {
    if(NULL == _g_Log) {
        TOYDBG("_g_Log is empty");
        return -1;
    }
    LogBody * p = _g_Log;
    pthread_mutex_lock(& _g_Log -> body_mutex);
    free_logbody(_g_Log);
    _g_Log = NULL;
    pthread_mutex_unlock(& p -> body_mutex);
    free_mem(p);

    return 0;
}

int toyLog_write_files(int priority, const char * fmt, va_list arg_list) {
    int i = 0; 
    for(i = 0; i < _g_Log -> list_count; i++) {
        va_list arg_screen;
        va_copy(arg_screen, arg_list);
        toylog_write_mutex(_g_Log -> output_list[i], priority, fmt, arg_screen);
    }

    return 0;
}

int toylog_check_object(LogBody * log) {
    if(NULL == log) {
        return -1;
    }

    return 0;
}

int toylog_write_log(int priority, const char * fmt, ...) {
    if(toylog_check_object(_g_Log) < 0) {
        return -1;
    }

    va_list argptr;
    int ret;

    va_start(argptr, fmt);
    ret = toyLog_write_files(priority, fmt, argptr);
    va_end(argptr);

    return ret;
}

