CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

LIB = common.a

SRCS = pagedir.c
OBJS = $(SRCS:.c=.o)

$(LIB): $(OBJS)
	ar cr $@ $^
pagedir.o: pagedir.h

.PHONY: clean

clean:
	rm -f *~ *.o
	rm -f $(LIB)
