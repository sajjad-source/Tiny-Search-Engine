LIBS = ../common/common.a ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make

.PHONY: clean valgrind

all: indexer indextest

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

indexer.o: ../libcs50/file.h ../libcs50/webpage.h ../common/word.h ../common/pagedir.h ../common/index.h

indextest.o: ../common/index.h ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/file.h

test:
	./testing.sh > testing.out 2>&1

clean:
	rm -f *~ *.o *.dSYM
	rm -f indexer indextest
	rm -f core