/*
 * o
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
    free_mem(line_list);

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

char *toy_strndup(const char *s, size_t n) {
    char * p = strndup(s, n);
    //TOYDBG("malloc memory : [%08X]", p);

    return p;
}

void *malloc_mem(size_t size) {
    void * p = malloc(size);
    //TOYDBG("malloc memory : [%08X]", p);
    if(NULL != p) {
        mzero(p, size);
    }

    return p;
}

void free_mem(void * p) {
    if(NULL != p) {
        //TOYDBG("free   memory : [%08X]", p);
        free(p);
    }
}

int get_line_pos(char * const * line_list, const char * line) {
    if(NULL == line_list || NULL == line) {
        return -1;
    }
    int len = strlen(line);
    int i = 0;
    for(i = 0; line_list[i] != NULL; i++) {
        if(strncasecmp(line, line_list[i], len) == 0) {
            return i;
        }
    }

    return -1;
}

int get_value(char ** value, const char * line) {
    if(NULL == value || NULL == line) {
        return -1;
    }

    const char * peq = strchr(line, '=');
    if(NULL == peq) {
        return -1;
    }
    peq += 1;
    int len = strlen(peq);
    TOYDBG("value : [%s]", peq);
    int pos = get_first_not_of(peq, len, "\t ");
    TOYDBG("first pos is : [%d]", pos);
    if (pos < 0) {
        return -1;
    }
    *value = toy_strndup(peq + pos, len - pos);

    return 0;
}

int get_value_by_key(char ** value, const char * key, char * const * line_list) {
    if(NULL == value || NULL == key || NULL == line_list) {
        return -1;
    }
    int file_pos = get_line_pos(line_list, key);
    if(file_pos < 0) {
        return -1;
    }

    return get_value(value, line_list[file_pos]);
}

int str_split(char *** list, const char * line, const char * ptn, int ignore) {
    if(NULL == list || NULL == line || NULL == ptn) {
        return -1;
    }
    int count = get_char_count(line, strlen(line), ptn);
    TOYDBG("get [%d] split char", count);
    *list = malloc_mem(sizeof(char *) * (count + 1));
    if(NULL == *list) {
        return -1;
    }
    char **plist = *list;

    count = 0;
    int sublen = strlen(ptn);
    const char * pos = line;
    int len = 0;
    int i = 0;
    for(i = 0; line[i] != '\0'; i++) {
        int x = 0;
        int found = 0;
        for(x = 0; x < sublen; x++ ) {
            if(line[i] == ptn[x]) {
                found = 1;
                break;
            }
        }
        len ++;
        if(0 == found) {
            continue;
        }

        //found
        if(1 == len && ignore) {
            //TOYDBG("ignore : [%s]", pos);
            pos += len;
            len = 0;
            //TOYDBG("next : [%s]", pos);
            continue;
        }
        plist[count++] = toy_strndup(pos, len - 1);
        pos += len;
        len = 0;
    }
    //found
    if(!(0 == len && ignore)) {
        plist[count++] = toy_strndup(pos, len);
    }

    return 0;
}

int get_toyfile_list(char *** file_list, char * const * line_list) {
    if(NULL == line_list) {
        return -1;
    }
    char *value = NULL;
    if(get_value_by_key(&value, "toyfile", line_list) < 0) {
        TOYDBG("get value of [toyfile] faild");
        return -1;
    }
    TOYDBG("key   : [%s]", "toyfile");
    TOYDBG("value : [%s]\n", value);
    if(str_split(file_list, value, ", \t", 1) < 0) {
        TOYDBG("split string faild");
    }

    free_mem(value);

    return 0;
}

LogOutput * get_logoutput(char * const* line_list, const char * toyfile) {
    if(NULL == line_list || NULL == toyfile) {
        return NULL;
    }

    int  _log_type = 0;
    char _log_file[MAX_FILE_NAME_LEN] = {0};
    char _log_layout[MAX_FILE_NAME_LEN] = {0};
    int  _log_priority = TOYLOG_PRIORITY_UNKNOWN;
    int  _log_color = 0;

    char font[128] = {0};
    snprintf(font, sizeof(font) - 1, "%s.", toyfile);
    int len = strlen(font);
    int i = 0;
    char *value = NULL;
    for (i = 0; line_list[i] != NULL; i++) {
        const char * pline = line_list[i];
        if (strncasecmp(pline, font, len) != 0) {
            continue;
        }
        if(NULL != value) {
            free_mem(value);
            value = NULL;
        }

        if(strncasecmp(pline + len, "type", strlen("type")) == 0) {
            if(get_value(&value, pline) < 0) {
                TOYDBG("get value of [type] faild");
            } else if (strcasecmp(value, "file") == 0){
                _log_type = LOG_TYPE_FILE;
            } else if (strcasecmp(value, "console") == 0) {
                _log_type = LOG_TYPE_CONCLE;
            }
            continue;
        }
        if(strncasecmp(pline + len, "file", strlen("file")) == 0) {
            if(get_value(&value, pline) < 0 || '\0' == value[0]) {
                TOYDBG("get value of [%s] faild", "file");
            } else {
                snprintf(_log_file, sizeof(_log_file) - 1, value);
            }
            continue;
        }
        if(strncasecmp(pline + len, "layout", strlen("layout")) == 0) {
            if(get_value(&value, pline) < 0 || '\0' == value[0]) {
                TOYDBG("get value of [%s] faild", "layout");
            } else {
                snprintf(_log_layout, sizeof(_log_layout) - 1, "%s", value);
            }
            continue;
        }
        if(strncasecmp(pline + len, "priority", strlen("priority")) == 0) {
            if(get_value(&value, pline) < 0) {
                TOYDBG("get value of [%s] faild", "priority");
            } else {
                _log_priority = toylog_convert_priority(value);
                _log_priority = -1 == _log_priority ? TOYLOG_PRIORITY_UNKNOWN : _log_priority;
            }
            continue;
        }
        if(strncasecmp(pline + len, "color", strlen("color")) == 0) {
            if(get_value(&value, pline) < 0) {
                TOYDBG("get value of [%s] faild", "color");
            } else {
                _log_color = strcasecmp(value, "true") == 0 ? 1 : 0;
            }
            continue;
        }
    }
    LogOutput * plog = (LogOutput *)malloc_mem(sizeof(LogOutput));
    if(NULL == plog) {
        return NULL;
    }
    plog -> log_type = _log_type;
    plog -> log_file = strdup(_log_file);
    plog -> layout   = strdup(_log_layout);
    plog -> priority = _log_priority;
    plog -> color    = _log_color;

    return plog;
}

int parse_lines(LogBody * log, char * const* line_list) {
    if(NULL == log || NULL == line_list) {
        return -1;
    }

    //XXX for debug, show lines
    int i = 0; 
    for(i = 0; line_list[i] != NULL; i++) {
        printf("[%s]\n", line_list[i]);
    }

    char **file_list = NULL;
    if(get_toyfile_list(&file_list, line_list) < 0) {
        TOYDBG("get toyfile(s) faild");
        return -1;
    }

    int count = 0;
    for(i = 0; file_list[i] != NULL; i++) {
        TOYDBG("file : [%s]", file_list[i]);
    }
    count = i;
    int pos = 0;
    LogOutput ** log_list = (LogOutput **)malloc_mem(sizeof(LogOutput *) * (count + 1));
    if(NULL == log_list) {
        TOYDBG("get memory faild");
        free_line_list(&file_list);
        return -1;
    }
    for (i = 0; i < count; i++) {
        LogOutput * pout = get_logoutput(line_list, file_list[i]);
        if(NULL == pout) {
            TOYDBG("get logoutput of toyfile [%s] faild", file_list[i]);
            continue;
        }
        log_list[pos++] = pout;
        show_logoutput(pout);
    }
    free_line_list(&file_list);
    log -> output_list = log_list;

    return 0;
}

int get_char_count(const char * mem, int len, const char * ptn) {
    int count = 0;
    int sublen = strlen(ptn);
    int i = 0; 
    for(i = 0; i < len; i++) {
        int x = 0;
        int found = 0;
        for(x = 0; x < sublen; x++) {
            if(ptn[x] == mem[i]) {
                found = 1;
                break;
            }
        }
        if(found) {
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

int get_first_not_of(const char * p, int len, const char * sub) {
    if(NULL == p) {
        return -1;
    }
    int sublen = strlen(sub);
    int i = 0;
    for(i = 0; i < len; i++) {
        int found = 0;
        int x = 0;
        for(x = 0; x < sublen; x++) {
            if(sub[x] == p[i]) {
                found = 1;
                break;
            }
        }
        if(0 == found) {
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
        int found = 0;
        for(i = 0; i < sublen; i++) {
            if(sub[i] == p[x]) {
                found = 1;
                break;
            }
        }
        if(0 == found) {
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
    char * fmt = toy_strndup(npos, nlen);

    return fmt;
}

int split_memory(char *** line_list, const char *mem) {
    if(NULL == mem || NULL == line_list) {
        return -1;
    }
#if 1
    int len = 0;
    int i = 0;
    len = get_char_count(mem, strlen(mem), "\n");
    char ** list = (char **)malloc_mem(sizeof(char *) * (len + 1));
    if(NULL == list) {
        return -1;
    }

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
#else
    if(str_split(line_list, mem, "\n", 1) < 0) {
        TOYDBG("split memory faild");
        return -1;
    }
    char ** plist = *line_list;
    int i = 0;
    for(i = 0; plist[i] != NULL; i++) {
        char * p = plist[i];
        plist[i] = get_formatted_string(p, strlen(p));
        free_mem(p);
    }
#endif
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
    *line_list = (char *)malloc_mem(sizeof(char) * (len + 1));
    if(NULL == *line_list) {
        fclose(fp);
        return -1;
    }
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
        free_mem(list[i]);
        list[i] = NULL;
    }
    free_mem(list);
    *line_list = NULL;
}

