#define BtreeIdx "btIdx.bin"

#define MAXKEYS   4               // numero maximo de chaves em uma pagina
#define MINKEYS   2               // numero minimo de chaves em uma pagina apos split
#define NIL       (-1)            // RRN nulo
#define NOKEY     (-1)            // Chave nula
#define NO        0               
#define YES       1
#define PAGESIZE  sizeof(regBTPage)  // tamanho de uma pagina

typedef struct {
	int codigoControle;
	short RRN;	
} regAP1Page;

typedef struct {
  short keycount;         // numero de chaves presentes na pagina
  regAP1Page key[MAXKEYS];     // chaves da pagina
  short child[MAXKEYS+1]; // ponteiros 
} regBTPage;

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

void cadastraCachorro();
void existeCachorro(int codigo);
int procuraCachorro(int codigo);
void cadastraVacina();
void inicializar();
void encerrar();
void criaHash();
int funcaoHash(int codigo);
void insereHash(int hashAddress, int chave, int offset);
int buscaVacinaHash(int codigoControle, int verifica);
int insereBTree(int codControle, short RRN);
void imprimeVacina(int codVacina, int posVacina);





