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
#define NUM_ELEMS ((ROWS*COLS)/NUM_THREADS)

/* variaveis global */
int matriz[i][j]
int contPrimosTotal = 0;

/* funcoes globais */
int ehPrimo(int numero);
int sondagem(matriz[i]);

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

/*************************************FUNCAO DE SONDAGEM*****************************************/

int sondagem(int macrobloco){
        for (int i = 0; i < ROWS; i++) { // sonda o macrobloco //
            for (int j = 0; j < COLS; j++) {
                contPrimos += ehPrimo(macrobloco[i][j]);
            }
        }
        pthread_exit(0);
}

/**************************************************FUNÇÃO MAIN()***********************************************************/

int main() {
    int contPrimos = 0;
    time_t t;
    int **matriz;

    matriz = Alocar_matriz_real(NUM_THREADS, NUM_ELEMS); //Alocando matriz

    t = time(NULL);
    srand(time(0));

    pthread_t workers[NUM_THREADS]; /* um array de threads a serem unidas */

    /**************MATRIZES E MACROBLOCOS**************/

    for (int i = 0; i < NUM_THREADS; i++){ // A matriz já é o conjunto de macroblocos //
    	for (int j = 0; j < NUM_ELEMS; j++){
    		matriz[i][j] = rand() % 32000;
    	pthread_t (i+1); /* o identificador da thread */
        pthread_attr_t attr; /* os atributos da thread */
        pthread_attr_init(&attr); /* ajusta os atributos padrao da thread */
        pthread_create(&(i+1), &attr, sondagem, argv[1]); /* cria a thread */
        pthread_join((i+1), NULL); /* espera pelo termino da thread */
        pthread_join(workers[i], NULL);
    }

    /*************DANDO UM FIM NAS THREADS*************/
 
    printf("Primos encontrados %d \n", contPrimos);


    return 0;
}
