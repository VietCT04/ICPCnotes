# Divide & Conquer DP Optimization

## The DP shape

A common partition DP is

```text
dp[g][i] = min over j < i of
           dp[g-1][j] + C(j+1, i)
```

Interpretation:

- first `j` elements are optimally split into `g-1` groups;
- the last group is `[j+1, i]`;
- `C(l,r)` is the cost of one segment.

Define

```text
opt[g][i] = the smallest j attaining that minimum
```

The naive transition tries all `j` for every `i`, giving `O(k n^2)`.

---

## The required property

Divide & Conquer DP Optimization applies when the optimal split points are monotone:

```text
opt[g][i] <= opt[g][i+1]
```

for each fixed layer `g`.

Then a recursive state

```text
solve(L, R, optL, optR)
```

means:

- compute `cur[i]` for every `i` in `[L,R]`;
- their optimal split positions are known to lie in `[optL,optR]`.

Take

```text
mid = (L + R) / 2
```

Try only

```text
j in [optL, min(mid-1, optR)]
```

Suppose the best split is `best`.

Monotonicity gives

```text
left half:  opt <= best
right half: opt >= best
```

so recurse as

```cpp
solve(L, mid - 1, optL, best);
solve(mid + 1, R, best, optR);
```

---

## Why the transition count becomes O(n log n)

At one recursion depth, candidate intervals are ordered and only overlap at boundary optima up to constant factors, so the total number of candidate checks is `O(n)`.

There are `O(log n)` recursion depths.

Therefore one DP layer takes

```text
O(n log n * Tcost)
```

where `Tcost` is the time to evaluate `C(j+1, i)`.

For `k` layers and `O(1)` transition-cost evaluation:

```text
O(k n log n)
```

---

# How to prove monotone opt

The standard route is

```text
quadrangle inequality / Monge property
        => monotone column minima
        => Divide & Conquer DP Optimization
```

Define the transition matrix

```text
A[j][i] = prev[j] + C(j+1, i)
```

For each column `i`, `opt[i]` is the smallest row containing the minimum.

A sufficient condition is the Monge inequality:

```text
A[j1][i1] + A[j2][i2]
<=
A[j1][i2] + A[j2][i1]
```

for `j1 < j2` and `i1 < i2`.

Because `prev[j]` is only a row constant, the `prev` terms cancel. Thus it is enough to prove the corresponding quadrangle inequality for `C`.

## Monge implies monotone minima

Assume, for contradiction,

```text
i1 < i2
opt[i1] = p
opt[i2] = q
q < p
```

Optimality gives

```text
A[p][i1] <= A[q][i1]
A[q][i2] <= A[p][i2]
```

Adding them gives one inequality direction.

Monge applied to `q < p` and `i1 < i2` gives the opposite direction. Hence equality must hold, which means `q` is also optimal for `i1`.

But `q < p`, contradicting the definition that `opt[i1]` is the smallest optimal split.

Therefore

```text
opt[i1] <= opt[i2]
```

---

# CF 868F — Yet Another Minimization Problem

Here

```text
C(l,r) = number of unordered equal-value pairs inside [l,r]
```

We need to prove

```text
C(a,c) + C(b,d)
<=
C(a,d) + C(b,c)
```

for `a <= b <= c <= d`.

Partition `[a,d]` into

```text
X = [a, b-1]
Y = [b, c]
Z = [c+1, d]
```

Count equal pairs according to which regions contain their endpoints.

Every contribution appears equally on both sides except pairs with one endpoint in `X` and one endpoint in `Z`.

Those pairs appear only on the right-hand side, so

```text
RHS - LHS = number of equal pairs crossing X and Z >= 0
```

which proves the quadrangle inequality.

### Intuitive version

Compare the new pairs created when extending the right endpoint from `c` to `d`:

```text
C(a,d) - C(a,c)
```

versus

```text
C(b,d) - C(b,c)
```

The interval beginning at `a` contains everything the interval beginning at `b` contains, plus extra elements on the left. Extending it can therefore create at least as many equal pairs.

Rearranging gives the same quadrangle inequality.

---

# Maintaining C(l,r) with a movable window

For CF 868F, recomputing `C(l,r)` from scratch would destroy the optimization.

Maintain a current interval `[L,R]`, frequencies `cnt[value]`, and `cost`.

When adding one occurrence of value `x`:

```cpp
cost += cnt[x];
++cnt[x];
```

If there were already `cnt[x]` copies, the new element creates exactly that many new equal pairs.

When removing it:

```cpp
--cnt[x];
cost -= cnt[x];
```

Move the window by

```cpp
while (L > l) add(--L);
while (R < r) add(++R);
while (L < l) remove(L++);
while (R > r) remove(R--);
```

One `moveTo(l,r)` call can be `O(n)` in isolation, but across the D&C traversal the pointer movements are amortized. For this problem they remain `O(n log n)` per DP layer up to constant factors.

Thus the full solution is

```text
O(k n log n)
```

with `O(n)` memory when only the previous and current DP layers are stored.

---

## Recognition checklist

When you see

```text
dp[i] = min_j { prev[j] + C(j,i) }
```

ask:

1. Is this a partition DP with one last split point?
2. Can I prove a quadrangle/Monge inequality for `C`?
3. Does that imply monotone `opt`?
4. Can `C(l,r)` be evaluated quickly or maintained incrementally?

If yes, Divide & Conquer DP Optimization is a strong candidate.

## Common pitfalls

- Do not assume monotone `opt`; prove it or rely on a known theorem whose conditions you have verified.
- Define tie-breaking consistently, usually the smallest optimal `j`.
- Respect valid split ranges, e.g. `j < i` and enough elements for previous groups.
- D&C optimization reduces the number of candidate transitions; it does not automatically make an expensive `C(l,r)` cheap.
