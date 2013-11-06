/*
	Made by Thiago Vieira
*/

#include "externalSorting.h"

/* Função que abre a stream da fita. */
void openStream(fita *obj, char fileName[], const char op[])
{
	obj->f = fopen(fileName, op);
}

/* Função que fecha a stream da fita. */
void closeStream(fita *obj)
{
	fclose(obj->f);
}

/* Função que escreve um inteiro na fita */
void writeInteger(fita *obj, unsigned int value)
{
	writeStream(obj->f, value);
}

/* Função que escreve um inteiro em uma stream */
void writeStream(FILE *file, unsigned int value)
{
	fprintf(file, "%u ", value);
}

/* Função para escrever o final de um bloco */
void writeEndOfBlock(fita *obj)
{
	fprintf(obj->f, "0\n");
}

/* Método que ordena o arquivo input e imprime o resultado em output. */
/* @Param input: Stream que será usada para ler os dados a serem ordenados. */
/* @Param output: Stream que será usada para escrever os dados do input ordenadamente. */
/* @Param maxMemory:  Parametro que indica o máximo de memória que a heap pode usar. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas a serem usadas. */
void sort(FILE *input, FILE *output, int maxMemory, int numberScratchFiles)
{
	fita *fitas = NULL;
	int i, max = floor(((double)maxMemory)/sizeof(heap_item)); // Max guarda o número máximo de elementos que a heap pode guardar.
	char finishedBlock = 0;  int op = 0, finalBlock = 0;
	unsigned int data = 0, atualValue = 0;
	char f[13] = "f", atualMark = '\0';
	heap priority_queue;
	
	fitas = (fita*)alloc(numberScratchFiles, sizeof(fita));
	InitHeap(&priority_queue, comp);
	i = 0;
	
	/* Leitura dos primeiros max números da entrada */
	fscanf(input, "%u", &data); // http://stackoverflow.com/questions/5431941/while-feof-file-is-always-wrong
	while(i < max && !feof(input))
	{
		PushHeap(&priority_queue, data, -1, 0);
		fscanf(input, "%u", &data);
		i++;
	}
	
	/* Abrindo as streams das fitas */
	for(i = 0; i < numberScratchFiles; i++)
	{
		sprintf(f, "f[%d]", i);
		openStream(&fitas[i], f, "w");
	}
	
	/* Lendo o resto da entrada e preenchendo as fitas. */
	i = 0; // A variavel i representará a fita atual, a fita em que está sendo escrito o bloco atual.
	while(!feof(input))
	{
		atualMark = FrontHeapMark(&priority_queue);
		atualValue = FrontHeapValue(&priority_queue);
		
		PopHeap(&priority_queue);
		if(data < atualValue)
			PushHeap(&priority_queue, data, -1, (atualMark+1)%2); // (atualMark+1)%2 inverte o valor de atualMark, de 0 para 1 ou de 1 para 0.
		else
			PushHeap(&priority_queue, data, -1, atualMark);
		
		writeInteger(&fitas[i], atualValue);
		
		finishedBlock = 0;
		/* Verificando se estou no último elemento de um bloco. */
		if(atualMark != FrontHeapMark(&priority_queue))
		{
			finishedBlock = 1;
			writeEndOfBlock(&fitas[i]);
			addCounter(&i, 0, numberScratchFiles);
		}
		fscanf(input, "%u", &data);
	}
	
	while(!EmptyHeap(&priority_queue))
	{
		atualMark = FrontHeapMark(&priority_queue);
		atualValue = FrontHeapValue(&priority_queue);
		
		PopHeap(&priority_queue);
		writeInteger(&fitas[i], atualValue);
		
		finishedBlock = 0;
		/* Verificando se estou no último elemento de um bloco. */
		if(atualMark != FrontHeapMark(&priority_queue))
		{
			finishedBlock = 1;
			writeEndOfBlock(&fitas[i]);
			addCounter(&i, 0, numberScratchFiles);
		}
	}
	
	if(finishedBlock == 0)
		writeEndOfBlock(&fitas[i]);
	ClearHeap(&priority_queue);
	
	/* Fechando as streams das fitas. */
	for(i = 0; i < numberScratchFiles; i++)
		closeStream(&fitas[i]);
	free(fitas);
	
	op = 0;
	while((finalBlock = IsItOver(op, numberScratchFiles)) < 0)
	{
		merge(op, max, numberScratchFiles);
		op = (op+1)%2;
	}
	
	writeBack(output, finalBlock);
}

/* Método que faz a intercalação das fitas. */
/* @Param maxHeapElements: Parametro que indica o número máximo de elementos que a heap pode ter. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas que estão sendo usadas. */
void merge(int op, int maxHeapElements, int numberScratchFiles)
{
	int i = 0, iWrite = 0;
	int numberfeof = 0;
	unsigned int data, temp;
	heap priority_queue;
	char f[13] = "f";
	fita *scratch = (fita*)alloc(2*numberScratchFiles, sizeof(fita));
	
	InitHeap(&priority_queue, comp);
	BeginMerge(&priority_queue, scratch, op, numberScratchFiles);
	
	if(op == 0)
		iWrite = numberScratchFiles;
	else iWrite = 0;
	
	numberfeof = 0;
	
	while(numberfeof < numberScratchFiles)
	{
		i = FrontHeapOrigin(&priority_queue);
		while(!EmptyHeap(&priority_queue))
		{
			temp = FrontHeapValue(&priority_queue);
			
			sprintf(f, "f[%d]", iWrite);
			openStream(&scratch[iWrite], f, "a+");
			writeInteger(&scratch[iWrite], temp);
			closeStream(&scratch[iWrite]);
			i = FrontHeapOrigin(&priority_queue);
			PopHeap(&priority_queue);
			
			if(!feof(scratch[i].f))
			{
				fscanf(scratch[i].f, "%u", &data);
				if(data != 0)
					PushHeap(&priority_queue, data, i, 0);
			}
			else numberfeof++;
		}
		
		writeEndBlockMerge(scratch, iWrite, op, numberScratchFiles);
		addCounterOp(&iWrite, (op+1)%2, numberScratchFiles);
		numberfeof = ReadScratchs(&priority_queue, scratch, op, numberScratchFiles);
	}
	
	EndMerge(scratch, op, numberScratchFiles);
	ClearHeap(&priority_queue);
	free(scratch);
}

