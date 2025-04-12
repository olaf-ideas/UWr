CC = g++
FLAGS=-Wall -Wextra -std=c++20 -ggdb3 -fsanitize=undefined,address -Wshadow

TARGET = router

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

%.o: %.c
	$(CC) $(FLAGS) -c -o $@ $< 

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGET)

.PHONY: clean distclean