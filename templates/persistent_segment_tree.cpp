#include <bits/stdc++.h>
using namespace std;
const int N = 3e5 + 5;
const int M = N * 20;
int n, q;
int a[N], root[N];
int ls[M], rs[M], sum[M], tot;
int update(int pre, int l, int r, int x){
    int p = ++tot;
    ls[p] = ls[pre];
    rs[p] = rs[pre];
    sum[p] = sum[pre] + 1;
    if (l == r) return p;
    int mid = (l + r) / 2;
    if (x <= mid){
        ls[p] = update(ls[pre], l, mid, x);
    } else {
        rs[p] = update(rs[pre], mid + 1, r, x);
    }
    return p;
}
int queryHeavy(int u, int v, int l, int r, int lim){
    if (sum[u] - sum[v] <= lim) return -1;
    if (l == r) return l;
    int mid = (l + r) / 2;
    int res = queryHeavy(ls[u], ls[v], l, mid, lim);
    if (res != -1) return res;
    return queryHeavy(rs[u], rs[v], mid + 1, r, lim);
}
int kth(int u, int v, int l, int r, int k){
    if (l == r) return l;
    int mid = (l + r) / 2;
    int cnt = sum[ls[u]] - sum[ls[v]];
    if (k <= cnt) return kth(ls[u], ls[v], l, mid, k);
    return kth(rs[u], rs[v], mid + 1, r, k - cnt);
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> q;
    for (int i = 1; i <= n; i++){
        cin >> a[i];
        root[i] = update(root[i - 1], 1, n, a[i]);
    }
    while (q--){
        int l, r, k;
        cin >> l >> r >> k;
        int lim = (r - l + 1) / k;
        cout << queryHeavy(root[r], root[l - 1], 1, n, lim) << '\n';
    }
}
