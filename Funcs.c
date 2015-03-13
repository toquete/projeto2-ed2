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
	int codigoControle;
	short RRN;	
} regAP1Page;



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
#define PAGESIZE  sizeof(regBTPage)  // tamanho de uma pagina

typedef struct {
  short keycount;         // numero de chaves presentes na pagina
  regAP1Page key[MAXKEYS];     // chaves da pagina
  short child[MAXKEYS+1]; // ponteiros 
} regBTPage;

short btroot; // RRN da pagina raiz da arvore B
FILE *fpBtree;

/*************************************************************************************
Funcao: escreveRaiz
Parametro: copia do RRN da raiz
Retorno: -
Descricao: Atualiza a raiz da arvore no cabecalho do arquivo sempre que necessario.
************************************************************************************/
void escreveRaiz(short root){
	
	fseek(fpBtree, 0, SEEK_SET);
	fwrite(&root,sizeof(short),1,fpBtree);
}


/*************************************************************************************
Funcao: novaPage
Parametro: -
Retorno: o RRN da nova pagina criada
Descricao: retorna o RRN de uma nova pagina quando necessario
************************************************************************************/
short novaPage(){
	
	long endereco;
	
	fseek(fpBtree, 0, SEEK_END);
	endereco = ftell(fpBtree) - sizeof(short); //o numero de bytes do arquivo som o cabecalho
	
	return ((short) endereco / PAGESIZE); 
}

/*************************************************************************************
Funcao: pegaPage
Parametro: rrn da raiz a ser recuperada e o ponteiro do registro para ser ligo
Retorno: a pagina lida
Descricao: posiciona o ponteiro na pagina e faz a leitura da mesma
************************************************************************************/
int pegaPage(short rrn, regBTPage *page){
	long endereco;
	
	endereco = (long) rrn * (long)PAGESIZE + sizeof(short);
	fseek(fpBtree, endereco, 0); //posiciona o ponteiro no byte do registro
	
	
	return (fread(page, PAGESIZE, 1, fpBtree));
}


/*************************************************************************************
Funcao: escrevePage
Parametro: rrn da raiz a ser recuperada e o ponteiro do registro para ser ligo
Retorno: o retorno da funcao fwrite
Descricao: posiciona o ponteiro na arquivo e faz a escrita da pagina
************************************************************************************/
int escrevePage(short rrn, regBTPage *page){
	
	long endereco;
	
	endereco = (long)rrn * (long)PAGESIZE + sizeof(short);
	fseek(fpBtree, endereco, 0); //posiciona o ponteiro no byte do registro
	
	
	return (fwrite(page, PAGESIZE, 1, fpBtree));
}

/*************************************************************************************
Funcao: iniciaPage
Parametro: ponteiro da pagina criada
Retorno: -
Descricao: a funcao inicializa a pagina criada como nula
************************************************************************************/
void iniciaPage(regBTPage *page){
	
	int i;
	
	for(i = 0; i < MAXKEYS; i++){
		page -> key[i].codigoControle = NOKEY;
		page -> key[i].RRN = NIL;
		page -> child[i] = NIL;
	}
	
	page -> child[MAXKEYS] = NIL; //pois o vetor child tem maxkeys + 1
}


/*************************************************************************************
Funcao: criaRaiz
Parametro: registro de pagina de AP1, valor dos filhos a esq e a dir
Retorno: rrn da nova raiz
Descricao: quando necessario a funcao cria uma nova raiz e retona o seu novo rrn
************************************************************************************/
short criaRaiz(regAP1Page key, short left, short right){
	
	regBTPage page;
	short rrn;
	
	rrn = novaPage();
	iniciaPage(&page);
	
	page.key[0] = key;
	page.child[0] = left;
	page.child[1] = right;
	page.keycount = 1;
	
	escrevePage(rrn, &page);
	escreveRaiz(rrn);
	
	return (rrn);
}


/***********************************************************************************************************
Funcao: buscaNo
Parametro: codigo de controle a ser buscado, possivel pagina em que ele se encontra e a posicao na pagina
Retorno: 1 se achou o no e 0 se nao achou
Descricao: a funcao percorre uma pagina buscando o no que se quer recuperar
************************************************************************************************************/
int buscaNo(int codControle, regBTPage *page, short *pos){
	
	int i;
	
	for(i = 0; i < page->keycount; i++){
		*pos = i;
		
		if(codControle > page->key[i].codigoControle)
			break;
	}
	
	if(*pos < page->keycount && (codControle == page->key[*pos].codigoControle))
		return (YES);
	
	else
		return (NO);
}


