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
#include "toybitmap.h"

#define TEST_EQ(a, b) \
    do{\
        if(a == b) {\
            printf("\033[0;32m[LINE %5d : EQUE SUCCESS    ]\033[0m\n", __LINE__);\
        } else {\
            printf("\033[0;31m[LINE %5d : EQUE FAILURE    ]\033[0m\n", __LINE__);\
        }\
    }while(0)

#define TEST_NE(a, b) \
    do{\
        if(a != b) {\
            printf("\033[0;32m[LINE %5d : NOT EQUE SUCCESS]\033[0m\n", __LINE__);\
        } else {\
            printf("\033[0;31m[LINE %5d : NOT EQUE FAILURE]\033[0m\n", __LINE__);\
        }\
    }while(0)

void usage(const char * exename)
{
    printf("usage : %s [option]\n", NULL == exename ? "" : exename);
    printf("    -file\n");
    printf("    -conf config-file\n");
}

void test_bitmap()
{
    printf("test ToyBitmap\n");

    int x = init_map_size(NULL, 1);
    TEST_EQ(-1, x);

    ToyBitmap map;
    x = init_map_size(&map, 10);

    x = set_map_value(&map, -1, 123);
    TEST_EQ(-1, x);

    x = set_map_value(&map, 10, 123);
    TEST_EQ(-1, x);

    int i = 0;
    for(i = 0; i < 10; i++)
    {
        x = set_map_value(&map, i, i + 10);
        TEST_EQ(0, x);
    }

    char c = get_map_value(&map, -1);
    TEST_EQ(-1, c);

    c = get_map_value(&map, 10);
    TEST_EQ(-1, c);

    for(i = 0; i < 10; i++)
    {
        c = get_map_value(&map, i);
        TEST_EQ(c, i + 10);
    }

    free_map(&map);
}

void test_info()
{
    const char * ver = toylog_version();
    printf("version : [%s]\n", ver);

    const char ** help = toylog_help();
    int i = 0;
    printf("help info : \n");
    for(i = 0; help[i] != NULL; i++)
    {
        printf("%s\n", help[i]);
    }
}

void test_parser()
{
    const char * file_name = "./config.cfg";
    toylog_init(file_name);

    toylog_fatal("toylog start");

    toylog_fatal    ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_alert    ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_crit     ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_error    ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_warn     ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_notice   ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_info     ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_debug    ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_race     ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_notset   ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);
    toylog_unknown  ("compile time : %s %s, line : %d", __TIME__, __DATE__, __LINE__);

    toylog_end();

    printf("log test OK\n");
}

int main(int argc, char * argv[])
{
    usage(argv[0]);

    test_info();

    test_bitmap();

    test_parser();

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

    return 0;
}

