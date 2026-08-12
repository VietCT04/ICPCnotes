#include <bits/stdc++.h>
using namespace std;

// Hopcroft-Karp maximum bipartite matching.
// Left vertices: 0..L-1
// Right vertices: 0..R-1
// Complexity: O(E * sqrt(V)).

struct HopcroftKarp {
    int L, R;
    vector<vector<int>> g;
    vector<int> dist;
    vector<int> matchLeft, matchRight;

    HopcroftKarp(int L, int R)
        : L(L), R(R), g(L), dist(L),
          matchLeft(L, -1), matchRight(R, -1) {}

    void addEdge(int u, int v) {
        g[u].push_back(v);
    }

    bool bfs() {
        queue<int> q;
        fill(dist.begin(), dist.end(), -1);

        for (int u = 0; u < L; ++u) {
            if (matchLeft[u] == -1) {
                dist[u] = 0;
                q.push(u);
            }
        }

        bool foundAugmentingPath = false;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                int nextLeft = matchRight[v];
                if (nextLeft == -1) {
                    foundAugmentingPath = true;
                } else if (dist[nextLeft] == -1) {
                    dist[nextLeft] = dist[u] + 1;
                    q.push(nextLeft);
                }
            }
        }

        return foundAugmentingPath;
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            int nextLeft = matchRight[v];

            if (nextLeft == -1 ||
                (dist[nextLeft] == dist[u] + 1 && dfs(nextLeft))) {
                matchLeft[u] = v;
                matchRight[v] = u;
                return true;
            }
        }

        dist[u] = -1;
        return false;
    }

    int maximumMatching() {
        int matching = 0;

        while (bfs()) {
            for (int u = 0; u < L; ++u) {
                if (matchLeft[u] == -1 && dfs(u)) {
                    ++matching;
                }
            }
        }

        return matching;
    }
};
