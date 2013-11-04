/*
	Made by Thiago Vieira
*/

#ifndef _utility_h
#define _utility_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* Função que converte uma string para um inteiro */
int stringtoint(char *arg);

/* Função passada para o TAD heap. A heap usa essa função para ordenar os elementos. */
int comp(const unsigned int a, const unsigned int b);

/* Função que retorna a variável com o menor valor. */
int min(int a, int b);

/* Função que retorna a variável com o maior valor. */
int max(int a, int b);

int isFileEmpty(char fileName[]);

int streamIsEmpty(FILE *file);

void ClearFile(char fileName[]);

#endif
