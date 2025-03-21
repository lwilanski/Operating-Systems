CC = gcc
CFLAGS = -Wall -lrt -lpthread
OBJS = odb1 wys1

all: $(OBJS)

odb1: odb1.c
	$(CC) $(CFLAGS) -o odb1 odb1.c

wys1: wys1.c
	$(CC) $(CFLAGS) -o wys1 wys1.c

clean:
	rm -f $(OBJS)

run_odb1: odb1
	./odb1

run_wys1: wys1
	./wys1
