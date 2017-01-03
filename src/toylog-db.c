/*
 * =====================================================================================
 *
 *       Filename:  toylog-db.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/2017 08:01:07 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), fengbohello@foxmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdarg.h>
#include "toydebug.h"
#include "toytypes.h"
#include "toylog-db.h"

int toylog_write_to_db(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output || NULL == output -> out) {
        TOYDBG("output or out is NULL");
        return -1;
    }

    return 0;
}

