#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>
#include <math.h>


//N: QUANTIDADE DE INTEIROS
//M: TAMANHO DO BUFFER
//C: QUANTIDADE DE CONSUMIDORES


int N, M, C;

int in, out;

//vazio deve ter M sinais disponiveis
sem_t vazio;

//cheio de ser inicializado com zero sinais disponiveis
sem_t cheio;
pthread_mutex_t mutex;

pthread_mutex_t mutex_inserir;


int* buffer;


void exitar_erro(char* msg) {
    fprintf(stderr, msg);
    exit(-1);
}

void* Malloc(int tamanho) {
    void* ponteiro = malloc(tamanho);
    if (ponteiro == NULL) {
        exitar_erro("ocorreu um erro na alocaçao de memoria...\nexecuçao finalizada!\n");
    }
    return ponteiro;
}


int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
}

void * consumidor(void* args) {
    long long int id = (long long int) args;
    long long int totalDePrimos = 0;
    while (buffer[out] != N + 1) {
        int numeroAtual;

        sem_wait(&cheio);
        pthread_mutex_lock(&mutex);

        numeroAtual = buffer[out];
        if (numeroAtual == N + 1) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        out = (out + 1) % M;
        sem_post(&vazio);
        pthread_mutex_unlock(&mutex);

        totalDePrimos += ehPrimo(numeroAtual);
        if (numeroAtual == N) {
            buffer[out] = N + 1;

            for (int i = 0; i <= C + 1; i++) {
                sem_post(&cheio);
            }
            break;

        }
    }

    pthread_exit((void*) totalDePrimos);
}


void* produtor(void* args) {
    for (int i = 1; i <= N; i++){
        sem_wait(&vazio);
        buffer[in] = i;
        in = (in + 1) % M;
        sem_post(&cheio);


    }
    printf(">terminando produçao<\n");
}







int main(int argc, char** argv) {

    if (argc < 4) {
        exitar_erro("ao executar o programa informe: \nN: o quantidade de numeros a serem checados \nM:o tamanho do buffer de numeros \nC: quantidade de threads consumidoras");
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    C = atoi(argv[3]);

    if (!N || !M || !C) {
        exitar_erro("entradas invalidas!\n");
    }

    buffer = (int *) Malloc(sizeof(int) * M);

    pthread_mutex_init(&mutex, 0);

    sem_init(&cheio, 0, 0);
    sem_init(&vazio, 0, M);

    pthread_t produtora_id;
    if (pthread_create(&produtora_id, 0, produtor, 0)) exitar_erro("erro ao criar thread!\n");

    pthread_t ids_consumidoras[C];
    for (long long int i = 0; i < C; i++) {
        if (pthread_create(&ids_consumidoras[i], 0, consumidor, (void *) i))
            exitar_erro("erro ao criar threads\n");
    }

    for (int i = 0; i < C; i++) {
        if (pthread_join(ids_consumidoras[i], 0)) 
            exitar_erro("erro no join!!!!\n");
    }
    return 0;
}