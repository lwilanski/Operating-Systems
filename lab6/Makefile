CC=gcc
CFLAGS=-Wall -g

SRC_DIR=src
EXECUTABLE1=ex1
EXECUTABLE2=front
EXECUTABLE3=back

all: $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3)

$(EXECUTABLE1):
	$(CC) $(CFLAGS) $(SRC_DIR)/ex1.c -o $(EXECUTABLE1) -lm

$(EXECUTABLE2):
	$(CC) $(CFLAGS) $(SRC_DIR)/front.c -o $(EXECUTABLE2) -lm

$(EXECUTABLE3):
	$(CC) $(CFLAGS) $(SRC_DIR)/back.c -o $(EXECUTABLE3) -lm

clean:
	rm -f $(EXECUTABLE1) $(EXECUTABLE2) $(EXECUTABLE3)

test:
	@echo "Test: Cwiczenie 1 - numeryczne obliczanie calki funkcji f(x) = 4/(x^2 + 1) na przedziale (0, 1)"
	@echo "Test: Szerokość prostokąta 0.000000001 i 1 proces"
	./ex1 0.000000001 1
	@echo "Test: Szerokość prostokąta 0.000000001 i 5 procesów"
	./ex1 0.000000001 5
	@echo "Jak widzimy, dla takiej samej dokladnosci, podejscie z wieloma procesami okazalo sie szybsze"
