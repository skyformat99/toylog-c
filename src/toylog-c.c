#include <unistd.h>
#include "toylog-c.h"

#ifdef MULTI_THREAD 
#include <pthread.h>
#endif

logbase * g_Log = NULL;

int miniLog_get_default_name(char * file_name, int len)
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

int miniLog_init(const char * file_name)
{
    const char * miniLog_file_name = NULL;
    if(NULL == file_name)
    {
        miniLog_file_name = "/var/miniLog.log";
    }
    else
    {
        miniLog_file_name = file_name;
    }
    if(NULL == g_Log)
    {
        g_Log = (logbase *)malloc(sizeof(logbase));
        memset(g_Log, 0, sizeof(logbase));
        g_Log -> file_name = NULL;
        g_Log -> file_point = NULL;
    }
    if(0 != miniLog_update_file(g_Log, miniLog_file_name))
    {
        return -1;
    }
#ifdef MULTI_THREAD 
    memset(& g_Log -> p_Mutex, sizeof(g_Log ->  p_Mutex), 0);
    pthread_mutex_init(& g_Log -> p_Mutex, NULL);
#endif

    return miniLog_open_file(g_Log);
}

int miniLog_check_file(logbase * pLog)
{
    if(NULL == pLog || NULL == pLog -> file_point)
    {
        char file_name[MAX_FILE_NAME_LEN] = {0};
        if(miniLog_get_default_name(file_name, sizeof(file_name)) < 0)
        {
            return -1;
        }

        return miniLog_init(file_name);
    }

    return 0;
}

int miniLog_update_file(logbase * pLog, const char * file_name)
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

int miniLog_open_file(logbase * pLog)
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
    miniLog_setlevel(MINILOG_LEVEL_ALL | MINILOG_TAG_ALL);
    //miniLog_addlevel(MINILOG_TAG_ALL);

    return 0;
}

int miniLog_close_file(logbase * pLog)
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

int miniLog_write_log_v(int ilevel, const char * fmt, va_list arg_list)
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

        miniLog_write_file(ilevel, fmt, arg_screen, fp);
    }
    
    if(NULL != g_Log -> file_point)
    {
        FILE * fp = g_Log -> file_point;
        va_list arg;
        va_copy(arg, arg_list);

        miniLog_write_file(ilevel, fmt, arg, fp);
    }

#ifdef MULTI_THREAD 
    pthread_mutex_unlock(& g_Log -> p_Mutex);
#endif

    return 0;
}

int miniLog_write_file(int ilevel, const char * fmt, va_list argptr, FILE * fp)
{
    miniLog_write_ppid(ilevel, fp);
    miniLog_write_pid(ilevel, fp);
    miniLog_write_header(ilevel, fp);
    vfprintf(fp, fmt, argptr);
    fprintf (fp, "\n");
    fflush  (fp);

    va_end(argptr);

    return 0;
}

int miniLog_write_log(int ilevel, const char * fmt, ...)
{
    miniLog_check_file(g_Log);

    va_list argptr;
    int cnt;

    va_start(argptr, fmt);
    cnt = miniLog_write_log_v(ilevel, fmt, argptr);
    va_end(argptr);

    return cnt;
}

int miniLog_destroy()
{
    if(NULL == g_Log)
    {
        return 0;
    }
    miniLog_close_file(g_Log);
    free(g_Log);
    g_Log = NULL;

    return -1;
}

int miniLog_end()
{
    return miniLog_destroy();
}

int miniLog_setlevel(int levels)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level = levels;

    return 0;
}

int miniLog_dellevel(int level)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level &= (~level);

    return 0;
}

int miniLog_addlevel(int level)
{
    if(NULL == g_Log)
    {
        return -1;
    }
    g_Log -> log_level |= level;

    return 0;
}

int miniLog_write_header(int ilevel, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    const char * pLevel = NULL;
    switch(ilevel)
    {
        case MINILOG_LEVEL_LOG : pLevel = "log";   break;
        case MINILOG_LEVEL_ERR : pLevel = "error"; break;
        case MINILOG_LEVEL_INF : pLevel = "info";  break;
        case MINILOG_LEVEL_TRC : pLevel = "track"; break;
        default : pLevel = ""; break;
    }
    fprintf(fp, "[%s] ", pLevel);

    return 0;
}

int miniLog_write_time  (int level, FILE * fp)
{

    return 0;
}

int miniLog_write_pid   (int level, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    if(g_Log -> log_level & MINILOG_TAG_PID)
    {
        pid_t p = getpid();
        fprintf(fp, "[%d] ", (int)p);
    }

    return 0;
}

int miniLog_write_ppid  (int level, FILE * fp)
{
    if(NULL == fp)
    {
        return -1;
    }
    if(g_Log -> log_level & MINILOG_TAG_PPID)
    {
        pid_t p = getppid();
        fprintf(fp, "[%d] ", (int)p);
    }

    return 0;
}

