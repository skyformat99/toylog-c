/*
 * =====================================================================================
 *
 *       Filename:  toylog-write.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/24/2016 06:08:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include "toydebug.h"
#include "toytypes.h"

int toylog_write_file(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    if (NULL == output -> formatted_layout && NULL != output -> out) {
        vfprintf(output -> out, fmt, arg_list);
        return 0;
    } 
    struct timeval start;
    gettimeofday(&start, NULL);
    struct tm t;
    mzero(&t, sizeof(t));
    toylog_localtime(&t, &start.tv_sec);
    int i = 0;
    for(i = 0; output -> formatted_layout[i].layout_type != 0; i++) {
        int  type = output -> formatted_layout[i].layout_type;
        char *msg = output -> formatted_layout[i].msg;
        FILE * fp = output -> out;

        switch(type) {
            case __MSG_TYPE_y : 
                fprintf(fp, "%02d", (t.tm_year + 1900) % 100);
                break;
            case __MSG_TYPE_Y :
                fprintf(fp, "%04d", t.tm_year + 1900);
                break;
            case __MSG_TYPE_m :
                fprintf(fp, "%02d", t.tm_mon);
                break;
            case __MSG_TYPE_b :
                fprintf(fp, "%s", month_to_simple(t.tm_mon + 1));
                break;
            case __MSG_TYPE_d :
                fprintf(fp, "%02d", t.tm_mday);
                break;
            case __MSG_TYPE_u :
                fprintf(fp, "%s", week_to_full(t.tm_wday));
                break;
            case __MSG_TYPE_a :
                fprintf(fp, "%s", week_to_simple(t.tm_wday));
                break;
            case __MSG_TYPE_H :
                fprintf(fp, "%02d", t.tm_hour);
                break;
            case __MSG_TYPE_I :
                fprintf(fp, "%02d", t.tm_hour % 12);
                break;
            case __MSG_TYPE_M :
                fprintf(fp, "%02d", t.tm_min);
                break;
            case __MSG_TYPE_S :
                fprintf(fp, "%02d", t.tm_sec);
                break;
            case __MSG_TYPE_l :
                fprintf(fp, "%03ld", start.tv_usec / 1000);
                break;
            case _MSG_TYPE_m :
                vfprintf(fp, fmt, arg_list);
                break;
            case _MSG_TYPE_p :
                fprintf(fp, "[%s]", toylog_priority_str(priority));
                break;
            case _MSG_TYPE_r :
                fprintf(fp, "%ld", (start.tv_sec - output -> start.tv_sec) * 1000 + 
                        (start.tv_usec - output -> start.tv_usec) / 1000);
                break;
            case _MSG_TYPE_CHAR :
                fprintf(fp, "%s", msg);
                break;
            default :
                break;
        }
        fflush(fp);
    }

    return 0;
}

int toylog_file_full(LogOutput * output) {
    if( NULL == output || 
        NULL == output -> log_file ||
        NULL == output -> out) {
        TOYDBG("output or filename is NULL");
        return 1;
    }
    struct stat buf;
    if(fstat(output -> out -> _fileno, &buf) < 0) {
        TOYDBG("get file fstat faild");
        return 1;
    }
    if(buf.st_size > output -> filesize) {
        return 1;
    }

    return 0;
}

int toylog_move_file(LogOutput * output, int index) {
    if(NULL == output) {
        return -1;
    }
    char szlbuf[MAX_FILE_NAME_LEN] = {0};
    char szgbuf[MAX_FILE_NAME_LEN] = {0};
    get_filename(szlbuf, sizeof(szlbuf), output, index);
    get_filename(szgbuf, sizeof(szgbuf), output, index + 1);
    if(access(szlbuf, F_OK) != 0) {
        TOYDBG("file not exists : [%s]", szlbuf);
        return 0;
    }
    if(index >= output -> filecount) {
        TOYDBG("remove file : [%s]", szlbuf);
        return remove(szlbuf);
    }
    toylog_move_file(output, index + 1);

    TOYDBG("rename file [%s] to [%s]", szlbuf, szgbuf);
    return rename(szlbuf, szgbuf);
}

int toylog_adjust_file(LogOutput * output) {
    if(NULL == output) {
        return -1;
    }
    char szbuf[MAX_FILE_NAME_LEN] = {0};
    get_filename(szbuf, sizeof(szbuf), output, 0);
    if(NULL != output -> file) {
        fclose(output -> out);
        output -> out = NULL;
    }
    truncate(szbuf, output -> filesize);
    toylog_move_file(output, 0);
    FILE * fp = fopen(szbuf, "a");
    if(NULL != fp) {
        output -> out = fp;
        return 0;
    }

    return -1;
}

int toylog_write_mutex(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    int ret = 0;
    pthread_mutex_lock(& output -> file_mutex);
        switch(output -> log_type) {
            case LOG_TYPE_CONCLE :
                TOYDBG("write console");
                toylog_write_file(output, priority, fmt, arg_list);
                break;
            case LOG_TYPE_FILE :
                {
                    //TOYDBG("write file");
                    va_list arg_screen;
                    va_copy(arg_screen, arg_list);
                    toylog_write_file(output, priority, fmt, arg_list);
                    if(toylog_file_full(output)) {
                        toylog_adjust_file(output);
                        toylog_write_file(output, priority, fmt, arg_screen);
                    }
                }
                break;
            default :
                break;
        }
    pthread_mutex_unlock(& output -> file_mutex);

    return ret;
}

