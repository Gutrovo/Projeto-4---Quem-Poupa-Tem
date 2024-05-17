#include "funcoes.h"
#include <stdio.h>
#include <time.h>

int Digitos(long long num) {
    int digitos = 0;
    while (num != 0) {
        num /= 10;
        digitos++;
    }
    return digitos;
}




ERROS addCriaCliente(Infos infos[], int *pos) {
    if (*pos >= TOTAL) {
        return MAX_TAREFA; 
    }

 
    printf("Entre com seu nome: ");
    fgets(infos[*pos].nome, sizeof(infos[*pos].nome), stdin);
    infos[*pos].nome[strcspn(infos[*pos].nome, "\n")] = 0; 

   
    long cpf; 
    while (1) {
        printf("Entre com seu CPF (11 dígitos): ");
        scanf("%lld", &cpf); 
        clearBuffer(); 

        if (Digitos(cpf) == 11){ 
           
            for (int i = 0; i < 11; i++) {
                infos[*pos].cpf[10 - i] = (int)(cpf % 10); 
                cpf /= 10; 
            }
            break;
        } else {
            printf("CPF inválido! O CPF deve ter 11 dígitos.\n");
        }
    }

   
    printf("Digite o tipo da conta (comum/plus): ");
    
    char tipo[10];
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = 0; 

    if (strcmp(tipo, "comum") == 0) {
        infos[*pos].tipo_conta = 0;
    } else if (strcmp(tipo, "plus") == 0) {
        infos[*pos].tipo_conta = 1; 
    } else {
        printf("Tipo de conta inválido.\n");
        return OK; 
    }

  
    printf("Digite seu saldo inicial: ");
    scanf("%f", &infos[*pos].saldo); 
    clearBuffer(); 

   
    printf("Digite sua senha (máximo 6 dígitos): ");
    scanf("%d", &infos[*pos].senha); 
    clearBuffer();

    infos[*pos].qtd_transacoes = 0;
    (*pos)++; 

    return OK; 
}

ERROS deletar(Infos infos[], int *pos) {
    if (*pos == 0) {
        return SEM_TAREFAS;  
    }

    char cpf_deletar[15];
    printf("Entre com o número do cpf a ser deletado: ");
    fgets(cpf_deletar, sizeof(cpf_deletar), stdin);
        cpf_deletar[strcspn(cpf_deletar, "\n")] = 0;


    

    int encontrado = -1;
    for (int i = 0; i < *pos; i++) {
        char cpf_comparacao[15];
        strcpy(cpf_comparacao, infos[i].cpf);
        

        if (strcmp(cpf_comparacao, cpf_deletar) == 0) {  
            encontrado = i;  
            break;
        }
    }

    if (encontrado == -1) {
        return NAO_ENCONTRADO;  
    }


    for (int i = encontrado; i < *pos - 1; i++) {
        infos[i] = infos[i + 1];  
    }

    (*pos)--;  

    printf("Cliente deletado com sucesso\n");

    return OK;
}

ERROS listar(Infos infos[], int *pos) {
    if (*pos == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return SEM_TAREFAS;
    }
    
    printf("-------------------------\n");
    printf("Lista de clientes:\n");
    for (int i = 0; i < *pos; i++) {
        printf("Nome: %s\n", infos[i].nome);  
        printf("CPF: ");
        for (int j = 0; j < 11; j++) {
            printf("%d", infos[i].cpf[j]);
        }
        printf("\n");   

       
        if (infos[i].tipo_conta == 0) {
            printf("Tipo de conta: comum\n");
        } else if (infos[i].tipo_conta == 1) {
            printf("Tipo de conta: plus\n");
        } else {
            printf("Tipo de conta: desconhecido\n");
        }

   
        printf("Saldo inicial: %.2f\n", infos[i].saldo);
    }
        printf("-------------------------\n");
    
    return OK;
}

int buscaClientePorCPF(Infos infos[], int pos, long long cpf) {
    int cpf_digitos[11];
    for (int i = 0; i < 11; i++) {
        cpf_digitos[10 - i] = (int)(cpf % 10);
        cpf /= 10; 
    }

    for (int i = 0; i < pos; i++) {
        int igual = 1; 
        for (int j = 0; j < 11; j++) {
            if (infos[i].cpf[j] != cpf_digitos[j]) { 
                igual = 0; 
                break;
            }
        }
        if (igual) {
            return i; 
        }
    }
    return -1; 
}


