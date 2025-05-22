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

struct Connection {
    int start, end, weight;
    Connection(int s, int e, int w) : start(s), end(e), weight(w) {}
};

class GraphNetwork {
private:
    int nodes;
    std::vector<Connection> mstEdges;

public:
    GraphNetwork(int n) : nodes(n) {}

    void addToMST(int s, int e, int w) {
        mstEdges.emplace_back(s, e, w);
    }

    std::vector<Connection> getMSTEdges() const {
        return mstEdges;
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

    auto cmp = [](const Connection& a, const Connection& b) { return a.weight > b.weight; };
    std::priority_queue<Connection, std::vector<Connection>, decltype(cmp)> edgeQueue(cmp);

    for (int i = 0; i < edges; ++i) {
        int u, v, w;
        in >> u >> v >> w;
        edgeQueue.emplace(u, v, w);
    }

    GraphNetwork mst(vertices);
    DisjointSet ds(vertices);
    int totalWeight = 0;

    while (!edgeQueue.empty()) {
        Connection edge = edgeQueue.top();
        edgeQueue.pop();
        if (ds.findRoot(edge.start) != ds.findRoot(edge.end)) {
            mst.addToMST(edge.start, edge.end, edge.weight);
            ds.unite(edge.start, edge.end);
            totalWeight += edge.weight;
        }
    }

    if (displaySolution) {
        for (const auto& edge : mst.getMSTEdges()) {
            out << "(" << edge.start << "," << edge.end << ") ";
            std::cout << "(" << edge.start << "," << edge.end << ") ";
        }
        std::cout << "\n";
        out << "\n";
    } else {
        out << totalWeight;
        std::cout << totalWeight; // Removido o "\n" para evitar que o script interprete a nova linha
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