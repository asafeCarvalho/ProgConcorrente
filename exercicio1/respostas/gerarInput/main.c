#include <stdio.h> 
#include <stdlib.h>
#include <time.h>


//retorna o produto interno entre dois novos vetores aleatorios
double gerarVetores(float* vetor1, float* vetor2, int tamanhoVetor) {
    float valorMaximo = (float) 1e5;
    double produtoInterno = 0;
    for (int i = 0; i < tamanhoVetor; i++) {
        float rand1 = valorMaximo * ( (float) rand() / RAND_MAX);
        float rand2 = valorMaximo * ( (float) rand() / RAND_MAX);
        vetor1[i] = rand1;
        vetor2[i] = rand2;
        produtoInterno += rand1 * rand2;
    }
    return produtoInterno;
}

int main(int argc, char** argv) {
    srand(time(0));
    if (argc < 2) {
        printf("Por favor, informe o tamanho do vetor desejado.\n");
        return 0;
    } else if (atoi(argv[1]) <= 0) {
        printf("Por favor, informe um número positivo");
    }
    int tamanhoVetor;
    tamanhoVetor = atoi(argv[1]);
    float vetor1[tamanhoVetor], vetor2[tamanhoVetor];
    FILE* descritorArq = fopen("../inputs/teste.bin", "wb");
    if (!descritorArq) {
        printf("erro na criação do aquivo\n");
        return 0;
    }

    return 0;
}