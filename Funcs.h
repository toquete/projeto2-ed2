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
void insereHash(int hashAddress, int chave, int RRN);
int buscaVacinaHash(int codigoControle);
