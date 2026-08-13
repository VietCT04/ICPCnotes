#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (1LL << 62);

struct DivideConquerDP {
    int n;
    vector<ll> prevDp, curDp;

    DivideConquerDP(int n) : n(n), prevDp(n + 1, INF), curDp(n + 1, INF) {}

    ll cost(int l, int r) {
        return 0;
    }

    void compute(int l, int r, int optL, int optR) {
        if (l > r) return;

        int mid = (l + r) >> 1;
        pair<ll, int> best = {INF, -1};

        int upper = min(mid - 1, optR);
        for (int j = optL; j <= upper; ++j) {
            ll candidate = prevDp[j] + cost(j + 1, mid);
            if (candidate < best.first)
                best = {candidate, j};
        }

        curDp[mid] = best.first;
        int opt = best.second;

        if (l <= mid - 1)
            compute(l, mid - 1, optL, opt);
        if (mid + 1 <= r)
            compute(mid + 1, r, opt, optR);
    }
};

/*
DP form:
    dp[g][i] = min_{j < i}(dp[g-1][j] + C(j+1, i))

Requirement:
    opt[g][i] <= opt[g][i+1]

Typical sufficient condition:
    C satisfies the quadrangle / Monge inequality.

Complexity per layer:
    O(n log n * Tcost)

If C is O(1):
    O(n log n) per layer
    O(k n log n) total

The cost() function is intentionally left problem-specific.
*/
