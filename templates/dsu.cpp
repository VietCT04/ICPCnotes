#include <bits/stdc++.h>
using namespace std;

// Disjoint Set Union / Union-Find.
//
// Amortized complexity per operation: O(alpha(n)), effectively constant.
// Common uses: Kruskal MST/MSF, connectivity while adding edges, component sizes.

struct DSU {
    vector<int> parent, sz;

    explicit DSU(int n) : parent(n), sz(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        return true;
    }

    int size(int x) {
        return sz[find(x)];
    }
};

/*
Maximum spanning forest pattern:

sort edges by descending weight;
for each (w,u,v):
    if (dsu.unite(u,v)) kept += w;

If the original problem asks for minimum deleted weight so the remaining graph
is acyclic:
    answer = total_edge_weight - kept
*/
