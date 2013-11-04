/*
	Made by Thiago Vieira
*/

#include "externalSorting.h"

void openStream(fita *obj, char fileName[], const char op[])
{
	obj->f = fopen(fileName, op);
}

void closeStream(fita *obj)
{
	fclose(obj->f);
}

void writeInteger(fita *obj, unsigned int value)
{
	writeStream(obj->f, value);
}

void writeStream(FILE *file, unsigned int value)
{
	fprintf(file, "%u ", value);
}

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
	char finishedBlock = 0;
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
		PushHeap(&priority_queue, data, 0);
		fscanf(input, "%u", &data);
		i++;
	}
	
	/* Abrindo as streams das fitas */
	for(i = 0; i < numberScratchFiles; i++)
	{
		sprintf(f, "f%d.scratch", i);
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
			PushHeap(&priority_queue, data, (atualMark+1)%2); // (atualMark+1)%2 inverte o valor de atualMark, de 0 para 1 ou de 1 para 0.
		else
			PushHeap(&priority_queue, data, atualMark);
		
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
	
	merge(max, numberScratchFiles);
}

/* Método que faz a intercalação das fitas. */
/* @Param maxHeapElements: Parametro que indica o número máximo de elementos que a heap pode ter. */
/* @Param numberScratchFiles: Parametro que recebe o número de fitas que estão sendo usadas. */
void merge(int maxHeapElements, int numberScratchFiles)
{
	int i = 0, currentScratchFile = numberScratchFiles, numberActiveFiles = 0;
	int numberfeof = 0, op = 0, output;
	unsigned int data;
	heap priority_queue;
	char f[13] = "f";
	fita *scratch = (fita*)alloc(2*numberScratchFiles, sizeof(fita));
	
	
	InitHeap(&priority_queue, comp);
	op = 0;
	while((output = IsItOver(op, numberScratchFiles)) < 0)
	{
		BeginMerge(&priority_queue, scratch, op, numberScratchFiles);
		numberfeof = 0;
		
		while(numberfeof < numberScratchFiles)
		{
			activateScratchFiles(scratch, 2*numberScratchFiles);
			numberActiveFiles = numberScratchFiles;
			
			if(op == 0)
				i = 0;
			else
				i = numberScratchFiles;
			
			while(numberActiveFiles > 0)
			{
				if(scratch[i].active == 0)
				{
					addCounterOp(&i, op, numberScratchFiles);
					continue;
				}
				
				fscanf(scratch[i].f, "%u", &data);
				
				if(feof(scratch[i].f))
				{
					numberfeof++;
					scratch[i].active = 0;
					numberActiveFiles--;
				}
				else if(data == 0)
				{
					scratch[i].active = 0;
					numberActiveFiles--;
				}
				else
				{
					if(SizeHeap(&priority_queue) > 0)
					{
						if(op)
						{
							sprintf(f, "f%d.scratch", i-numberScratchFiles);
							openStream(&scratch[i-numberScratchFiles], f, "a+");
							writeInteger(&scratch[i-numberScratchFiles], FrontHeapValue(&priority_queue));
							closeStream(&scratch[i-numberScratchFiles]);
						}
						else
						{
							sprintf(f, "f%d.scratch", i+numberScratchFiles);	
							openStream(&scratch[i+numberScratchFiles], f, "a+");
							writeInteger(&scratch[i+numberScratchFiles], FrontHeapValue(&priority_queue));
							closeStream(&scratch[i+numberScratchFiles]);
						}
						PopHeap(&priority_queue);
					}
					PushHeap(&priority_queue, data, 0);				
				}
				//addCounterOp(&i, op, numberScratchFiles);
			}
			
			/*
			if(op == 0)
			{
				for(i = numberScratchFiles; i < 2*numberScratchFiles; i++)
				{
					sprintf(f, "f%d.scratch", i);
					openStream(&scratch[i], f, "a+");
					writeEndOfBlock(&scratch[i]);
					closeStream(&scratch[i]);
				}
			}
			else
			{
				for(i = 0; i < numberScratchFiles; i++)
				{
					sprintf(f, "f%d.scratch", i);
					openStream(&scratch[i], f, "a+");
					writeEndOfBlock(&scratch[i]);
					closeStream(&scratch[i]);
				}
			}*/
		}
		// TODO: Limpar a heap
		EndMerge(scratch, op, numberScratchFiles);
		op = (op+1)%2;
	}
	
	printf("OH %d\n", output);
	ClearHeap(&priority_queue);
	free(scratch);
}

/* */
int IsItOver(int op, int numberScratchFiles)
{
	int i, begin = 0, end = 0, outputFile = -1;
	char f[13] = "f";
	FILE *temp = NULL;
	
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
		sprintf(f, "f%d.scratch", i);
		
		if(!isFileEmpty(f))
		{
			if(outputFile > 0)
			{
				outputFile = -1;
				break;
			}
			else outputFile = i;
		}
	}
	
	return outputFile;
}

void BeginMerge(heap *h, fita *scratch, int op, int numberScratchFiles)
{
	int i, begin, end, data = 0;
	char f[13] = "f";
	
	if(op == 0)
	{
		begin = 0;
		end = numberScratchFiles;
		
		for(i = numberScratchFiles; i < 2*numberScratchFiles; i++)
		{
			sprintf(f, "f%d.scratch", i);
			ClearFile(f);
		}
	}
	else
	{
		begin = numberScratchFiles;
		end = 2*numberScratchFiles;
		
		for(i = 0; i < numberScratchFiles; i++)
		{
			sprintf(f, "f%d.scratch", i);
			ClearFile(f);
		}
	}
	
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f%d.scratch", i);
		openStream(&scratch[i], f, "r");
		fscanf(scratch[i].f, "%u", &data);
		if(data == 0 || feof(scratch[i].f)) continue;
		PushHeap(h, data, 0);
	}
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
	*i %= mod;
	if(*i == 0)
		*i = begin;
}

void ClearScratchFiles(int begin, int end)
{
	int i = 0;
	char f[13] = "f";
	
	for(i = begin; i < end; i++)
	{
		sprintf(f, "f%d.scratch", i);
		ClearFile(f);
	}
}

