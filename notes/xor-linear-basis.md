# XOR Linear Basis

## What it represents

A XOR linear basis stores a small set of numbers that can generate exactly the same XOR span as all inserted numbers.

For `B` bits, the basis has at most `B` vectors. `basis[b]` is either `0` or a vector whose highest set bit is `b`.

This is Gaussian elimination over GF(2):

- addition is XOR;
- coefficients are only `0` or `1`;
- choosing a subset means choosing which basis vectors have coefficient `1`.

If the input numbers are `x1, x2, ...`, every achievable subset XOR is a linear combination

```text
c1*x1 XOR c2*x2 XOR ...
```

where every `ci` is `0` or `1`.

## Insertion

To insert `x`, eliminate its highest set bit from high to low.

```cpp
for (int b = B - 1; b >= 0; --b) {
    if (!(x & (1 << b))) continue;

    if (!basis[b]) {
        basis[b] = x;
        break;
    }

    x ^= basis[b];
}
```

If `x` becomes `0`, it was already representable by the current basis and adds no new dimension.

### Example

Insert:

```text
6 = 110
5 = 101
3 = 011
```

One possible basis is:

```text
basis[2] = 110
basis[1] = 011
```

`101` is representable because:

```text
110 XOR 011 = 101
```

So the rank is `2`, not `3`.

## Maximum subset XOR

Once the basis is triangular by highest bit, greedily improve the answer from high bit to low bit:

```cpp
int ans = 0;
for (int b = B - 1; b >= 0; --b)
    ans = max(ans, ans ^ basis[b]);
```

Why greedy works: at bit `b`, only the basis vector whose pivot is `b` can change that highest undecided bit without changing any higher pivot bit.

## Complexity

For `B` bits:

- insert one number: `O(B)`;
- build from `n` numbers: `O(nB)`;
- maximum XOR query: `O(B)`;
- memory: `O(B)`.

For 32-bit integers, `B <= 31/32`; for `long long`, usually `B <= 60/63`.

---

# Range queries: keep positions with the basis

A useful extension appears in CF 1100F — Ivan and Burgers.

Queries ask for the maximum subset XOR inside `[l, r]`. Process queries offline by increasing `r`. After inserting `a[r]`, the basis represents the prefix `[1, r]`.

The remaining problem is excluding indices `< l`.

Store with every pivot:

```cpp
basis[b]
pos[b]
```

`pos[b]` is the best/latest possible minimum index of a representation for that pivot vector.

When inserting `(x, idx)`, if an existing pivot has an older position, swap:

```cpp
if (idx > pos[b]) {
    swap(idx, pos[b]);
    swap(x, basis[b]);
}
x ^= basis[b];
```

The carried `idx` becomes the minimum index of the current representation. The swap rule keeps, for every pivot, the representation whose minimum index is as large as possible.

Therefore for query `[l, r]`, a pivot is usable iff:

```cpp
pos[b] >= l
```

and the maximum XOR can again be built greedily from high bit to low bit.

## Indexed-basis insertion invariant

During insertion, suppose both `x` and `basis[b]` have pivot `b`.

- keep the one with larger `pos` as `basis[b]`;
- XOR the older one with it and continue downward.

This is the XOR analogue of elimination while preserving the strongest possible range-validity information.

## Recognition patterns

Think of XOR linear basis when you see:

- maximum/minimum XOR of any subset;
- number of distinct subset XORs (`2^rank`);
- checking whether a value is representable as a subset XOR;
- offline range maximum-subset-XOR queries;
- dynamic/persistent variants of XOR span.

## Common pitfalls

- A binary trie solves pairwise XOR-style problems; it does **not** replace a linear basis for arbitrary subset XOR.
- The basis stores a span, not necessarily original elements.
- For range queries, storing only a pivot value is insufficient; positional metadata must follow elimination correctly.
