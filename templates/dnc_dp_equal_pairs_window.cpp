#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (1LL << 60);

struct EqualPairsDnCDP {
    int n;
    vector<int> a, freq;
    vector<ll> prevDp, curDp;

    int L = 1, R = 0;
    ll windowCost = 0;

    EqualPairsDnCDP(int n, vector<int> a)
        : n(n), a(move(a)), freq(n + 1), prevDp(n + 1, INF), curDp(n + 1, INF) {}

    void add(int pos) {
        windowCost += freq[a[pos]];
        ++freq[a[pos]];
    }

    void removePos(int pos) {
        --freq[a[pos]];
        windowCost -= freq[a[pos]];
    }

    void moveTo(int l, int r) {
        while (L > l) add(--L);
        while (R < r) add(++R);
        while (L < l) removePos(L++);
        while (R > r) removePos(R--);
    }

    void compute(int l, int r, int optL, int optR) {
        if (l > r) return;

        int mid = (l + r) >> 1;
        pair<ll, int> best = {INF, -1};

        int upper = min(mid - 1, optR);
        for (int j = optL; j <= upper; ++j) {
            moveTo(j + 1, mid);
            ll candidate = prevDp[j] + windowCost;
            if (candidate < best.first)
                best = {candidate, j};
        }

        curDp[mid] = best.first;
        int opt = best.second;

        compute(l, mid - 1, optL, opt);
        compute(mid + 1, r, opt, optR);
    }

    ll solve(int groups) {
        prevDp.assign(n + 1, INF);
        prevDp[0] = 0;

        for (int g = 1; g <= groups; ++g) {
            fill(curDp.begin(), curDp.end(), INF);
            fill(freq.begin(), freq.end(), 0);
            L = 1;
            R = 0;
            windowCost = 0;

            compute(g, n, g - 1, n - 1);
            swap(prevDp, curDp);
        }

        return prevDp[n];
    }
};

/*
Segment cost:
    number of unordered equal-value pairs inside [l, r]

Assumes:
    1 <= a[i] <= n
    1-indexed a
    monotone optimal split points (true for this equal-pairs cost)

Complexity:
    O(k n log n) time
    O(n) memory

This is the core pattern used by CF 868F.
*/
