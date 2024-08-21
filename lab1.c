/* Aluno: Nicholas de Araujo Figueira DRE: 121088218 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Definindo o tipo Vector com os atributos necessários.
//idThread é desnecessário, mas deixei por facilitar testes caso sejam necessários
typedef struct {
   int nElements, *array, idThread; 
} Vector;

//Função responsável por realizar a soma de cada fragmento de vetor
void *SumVector (void *vec) {
    Vector vector = *(Vector*) vec;

    for(int i = 0; i < vector.nElements; i++){
        vector.array[i] += 1;
    }

    free(vec);
    pthread_exit(NULL);
};

int main(int argc, char* argv[]) {
    Vector *vector;
    int nthreads, size;
   
    if(argc < 3) { 
      printf("--ERRO: informe a qtd de threads e o tamanho do array, respectivamente\n"); 
      return 1;
    }
    nthreads = atoi(argv[1]);       
    size = atoi(argv[2]);

    if (nthreads > size) {
        printf("Numero de threads passado %i eh maior do que a quantidade de elementos %i \n", nthreads, size);
        return 1;
    }

    pthread_t tid_sistema[nthreads];
    int arr[size];

    //Definindo o array inicial
    for(int i = 0; i < size; i++) {
        arr[i] = i + 10;
    }

    //Começa-se a separar quantos o número elementos por thread 
    //para saber quanto cada uma será responsável
    int elementsPerThread = size / nthreads;
    int remainingElements = size % nthreads;
    int start = 0;

    for(int i = 0; i < nthreads; i++) {

        vector = malloc(sizeof(Vector));
        if (vector == NULL) {
            printf("--ERRO: malloc()\n"); 
            return 1;
        } 
        vector->idThread = i + 1; 
        vector->nElements = elementsPerThread; 
        //Deslocamento para apontar para a próxima posição a fim de evitar sobreposição
        vector->array = arr + i * elementsPerThread;

        //Essa parte faz o melhor balanceamento de elementos por thread para assegurar um equilíbrio
        if (remainingElements > 0) { 
            vector->nElements += 1;
            remainingElements -= 1;
        }
        vector->array = arr + start;
        start += vector->nElements; //Serve para apontar para o início da próxima thread

        //Cria as threads chamando a função SumVector
        if (pthread_create(&tid_sistema[i], NULL, SumVector, vector)) {
            printf("--ERRO: pthread_create() da thread %d\n", i); 
        }
    }
    printf("Threads Criadas\n");

    //Espera todas as threads terminarem
    for (int i = 0; i < nthreads; i++) {
        if (pthread_join(tid_sistema[i], NULL)) {
            printf("--ERRO: pthread_join() da thread %d\n", i); 
        } 
    }
    printf("--Thread principal terminou\n");

    //Pega o array final gerado e compara com os valores de i incrementados 11 
    //para verificarmos se a soma foi aplicada corretamente
    int isCorrect = 1; 
    for (int i = 0; i < size; i++) {
        if (arr[i] != i + 11) { 
            isCorrect = 0;
            printf("Erro na posicao %i: valor esperado %i, valor encontrado %i\n", i, i + 11, arr[i]);
            return 1;
        }
    }
    if (isCorrect) {
        printf("Todos os valores do array estao corretos!\n");
    } else {
        printf("Existem erros no array final.\n");
    }
};