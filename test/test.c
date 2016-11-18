/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2016 09:13:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include "toylog-c.h"

int main(int argc, char * argv[])
{
    miniLog("compile time : %s %s", __TIME__, __DATE__);
    int x = 0;
    const char * xx = "hello world !";
    miniLog("%d : %s", x, xx);

    return 0;
}

