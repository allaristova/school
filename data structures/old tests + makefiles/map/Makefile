CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC


all: $(DIR_FILE)/main.out

$(DIR_FILE)/main.out: $(DIR_FILE)/main.o $(DIR_BIN)/libmap.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lmap -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/main.o: $(DIR_FILE)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/map.o: $(DIR_SRC)/map.c $(DIR_HEAD)/map.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libmap.so: $(DIR_SRC)/map.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_FILE)/main.o
	rm $(DIR_SRC)/map.o
	rm $(DIR_BIN)/libmap.so