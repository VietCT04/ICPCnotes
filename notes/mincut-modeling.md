# Min-Cut Modeling Cheat Sheet

This note focuses on interpretation rather than the max-flow algorithm itself.

## Universal edge meaning

For a directed edge

`u --c--> v`

an `s-t` cut pays `c` exactly when

- `u` is on the source side, and
- `v` is on the sink side.

It does **not** pay for an edge directed from sink side back to source side.

That one fact is enough to derive most standard constructions.

---

## Selected / not selected convention

Assume:

- source side = selected / TRUE
- sink side = not selected / FALSE

Then:

| Edge | Meaning |
|---|---|
| `s -> X (p)` | pay `p` if `X` is rejected |
| `X -> t (c)` | pay `c` if `X` is selected |
| `A -> B (INF)` | hard implication `A => B` |
| `A -> B (c)` | pay penalty `c` only for `A=true, B=false` |

So:

- `INF` means a state combination is forbidden
- finite capacity means the state combination is allowed but penalized

---

## Maximum-weight closure

Suppose:

- selecting `A` gives profit `p`
- selecting `B` costs `c`
- selecting `A` requires selecting `B`

Build:

```text
s --p--> A --INF--> B --c--> t
```

Let `P` be the sum of all positive profits.

Then:

`answer = P - mincut`.

The cut value equals

`lost profits from rejected positive objects + costs of selected negative objects`.

### Why maximization is still possible

Min-cut always minimizes, but many maximization objectives can be rewritten as

`constant - minimum loss`.

For example:

`max(selected profits - selected costs)`

is equivalent to

`total positive profits - min(rejected profits + selected costs)`.

---

## Hard vs soft dependency example

```text
s --10--> A
A --6--> B
B --4--> t
```

Interpretation:

- rejecting `A` loses `10`
- selecting `B` costs `4`
- selecting `A` without `B` is allowed, but costs penalty `6`

Possible cut costs:

- reject `A`: `10`
- keep `A`, reject `B`: `6`
- keep both: `4`

So min-cut chooses to keep both.

If `A -> B` had capacity `INF`, keeping `A` while rejecting `B` would be forbidden.

---

## Binary states instead of selected/unselected

The two cut sides can represent any two labels.

For example:

- source side = state `0`
- sink side = state `1`

If object `X` initially has state `0` and changing to `1` costs `v`:

`source -> X (v)`.

If object `X` initially has state `1` and changing to `0` costs `v`:

`X -> sink (v)`.

A zero-capacity edge on the opposite side is unnecessary: it would never affect the cut value.

---

## All-of-a-set constraints with an auxiliary node

Suppose person `P` earns reward `w` only if all listed variables are state `0`.

Use:

```text
s --w--> P
P --INF--> X1
P --INF--> X2
...
```

If `P` remains on source side, every required `Xi` is forced to source side too.
If the condition cannot be satisfied, moving `P` to sink side loses reward `w`.

If the person instead requires all variables to be state `1`, reverse the dependency orientation:

```text
X1 --INF--> P
X2 --INF--> P
...
P --w--> t
```

The orientation changes because state `1` is represented by the sink side.

---

## Recognizing min-cut problems

Strong signals:

1. Every object has one of two states.
2. Some state choices have additive costs/rewards.
3. Choosing one state can force another object into a state.
4. Violating a relation has a fixed penalty.
5. The objective can be written as a constant minus a sum of local penalties.

Useful mental translation:

> A directed capacity is the price of exactly one binary combination: tail on source side, head on sink side.

Do not memorize complete graph constructions. Decide what the two sides mean, then ask which state combination each cost should charge.
