all:
	clang -O3 -lm -lSDL src/*.c

debug:
	clang -g -lm -lSDL src/*.c
