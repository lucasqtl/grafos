#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <cstring>
#include <limits>
#include <sstream>

using namespace std;

// Definição da classe Grafo
class Grafo {
private:
    int numVertices;
    vector<vector<double>> adjMatrix; // Matriz de adjacência

public:
    Grafo(int vertices) : numVertices(vertices), adjMatrix(vertices + 1, vector<double>(vertices + 1, 0)) {}
    
    void addEdge(int u, int v, double weight) {
        if (u > 0 && v > 0 && u <= numVertices && v <= numVertices) {
            adjMatrix[u][v] = weight;
            adjMatrix[v][u] = weight; // Grafo não direcionado
        }
    }
    
    vector<pair<int, double>> getNeighbors(int vertex) const {
        vector<pair<int, double>> neighbors;
        for (int i = 1; i <= numVertices; ++i) {
            if (adjMatrix[vertex][i] > 0) {
                neighbors.emplace_back(i, adjMatrix[vertex][i]);
            }
        }
        return neighbors;
    }
    
    int getSize() const { return numVertices; }
};

// Função para executar o algoritmo de Dijkstra
vector<double> shortestPaths(Grafo& graph, int start) {
    int vertices = graph.getSize();
    vector<double> distances(vertices + 1, numeric_limits<double>::max());
    set<pair<double, int>> unvisited; // {distancia, vertice}

    distances[start] = 0;
    unvisited.insert({0, start});

    while (!unvisited.empty()) {
        int current = unvisited.begin()->second;
        unvisited.erase(unvisited.begin());

        vector<pair<int, double>> neighbors = graph.getNeighbors(current);
        for (const auto& neighbor : neighbors) {
            int nextVertex = neighbor.first;
            double weight = neighbor.second;
            double newDist = distances[current] + weight;

            if (newDist < distances[nextVertex]) {
                unvisited.erase({distances[nextVertex], nextVertex});
                distances[nextVertex] = newDist;
                unvisited.insert({newDist, nextVertex});
            }
        }
    }

    return distances;
}

// Função de ajuda
void displayHelp() {
    cout << "-h : exibe esta mensagem de ajuda" << endl;
    cout << "-o <arquivo> : redireciona a saída para o arquivo especificado" << endl;
    cout << "-f <arquivo> : especifica o arquivo de entrada com o grafo" << endl;
    cout << "-i <numero> : define o vértice inicial" << endl;
}

// Função para processar o arquivo de entrada
void processFile(const string& inputFile, const string& outputFile, int startVertex) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile) {
        cerr << "Erro: Não foi possível abrir o arquivo de entrada " << inputFile << endl;
        return;
    }

    // Ignorar cabeçalho (ex.: .mtx)
    string header;
    while (getline(inFile, header) && header.find("%%") != string::npos);

    // Ler número de vértices e arestas
    stringstream ss(header);
    int vertices, edges;
    ss >> vertices >> edges;

    Grafo graph(vertices);

    // Ler arestas
    for (int i = 0; i < edges; ++i) {
        int u, v;
        double w = 1.0;
        if (!(inFile >> u >> v) || u < 1 || v < 1 || u > vertices || v > vertices) continue;
        inFile >> w; // Tenta ler peso, se não houver, usa 1.0
        graph.addEdge(u, v, w);
    }

    // Executar Dijkstra
    vector<double> distances = shortestPaths(graph, startVertex);

    // Imprimir resultados
    for (int i = 1; i <= vertices; ++i) {
        if (distances[i] == numeric_limits<double>::max()) {
            cout << i << ":-1 ";
            outFile << i << ":-1 ";
        } else {
            cout << i << ":" << static_cast<int>(distances[i]) << " ";
            outFile << i << ":" << static_cast<int>(distances[i]) << " ";
        }
    }
    cout << endl;
    outFile << endl;

    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    string inputFile, outputFile = "saida.txt";
    int startVertex = 1;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0) {
            displayHelp();
            return 0;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            startVertex = stoi(argv[++i]);
        }
    }

    if (inputFile.empty()) {
        cerr << "Erro: Nenhum arquivo de entrada especificado." << endl;
        return 1;
    }

    processFile(inputFile, outputFile, startVertex);
    return 0;
}