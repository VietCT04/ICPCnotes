# Persistent Segment Tree

A persistent segment tree keeps old versions instead of overwriting them. For prefix-frequency problems, `root[i]` represents the frequencies of values in `a[1..i]`.

## What a node stores

The tree is built over the value domain. A node `p` stores:

```cpp
int ls[p], rs[p], sum[p];
```

`sum[p]` is the number of inserted values that lie inside the value interval represented by that node. `ls[p]` and `rs[p]` are child indices.

Example after inserting `[2,1,2]` into value range `[1,4]`:

```text
                    [1..4] sum=3
                   /             \
          [1..2] sum=3          [3..4] sum=0
          /        \
      [1] sum=1   [2] sum=2
```

## Persistence by path copying

When inserting one value, only one root-to-leaf path changes. Create new copies only on that path and reuse every unchanged subtree from the previous version.

```cpp
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
```

Build prefix versions:

```cpp
for (int i = 1; i <= n; i++){
    root[i] = update(root[i - 1], 1, V, a[i]);
}
```

Only `O(log V)` new nodes are created per insertion, so total memory is `O(n log V)`.

## Range frequencies from two versions

For query `[l,r]`:

```text
root[r]     = frequencies in [1..r]
root[l - 1] = frequencies in [1..l-1]
```

Therefore, at corresponding nodes `u` and `v`:

```cpp
sum[u] - sum[v]
```

is the number of elements from array interval `[l,r]` whose value lies in the current node's value interval.

This is the main prefix-difference trick.

## CF 840D — Destiny

For query `[l,r,k]`, let:

```cpp
lim = (r - l + 1) / k;
```

We need the smallest value appearing more than `lim` times.

At a value-range node, if:

```cpp
sum[u] - sum[v] <= lim
```

then no single value in that entire subtree can occur more than `lim`, so prune it.

Search left first because the problem asks for the smallest value:

```cpp
int query(int u, int v, int l, int r, int lim){
    if (sum[u] - sum[v] <= lim) return -1;
    if (l == r) return l;
    int mid = (l + r) / 2;
    int res = query(ls[u], ls[v], l, mid, lim);
    if (res != -1) return res;
    return query(rs[u], rs[v], mid + 1, r, lim);
}
```

Why this is fast: every explored subtree contains more than `len/k` elements, so at one depth there can be at most `k-1` such disjoint subtrees. For CF 840D, `k <= 5`, giving `O(k log V)` per query.

## Mental model

Think of `root[i]` as a saved snapshot after processing the first `i` elements. Old snapshots remain usable because new versions share all unchanged nodes with previous versions.

## Recognition patterns

Useful for:

- k-th smallest in `[l,r]`;
- frequency/count queries on subarrays;
- heavy-frequency queries;
- historical/versioned queries;
- problems where two prefix versions can be subtracted.
