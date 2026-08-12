#include <bits/stdc++.h>
using namespace std;

// Fenwick tree / BIT.
// Point add + prefix/range sum in O(log n).
//
// For range add + point query, store a difference array in the BIT:
//   add(l, +x), add(r+1, -x)
//   point(i) = prefixSum(i)

struct Fenwick {
    using ll = long long;

    int n;
    vector<ll> bit;

    explicit Fenwick(int n) : n(n), bit(n + 1, 0) {}

    void add(int idx, ll delta) {
        for (++idx; idx <= n; idx += idx & -idx) {
            bit[idx] += delta;
        }
    }

    ll prefixSum(int idx) const {
        ll ans = 0;
        for (++idx; idx > 0; idx -= idx & -idx) {
            ans += bit[idx];
        }
        return ans;
    }

    ll rangeSum(int l, int r) const {
        if (l > r) return 0;
        return prefixSum(r) - (l ? prefixSum(l - 1) : 0);
    }

    void rangeAdd(int l, int r, ll delta) {
        if (l > r) return;
        add(l, delta);
        if (r + 1 < n) add(r + 1, -delta);
    }

    ll pointQuery(int idx) const {
        return prefixSum(idx);
    }
};

/*
Typical uses from the notes:

1. Count inversions after coordinate compression.
2. Range-add / point-query compensation when recoloring intervals.

Complexity:
    update/query: O(log n)
    memory: O(n)
*/
