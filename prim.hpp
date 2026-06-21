#pragma once

#include <vector>
#include <queue>

#include "graph.hpp"

class Prim
{
public:

    static std::pair<double, std::vector<Edge>>
    run(const Graph& g)
    {
        std::vector<std::vector<std::pair<int,double>>> adj(g.V);

        for(const auto& e : g.edges)
        {
            adj[e.from].push_back(
                {e.to, e.weight}
            );

            adj[e.to].push_back(
                {e.from, e.weight}
            );
        }

        std::vector<bool>
            inMST(g.V, false);

        std::priority_queue<
            std::pair<double,int>,
            std::vector<
                std::pair<double,int>
            >,
            std::greater<
                std::pair<double,int>
            >
        > pq;

        pq.push({0,0});

        double totalWeight = 0;

        std::vector<Edge> mst;

        while(!pq.empty())
        {
            auto [weight, u] = pq.top();

            pq.pop();

            if(inMST[u])
                continue;

            inMST[u] = true;

            totalWeight += weight;

            for(const auto&
                [v, w] : adj[u])
            {
                if(!inMST[v])
                {
                    pq.push({w,v});
                }
            }
        }

        return {
            totalWeight,
            mst
        };
    }
};