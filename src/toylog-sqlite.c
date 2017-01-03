/*
 * =====================================================================================
 *
 *       Filename:  toylog-sqlite.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年01月03日 12时18分46秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "toytypes.h"
#include "toydebug.h"
#include "toylog-sqlite.h"
#include "sqlite3.h"

void * open_sqlite(const char * filename) {
    if(NULL == filename) {
        TOYDBG("sqlite database filename is NULL");
        return NULL;
    }
    sqlite3 *conn = NULL;
    char *err_msg = NULL;
    if (SQLITE_OK != sqlite3_open(filename, &conn)) {
        TOYDBG("sqlite3_open DB file faild : [%s]", filename);
        return NULL;
    }

    return conn;
}

