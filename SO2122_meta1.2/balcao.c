#include "comuns.h"
#include <sys/wait.h>

int b_fifo_fd;
int c_fifo_fd;
int m_fifo_fd;

int main(int argc, char **argv){

  int bal_to_cla[2];
  int cla_to_bal[2];
  pipe(bal_to_cla);
  pipe(cla_to_bal);//
  int id,res;
  utent_t utent;
  balcao_t balc;
  char c_fifo_fname[50];
  char m_fifo_fname[50];

///////////////////////////
fprintf(stdout,"\nMEDICALso\n");
  res = mkfifo(BALC_FIFO, 0777);
  if (res == -1){
    perror("\nNao foi possivel abrir o Balcao");
    exit(EXIT_FAILURE);
}
fprintf(stderr, "\nBalcao de Atendimento criado\n");


b_fifo_fd = open(BALC_FIFO, O_RDWR);
if (b_fifo_fd == -1){
perror("\nErro ao abrir Balcao");
exit(EXIT_FAILURE);
}
 fprintf(stderr, "\nBom Dia\n Balcao aberto para atendimento");

 memset(utent.palavra, '\0', TAM_MAX);

 while (1){

res = read(b_fifo_fd, & utent, sizeof(utent));
if(res < sizeof(utent)){
  fprintf(stderr,"\nRecebida mensagem incompleta " "[bytes lidos: %d]", res);
  continue;
}

fprintf(stderr,"\nRecebido de %s sintoma %s\n",utent.nome, utent.palavra);

 if(!strcasecmp(utent.palavra, "fimb")){

 close(b_fifo_fd);
   unlink(BALC_FIFO);
   //exit(EXIT_SUCCESS);
   break;
 }
 if(!strcasecmp(utent.palavra, "fim")){
   close(c_fifo_fd);
   fprintf(stderr,"\nFIFO utente %s fechado\n",utent.nome);

 }

  strcpy(balc.palavra,utent.palavra);
  strcpy(balc.pnome,utent.nome);
  balc.pid = utent.pid_utent;
  fprintf(stderr, "\nutente %s sintoma %s\n",balc.pnome, balc.palavra);

 sprintf(c_fifo_fname, CLIENT_FIFO, utent.pid_utent);

 c_fifo_fd = open(c_fifo_fname, O_WRONLY);

 if(c_fifo_fd == -1)
   perror("\Erro no open - Ninguem quis a resposta");
   else{
     fprintf(stderr, "\nFIFO utente aberto para WRITE");
     strcpy(balc.palavra,"Nao SEI");
     res = write(c_fifo_fd, & balc, sizeof(balc));
         if(res == sizeof(balc))
           fprintf(stderr,"\nescreveu ao utente %s\n",utent.palavra);
           else
           perror("\nerro a escrever ao utente");

         close(c_fifo_fd);
         fprintf(stderr,"\nFIFO utente fechado\n");
         }

}
//////////////////////////
  fprintf(stdout,"\nBalcao de atendimento\n");
  printf("Insira os sintomas:\n");


  id=fork();

  if(id == 0){
    dup(cla_to_bal[1]);
    close(cla_to_bal[1]);
    close(cla_to_bal[0]);

    dup(bal_to_cla[0]);
    close(bal_to_cla[0]);
    close(bal_to_cla[1]);

    execl("classificador", "classificador",NULL);

  }else{

  dup(cla_to_bal[0]);
  close(cla_to_bal[0]);
  close(cla_to_bal[1]);

  dup(bal_to_cla[1]);
  close(bal_to_cla[1]);
  close(bal_to_cla[0]);

  wait(NULL);


}


return 0;
}
