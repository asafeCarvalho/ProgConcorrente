#include <bits/stdc++.h>

typedef std::vector<std::vector<long long>> matR;

std::vector<std::tuple<int,int,int>> lerInput() {
    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    std::vector<std::tuple<int,int,int>> adjacencia;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        adjacencia.push_back(std::make_tuple(peso, u, v));
    }
    return adjacencia;
}

void printarMatriz(std::vector<std::vector<long long>>& distancias, int totalVertices) {
    for (int i = 0; i < totalVertices; i++) {
        for (int j = 0; j < totalVertices; j++) {
            std::cout << (distancias[i][j] == INT_MAX ? "\u221E" : std::to_string(distancias[i][j])) + "" << " ";
        }
        printf("\n");
    }
    printf("\n\n");
}


void dijkstra(std::vector<std::tuple<int,int,int>>& arestas, int verticesTotal, int origem) {
    std::priority_queue<std::pair<long long int,int>, std::vector<std::pair<long long int,int>>, std::greater<std::pair<long long int,int>> > fila;
    std::vector<std::vector<std::pair<int,int>>> adjacencia(verticesTotal, std::vector<std::pair<int,int>>{});
    for (auto atual: arestas) {
        int p = std::get<0>(atual);
        int u = std::get<1>(atual);
        int v = std::get<2>(atual);
        
    }
    std::vector<long long int> distancia(verticesTotal, INT_MAX);
    fila.push({0, origem});
    while (!fila.empty()) {
        std::pair<long long int,int> topo = fila.top();
        long long int peso =  topo.first, v = topo.second;
        fila.pop();
        if (distancia[v] != INT_MAX) continue;
        distancia[v] = peso;
        for (auto vizinho: adjacencia[v]) {
            fila.push({peso + vizinho.first, vizinho.second});
        }
    }
    for (auto valor: distancia) std::cout << valor << ' ';
    printf("\n");
}

matR bellman(int origem, int totalVertices, std::vector<std::vector<long long>>& arestas) {
    std::vector<long long> output(totalVertices, 0);
    for (int i = 1; i <= totalVertices; i++) {

    }
    return novo;
}


int main() {


    return 0;
}