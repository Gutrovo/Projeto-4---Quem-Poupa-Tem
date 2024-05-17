#define TOTAL 255
#define TRANSACOES 100 




typedef struct {
    char descricao[50]; 
    float valor;
    char data[20]; 
} Transacao;

typedef struct {
    char nome[50];
    int cpf[12]; 
    int tipo_conta; 
    int senha;
    float saldo;
    Transacao transacoes[TRANSACOES]; 
    int qtd_transacoes;
    
} Infos;

typedef enum {OK, MAX_TAREFA, SEM_TAREFAS, NAO_ENCONTRADO, ABRIR, FECHAR, ESCREVER, LER} ERROS;

typedef ERROS (*funcao)(Infos[], int*);

ERROS addCriaCliente(Infos infos[], int *pos);
ERROS deletar(Infos infos[], int *pos);
ERROS listar(Infos infos[], int *pos);
ERROS debito(Infos infos[], int *pos);
ERROS deposito(Infos infos[], int *pos);
ERROS extrato(Infos infos[], int *pos);
ERROS transferencia(Infos infos[], int *pos);
ERROS gravar(Infos infos[], int *pos);
ERROS salvar(Infos infos[], int *pos);
ERROS carregar(Infos infos[], int *pos);

void clearBuffer();


