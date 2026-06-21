#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <cmath>
#include "graph.hpp"
#include "kruskal.hpp"
#include "prim.hpp"

void saveGraph(const Graph &g, const std::string &type)
{
    std::string fileName = "Graph_" + type + "_" + std::to_string(g.V) + "_" + std::to_string(g.edges.size()) + ".txt";
    std::ofstream out(fileName);

    if (!out.is_open())
    {
        std::cerr << "Erro ao criar arquivo.\n";
        return;
    }

    out << g.V << " " << g.edges.size() << '\n';

    for (const auto &edge : g.edges)
    {
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

Graph loadGraph(const std::string& fileName)
{
    std::ifstream in(fileName);

    int V;
    int E;

    in >> V >> E;

    Graph g(V);

    for(int i = 0; i < E; i++)
    {
        int u;
        int v;
        double w;

        in >> u >> v >> w;

        g.addEdge(u, v, w);
    }

    in.close();

    return g;
}

void saveResult(
    const std::string& tipo,
    int vertices,
    int arestas,
    const std::string& algoritmo,
    double tempo, 
    double desvio
)
{
    std::ifstream check("resultados.csv");
    bool arquivoExiste = check.good();
    check.close();

    std::ofstream out(
        "resultados.csv",
        std::ios::app
    );

    if (!arquivoExiste)
    {
        out << "tipo,vertices,arestas,algoritmo,tempo_ms,desvio_ms\n";
    }

    out << tipo << ","
        << vertices << ","
        << arestas << ","
        << algoritmo << ","
        << tempo << ","
        << desvio
        << "\n";

    out.close();
}

double calcularDesvioPadrao(const std::vector<double>& tempos, double media){
    if(tempos.size() < 2)
        return 0.0;

    double soma = 0;

    for(double t : tempos)
    {
        soma += (t - media) * (t - media);
    }

    return std::sqrt(
        soma / (tempos.size() - 1)
    );
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cout
            << "Uso:\n"
            << "graph.exe <random|geometric> <vertices> <arestas> <execucao>\n";

        return 1;
    }

    std::string type = argv[1];

    int V = std::stoi(argv[2]);
    int E = std::stoi(argv[3]);
    int N = std::stoi(argv[4]);

    std::string fileName ="Graph_" + type + "_" + std::to_string(V) + "_" + std::to_string(E) + ".txt";

    if (E < V - 1)
    {
        std::cerr
            << "Erro: um grafo conexo precisa de pelo menos "
            << (V - 1)
            << " arestas.\n";

        return 1;
    }

    if (E > V * (V - 1) / 2)
    {
        std::cerr
            << "Erro: um grafo simples com "
            << V
            << " vertices pode ter no maximo "
            << V * (V - 1) / 2
            << " arestas.\n";

        return 1;
    }

    Graph g(0);

std::ifstream test(fileName);

bool fileExists = test.good();

test.close();

    if(fileExists)
    {
        std::cout << "Instancia encontrada. Reutilizando arquivo.\n";

        g = loadGraph(fileName);
    }
    else
    {
        std::cout << "Instancia inexistente. Gerando novo grafo.\n";

        if(type == "random")
        {
            g = GraphGenerator::randomWeights(V, E);
        }
        else if(type == "geometric")
        {
            g = GraphGenerator::geometricWeights(V, E);
        }
        else
        {
            std::cerr << "Tipo invalido. Use random ou geometric.\n";

            return 1;
        }

    saveGraph(g, type);
}

    std::cout << "\nCalculando as Arvores Geradoras Minimas...\n";
    std::vector<double> temposKruskal;
    std::pair<double, std::vector<Edge>> resultadoKruskal;

    for (int i = 0; i < N; i++)
    {
        auto start_kruskal = std::chrono::high_resolution_clock::now();
        resultadoKruskal = Kruskal::run(g);
        auto end_kruskal = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> tempo_kruskal = end_kruskal - start_kruskal;

        temposKruskal.push_back(tempo_kruskal.count());
    }

    double somaKruskal = 0;

    for (double t : temposKruskal)
    {
        somaKruskal += t;
    }

    double mediaKruskal = somaKruskal / temposKruskal.size();
    double desvioKruskal = calcularDesvioPadrao(temposKruskal, mediaKruskal);
    saveResult(type, V, E, "Kruskal", mediaKruskal, desvioKruskal);
    std::cout << "\n--- Resultado Kruskal ---\n";
    std::cout << "Peso Total da AGM: "
              << resultadoKruskal.first
              << "\n";

    std::cout << "Qtd de Arestas na AGM: "
              << resultadoKruskal.second.size()
              << "\n";

    std::cout << "Tempo medio: "
              << mediaKruskal
              << " ms\n";
    std::cout << "Desvio padrao: "
          << desvioKruskal << " ms\n";

    std::vector<double> temposPrim;
    std::pair<double, std::vector<Edge>> resultadoPrim;

    for (int i = 0; i < N; i++)
    {
        auto start_prim = std::chrono::high_resolution_clock::now();
        resultadoPrim = Prim::run(g);
        auto end_prim = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> tempo_prim = end_prim - start_prim;

        temposPrim.push_back(tempo_prim.count());
    }

    double somaPrim = 0;

    for (double t : temposPrim)
    {
        somaPrim += t;
    }

    double mediaPrim = somaPrim / temposPrim.size();
    double desvioPrim = calcularDesvioPadrao(temposPrim, mediaPrim);
    saveResult(type, V, E, "Prim", mediaPrim, desvioPrim);

    std::cout << "\n--- Resultado Prim ---\n";
    std::cout << "Peso Total da AGM: "
              << resultadoPrim.first
              << "\n";

    std::cout << "Qtd de Arestas na AGM: "
              << resultadoPrim.second.size()
              << "\n";

    std::cout << "Tempo medio: "
              << mediaPrim
              << " ms\n\n";
    std::cout << "Desvio padrao: "
          << desvioPrim << " ms\n";

    return 0;
}
