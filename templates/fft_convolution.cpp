#include <bits/stdc++.h>
using namespace std;

// Iterative FFT + integer convolution.
//
// Complexity:
//   FFT of size N: O(N log N)
//   convolution of arrays of lengths n, m: O((n+m) log(n+m))
//
// Typical use: compute all shifted dot products at once.
// If pattern P has length m, reverse it before convolution so that
// alignment starting at s appears at index s + m - 1.

using cd = complex<double>;
const double PI = acos(-1.0);

void fft(vector<cd>& a, bool invert) {
    int n = (int)a.size();

    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2.0 * PI / len * (invert ? -1.0 : 1.0);
        cd wlen(cos(angle), sin(angle));

        for (int i = 0; i < n; i += len) {
            cd w(1.0);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;

                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd& x : a) x /= n;
    }
}

vector<long long> convolution(const vector<int>& a, const vector<int>& b) {
    if (a.empty() || b.empty()) return {};

    int need = (int)a.size() + (int)b.size() - 1;
    int n = 1;
    while (n < need) n <<= 1;

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < (int)a.size(); ++i) fa[i] = a[i];
    for (int i = 0; i < (int)b.size(); ++i) fb[i] = b[i];

    fft(fa, false);
    fft(fb, false);

    for (int i = 0; i < n; ++i) fa[i] *= fb[i];

    fft(fa, true);

    vector<long long> result(need);
    for (int i = 0; i < need; ++i) {
        result[i] = llround(fa[i].real());
    }

    return result;
}

/*
Sliding-dot-product reminder:

Want for every start s:
    sum_{j=0}^{m-1} A[s+j] * P[j]

Build:
    B[j] = P[m-1-j]

Then after C = convolution(A, B):
    score(start=s) = C[s + m - 1]
*/
