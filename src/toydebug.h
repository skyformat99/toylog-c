#ifndef _TOYDEBUG_H_
#define _TOYDEBUG_H_

#ifdef TOYDEBUG
#define TOYDBG(fmt, arg...) \
    do{\
        printf("%s %d : ", __FILE__, __LINE__);\
        printf(fmt, ##arg);\
        printf("\n");\
    }while(0)
#endif

#endif /* end file */

