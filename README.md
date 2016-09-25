# minilogc
a C based log library

**Example**

##Use default log file

    #include <stdio.h>
    #include "minilog.h"
    
    int main(int argc, char * argv[])
    {
        int x = 0;
        const char * xx = "hello world !";
        miniLog("%d : %s", x, xx);
    
        return 0;
    }
    
when you run test, it will create a file "test.log" in the same dir of "**test**", and write log in the log file.

    minilog $ ./test 
    [5575] [5728] [log] 0 : hello world !
    minilog $ ls
    Makefile  minilog.c  minilog.h  minilog.o  test  test.c  test.log

##Use your log file

    #include <stdio.h>
    #include "minilog.h"
    
    int main(int argc, char * argv[])
    {
        miniLog_init("hello.log");
        int x = 0;
        const char * xx = "hello world !";
        miniLog("%d : %s", x, xx);
    
        return 0;
    }

And this time, there is a log file named "hello.log" in **the current dir**.

    minilog $ ./test 
    [5575] [5783] [log] 0 : hello world !
    minilog $ ls
    hello.log  Makefile  minilog.c  minilog.h  minilog.o  test  test.c  test.o
