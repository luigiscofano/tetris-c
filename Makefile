CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror -O2
LDFLAGS =

.PHONY: all test sanitize demo clean
all: build/tetris

build:
	mkdir -p build

build/tetris: src/main.c src/terminal.c src/tetris.c include/tetris.h include/terminal.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c src/terminal.c src/tetris.c $(LDFLAGS) -o $@

build/test_tetris: tests/test_tetris.c src/tetris.c include/tetris.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_tetris.c src/tetris.c $(LDFLAGS) -o $@

test: all build/test_tetris
	./build/test_tetris
	python3 tests/test_cli.py
	python3 tests/test_terminal.py

sanitize:
	$(MAKE) clean
	$(MAKE) test CFLAGS='-std=c11 -Wall -Wextra -Wpedantic -Werror -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer' LDFLAGS='-fsanitize=address,undefined'

demo: all
	./build/tetris --seed 42 --replay examples/partida.txt

clean:
	rm -rf build
