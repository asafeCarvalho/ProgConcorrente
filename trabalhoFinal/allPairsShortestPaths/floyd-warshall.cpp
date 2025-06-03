#include <bits/stdc++.h>

typedef std::vector<std::vector<long long>> matriz;

std::vector<std::vector<long long>> lerInput() {
    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    std::vector<std::vector<long long>> matriz(totalVertices, std::vector<long long>(totalVertices, INT_MAX));
    for (int i = 0; i < totalVertices; i++) matriz[i][i] = 0;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        matriz[u - 1][v - 1] = std::min((long long) peso, matriz[u - 1][v - 1]);
    }

    return matriz;
}

void printarMatriz(std::vector<std::vector<long long>>& distancias, int totalVertices) {
    for (int i = 0; i < totalVertices; i++) {
        for (int j = 0; j < totalVertices; j++) {
            std::cout << (distancias[i][j] == INT_MAX ? "\u221E" : std::to_string(distancias[i][j])) + "" << " ";
        }
        printf("\n");
    }-
    printf("\n\n");
}

std::vector<std::vector<long long>> floydWarshall(std::vector<std::vector<long long>>& adjancencia) {
    std::vector<std::vector<long long>> anterior = adjancencia;
    int totalVertices = adjancencia.size();
    for (int k = 0; k < totalVertices; k++) {
        std::vector<std::vector<long long>> nova = anterior;
        for (int i = 0; i < totalVertices; i++) {
            for (int j = 0; j < totalVertices; j++) {
                if (anterior[i][k] != INT_MAX && anterior[k][j] != INT_MAX && nova[i][j] > anterior[i][k] + anterior[k][j]) {
                    nova[i][j] = anterior[i][k] + anterior[k][j];
                }
            }
        }
        printarMatriz(nova, totalVertices);
        anterior = nova;
    }

    printarMatriz(anterior, totalVertices);
    return anterior;
}

int main() {
    std::vector<std::vector<long long>> adjacencia = lerInput();
    floydWarshall(adjacencia);
}