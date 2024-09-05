/* Aluno: Nicholas de Araujo Figueira   DRE: 121088218 */

/*  Programa que lê e multiplica duas matrizes (quadradas ou não) de forma sequencial
    Entrada: nome do arquivo da primeira e segunda matriz
    Saída: Retorna a matriz final resultante do produto 
*/

#include<stdio.h>
#include<stdlib.h>

#define IMPRIME

struct resultado {
   int linhas, colunas;
   float *matriz;
};

// Lê uma matriz de arquivo binário
struct resultado lerMatriz(const char* nomeArquivo) {
    FILE *arquivo;
    size_t ret;
    long long int tam;
    struct resultado resultado = {0, 0, NULL}; 

    arquivo = fopen(nomeArquivo, "rb");
    if(!arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo %s\n", nomeArquivo);
        return resultado;
    }

    ret = fread(&resultado.linhas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return resultado;
    }

    ret = fread(&resultado.colunas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return resultado;
    }

    tam = resultado.linhas * resultado.colunas;
    resultado.matriz = (float*) malloc(sizeof(float) * tam);
    if(!resultado.matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz do arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return resultado;
    }

    ret = fread(resultado.matriz, sizeof(float), tam, arquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz no arquivo %s\n", nomeArquivo);
        free(resultado.matriz);
        resultado.matriz = NULL;  
    }

    fclose(arquivo);
    return resultado;
}

// Imprime a matriz fornecida
void imprimeMatriz(struct resultado matriz) {
    for(int i = 0; i < matriz.linhas; i++) { 
        for(int j = 0; j < matriz.colunas; j++)
            fprintf(stdout, "%.6f ", matriz.matriz[i * matriz.colunas + j]);
        fprintf(stdout, "\n");
    }
}

// Multiplica duas matrizes
struct resultado multiplicaMatrizes(struct resultado matriz1, struct resultado matriz2) {
    struct resultado produto;
    produto.linhas = matriz1.linhas;
    produto.colunas = matriz2.colunas;
    produto.matriz = (float*)malloc(produto.linhas * produto.colunas * sizeof(float));
    
    if (!produto.matriz) {
        fprintf(stderr, "Erro de alocao da memoria para o produto da matriz\n");
        return produto;
    }

    for (int i = 0; i < matriz1.linhas; i++) {
        for (int j = 0; j < matriz2.colunas; j++) {
            produto.matriz[i * produto.colunas + j] = 0.0f;

            for (int k = 0; k < matriz1.colunas; k++) {
                produto.matriz[i * produto.colunas + j] += matriz1.matriz[i * matriz1.colunas + k] * matriz2.matriz[k * matriz2.colunas + j];
            }
        }
    }
    return produto;
}

int main(int argc, char*argv[]) {

    struct resultado matriz1, matriz2, resultado_produto;




    //Lembrar de atualizar o argc para poder ter arquivo de saída para guardar o resultado
    if(argc < 3) {
        fprintf(stderr, "Digite: <arquivoMatriz1> <arquivoMatriz2>\n");
        return 1;
    }



    

    matriz1 = lerMatriz(argv[1]);
    if(!matriz1.matriz) {
        return 2;
    }

    matriz2 = lerMatriz(argv[2]);
    if(!matriz2.matriz) {
        return 2;
    }

    // Verifica se as matrizes podem ser multiplicadas
    if(matriz1.colunas != matriz2.linhas) {
        fprintf(stderr, "Erro: o numero de colunas da primeira matriz deve ser igual ao numero de linhas da segunda matriz.\n");
        free(matriz1.matriz);
        free(matriz2.matriz);
        return 3;
    }

    #ifdef IMPRIME
    imprimeMatriz(matriz1);
    printf("\n\n");
    imprimeMatriz(matriz2);
    #endif 

    resultado_produto = multiplicaMatrizes(matriz1, matriz2);
    if (!resultado_produto.matriz) {
        free(matriz1.matriz);
        free(matriz2.matriz);
        return 4;
    }

    printf("\nProduto das matrizes:\n");
    imprimeMatriz(resultado_produto);

    free(matriz1.matriz);   
    free(matriz2.matriz);
    free(resultado_produto.matriz);

    return 0;   
}


/*
//Abre os arquivos passados e lê a matriz
float* leMatriz(const char* nomeArquivo, int *linhas, int *colunas) {
    FILE *arquivo;
    size_t ret;
    long long int tam;
    float *matriz;

    arquivo = fopen(nomeArquivo, "rb");
    if(!arquivo) {
        fprintf(stderr, "Erro de abertura do arquivo %s\n", nomeArquivo);
        return NULL;
    }

    ret = fread(linhas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return NULL;
    }

    ret = fread(colunas, sizeof(int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz no arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return NULL;
    }

    tam = (*linhas) * (*colunas);
    matriz = (float*) malloc(sizeof(float) * tam);
    if(!matriz) {
        fprintf(stderr, "Erro de alocao da memoria da matriz do arquivo %s\n", nomeArquivo);
        fclose(arquivo);
        return NULL;
    }

    ret = fread(matriz, sizeof(float), tam, arquivo);
    if(ret < tam) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz no arquivo %s\n", nomeArquivo);
        free(matriz);
        fclose(arquivo);
        return NULL;
    }

    fclose(arquivo);
    return matriz;
}
*/

/*
// Imprime a matriz fornecida
void imprimeMatriz(float matriz, int linhas, int colunas) {
    for(int i = 0; i < linhas; i++) { 
        for(int j = 0; j < colunas; j++)
            fprintf(stdout, "%.6f ", matriz[i * colunas + j]);
        fprintf(stdout, "\n");
    }
}
*/

/*
float* multiplicaMatriz(float* matriz1, float* matriz2, int linhas1, int colunas2) {
    float *resultado[linhas1][colunas2];

        for (int i = 0; i < linhas1; i++) {
            for (int j = 0; j < colunas2; j++) {
                resultado[i][j] = 0;
 
                for (int k = 0; k < R2; k++) {
                    resultado[i][j] += m1[i][k] * m2[k][j];
                }
 
            printf("%d\t", result[i][j]);
        }
 
        printf("\n");
    }
}
*/