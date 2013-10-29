/*
	Feito por Thiago Vieira
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "heap.h"

/* Função que converte uma string para um inteiro */
int stringtoint(char *arg);

/* Função que passo para a heap ordenar os elementos. Estilo qsort */
int comp(const unsigned int, const unsigned int);


int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Siga o padrao:\n./tp3 [entrada] [saida] [Numero de fitas] [Tamanho da memoria disponivel]\n");
		return 1;
	}
	
	int numFitas = stringtoint(argv[3]), tamMemoria = stringtoint(argv[4]);
	FILE **fitas = (FILE**)alloc(numFitas, sizeof(FILE*));
	FILE *entrada = fopen(argv[1], "r"), *saida;
	int i, input, atualValue = 0, max = floor(((double)tamMemoria)/sizeof(heap_item));
	char f[13] = "f", atualMark = '\0';
	heap priority_queue;
	
	InitHeap(&priority_queue, comp);
	i = 0;
	while(i < max && !feof(entrada))
	{
		fscanf(entrada, "%d", &input);
		PushHeap(&priority_queue, input, 0);
		i++;
	}
	
	/* Abrindo as streams das fitas */
	for(i = 0; i < numFitas; i++)
	{
		sprintf(f, "f%d", i);
		fitas[i] = fopen(f, "w");
	}
	
	i = 0;
	while(!feof(entrada))
	{
		atualMark = FrontHeapMark(&priority_queue);
		atualValue = FrontHeapValue(&priority_queue);
		fscanf(entrada, "%d", &input);
		if(input < atualValue)
		{
			// Trocar atualMark
		}
	}

	/* Fechando as streams das fitas. */
	for(i = 0; i < numFitas; i++)
		fclose(fitas[i]);
	
	fclose(entrada);
	free(fitas);
	return 0;
}

int comp(const unsigned int a, const unsigned int b)
{
	return a < b;
}

/* Função que converte uma string para um inteiro */
int stringtoint(char *arg)
{
	int i = strlen(arg)-1, cont = 0, resp = 0;
	
	while(i >= 0)
	{
		resp += pow(10, cont)*(arg[i]-'0');
		cont++;
		i--;
	}
	return resp;
}
