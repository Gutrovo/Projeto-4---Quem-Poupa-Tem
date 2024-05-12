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
