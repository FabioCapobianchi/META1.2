#include "comuns.h"

int Verifica_cliente(utent c1, char cname[])
{
      utent aux;
      aux=c1;

    while(aux != NULL)
    {
        if(strcmp(aux->nome,cname)==0)
        {
            return 0;
        }
        aux = aux->next;
    }

    return 1;
}

utent criaCliente(utent c1, utent_t copia)
   {
	   utent novo;
	   novo=malloc(sizeof(utent_t));

 if(novo==NULL)
	 {
	  return NULL;
	 }else{
		   novo->pid_utent = copia.pid_utent;
		   strcpy(novo->nome, copia.nome);
		   strcpy(novo->palavra, copia.palavra);
		   novo->next= NULL;

    if(c1==NULL)
        c1 = novo;
    else
	   { novo->next = c1;
      c1 = novo;
     }
   }
	   if(c1!=NULL)
	   fprintf(stderr,"\nCliente %s gravado\n", novo->nome);

   return c1;
}