ERROS debito(Infos infos[], int *pos) {
    long long cpf;
    int senha;
    float valorDeb;
    char data[11]; 
    long indiceCliente;


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(data, sizeof(data), "%d/%m/%Y", &tm); 

    
    printf("Digite seu CPF (11 dígitos): ");
    scanf("%lld", &cpf); 
    clearBuffer(); 

    printf("Digite sua senha: ");
    scanf("%d", &senha); 

    printf("Digite o valor a ser debitado: ");
    scanf("%f", &valorDeb); 

   
    indiceCliente = buscaClientePorCPF(infos, *pos, cpf);

    if (indiceCliente == -1) { 
        printf("Cliente não encontrado.\n");
        return NAO_ENCONTRADO;
    }

    
    if (infos[indiceCliente].senha != senha) { 
        printf("Senha incorreta.\n");
        return OK; 
    }

   
    float tarifa;
    if (infos[indiceCliente].tipo_conta == 0) {
        tarifa = valorDeb * 0.05;
        if (infos[indiceCliente].saldo - (valorDeb + tarifa) < -1000) {
            printf("Débito recusado. Saldo insuficiente.\n");
            return OK;
        }
    } else if (infos[indiceCliente].tipo_conta == 1) { 
        tarifa = valorDeb * 0.03;
        if (infos[indiceCliente].saldo - (valorDeb + tarifa) < -5000) {
            printf("Débito recusado. Saldo insuficiente.\n");
            return OK;
        }
    } else {
        printf("Tipo de conta inválido.\n");
        return OK;
    }

   
    infos[indiceCliente].saldo -= (valorDeb + tarifa); 


    int trans_idx = infos[indiceCliente].qtd_transacoes;
    if (trans_idx < TRANSACOES) { 
        strcpy(infos[indiceCliente].transacoes[trans_idx].descricao, "Débito");
        infos[indiceCliente].transacoes[trans_idx].valor = valorDeb + tarifa;
        strcpy(infos[indiceCliente].transacoes[trans_idx].data, data); 
        infos[indiceCliente].qtd_transacoes++; 
    } else {
        printf("Limite de transações atingido.\n");
    }

    
    printf("Débito realizado com sucesso!\n");
    printf("Tarifa: %.2f\n", tarifa);
    printf("Saldo atual: %.2f\n", infos[indiceCliente].saldo);

    return OK;
}










ERROS deposito(Infos infos[], int *pos) {
    long long cpf;
    int senha;
    float valorDep;
    char data[11]; 
    long indiceCliente;


    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(data, sizeof(data), "%d/%m/%Y", &tm); 

  
    printf("Digite seu CPF (11 dígitos): ");
    scanf("%lld", &cpf); 
    clearBuffer(); 

    printf("Digite sua senha: ");
    scanf("%d", &senha);

    printf("Digite o valor a ser depositado: ");
    scanf("%f", &valorDep); 

    
    indiceCliente = buscaClientePorCPF(infos, *pos, cpf);

    if (indiceCliente == -1) { 
        printf("Cliente não encontrado.\n");
        return NAO_ENCONTRADO;
    }

   
    if (infos[indiceCliente].senha != senha) { 
        printf("Senha incorreta.\n");
        return OK; 
    }

     infos[indiceCliente].saldo += valorDep;
    int trans_idx = infos[indiceCliente].qtd_transacoes;
    if (trans_idx < TRANSACOES) { 
        strcpy(infos[indiceCliente].transacoes[trans_idx].descricao, "Depósito");
        infos[indiceCliente].transacoes[trans_idx].valor = valorDep;
        strcpy(infos[indiceCliente].transacoes[trans_idx].data, data); 
        infos[indiceCliente].qtd_transacoes++; 
    } else {
        printf("Limite de transações atingido.\n");
    }
    
  printf("Depósito realizado com sucesso!\n");
  printf("Saldo atual: %.2f\n", infos[indiceCliente].saldo);

   return OK;
        

}


ERROS extrato(Infos infos[], int *pos){
    long long cpf;
    int senha;
    


    printf("Digite seu CPF (11 dígitos): ");
    scanf("%lld", &cpf); 
    clearBuffer(); 

    printf("Digite sua senha: ");
    scanf("%d", &senha);
    clearBuffer(); 

 
    int indiceCliente = buscaClientePorCPF(infos, *pos, cpf);

    if (indiceCliente == -1) { 
        printf("Cliente não encontrado.\n");
        return NAO_ENCONTRADO;
    }


    if (infos[indiceCliente].senha != senha) { 
        printf("Senha incorreta.\n");
        return OK; 
    }


    printf("------------------\n");
    printf("Cliente: %s\n", infos[indiceCliente].nome);
    printf("CPF: ");
    for (int i = 0; i < 11; i++) { 
        printf("%d", infos[indiceCliente].cpf[i]); 
    }
    printf("\n");

    printf("Tipo de conta: %s\n", (infos[indiceCliente].tipo_conta == 0) ? "comum" : "plus");
    printf("Saldo atual: %.2f\n", infos[indiceCliente].saldo);
    printf("------------------\n");

    printf("\n");
    
    printf("Histórico de transações:\n");
    for (int i = 0; i < infos[indiceCliente].qtd_transacoes; i++) {
        printf("Transação %d:\n", i + 1);
        printf("  Data: %s\n", infos[indiceCliente].transacoes[i].data);
        printf("  Descrição: %s\n", infos[indiceCliente].transacoes[i].descricao);
        printf("  Valor: %.2f\n", infos[indiceCliente].transacoes[i].valor);
    }

    printf("-------------------------\n");

    return OK;
}

