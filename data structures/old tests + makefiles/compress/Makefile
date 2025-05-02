CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC

all: $(DIR_FILE)/compressor.out 

$(DIR_FILE)/compressor.out: $(DIR_FILE)/compressor.o $(DIR_BIN)/libcompress.so 
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lcompress -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/compressor.o: $(DIR_FILE)/compressor.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/compress.o: $(DIR_SRC)/compress.c $(DIR_HEAD)/compress.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libcompress.so: $(DIR_SRC)/compress.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_FILE)/compressor.o
	rm $(DIR_SRC)/compress.o
	rm $(DIR_BIN)/libcompress.so
	rm $(DIR_FILE)/output.txt