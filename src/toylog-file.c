/*
 * =====================================================================================
 *
 *       Filename:  toylog-file.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/24/2016 05:26:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  fengbohello (fengbohello@foxmail.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "toydebug.h"
#include "toylog-file.h"

int get_digit(int num) {
    if(num < 0) {
        num = - num;
    }
    int n = 0;
    while(num != 0) {
        num /= 10;
        n++;
    }

    return n;
}

int get_filename(char *szfilename, int len, LogOutput * output, int index) {
    if(NULL == output || NULL == output -> log_file) {
        return -1;
    }
    char szfmt[MAX_FILE_NAME_LEN] = {0};
    int idigit = get_digit(output -> filecount);
    if(0 == idigit) {
        snprintf(szfmt, sizeof(szfmt) - 1, "%%s.%%d");
    } else {
        snprintf(szfmt, sizeof(szfmt) - 1, "%%s.%%0%dd", idigit);
    }
    snprintf(szfilename, len, szfmt, output -> log_file, index);
    //TOYDBG("Filename is : [%s]", szfilename);

    return 0;
}



