/*
 * =====================================================================================
 *
 *       Filename:  toytypes.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2016 06:56:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <strings.h>
#include "toydebug.h"
#include "toytypes.h"

int toylog_convert_priority(const char * priority) {
    if(strcasecmp(priority, "FATAL") == 0) {
        return TOYLOG_PRIORITY_FATAL;
    }
    if(strcasecmp(priority, "ALERT") == 0) {
        return TOYLOG_PRIORITY_ALERT;
    }
    if(strcasecmp(priority, "CRIT") == 0) {
        return TOYLOG_PRIORITY_CRIT;
    }
    if(strcasecmp(priority, "ERROR") == 0) {
        return TOYLOG_PRIORITY_ERROR;
    }
    if(strcasecmp(priority, "WARN") == 0) {
        return TOYLOG_PRIORITY_WARN;
    }
    if(strcasecmp(priority, "NOTICE") == 0) {
        return TOYLOG_PRIORITY_NOTICE;
    }
    if(strcasecmp(priority, "INFO") == 0) {
        return TOYLOG_PRIORITY_INFO;
    }
    if(strcasecmp(priority, "DEBUG") == 0) {
        return TOYLOG_PRIORITY_DEBUG;
    }
    if(strcasecmp(priority, "RACE") == 0) {
        return TOYLOG_PRIORITY_RACE;
    }
    if(strcasecmp(priority, "NOTSET") == 0) {
        return TOYLOG_PRIORITY_NOTSET;
    }
    if(strcasecmp(priority, "UNKNOWN") == 0) {
        return TOYLOG_PRIORITY_UNKNOWN;
    }

    return -1;
}

void show_logoutput(LogOutput * log) {
    if(NULL == log) {
        return ;
    }
    TOYDBG("==============================");
    TOYDBG("type     = [%d]", log -> log_type);
    TOYDBG("file     = [%s]", log -> log_file);
    TOYDBG("engine   = [%s]", log -> engine);
    TOYDBG("protocol = [%s]", log -> protocol);
    TOYDBG("layout   = [%s]", log -> layout);
    TOYDBG("priority = [%d]", log -> priority);
    TOYDBG("color    = [%d]", log -> color);
    TOYDBG("formatted_layout :");
    int i = 0;
    for(i = 0; NULL != log -> formatted_layout && 0 != log -> formatted_layout[i].layout_type; i++) {
        TOYDBG("\ttype : [%03x], msg  : [%s]", 
                log -> formatted_layout[i].layout_type, 
                NULL == log -> formatted_layout[i].msg ? "" : log -> formatted_layout[i].msg);
    }
    TOYDBG("==============================");
}

