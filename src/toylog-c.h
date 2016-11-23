#ifndef __LOG_H__
#define __LOG_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "toytypes.h"
#include "toyparser.h"
#include "toydebug.h"

#define toylog_fatal   (fmt, arg...) \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_FATAL, fmt, ##arg);\
    }while(0)

#define toylog_alert   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_ALERT, fmt, ##arg);\
    }while(0)

#define toylog_crit    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_CRIT, fmt, ##arg);\
    }while(0)

#define toylog_error   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_ERROR, fmt, ##arg);\
    }while(0)

#define toylog_warn    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_WARN, fmt, ##arg);\
    }while(0)

#define toylog_notice  (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_NOTICE, fmt, ##arg);\
    }while(0)

#define toylog_info    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_INFO, fmt, ##arg);\
    }while(0)

#define toylog_debug   (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_DEBUG, fmt, ##arg);\
    }while(0)

#define toylog_race    (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_RACE, fmt, ##arg);\
    }while(0)

#define toylog_notset  (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_NOTSET, fmt, ##arg);\
    }while(0)

#define toylog_unknown (fmt, arg...)  \
    do{\
        toylog_write_log(TOYLOG_PRIORITY_UNKNOWN, fmt, ##arg);\
    }while(0)

const char * toylog_version();
const char ** toylog_help();
int toylog_init(const char * config_file);
int toylog_priority_set(int priority, const char * toyfile);
int toylog_end();
int toylog_write_log(int priority, const char * fmt, ...);

int toylog_priority_filter(int priority);

#endif

