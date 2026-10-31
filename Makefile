CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
.PHONY: all test clean
all: build/test_tetris
build:
	mkdir -p build
build/test_tetris: tests/test_tetris.c src/tetris.c include/tetris.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_tetris.c src/tetris.c -o $@
test: all
	./build/test_tetris
clean:
	rm -rf build
