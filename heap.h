/*
    Feito por Thiago Vieira
*/

#ifndef _heap_h
#define _heap_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct{
	unsigned int value;
	char mark;
} heap_item;

typedef struct{
	heap_item *values;
	int size;
	
	/* http://stackoverflow.com/questions/9410/how-do-you-pass-a-function-as-a-parameter-in-c */
	int (*func)(const unsigned int, const unsigned int); // Função usada para organizar a heap.
	/*
		Func deve retornar true se o primeiro parâmetro deve vir antes do segundo.
	*/

} heap;

/* Método que inicializa a heap. A função recebida é usada para organizar os valores pela heap. */
void InitHeap(heap *obj, int (*func)(const unsigned int, const unsigned int));

/* Método que limpa a heap. */
void ClearHeap(heap *obj);

/* Método que insere um novo elemento na heap. */
void PushHeap(heap *obj, unsigned int valor, char mark);

/* Função que retorna o valor do elemento raiz da heap. */
unsigned int FrontHeapValue(heap *obj);

/* Função que retorna o valor da variável mark do elemento raiz da heap. */
char FrontHeapMark(heap *obj);

/* Método que organiza a heap. */
void Heapify(heap *obj, int index);

/* Método que retira o elemento raiz da heap. */
void PopHeap(heap *obj);

/* Função que retorna o tamanho da heap. */
int SizeHeap(heap *obj);

/* Função que retorna se a heap está vazia. */
int EmptyHeap(heap *obj);

/* Função que retorna o valor do indice do filho esquerdo do nó i. */
int Left(int i);

/* Função que retorna o valor do indice do filho direito do nó i */
int Right(int i);

/* Função que retorna o valor do indice do pai do nó i. */
int Father(int i);
#endif
