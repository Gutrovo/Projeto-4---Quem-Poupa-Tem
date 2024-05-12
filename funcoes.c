#include "funcoes.h"
#include <stdio.h>
#include <time.h>


ERROS addCriaCliente(Infos infos[], int *pos) {
    if (*pos >= TOTAL) {
        return MAX_TAREFA; 
    }

 
    printf("Entre com seu nome: ");
    fgets(infos[*pos].nome, sizeof(infos[*pos].nome), stdin);
    infos[*pos].nome[strcspn(infos[*pos].nome, "\n")] = 0; 

   
    long long cpf; 
    while (1) {
        printf("Entre com seu CPF (11 dígitos): ");
        scanf("%lld", &cpf); 
        clearBuffer(); 

        if (cpf >= 10000000000LL && cpf <= 99999999999LL) { 
           
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


ERROS deposito(Infos infos[], int *pos) {
    long long cpf;
    int senha;
    float valorDep;
    char data[11]; 


    
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

    
    int indiceCliente = buscaClientePorCPF(infos, *pos, cpf);

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
    if (trans_idx < MAX_TRANSACOES) { 
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
