# SOS DP / Zeta Transform

SOS DP is useful when every state is a bitmask and, for each mask, you need an aggregate over all of its subsets or all of its supersets.

If there are `B` bits, there are `2^B` masks. Enumerating all submasks of every mask costs about `O(3^B)`. SOS DP computes all answers in `O(B * 2^B)`.

## What is `f[mask]`?

`f[mask]` is the information attached to exactly that mask before the transform.

A common example is:

```cpp
f[mask] = number of array elements exactly equal to mask;
```

For example, with three bits:

```text
a = [1, 3, 3, 5, 7]

1 = 001
3 = 011
3 = 011
5 = 101
7 = 111
```

Initially:

```text
mask    f[mask]
000        0
001        1
010        0
011        2
100        0
101        1
110        0
111        1
```

So `f[011] = 2` means exactly two values are equal to `011`.

## Sum over subsets

We want:

```text
dp[mask] = sum f[sub] for every sub ⊆ mask
```

Transition:

```cpp
for (int bit = 0; bit < B; ++bit)
    for (int mask = 0; mask < (1 << B); ++mask)
        if (mask & (1 << bit))
            dp[mask] += dp[mask ^ (1 << bit)];
```

Interpretation:

```text
A 1-bit receives information from the same mask with that bit removed.
```

Example for `mask = 101`:

```text
submasks: 000, 001, 100, 101
```

For the example array, only `001` and `101` occur, so the transformed value is `2`.

## Sum over supersets

We want:

```text
dp[mask] = sum f[super] for every super ⊇ mask
```

Transition:

```cpp
for (int bit = 0; bit < B; ++bit)
    for (int mask = 0; mask < (1 << B); ++mask)
        if (!(mask & (1 << bit)))
            dp[mask] += dp[mask | (1 << bit)];
```

Interpretation:

```text
A 0-bit receives information from the same mask with that bit added.
```

Example for `mask = 011`:

```text
supermasks: 011, 111
```

In the example array, `011` occurs twice and `111` once, so the transformed value is `3`.

Equivalently, after a superset frequency transform:

```cpp
cnt[mask]
```

can represent the number of values `x` satisfying:

```cpp
(x & mask) == mask
```

That is, `x` contains every bit required by `mask`.

## Mental shortcut

```text
Need aggregate over subsets?
    if bit is 1, receive from mask with that bit removed.

Need aggregate over supersets?
    if bit is 0, receive from mask with that bit added.
```

## Invariant

After processing the first `k` bit positions, `dp[mask]` already aggregates all masks obtained by freely changing only those processed bits in the permitted direction:

- subset transform: processed 1-bits may be turned off;
- superset transform: processed 0-bits may be turned on.

After all `B` bits are processed, this is exactly the full subset or superset family.

## Complexity

```text
Time:  O(B * 2^B)
Space: O(2^B)
```

For `B = 20`, this is about 20 million transitions and is a standard competitive-programming technique.

## Recognition pattern

Consider SOS DP when:

- the state space is all masks of a small bit width, usually around 20-24 bits;
- each query asks about all subsets or all supersets of a mask;
- you need those aggregates for many masks, not just one;
- direct submask enumeration for every mask would be too slow.

Typical aggregates include frequencies, sums, counts, and other compatible DP values.
