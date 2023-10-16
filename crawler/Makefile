OBJS = crawler.o ../common/pagedir.o 
LIBS = ../libcs50/libcs50.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: ../libcs50/webpage.h ../libcs50/bag.h ../libcs50/hashtable.h ../common/pagedir.h
../common/pagedir.o: ../common/pagedir.h

.PHONY: test valgrind clean

test:
	./testing.sh > testing.out 2>&1

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f crawler
	rm -f core