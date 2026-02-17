CC = riscv64-linux-gnu-gcc
CFLAGS = -Wall -Wextra -std=c99 -static
TARGET = hello_world.bin
SRC = hello_world.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean