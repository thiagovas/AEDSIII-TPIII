# Made by Thiago Vieira
# The option -c in gcc: Compile and assemble, but do not link
CC=gcc

all: build clearafter

build: main.o heap.o vector.o externalSorting.o utility.o
	$(CC) main.o heap.o vector.o externalSorting.o -o tp3 -lm -O2

main.o: utility.o externalSorting.o
	$(CC) -c main.c -o main.o

heap.o:
	$(CC) -c heap.c -o heap.o

vector.o:
	$(CC) -c vector.c -o vector.o

externalSorting.o: utility.o heap.o
	$(CC) -c externalSorting.c -o externalSorting.o

utility.o:
	$(CC) -c utility.c -o utility.o

#Clear temporary files after compiling
clearafter:
	#Option -r: Remove recursive and
	#Option -f: ignore nonexistent files, never prompt
	rm -rf *.o
	rm -rf *~ #Remove temporary files

clear: clearafter
	rm -f tp3
	rm -rf *.scratch
