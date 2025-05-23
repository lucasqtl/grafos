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
        nextVertex[u][v] = v;
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

    void printDistances(bool showPaths, std::ofstream& outFile) {
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (distances[i][j] == std::numeric_limits<int>::max()) {
                    std::cout << "-";
                    outFile << "-";
                } else {
                    std::cout << distances[i][j];
                    outFile << distances[i][j];
                }
                if (j < vertices - 1) {
                    std::cout << " ";
                    outFile << " ";
                }
            }
            std::cout << "\n";
            outFile << "\n";
        }

        if (showPaths) {
            for (int i = 0; i < vertices; ++i) {
                for (int j = 0; j < vertices; ++j) {
                    if (distances[i][j] != std::numeric_limits<int>::max() && i != j) {
                        std::cout << "Caminho " << (i + 1) << "->" << (j + 1) << ": ";
                        outFile << "Caminho " << (i + 1) << "->" << (j + 1) << ": ";
                        int u = i, v = j;
                        std::vector<int> path;
                        while (u != v) {
                            path.push_back(u + 1);
                            u = nextVertex[u][v];
                        }
                        path.push_back(v + 1);
                        for (size_t k = 0; k < path.size(); ++k) {
                            std::cout << path[k];
                            outFile << path[k];
                            if (k < path.size() - 1) {
                                std::cout << "->";
                                outFile << "->";
                            }
                        }
                        std::cout << " (custo: " << distances[i][j] << ")\n";
                        outFile << " (custo: " << distances[i][j] << ")\n";
                    }
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
}

void processInput(const std::string& inputFile, const std::string& outputFile, bool showPaths) {
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
    graph.printDistances(showPaths, outFile);

    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    std::string inputFile, outputFile = "output.bin";
    bool showPaths = false, showHelp = false;

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

    processInput(inputFile, outputFile, showPaths);
    return 0;
}