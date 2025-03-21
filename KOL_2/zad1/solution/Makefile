TARGET = zad1
SRC = zad1.c
CFLAGS = -Wall -pthread

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
