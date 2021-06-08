#Makefile
srcdir = ./src
builddir = ./build
CC = gcc
LDLIBS=-lncurses

tetris: src/tetris.c
	gcc src/tetris.c -lncurses -o ./build/tetris

clean:
	rm build/tetris
