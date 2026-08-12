#include <bits/stdc++.h>
using namespace std;

// Segment tree where each node stores only the K smallest values in its range.
// Useful when a proof shows the answer depends on a bounded candidate set.
//
// CF 1665E MinimizOR uses K = 31 because a[i] < 2^30.
//
// Complexity:
//   build: O(K n) up to merge constants
//   query: O(K log n)
//   memory: O(K n)

struct SmallestKSegTree {
    static constexpr int K = 31;

    int n;
    vector<vector<int>> tree;

    explicit SmallestKSegTree(const vector<int>& a)
        : n((int)a.size()), tree(4 * max(1, (int)a.size())) {
        if (n) build(1, 0, n - 1, a);
    }

    static vector<int> mergeSmallest(const vector<int>& a,
                                     const vector<int>& b) {
        vector<int> res;
        res.reserve(K);

        int i = 0, j = 0;
        while ((i < (int)a.size() || j < (int)b.size()) &&
               (int)res.size() < K) {
            if (j == (int)b.size() ||
                (i < (int)a.size() && a[i] <= b[j])) {
                res.push_back(a[i++]);
            } else {
                res.push_back(b[j++]);
            }
        }

        return res;
    }

    void build(int node, int l, int r, const vector<int>& a) {
        if (l == r) {
            tree[node] = {a[l]};
            return;
        }

        int mid = (l + r) / 2;
        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);
        tree[node] = mergeSmallest(tree[node * 2], tree[node * 2 + 1]);
    }

    vector<int> query(int node, int l, int r, int ql, int qr) const {
        if (qr < l || r < ql) return {};
        if (ql <= l && r <= qr) return tree[node];

        int mid = (l + r) / 2;
        auto left = query(node * 2, l, mid, ql, qr);
        auto right = query(node * 2 + 1, mid + 1, r, ql, qr);
        return mergeSmallest(left, right);
    }

    vector<int> query(int l, int r) const {
        return query(1, 0, n - 1, l, r);
    }
};

/*
Example for MinimizOR:

auto cand = seg.query(l, r);
int ans = INT_MAX;
for (int i = 0; i < (int)cand.size(); ++i)
    for (int j = i + 1; j < (int)cand.size(); ++j)
        ans = min(ans, cand[i] | cand[j]);

Total per query: O(K log n + K^2).
*/
