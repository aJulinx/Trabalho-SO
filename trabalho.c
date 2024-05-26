#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//Define o tamanho da matriz
#define LINHAS 9
#define COLUNAS 9
//Semente para geracao dos numeros aleatorios
#define MAX_RANDOM 31999
//Define o tamanho dos macroblocos
#define TAM_MACRO 3
//Define a quantitdade de threads
#define NUM_THREADS 100



//Estruct da matriz, contem o ponteiro para a matriz, informações que ajudam a localizar os macroblocos
typedef struct {
    int **pont;
    int linha;
    int fim_linha;
    int inicio_coluna;
    int fim_coluna;
    int fim_matriz;
    int cont;
}Matriz;


//Variaveis Globais
Matriz* mat;
int totalprimos;
pthread_mutex_t mutex;

//Seguindo o roteiro
void *buscaParalela(void* param);

//Funcao que aloca a matriz, obrigado Giraldeli
Matriz* alocar_matriz(int m, int n) {
    //Cria a struct
    Matriz* mat = malloc(sizeof(Matriz));
    
    //Verifica se alocou
    if (mat == NULL) {
        printf("** Erro: Deu erro, faz parte");
        return NULL;
    }
    //Verifica se os parametros sao validos
    if (m < 1 || n < 1) {
        //verifica se é valido /
        printf(" Erro: Parametro invalido \n");
        return (NULL);
    }

    // aloca as linhas da matriz /
    mat->pont = calloc(m, sizeof(int*));
    if (mat->pont == NULL) {
        printf(" Erro: Memoria insuficiente");
        return(NULL);
    }
    for (int i = 0; i < m; i++) {
        mat->pont[i] = calloc(n, sizeof(int)); // m vetores de n int /
        if (mat->pont[i] == NULL) {
            printf(" Erro: Memoria insuficiente");
            return (NULL);
        }
    }

    mat->inicio_coluna = 0;
    mat->fim_coluna = TAM_MACRO;
    mat->fim_matriz = 0;

    return (mat); // retorna o ponteiro para matriz /

}

//Libera a matriz, obrigado Giraldeli²
void liberar_matriz_real(int m, int n, Matriz *mat) {

    if (mat == NULL) {
        return (NULL);
    }

    if (m < 1 || n < 1) {
        // verifica se é valido /
        printf(" Erro: Parametro invalido \n");
        return (NULL);
    }

    for (int i = 0; i < m; i++) {
        free(mat->pont[i]); // libera as linhas da matriz /
    }

    free(mat); // libera a matriz /

    return (NULL);
}

//Funcao para verificar se eh primo, obrigado Giraldeli³
int ehPrimo(int numero) {
    int raiz = sqrt(numero);

    if (numero <= 1) return 0;
    if (numero == 2) return 1;
    if (numero % 2 == 0) return 0;

    for (int i = 3; i <= raiz; i+=2) {
        if (numero % i == 0) {
            return 0;
        }
    }

    return 1;
}

//Funcao para preencher a matriz com os numeros aleatorios
void preencher_matriz(Matriz *mat) {
    srand(time(NULL));
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mat->pont[i][j] = rand() % (MAX_RANDOM);
        }
    }
}



//Busca Serial, ira percorrer macrobloco por macroblo, chamando a funcao de primos
void buscaSerial(Matriz* mat) {
    //Nao sei por que, mas tentei fazer isso na funcao que cria a matriz, mas nao gerava o resultado que eu espera
    //entao fiz aqui dentro e deu certo, eu so aceitei e deixei
    //Seta o tamanho do macrobloco atual
    /*mat->linha = 0;
    mat->fim_linha = TAM_MACRO;

    //Enquanto todas as linhas da matriz nao forem percorridos
    while (mat->linha < LINHAS) {
        //Testei do lado de fora e o resultado nao foi certo, novamente nao faco ideia do que fiz de errado, mas aqui dentro da certo
        mat->inicio_coluna = 0;
        mat->fim_coluna = TAM_MACRO;
        //Como as colunas dos macroblocos variam mais
        while (mat->inicio_coluna < COLUNAS) {
            for (int i = mat->linha; i < mat->fim_linha && i < LINHAS; i++) {
                for (int j = mat->inicio_coluna; j < mat->fim_coluna && j < COLUNAS; j++) {
                    totalprimos += ehPrimo(mat->pont[i][j]);                
                }
                
            }       
            mat->inicio_coluna += TAM_MACRO;
            mat->fim_coluna += TAM_MACRO;
        }
        mat->linha += TAM_MACRO;
        mat->fim_linha += TAM_MACRO;
    }*/

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            totalprimos += ehPrimo(mat->pont[i][j]);
        }
    }
}

void *buscaParalela(void *param) {
    int primos;

    //Enquanto todas as linhas da matriz nao forem percorridos
    while (mat->linha < LINHAS) {
        //Testei do lado de fora e o resultado nao foi certo, novamente nao faco ideia do que fiz de errado, mas aqui dentro da certo
        mat->inicio_coluna = 0;
        mat->fim_coluna = TAM_MACRO;
        //Como as colunas dos macroblocos variam mais
        while (mat->inicio_coluna < COLUNAS) {
            for (int i = mat->linha; i < mat->fim_linha && i < LINHAS; i++) {
                for (int j = mat->inicio_coluna; j < mat->fim_coluna && j < COLUNAS; j++) {                                      
                    pthread_mutex_lock(&mutex);
                    primos = ehPrimo(mat->pont[i][j]);
                    totalprimos += primos;
                    pthread_mutex_unlock(&mutex);
                    
                }

            }
            //pthread_mutex_lock(&mutex);
            mat->inicio_coluna += TAM_MACRO;
            mat->fim_coluna += TAM_MACRO;
            //pthread_mutex_unlock(&mutex);
        }
       // pthread_mutex_lock(&mutex);
        mat->linha += TAM_MACRO;
        mat->fim_linha += TAM_MACRO;
       // pthread_mutex_unlock(&mutex);
    }

}

int main(int argc, char* argv[]) {

    pthread_t workers[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    mat = alocar_matriz(LINHAS, COLUNAS);
    preencher_matriz(mat);
    
    mat->cont = 0;
    mat->linha = 0;
    mat->fim_linha = TAM_MACRO;


    buscaSerial(mat);

    printf("Total primos encontrados %d \n", totalprimos);

    totalprimos = 0;


    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&workers[i], NULL, buscaParalela, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }

    

    
    printf("Total de primos encontrados nas threads %d", totalprimos);

    
    return 0;
}
