#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <malloc.h>
#include <pthread.h>

#define NUM_THREADS 1 /* define o numero de threads */
#define ROWS 10
#define COLS 10

/* variaveis global */
int matriz[i][j]
int contPrimosTotal = 0;

/* funcoes globais */
int ehPrimo(int numero);
int sondagem(macrobloco[i][j]);

int** Alocar_matriz_real() {
    int** p;
    int i;

    if (ROWS < 1 || COLS < 1) {
        printf("Parametros invalidos");
        return (NULL);
    }

    p = calloc(ROWS, sizeof(int*));
    if (p == NULL) {
        printf("ERROR, MEMORIA INSUFICIENTE");
        exit(1);
    }

    for (i = 0; i < ROWS; i++) {
        p[i] = calloc(COLS, sizeof(int));
        if (p[i] == NULL) {
            printf("** ERROR, Memoria Insuficiente **");
            exit(1);
        }
    }

    return (p);

    /*Matriz* create(ROWS, COLS);
    int** matriz = (int**)malloc(ROWS * sizeof(int*));
    for (int i = 0; i < COLS; i++)
        matriz[i] = (int*)malloc(COLS * sizeof(int));

    return matriz;*/
}

int** Liberar_matriz_real(int** matriz) {
    int i;

    if (matriz == NULL) return (NULL);

    if (ROWS < 1 || COLS < 1) {
        printf("** Erro: Parametro invalido **");
        return (matriz);
    }

    for (i = 0; i < ROWS; i++) free(matriz[i]);

    free(matriz);

    return (NULL);

}

int main() {
    int contPrimos = 0;
    time_t t;
    int **matriz;

    matriz = Alocar_matriz_real(ROWS, COLS); //Alocando matriz

    t = time(NULL);
    srand(time(0));

    for (int i = 0; i < ROWS; i++) { //Preenchendo a matriz
        for (int j = 0; j < COLS; j++) {
            matriz[i][j] = rand() % 32000;
        }
    }

    /* criação das threads (também num loop for)*/
    /* CRIACAO DOS MACROBLOCOS */
    for (int i = 1; i <= NUM_THREADS; i++){
    	pthread_t tid; /* o identificador da thread */
    	pthread_attr_t attr; /* os atributos da thread */
    	pthread_attr_init(&attr); /* ajusta os atributos padrao da thread */
    	pthread_create(&tid, &attr, sondagem, argv[1]); /* cria a thread */
    }
    
    printf("Primos encontrados %d \n", contPrimos);


    return 0;
}

/* FUNCAO DE SONDAGEM */

int sondagem(int macrobloco){
        for (int i = 0; i < ROWS; i++) { // sonda o macrobloco //
            for (int j = 0; j < COLS; j++) {
                contPrimos += ehPrimo(macrobloco[i][j]);
            }
        }
return contPrimos;

/**********************FUNCAO EHPRIMO***********************/

int ehPrimo(int numero) {
    int cont = 0;
    int raiz = sqrt(numero);
    for (int i = 0; i <= raiz; i++) {
        if (numero % i == 0) {
            return 1;
        }
    }

    /*if (cont == 1) {
        printf("O numero %d eh primo ", &numero);
        printf("");
        return 1;
    }*/

    return 0;
}
