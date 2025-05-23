#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <string>

class DisjointSet {
private:
    std::vector<int> parent;
    std::vector<int> size;

public:
    DisjointSet(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
        }
    }

    int findRoot(int x) {
        if (parent[x] != x) {
            parent[x] = findRoot(parent[x]);
        }
        return parent[x];
    }

    void unite(int x, int y) {
        int rootX = findRoot(x);
        int rootY = findRoot(y);
        if (rootX != rootY) {
            if (size[rootX] < size[rootY]) {
                parent[rootX] = rootY;
                size[rootY] += size[rootX];
            } else {
                parent[rootY] = rootX;
                size[rootX] += size[rootY];
            }
        }
    }
};

class GraphNetwork {
private:
    int nodes;
    std::vector<std::vector<std::pair<int, int>>> adjList;
    std::vector<std::pair<std::pair<int, int>, int>> mstEdges;

public:
    GraphNetwork(int n) : nodes(n) {
        adjList.resize(n + 1);
    }

    void addEdge(int u, int v, int w) {
        adjList[u].emplace_back(v, w);
        adjList[v].emplace_back(u, w);
    }

    void addToMST(int s, int e, int w) {
        mstEdges.emplace_back(std::make_pair(s, e), w);
    }

    std::vector<std::pair<std::pair<int, int>, int>> getMSTEdges() const {
        return mstEdges;
    }

    std::vector<std::vector<std::pair<int, int>>> getAdjList() const {
        return adjList;
    }
};

void showUsage() {
    std::cout << "-h : mostra o help\n";
    std::cout << "-o <arquivo> : redireciona a saída para o arquivo\n";
    std::cout << "-f <arquivo> : indica o arquivo que contém o grafo de entrada\n";
    std::cout << "-s : mostra a solução\n";
    std::cout << "-i : vértice inicial (não usado para Kruskal, mas suportado)\n";
}

void processGraph(const std::string& inputFile, const std::string& outputFile, bool displaySolution) {
    std::ifstream in(inputFile);
    std::ofstream out(outputFile);

    if (!in || !out) {
        std::cerr << "Erro ao abrir arquivos.\n";
        return;
    }

    int vertices, edges;
    in >> vertices >> edges;

    GraphNetwork graph(vertices);
    auto cmp = [](const std::pair<std::pair<int, int>, int>& a, const std::pair<std::pair<int, int>, int>& b) { return a.second > b.second; };
    std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, decltype(cmp)> edgeQueue(cmp);

    for (int i = 0; i < edges; ++i) {
        int u, v, w;
        in >> u >> v >> w;
        graph.addEdge(u, v, w);
        edgeQueue.emplace(std::make_pair(u, v), w);
    }

    DisjointSet ds(vertices);
    int totalWeight = 0;

    while (!edgeQueue.empty()) {
        auto edge = edgeQueue.top();
        edgeQueue.pop();
        int u = edge.first.first;
        int v = edge.first.second;
        int w = edge.second;
        if (ds.findRoot(u) != ds.findRoot(v)) {
            graph.addToMST(u, v, w);
            ds.unite(u, v);
            totalWeight += w;
        }
    }

    if (displaySolution) {
        for (const auto& edge : graph.getMSTEdges()) {
            out << "(" << edge.first.first << "," << edge.first.second << ") ";
            std::cout << "(" << edge.first.first << "," << edge.first.second << ") ";
        }
        std::cout << "\n";
        out << "\n";
    } else {
        out << totalWeight;
        std::cout << totalWeight;
    }

    in.close();
    out.close();
}

int main(int argc, char* argv[]) {
    std::string inFile, outFile = "output.bin";
    bool showSol = false, showHelp = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h") {
            showHelp = true;
        } else if (arg == "-f" && i + 1 < argc) {
            inFile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outFile = argv[++i];
        } else if (arg == "-s") {
            showSol = true;
        }
    }

    if (showHelp) {
        showUsage();
        return 0;
    }

    if (inFile.empty()) {
        std::cerr << "Arquivo de entrada não especificado.\n";
        return 1;
    }

    processGraph(inFile, outFile, showSol);
    return 0;
}