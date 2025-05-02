CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall

all: $(DIR_FILE)/test_vector.out

$(DIR_FILE)/test_vector.out: $(DIR_FILE)/test_vector.o $(DIR_BIN)/libvector.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lvector -Wl,-rpath,$(DIR_BIN)

$(DIR_FILE)/test_vector.o: $(DIR_FILE)/test_vector.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/vector.o: $(DIR_SRC)/vector.c $(DIR_HEAD)/vector.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libvector.so: $(DIR_SRC)/vector.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_FILE)/test_vector.o
	rm $(DIR_SRC)/vector.o
	rm $(DIR_BIN)/libvector.so