#include <bits/stdc++.h>
#include "timer.h"


typedef std::vector<std::vector<long long*>> matR;
typedef long long ll;

pthread_mutex_t mutex;

ll totalVertices;
ll totalArestas;


int proximoVertice; //variavel para threads

ll** distanciasFinais;
std::vector<std::vector<long long*>> adjacencia; //adjacencia do grafo
std::vector<std::vector<ll*>> adjacenciaRebalanceada;

std::vector<ll> distanciasFalsas;
int numThtreads;


void exitar_erro(char* msg) {
    fprintf(stderr, msg);
    exit(-1);
}

void* Malloc(int tamanho) {
    void* ponteiro = malloc(tamanho);
    if (ponteiro == NULL) {
        exitar_erro((char *) "ocorreu um erro na alocaçao de memoria...\nexecuçao finalizada!\n");
    }
    return ponteiro;
}


void lerInput(FILE* arq) {
    adjacencia;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        fscanf(arq, "%d %d %d\n", &u, &v, &peso);
        ll* aresta = (ll*) Malloc(sizeof(ll) * 2);
        aresta[0] = v; aresta[1] = peso;
        adjacencia[u].push_back(aresta);
    }
}



void dijkstra(int origem) {
    // if (origem % 1024 == 0) std::cout << "origem " << origem << "\n";

    std::priority_queue<std::pair<long long int,int>, std::vector<std::pair<long long int,int>>, std::greater<std::pair<long long int,int>> > fila;

    ll* distancia = (ll *) Malloc(sizeof(ll) * (totalVertices + 1));
    for (int i = 0; i <= totalVertices; i++) distancia[i] = LLONG_MAX;
    fila.push({0, origem});
    while (!fila.empty()) {
        std::pair<long long int,int> topo = fila.top();
        long long int peso =  topo.first, v = topo.second;
        fila.pop();
        if (distancia[v] < peso) continue;
        distancia[v] = peso;
        for (ll*& aresta: adjacenciaRebalanceada[v]) {
            ll vizinho = aresta[0], arestaPeso = aresta[1]; 
            if (distancia[vizinho] > arestaPeso + peso){
                fila.push({arestaPeso + peso, vizinho});
                distancia[vizinho] = arestaPeso + peso;
            }
        }
    }
    free(distancia);
}


matR novoVertice() {

    for (int i = 1; i <= totalVertices; i++) {
        ll* aresta = (ll*) Malloc(sizeof(ll) * 2);
        aresta[0] = i; 
        aresta[1] = 0;
        adjacencia[0].push_back(aresta);
    }
    return adjacencia;
}


std::vector<long long> bellman() {
    std::vector<long long> distancia(totalVertices + 1, LLONG_MAX);
    distancia[0] = 0;
    for (int i = 0; i <= totalVertices; i++) {
        for (int u = 0; u <= totalVertices; u++) {
            for (ll* aresta: adjacencia[u]) {
                ll v = aresta[0];
                ll peso = aresta[1];
                if (distancia[v] > distancia[u] + peso) {
                    distancia[v] = distancia[u] + peso;
                }
            }
        }
        // printf("i = %d bellman\n", i);
    }

    for (int u = 0; u <= totalVertices; u++) {
        std::vector<ll*> atual = adjacencia[u];
        for (ll* aresta: atual) {
            ll v = aresta[0], peso = aresta[1];
            if (distancia[v] > distancia[u] + peso) {
                distancia[v] = distancia[u] + peso;
                return std::vector<ll> (totalVertices, INT_MIN);
            }
        }
    }
    return distancia;
}

 std::vector< std::vector<ll*>> novaAdjacencia() {
    std::vector< std::vector<ll*>> nova = std::vector< std::vector<ll*> > (totalVertices + 1, std::vector<ll*> {});
    for (int i = 1 ; i <= totalVertices; i++) {
        for (ll* aresta: adjacencia[i]) {
            ll* novaAresta = (ll*) Malloc(sizeof(ll) * 2);
            novaAresta[0] = aresta[0];
            novaAresta[1] = aresta[1] + ( distanciasFalsas[i] - distanciasFalsas[aresta[0]]);
            nova[i].push_back(novaAresta);
        }
    }
    return nova;


}


int main(int argc, char** argv) {

   if (argc != 2) {
        exitar_erro((char*) "Por favor, execute o programa da seguinte forma:\n./sequencial <nomeDoArquivo>\n\n");
    } 

    char* nomeArquivo = argv[1];
    
    FILE* arq = fopen(nomeArquivo, "r");

    if (arq == NULL) exitar_erro((char*) "erro ao abrir arquivo\n");



    fscanf(arq, "%d %d\n", &totalVertices, &totalArestas);

    adjacencia = std::vector< std::vector<ll*> > (totalVertices + 1, std::vector<ll*> {});
    printf("lendo arquivo\n");
    lerInput(arq);
    printf("adicionando novo vertice\n");
    novoVertice();
    printf("executando bellman\n");
    distanciasFalsas = std::vector<ll> (totalVertices + 1, 0);
    distanciasFalsas = bellman();
    printf("fim do bellman\n");
    if (distanciasFalsas[0] < 0) exitar_erro((char*) "Não é possível encontrar o menores caminhos pois existe um ciclo negativo\n");
    for (auto atual: distanciasFalsas) std::cout << atual << "\n";

    return 0;

    adjacenciaRebalanceada = novaAdjacencia();
    proximoVertice = totalVertices;


    double inicio, fim, delta, total = 0;
    GET_TIME(inicio);
        for (int i =  1; i <= totalVertices; i++) {
            dijkstra(i);
        }
    GET_TIME(fim);


    printf("%.6f\n", fim - inicio);



    return 0;
}