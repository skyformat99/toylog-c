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

int toylog_write_console(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    int ret = 0;
    pthread_mutex_lock(& log -> body_mutex);
    if (NULL == formatted_layout) {
        vfprintf(stdout, fmt, arg_list);
    } else {
        ret = toylog_write(output, priority, fmt, arg_list);
    }
    pthread_mutex_unlock(& log -> body_mutex);

    return ret;
}


