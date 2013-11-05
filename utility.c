/*
	Made by Thiago Vieira
*/

#include "utility.h"

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

int isFileEmpty(char fileName[])
{
    FILE *file = fopen(fileName, "a+");
    int resp = isStreamEmpty(file);
    fclose(file);
    return resp;
}

int isStreamEmpty(FILE *file)
{
	long currentPos = ftell(file);
	fseek(file, 0, SEEK_END);
	
	if (ftell(file) == 0)
		return 1;
	
	fseek(file, currentPos, SEEK_SET);
	return 0;
}

int fileSize(char fileName[])
{
	int resp = 0;
	FILE *temp = fopen(fileName, "a+");
	fseek(temp, 0L, SEEK_END);
	resp = ftell(temp);
	fseek(temp, 0L, SEEK_SET);
	fclose(temp);
	return resp;
}

void ClearFile(char fileName[])
{
	FILE *arq = fopen(fileName, "w");
	fclose(arq);
}

/* Função passada para o TAD heap. A heap usa essa função para ordenar os elementos. */
int comp(const unsigned int a, const unsigned int b)
{
	return a < b;
}

/* Função que retorna a variável com o menor valor. */
int min(int a, int b)
{
	return a < b ? a : b;
}

/* Função que retorna a variável com o maior valor. */
int max(int a, int b)
{
	return a > b ? a : b;
}
