CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

LIB = common.a

SRCS = pagedir.c word.c index.c
OBJS = $(SRCS:.c=.o)

$(LIB): $(OBJS)
	ar cr $@ $^

pagedir.o: pagedir.h
word.o: word.h
index.o: index.h

clean:
	rm -f *~ *.o
	rm -f $(LIB)
