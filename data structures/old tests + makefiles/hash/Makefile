CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC

all: $(DIR_FILE)/main.out

$(DIR_FILE)/main.out: $(DIR_FILE)/main.o $(DIR_BIN)/libhash.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lhash -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/main.o: $(DIR_FILE)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/hash.o: $(DIR_SRC)/hash.c $(DIR_HEAD)/hash.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libhash.so: $(DIR_SRC)/hash.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_FILE)/main.o
	rm $(DIR_SRC)/hash.o
	rm $(DIR_BIN)/libhash.so

