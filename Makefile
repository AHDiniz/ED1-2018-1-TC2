# Makefile para SO's baseados em Linux

CC = gcc

SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.0)

EXE = compact.out

CFLAGS = -Wall -g -O3

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o