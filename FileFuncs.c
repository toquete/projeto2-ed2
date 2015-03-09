#include <stdio.h>
#include <stdlib.h>
#include <conio.h>


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
