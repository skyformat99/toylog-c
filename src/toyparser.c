/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  parse json data
 *
 *        Version:  1.0
 *        Created:  11/19/2016 04:10:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (fengbohello@foxmail.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "toyparser.h"
#include "toybitmap.h"
#include "toydebug.h"

int parse_file  (LogBody * log, const char * file) {
    if(NULL == log || NULL == file) {
        return -1;
    }
    char * line_list = NULL;
    if(read_file(&line_list, file) < 0) {
        return -1;
    }

    int ret = parse_memory(log, line_list);
    free(line_list);

    return ret;
}

int parse_memory(LogBody * log, const char * mem) {
    char ** line_list = NULL;
    if(split_memory(&line_list, mem) < 0) {
        return -1;
    }

    int ret = parse_lines(log, line_list);
    free_line_list(&line_list);

    return ret;
}

int parse_lines(LogBody * log, char * const* line_list) {
    if(NULL == log || NULL == line_list) {
        return -1;
    }
    int i = 0; 
    for(i = 0; line_list[i] != NULL; i++) {
        printf("[%s]\n", line_list[i]);
    }

    return 0;
}

int get_char_count(const char * mem, int len, char c) {
    int count = 0;
    int i = 0; 
    for(i = 0; i < len; i++) {
        if(c == mem[i]) {
            count++;
        }
    }

    return count;
}

int get_first_of(const char * p, int len, const char * sub) {
    if(NULL == p) {
        return -1;
    }
    int sublen = strlen(sub);
    int i = 0;
    for(i = 0; i < len; i++) {
        int found = 0;
        int x = 0;
        for(x = 0; x <  sublen; x++) {
            if(sub[x] == p[i]) {
                found = 1;
                break;
            }
        }
        if(found) {
            return i;
        }
    }

    return -1;
}

int get_first_not_of_r(const char * p, int len, const char * sub) {
    if(NULL == p || NULL == sub) {
        return -1;
    }
    int x = len - 1;
    int sublen = strlen(sub);
    for(; x >= 0; x--) {
        int i = 0;
        int fount = 0;
        for(i = 0; i < sublen; i++) {
            if(sub[i] == p[x]) {
                fount = 1;
                break;
            }
        }
        if(0 == fount) {
            return x + 1;
        }
    }

    return -1;
}

char *get_formatted_string(const char * pos, int len) {
    //store the length of string and the position
    const char * npos = pos;
    int nlen = len;

    //format string's font
    int j = 0;
    for(j = 0; j < len; j++) {
        if (' ' == npos[j] || '\t' == npos[j]) {
            continue;
        }
        npos += j;
        break;
    }
    nlen -= j;
    if ('\n' == npos[0] || '#' == npos[0] || '\0' == npos[0]) {
        TOYDBG("format font is null");
        return NULL;
    }

    //format string's end
    int x = get_first_of(npos, nlen, "#");
    if(x > 0) {
        nlen = x;
    }
    x = get_first_not_of_r(npos, nlen, "\n\r\t ");
    if(x > 0) {
        nlen = x;
    }

    //copy formatted string to new memory
    TOYDBG("nlen is [%d]", nlen);
    return strndup(npos, nlen);
}

int split_memory(char *** line_list, const char *mem) {
    if(NULL == mem || NULL == line_list) {
        return -1;
    }
    int len = 0;
    int i = 0;
    len = get_char_count(mem, strlen(mem), '\n');
    char ** list = (char **)malloc(sizeof(char *) * (len + 1));
    if(NULL == list) {
        return -1;
    }
    mzero(list, sizeof(char *) * (len + 1));

    int cnt = 0;
    const char * pos = mem + 0;
    len = 0;
    for(i = 0; mem[i] != '\0'; i++) {
        if ('\n' != mem[i]) {
            len ++;
            continue;
        }
        TOYDBG("pos is : [%d]", i);
        char * p = get_formatted_string(pos, len);
        if (NULL != p) {
            list[cnt++] = p;
        } else {
            TOYDBG("p is NULL");
        }

        //reset length ang position
        pos = pos + len + 1;
        len = 0;
    }

    *line_list = list;
    return 0;
}

int read_file(char ** line_list, const char * file_name) {
    if(NULL == line_list || NULL == file_name) {
        return -1;
    }
    if(access(file_name, R_OK) != 0) {
        return -1;
    }
    FILE * fp = fopen(file_name, "r");
    if(NULL == fp) {
        return -1;
    }
    if(fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return -1;
    }
    int len = ftell(fp);
    if(-1 == len) {
        fclose(fp);
        return -1;
    }
    rewind(fp);
    *line_list = (char *)malloc(sizeof(char) * (len + 1));
    if(NULL == *line_list) {
        fclose(fp);
        return -1;
    }
    mzero(*line_list, len + 1);
    fread(*line_list, 1, len, fp);
    fclose(fp);

    return 0;
}

void free_line_list(char *** line_list) {
    if(NULL == line_list || NULL == *line_list) {
        return ;
    }
    char **list = *line_list;
    int i = 0;
    for(i = 0; list[i] != NULL; i++) {
        free(list[i]);
        list[i] = NULL;
    }
    free(list);
    *line_list = NULL;
}
