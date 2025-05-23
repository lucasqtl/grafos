#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <limits>
#include <sstream>

using namespace std;

class Grafo {
private:
    int numVertices;
    vector<vector<pair<int, double>>> adjList;

public:
    Grafo(int vertices) : numVertices(vertices), adjList(vertices + 1) {}

    void addEdge(int u, int v, double weight) {
        if (u > 0 && v > 0 && u <= numVertices && v <= numVertices) {
            adjList[u].emplace_back(v, weight);
            adjList[v].emplace_back(u, weight);
        }
    }

    vector<pair<int, double>> getNeighbors(int vertex) const {
        return adjList[vertex];
    }

    int getSize() const { return numVertices; }
};

vector<double> shortestPaths(Grafo& graph, int start) {
    int vertices = graph.getSize();
    vector<double> distances(vertices + 1, numeric_limits<double>::max());
    set<pair<double, int>> unvisited;

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

void displayHelp() {
    cout << "-h : exibe esta mensagem de ajuda" << endl;
    cout << "-o <arquivo> : redireciona a saída para o arquivo especificado" << endl;
    cout << "-f <arquivo> : especifica o arquivo de entrada com o grafo" << endl;
    cout << "-i <numero> : define o vértice inicial" << endl;
}

void processFile(const string& inputFile, const string& outputFile, int startVertex) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile) {
        cerr << "Erro: Nao foi possivel abrir o arquivo de entrada " << inputFile << endl;
        return;
    }

    string header;
    while (getline(inFile, header) && header.find("%%") != string::npos);

    stringstream ss(header);
    int vertices, edges;
    ss >> vertices >> edges;

    Grafo graph(vertices);

    for (int i = 0; i < edges; ++i) {
        int u, v;
        double w = 1.0;
        if (!(inFile >> u >> v) || u < 1 || v < 1 || u > vertices || v > vertices) continue;
        inFile >> w;
        graph.addEdge(u, v, w);
    }

    vector<double> distances = shortestPaths(graph, startVertex);

    for (int i = 1; i <= vertices; ++i) {
        if (distances[i] == numeric_limits<double>::max()) {
            cout << i << ":-1 ";
            outFile << i << ":" << -1;
        } else {
            cout << i << ":" << static_cast<int>(distances[i]) << " ";
            outFile << i << ":" << static_cast<int>(distances[i]);
        }
        if (i < vertices) {
            cout << " ";
            outFile << " ";
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
        if (argv[i][0] == '-' && i + 1 < argc) {
            if (argv[i][1] == 'o') outputFile = argv[++i];
            else if (argv[i][1] == 'f') inputFile = argv[++i];
            else if (argv[i][1] == 'i') startVertex = stoi(argv[++i]);
            else if (argv[i][1] == 'h') {
                displayHelp();
                return 0;
            }
        }
    }

    if (inputFile.empty()) {
        cerr << "Erro: Nenhum arquivo de entrada especificado." << endl;
        return 1;
    }

    processFile(inputFile, outputFile, startVertex);
    return 0;
}