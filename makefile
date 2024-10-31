CFLAGS = -std=c17 -Wall -Wextra -Werror
all:
	gcc chip8.c -o out $(CFLAGS) `sdl2-config --cflags --libs`
