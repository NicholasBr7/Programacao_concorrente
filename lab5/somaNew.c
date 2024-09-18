/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Comunicação entre threads usando variável compartilhada e exclusao mutua com bloqueio */

/* Aluno: Nicholas de Araujo Figueira 
DRE: 121088218 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TOTAL 20

long int soma = 0;
int imprimirMultiplo, contador; // variavel compartilhada entre as threads
pthread_mutex_t mutex; // variavel de lock para exclusao mutua
pthread_cond_t cond, cond_extra;

// funcao executada pelas threads
void *ExecutaTarefa(void *arg){
    long int id = (long int)arg;
    printf("Thread : %ld esta executando...\n", id);

    for (int i = 0; i < 100000; i++) {

        pthread_mutex_lock(&mutex);
    
        //0 é o primeiro múltiplo e quero evitá-lo
        if ((soma > 0 && soma % 10 == 0) && contador < TOTAL) {
            imprimirMultiplo = 1; //vai passar a permitir que a thread extra imprima
            pthread_cond_signal(&cond_extra);
        } while (imprimirMultiplo) {
            pthread_cond_wait(&cond, &mutex);
        }
        soma++;
        pthread_mutex_unlock(&mutex);
    }
    printf("Thread : %ld terminou!\n", id);
    pthread_exit(NULL);
}

// funcao executada pela thread de log
void *extra(void *args){
    printf("Extra : esta executando...\n");

    pthread_mutex_lock(&mutex);
    while (contador < TOTAL) {
        if (imprimirMultiplo) { 
            contador++; 
            printf("Contador (%d) soma = %ld \n", contador , soma);
            imprimirMultiplo = 0; //Reseta a flag de impressão para os futuros múltiplos
            pthread_cond_broadcast(&cond);
        } else { //Nesse caso pula e fica esperando sinal
            pthread_cond_wait(&cond_extra, &mutex);
        }
    }
    pthread_mutex_unlock(&mutex);
    printf("Extra : terminou!\n");
    pthread_exit(NULL);
}

// fluxo principal
int main(int argc, char *argv[])
{
  pthread_t *tid; // identificadores das threads no sistema
  int nthreads;   // qtde de threads (passada linha de comando)

  //--le e avalia os parametros de entrada
  if (argc < 2) {
    printf("Digite: %s <numero de threads>\n", argv[0]);
    return 1;
  }
  nthreads = atoi(argv[1]);

  //--aloca as estruturas
  tid = (pthread_t *)malloc(sizeof(pthread_t) * (nthreads + 1));
  if (tid == NULL) {
    puts("ERRO--malloc");
    return 2;
  }

  //--inicilaiza o mutex e as condicionais
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);
  pthread_cond_init(&cond_extra, NULL);

  //--cria as threads
  for (long int t = 0; t < nthreads; t++) {
    if (pthread_create(&tid[t], NULL, ExecutaTarefa, (void *)t)) {
      printf("--ERRO: pthread_create()\n");
      exit(-1);
    }
  }

  //--cria thread de log
  if (pthread_create(&tid[nthreads], NULL, extra, NULL)) {
    printf("--ERRO: pthread_create()\n");
    exit(-1);
  }

  //--espera todas as threads terminarem
  for (int t = 0; t < nthreads + 1; t++) {
    if (pthread_join(tid[t], NULL))
    {
      printf("--ERRO: pthread_join() \n");
      exit(-1);
    }
  }

  //--finaliza o mutex e as condicionais
  pthread_cond_destroy(&cond);
  pthread_cond_destroy(&cond_extra);
  pthread_mutex_destroy(&mutex);

  printf("Valor de 'soma' = %ld\n", soma);

  return 0;
}
