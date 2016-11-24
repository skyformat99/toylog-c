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

const char *_g_pri_fatal  = "FATAL";
const char *_g_pri_alert  = "ALERT"; 
const char *_g_pri_crit   = "CRIT";
const char *_g_pri_error  = "ERROR"; 
const char *_g_pri_warn   = "WARN";
const char *_g_pri_notice = "NOTICE";
const char *_g_pri_info   = "INFO";
const char *_g_pri_debug  = "DEBUG"; 
const char *_g_pri_race   = "RACE";
const char *_g_pri_notset = "NOTSET";
const char *_g_pri_unknown= "UNKNOWN";
const char *_g_null = "";

const char * toylog_priority_str(int priority) {
    const char * pri = _g_null;
    switch (priority) {
        case TOYLOG_PRIORITY_FATAL :
            pri = _g_pri_fatal;
            break;
        case TOYLOG_PRIORITY_ALERT :
            pri = _g_pri_alert;
            break;
        case TOYLOG_PRIORITY_CRIT    :
            pri = _g_pri_crit;
            break;
        case TOYLOG_PRIORITY_ERROR   :
            pri = _g_pri_error;
            break;
        case TOYLOG_PRIORITY_WARN    :
            pri = _g_pri_warn;
            break;
        case TOYLOG_PRIORITY_NOTICE  :
            pri = _g_pri_notice;
            break;
        case TOYLOG_PRIORITY_INFO    :
            pri = _g_pri_info;
            break;
        case TOYLOG_PRIORITY_DEBUG   :
            pri = _g_pri_debug;
            break;
        case TOYLOG_PRIORITY_RACE    :
            pri = _g_pri_race;
            break;
        case TOYLOG_PRIORITY_NOTSET  :
            pri = _g_pri_notset;
            break;
        case TOYLOG_PRIORITY_UNKNOWN :
            pri = _g_pri_unknown;
            break;
        default :
            pri = _g_null;
            break;
    }

    return pri;
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

const char * _g_month[] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
    ""
};

const char * month_to_simple(int mon) {
    if(mon < 0 || mon > 11) {
        mon = 12;
    }

    return _g_month[mon];
}

const char * _g_week_full[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    ""
};

const char * _g_week_simple[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    ""
};

const char * week_to_simple(int week) {
    if(week < 0 || week > 6) {
        week = 7;
    }

    return _g_week_simple[week];
}

const char * week_to_full(int week) {
    if(week < 0 || week > 6) {
        week = 7;
    }

    return _g_week_full[week];
}

