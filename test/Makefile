.PHONY : dummy

CC	= gcc -g
CXX	= g++ -g 
MAKE= $(CC)
CFLAGS	= -Wall -I../src/
LDFLAGS	= ../lib/libtoylog.a #-L../lib/ -ltoylog
ALL_FILE= test

%.i : %.c
	$(MAKE) -E -o $@ $< $(CFLAGS)

%.i : %.cpp
	$(MAKE) -E -o $@ $< $(CFLAGS)

%.o : %.c
	$(MAKE) -c -o $@ $< $(CFLAGS)

%.o : %.cpp
	$(MAKE) -c -o $@ $< $(CFLAGS)

all : $(ALL_FILE)

clean : dummy
	rm -rf *.o *.log $(ALL_FILE)

tags : dummy
	@ctags -R --fields=+iaS --extra=+q
	@echo -e "\033[0;32mctags excute ok\033[0m"

inc : dummy

##############################
## task 
##############################

test : test.o 
	$(MAKE) -o $@ $^ $(LDFLAGS)

