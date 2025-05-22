#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>

class DirectedGraph {
private:
    int verticesCount;
    std::vector<std::list<int>> edges;
    std::vector<std::list<int>> transposedEdges;

public:
    DirectedGraph(int numVertices) : verticesCount(numVertices) {
        edges.resize(numVertices);
        transposedEdges.resize(numVertices);
    }

    void insertEdge(int from, int to) {
        edges[from].push_back(to);
        transposedEdges[to].push_back(from);
    }

    void firstDFS(int node, std::vector<bool>& visited, std::vector<int>& finishOrder) {
        visited[node] = true;
        for (int next : edges[node]) {
            if (!visited[next]) {
                firstDFS(next, visited, finishOrder);
            }
        }
        finishOrder.push_back(node);
    }

    void secondDFS(int node, std::vector<bool>& visited, std::vector<int>& component) {
        visited[node] = true;
        component.push_back(node + 1);
        for (int next : transposedEdges[node]) {
            if (!visited[next]) {
                secondDFS(next, visited, component);
            }
        }
    }

    void computeSCCs(std::ofstream& outFile) {
        std::vector<bool> visited(verticesCount, false);
        std::vector<int> finishOrder;

        // First DFS to get finishing order
        for (int i = 0; i < verticesCount; i++) {
            if (!visited[i]) {
                firstDFS(i, visited, finishOrder);
            }
        }

        // Reset visited for second DFS
        visited.assign(verticesCount, false);

        // Second DFS on transposed graph
        for (int i = finishOrder.size() - 1; i >= 0; i--) {
            int v = finishOrder[i];
            if (!visited[v]) {
                std::vector<int> component;
                secondDFS(v, visited, component);
                for (size_t j = 0; j < component.size(); j++) {
                    std::cout << component[j];
                    outFile << component[j];
                    if (j < component.size() - 1) {
                        std::cout << " ";
                        outFile << " ";
                    }
                }
                std::cout << std::endl;
                outFile << std::endl;
            }
        }
    }
};

void displayHelp() {
    std::cout << "-h : mostra o help\n";
    std::cout << "-o <arquivo> : redireciona a saida para o arquivo\n";
    std::cout << "-f <arquivo> : indica o arquivo que contém o grafo de entrada\n";
}

int main(int argc, char* argv[]) {
    std::string inputFile, outputFile = "output.bin";
    bool showHelp = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") {
            showHelp = true;
        } else if (std::string(argv[i]) == "-f" && i + 1 < argc) {
            inputFile = argv[++i];
        } else if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            outputFile = argv[++i];
        }
    }

    if (showHelp) {
        displayHelp();
        return 0;
    }

    if (inputFile.empty()) {
        std::cerr << "No input file specified.\n";
        return 1;
    }

    std::ifstream inFile(inputFile);
    if (!inFile) {
        std::cerr << "Error opening input file: " << inputFile << "\n";
        return 1;
    }

    std::ofstream outFile(outputFile);
    if (!outFile) {
        std::cerr << "Error opening output file: " << outputFile << "\n";
        return 1;
    }

    int V, E;
    inFile >> V >> E;
    DirectedGraph graph(V);

    for (int i = 0; i < E; i++) {
        int u, v;
        inFile >> u >> v;
        graph.insertEdge(u - 1, v - 1);
    }

    inFile.close();
    graph.computeSCCs(outFile);
    outFile.close();

    return 0;
}