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
#include "vector.h"

typedef struct{
	FILE *f;
	char active = '\0';
} fita;

void openStream(fita *obj, char nomeFita[]);

void closeStream(fita *obj);

void writeInteger(fita *obj, int value);

void writeEndOfBlock(fita *obj);

void activateScratchFiles(fita *scratchFiles, int n);

void deactivateScratchFiles(fita *scratchFiles, int index);

int isActive(fita *scratchFiles, int index);

/* Método que ordena o arquivo input e imprime o resultado e output. */
/* @Param input: Stream que será usada para ler os dados a serem ordenados. */
/* @Param output: Stream que será usada para escrever os dados do input ordenadamente. */
/* @Param maxMemory:  Parametro que indica o máximo de memória que a heap pode usar. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas a serem usadas. */
void sort(FILE *input, FILE *output, int maxMemory, int numberScratchFiles);

/* Método que faz a intercalação das fitas. */
/* @Param maxHeapElements: Parametro que indica o número máximo de elementos que a heap pode ter. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas que estão sendo usadas. */
void merge(int maxHeapElements, int numberScratchFiles);

void addCounter(int &i, int mod);


#endif

