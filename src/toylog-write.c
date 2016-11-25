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
                TOYDBG("write file");
                toylog_write_file(output, priority, fmt, arg_list);
                break;
            default :
                break;
        }
    pthread_mutex_unlock(& output -> file_mutex);

    return ret;
}

