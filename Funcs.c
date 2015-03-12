#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "FileFuncs.h"

/********************************************************
           Definições das estruturas/ponteiros 
                dos arquivos principais
*********************************************************/
#define Ap1 "AP1.bin"
#define Ap2 "AP2.bin"

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

FILE *fpAP1;
FILE *fpAP2;
/*******************************************************/

//****************   ARVORE B   ***********************
//**** constantes
#define BtreeIdx "btIdx.bin"

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
FILE *fpBtree;

//*****************  Fim ARVORE B   *****************************

//**********************      HASH      *************************
#define HashIdx "hashIdx.bin"

#define CESTOS 2                  //quantidade de cestos (buckets)
#define MAXHASH 11                //quantidade máxima de celulas hash no índice
 
typedef struct {
    int chave;
    int pos;            
} cellHash;

typedef struct {
    int cont;
    cellHash registro[CESTOS];   
} regHash;

FILE *fpHash;
//***************************************************************

void criaHash()
{
    int i, j;
    regHash reg;
    
    fseek(fpHash, 0, SEEK_SET);
    for (i = 0; i < MAXHASH; i++)
    {
        reg.cont = 0;
        for (j = 0; j < CESTOS; j++)
        {
            reg.registro[j].chave = -1;
            reg.registro[j].pos   = -1;   
        }
        fwrite(&reg, sizeof(regHash), 1, fpHash);
    }    
}

int funcaoHash(int codigo)
{
    return codigo%MAXHASH;
}

void insereHash(int hashAddress, int chave, int RRN)
{
    int tentativa = 0;
    regHash reg;
    
    fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);
    fread(&reg, sizeof(regHash), 1, fpHash);
    
    printf("\n Endereco: %d", hashAddress);
    if (reg.cont == 2)
    {
        while (reg.cont == 2)
        {
            tentativa++;
            printf("\n Colisao");
            printf("\n Tentativa %d", tentativa);
            if (hashAddress == (MAXHASH -1))
              hashAddress = 0;
            else
              hashAddress = funcaoHash(hashAddress + 1);    
            fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);
            fread(&reg, sizeof(regHash), 1, fpHash);
        }
    }
    printf("\n Chave %d inserida com sucesso!", chave);
    getch();
    
    reg.cont                         = reg.cont + 1;
    reg.registro[reg.cont - 1].chave = chave;
    reg.registro[reg.cont - 1].pos   = RRN;
    fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);
    fwrite(&reg, sizeof(regHash), 1, fpHash);
}

int buscaVacinaHash(int codigoControle)
{
    regHash regIdx;
    regAP2  regArqP2;
    int achou = NO, i, acessos = 1, hashAddress;
    
    hashAddress = funcaoHash(codigoControle);
    fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);
    fread(&regIdx, sizeof(regHash), 1, fpHash);
    
    while (regIdx.cont != -1)
    {
        for (i = 0; i < regIdx.cont; i++)
        {
            if (regIdx.registro[i].chave == codigoControle)
            {
                achou = YES;
                break;
            }
        }
        if (!achou)
        {
            if (hashAddress == (MAXHASH - 1))
              hashAddress = 0;
            else
              hashAddress = funcaoHash(hashAddress + 1);
            acessos++;
            fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);   
            fread(&regIdx, sizeof(regHash), 1, fpHash);
        }
        else
          break;
    }
    
    if (achou)
      printf("\n Chave %d encontrada, endereco %d, %d acesso(s)", codigoControle, hashAddress, acessos);
    else
      printf("\n Chave %d nao encontrada", codigoControle);
    getch();
}

void inicializar() 
{
	int size;
	char valorRoot[5];
	
	fpAP1   = fileOpen(Ap1);
	fpAP2   = fileOpen(Ap2);
	fpHash  = fileOpen(HashIdx);
	fpBtree = fileOpen(BtreeIdx);
	
	if (isEmpty(fpHash))
	  criaHash();
	
    fseek(fpBtree,1,SEEK_END);
    size = ftell(fpBtree) - 1;
    printf("tam %d",size);
    if(size){
    	fread(&btroot, sizeof(short), 1, fpBtree);
	}
	else{
		btroot = NIL;
		itoa(btroot, valorRoot, 10);
		fwrite(valorRoot, sizeof(short), 1, fpBtree);
	}
	
	printf("raiz     %d", btroot);
	rewind(fpBtree);
	fread(valorRoot,sizeof(short),1,fpBtree);
	printf("     %d", atoi(valorRoot));
	system("pause");
}

void encerrar()
{
    fclose(fpAP1);
	fclose(fpAP2);
	fclose(fpHash);
	fclose(fpBtree);
}

void cadastraCachorro()
{
    regAP2 reg;
    
    system("CLS");
    printf(" Codigo: ");
    scanf("%d", &reg.codigoCachorro);
    while (existeCachorro(reg.codigoCachorro))
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
    
    fseek(fpAP2, 0, SEEK_END);
    fwrite(&reg, sizeof(regAP2), 1, fpAP2);   
}

int existeCachorro(int codigo)
{
    regAP2 reg;
	
	fseek(fpAP2, 0, SEEK_SET);
	while (fread(&reg, sizeof(regAP2), 1, fpAP2))
	{
		if (reg.codigoCachorro == codigo)
			return YES;
	}
    return NO;
}

int procuraCachorro(int codigo)
{
    regAP2 reg;
    int offset;
	
	fseek(fpAP2, 0, SEEK_SET);
	offset = 0;
	while (fread(&reg, sizeof(regAP2), 1, fpAP2))
	{
		if (reg.codigoCachorro == codigo)
			return offset;
		offset = ftell(fpAP2);
	}   
}

void cadastraVacina()
{
    regAP1 reg;
    
    system("CLS");
    printf(" Codigo de controle: ");
    scanf("%d", &reg.codigoControle);
    //TODO: verificar no indice se o código existe!
    printf("\n Codigo do cachorro: ");
    scanf("%d", &reg.codigoCachorro);
    while (!existeCachorro(reg.codigoCachorro))
    {
        system("CLS");
		printf("\n Codigo de cachorro inexistente. Digite novamente!");
		getch(); 
        system("CLS");
        printf(" Codigo do cachorro: %d", reg.codigoControle);
		printf("\n\n Codigo do cachorro: ");
        scanf("%d", &reg.codigoCachorro);
    }
    fflush(stdin);
    printf("\n Nome da vacina: ");
    gets(reg.vacina);
    printf("\n Data de vacinacao (mm/aa): ");
    gets(reg.dataVacina);
    printf("\n Responsavel pela vacina: ");
    gets(reg.respVacina);
    
    fseek(fpAP1, 0, SEEK_END);
    insereHash(funcaoHash(reg.codigoControle), reg.codigoControle, ftell(fpAP1));
    fwrite(&reg, sizeof(regAP1), 1, fpAP1);   
}
