# ICPC Notes

Reusable competitive-programming notes and templates.

## Notes

- [Advanced reusable patterns](notes/advanced-patterns.md) — segment-tree state compression, digit DP, coordinate compression, DSU/Kruskal modeling, matching/Kőnig, reverse processing, k-core peeling, FFT, min-cut modeling, and more.
- [Min-cut modeling cheat sheet](notes/mincut-modeling.md) — how to interpret every directed edge, hard vs soft dependencies, maximum-weight closure, and binary-state cuts.
- [SOS DP / Zeta Transform](notes/sos-dp.md) — what `f[mask]` means, subset and superset transforms, invariants, examples, recognition patterns, and complexity.
- [Divide & Conquer DP Optimization](notes/divide-conquer-dp.md) — monotone opt, Monge/quadrangle inequality proofs, recursion bounds, and movable-window costs for CF 868F.
- [XOR Linear Basis](notes/xor-linear-basis.md) — GF(2) elimination, maximum subset XOR, representability, and indexed bases for range queries such as CF 1100F.

## Templates

- [Dinic max-flow / min-cut](templates/dinic.cpp)
- [FFT convolution](templates/fft_convolution.cpp)
- [Fibonacci lazy segment tree](templates/fibonacci_lazy_segment_tree.cpp)
- [Kuhn bipartite matching](templates/kuhn_matching.cpp)
- [Hopcroft-Karp bipartite matching](templates/hopcroft_karp.cpp)
- [DSU / Union-Find](templates/dsu.cpp)
- [Fenwick tree](templates/fenwick.cpp)
- [Segment tree storing the K smallest candidates](templates/smallest_k_segment_tree.cpp)
- [SOS DP subset / superset transforms](templates/sos_dp.cpp)
- [Divide & Conquer DP Optimization](templates/divide_conquer_dp.cpp)
- [D&C DP + equal-pairs movable window](templates/dnc_dp_equal_pairs_window.cpp)
- [XOR Linear Basis + indexed range variant](templates/xor_linear_basis.cpp)

Each template includes complexity notes and comments about the modeling pattern it is intended for.

## Problems covered by the notes

- CF 446C — DZY Loves Fibonacci Numbers
- CF 1408E — Avoid Rainbow Cycles
- CF 1601C — Optimal Insertion
- CF 1073E — Segment Sum
- CF 1557D — Ezzat and Grid
- CF 1638E — Colorful Operations
- CF 1389F — Bicolored Segments
- CF 1139E — Maximize Mex
- CF 1027F — Session in BSU
- CF 1407E — Egor in the Republic of Dagestan
- CF 1439B — Graph Subset Problem
- CF 528D — Fuzzy Search
- CF 1665E — MinimizOR
- CF 1198E — Rectangle Painting 2
- CF 1082G — Petya and Graph
- CF 2026E — Best Subsequence
- CF 311E — Biologist
- CF 868F — Yet Another Minimization Problem
- CF 1100F — Ivan and Burgers
