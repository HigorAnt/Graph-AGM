#pragma once

#include <vector>

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

        if (root_i == root_j)
            return;

        if (rank[root_i] < rank[root_j])
        {
            parent[root_i] = root_j;
        }
        else if (rank[root_i] > rank[root_j])
        {
            parent[root_j] = root_i;
        }
        else
        {
            parent[root_j] = root_i;
            rank[root_i]++;
        }
    }
};