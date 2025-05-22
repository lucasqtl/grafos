#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <limits>

struct EdgeInfo {
    int vertex;
    int weight;
    EdgeInfo(int v, int w) : vertex(v), weight(w) {}
};

class UndirectedGraph {
private:
    int vertexCount;
    std::vector<std::vector<EdgeInfo>> adjacency;

public:
    UndirectedGraph(int vertices) : vertexCount(vertices) {
        adjacency.resize(vertices);
    }

    void insertEdge(int u, int v, int weight) {
        adjacency[u].emplace_back(v, weight);
        adjacency[v].emplace_back(u, weight);
    }

    void computeMST(int startVertex, bool displaySolution, std::ofstream& outFile) {
        std::vector<int> minWeight(vertexCount, std::numeric_limits<int>::max());
        std::vector<int> parent(vertexCount, -1);
        std::vector<bool> inTree(vertexCount, false);

        auto cmp = [](const EdgeInfo& a, const EdgeInfo& b) { return a.weight > b.weight; };
        std::priority_queue<EdgeInfo, std::vector<EdgeInfo>, decltype(cmp)> minQueue(cmp);

        minQueue.emplace(startVertex, 0);
        minWeight[startVertex] = 0;

        while (!minQueue.empty()) {
            int current = minQueue.top().vertex;
            minQueue.pop();

            if (inTree[current]) continue;
            inTree[current] = true;

            for (const auto& edge : adjacency[current]) {
                int neighbor = edge.vertex;
                int edgeWeight = edge.weight;

                if (!inTree[neighbor] && minWeight[neighbor] > edgeWeight) {
                    minWeight[neighbor] = edgeWeight;
                    parent[neighbor] = current;
                    minQueue.emplace(neighbor, minWeight[neighbor]);
                }
            }
        }

        if (displaySolution) {
            for (int i = 0; i < vertexCount; ++i) {
                if (parent[i] != -1) {
                    std::cout << "(" << parent[i] + 1 << "," << i + 1 << ") ";
                    outFile << "(" << parent[i] + 1 << "," << i + 1 << ") ";
                }
            }
            std::cout << "\n";
            outFile << "\n";
        } else {
            int totalCost = 0;
            for (int i = 0; i < vertexCount; ++i) {
                if (minWeight[i] != std::numeric_limits<int>::max()) {
                    totalCost += minWeight[i];
                }
            }
            std::cout << totalCost;
            outFile << totalCost;
        }
    }
};

void displayHelp() {
    std::cout << "-h : mostra o help\n";
    std::cout << "-o <arquivo> : redireciona a saida para o arquivo\n";
    std::cout << "-f <arquivo> : indica o arquivo que contém o grafo de entrada\n";
    std::cout << "-s : mostra a solução\n";
    std::cout << "-i : vértice inicial (para o algoritmo de Prim)\n";
}

void processInput(const std::string& inputPath, const std::string& outputPath, int startVertex, bool showSolution) {
    std::ifstream inFile(inputPath);
    std::ofstream outFile(outputPath);

    if (!inFile || !outFile) {
        std::cerr << "Erro ao abrir arquivos.\n";
        return;
    }

    int vertices, edges;
    inFile >> vertices >> edges;

    UndirectedGraph graph(vertices);
    for (int i = 0; i < edges; ++i) {
        int u, v, weight;
        inFile >> u >> v >> weight;
        graph.insertEdge(u - 1, v - 1, weight);
    }

    // Ajuste para vértice inicial: se não fornecido ou inválido, usa o vértice 0
    if (startVertex < 0 || startVertex >= vertices) {
        startVertex = 0;
    }

    graph.computeMST(startVertex, showSolution, outFile);

    inFile.close();
    outFile.close();
}

int main(int argc, char* argv[]) {
    std::string inputFile, outputFile = "output.bin";
    bool showSolution = false, showHelp = false;
    int initialVertex = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h") {
            showHelp = true;
        } else if (arg == "-f" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        } else if (arg == "-s") {
            showSolution = true;
        } else if (arg == "-i" && i + 1 < argc) {
            initialVertex = std::stoi(argv[++i]) - 1;
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

    processInput(inputFile, outputFile, initialVertex, showSolution);
    return 0;
}