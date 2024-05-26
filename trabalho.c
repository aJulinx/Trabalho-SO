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

// Define o tamanho da matriz
#define LINHAS 10000
#define COLUNAS 10000
// Semente para geracao dos numeros aleatorios
#define MAX_RANDOM 31999
// Define o tamanho dos macroblocos
#define TAM_MACRO 1
// Define a quantidade de threads
#define NUM_THREADS 6

// Estrutura da matriz, contem o ponteiro para a matriz e outras informações
typedef struct {
    int** pont;
    int proximo_macrobloco;
} Matriz;

// Variáveis Globais
Matriz* mat;
pthread_mutex_t mutex;
int totalprimos;


// Função que aloca a matriz seguindo o livro do professor
Matriz* alocar_matriz(int m, int n) {
    // Cria a struct
    Matriz* mat = malloc(sizeof(Matriz));
    // Verifica se alocou
    if (mat == NULL) {
        printf("** Erro: Memoria insuficiente **");
        return NULL;
    }
    // Verifica se os parâmetros são válidos
    if (m < 1 || n < 1) {
        printf("** Erro: Parametro invalido **\n");
        return NULL;
    }
    // Aloca as linhas da matriz
    mat->pont = calloc(m, sizeof(int*));
    if (mat->pont == NULL) {
        printf("** Erro: Memoria insuficiente **\n");
        return NULL;
    }
    for (int i = 0; i < m; i++) {
        mat->pont[i] = calloc(n, sizeof(int)); // m vetores de n int
        if (mat->pont[i] == NULL) {
            printf("** Erro: Memoria insuficiente **\n");
            return NULL;
        }
    }

    return mat; // Retorna o ponteiro para a matriz
}

// Libera a matriz
void liberar_matriz_real(int m, int n, Matriz* mat) {
    if (mat == NULL) {
        return;
    }
    if (m < 1 || n < 1) {
        printf("** Erro: Parametro invalido **\n");
        return;
    }
    for (int i = 0; i < m; i++) {
        free(mat->pont[i]); // Libera as linhas da matriz
    }
    free(mat); // Libera a matriz
}

// Função para verificar se é primo, seguindo as dicas do professor
int ehPrimo(int numero) {
    int raiz = sqrt(numero);
    if (numero <= 1) return 0;
    if (numero == 2) return 1;
    if (numero % 2 == 0) return 0;
    for (int i = 3; i <= raiz; i += 2) {
        if (numero % i == 0) {
            return 0;
        }
    }
    return 1;
}

// Função para preencher a matriz com números aleatórios
void preencher_matriz(Matriz* mat) {
    srand(time(NULL));
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mat->pont[i][j] = rand() % (MAX_RANDOM);
        }
    }
}

// Busca Serial, ela é tão simples assim mesmo ?
void buscaSerial(Matriz* mat) {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            totalprimos += ehPrimo(mat->pont[i][j]);
        }
    }
}

// Função de busca paralela
void* buscaParalela(void* param) {
    //Variaveis auxiliares, criei elas para faciltiar a manipulação das threads, ao utilizar diretamente os elementos da struct, a função não funciona do jeito certo
    int primos;
    int linha, coluna, fim_linha, fim_coluna;

    //De inicio havia criado uma variavel na struc que serviria para isso, decidi usar algo que os professores odeiam, break
    //o motivo foi que, ao utilziar a variavel na struct para isso, acabei tendo muito trabalho e ainda sim erros, algumas threads finalizavam e marcavam como concluido
    //fazendo necessario o uso de mutex, como o abaixo
    while (1) {
        //Mutex para identificar se ainda há elementos na matriz para percorrer, é a variável de controle do while
        pthread_mutex_lock(&mutex);
        if (mat->proximo_macrobloco >= (LINHAS / TAM_MACRO) * (COLUNAS / TAM_MACRO)) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        //Adaptei para o novo while, antes esse mutex verificava a variavel da struc que marcava a matriz como percorrida
        //o motivo do mutex foi justamente para controlar as threads e nao deixar elas marcarem como concluido antes do fim

        //Variavei para guiar as threads para o proximo macrobloco, mutex nela para garantir que as threads nao se confundam
        int bloco_atual = mat->proximo_macrobloco++;
        pthread_mutex_unlock(&mutex);

        //Parte mais importante, ela define os macroblocos para a thread percorrer, linha por linha e coluna por coluna.        
        //Calcula o numero de blocos por linha, encontra em qual linha da matriz esta o bloco, e encontra o indice de inicio do macrobloco
        linha = (bloco_atual / (COLUNAS / TAM_MACRO)) * TAM_MACRO;
        coluna = (bloco_atual % (COLUNAS / TAM_MACRO)) * TAM_MACRO;
        //Determinam em qual elemento da matriz é o fim do macrobloco atual
        fim_linha = linha + TAM_MACRO;
        fim_coluna = coluna + TAM_MACRO;

        //Reseta o contador de primos
        primos = 0;

        //Busca serial basicamente
        for (int i = linha; i < fim_linha && i < LINHAS; i++) {
            for (int j = coluna; j < fim_coluna && j < COLUNAS; j++) {
                if (ehPrimo(mat->pont[i][j])) {
                    primos++;
                }
            }
        }

        //Mutex para acrescentar os primos encontrados pelas threads a variavel global
        pthread_mutex_lock(&mutex);
        totalprimos += primos;
        pthread_mutex_unlock(&mutex);
    }
    //Termina a thread
    pthread_exit(0);
}


int main(int argc, char* argv[]) {
    //Double para o tempo ser mais preciso
    double tempo;
    double tempo2;
    //Criação das threads e o mutex
    pthread_t workers[NUM_THREADS];
    pthread_mutex_init(&mutex, NULL);

    //Aloca a matriz e a preencha
    mat = alocar_matriz(LINHAS, COLUNAS);
    preencher_matriz(mat);

    //Define o macrobloco 
    mat->proximo_macrobloco = 0;


    //Medição do tempo de busca
    //Essa parte procurei na internet para saber como fazer a medição
    clock_t iniciar_cronometro = clock();
    buscaSerial(mat);
    clock_t finalizar_cronometro = clock();
    tempo = (double)(finalizar_cronometro - iniciar_cronometro) / CLOCKS_PER_SEC;

    printf("Primos encontrados : %d\n", totalprimos);
    printf("Busca Serial em : %f segundos\n", tempo);

    //Reset totalprimos para busca paralela
    totalprimos = 0;
    mat->proximo_macrobloco = 0; // Reset para a busca paralela

    // Medir o tempo da busca paralela
    clock_t iniciar_cronometro2 = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&workers[i], NULL, buscaParalela,NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    clock_t finalizar_cronometro2 = clock();
    tempo2 = (double)(finalizar_cronometro2 - iniciar_cronometro2) / CLOCKS_PER_SEC;

    printf("Primos encontrados : %d\n", totalprimos);
    printf("Busca Paralela em : %f segundos\n", tempo2);

    //Liberação da memoria
    liberar_matriz_real(LINHAS, COLUNAS, mat);
    pthread_mutex_destroy(&mutex);

    return 0;
}
