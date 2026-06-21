#pragma once

#include <vector>
#include <random>
#include <unordered_set>
#include <functional>
#include <cmath>

struct Edge {
    int from;
    int to;
    double weight;
};

struct Point {
    double x;
    double y;
};

class Graph {
public:
    int V;
    std::vector<Edge> edges;

    Graph(int vertices) : V(vertices) {}

    void addEdge(int u, int v, double w) {
        edges.push_back({u, v, w});
    }
};

class GraphGenerator {
private:

    static long long edgeKey(int u, int v) {
        if (u > v)
            std::swap(u, v);

        return (static_cast<long long>(u) << 32) | v;
    }

    static Graph buildGraph(
        int V,
        int E,
        const std::function<double(int, int)>& weightFunction
    ) {
        Graph g(V);

        std::random_device rd;
        std::mt19937 gen(rd());

        std::unordered_set<long long> usedEdges;

        for (int v = 1; v < V; v++) {

            std::uniform_int_distribution<> parentDist(0, v - 1);

            int u = parentDist(gen);

            double w = weightFunction(u, v);

            g.addEdge(u, v, w);

            usedEdges.insert(edgeKey(u, v));
        }

        std::uniform_int_distribution<> vertexDist(0, V - 1);

        while ((int)g.edges.size() < E) {

            int u = vertexDist(gen);
            int v = vertexDist(gen);

            if (u == v)
                continue;

            long long key = edgeKey(u, v);

            if (usedEdges.count(key))
                continue;

            usedEdges.insert(key);

            double w = weightFunction(u, v);

            g.addEdge(u, v, w);
        }

        return g;
    }

public:

    static Graph randomWeights(
        int V,
        int E,
        int minWeight = 1,
        int maxWeight = 100
    ) {

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> weightDist(
            minWeight,
            maxWeight
        );

        auto randomWeight = [&](int, int) {
            return static_cast<double>(
                weightDist(gen)
            );
        };

        return buildGraph(V, E, randomWeight);
    }

    static Graph geometricWeights(
        int V,
        int E,
        int maxCoordinate = 100
    ) {

        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_real_distribution<> coordDist(
            0.0,
            maxCoordinate
        );

        std::vector<Point> points(V);

        for (auto& p : points) {
            p.x = coordDist(gen);
            p.y = coordDist(gen);
        }

        auto geometricWeight =
            [&](int u, int v) {

            double dx = points[u].x - points[v].x;

            double dy = points[u].y - points[v].y;

            return std::sqrt(
                dx * dx + dy * dy
            );
        };

        return buildGraph(V, E, geometricWeight);
    }
};