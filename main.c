#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Funcs.h"
#include "FileFuncs.h"

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

void validaOpcao(int opcao)
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
              cadastraCachorro();
            else
              cadastraVacina();
            break;                
    }   
}

int main()
{
    int opcao;
    
    inicializar();
    
    do
    {
        cabecalho(MENUPRINCIPAL);
        scanf("%d", &opcao);
        validaOpcao(opcao);
    }
    while(opcao != 0);
		
	encerrar();
	
    system("pause");
    return 0;
}
