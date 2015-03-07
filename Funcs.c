#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#include "FileFuncs.h"


/********************************************************
			Definicoes das estruturas
*********************************************************/
typedef struct registroAp1 {
	int codigoControle;
    int codigoCachorro;
    char vacina[50];
    char dataVacina[50];
    char respVacina[50];	
}regAP1;

typedef struct registroAp2 {
	int codigoCachorro;
    char raca[50];
    char nomeCachorro[50];	
}regAP2;


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

