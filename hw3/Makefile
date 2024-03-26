CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = hw3
SRC = hw3.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)
