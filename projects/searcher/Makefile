CC = gcc
DIR_SRC = ./lib/src
DIR_BIN = ./lib/bin

DIR_HEAD = ./lib/headers
CFLAGS = -Wall -fPIC

all: ./indexer/indexer1.out ./indexer/indexer2.out ./search/search.out

./indexer/indexer1.out: ./indexer/indexer1.o
	$(CC) $(CFLAGS) -o $@ $< 

./indexer/indexer2.out: ./indexer/indexer2.o $(DIR_BIN)/libmap.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lmap -Wl,-rpath,$(DIR_BIN)

./indexer/indexer1.o: ./indexer/indexer1.c
	$(CC) $(CFLAGS) -c $< -o $@

./indexer/indexer2.o: ./indexer/indexer2.c
	$(CC) $(CFLAGS) -c $< -o $@

./search/search.out: ./search/search.o $(DIR_BIN)/libmap.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -lmap -Wl,-rpath,$(DIR_BIN)

./search/search.o: ./search/search.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/map.o: $(DIR_SRC)/map.c $(DIR_HEAD)/map.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/libmap.so: $(DIR_SRC)/map.o
	$(CC) $(CFLAGS) -shared -o $@ $^
clean:
	rm $(DIR_SRC)/*.o
	rm $(DIR_BIN)/*.so
	rm ./search/*.o
	rm ./indexer/*.o
	rm ./search/*.out
	rm ./indexer/*.out