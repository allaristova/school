CC = gcc
DIR_SRC = ../../lib/src
DIR_BIN = ../../lib/bin

DIR_HEAD = ../../lib/headers
DIR_FILE = .
CFLAGS = -Wall -fPIC

all: $(DIR_FILE)/test_list.out


$(DIR_FILE)/test_list.out: $(DIR_FILE)/test_list.o $(DIR_BIN)/liblist.so $(DIR_BIN)/libdeque.so $(DIR_BIN)/libstack.so $(DIR_BIN)/libqueue.so
	$(CC) $(CFLAGS) -o $@ $<  -L$(DIR_BIN) -llist -Wl,-rpath,$(DIR_BIN)


$(DIR_FILE)/test_list.o: $(DIR_FILE)/test_list.c
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/list.o: $(DIR_SRC)/list.c $(DIR_HEAD)/list.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/deque.o: $(DIR_SRC)/deque.c $(DIR_HEAD)/deque.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/stack.o: $(DIR_SRC)/stack.c $(DIR_HEAD)/stack.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_SRC)/queue.o: $(DIR_SRC)/queue.c $(DIR_HEAD)/queue.h
	$(CC) $(CFLAGS) -c $< -o $@

$(DIR_BIN)/liblist.so: $(DIR_SRC)/list.o
	$(CC) $(CFLAGS) -shared -o $@ $^

$(DIR_BIN)/libqueue.so: $(DIR_SRC)/queue.o
	$(CC) $(CFLAGS) -shared -o $@ $^

$(DIR_BIN)/libdeque.so: $(DIR_SRC)/deque.o
	$(CC) $(CFLAGS) -shared -o $@ $^

$(DIR_BIN)/libstack.so: $(DIR_SRC)/stack.o
	$(CC) $(CFLAGS) -shared -o $@ $^

clean:
	rm $(DIR_FILE)/*.o
	rm $(DIR_SRC)/*.o
	rm $(DIR_BIN)/*.so