CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -I../include -g -O0
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer

SRC = $(wildcard *.c)
BIN = utest

.PHONY: all test clean

all: clean test

test: $(BIN)
	@./$(BIN)

$(BIN): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	$(RM) $(BIN)

