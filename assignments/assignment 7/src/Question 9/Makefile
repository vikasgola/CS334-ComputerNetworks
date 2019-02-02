CC=gcc
CFLAGS=-Wall -Wextra

OBJS=server.o net.o file.o mime.o

all: server

server: $(OBJS)
	gcc -o $@ $^

net.o: net.c net.h

server.o: server.c net.h

file.o: file.c file.h

mime.o: mime.c mime.h

clean:
	rm -f $(OBJS)
	rm -f server
	rm -f cache_tests/cache_tests
	rm -f cache_tests/cache_tests.exe
	rm -f cache_tests/cache_tests.log

TEST_SRC=$(wildcard cache_tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))


test:
	tests

tests: clean $(TESTS)
	sh ./cache_tests/runtests.sh

.PHONY: all, clean, tests
