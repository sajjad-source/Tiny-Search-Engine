LIBS = ../common/common.a ../libcs50/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../libcs50 -I../common
MAKE = make

.PHONY: clean valgrind test all

all: querier fuzzquery

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

querier.o: ../libcs50/file.h ../libcs50/webpage.h ../common/word.h ../common/pagedir.h ../common/index.h ../libcs50/counters.h
fuzzquery.o: ../common/index.h

test:
	./testing.sh > testing.out 2>&1

valgrind: querier
	valgrind ./querier

clean:
	rm -f *~ *.o *.dSYM
	rm -f querier fuzzquery
	rm -f core
	rm -f testing.out