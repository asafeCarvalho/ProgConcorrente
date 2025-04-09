#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
    // float* vetor1;
    // float* vetor2;
    int inicio, fim;
} argT;

float* vetor1;
float* vetor2;

void* ProdInterno(void* tArgs) {
    // argT* argumentos = (argT*) tArgs;
    // // printf("hello from thread! %d %d\n", argumentos->inicio, argumentos->fim);
    // int i = argumentos->inicio;
    // int fim = argumentos->fim;
    // double produto = 0;
    // for (; i < fim; i++) {
    //     produto += (vetor1[i]) * (vetor2[i]);
    // }
    // double* retorno = (double*) malloc(sizeof(double));
    // if (retorno != NULL) *retorno = produto;
    // else {printf("erro ao alocar espaço na memória\n"); exit(1);}
    // pthread_exit((void * ) retorno);
    // exit(1);
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


    // float vetor1[tamanhoVetor];
    // float vetor2[tamanhoVetor];
    vetor1 = (float *) malloc(sizeof(float) * tamanhoVetor);
    vetor2 = (float *) malloc(sizeof(float) * tamanhoVetor);

    ret = fread(vetor1, sizeof(float), tamanhoVetor, arq);
    if (!ret) {printf("erro na leitura do arquivo"); return 0;}
    ret = fread(vetor2, sizeof(float), tamanhoVetor, arq);
    if (!ret) {printf("erro na leitura do arquivo"); return 0;}

    int totalThreads = atoi(argv[2]);
    if (totalThreads > tamanhoVetor) totalThreads = tamanhoVetor;
    int bloco = tamanhoVetor / totalThreads;

    pthread_t* pids = (pthread_t *) malloc(sizeof(pid_t) * totalThreads);
    argT** todosArgumentos = (argT**) malloc(sizeof(argT*) * totalThreads);

    for(int i = 0; i < totalThreads; i++){
        argT* args = (argT*) malloc(sizeof(argT));
        if (args == NULL) {printf("malloc falhou!!!!!\n"); return 1;}
        // argumentos->vetor1 = vetor1;
        // argumentos->vetor2 = vetor2;
        args->inicio= bloco * i;
        args->fim= i + 1 == totalThreads ? tamanhoVetor : bloco * (i + 1);
        printf("endereco argsT %d: %p\n", i, (void *) args);
        todosArgumentos[i] = args;
        if(pthread_create(&pids[i], NULL, ProdInterno, (void *) &args)) {
            printf("error!!!! p_create\n\n");
        };
    }

    printf("\n\n");
    double respostaObtida = 0;
    double* retornoThread;
    for (int i = 0; i < totalThreads; i++) {
        if (pthread_join(pids[i], (void *) &retornoThread)) {
            printf("erro no join das threads\n");
            return 0;
        } 
        // respostaObtida += *retornoThread;
        // free(retornoThread);
        printf("FREE argsT %d: %p\n", i, (void *) todosArgumentos[i]);

        // if (i < 6) free((void*) todosArgumentos[i]);
    }
    double respostaCorreta;
    if (!fread(&respostaCorreta, sizeof(double), 1, arq)) {
        printf("erro ao ler o arquivo %s\n", argv[0]);
    };
    printf("resposta correta = %f\nresposta obtida = %f\n", respostaCorreta, respostaObtida);
    // free(pids);
    return 1;
}