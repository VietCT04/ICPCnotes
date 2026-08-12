#include <bits/stdc++.h>
using namespace std;

// Dinic max-flow / min-cut template.
//
// General worst-case complexity: O(V^2 E).
// In many competitive-programming flow networks it is much faster in practice.
// For unit-capacity networks there are stronger bounds.
//
// After maxFlow(s, t), the returned value is also the minimum s-t cut value.
// To recover the source side of one minimum cut, run DFS/BFS from s using
// residual edges with cap > 0.

struct Dinic {
    using ll = long long;

    struct Edge {
        int to;
        int rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, ptr;

    explicit Dinic(int n) : n(n), g(n), level(n), ptr(n) {}

    void addEdge(int u, int v, ll cap) {
        Edge forward{v, (int)g[v].size(), cap};
        Edge reverse{u, (int)g[u].size(), 0};
        g[u].push_back(forward);
        g[v].push_back(reverse);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(s);
        level[s] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const Edge& e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    ll dfs(int u, int t, ll pushed) {
        if (u == t || pushed == 0) return pushed;

        for (int& cid = ptr[u]; cid < (int)g[u].size(); ++cid) {
            Edge& e = g[u][cid];

            if (e.cap == 0 || level[e.to] != level[u] + 1) continue;

            ll flow = dfs(e.to, t, min(pushed, e.cap));
            if (flow == 0) continue;

            e.cap -= flow;
            g[e.to][e.rev].cap += flow;
            return flow;
        }

        return 0;
    }

    ll maxFlow(int s, int t) {
        const ll INF = numeric_limits<ll>::max() / 4;
        ll flow = 0;

        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (ll pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }

        return flow;
    }

    vector<int> minCutSourceSide(int s) const {
        vector<int> vis(n, 0);
        queue<int> q;
        q.push(s);
        vis[s] = 1;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const Edge& e : g[u]) {
                if (e.cap > 0 && !vis[e.to]) {
                    vis[e.to] = 1;
                    q.push(e.to);
                }
            }
        }

        return vis;
    }
};

/*
Binary-decision interpretation when SOURCE side = TRUE / selected:

    source --p--> X
        pay p if X is FALSE / rejected

    X --c--> sink
        pay c if X is TRUE / selected

    A --INF--> B
        hard implication A => B

    A --c--> B
        soft implication: pay c only for A=TRUE, B=FALSE

Maximum-weight closure:
    answer = sum(all positive profits) - mincut
*/
