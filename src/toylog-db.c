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
#include <string.h>
#include "toydebug.h"
#include "toytypes.h"
#include "toylog-db.h"

int toylog_write_to_db(LogOutput * output, int priority, const char * fmt, va_list arg_list) {
    if(NULL == output) {
        TOYDBG("output is NULL");
        return -1;
    }
    LogDB * db = (LogDB *)(output -> out);
    if(NULL == db) {
        TOYDBG("output -> out is NULL");
        return -1;
    }
    switch(db -> db_engine_type) {
        case ENGINE_SQLITE :  
            {
                toylog_write_sqlite(output);
            }
            break;
        case ENGINE_MYSQL : 
            {
                toylog_write_mysql(output);
            }
            break;
        default :
            break;
    }

    return 0;
}

int toylog_open_db(LogOutput * output) {
    if(NULL == output) {
        TOYDBG("output is NULL");
        return -1;
    }
    if(NULL == output -> engine) {
        TOYDBG("engine name is NULL");
        return -1;
    }
    int ret = 0;
    if(strcasecmp("sqlite", output -> engine) == 0) {
        ret = toylog_open_sqlite(output);
    } else if (strcasecmp("mysql", output -> engine) == 0) {
        ret = toylog_open_mysql(output);
    } else {
        TOYDBG("can not support this database engine : [%s]", output -> engine);
        ret = -1;
    }

    return ret;
}

int toylog_close_db(LogOutput * output) {

    return 0;
}

/* *************************************
 * function for sqlite
 * date : 2017-01-03
 * ************************************/

int toylog_open_sqlite(LogOutput * output) {
    if(NULL == output) {
        TOYDBG("output is NULL");
        return -1;
    }
    if(NULL == output -> log_file) {
        TOYDBG("output -> log_file is NULL");
        return -1;
    }
    void * conn = open_sqlite(output -> log_file);
    if(NULL == conn) {
        TOYDBG("open sqlite database file faild : [%s]", output -> log_file);
        return -1;
    }
    LogDB * db = (LogDB *)malloc(sizeof(LogDB));
    if(NULL == db) {
        TOYDBG("malloc faild when open sqlite DB");
        return -1;
    }
    db -> db_engine_type = ENGINE_SQLITE;
    db -> conn = conn;

    output -> out = db;

    return 0;
}

int toylog_close_sqlite(LogOutput * output) {
    return 0;
}

int toylog_write_sqlite(LogOutput * output) {
    return 0;
}

/* **************************************
 * function for mysql
 * date : 2017-01-03
 * *************************************/
int toylog_open_mysql(LogOutput * output) {
    return 0;
}

int toylog_close_mysql(LogOutput * output) {
    return 0;
}

int toylog_write_mysql(LogOutput * output) {
    return 0;
}

