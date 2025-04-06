#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>

// gera dois novos vetores e retorna o produto interno entre eles
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

int gerarArquivos(char* nomeArquivo, float* vetor1, float* vetor2, int tamanhoVetor, double produtoInterno) {
    if (strlen(nomeArquivo) > 20) {
        printf("Por favor, escolha um nome de arquivo com menos de 20 caracteres\n");
        return 0;
    }
    char caminho[100] = "../inputs/";
    strcat(caminho, nomeArquivo);
    strcat(caminho, ".bin");
    FILE* descritorArq = fopen(caminho, "wb");
    if (!descritorArq) {
        printf("erro na criação do aquivo\n");
        return 0;
    }

    int tamanho_w = fwrite(&tamanhoVetor, sizeof(int), 1, descritorArq);
    int vetor1_w  = fwrite(vetor1, sizeof(float), tamanhoVetor, descritorArq);
    int vetor2_w = fwrite(vetor2, sizeof(float), tamanhoVetor, descritorArq);
    int resposta_w = fwrite(&produtoInterno, sizeof(double), 1, descritorArq);
    int soma = (int) sizeof(int) + (int) sizeof(float) * tamanhoVetor * 2 + (int) sizeof(double);
    printf("total %d\n", soma); 
    if (tamanho_w != 1 || vetor1_w != tamanhoVetor || vetor2_w != tamanhoVetor || resposta_w != 1) {
        printf("Erro inesperado na criacao dos arquivos!");
        return 0;
    }


    return 1;
}

int main(int argc, char** argv) {
    srand(time(0));
    if (argc != 3) {
        printf("\nPor favor, informe o nome e tamanho do vetor como no exemplo a seguir:\n ./main <tamanho> <nome>\n\n");
        return 0;
    } else if (atoi(argv[1]) <= 0) {
        printf("Por favor, informe um número positivo\n");
    }
    int tamanhoVetor;
    tamanhoVetor = atoi(argv[1]);
    float vetor1[tamanhoVetor], vetor2[tamanhoVetor];
    double produtoInterno = gerarVetores(vetor1, vetor2, tamanhoVetor);
    gerarArquivos(argv[2], vetor1, vetor2, tamanhoVetor, produtoInterno);

    return 0;
}