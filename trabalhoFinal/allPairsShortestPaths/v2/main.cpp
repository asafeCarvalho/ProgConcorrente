#include <bits/stdc++.h>

typedef  std::vector<std::pair<long long, int>> linhaMatriz;
typedef  std::vector<linhaMatriz> matrizDistancia;


matrizDistancia lerInput() {
    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    matrizDistancia matriz(totalVertices + 1, linhaMatriz(totalVertices + 1, std::pair<long long, int>{INT_MAX, -1}));
    for (int i = 1; i <= totalVertices; i++) matriz[i][i] = {0, i};
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        if (matriz[u][v].first > peso) {
            matriz[u][v].first = peso;
            matriz[u][v].second = u;
        }
    }

    return matriz;
}

void iteracao(matrizDistancia& anterior, matrizDistancia& arestas) {
    int totalVertices = anterior.size() - 1;
    matrizDistancia novo(totalVertices + 1, linhaMatriz (totalVertices + 1));
    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            long long distancia = INT_MAX;
            int antecessor = -1;
            for (int k = 1; k <= totalVertices; k++) {
                if (anterior[i][k].first != INT_MAX && arestas[k][j].first != INT_MAX && distancia > anterior[i][k].first + arestas[k][j].first) {
                    distancia = anterior[i][k].first + arestas[k][j].first;
                    antecessor = k;
                }
            }
            novo[i][j] = {distancia, antecessor};
        }
    }
    anterior = novo;
}


matrizDistancia AllPairsShortesPaths(matrizDistancia adjacencia) {
    int totalVertices = adjacencia.size() - 1;
    matrizDistancia distancias = adjacencia;
    for (int i = 2; i < totalVertices; i++) {
        iteracao(distancias, adjacencia);
    }
    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            // infinito \u221E
            std::cout << " { " << (distancias[i][j].first == INT_MAX ? "\u221E" : std::to_string(distancias[i][j].first)) << "";
            printf(", %d} ", distancias[i][j].second);
        }
        printf("\n");
    }
    return distancias;
}

int main() {
    matrizDistancia adjacencia = lerInput();
    AllPairsShortesPaths(adjacencia);
    return 0;
}
