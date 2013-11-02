/*
    Made by Thiago Vieira
*/

#include "vector.h"

/* Função que aloca n posições de nBytes bytes e retorna um ponteiro de void. */
void* alloc(int n, size_t nBytes)
{
    void *ptr = calloc(n, nBytes);
    if(ptr == NULL)
    {
        printf("Error: Memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

/* Método que inicializa a estrutura vector e já preenche as posições com o defaultValue. */
void InitVector(vector *obj, int size, int defaultValue)
{
	int i = 0;
	obj->sizeObj = size;
	obj->list = (item*)alloc(size, sizeof(item));
	
	for(i = 0; i < size; i++)
		obj->list[i].value = defaultValue;
}

/* Método que inicializa um vector vazio. */
void InitEmptyVector(vector *obj)
{
	obj->sizeObj = 0;
	obj->list = NULL;
}

/* Método que insere um novo elemento no fim do vector obj. */
void Push_back(vector *obj, int value)
{
	obj->list = (item*)realloc(obj->list, (obj->sizeObj + 1)*sizeof(item));
	obj->list[obj->sizeObj].value = value;
	obj->sizeObj += 1;
}

/* Método que insere um novo elemento no inicio do vector. */
void Push_front(vector *obj, int value)
{
	int i = 0;
	obj->list = (item*)realloc(obj->list, (obj->sizeObj + 1)*sizeof(item));
	
	for(i = obj->sizeObj; i > 0; i++)
		obj->list[i] = obj->list[i-1];
	obj->list[0].value = value;
	
	obj->sizeObj += 1;
}

/* Método que retira o último elemento do vector. */
void Pop_back(vector *obj)
{
	obj->list = (item*)realloc(obj->list, (obj->sizeObj - 1)*sizeof(item));
	obj->sizeObj -= 1;
}

/* Método que imprime os valores do vector. */
void PrintVector(vector obj)
{
	if(obj.sizeObj == 0) return;
	
	int i = 1;
	
	printf("%d", obj.list[0].value);
	for(i = 1; i < obj.sizeObj; i++)
		printf(" %d", obj.list[i].value);
	printf("\n");
}

/* Função que retorna o tamanho do vector. */
int SizeVector(vector obj)
{ return obj.sizeObj; }

/* Função que retorna se o vector esta vazio */
int Empty(vector *obj)
{
	if(obj == NULL) return 1;
	if(obj->list == NULL) return 1;
	return SizeVector(*obj) == 0;
}

/* Método que limpa o vector. */
void ClearVector(vector *obj)
{
	if(obj == NULL) return;
	if(obj->list == NULL) return;
	obj->sizeObj = 0;
	free(obj->list);
	obj->list = NULL;
}

/* Função que retorna o valor do elemento de posição index do vector. */
int At(vector *obj, int index)
{	return obj->list[index].value; }

/* Método que edita o valor de um elemento do vector. */
void EditItem(vector *obj, int index, int newValue)
{	obj->list[index].value = newValue; }
