CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-lrt -lpthread

SRC_DIR=src

SERVER_EXEC=server
CLIENT_EXEC=client

all: $(SERVER_EXEC) $(CLIENT_EXEC)

$(SERVER_EXEC): $(SRC_DIR)/server.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(CLIENT_EXEC): $(SRC_DIR)/client.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(SERVER_EXEC) $(CLIENT_EXEC)

.PHONY: all clean
