CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11 -pedantic -O0

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, obj/%.o, $(SRC))

all: build regex

build:
	mkdir -p obj

regex: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

obj/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^ -c

clean:
	rm -rf $(OBJ) regex

run: regex
	./regex
