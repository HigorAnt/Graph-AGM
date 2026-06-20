#pragma once

#include <vector>
#include <algorithm>

#include "graph.hpp"
#include "dsu.hpp"

class Kruskal
{
public:

    static std::pair<double, std::vector<Edge>>
    run(const Graph& g)
    {
        std::vector<Edge> edges = g.edges;

        std::sort(
            edges.begin(),
            edges.end(),
            [](const Edge& a, const Edge& b)
            {
                return a.weight < b.weight;
            }
        );

        DSU dsu(g.V);

        std::vector<Edge> mst;

        double totalWeight = 0;

        for(const auto& edge : edges)
        {
            if(dsu.find(edge.from)
                != dsu.find(edge.to))
            {
                mst.push_back(edge);

                totalWeight += edge.weight;

                dsu.unite(
                    edge.from,
                    edge.to
                );
            }
        }

        return {
            totalWeight,
            mst
        };
    }
};