#include <stdio.h>
#include "funcoes.h"

int main(){
     funcao fs[] = {addCriaCliente,deletar,listar,debito,deposito,extrato,transferencia,gravar,salvar,carregar};
     Infos infos[TOTAL];

int pos;
int opcao;
do{
printf("Bem vindo! \n");
printf("------------------\n");
printf("0 - Sair\n");
printf("1 - Criar conta\n");
printf("2 - Apagar cliente \n");
printf("3 - Listar cliente\n");
printf("4 - Debito\n");
printf("5 - Deposito\n");
printf("6 - Extrato\n");
printf("7 - Transferencias entre contas\n");
printf("8 - salvar em txt\n");
printf("9 - salvar em bin\n");
printf("10 - Carregar em bin\n");
printf("------------------\n");
printf("Escolha uma Opção: ");
scanf("%d",&opcao);
    
   clearBuffer(); 

          if (opcao > 0 && opcao < 11) {
              ERROS erro = fs[opcao - 1](infos, &pos);
              printf("------------------\n");
          } else if (opcao == 0) {
              printf("Até mais =( \n");
          } else {
              printf("**Opção Inválida**\n");
              printf("------------------\n");
          }
      } while (opcao > 0);

      return 0;
}
