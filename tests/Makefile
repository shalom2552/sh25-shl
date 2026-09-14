CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -I../ -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer

SRC = $(wildcard *.c)
BIN = test_runner

.PHONY: all test clean

all: clean test

test: $(BIN)
	@./$(BIN)

$(BIN): $(SRC)
	@$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	$(RM) $(BIN)

