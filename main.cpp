#include <iostream>
#include <fstream>
#include <string>

#include "Graph.hpp"

void saveGraph(const Graph& g, const std::string& type)
{
    std::string fileName =
        "Graph_" +
        type +
        "_" +
        std::to_string(g.V) +
        "_" +
        std::to_string(g.edges.size()) +
        ".txt";

    std::ofstream out(fileName);

    if (!out.is_open()) {
        std::cerr << "Erro ao criar arquivo.\n";
        return;
    }

    out << g.V << " " << g.edges.size() << '\n';

    for (const auto& edge : g.edges) {
        out << edge.from
            << " "
            << edge.to
            << " "
            << edge.weight
            << '\n';
    }

    out.close();

    std::cout << "Arquivo gerado: "
              << fileName
              << '\n';
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cout
            << "Uso:\n"
            << "graph.exe <random|geometric> <vertices> <arestas>\n";

        return 1;
    }

    std::string type = argv[1];

    int V = std::stoi(argv[2]);
    int E = std::stoi(argv[3]);

    if (E < V - 1) {
        std::cerr
            << "Erro: um grafo conexo precisa de pelo menos "
            << (V - 1)
            << " arestas.\n";

        return 1;
    }

    if (E > V * (V - 1) / 2) {
        std::cerr
            << "Erro: um grafo simples com "
            << V
            << " vertices pode ter no maximo "
            << V * (V - 1) / 2
            << " arestas.\n";

        return 1;
    }

    Graph g(0);

    if (type == "random") {

        g = GraphGenerator::randomWeights(V, E);

    }
    else if (type == "geometric") {

        g = GraphGenerator::geometricWeights(V, E);

    }
    else {

        std::cerr
            << "Tipo invalido. Use random ou geometric.\n";

        return 1;
    }

    saveGraph(g, type);

    return 0;
}