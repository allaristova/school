CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC

all: $(DIR_FILE)/main.out

$(DIR_FILE)/main.out: $(DIR_FILE)/main.o $(DIR_BIN)/libhlist.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lhlist -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/main.o: $(DIR_FILE)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/hlist.o: $(DIR_SRC)/hlist.c $(DIR_HEAD)/hlist.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libhlist.so: $(DIR_SRC)/hlist.o
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm $(DIR_FILE)/*.o
	rm $(DIR_SRC)/*.o
	rm $(DIR_BIN)/*.so

