/* Aluno: Nicholas de Araujo Figueira   DRE: 121088218 */

/*  Programa que gera e  multiplica duas matrizes (quadradas ou não) de forma concorrente
    Entrada: nome do arquivo da primeira e segunda matriz
    Saída: Retorna a matriz final resultante do produto 
*/

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include "timer.h"

#define IMPRIME

float *mat; //matriz de entrada
float *vet; //vetor de entrada
float *saida; //vetor de saida
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

struct matriz {
   int id, linhas, colunas;
   float *matriz;
};

// Lê uma matriz de arquivo binário
struct matriz lerMatriz(const char* nomeArquivo) {
    FILE *arquivo;
    size_t ret;
    long long int tam;
    struct matriz matriz = {0, 0, 0, NULL}; 

    arquivo = fopen(nomeArquivo, "rb");
    if(!arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo %s\n", nomeArquivo);
        return matriz;
    }

    ret = fread(&matriz.linhas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return matriz;
    }

    ret = fread(&matriz.colunas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return matriz;
    }

    tam = matriz.linhas * matriz.colunas;
    matriz.matriz = (float*) malloc(sizeof(float) * tam);
    if(!matriz.matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz do arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return matriz;
    }

    ret = fread(matriz.matriz, sizeof(float), tam, arquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz no arquivo %s\n", nomeArquivo);
        free(matriz.matriz);
        matriz.matriz = NULL;  
    }

    fclose(arquivo);
    return matriz;
}

//funcao que as threads executarao
void * multiplicar(void *arg) {
   tArgs *args = (tArgs*) arg;
   for(int i=args->id; i<args->dim; i+=nthreads)
      for(int j=0; j<args->dim; j++) 
         saida[i] += mat[i*(args->dim) + j] * vet[j];
   pthread_exit(NULL);
}


int main (int argc, char *argv[]) {
    int dim; //dimensao da matriz de entrada
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao
    struct matriz matriz1, matriz2, resultado;
    double inicio, fim, delta;

    if(argc < 4) {
      printf("Digite: <arquivoMatriz1> <arquivoMatriz2> <arquivoResultado> <numero de threads>\n");
      return 1;
   }

    matriz1 = lerMatriz(argv[1]);
    if(!matriz1.matriz) {
        return 2;
    }

    matriz2 = lerMatriz(argv[2]);
    if(!matriz2.matriz) {
        free(matriz1.matriz);
        return 2;
    }

    // Verifica se as matrizes podem ser multiplicadas
    if(matriz1.colunas != matriz2.linhas) {
        fprintf(stderr, "Erro: o numero de colunas da primeira matriz deve ser igual ao numero de linhas da segunda matriz.\n");
        free(matriz1.matriz);
        free(matriz2.matriz);
        return 3;
    }

    nthreads = atoi(argv[4]);



    resultado.matriz = (float*)malloc(matriz1.linhas * matriz2.colunas * sizeof(float));

    if (!resultado.matriz) {
        fprintf(stderr, "Erro de alocao da memoria para o produto da matriz\n");
        return 3;
    }

    for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++)
            mat[i*dim+j] = 1;    //equivalente mat[i][j]
        vet[i] = 1; 
        saida[i] = 0;
    }


    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL) {
        puts("ERRO--malloc"); 
        return 2;
    }
    //criacao das threads
    for(int i = 0; i < nthreads; i++) {
        (args + i)->id = i;
        (args + i)->dim = dim;
        if(pthread_create(tid+i, NULL, multiplicar, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
      }
   } 

    //espera pelo termino da threads
    for(int i=0; i<nthreads; i++) {
        pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim)   
    delta = fim - inicio;
    printf("Tempo multiplicacao (dimensao %d) (nthreads %d): %lf\n", dim, nthreads, delta);

    //liberacao da memoria
    GET_TIME(inicio);
    free(mat);
    free(vet);
    free(saida);
    free(args);
    free(tid);
    GET_TIME(fim)   
    delta = fim - inicio;
    printf("Tempo finalizacao:%lf\n", delta);

   return 0;
}
