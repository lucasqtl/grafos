#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>

class AllPairsGraph {
private:
    int vertices;
    std::vector<std::vector<int>> distances;
    std::vector<std::vector<int>> nextVertex;

public:
    AllPairsGraph(int v) : vertices(v) {
        distances.resize(v, std::vector<int>(v, std::numeric_limits<int>::max()));
        nextVertex.resize(v, std::vector<int>(v, -1));
        for (int i = 0; i < v; ++i) {
            distances[i][i] = 0;
        }
    }

    void addEdge(int u, int v, int weight) {
        distances[u][v] = weight;
        distances[v][u] = weight; // Adiciona a aresta no sentido oposto (grafo não direcionado)
        nextVertex[u][v] = v;
        nextVertex[v][u] = u; // Atualiza o próximo vértice nos dois sentidos
    }

    void floydWarshall() {
        for (int k = 0; k < vertices; ++k) {
            for (int i = 0; i < vertices; ++i) {
                for (int j = 0; j < vertices; ++j) {
                    if (distances[i][k] != std::numeric_limits<int>::max() &&
                        distances[k][j] != std::numeric_limits<int>::max() &&
                        distances[i][j] > distances[i][k] + distances[k][j]) {
                        distances[i][j] = distances[i][k] + distances[k][j];
                        nextVertex[i][j] = nextVertex[i][k];
                    }
                }
            }
        }
    }

    void printDistances(int start, bool showPaths, std::ofstream& outFile) {
        for (int j = 0; j < vertices; ++j) {
            int dist = (distances[start][j] == std::numeric_limits<int>::max()) ? -1 : distances[start][j];
            std::cout << (j + 1) << ":" << dist;
            outFile << (j + 1) << ":" << dist;
            if (j < vertices - 1) {
                std::cout << " ";
                outFile << " ";
            }
        }
        std::cout << "\n";
        outFile << "\n";

        if (showPaths) {
            for (int j = 0; j < vertices; ++j) {
                if (distances[start][j] != std::numeric_limits<int>::max() && start != j) {
                    std::cout << "Caminho " << (start + 1) << "->" << (j + 1) << ": ";
                    outFile << "Caminho " << (start + 1) << "->" << (j + 1) << ": ";
                    int u = start, v = j;
                    std::vector<int> path;
                    while (u != v) {
                        path.push_back(u + 1);
                        u = nextVertex[u][v];
                    }
                    path.push_back(v + 1);
                    for (size_t i = 0; i < path.size(); ++i) {
                        std::cout << path[i];
                        outFile << path[i];
                        if (i < path.size() - 1) {
                            std::cout << "->";
                            outFile << "->";
                        }
                    }
                    std::cout << " (custo: " << distances[start][j] << ")\n";
                    outFile << " (custo: " << distances[start][j] << ")\n";
                }
            }
        }
    }
};

void displayHelp() {
    std::cout << "-h : mostra o help\n";
    std::cout << "-o <arquivo> : redireciona a saida para o arquivo\n";
    std::cout << "-f <arquivo> : indica o arquivo que contém o grafo de entrada\n";
    std::cout << "-s : mostra os caminhos\n";
    std::cout << "-i : vértice inicial (para exibir distâncias a partir desse vértice)\n";
}

void processInput(const std::string& inputFile, const std::string& outputFile, int startVertex, bool showPaths) {
    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    if (!inFile || !outFile) {
        std::cerr << "Erro ao abrir arquivos.\n";
        return;
    }

    int v, e;
    inFile >> v >> e;

    AllPairsGraph graph(v);
    for (int i = 0; i < e; ++i) {
        int u, v, w;
        inFile >> u >> v >> w;
        graph.addEdge(u - 1, v - 1, w);
    }

    graph.floydWarshall();
    if (startVertex < 0 || startVertex >= v) {
        startVertex = 0;
    }
    graph.printDistances(startVertex, showPaths, outFile);

    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    std::string inputFile, outputFile = "output.bin";
    bool showPaths = false, showHelp = false;
    int startVertex = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h") {
            showHelp = true;
        } else if (arg == "-f" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-s") {
            showPaths = true;
        } else if (arg == "-i" && i + 1 < argc) {
            startVertex = std::stoi(argv[++i]) - 1;
        }
    }

    if (showHelp) {
        displayHelp();
        return 0;
    }

    if (inputFile.empty()) {
        std::cerr << "Arquivo de entrada não especificado.\n";
        return 1;
    }

    processInput(inputFile, outputFile, startVertex, showPaths);
    return 0;
}