CC = gcc

CFLAGS = -Wall -pthread

SRC_DIR = src

SRC = $(SRC_DIR)/santa.c

TARGET = santa

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

