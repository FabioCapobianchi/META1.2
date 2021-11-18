#include "comuns.h"
#include <signal.h>


int main(int argc, char **argv){

int b_fifo_fd;
int m_fifo_fd;
int p_fifo_fd;
medic_t medic;
balcao_t balcao;
utent_t utente;
char m_fifo_fname[25];
char p_fifo_fname[50];
int read_res;
int res;

 while(argc < 2){
   fprintf(stdout,"Faltam parametros!!!.\n Ex: ./medico <nome medico> <especialidade\n");
   exit(EXIT_FAILURE);
 }

//Cria o FIFO do medico
  strcpy(medic.mnome,argv[1]);
  strcpy(medic.espec,argv[2]);
    medic.pid_medic = getpid();
    sprintf(m_fifo_fname, MEDIC_FIFO, medic.pid_medic);

    if(mkfifo(m_fifo_fname, 0777) == -1){
      perror("\nmkfifo do FIFO medico deu erro");
        exit(EXIT_FAILURE);
}
printf("ola\n");

fprintf(stderr,"\nFIFO do medico criado");

//abre o FIFO do balcao WRITE / BLOCKING!!!!!
  b_fifo_fd = open(BALC_FIFO, O_WRONLY);
    if(b_fifo_fd == -1){
      fprintf(stderr, "\nO Balcao não esta a correr\n");
      unlink(m_fifo_fname);
        exit(EXIT_FAILURE);
}
 fprintf(stderr,"\nFIFO do Balcao aberto WRITE / BLOCKING");
 //

//abre o FIFO do medico  READ(+Write) Block!!!
 m_fifo_fd = open(m_fifo_fname, O_RDWR);
 if(m_fifo_fd == -1){
 perror("\nErro ao abrir o FIFO do medico");
 close(m_fifo_fd);
 unlink(m_fifo_fname);
 exit(EXIT_FAILURE);
 }

  fprintf(stderr, "\nFIFO do medico aberto para READ(+Write) Block");

  memset(medic.pr, '\0', TAM_MAX);

  printf("Dr/a. %s especialidade: %s\n",medic.mnome,medic.espec);

// B) Envia ao balcao
  write(b_fifo_fd, &medic, sizeof(medic));

// c) recebe do balcao//////////////////////////////////

  read_res = read(m_fifo_fd, &balcao, sizeof(balcao));
  if(read_res == sizeof(balcao))
      printf("\nDoente -> %s sintomas -> %s PID -> %d" , utente.nome, utente.palavra, utente.pid_utent);
    else
      printf("\nSem resposta do balcao" "[bytes lidos: %d\n]", read_res);

   //strcpy(p_fifo_fname,utente.nome);

while(1){
  sprintf(p_fifo_fname, CLIENT_FIFO, utente.pid_utent);

  p_fifo_fd = open(p_fifo_fname, O_WRONLY);

  if(p_fifo_fd == -1)
    perror("\Erro no open - Ninguem quis a resposta");
    else{
      fprintf(stderr, "\nFIFO utente pronto para iniciar a consulta\n");
      }
      scanf("%[^\n]",medic.pr);

      res = write(p_fifo_fd, & medic, sizeof(medic));
      if(res == sizeof(medic))
        fprintf(stderr,"\nescreveu ao utente %s\n",medic.pr);
        else
        perror("\nerro a escrever ao utente");

        res = read(b_fifo_fd, & utente, sizeof(utente));
        if(res < sizeof(utente)){
          fprintf(stderr,"\nRecebida mensagem incompleta " "[bytes lidos: %d]", res);
          continue;
        }

}
close(p_fifo_fd);
fprintf(stderr,"\nFIFO utente fechado\n");
close(m_fifo_fd);
close(b_fifo_fd);
//close(p_fifo_fd);
unlink(m_fifo_fname);
return 0;

}
