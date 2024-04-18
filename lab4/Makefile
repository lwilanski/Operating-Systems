CC=gcc
CFLAGS=-Wall -g

SRC_DIR=src

EXE1=ex1
EXE2=ex2

SRC_FILES1=$(SRC_DIR)/ex1.c
SRC_FILES2=$(SRC_DIR)/ex2.c

all: $(EXE1) $(EXE2)

$(EXE1): $(SRC_FILES1)
	$(CC) $(CFLAGS) -o $@ $<

$(EXE2): $(SRC_FILES2)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXE1) $(EXE2)
