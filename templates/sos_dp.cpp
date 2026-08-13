#include <bits/stdc++.h>
using namespace std;

/*
    SOS DP / Zeta Transform

    Let f[mask] store information about exactly `mask`.

    Example:
        f[mask] = frequency of array values exactly equal to mask.

    After subsetSOS(f):
        f[mask] = sum of original f[sub] over all sub ⊆ mask.

    After supersetSOS(f):
        f[mask] = sum of original f[super] over all super ⊇ mask.

    Time:  O(B * 2^B)
    Space: O(2^B)

    The operation does not have to be addition. The same transition can be
    adapted to counts, sums, min/max, OR/AND aggregates, etc. when the
    combine operation is valid for the intended DP.
*/

template <class T>
void subsetSOS(vector<T>& f, int B) {
    for (int bit = 0; bit < B; ++bit) {
        for (int mask = 0; mask < (1 << B); ++mask) {
            if (mask & (1 << bit)) {
                f[mask] += f[mask ^ (1 << bit)];
            }
        }
    }
}

template <class T>
void supersetSOS(vector<T>& f, int B) {
    for (int bit = 0; bit < B; ++bit) {
        for (int mask = 0; mask < (1 << B); ++mask) {
            if (!(mask & (1 << bit))) {
                f[mask] += f[mask | (1 << bit)];
            }
        }
    }
}

int main() {
    const int B = 3;

    vector<int> a = {1, 3, 3, 5, 7};
    vector<long long> freq(1 << B, 0);

    for (int x : a) ++freq[x];

    auto subsetCount = freq;
    subsetSOS(subsetCount, B);

    auto supersetCount = freq;
    supersetSOS(supersetCount, B);

    // subsetCount[5] counts values whose set bits are a subset of 101.
    // Values 1 (001) and 5 (101) qualify, so the answer is 2.
    cout << subsetCount[5] << '\n';

    // supersetCount[3] counts values containing every bit of 011.
    // Values 3, 3, and 7 qualify, so the answer is 3.
    cout << supersetCount[3] << '\n';
}
