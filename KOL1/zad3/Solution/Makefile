CC=gcc
CFLAGS=-Wall
TARGET1=zad3_odb
TARGET2=zad3_nad

all: $(TARGET1) $(TARGET2)

$(TARGET1): zad3_odb.c
	$(CC) $(CFLAGS) -o $(TARGET1) zad3_odb.c

$(TARGET2): zad3_nad.c
	$(CC) $(CFLAGS) -o $(TARGET2) zad3_nad.c

clean:
	rm -f $(TARGET1) $(TARGET2) /tmp/potok1
