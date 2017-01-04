CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror
EXE = hm

SRC = $(wildcard *.c)

all:
	$(CC) $(CFLAGS) $(SRC) -o $(EXE)

clean:
	rm -f $(EXE)
