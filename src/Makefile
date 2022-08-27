CC = gcc
CFLAGS = -c -Wall
EXTRACFLAGS = 

CCPP = g++
CPPFLAGS = -c -Wall


CSOURCES = main.c oxygen_cell.c
CPPSOURCES = 
INCLUDE_DIRS =

COBJECTS = $(CSOURCES: .c=.o)
CPPOBJECTS = $(CPPSOURCES: .cpp=.o)


%.o: %.c
	$(CC) $(CFLAGS) $^

%.o: %.cpp
	$(CCPP) $(CPPFLAGS) $^

main.out: $(COBJECTS)
	$(CC) $(EXTRACFLAGS) -o $@ $^


.PHONY: all
all: main.out

.PHONY: debug_log
debug_log: EXTRACFLAGS += -DDEBUG_LOG
debug_log: main.out

.PHONY: clean
clean:
	rm -f *.o *.out
