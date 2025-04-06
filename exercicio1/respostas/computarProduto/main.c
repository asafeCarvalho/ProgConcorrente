#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct argT
{
    float* vetor1;
    float* vetor2;
    int inicio, fim;
} argT;


void* ProdInterno(void* tArgs) {
    argT* argumentos = (argT*) tArgs;
    double produto = 0;
    for (int i = argumentos->inicio; i < argumentos->fim; i++) {
        produto += (argumentos->vetor1[i]) * (argumentos->vetor2[i]);
    }
    double* retorno = (double*) malloc(sizeof(double));
    if (retorno != NULL) *retorno = produto;
    else {printf("erro ao alocar espaço na memória\n"); exit(1);}
    free(argumentos);
    pthread_exit((void*) retorno);
    exit(1);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Forneca o arquivo de nome entrada e a quantidade de threads:\n./main <nome> <total de threads>\n");
        return 0;
    }
    FILE* arq = fopen(argv[1], "rb");
    if (!arq) {
        printf("Erro na abertura do arquivo %s\n", argv[1]);
        return 0;
    }
    size_t ret;
    int tamanhoVetor;
    ret = fread(&tamanhoVetor, sizeof(int), 1, arq);
    if (!ret) {printf("erro na leitura do arquivo"); return 0;}


    float vetor1[tamanhoVetor];
    float vetor2[tamanhoVetor];
    ret = fread(vetor1, sizeof(float), tamanhoVetor, arq);
    if (!ret) {printf("erro na leitura do arquivo"); return 0;}
    ret = fread(vetor2, sizeof(float), tamanhoVetor, arq);
    if (!ret) {printf("erro na leitura do arquivo"); return 0;}

    int totalThreads = atoi(argv[2]);
    if (totalThreads > tamanhoVetor) totalThreads = tamanhoVetor;
    int bloco = tamanhoVetor / totalThreads;

    pthread_t* pids = (pthread_t *) malloc(sizeof(pid_t) * totalThreads);

    for(int i = 0; i < totalThreads; i++){
        argT* argumentos = (argT*) malloc(sizeof(argT));
        argumentos->vetor1 = vetor1;
        argumentos->vetor2 = vetor2;
        argumentos->inicio= bloco * i;
        argumentos->inicio= i + 1 == totalThreads ? tamanhoVetor : bloco * (i + 1);
        pthread_create(&pids[i], NULL, ProdInterno, (void *) argumentos);
    }

    double respostaObtida = 0;
    double* retornoThread;
    for (int i = 0; i < totalThreads; i++) {
        if (pthread_join(pids[i], (void *) &retornoThread)) {
            printf("erro no join das threads\n");
            return 0;
        } 
        respostaObtida += *retornoThread;
        free(retornoThread);
    }
    double respostaCorreta;
    if (!fread(&respostaCorreta, sizeof(double), 1, arq)) {
        printf("erro ao ler o arquivo %s\n", argv[0]);
    };
    printf("resposta correta = %f\nresposta obtida = %f\n", respostaCorreta, respostaObtida);
    free(pids);
    return 1;
}