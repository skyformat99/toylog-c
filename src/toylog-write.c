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

int toylog_write(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        return -1;
    }
    
}


