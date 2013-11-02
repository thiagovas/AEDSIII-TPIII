/*
	Made by Thiago Vieira
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "externalSorting.h"
#include "utility.h"

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("Siga o padrao:\n./tp3 [entrada] [saida] [Numero de fitas] [Tamanho da memoria disponivel]\n");
		return 1;
	}
	
	FILE *entrada, *saida;
	entrada = fopen(argv[1], "r");
	saida = fopen(argv[2], "w");
	
	sort(entrada, saida, stringtoint(argv[4]), stringtoint(argv[3]));
	
	fclose(entrada);
	fclose(saida);
	
	return 0;
}

