//Aluno: Nicholas de Araujo Figueira

//Programa concorrente que cria e faz operacoes sobre uma lista de inteiros
//Usando padrão leitor e consumidor com locks com prioridade para escrita e logs

#include <stdio.h>
#include <stdlib.h>
#include "list_int.c"
#include <pthread.h>
#include "timer.h"

#define QTDE_OPS 100 //quantidade de operacoes sobre a lista (insercao, remocao, consulta)
#define QTDE_INI 100 //quantidade de insercoes iniciais na lista
#define MAX_VALUE 100 //valor maximo a ser inserido
#define IMPRIME

//lista compartilhada iniciada 
struct list_node_s* head_p = NULL; 

//escr_pendente controla o número de escritores para impedir leituras antes de escritas
int nthreads, leit = 0, escr = 0, escr_pendente = 0;

//Mutex e condicionais
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;





//Função de entrada de consumidor
void entraLeitura() {
    pthread_mutex_lock(&mutex);
    #ifdef IMPRIME
    printf("Leitura começou \n");
    #endif
    while ( (escr > 0) || (escr_pendente > 0) ) {
        escr_pendente > 0 ? printf("Não é possível ler, pois há %d escritores na fila \n", escr_pendente) : 
        printf("Não é possível ler, pois há %d escritores à frente \n", escr);
        pthread_cond_wait(&cond_leit, &mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

//Função saída do consumidor
void saiLeitura() {
    pthread_mutex_lock(&mutex);
    leit--;
    #ifdef IMPRIME
    printf("Leitura concluída \n");
    #endif
    if (leit == 0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

//Função entrada de produtor
void entraEscrita() {
    pthread_mutex_lock(&mutex);
    escr_pendente++;
    #ifdef IMPRIME
    printf("Escrita começou \n");
    #endif
    while( (leit > 0) || (escr > 0) ) {
        escr > 0 ? printf("Não é possível escrever, pois há %d escritores à frente \n", escr) : 
        printf("Não é possível escrever, pois há %d leitores à frente \n", leit);
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escr_pendente--;
    escr++;
    pthread_mutex_unlock(&mutex);
}

//Função de saída de produtor
void saiEscrita () {
    pthread_mutex_lock(&mutex);
    escr--;
    #ifdef IMPRIME
    printf("Escrita concluída \n");
    #endif
    //Se não falta escrever ninguém, libera-se leitra. Caso contrário, precisa-se escrever o faltante
    if (escr_pendente == 0) {
        pthread_cond_broadcast(&cond_leit);
    } else {
        pthread_cond_signal(&cond_escr);
    }
    pthread_mutex_unlock(&mutex);
}









//tarefa das threads
void* tarefa(void* arg) {
   long int id = (long int) arg;
   int op;
   int in, out, read; 
   in=out=read = 0; 

   //realiza operacoes de consulta (98%), insercao (1%) e remocao (1%)
   for(long int i=id; i<QTDE_OPS; i+=nthreads) {
      op = rand() % 100;
      //Passa-se a trocar os locks anteriores para o padrão com prioridade pedida
      if(op<98) {
	    entraLeitura();  
            Member(i%MAX_VALUE, head_p);   /* Ignore return value */
	    saiLeitura();    
	    read++;
      } else if(98<=op && op<99) {
	    entraEscrita();
            Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
	    saiEscrita();  
	    in++;
      } else if(op>=99) {
	    entraEscrita(); 
            Delete(i%MAX_VALUE, &head_p);  /* Ignore return value */
	    saiEscrita(); 
	    out++;
      }
   }
   //registra a qtde de operacoes realizadas por tipo
   printf("Thread %ld: in=%d out=%d read=%d\n", id, in, out, read);
   pthread_exit(NULL);
}

/*-----------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   pthread_t *tid;
   double ini, fim, delta;
   
   //verifica se o numero de threads foi passado na linha de comando
   if(argc<2) {
      printf("Digite: %s <numero de threads>\n", argv[0]); return 1;
   }
   nthreads = atoi(argv[1]);

   //insere os primeiros elementos na lista
   for(int i=0; i<QTDE_INI; i++)
      Insert(i%MAX_VALUE, &head_p);  /* Ignore return value */
   

   //aloca espaco de memoria para o vetor de identificadores de threads no sistema
   tid = malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {  
      printf("--ERRO: malloc()\n"); return 2;
   }

    //inicializa a variavel mutex
    pthread_mutex_init(&mutex, NULL);

    //inicializa as variáveis condicionais
    pthread_cond_init(&cond_leit, NULL);
    pthread_cond_init(&cond_escr, NULL);    

   //tomada de tempo inicial
   GET_TIME(ini);
   
   //cria as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)) {
         printf("--ERRO: pthread_create()\n"); return 3;
      }
   }
   
   //aguarda as threads terminarem
   for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL)) {
         printf("--ERRO: pthread_join()\n"); return 4;
      }
   }

   //tomada de tempo final
   GET_TIME(fim);
   delta = fim-ini;
   printf("Tempo: %lf\n", delta);


   
    //--finaliza o mutex e as condicionais
    pthread_cond_destroy(&cond_leit);
    pthread_cond_destroy(&cond_escr);
    pthread_mutex_destroy(&mutex);

    //libera o espaco de memoria do vetor de threads
    free(tid);
    //libera o espaco de memoria da lista
    Free_list(&head_p);

    return 0;
}  /* main */