#include <bits/stdc++.h>


std::vector<std::vector<long long>> lerInput() {
    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    std::vector<std::vector<long long>> matriz(totalVertices + 1, std::vector<long long>(totalVertices + 1, INT_MAX));
    for (int i = 1; i <= totalVertices; i++) matriz[i][i] = 0;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        matriz[u][v] = std::min((long long) peso, matriz[u][v]);
    }

    return matriz;
}

void iteracao(std::vector<std::vector<long long>>& anterior, std::vector<std::vector<long long>>& arestas) {
    int totalVertices = anterior.size() - 1;
    std::vector<std::vector<long long>> novo(totalVertices + 1, std::vector<long long>(totalVertices + 1));
    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            long long distancia = INT_MAX;
            for (int k = 1; k <= totalVertices; k++) {
                if (anterior[i][k] != INT_MAX && arestas[k][j] != INT_MAX && distancia > anterior[i][k] + arestas[k][j]) {
                    distancia = anterior[i][k] + arestas[k][j];
                }
                // if (distancia > arestas[i][j] + anterior[k][j]) {
                //     distancia = arestas[i][j] + anterior[k][j];
                // }
            }
            novo[i][j] = distancia;
        }
    }
    anterior = novo;
}

void printarMatriz(std::vector<std::vector<long long>>& distancias, int totalVertices) {
    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            std::cout << (distancias[i][j] == INT_MAX ? "\u221E" : std::to_string(distancias[i][j])) + "" << " ";
        }
        printf("\n");
    }
}

// int percorrerCaminho(std::vector<std::vector<long long>>& distancia, int valor, int i, int j, int totalVertices) {
//     printf("%d %d %d\n", i, j, valor);

//     if (valor != INT_MAX && valor == 0) return -1;
//     for (int k = 1; k <= totalVertices; k++) {
//         if (valor - distancia[i][k] == distancia[k][j] && i != k) {
//             int retorno = percorrerCaminho(distancia, valor - distancia[i][k], k, j, totalVertices);
//             // if (retorno == -1) return k;
//             // else if (retorno != 0) return retorno;
//         }
//     }

//     return 0;

// }

// std::vector<std::vector<int>> matrizPredecessor(std::vector<std::vector<long long>>& distancia, int totalVertices) {
//     std::vector<std::vector<int>> pi(totalVertices, std::vector<int>(totalVertices));
//     for (int i = 1; i <= totalVertices; i++) {
//         for (int j = 1; j <= totalVertices; j++) {
//             if (i == j || distancia[i][j] == INT_MAX) continue;
//             int antecessor = percorrerCaminho(distancia, distancia[i][j], i, j, totalVertices);
//             printf("i = %d, j = %d, antecessor = %d", i, j , antecessor);
//             printf("\n\n");
//         }
//     }

//     return pi;
// }

std::vector<std::vector<long long>> AllPairsShortesPaths(std::vector<std::vector<long long>> adjacencia) {
    int totalVertices = adjacencia.size() - 1;
    std::vector<std::vector<long long>> distancias = adjacencia;
    for (int i = 2; i < totalVertices; i++) {
        iteracao(distancias, adjacencia);
    }
    // lsmatrizPredecessor(distancias, totalVertices);
    return distancias;
}

int main() {
    std::vector<std::vector<long long>> adjacencia = lerInput();
    AllPairsShortesPaths(adjacencia);
    return 0;
}