/***********************************************************************************************************
Funcao: inserePage
Parametro: registro de pagina AP1, o novo filho, registro de pagina de arvore
Retorno: -
Descricao: desloca os nos da pagina e adiciona a nova pagina e o RRN do filho
************************************************************************************************************/
void inserePage(regAP1Page key, short r_child, regBTPage *page){
	
	int i;
	
	for(i = page->keycount; key.codigoControle < page->key[i-1].codigoControle; i--){
		
		if (i < 0)
			break;
		
		page->key[i] = page->key[i-1];
		page->child[i+1] = page->child[i];
	}
	
	page->keycount++; 
	page->key[i] = key;
  	page->child[i+1] = r_child;
}


/*--------------------------------------------------------------------------------------------------------
Funcao: split
Parametro: (...)
Retorno: -
Descricao: divide uma pagina em caso de overflow e determina o RRN e a chave da pagina que foi promovida
----------------------------------------------------------------------------------------------------------*/
void split(regAP1Page key, short r_child, regBTPage *p_oldpage, regAP1Page *promo_key, short *promo_r_child, regBTPage *p_newpage){

	int j;
	short mid;
	regAP1Page workkeys[MAXKEYS+1];
	short workchil[MAXKEYS+2];
	  
	  // obter as chaves e os ponteiros da pagina atual
	  for (j = 0; j < MAXKEYS; j++){
	    workkeys[j] = p_oldpage->key[j];
	    workchil[j] = p_oldpage->child[j];
	  }  
	  workchil[j] = p_oldpage->child[j];
	  
	  // ordenar e acrescentar a nova pagina (o vetor tem MAXKEYS+1 chaves)
	  for (j = MAXKEYS; key.codigoControle < workkeys[j-1].codigoControle && j > 0; j--){
	    workkeys[j] = workkeys[j-1];
	    workchil[j+1] = workchil[j];
	  }
	  workkeys[j] = key;
	  workchil[j+1] = r_child;
	  
	  // obter RRN de nova pagina e inicializada
	  *promo_r_child = novaPage(); 
	  iniciaPage(p_newpage);
	
	  // separar as pagina antiga, adicionando chaves a pagina nova  
	  for (j = 0; j < MINKEYS; j++){
	    p_oldpage->key[j] = workkeys[j];
	    p_oldpage->child[j] = workchil[j];
	    
	    p_newpage->key[j] = workkeys[j+1+MINKEYS];
	    p_newpage->child[j] = workchil[j+1+MINKEYS];
	
	    p_oldpage->key[j+MINKEYS].codigoControle = NOKEY;
	    p_oldpage->key[j+MINKEYS].RRN = NIL;
	    
	    p_oldpage->child[j+1+MINKEYS] = NIL;
	  }
	  
	  p_oldpage->child[MINKEYS] = workchil[MINKEYS];
	  
	  p_newpage->child[MINKEYS] = workchil[j+1+MINKEYS];
	  
	  p_newpage->keycount = MAXKEYS - MINKEYS; // atualizar contador da pagina nova
	  
	  p_oldpage->keycount = MINKEYS;
	  
	  *promo_key = workkeys[MINKEYS];
}

int insere(short rrn, regAP1Page key, short *promo_r_child, regAP1Page *promo_key, int *duplicatedKey)
{
  regBTPage  page, newpage;  
  int     promoted; // boolean values
  short   pos,p_b_rrn;  // rrn promoted from below
  regAP1Page p_b_key;  // key promoted from below
  
  if (rrn == NIL)
  {
    // se rrn passado for NIL, indicar chave promovida
    *promo_key = key;
    *promo_r_child = NIL;
    return(YES);
  }
  
  pegaPage(rrn, &page); // ler a pagina no RRN passado
  // procurar por uma chave igual a que deve ser inserida nas chaves da pagina
  *duplicatedKey = buscaNo( key.codigoControle, &page, &pos);
  if (*duplicatedKey)
  {
    printf("Chave %d Duplicada\n", key.codigoControle);
    return(0);
  }
  // chamada recursiva de insert, para tentar inserir nas paginas filhas
  promoted = insere(page.child[pos], key, &p_b_rrn, &p_b_key, duplicatedKey);
  if (!promoted)
  {
    return(NO);
  }
  
  if(page.keycount < MAXKEYS)
  {
    // se ainda tem espaco na pagina, inserir nela mesmo
    inserePage(p_b_key, p_b_rrn, &page);
    escrevePage(rrn, &page);
    return(NO);
  }
  else
  {
    // senao, realizar divisao da pagina
    printf("Divisao de No.\n");
    split(p_b_key, p_b_rrn, &page, promo_key, promo_r_child, &newpage);
    printf("Chave %d Promovida.\n", promo_key->codigoControle);
    
    escrevePage(rrn, &page);
    escrevePage(*promo_r_child, &newpage);
    
    return(YES);
  }
}



