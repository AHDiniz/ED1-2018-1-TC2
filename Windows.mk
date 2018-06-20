# Makefile para Windows

CC = gcc

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

EXE = compact.exe

CFLAGS = -Wall -g -O3 -lmingw32

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o