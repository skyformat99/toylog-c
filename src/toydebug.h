#ifndef _TOYDEBUG_H_
#define _TOYDEBUG_H_

#ifdef TOYDEBUG
#define TOYDBG(fmt, arg...) \
    do{\
        printf("%s %3d : ", __FILE__, __LINE__);\
        printf(fmt, ##arg);\
        printf("\n");\
    }while(0)

#else
#define TOYDBG(fmt, arg...) 
#endif

#endif /* end file */

