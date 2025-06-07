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




int main() {

    int totalVertices, totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    matR adjacencia = lerInput(totalVertices, totalArestas);
    matR novaAdjacencia = novoVertice(totalVertices, adjacencia);



    std::vector<ll> distanciasFalsas = bellman(0, totalVertices + 1, novaAdjacencia);


    for (ll peso: distanciasFalsas) {
        std::cout << peso << "\n";
    }


    return 0;
}



