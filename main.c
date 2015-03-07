#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Funcs.h"
#include "FileFuncs.h"

#define Ap1 "AP1.bin"
#define Ap2 "AP2.bin"
#define BtreeIdx "btIdx.bin"
#define HashIdx "hashIdx.bin"

#define MENUPRINCIPAL 0
#define INSERCAO      1
#define ARVOREB       1
#define CACHORRO      1
#define HASH          2
#define LISTA         2
#define VACINA        2
#define PESQUISA      3
#define TRUE          1
#define FALSE         0

int cabecalho(int tipomenu)
{
    system("CLS");
    printf("------------------------------------------------------------------------------\n");
    printf("|                         VETERINARIA - PROJETO II                            |\n");
    printf("|                                   ED II                                     |\n");
    printf("------------------------------------------------------------------------------\n");
    switch(tipomenu)
    {
        case MENUPRINCIPAL:
            printf("--------------------------------MENU PRINCIPAL--------------------------------\n");
            printf("------------------------------------------------------------------------------\n\n");
            printf("1. Inserir Nova Vacina\n");
            printf("2. Listar Todas as Vacinas\n");
            printf("3. Pesquisar uma Vacina\n");
            printf("\n\nEscolha a opcao: ");
            break;
        case INSERCAO:
            printf("-----------------------------------INSERCAO-----------------------------------\n");
            printf("------------------------------------------------------------------------------\n\n");
            printf("1. Cadastra Cachorro\n");
            printf("2. Cadastra Vacina\n");
            printf("\n\nEscolha a opcao: ");
            break;
        case LISTA:
            printf("-----------------------------LISTAGEM DAS VACINAS-----------------------------\n");
            printf("------------------------------------------------------------------------------\n\n"); 
            break;
        case PESQUISA:
            printf("-----------------------------------PESQUISA-----------------------------------\n");
            printf("------------------------------------------------------------------------------\n\n"); 
            printf("1. Arvore B\n");
            printf("2. Hash\n");
            printf("\n\nEscolha a opcao: ");
            break;
        default:
            printf("Opcao invalida! Verifique!");
            getch();
            break;
    }
}

void abreArquivos(FILE **fpAP1, FILE **fpAP2, FILE **fpBtree, FILE **fpHash)
{
    *fpAP1   = fileOpen(Ap1);
    *fpAP2   = fileOpen(Ap2);
    *fpBtree = fileOpen(BtreeIdx);
    *fpHash  = fileOpen(HashIdx);
}
void fechaArquivos(FILE **fpAP1, FILE **fpAP2, FILE **fpBtree, FILE **fpHash)
{
    fclose(*fpAP1);
	fclose(*fpAP2);
	fclose(*fpBtree);
	fclose(*fpHash);
}

void validaOpcao(int opcao, FILE **fpAP1, FILE **fpAP2, FILE **fpBtree, FILE **fpHash)
{ 
    int op;
    
    switch(opcao)
    {
        case INSERCAO:
            cabecalho(INSERCAO);
            scanf("%d", &op);
            /*while ((op < 1) || (op > 2))
            {
                system("CLS");
                printf("Opcao invalida! Digite novamente!");
                getch();
                cabecalho(INSERCAO);
            }*/
            if (op == 1)
              cadastraCachorro(fpAP2);
            else
              cadastraVacina(fpAP1, fpAP2, fpBtree, fpHash);
            break;                
    }   
}

int main()
{
    int opcao;
	FILE *fpAP1, *fpAP2, *fpBtree, *fpHash;
	
	abreArquivos(&fpAP1, &fpAP2, &fpBtree, &fpHash);
    
    do
    {
        cabecalho(MENUPRINCIPAL);
        scanf("%d", &opcao);
        validaOpcao(opcao, &fpAP1, &fpAP2, &fpBtree, &fpHash);
    }
    while(opcao != 0);
	
	fechaArquivos(&fpAP1, &fpAP2, &fpBtree, &fpHash);	
	
    system("pause");
    return 0;
}