ERROS transferencia(Infos infos[], int *pos) {
    long long cpfOrigem;
    long long cpfDestino;
    int senha;
    float valor;
    float tarifa;
    float valorTotal;
    char data[11]; 



    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(data, sizeof(data), "%d/%m/%Y", &tm); 

    printf("Digite o CPF do remetente (11 dígitos): ");
    scanf("%lld", &cpfOrigem);
    clearBuffer(); 

    printf("Digite a senha do remetente: ");
    scanf("%d", &senha);

    printf("Digite o CPF do destinatário (11 dígitos): ");
    scanf("%lld", &cpfDestino);
    clearBuffer();

    printf("Digite o valor a ser transferido: ");
    scanf("%f", &valor);

    int indiceOrigem = buscaClientePorCPF(infos, *pos, cpfOrigem);
    int indiceDestino = buscaClientePorCPF(infos, *pos, cpfDestino);

    if (indiceOrigem == -1 || indiceDestino == -1) {
        printf("CPF do remetente ou destinatário não encontrado.\n");
        return NAO_ENCONTRADO;
    }

    if (infos[indiceOrigem].senha != senha) {
        printf("Senha incorreta.\n");
        return OK; 
    }

    if (infos[indiceOrigem].tipo_conta == 0) { 
        tarifa = valor ;
        valorTotal = valor + tarifa;
        if (infos[indiceOrigem].saldo - valorTotal < -1000) {
            printf("Transferência recusada, saldo insuficiente.\n");
            return OK; 
        }
    } else if (infos[indiceOrigem].tipo_conta == 1) { 
        tarifa = valor ;
        valorTotal = valor + tarifa;
        if (infos[indiceOrigem].saldo - valorTotal < -5000) {
            printf("Transferência recusada, saldo insuficiente.\n");
            return OK;
        }
    }


    infos[indiceOrigem].saldo -= valorTotal; 
    infos[indiceDestino].saldo += valor; 

   
    int idxOrigem = infos[indiceOrigem].qtd_transacoes;
    if (idxOrigem < TRANSACOES) {
        strcpy(infos[indiceOrigem].transacoes[idxOrigem].descricao, "Transferência");
        infos[indiceOrigem].transacoes[idxOrigem].valor = valorTotal;
        strcpy(infos[indiceOrigem].transacoes[idxOrigem].data, data); 
        infos[indiceOrigem].qtd_transacoes++;
    } else {
        printf("Limite de transações para o remetente atingido.\n");
    }

    int idxDestino = infos[indiceDestino].qtd_transacoes;
    if (idxDestino <TRANSACOES) {
        strcpy(infos[indiceDestino].transacoes[idxDestino].descricao, "Recebimento de transferência");
        infos[indiceDestino].transacoes[idxDestino].valor = valor; 
        strcpy(infos[indiceDestino].transacoes[idxDestino].data, data); 
        infos[indiceDestino].qtd_transacoes++;
    } else {
        printf("Limite de transações para o destinatário atingido.\n");
    }

    printf("Transferência realizada com sucesso!\n");

    return OK; 
}



ERROS gravar(Infos infos[], int *pos){
    FILE *f = fopen("arquivo.txt", "w");
        if (f == NULL) {
            return ABRIR;
        }

    for (int i = 0; i < *pos; i++) {

        
        fprintf(f, "Nome: %s\n", infos[i].nome); 
        fprintf(f, "CPF: "); 
        for (int j = 0; j < 11; j++) { 
            fprintf(f, "%d", infos[i].cpf[j]);
        }
        fprintf(f, "\n");

        fprintf(f, "Tipo de conta: %s\n", 
                (infos[i].tipo_conta == 0) ? "comum" : "plus"); 
        fprintf(f, "Saldo: %.2f\n", infos[i].saldo); 

        
        fprintf(f, "Histórico de transações:\n");
        for (int k = 0; k < infos[i].qtd_transacoes; k++) {
            fprintf(f, "  Descrição: %s\n", infos[i].transacoes[k].descricao);
            fprintf(f, "  Valor: %.2f\n", infos[i].transacoes[k].valor);
            fprintf(f, "  Data: %s\n", infos[i].transacoes[k].data);
        }

        fprintf(f, "-------------------------\n");
    }


       

        if (fclose(f) != 0) { 
            return FECHAR; 
        }

        return OK;
    }





ERROS salvar(Infos infos[], int *pos){
    FILE *f = fopen("tarefas.bin", "wb");
    if(f == NULL)
        return ABRIR;

    int qtd = fwrite(infos, TOTAL, sizeof(Infos), f);
    if(qtd == 0)
        return ESCREVER;

    qtd = fwrite(pos, 1, sizeof(int), f);
    if(qtd == 0)
        return ESCREVER;

    if(fclose(f))
        return FECHAR;

    return OK;
}


ERROS carregar(Infos infos[], int *pos){
    FILE *f = fopen("tarefas.bin", "rb");
    if(f == NULL)
        return ABRIR;

    int qtd = fread(infos, TOTAL, sizeof(Infos), f);
    if(qtd == 0)
        return LER;

    qtd = fread(pos, 1, sizeof(int), f);
    if(qtd == 0)
        return LER;

    if(fclose(f))
        return FECHAR;

    return OK;

}



void clearBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}
