CC=gcc
CFLAGS=-Wall
SRC_DIR=src

all: catcher sender ex1

catcher: $(SRC_DIR)/catcher.c
	$(CC) $(CFLAGS) $(SRC_DIR)/catcher.c -o catcher

sender: $(SRC_DIR)/sender.c
	$(CC) $(CFLAGS) $(SRC_DIR)/sender.c -o sender

ex1: $(SRC_DIR)/ex1.c
	$(CC) $(CFLAGS) $(SRC_DIR)/ex1.c -o ex1

clean:
	rm -f catcher sender ex1