/* Função que escreve o final de bloco na fita i */
void writeEndBlockMerge(fita *scratch, int i, int op, int numberScratchFiles)
{
	char f[13] = "f";
	
	sprintf(f, "f[%d]", i);
	openStream(&scratch[i], f, "a+");
	writeEndOfBlock(&scratch[i]);
	closeStream(&scratch[i]);
}

/* Função que verifica se a intercalação acabou */
int IsItOver(int op, int numberScratchFiles)
{
	int i, begin = 0, end = 0, outputFile = -1;
	char f[13] = "f";
	
	if(op == 0)
	{
		begin = 0;
		end = numberScratchFiles;
	}
	else
	{
		begin = numberScratchFiles;
		end = 2*numberScratchFiles;
	}
	
	//printf("\nop: %d\n", op);
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f[%d]", i);
		
		if(fileSize(f) > 2)
		{
			//printf("%s: %d\n", f, fileSize(f));
			if(outputFile >= 0)
			{
				outputFile = -1;
				break;
			}
			else outputFile = i;
		}
	}
	
	//printf("Output: %d\n", outputFile);
	return outputFile;
}

/* Método que faz a primeira leitura das fitas */
void BeginMerge(heap *h, fita *scratch, int op, int numberScratchFiles)
{
	int begin, end;
	unsigned int i, data = 0;
	char f[13] = "f";
	
	if(op == 0)
	{
		begin = 0;
		end = numberScratchFiles;
		
		for(i = numberScratchFiles; i < 2*numberScratchFiles; i++)
		{
			sprintf(f, "f[%d]", i);
			ClearFile(f);
		}
	}
	else
	{
		begin = numberScratchFiles;
		end = 2*numberScratchFiles;
		
		for(i = 0; i < numberScratchFiles; i++)
		{
			sprintf(f, "f[%d]", i);
			ClearFile(f);
		}
	}
	
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f[%d]", i);
		openStream(&scratch[i], f, "r");
		fscanf(scratch[i].f, "%u", &data);
		if(data == 0 || feof(scratch[i].f)) continue;
		PushHeap(h, data, i, 0);
	}
}

/* Outra funçao para fazer a primeira leitura das fitas. Ela retorna a quantidade de End-Of-Files */
int ReadScratchs(heap *pq, fita *scratch, int op, int numberScratchFiles)
{
	int begin, end, cont = 0;
	unsigned int i, data = 0;
	char f[13] = "f";
	
	if(op == 0)
	{
		begin = 0;
		end = numberScratchFiles;
	}
	else
	{
		begin = numberScratchFiles;
		end = 2*numberScratchFiles;
	}
	
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f[%d]", i);
		fscanf(scratch[i].f, "%u", &data);
		if(feof(scratch[i].f))
		{
			cont++;
			continue;
		}
		if(data == 0) continue;
		PushHeap(pq, data, i, 0);
	}
	return cont;
}

/* Função que escreve a resposta no output. */
void writeBack(FILE *output, int finalBlock)
{
	char f[13] = "f";
	unsigned int data = 0;
	
	sprintf(f, "f[%d]", finalBlock);
	FILE *temp = fopen(f, "r");
	
	while(!feof(temp))
	{
		fscanf(temp, "%u", &data);
		if(data == 0) break;
		fprintf(output, "%u\n", data);
	}
	fclose(temp);
}

void EndMerge(fita *scratch, int op, int numberScratchFiles)
{
	int i, begin, end;
	
	if(op == 0)
	{
		begin = 0;
		end = numberScratchFiles;
	}
	else
	{
		begin = numberScratchFiles;
		end = 2*numberScratchFiles;
	}
	
	for(i = begin; i < end; i++)
		closeStream(&scratch[i]);
}

void activateScratchFiles(fita *scratchFiles, int n)
{
	int i = 0;
	for(i = 0; i < n; i++)
		scratchFiles[i].active = 1;
}

void deactivateScratchFile(fita *scratchFiles, int index)
{
	scratchFiles[index].active = 0;
}

int isActive(fita *scratchFiles, int index)
{
	return scratchFiles[index].active == 1;
}

void addCounterOp(int *i, int op, int numberScratchFiles)
{
	if(op == 0)
		addCounter(i, 0, numberScratchFiles);
	else
		addCounter(i, numberScratchFiles, 2*numberScratchFiles);
}

void addCounter(int *i, int begin, int mod)
{
	*i += 1;
	*i = ((*i%mod)+mod)%mod;
	if(*i == 0)
		*i = begin;
}

void ClearScratchFiles(int begin, int end)
{
	int i = 0;
	char f[13] = "f";
	
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f[%d]", i);
		ClearFile(f);
	}
}

