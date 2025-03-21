SERVER = server
CLIENT = client

SRC_DIR = src

SERVER_SRC = $(SRC_DIR)/server.c
CLIENT_SRC = $(SRC_DIR)/client.c

CFLAGS = -Wall -pthread

# Domyślne cele
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(CLIENT): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(SERVER) $(CLIENT)

run_server:
	./$(SERVER) 127.0.0.1 8080

run_client:
	./$(CLIENT) client1 127.0.0.1 8080

.PHONY: all clean run_server run_client
