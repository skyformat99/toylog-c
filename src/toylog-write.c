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
#include "toylog-console.h"
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
                toylog_write_to_console(output, priority, fmt, arg_list);
                break;
            case LOG_TYPE_FILE :
                {
                    //TOYDBG("write file");
                    toylog_write_to_file(output, priority, fmt, arg_list);
                }
                break;
#if ENABLE_DB
            case LOG_TYPE_DB :
                {
                    // write to DB
                    toylog_write_to_db(output, priority, fmt, arg_list);
                }
                break;
#endif
            default :
                break;
        }
    pthread_mutex_unlock(& output -> file_mutex);

    return ret;
}

