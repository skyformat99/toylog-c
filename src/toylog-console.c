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
#include "toylog-file.h"

int toylog_write_to_console(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    return toylog_write_file(output, priority, fmt, arg_list);
}

