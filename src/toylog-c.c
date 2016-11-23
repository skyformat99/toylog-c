#include <unistd.h>
#include "toytypes.h"
#include "toylog-c.h"

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

/* ***********************
 * static functions
 * **********************/
/* 
int toyLog_get_default_name(char * file_name, int len);
int toyLog_check_file(logbase * pLog);
int toyLog_open_file(logbase * pLog);

int toyLog_write_log(int level, const char * fmt, ...);
int toyLog_write_log_v(int level, const char * fmt, va_list arg_list);
int toyLog_write_file(int level, const char * fmt, va_list arg_screen, FILE * fp);
int toyLog_close_file(logbase * pLog);
int toyLog_destroy();
int toyLog_setlevel(int levels);
int toyLog_dellevel(int level);
int toyLog_addlevel(int level);

int toyLog_write_header(int level, FILE * fp);
int toyLog_write_time  (int level, FILE * fp);
int toyLog_write_pid   (int level, FILE * fp);
int toyLog_write_ppid  (int level, FILE * fp);

int toyLog_get_default_name(char * file_name, int len)
{
    if(NULL == file_name || len < 0)
    {
        return -1;
    }
    int ret = readlink("/proc/self/exe", file_name, len);
    if(ret < 0)
    {
        return -1;
    }
    file_name[ret] = '\0';
    snprintf(file_name + ret, len - ret, ".log");

    return 0;
}

int toyLog_init(const char * file_name)
{
    const char * toyLog_file_name = NULL;
    if(NULL == file_name)
    {
        toyLog_file_name = "/var/miniLog.log";
    }
    else
    {
        toyLog_file_name = file_name;
    }
    if(NULL == g_Log)
    {
        g_Log = (logbase *)malloc(sizeof(logbase));
        memset(g_Log, 0, sizeof(logbase));
        g_Log -> file_name = NULL;
        g_Log -> file_point = NULL;
    }
    if(0 != toyLog_update_file(g_Log, miniLog_file_name))
    {
        return -1;
    }
#ifdef MULTI_THREAD 
    memset(& g_Log -> p_Mutex, sizeof(g_Log ->  p_Mutex), 0);
    pthread_mutex_init(& g_Log -> p_Mutex, NULL);
#endif

    return toyLog_open_file(g_Log);
}

int toyLog_check_file(logbase * pLog)
{
    if(NULL == pLog || NULL == pLog -> file_point)
    {
        char file_name[MAX_FILE_NAME_LEN] = {0};
        if(toyLog_get_default_name(file_name, sizeof(file_name)) < 0)
        {
            return -1;
        }

        return toyLog_init(file_name);
    }

    return 0;
}

int toyLog_update_file(logbase * pLog, const char * file_name)
{
    if(NULL == pLog || NULL == file_name)
    {
        return -1;
    }
    if(NULL != pLog -> file_name)
    {
        free(pLog -> file_name);
        pLog -> file_name = NULL;
    }
    size_t name_len = strlen(file_name);
    char * pMem = (char *)malloc(name_len + 1);
    if(NULL == pMem)
    {
        return -1;
    }
    memcpy(pMem, file_name, name_len + 1);
    g_Log -> file_name = pMem;

    return 0;
}

int toyLog_open_file(logbase * pLog)
{
    if(NULL == pLog || NULL == pLog -> file_name)
    {
        return -1;
    }

    FILE * fp = fopen(pLog -> file_name, "a");
    if(NULL == fp)
    {
        fp = fopen(pLog -> file_name, "w");
    }
    if(NULL == fp)
    {
        return -1;
    }
    pLog -> file_point = fp;
    pLog -> on_screen = 1;
    toyLog_setlevel(MINILOG_LEVEL_ALL | MINILOG_TAG_ALL);
    //toyLog_addlevel(MINILOG_TAG_ALL);

    return 0;
}

int toyLog_close_file(logbase * pLog)
{
    if(NULL == pLog)
    {
        return -1;
    }
    if(NULL != pLog -> file_name)
    {
        free(pLog -> file_name);
        pLog -> file_name = NULL;
    }
    if(NULL != pLog -> file_point)
    {
        fclose(pLog -> file_point);
        pLog -> file_point = NULL;
    }

    return 0;
}

int toyLog_write_log_v(int ilevel, const char * fmt, va_list arg_list)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    if(0 == (ilevel & g_Log -> log_level))
    {
        return 0;
    }
#ifdef MULTI_THREAD 
    pthread_mutex_lock(& g_Log -> p_Mutex);
#endif
    if(g_Log -> on_screen)
    {
        FILE * fp = stdout;
        va_list arg_screen;
        va_copy(arg_screen, arg_list);

        toyLog_write_file(ilevel, fmt, arg_screen, fp);
    }
    
    if(NULL != g_Log -> file_point)
    {
        FILE * fp = g_Log -> file_point;
        va_list arg;
        va_copy(arg, arg_list);

        toyLog_write_file(ilevel, fmt, arg, fp);
    }

#ifdef MULTI_THREAD 
    pthread_mutex_unlock(& g_Log -> p_Mutex);
#endif

    return 0;
}

int toyLog_write_file(int ilevel, const char * fmt, va_list argptr, FILE * fp)
{
    toyLog_write_ppid(ilevel, fp);
    toyLog_write_pid(ilevel, fp);
    toyLog_write_header(ilevel, fp);
    vfprintf(fp, fmt, argptr);
    fprintf (fp, "\n");
    fflush  (fp);

    va_end(argptr);

    return 0;
}

int toyLog_write_log(int ilevel, const char * fmt, ...)
{
    toyLog_check_file(g_Log);

    va_list argptr;
    int cnt;

    va_start(argptr, fmt);
    cnt = toyLog_write_log_v(ilevel, fmt, argptr);
    va_end(argptr);

    return cnt;
}

int toyLog_destroy()
{
    if(NULL == g_Log)
    {
        return 0;
    }
    toyLog_close_file(g_Log);
    free(g_Log);
    g_Log = NULL;

    return -1;
}

int toyLog_end()
{
    return toyLog_destroy();
}

int toyLog_setlevel(int levels)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level = levels;

    return 0;
}

int toyLog_dellevel(int level)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level &= (~level);

    return 0;
}

int toyLog_addlevel(int level)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level |= level;

    return 0;
}

int toyLog_write_header(int ilevel, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    const char * pLevel = NULL;
    switch(ilevel)
    {
        case toyLOG_LEVEL_LOG : pLevel = "log";   break;
        case toyLOG_LEVEL_ERR : pLevel = "error"; break;
        case toyLOG_LEVEL_INF : pLevel = "info";  break;
        case toyLOG_LEVEL_TRC : pLevel = "track"; break;
        default : pLevel = ""; break;
    }
    fprintf(fp, "[%s] ", pLevel);

    return 0;
}

int toyLog_write_time  (int level, FILE * fp)
{

    return 0;
}

int toyLog_write_pid   (int level, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    if(g_Log -> log_level & toyLOG_TAG_PID)
    {
        pid_t p = getpid();
        fprintf(fp, "[%d] ", (int)p);
    }

    return 0;
}

int toyLog_write_ppid  (int level, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    if(g_Log -> log_level & toyLOG_TAG_PPID)
    {
        pid_t p = getppid();
        fprintf(fp, "[%d] ", (int)p);
    }

    return 0;
}
*/
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

int toylog_open_files (LogBody * log) {
    //TODO
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
    //TODO
    return 0;
}

int toyLog_write_log(int priority, const char * fmt, ...) {
    if(check_object() < 0) {
        return -1;
    }

    va_list argptr;
    int ret;

    va_start(argptr, fmt);
    ret = toyLog_write_files(priority, fmt, argptr);
    va_end(argptr);

    return ret;
}

