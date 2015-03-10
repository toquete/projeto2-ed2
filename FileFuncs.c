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

int isEmpty(FILE *file)
{
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
      return 1;
    return 0;
}
