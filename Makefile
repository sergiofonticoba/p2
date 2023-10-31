CC = gcc
CFLAGS = -Wall -I.

TARGET = p1
SRC = p1.c listas.c funcionesP1.c
OBJ = $(SRC:.c=.o)
HEADERS = listas.h funcionesP1.h

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