int insereBTree(int codControle, short RRN){

  regAP1Page new_key, promoted_key;
  short promoted_r_child;
  int chave_dup, result = 0;
  
  new_key.codigoControle = codControle;
  new_key.RRN = RRN;
  
  if (btroot == NIL){
    // criar o no raiz se a arvore estiver vazia
    btroot = criaRaiz(new_key, NIL, NIL);
    result = 1;
  }
  else{
    // senao, tentar inserir
    if(insere(btroot, new_key, &promoted_r_child, &promoted_key, &chave_dup)){
      // se essa primeira chamada retornar registro prmovido, atualizar root
      btroot = criaRaiz(promoted_key, btroot, promoted_r_child);
    }
        
    result = !chave_dup; // indicar resultado da insercao 
  }
  
  if (result)
    printf("Chave %d inserida com sucesso.\n", new_key.codigoControle);    
  
  return result;	
}

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

int buscaVacinaHash(int codigoControle, int verifica)
{
    regHash regIdx;
    regAP2  regArqP2;
    int achou = NO, i, acessos = 1, hashAddress;
    
    hashAddress = funcaoHash(codigoControle);
    fseek(fpHash, hashAddress*sizeof(regHash), SEEK_SET);
    fread(&regIdx, sizeof(regHash), 1, fpHash);
    
    while (regIdx.cont != 0)
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
    
    if (!verifica)
    {
        if (achou)
          printf("\n Chave %d encontrada, endereco %d, %d acesso(s)", codigoControle, hashAddress, acessos);
        else
          printf("\n Chave %d nao encontrada", codigoControle);    
        getch();
    }
    return achou;
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
	
    fseek(fpBtree,0,SEEK_END);
    size = ftell(fpBtree);
    rewind(fpBtree);
    if(size){
    	fread(valorRoot, sizeof(short), 1, fpBtree);
    	btroot = (short) atoi(valorRoot);
	}
	else{
		btroot = NIL;
		itoa(btroot, valorRoot, 10);
		fwrite(valorRoot, sizeof(short), 1, fpBtree);
	}
	
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
    return NIL;   
}

void cadastraVacina()
{
    regAP1 reg;
    
    system("CLS");
    printf(" Codigo de controle: ");
    scanf("%d", &reg.codigoControle);
    while (buscaVacinaHash(reg.codigoControle, YES))
    {
        system("CLS");
        printf("Codigo ja cadastrado. Verifique!");
        getch();
        printf(" Codigo de controle: ");
        scanf("%d", &reg.codigoControle);
    }
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
    insereBTree(reg.codigoControle, ftell(fpAP1));
    insereHash(funcaoHash(reg.codigoControle), reg.codigoControle, ftell(fpAP1));
    fwrite(&reg, sizeof(regAP1), 1, fpAP1);   
}

void percursoEmOrdem(short btroot){
    
    regBTPage auxPage;
    regAP1 vacinaAux;
    regAP2 cachorroAux;
    int i, j;
    
    fseek(fpBtree, btroot*PAGESIZE,SEEK_SET);
    fread(&auxPage, PAGESIZE, 1, fpBtree);
    
    
    for (i = 0; i < MAXKEYS; i++)
    {
        percursoEmOrdem(auxPage.child[i]);
        
        if(auxPage.child[i] == NIL)
        {
            for (j = 0; j < auxPage.keycount; j++)
            {
                //fseek(fpBTree, aux.child[j]*PAGESIZE, SEEK_SET);
                //fread(&auxPage, PAGESIZE, 1, fpBTree);
                fseek(fpAP1, auxPage.key[j].RRN, SEEK_SET);
                fread(&vacinaAux, sizeof(regAP1), 1, fpAP1);
                fseek(fpAP2, procuraCachorro(vacinaAux.codigoCachorro), SEEK_SET);
                fread(&cachorroAux, sizeof(regAP2), 1, fpAP2);    
            }
            
            
        }
        else{
            
        }      
    }
    
}











































