/*
	Made by Thiago Vieira
*/

#include "externalSorting.h"

void openStream(fita *obj, char nomeFita[])
{
	obj->f = fopen(nomeFita, "w");
}

void closeStream(fita *obj)
{
	fclose(obj->f);
}

void writeInteger(fita *obj, int value)
{
	fprintf(obj->f, "%d ", value);
}

void writeEndOfBlock(fita *obj)
{
	fprintf(obj->f, "0\n");
}

/* Método que ordena o arquivo input e imprime o resultado e output. */
/* @Param input: Stream que será usada para ler os dados a serem ordenados. */
/* @Param output: Stream que será usada para escrever os dados do input ordenadamente. */
/* @Param maxMemory:  Parametro que indica o máximo de memória que a heap pode usar. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas a serem usadas. */
void sort(FILE *input, FILE *output, int maxMemory, int numberScratchFiles)
{
	fita *fitas = (fita*)alloc(numberScratchFiles, sizeof(fita));
	int i, data, atualValue = 0, max = floor(((double)maxMemory)/sizeof(heap_item));
	char f[13] = "f", atualMark = '\0';
	heap priority_queue;
	
	InitHeap(&priority_queue, comp);
	i = 0;
	/* Leitura dos primeiros max números da entrada */
	while(i < max && !feof(input))
	{
		fscanf(input, "%d", &data);
		PushHeap(&priority_queue, data, 0);
		i++;
	}
	
	/* Abrindo as streams das fitas */
	for(i = 0; i < numberScratchFiles; i++)
	{
		sprintf(f, "f%d.scratch", i);
		openStream(&fitas[i], f);
	}
	
	/* Lendo o resto da entrada e preenchendo as fitas. */
	i = 0; // A variavel i representará a fita atual, a fita em que está sendo escrito o bloco atual.
	while(!feof(input))
	{
		atualMark = FrontHeapMark(&priority_queue);
		atualValue = FrontHeapValue(&priority_queue);
		fscanf(input, "%d", &data);
		
		PopHeap(&priority_queue);
		if(data < atualValue)
			PushHeap(&priority_queue, data, (atualMark+1)%2);
		else
			PushHeap(&priority_queue, data, atualMark);
		
		writeInteger(&fitas[i], atualValue);
		
		/* Verificando se estou no último elemento de um bloco. */
		if(atualMark != FrontHeapMark(&priority_queue))
		{
			writeEndOfBlock(&fitas[i]);
			i += 1;
			i %= numberScratchFiles;
		}
	}

	/* Fechando as streams das fitas. */
	for(i = 0; i < numberScratchFiles; i++)
		closeStream(&fitas[i]);
	
	free(fitas);
}
