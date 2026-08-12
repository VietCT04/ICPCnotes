#include <bits/stdc++.h>
using namespace std;

// Specialized lazy segment tree for range updates that add
// F1, F2, F3, ... = 1, 1, 2, 3, ... aligned to the update's left endpoint.
// Also supports range-sum queries.
//
// Core reusable idea:
//   lazy[node] = (a,b), the first two terms of a Fibonacci-like sequence
//   that must be added starting at this node's left boundary.
//
// Complexity:
//   build: O(n)
//   range update: O(log n)
//   range sum query: O(log n)
//   memory: O(n)

struct FibonacciLazySegTree {
    using ll = long long;

    int n;
    ll MOD;
    vector<ll> fib;
    vector<ll> tree;
    vector<pair<ll,ll>> lazy;

    FibonacciLazySegTree(const vector<ll>& a, ll mod)
        : n((int)a.size()), MOD(mod), fib(n + 5),
          tree(4 * max(1, n)), lazy(4 * max(1, n), {0, 0}) {
        precomputeFib();
        if (n) build(1, 0, n - 1, a);
    }

    void precomputeFib() {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < (int)fib.size(); ++i) {
            fib[i] = (fib[i - 1] + fib[i - 2]) % MOD;
        }
    }

    void build(int node, int l, int r, const vector<ll>& a) {
        if (l == r) {
            tree[node] = a[l] % MOD;
            return;
        }

        int mid = (l + r) / 2;
        build(node * 2, l, mid, a);
        build(node * 2 + 1, mid + 1, r, a);
        pull(node);
    }

    void pull(int node) {
        tree[node] = (tree[node * 2] + tree[node * 2 + 1]) % MOD;
    }

    // G1=a, G2=b, Gk=G(k-1)+G(k-2).
    ll term(ll a, ll b, int k) const {
        if (k == 1) return a;
        return (a * fib[k - 2] + b * fib[k - 1]) % MOD;
    }

    // Sum G1 + ... + Glen.
    ll sequenceSum(ll a, ll b, int len) const {
        ll secondCoeff = (fib[len + 1] - 1 + MOD) % MOD;
        return (a * fib[len] + b * secondCoeff) % MOD;
    }

    void apply(int node, int len, ll a, ll b) {
        tree[node] = (tree[node] + sequenceSum(a, b, len)) % MOD;
        lazy[node].first = (lazy[node].first + a) % MOD;
        lazy[node].second = (lazy[node].second + b) % MOD;
    }

    void push(int node, int l, int r) {
        auto [a, b] = lazy[node];
        if ((a == 0 && b == 0) || l == r) return;

        int mid = (l + r) / 2;
        int leftLen = mid - l + 1;
        int rightLen = r - mid;

        // Left child starts with the same pair.
        apply(node * 2, leftLen, a, b);

        // Right child starts after leftLen terms.
        ll rightA = term(a, b, leftLen + 1);
        ll rightB = term(a, b, leftLen + 2);
        apply(node * 2 + 1, rightLen, rightA, rightB);

        lazy[node] = {0, 0};
    }

    // Add F1,F2,... across [ql,qr], aligned so position ql gets F1.
    void update(int ql, int qr) {
        update(1, 0, n - 1, ql, qr, ql);
    }

    void update(int node, int l, int r, int ql, int qr, int updateLeft) {
        if (qr < l || r < ql) return;

        if (ql <= l && r <= qr) {
            int offset = l - updateLeft + 1; // 1-based Fibonacci offset
            ll a = fib[offset];
            ll b = fib[offset + 1];
            apply(node, r - l + 1, a, b);
            return;
        }

        push(node, l, r);
        int mid = (l + r) / 2;
        update(node * 2, l, mid, ql, qr, updateLeft);
        update(node * 2 + 1, mid + 1, r, ql, qr, updateLeft);
        pull(node);
    }

    ll query(int ql, int qr) {
        return query(1, 0, n - 1, ql, qr);
    }

    ll query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return tree[node];

        push(node, l, r);
        int mid = (l + r) / 2;
        return (query(node * 2, l, mid, ql, qr) +
                query(node * 2 + 1, mid + 1, r, ql, qr)) % MOD;
    }
};

/*
Useful identities for G1=a, G2=b:

    Gk = a*F(k-2) + b*F(k-1), k >= 2

    sum_{i=1..len} Gi
      = a*F(len) + b*(F(len+1)-1)

This pattern generalizes: if a lazy update follows a small linear recurrence,
store the recurrence state at the node boundary and derive the shifted state
when pushing to the right child.
*/
