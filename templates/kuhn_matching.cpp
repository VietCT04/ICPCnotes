#include <bits/stdc++.h>
using namespace std;

// Kuhn algorithm for maximum bipartite matching.
// Left side vertices: 0..L-1
// Right side vertices: 0..R-1
// g[u] contains right-side neighbors of left vertex u.
//
// Worst-case complexity: O(VE) (often written O(L * E)).
// For larger dense instances, prefer Hopcroft-Karp: O(E * sqrt(V)).

struct KuhnMatching {
    int L, R;
    vector<vector<int>> g;
    vector<int> matchRight;
    vector<int> seen;
    int timer = 0;

    KuhnMatching(int L, int R)
        : L(L), R(R), g(L), matchRight(R, -1), seen(L, 0) {}

    void addEdge(int left, int right) {
        g[left].push_back(right);
    }

    bool dfs(int u) {
        if (seen[u] == timer) return false;
        seen[u] = timer;

        for (int v : g[u]) {
            if (matchRight[v] == -1 || dfs(matchRight[v])) {
                matchRight[v] = u;
                return true;
            }
        }

        return false;
    }

    int maximumMatching() {
        int matching = 0;

        for (int u = 0; u < L; ++u) {
            ++timer;
            if (dfs(u)) ++matching;
        }

        return matching;
    }
};

/*
Bipartite identities:

Kőnig:
    minimum vertex cover size = maximum matching size

For every graph:
    maximum independent set size + minimum vertex cover size = |V|

Therefore for bipartite graphs:
    maximum independent set size = |V| - maximum matching size

Augmenting-path viewpoint:
    a matching is maximum iff there is no augmenting path.
*/
