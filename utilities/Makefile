
CC=g++

CPPFLAGS+=-I./include
CPPFLAGS+=-O3

all: libStackTimer.so test_stack_timer


libStackTimer.so: stack_timer.o
	$(CC) $^ -shared -o $@

stack_timer.o: stack_timer.cpp
	$(CC) -fPIC $(CPPFLAGS) -c $^


test_stack_timer: test_stack_timer.c libStackTimer.so
	gcc $^ -lStackTimer $(CPPFLAGS) -L. -o $@


clean:
	rm -f *.o stack_timer test_stack_timer
