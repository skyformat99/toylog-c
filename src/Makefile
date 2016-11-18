.PHONY : dummy

CC	= gcc -g
CXX	= g++ -g 
MAKE= $(CC)
CFLAGS	= -Wall -I./
LDFLAGS	= 
ALL_FILE= libtoylog.so libtoylog.a

%.i : %.c
	$(MAKE) -E -o $@ $< $(CFLAGS)

%.i : %.cpp
	$(MAKE) -E -o $@ $< $(CFLAGS)

%.o : %.c
	$(MAKE) -c -o $@ $< $(CFLAGS)

%.o : %.cpp
	$(MAKE) -c -o $@ $< $(CFLAGS)

%.os: %.c
	$(MAKE) -fPIC -c $< -o $@ $(CFLAGS) 

%.os: %.cpp
	$(MAKE) -fPIC -c $< -o $@ $(CFLAGS) 

all : $(ALL_FILE)

clean : dummy
	rm -rf *.o *.os $(ALL_FILE)

tags : dummy
	@ctags -R --fields=+iaS --extra=+q
	@echo -e "\033[0;32mctags excute ok\033[0m"

inc : dummy

##############################
## task 
##############################

libtoylog.a : toylog-c.os 
	ar cr $@ $^

libtoylog.so : toylog-c.os
	$(MAKE) -shared -Wl,-soname,$@ -o $@ $^ -lc $(LDFLAGS) 

