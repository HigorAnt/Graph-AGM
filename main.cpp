#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "graph.hpp"
#include "agm.hpp"

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

    std::cout << "\nCalculando as Arvores Geradoras Minimas...\n";

    // --- TESTANDO KRUSKAL E MEDINDO O TEMPO ---
    auto start_kruskal = std::chrono::high_resolution_clock::now();
    auto resultadoKruskal = AGM::kruskal(g);
    auto end_kruskal = std::chrono::high_resolution_clock::now();

    // Calculando a diferença em milissegundos
    std::chrono::duration<double, std::milli> tempo_kruskal = end_kruskal - start_kruskal;

    std::cout << "\n--- Resultado Kruskal ---\n";
    std::cout << "Peso Total da AGM: " << resultadoKruskal.first << "\n";
    std::cout << "Qtd de Arestas na AGM: " << resultadoKruskal.second.size() << "\n";
    std::cout << "Tempo de execucao: " << tempo_kruskal.count() << " ms\n";


    // --- TESTANDO PRIM E MEDINDO O TEMPO ---
    auto start_prim = std::chrono::high_resolution_clock::now();
    auto resultadoPrim = AGM::prim(g);
    auto end_prim = std::chrono::high_resolution_clock::now();

    // Calculando a diferença em milissegundos
    std::chrono::duration<double, std::milli> tempo_prim = end_prim - start_prim;

    std::cout << "\n--- Resultado Prim ---\n";
    std::cout << "Peso Total da AGM: " << resultadoPrim.first << "\n";
    std::cout << "Qtd de Arestas na AGM: " << resultadoPrim.second.size() << "\n";
    std::cout << "Tempo de execucao: " << tempo_prim.count() << " ms\n\n";

    return 0;
}
