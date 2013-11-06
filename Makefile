# Made by Thiago Vieira
# The option -c in gcc: Compile and assemble, but do not link
CC=gcc

all: build clearafter

build: main.o heap.o vector.o externalSorting.o utility.o
	$(CC) main.o heap.o vector.o externalSorting.o utility.o -o tp3 -lm -O2

main.o: externalSorting.o utility.o
	$(CC) -c main.c -o main.o -Wall

heap.o:
	$(CC) -c heap.c -o heap.o -Wall

vector.o:
	$(CC) -c vector.c -o vector.o -Wall

externalSorting.o: utility.o heap.o vector.o
	$(CC) -c externalSorting.c -o externalSorting.o -Wall

utility.o:
	$(CC) -c utility.c -o utility.o -Wall

#Clear temporary files after compiling
clearafter:
	#Option -r: Remove recursive and
	#Option -f: ignore nonexistent files, never prompt
	rm -rf *.o
	rm -rf *~ #Remove temporary files

clean: clearafter
	rm -f tp3
	rm -rf f[*
