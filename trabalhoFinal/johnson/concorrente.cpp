#include <bits/stdc++.h>


typedef std::vector<std::vector<long long>> matR;
typedef long long ll;

pthread_mutex_t mutex;

matR matrizDoGrafo;
ll totalVertices;
int proximoVertice;
ll** distanciasFinais;

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


std::vector<std::vector<long long>> lerInput(int totalVertices, int totalArestas) {
    std::vector<std::vector<long long>> adjacencia;
    for (int i = 0; i < totalArestas; i++) {
        int u, v, peso;
        scanf("%d %d %d\n", &u, &v, &peso);
        adjacencia.push_back(std::vector<ll> {u, v, peso});
    }
    return adjacencia;
}



long long* dijkstra(int origem) {
    std::priority_queue<std::pair<long long int,int>, std::vector<std::pair<long long int,int>>, std::greater<std::pair<long long int,int>> > fila;
    ll* distancia = (ll *) Malloc(sizeof(ll) * totalVertices + 1);
    for (int i = 0; i <= totalVertices; i++) distancia[i] = INT_MAX;
    fila.push({0, origem});
    while (!fila.empty()) {
        std::pair<long long int,int> topo = fila.top();
        long long int peso =  topo.first, v = topo.second;
        fila.pop();
        if (distancia[v] != INT_MAX) continue;
        distancia[v] = peso;
        for (int i = 1; i <= totalVertices; i++) {
            if (matrizDoGrafo[v][i] == INT_MAX) continue;
            fila.push({peso + matrizDoGrafo[v][i], i});
        }
    }
    return distancia;
}

void* thread_task(void* args) {
    ll id = (ll) args;
    int origem;
    while (1) {
        pthread_mutex_lock(&mutex);
        origem = proximoVertice;
        if (origem == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        proximoVertice--;
        pthread_mutex_unlock(&mutex);
        ll* distancias_origem =  dijkstra(origem);
        // pthread_mutex_lock(&mutex);
        distanciasFinais[origem] = distancias_origem;
        // for (int i = 1; i <= totalVertices; i++) {
        //         std::cout << (distancias_origem[i] == INT_MAX ? "\u221E" : std::to_string( distancias_origem[i] + distanciasFalsas[i] - distanciasFalsas[origem]) ) + "" << " ";

        // }
        // pthread_mutex_unlock(&mutex);
        // printf("id = %ld, origem = %d\n", id, origem);

    }

    return (void *) 0;

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
    }
    printf("\n\n");
}


int main() {

    // int totalVertices, totalArestas;
    // scanf("%d %d", &totalVertices, &totalArestas);

    int totalArestas;
    scanf("%d %d\n", &totalVertices, &totalArestas);
    distanciasFinais = (ll**) Malloc(sizeof(ll*) * (totalVertices + 1));
    matR adjacenciaOriginal = lerInput(totalVertices, totalArestas);

    matR novaAdjacencia = novoVertice(totalVertices, adjacenciaOriginal);    
    
    distanciasFalsas = bellman(0, totalVertices + 1, novaAdjacencia);

    matrizDoGrafo = gerarMatrz(totalArestas, adjacenciaOriginal, distanciasFalsas, totalVertices, 1);


    proximoVertice = totalVertices;
    pthread_mutex_init(&mutex, NULL);



    




    numThtreads = 4;
    proximoVertice = totalVertices;

    pthread_t tids[numThtreads];

    for (long long i = 0; i < numThtreads; i++) {
        if (pthread_create(&tids[i], NULL, thread_task, (void*) i)) {
            exitar_erro((char *) "!erro ao criar thread!\n");
        }
    }

    for (int i = 0; i < numThtreads; i++)  {
        if (pthread_join(tids[i], NULL)) {
            exitar_erro((char *) "erro ao dar join na thread");
        }
    }

    for (int i = 1; i <= totalVertices; i++) {
        for (int j = 1; j <= totalVertices; j++) {
            // std::cout << (matrizDoGrafo[i][j] == INT_MAX ? "\u221E" : std::to_string(matrizDoGrafo[i][j] )) + "" << " ";
            std::cout << (distanciasFinais[i][j] == INT_MAX ? "\u221E" : std::to_string( distanciasFinais[i][j] + distanciasFalsas[j] - distanciasFalsas[i]) ) + "" << " ";
        }
        printf("\n");
    }



    pthread_mutex_destroy(&mutex);


    return 0;
}