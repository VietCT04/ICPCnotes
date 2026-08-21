#include <bits/stdc++.h>
using namespace std;

// Min-Cost Max-Flow using successive shortest augmenting paths,
// Dijkstra, and Johnson potentials.
//
// Complexity: O(F * E log V) when capacities are integral and each
// augmentation sends at least one unit, where F is the total flow sent.
// More precisely: O(A * E log V), where A is the number of augmentations.
//
// This version assumes all original forward edge costs are non-negative,
// so initial potentials can be zero. Reverse residual edges may have negative
// costs; the potentials keep all reachable reduced costs non-negative.

struct MinCostMaxFlow {
    using ll = long long;

    struct Edge {
        int to;
        int rev;
        ll cap;
        ll cost;
    };

    int n;
    vector<vector<Edge>> g;

    explicit MinCostMaxFlow(int n) : n(n), g(n) {}

    void addEdge(int u, int v, ll cap, ll cost) {
        Edge forward{v, (int)g[v].size(), cap, cost};
        Edge reverse{u, (int)g[u].size(), 0, -cost};
        g[u].push_back(forward);
        g[v].push_back(reverse);
    }

    pair<ll, ll> minCostFlow(int s, int t, ll maxFlow) {
        const ll INF = numeric_limits<ll>::max() / 4;
        vector<ll> potential(n, 0), dist(n);
        vector<int> parentVertex(n), parentEdge(n);
        ll flow = 0, totalCost = 0;

        while (flow < maxFlow) {
            fill(dist.begin(), dist.end(), INF);
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();
                if (d != dist[u]) continue;

                for (int i = 0; i < (int)g[u].size(); ++i) {
                    const Edge& e = g[u][i];
                    if (e.cap == 0) continue;
                    ll nd = d + e.cost + potential[u] - potential[e.to];
                    if (nd >= dist[e.to]) continue;
                    dist[e.to] = nd;
                    parentVertex[e.to] = u;
                    parentEdge[e.to] = i;
                    pq.push({nd, e.to});
                }
            }

            if (dist[t] == INF) break;

            for (int v = 0; v < n; ++v) {
                if (dist[v] != INF) potential[v] += dist[v];
            }

            ll pushed = maxFlow - flow;
            for (int v = t; v != s; v = parentVertex[v]) {
                pushed = min(pushed, g[parentVertex[v]][parentEdge[v]].cap);
            }

            flow += pushed;
            totalCost += pushed * (potential[t] - potential[s]);

            for (int v = t; v != s; v = parentVertex[v]) {
                Edge& e = g[parentVertex[v]][parentEdge[v]];
                e.cap -= pushed;
                g[v][e.rev].cap += pushed;
            }
        }

        return {flow, totalCost};
    }

    pair<ll, ll> minCostMaxFlow(int s, int t) {
        const ll INF = numeric_limits<ll>::max() / 4;
        return minCostFlow(s, t, INF);
    }
};
