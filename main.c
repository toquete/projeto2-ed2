#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "Funcs.h"


void cabecalho()
{
    printf("------------------------------------------------------------------------------\n");
    printf("|                    VETERINARIA - PORJETO II                                 |\n");
    printf("|                               ED II                                         |\n");
    printf("------------------------------------------------------------------------------\n");
}

int menu()
{
    int op;
    do{printf("\n1. Inserir Nova Vacina\n");
      printf("\n2. Listagem Todas as Vacinas\n");
      printf("\n3. Buscar Vacina (Arvore-B)\n");
      printf("\n4. Buscar Vacina (Hash)\n");
      printf("\n0 - SAIR \n");
      
	  printf("\n\nDIGITE A OPERACAO: ");
      scanf("%d",&op);
    }while(op < 0 || op > 4);

    return op;
}

void validaOpcao(int opcao)
{    
}

int main()
{
    int opcao;

    
    do{
        system("cls");
        cabecalho();
        opcao = menu();
        validaOpcao(opcao);
    }while(opcao != 0);

    system("pause");
    return 0;
}
