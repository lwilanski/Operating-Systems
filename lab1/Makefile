CXX = gcc
CFLAGS = -Wall -std=c99 -g

.PHONY: all countdown clean

all: countdown

countdown: countdown.o
	$(CXX) $(CFLAGS) countdown.o -o countdown

countdown.o: src/countdown.c
	$(CXX) $(CFLAGS) -c src/countdown.c -o countdown.o

clean:
	rm -f countdown countdown.o
