/*
 * =====================================================================================
 *
 *       Filename:  toylog-console.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/24/2016 05:29:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "toylog-console.h"
#include "toytypes.h"
#include "toylog-write.h"

int toylog_write_console(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    int ret = 0;
    pthread_mutex_lock(& output -> file_mutex);
    if (NULL == output -> formatted_layout) {
        vfprintf(stdout, fmt, arg_list);
    } else {
        struct timeval start;
        gettimeofday(&start, NULL);
        struct tm t;
        mzero(&t, sizeof(t));
        toylog_localtime(&t, &start.tv_sec);
        int i = 0;
        for(i = 0; output -> formatted_layout[i].layout_type != 0; i++) {
            int  type = output -> formatted_layout[i].layout_type;
            char *msg = output -> formatted_layout[i].msg;

            switch(type) {
                case __MSG_TYPE_y : 
                    fprintf(stdout, "%02d", (t.tm_year + 1900) % 100);
                    break;
                case __MSG_TYPE_Y :
                    fprintf(stdout, "%04d", t.tm_year + 1900);
                    break;
                case __MSG_TYPE_m :
                    fprintf(stdout, "%02d", t.tm_mon);
                    break;
                case __MSG_TYPE_b :
                    fprintf(stdout, "%s", month_to_simple(t.tm_mon + 1));
                    break;
                case __MSG_TYPE_d :
                    fprintf(stdout, "%02d", t.tm_mday);
                    break;
                case __MSG_TYPE_u :
                    fprintf(stdout, "%s", week_to_full(t.tm_wday));
                    break;
                case __MSG_TYPE_a :
                    fprintf(stdout, "%s", week_to_simple(t.tm_wday));
                    break;
                case __MSG_TYPE_H :
                    fprintf(stdout, "%02d", t.tm_hour);
                    break;
                case __MSG_TYPE_I :
                    fprintf(stdout, "%02d", t.tm_hour % 12);
                    break;
                case __MSG_TYPE_M :
                    fprintf(stdout, "%02d", t.tm_min);
                    break;
                case __MSG_TYPE_S :
                    fprintf(stdout, "%02d", t.tm_sec);
                    break;
                case __MSG_TYPE_l :
                    fprintf(stdout, "%03ld", start.tv_usec / 1000);
                    break;
                case _MSG_TYPE_m :
                    vfprintf(stdout, fmt, arg_list);
                    break;
                case _MSG_TYPE_p :
                    fprintf(stdout, "%s", toylog_priority_str(type));
                    break;
                case _MSG_TYPE_r :
                    fprintf(stdout, "%ld", (start.tv_sec - output -> start.tv_sec) * 1000 + 
                            (start.tv_usec - output -> start.tv_usec) / 1000);
                    break;
                case _MSG_TYPE_CHAR :
                    fprintf(stdout, "%s", msg);
                    break;
                default :
                    break;
            }
        }
    }
    pthread_mutex_unlock(& output -> file_mutex);

    return ret;
}

