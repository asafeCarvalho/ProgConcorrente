#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "timer.h"

typedef long long int lli;

void erro_terminar(char* msg) {
    fprintf(stderr, msg);
    exit(-1);
}

lli n;
lli disponivel;
pthread_mutex_t dispMutex;
lli totalPrimos;
pthread_mutex_t primosMutex;

int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i< sqrt((double) n) + 1; i+=2) if(n%i==0) return 0;
    return 1;
}

void* primos_thread(void* args) {
    lli atual; 
    pthread_mutex_lock(&dispMutex);
    atual = disponivel + 1;
    disponivel++;
    pthread_mutex_unlock(&dispMutex);
    while (atual < n) {
        if (ehPrimo(atual)) {
            pthread_mutex_lock(&primosMutex);
            totalPrimos++;
            pthread_mutex_unlock(&primosMutex);
        }
        pthread_mutex_lock(&dispMutex);
        atual = disponivel + 1;
        disponivel++;
        pthread_mutex_unlock(&dispMutex);
    }

    pthread_exit(0);
}


int main(int argc, char** argv) {
    double inicio, fim, delta, total=0;
    int totalThreads;
    if (argc != 3) 
        erro_terminar("por favor, digite: \n./primos <quantidade de numeros> <quantidade de threads>\n");

    n = atoll(argv[1]);
    totalThreads = atoi(argv[2]);
    if (n == 0 || totalThreads == 0) erro_terminar("!!!inputs inválidos!!!\n");

    GET_TIME(inicio);
    pthread_t tids[totalThreads];

    for (int i = 0; i < totalThreads; i++) {
        if (pthread_create(&tids[i], NULL, primos_thread, NULL))
            erro_terminar("erro ao criar thread\n\n");   
    }


    for (int i = 0; i < totalThreads; i++) {
        if (pthread_join(tids[i], NULL))
            erro_terminar("erro ao dar join na thread\n\n");
        
    }
    GET_TIME(fim);


    printf("%.6f\n", totalPrimos, fim - inicio);
    


}