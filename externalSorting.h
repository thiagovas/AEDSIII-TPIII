/*
	Made by Thiago Vieira
*/

#ifndef _externalSorting_h
#define _externalSorting_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "heap.h"
#include "utility.h"

typedef struct{
	FILE *f;
} fita;

/*
	Made by Thiago Vieira
*/

#include "externalSorting.h"

void openStream(fita *obj, char nomeFita[]);

void closeStream(fita *obj);

void writeInteger(fita *obj, int value);

void writeEndOfBlock(fita *obj);

/* Método que ordena o arquivo input e imprime o resultado e output. */
/* @Param input: Stream que será usada para ler os dados a serem ordenados. */
/* @Param output: Stream que será usada para escrever os dados do input ordenadamente. */
/* @Param maxMemory:  Parametro que indica o máximo de memória que a heap pode usar. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas a serem usadas. */
void sort(FILE *input, FILE *output, int maxMemory, int numberScratchFiles);


#endif
