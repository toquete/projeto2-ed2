#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*************************************************************************************
Funcao: fileOpen
Parametro: uma string contendo o nome do arquivo a ser aberto
Retorno: um ponteiro para o arquivo aberto
Descricao: tenta abrir o arquivo primeiro em modo de leitura, caso ele exista. Se o
		   arquivo nao existir, a abertura eh feita em modo de escrita.
************************************************************************************/

FILE *fileOpen(char *fileName)
{
    FILE *fp;
	
    if ((fp = fopen(fileName,"r+b")) == NULL)
    {
        fclose(fp);
    	if((fp = fopen(fileName,"w+b")) == NULL)
            printf("ERRO AO ABRIR O ARQUIVO\n");
    }
    
    return fp;
}
/*************************************************************************************
Funcao: isEmpty
Parametro: ponteiro de um arquivo
Retorno: 1 se o arquivo eh vazio e 0 caso contrario
Descricao: conta o numero de bytes do arquivo
************************************************************************************/

int isEmpty(FILE *file)
{
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
      return 1;
    return 0;
}
