/*
    Made by Thiago Vieira
*/

#include "heap.h"

/* http://en.wikipedia.org/wiki/Heap_(data_structure) */

/* Método que inicializa a heap. A função recebida é usada para organizar os valores pela heap. */
void InitHeap(heap *obj, int (*func)(const unsigned int, const unsigned int))
{
	obj->size = 0;
	obj->values = NULL;
	obj->func = func;
}

/* Método que limpa a heap. */
void ClearHeap(heap *obj)
{
	if(obj->values == NULL) return;
	
	free(obj->values);
	obj->values = NULL;
	obj->size = 0;
}

/* Método que organiza a heap. */
void Heapify(heap *obj, int index)
{
	int l = Left(index), r = Right(index);
	int largest;
	heap_item temp;
	
	if(l < SizeHeap(obj) && obj->func(obj->values[l].value, obj->values[index].value))
		largest = l;
	else
		largest = index;
	
	if(r < SizeHeap(obj) && obj->func(obj->values[r].value, obj->values[largest].value))
		largest = r;
	
	if(largest != index)
	{
		temp = obj->values[largest];
		obj->values[largest] = obj->values[index];
		obj->values[index] = temp;
		
		Heapify(obj, largest);
	}
}

/* Método que insere um novo elemento na heap. */
void PushHeap(heap *obj, unsigned int valor, char mark)
{
	char atual = FrontHeapMark(obj);
	int index = SizeHeap(obj);
	heap_item *temp = (heap_item*) realloc(obj->values, (index+1)*sizeof(heap_item));
	heap_item swap;
	heap_item neue;
	
	neue.value = valor;
	neue.mark = mark;
	if(temp == NULL)
	{
		// Se temp for null, quer dizer que a alocação de memória não foi feita com sucesso.
		free(obj->values);
		printf("Memória não foi alocada com sucesso.\n");
		return;
	}
	
	obj->values = temp;
	obj->values[index] = neue;
	obj->size += 1;
	
	while(index > 0 && !obj->func(obj->values[Father(index)].value, obj->values[index].value))
	{
		swap = obj->values[Father(index)];
		obj->values[Father(index)] = obj->values[index];
		obj->values[index] = swap;
		index = Father(index);
	}
}

/* Função que retorna o valor do elemento raiz da heap. */
unsigned int FrontHeapValue(heap *obj)
{
	if(SizeHeap(obj) == 0) return -1;
	return obj->values[0].value;
}


/* Função que retorna o valor da variável mark do elemento raiz da heap. */
char FrontHeapMark(heap *obj)
{
	if(SizeHeap(obj) == 0) return 0;
	return obj->values[0].mark;
}

/* Método que retira o elemento raiz da heap. */
void PopHeap(heap *obj)
{
	obj->values[0] = obj->values[SizeHeap(obj)-1];
	obj->size -= 1;
	obj->values = realloc(obj->values, SizeHeap(obj)*sizeof(heap_item));
	Heapify(obj, 0);
}

/* Função que retorna o tamanho da heap. */
int SizeHeap(heap *obj)
{ return obj->size; }

/* Função que retorna se a heap está vazia. */
int EmptyHeap(heap *obj)
{
	return obj->size == 0;
}

/* Função que retorna o valor do indice do filho esquerdo do nó i. */
int Left(int i)
{
	return (i<<1)+1;
}

/* Função que retorna o valor do indice do filho direito do nó i */
int Right(int i)
{
	return Left(i)+1;
}

/* Função que retorna o valor do indice do pai do nó i. */
int Father(int i)
{
	return i>>1;
}
