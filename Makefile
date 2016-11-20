.PHONY : dummy

CC	= gcc -g
CXX	= g++ -g 
MAKE= $(CC)
CFLAGS	= -Wall -I./ 
LDFLAGS	= 
ALL_FILE= src

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
	rm -rf lib
	make -C src clean
	make -C test clean

tags : dummy
	@ctags -R --fields=+iaS --extra=+q
	@echo -e "\033[0;32mctags excute ok\033[0m"

inc : dummy

##############################
## task 
##############################

test : all dummy
	make -C test
	cd test; ./test

src : dummy
	make -C src
	mkdir -p lib; cp src/lib* lib/

