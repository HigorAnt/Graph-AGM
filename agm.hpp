#pragma once

#include <vector>
#include <algorithm>
#include <queue>
#include <utility>
#include "graph.hpp"

class DSU
{
  std::vector<int> parent;
  std::vector<int> rank;

public:
  DSU(int n)
  {
    parent.resize(n);
    rank.resize(n, 0);
    for (int i = 0; i < n; i++)
      parent[i] = i;
  }

  int find(int i)
  {
    if (parent[i] == i)
      return i;
    return parent[i] = find(parent[i]);
  }

  void unite(int i, int j)
  {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j)
    {
      if (rank[root_i] < rank[root_j])
        parent[root_i] = root_j;
      else if (rank[root_i] > rank[root_j])
        parent[root_j] = root_i;
      else
      {
        parent[root_j] = root_i;
        rank[root_i]++;
      }
    }
  }
};

class AGM
{
public:
  // Kruskal
  static std::pair<double, std::vector<Edge>> kruskal(const Graph &g)
  {
    if (g.V <= 1)
      return {0.0, {}};

    std::vector<Edge> edges = g.edges;

    std::sort(edges.begin(), edges.end(), [](const Edge &a, const Edge &b)
              { return a.weight < b.weight; });

    DSU dsu(g.V);
    std::vector<Edge> mst;
    double mst_weight = 0.0;

    for (const auto &edge : edges)
    {
      if (dsu.find(edge.from) != dsu.find(edge.to))
      {
        dsu.unite(edge.from, edge.to);
        mst.push_back(edge);
        mst_weight += edge.weight;
      }
    }

    return {mst_weight, mst};
  }

  // Prim
  static std::pair<double, std::vector<Edge>> prim(const Graph &g)
  {
    if (g.V <= 1)
      return {0.0, {}};

    std::vector<std::vector<std::pair<int, double>>> adj(g.V);
    for (const auto &edge : g.edges)
    {
      adj[edge.from].push_back({edge.to, edge.weight});
      adj[edge.to].push_back({edge.from, edge.weight});
    }

    double mst_weight = 0.0;
    std::vector<Edge> mst;
    std::vector<bool> inMST(g.V, false);

    using pdi = std::pair<double, std::pair<int, int>>;
    std::priority_queue<pdi, std::vector<pdi>, std::greater<pdi>> pq;

    inMST[0] = true;
    for (const auto &vizinho : adj[0])
    {
      pq.push({vizinho.second, {0, vizinho.first}});
    }

    while (!pq.empty() && mst.size() < (size_t)(g.V - 1))
    {
      auto top = pq.top();
      pq.pop();

      double weight = top.first;
      int u = top.second.first;
      int v = top.second.second;

      if (inMST[v])
        continue;

      inMST[v] = true;
      mst_weight += weight;
      mst.push_back({u, v, weight});

      for (const auto &vizinho : adj[v])
      {
        if (!inMST[vizinho.first])
        {
          pq.push({vizinho.second, {v, vizinho.first}});
        }
      }
    }

    return {mst_weight, mst};
  }
};
