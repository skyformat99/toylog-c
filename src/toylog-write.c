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
#include <stdarg.h>
#include "toydebug.h"
#include "toylog-file.h"
#include "toylog-db.h"
#include "toytypes.h"

int toylog_write_mutex(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    if(priority > output -> priority) {
        return 0;
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
            case LOG_TYPE_DB :
                {
                    // write to DB
                    toylog_write_db(output, priority, fmt, arg_list);
                }
                break;
            default :
                break;
        }
    pthread_mutex_unlock(& output -> file_mutex);

    return ret;
}

