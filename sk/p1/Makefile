CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2

TARGET = traceroute

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET)

.PHONY: clean distclean