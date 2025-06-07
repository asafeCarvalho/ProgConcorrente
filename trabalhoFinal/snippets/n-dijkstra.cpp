#include <bits/stdc++.h>


void dijkstra(std::vector<std::vector<std::pair<int,int>>>& adjacencia, int verticesTotal, int origem) {
    std::priority_queue<std::pair<long long int,int>, std::vector<std::pair<long long int,int>>, std::greater<std::pair<long long int,int>> > fila;
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

std::vector<std::vector<std::pair<int,int>>> lerInput() {
    int verticesTotal, arestasTotal;
    scanf("%d %d\n", &verticesTotal, &arestasTotal);
    std::vector<std::vector<std::pair<int,int>>> adjacencia(verticesTotal, std::vector<std::pair<int,int>>{});
    for (int i = 0; i < arestasTotal; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        adjacencia[u - 1].push_back({peso,v - 1});
    }
    return adjacencia;
}


int main() {
    std::vector<std::vector<std::pair<int,int>>> adjacencia = lerInput();

    int totalVertices = adjacencia.size();

    for (int i = 0; i < totalVertices; i++) {
        dijkstra(adjacencia, totalVertices, i);
    }
    
 
    return 0;
}