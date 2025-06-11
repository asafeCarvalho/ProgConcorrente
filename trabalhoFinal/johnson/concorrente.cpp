#include <bits/stdc++.h>
#include "timer.h"
#include <semaphore.h>


typedef std::vector<std::vector<long long*>> matR;
typedef long long ll;

pthread_mutex_t mutex;
pthread_mutex_t mutex_inserir;
sem_t inserir;

ll totalVertices;
ll totalArestas;


int proximoVertice; //variavel para threads

ll** distanciasFinais;
std::vector<std::vector<long long*>> adjacencia; //adjacencia do grafo
std::vector<std::vector<ll*>> adjacenciaRebalanceada;

std::vector<ll> distanciasFalsas;
int numThtreads;

class minhaComparacao
{
  bool reverse;
public:
  minhaComparacao(const bool& revparam=false)
    {reverse=revparam;}
  bool operator() ( long long*& lhs, long long*&rhs)
  {
    if (reverse) return ((*lhs)<(*rhs));
    else return ((*lhs)>(*rhs));
  }
};

std::priority_queue<long long*, std::vector<long long* >, minhaComparacao> fila_output;


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
        // printf("%d %d %d", u,v, peso);

        adjacencia[u].push_back(aresta);
    }
}



long long* dijkstra(int origem) {
    // std::cout << "INCIO origem " << origem << "\n";

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
    // std::cout << "\n";
    // std::string output = "origem " + std::to_string(origem) + " ";
    // for (int i = 1; i <= totalVertices; i++) {
    //     output += (distancia[i] == LLONG_MAX ? "\u221E" : std::to_string( distancia[i] + distanciasFalsas[i] - distanciasFalsas[origem]) ) + " ";
    // }
    // std::cout << output <<  "\n";
    // free(distancia);
    distancia[0] = origem;
    // std::cout << "FIMMM origem " << origem << "\n";

    return distancia;
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


void* thread_task(void* args) {
    ll id = (ll) args;
    int origem;
    
    while (1) {
        pthread_mutex_lock(&mutex);
        origem = proximoVertice;
        if (origem > totalVertices) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        proximoVertice++;
        pthread_mutex_unlock(&mutex);
        ll* resultado = dijkstra(origem);
        pthread_mutex_lock(&mutex_inserir);
        fila_output.push(resultado);
        pthread_mutex_unlock(&mutex_inserir);
        sem_post(&inserir);
    }
    pthread_exit(0);
    return (void *) 0;

}

void* printar(void* args) {
    int atual = 1;
    ll* distancia = 0;
    while (atual <= totalVertices) {
        pthread_mutex_lock(&mutex_inserir);
        while (fila_output.empty() || *(fila_output.top()) != atual) {
            // std::cout << "vida belaa " << atual << "\n" ;
            pthread_mutex_unlock(&mutex_inserir);
            sem_wait(&inserir);
            pthread_mutex_lock(&mutex_inserir);
        }
        distancia = fila_output.top();
        fila_output.pop();
        pthread_mutex_unlock(&mutex_inserir);
        for (int i = 1; i <= totalVertices; i++) {
            std::cout << (distancia[i] == LLONG_MAX ? "\u221E" : std::to_string( distancia[i] + distanciasFalsas[i] - distanciasFalsas[distancia[0]]) ) + " ";
        }
        printf("\n");
        free(distancia);
        distancia = 0;
        atual++;
    }
    return 0;
}

int main(int argc, char** argv) {


    if (argc != 3) {
        exitar_erro((char*) "Por favor, execute o programa da seguinte forma:\n./conc <numDeThreads> <nomeDoArquivo>\n\n");
    } 
    numThtreads = atoi(argv[1]);
    if (numThtreads == 0) exitar_erro((char*) "quantidade de threads invalida!!\n");

    char* nomeArquivo = argv[2];
    
    FILE* arq = fopen(nomeArquivo, "r");

    if (arq == NULL) exitar_erro((char*) "erro ao abrir arquivo\n");

    // return 0;


    fscanf(arq, "%d %d\n", &totalVertices, &totalArestas);

    adjacencia = std::vector< std::vector<ll*> > (totalVertices + 1, std::vector<ll*> {});
    lerInput(arq);
    novoVertice();
    distanciasFalsas = std::vector<ll> (totalVertices + 1, 0);
    // distanciasFalsas = bellman();
    if (distanciasFalsas[0] < 0) exitar_erro((char*) "Não é possível encontrar o menores caminhos pois existe um ciclo negativo\n");

    adjacenciaRebalanceada = novaAdjacencia();
    proximoVertice = 1;



    double inicio, fim, delta, total = 0;
    GET_TIME(inicio);

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex_inserir, NULL);
    sem_init(&inserir, 0, 0);
    pthread_t tids[numThtreads];
    pthread_t consumidora;
    if (pthread_create(&consumidora, NULL, printar, 0)) exitar_erro((char *) "!erro ao criar thread consumidora!\n");
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
    if (pthread_join(consumidora, 0)) exitar_erro((char *) "erro ao dar join na thread");
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mutex_inserir);
    GET_TIME(fim);

    
    printf("%.6f\n", fim - inicio);



    return 0;
}