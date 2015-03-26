#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


/***********************************************************************************************************
Funcao: fileOpen
Parametro: Nome do arquivo a ser aberto
Retorno: Ponteiro do arquivo aberto
Descricao: Abre o arquivo passado por parâmetro.
************************************************************************************************************/
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

/***********************************************************************************************************
Funcao: isEmpty
Parametros: ponteiro do arquivo a ser verificado
Retorno: YES - arquivo vazio; NO - arquivo não vazio
Descricao: Verifica se um determinado arquivo está vazio.
************************************************************************************************************/
int isEmpty(FILE *file)
{
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
      return 1;
    return 0;
}
