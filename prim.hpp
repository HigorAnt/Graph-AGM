#pragma once

#include <vector>
#include <queue>
#include "graph.hpp"

class Prim
{
public:
    static std::pair<double, std::vector<Edge>> run(const Graph& g)
    {
        if (g.V == 0) return {0.0, {}};

        std::vector<std::vector<std::pair<int, double>>> adj(g.V);
        for(const auto& e : g.edges)
        {
            adj[e.from].push_back({e.to, e.weight});
            adj[e.to].push_back({e.from, e.weight});
        }

        std::vector<bool> inMST(g.V, false);
        
        typedef std::pair<double, std::pair<int, int>> Node;
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

        pq.push({0.0, {0, -1}});

        double totalWeight = 0;
        std::vector<Edge> mst;

        while(!pq.empty())
        {
            auto [weight, nodeData] = pq.top();
            int u = nodeData.first;
            int parent = nodeData.second;
            pq.pop();

            if(inMST[u]) continue;

            inMST[u] = true;
            totalWeight += weight;

            if(parent != -1)
            {
                mst.push_back({parent, u, weight});
            }

            for(const auto& [v, w] : adj[u])
            {
                if(!inMST[v])
                {
                    pq.push({w, {v, u}});
                }
            }
        }

        return {totalWeight, mst};
    }
};