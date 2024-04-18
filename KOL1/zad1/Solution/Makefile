CC=gcc
CFLAGS=-Wall -fPIC
LDFLAGS=-ldl

all: libbibl1.so zad1

libbibl1.so: bibl1.o
	$(CC) -shared -o libbibl1.so bibl1.o

bibl1.o: bibl1.c bibl1.h
	$(CC) $(CFLAGS) -c bibl1.c

zad1: zad1.c
	$(CC) $(CFLAGS) zad1.c -o zad1 $(LDFLAGS)

clean:
	rm -f *.o *.so zad1
