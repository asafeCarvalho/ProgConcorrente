#include <bits/stdc++.h>

typedef std::vector<std::vector<long long>> matR;
typedef long long ll;

std::vector<std::vector<long long>> lerInput(int totalVertices, int totalArestas) {
    std::vector<std::vector<long long>> adjacencia;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        adjacencia.push_back(std::vector<ll> {u, v, peso});
    }
    return adjacencia;
}



std::vector<ll> dijkstra(matR matrizDoGrafo, int verticesTotal, int origem) {
    std::priority_queue<std::pair<long long int,int>, std::vector<std::pair<long long int,int>>, std::greater<std::pair<long long int,int>> > fila;
    std::vector<long long int> distancia(verticesTotal, INT_MAX);
    fila.push({0, origem});
    while (!fila.empty()) {
        std::pair<long long int,int> topo = fila.top();
        long long int peso =  topo.first, v = topo.second;
        fila.pop();
        if (distancia[v] != INT_MAX) continue;
        distancia[v] = peso;
        for (int i = 1; i <= verticesTotal; i++) {
            if (matrizDoGrafo[v][i] == INT_MAX) continue;
            fila.push({peso + matrizDoGrafo[v][i], i});
        }
    }
    
    return distancia;
}

matR novoVertice(int totalVertices, matR adjacencia) {

    for (int i = 1; i <= totalVertices; i++)
        adjacencia.push_back({0, i, 0});

    return adjacencia;
}


std::vector<long long> bellman(int origem, int totalVertices, std::vector<std::vector<long long>>& arestas) {
    std::vector<long long> distancia(totalVertices, 0);
    for (int i = 1; i <= totalVertices; i++) {
        for (std::vector<long long> atual: arestas) {
            ll u = atual[0], v = atual[1], peso = atual[2];
            if (distancia[v] > distancia[u] + peso) {
                distancia[v] = distancia[u] + peso;
            }
        }
    }

    for (std::vector<long long> atual: arestas) {
        ll u = atual[0], v = atual[1], peso = atual[2];
        if (distancia[v] > distancia[u] + peso) {
            return std::vector<ll> (totalVertices, INT_MIN);
        }
    }
    return distancia;
}

matR gerarMatrz(int totalArestas, matR arestas, std::vector<ll> pesos, int totalVertices, int oposto) {

    matR matrizGrafo = matR (totalVertices + 1, std::vector<ll> (totalVertices + 1, INT_MAX));
    for (std::vector<ll> atual: arestas) {
        int u = atual[0], v = atual[1], p = atual[2];
        p = p + (pesos[u] - pesos[v]) * oposto;
        matrizGrafo[u][v] = p;
        // printf(" u = %d, v =  %d, p = %d\n", u , v, p);
    }


    return matrizGrafo;

}

void printarMatriz(std::vector<std::vector<long long>>& distancias, int totalVertices) {
    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            std::cout << (distancias[i][j] == INT_MAX ? "\u221E" : std::to_string(distancias[i][j])) + "" << " ";
        }
        printf("\n");
    }-
    printf("\n\n");
}


int main() {

    // int totalVertices, totalArestas;
    // scanf("%d %d", &totalVertices, &totalArestas);

    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    matR adjacenciaOriginal = lerInput(totalVertices, totalArestas);
    matR novaAdjacencia = novoVertice(totalVertices, adjacenciaOriginal);    
    
    std::vector<ll> distanciasFalsas = bellman(0, totalVertices + 1, novaAdjacencia);

    matR matrizDoGrafo = gerarMatrz(totalArestas, adjacenciaOriginal, distanciasFalsas, totalVertices, 1);

    matR matrizDistancia (totalVertices + 1);

    for (int i = 1; i <= totalVertices; i++) {
        matrizDistancia[i] = dijkstra(matrizDoGrafo, totalVertices + 1, i);
    }

    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            std::cout << (matrizDistancia[i][j] == INT_MAX ? "\u221E" : std::to_string( matrizDistancia[i][j] + distanciasFalsas[j] - distanciasFalsas[i]) ) + "" << " ";
            // std::cout << (matrizDoGrafo[i][j] == INT_MAX ? "\u221E" : std::to_string(matrizDoGrafo[i][j] )) + "" << " ";

        }
        printf("\n");
    }
    // printarMatriz(matrizDistancia, totalVertices);

    // printf("INT_MAX %d\n" , INT_MAX);


    return 0;
}