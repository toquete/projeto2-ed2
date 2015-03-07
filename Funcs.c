#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "FileFuncs.h"


/********************************************************
			Definicoes das estruturas
*********************************************************/
typedef struct {
	int codigoControle;
    int codigoCachorro;
    char vacina[30];
    char dataVacina[6];
    char respVacina[30];	
} regAP1;

typedef struct {
	int codigoCachorro;
    char raca[30];
    char nomeCachorro[30];	
} regAP2;


/*******************************************************/


//****************   ARVORE B   ***********************
//**** constantes
#define MAXKEYS   4               // numero maximo de chaves em uma pagina
#define MINKEYS   2               // numero minimo de chaves em uma pagina apos split
#define NIL       (-1)            // RRN nulo
#define NOKEY     (-1)            // Chave nula
#define NO        0               
#define YES       1
#define PAGESIZE  sizeof(BTPAGE)  // tamanho de uma pagina


typedef struct btree_page {
  short keycount;         // numero de chaves presentes na pagina
  regAP1 key[MAXKEYS];     // chaves da pagina
  short child[MAXKEYS+1]; // ponteiros 
} BTPAGE;

short btroot; // RRN da pagina raiz da arvore B

void cadastraCachorro(FILE **AP2)
{
    regAP2 reg;
    
    system("CLS");
    printf(" Codigo: ");
    scanf("%d", &reg.codigoCachorro);
    while (existeCachorro(*AP2, reg.codigoCachorro))
    {
        system("CLS");
		printf("\n Codigo ja cadastrado. Digite novamente!");
		getch(); 
        system("CLS");
		printf(" Codigo: ");
        scanf("%d", &reg.codigoCachorro);
    }
    fflush(stdin);
    printf("\n Raca: ");
    gets(reg.raca);
    printf("\n Nome: ");
    gets(reg.nomeCachorro);
    
    fseek(*AP2, 0, SEEK_END);
    fwrite(&reg, sizeof(regAP2), 1, *AP2);   
}

int existeCachorro(FILE *AP2, int codigo)
{
    regAP2 reg;
	
	fseek(AP2, 0, SEEK_SET);
	while (fread(&reg, sizeof(regAP2), 1, AP2))
	{
		if (reg.codigoCachorro == codigo)
		{
			return YES;
			break;
		}	
	}
    return NO;
}

void cadastraVacina(FILE **fpAP1, FILE **fpAP2, FILE **fpBtree, FILE **fpHash)
{
    regAP1 reg;
    
    system("CLS");
    printf(" Codigo de controle: ");
    scanf("%d", &reg.codigoControle);
    //TODO: verificar no indice se o código existe!
    printf("\n Codigo do cachorro: ");
    scanf("%d", &reg.codigoCachorro);
    while (!existeCachorro(*fpAP2, reg.codigoCachorro))
    {
        system("CLS");
		printf("\n Codigo de cachorro inexistente. Digite novamente!");
		getch(); 
        system("CLS");
        printf(" Codigo do cachorro: %d", reg.codigoControle);
		printf("\n Codigo do cachorro: ");
        scanf("%d", &reg.codigoCachorro);
    }
    fflush(stdin);
    printf("\n Nome da vacina: ");
    gets(reg.vacina);
    printf("\n Data de vacinacao (mm/aa): ");
    gets(reg.dataVacina);
    printf("\n Responsavel pela vacina: ");
    gets(reg.respVacina);
    
    fseek(*fpAP1, 0, SEEK_END);
    fwrite(&reg, sizeof(regAP1), 1, *fpAP1);   
}

