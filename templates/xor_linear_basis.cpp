#include <bits/stdc++.h>
using namespace std;

template <int B = 31>
struct XorBasis {
    array<int, B> basis{};

    bool insert(int x) {
        for (int b = B - 1; b >= 0; --b) {
            if (!(x & (1 << b))) continue;
            if (!basis[b]) {
                basis[b] = x;
                return true;
            }
            x ^= basis[b];
        }
        return false;
    }

    bool canRepresent(int x) const {
        for (int b = B - 1; b >= 0; --b) {
            if (!(x & (1 << b))) continue;
            if (!basis[b]) return false;
            x ^= basis[b];
        }
        return true;
    }

    int maxXor(int seed = 0) const {
        int ans = seed;
        for (int b = B - 1; b >= 0; --b)
            ans = max(ans, ans ^ basis[b]);
        return ans;
    }
};

template <int B = 31>
struct IndexedXorBasis {
    array<int, B> basis{};
    array<int, B> pos{};

    IndexedXorBasis() {
        pos.fill(-1);
    }

    void insert(int x, int idx) {
        for (int b = B - 1; b >= 0; --b) {
            if (!(x & (1 << b))) continue;

            if (!basis[b]) {
                basis[b] = x;
                pos[b] = idx;
                return;
            }

            if (idx > pos[b]) {
                swap(x, basis[b]);
                swap(idx, pos[b]);
            }

            x ^= basis[b];
        }
    }

    int maxXorFrom(int left, int seed = 0) const {
        int ans = seed;
        for (int b = B - 1; b >= 0; --b) {
            if (pos[b] < left) continue;
            ans = max(ans, ans ^ basis[b]);
        }
        return ans;
    }
};

/*
Basic XOR basis:
- insert: O(B)
- representability query: O(B)
- maximum subset XOR: O(B)
- memory: O(B)

Indexed XOR basis:
- insert (value, position): O(B)
- maximum subset XOR restricted to positions >= left: O(B)
- useful for offline queries processed by increasing right endpoint

For long long, replace int with long long and use 1LL << b.
*/
