/*
 * =====================================================================================
 *
 *       Filename:  toybitmap.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/2016 06:36:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (fengbohello@foxmail.com), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "toybitmap.h"

static void *mzero(void *s, int n)
{
    if(NULL == s || n <= 0)
    {
        return s;
    }

    return memset(s, 0, n);
}

int init_map_size(ToyBitmap * map, int size)
{
    if(NULL == map || size <= 0)
    {
        return -1;
    }
    map -> map = NULL;
    map -> map_len = -1;

    char * p = (char *)malloc(sizeof(char) * size);
    if(NULL == p)
    {
        return -1;
    }
    mzero(p, sizeof(char) * size);
    map -> map = p;
    map -> map_len = size;

    return 0;
}

int set_map_value(ToyBitmap * map, int pos, char value)
{
    if( NULL == map || 
        NULL == map -> map || 
        pos < 0 || 
        pos >= map -> map_len)
    {
        return -1;
    }

    map -> map[pos] = value;

    return 0;
}

char get_map_value(ToyBitmap * map, int pos)
{
    if( NULL == map ||
        NULL == map -> map ||
        pos < 0 ||
        pos >= map -> map_len)
    {
        return -1;
    }

    return map -> map[pos];
}

