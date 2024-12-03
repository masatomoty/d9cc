CFLAGS=-std=c11 -g -static

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
CC=gcc

9cc: $(OBJS)
		$(CC) -o 9cc $(OBJS) $(LDFLAGS)
$(OBJS): 9cc.h

test: 9cc
		./test.sh

clean:
		rm -f 9cc *.o *~ tmp*

.PHONY: test clean
