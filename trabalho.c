#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define LINHAS 10000
#define COLUNAS 10000
#define MAX_RANDOM 31999
#define TAM_MACRO 1

typedef struct {
    int **pont;
    int linha;
    int fim_linha;
    int inicio_coluna;
    int fim_coluna;
}Matriz;

Matriz* alocar_matriz(int m, int n) {
    Matriz* mat = malloc(sizeof(Matriz));
    
    if (mat == NULL) {
        printf("** Erro: Deu erro, faz parte");
        return NULL;
    }
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

    return (mat); // retorna o ponteiro para matriz /

}

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

void preencher_matriz(Matriz *mat) {
    srand(time(NULL));
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            mat->pont[i][j] = rand() % (MAX_RANDOM + 1);
        }
    }
}
Matriz* mat;
int totalprimos;

void BuscaSerial(Matriz* mat) {
    mat->linha = 0;
    mat->fim_linha = TAM_MACRO;
    mat->inicio_coluna = 0;
    mat->fim_coluna = TAM_MACRO;

    while (mat->linha < LINHAS) {
        mat->inicio_coluna = 0;
        mat->fim_coluna = TAM_MACRO;
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
    }
}


int main(int argc, char* argv[]) {
    srand(time(NULL));


    mat = alocar_matriz(LINHAS, COLUNAS);
    preencher_matriz(mat);
    BuscaSerial(mat);

    printf("Total primos encontrados %d", totalprimos);




    return 0;
}
