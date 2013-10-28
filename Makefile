# Made by Thiago Vieira
# The option -c in gcc: Compile and assemble, but do not link
CC=gcc

all: build clearafter

build: main.o heap.o
	$(CC) main.o heap.o -o tp3

main.o: heap.h
	$(CC) -c main.c -o main.o

heap.o:
	$(CC) -c heap.c -o heap.o

#Clear temporary files after compiling
clearafter:
	#Option -r: Remove recursive and
	#Option -f: ignore nonexistent files, never prompt
	rm -rf *.o
	rm -rf *~ #Remove temporary files

clear: clearafter
	rm -f tp1
