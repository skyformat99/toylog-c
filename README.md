# minilogc
a C based log library

**Example**

    #include <stdio.h>
    #include "minilog.h"
    
    int main(int argc, char * argv[])
    {
        int x = 0;
        const char * xx = "hello world !";
        miniLog("%d : %s", x, xx);
    
        return 0;
    }
    
when you run test, it will create a file "test.log" in the same dir, and write log in the log file.
    
