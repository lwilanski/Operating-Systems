CC=gcc
CFLAGS=-Wall -g

SRC_DIR=src

EXECUTABLE1=reverse
EXECUTABLE2=reverse1024
EXECUTABLE3=list_files

all: $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3)

$(EXECUTABLE1): $(SRC_DIR)/reverse.c
	$(CC) $(CFLAGS) -o $(EXECUTABLE1) $<

$(EXECUTABLE2): $(SRC_DIR)/reverse1024.c
	$(CC) $(CFLAGS) -o $(EXECUTABLE2) $<

$(EXECUTABLE3): $(SRC_DIR)/list_files.c
	$(CC) $(CFLAGS) -o $(EXECUTABLE3) $<

clean:
	rm -f $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3)

test:
	@echo "Test: Odwracanie pliku po 1 bajcie"
	./$(EXECUTABLE1) test.txt r_test.txt
	@echo "Odwrócona zawartość test.txt, zapisano w pliku r_test.txt"
	@echo "Test: Odwracanie pliku blokami po 1024 bajty"
	./$(EXECUTABLE2) test.txt r1024_test.txt
	@echo "Odwrócona zawartość test.txt, zapisano w pliku r1024_test.txt"
	@echo "Test: Wylistowanie i obliczenie rozmiaru wszystkich plików w katalogu bieżącym"
	./$(EXECUTABLE3) .
