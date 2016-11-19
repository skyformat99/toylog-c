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
#include <string.h>
#include "toylog-c.h"

void usage(const char * exename)
{
    printf("usage : %s [option]\n", NULL == exename ? "" : exename);
    printf("    -file\n");
    printf("    -conf config-file\n");
}

int main(int argc, char * argv[])
{
    usage(argv[0]);

    const char * ver = toylog_version();
    printf("version : [%s]\n", ver);

    const char ** help = toylog_help();
    int i = 0;
    printf("help info : \n");
    for(i = 0; help[i] != NULL; i++)
    {
        printf("%s\n", help[i]);
    }

//    if(argc > 1)
//    {
//        if(strcmp("-file", argv[1]) == 0)
//        {
//            //output log to a file
//            toylog_init_file("testlogfile.log");
//        }
//        else if (strcmp("-conf", argv[1]) == 0 && argc > 2)
//        {
//            //load config file, adn configure toylog
//            toylog_init_config(argv[2]);
//        }
//    }
//
//    toylog_fatal("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_alert("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_crit("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_error("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_warn("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_notice("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_info("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_debug("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_race("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_notset("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
//
//    toylog_unknown("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);

    return 0;
}

