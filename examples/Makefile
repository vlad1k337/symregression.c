CC = cc

CFLAGS   = -Wall -Wextra -Wpedantic 
CPPFLAGS = -I../include/ 
LDFLAGS  = -lm

SRC = $(wildcard *.c)

TARGET = a.out

all: $(TARGET)

debug: CFLAGS += -g3 -ggdb -fsanitize=address
debug: all

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

