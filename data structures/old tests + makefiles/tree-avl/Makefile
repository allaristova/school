CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC

all: $(DIR_FILE)/tree_main.out

$(DIR_FILE)/tree_main.out: $(DIR_FILE)/tree_main.o $(DIR_BIN)/libtree.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -ltree -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/tree_main.o: $(DIR_FILE)/tree_main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/tree.o: $(DIR_SRC)/tree.c $(DIR_HEAD)/tree.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/vector.o: $(DIR_SRC)/vector.c $(DIR_HEAD)/vector.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libtree.so: $(DIR_SRC)/tree.o $(DIR_SRC)/vector.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_FILE)/tree_main.o
	rm $(DIR_SRC)/tree.o
	rm $(DIR_BIN)/libtree.so