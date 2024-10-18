/*Aluno: Nicholas de Araujo Figueira*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM 10 // Tamanho do buffer
#define NTHREADS 3
#define TRUE 1

char buffer1[TAM], buffer2[TAM]; // Buffers para armazenar caracteres entre as threads
int in1 = 0, out1 = 0, in2 = 0, out2 = 0, arq_lido = 0; // Variáveis de controle para os buffers
sem_t entra1, sai1, entra2, sai2; // Semáforos para coordenar acesso aos buffers


//Lê o arquivo e organiza os blocos no buffer 
void* thread1(void* arg) {
    FILE* arquivo = (FILE*) arg;  
    char ch;

    while (TRUE) {
        ch = fgetc(arquivo);
        if (ch == EOF) { //Terminou de ler o arquivo
            arq_lido = 1;  
            sem_post(&sai1);  //Desbloqueia thread 2
            break;
        }

        sem_wait(&entra1);
        buffer1[in1] = ch;
        in1 = (in1 + 1) % TAM;
        sem_post(&sai1);
    }
    fclose(arquivo);
    pthread_exit(NULL);
}

void* thread2() {
    int caracteres = 0; // Caracteres processados
    int n = 0; // Lógica para inserir quebras de linha
    char elem; 

    while (TRUE) {
        sem_wait(&sai1);
        if (arq_lido && in1 == out1){  // Terminou de ler e não há mais caracteres no buffer1.
            sem_post(&sai2);  // Desbloqueia a thread 3.
            break;
        }

        elem = buffer1[out1];
        out1 = (out1 + 1) % TAM;
        sem_post(&entra1);

        sem_wait(&entra2);
        buffer2[in2] = elem;
        in2 = (in2 + 1) % TAM;
        sem_post(&sai2);

        caracteres++;

        // Insere nova linha a cada 2n+1 caracteres até n=11, depois a cada 10 caracteres
        if (caracteres == 2 * n + 1 || (n >= 11 && caracteres == 10)) {
            sem_wait(&entra2);
            buffer2[in2] = '\n';
            in2 = (in2 + 1) % TAM;
            sem_post(&sai2);
            caracteres = 0;

            if (n <= 10) n++;  
        }
    }
    pthread_exit(NULL);
}

void* thread3() {
    while (TRUE) {
        sem_wait(&sai2);
        if (arq_lido && in2 == out2) { // Terminou de ler e não há mais caracteres no buffer1.
            break;
        }
        char elem = buffer2[out2];
        out2 = (out2 + 1) % TAM;
        sem_post(&entra2);
        printf("%c", elem);
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {
    FILE *file;
    pthread_t tid[NTHREADS];

    //Inicia Semáforo
    sem_init(&entra1, 0, 1);
    sem_init(&sai1, 0, 0);
    sem_init(&entra2, 0, 1);
    sem_init(&sai2, 0, 0);


    if (argc < 2) {
        printf("Formato inválido: <nomePrograma> <arquivoEntrada>\n");
        return 1;
    }
    file = fopen(argv[1], "r");
    if (!file) {
        printf("Não foi possível abrir o arquivo");
        return 1;
    }


    if (pthread_create(&tid[0], NULL, thread1, (void*) file)) {
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }
    if (pthread_create(&tid[1], NULL, thread2, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }
    if (pthread_create(&tid[2], NULL, thread3, NULL)) {
        printf("--ERRO: pthread_create()\n"); exit(-1); 
    }

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join()\n"); exit(-1); 
        }
    }

    return 0;
}




