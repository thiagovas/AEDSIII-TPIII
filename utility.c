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
