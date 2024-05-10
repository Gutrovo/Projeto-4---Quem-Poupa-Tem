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
