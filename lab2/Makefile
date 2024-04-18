CC=gcc
CFLAGS=-Wall -g -O0

SRC_DIR=src
OBJ_DIR_STATIC=obj_static
OBJ_DIR_SHARED=obj_shared
LIB_DIR=lib

LIB_STATIC=$(LIB_DIR)/libcollatz.a
LIB_SHARED=$(LIB_DIR)/libcollatz.so

OBJ_STATIC=$(OBJ_DIR_STATIC)/collatz.o
OBJ_SHARED=$(OBJ_DIR_SHARED)/collatz.o

.PHONY: all static shared clean

all: static shared

static: $(LIB_STATIC)

$(LIB_STATIC): $(OBJ_STATIC)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_STATIC): $(SRC_DIR)/collatz.c
	mkdir -p $(OBJ_DIR_STATIC)
	$(CC) $(CFLAGS) -c $< -o $@

shared: $(LIB_SHARED)

$(LIB_SHARED): $(OBJ_SHARED)
	mkdir -p $(LIB_DIR)
	$(CC) -shared -o $@ $^

$(OBJ_SHARED): $(SRC_DIR)/collatz.c
	mkdir -p $(OBJ_DIR_SHARED)
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

client_static: $(SRC_DIR)/main.c $(LIB_STATIC)
	$(CC) $(CFLAGS) -o $@ $^

client_shared: $(SRC_DIR)/main.c $(LIB_SHARED)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/main.c -L$(LIB_DIR) -lcollatz -Wl,-rpath,$(LIB_DIR)

client_dynamic: $(SRC_DIR)/dynamic_client.c
	$(CC) $(CFLAGS) -o $@ $< -ldl -Wl,-rpath,$(LIB_DIR)

clean: 
	rm -rf $(OBJ_DIR_STATIC) $(OBJ_DIR_SHARED) $(LIB_DIR) client_shared client_static client_dynamic
