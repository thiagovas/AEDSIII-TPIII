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
	char active;
} fita;

void openStream(fita *obj, char fileName[], const char op[]);

void closeStream(fita *obj);

int ReadScratchs(heap *pq, fita *scratch, int op, int numberScratchFiles);

void writeInteger(fita *obj, unsigned int value);

void writeStream(FILE *file, unsigned int value);

void writeEndOfBlock(fita *obj);

/* Método que ordena o arquivo input e imprime o resultado em output. */
/* @Param input: Stream que será usada para ler os dados a serem ordenados. */
/* @Param output: Stream que será usada para escrever os dados do input ordenadamente. */
/* @Param maxMemory:  Parametro que indica o máximo de memória que a heap pode usar. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas a serem usadas. */
void sort(FILE *input, FILE *output, int maxMemory, int numberScratchFiles);

/* Método que faz a intercalação das fitas. */
/* @Param maxHeapElements: Parametro que indica o número máximo de elementos que a heap pode ter. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas que estão sendo usadas. */
void merge(int op, int maxHeapElements, int numberScratchFiles);

void writeEndBlockMerge(fita *scratch, int i, int op, int numberScratchFiles);

void BeginMerge(heap *h, fita *scratch, int op, int numberScratchFiles);

void writeBack(FILE *output, int finalBlock);

void EndMerge(fita *scratch, int op, int numberScratchFiles);

int IsItOver(int op, int numberScratchFiles);

void activateScratchFiles(fita *scratchFiles, int n);

void deactivateScratchFile(fita *scratchFiles, int index);

int isActive(fita *scratchFiles, int index);

void addCounterOp(int *i, int op, int numberScratchFiles);

void addCounter(int *i, int begin, int mod);

void ClearScratchFiles(int begin, int end);

#endif

