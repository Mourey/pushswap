*This project has been created as part of the 42 curriculum by rmourey-, lcalvarro*

## Description

Push_swap is a sorting algorithm project that sorts a stack of integers using a
limited set of operations (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`, `rr`,
`rra`, `rrb`, `rrr`) on two stacks (A and B). The goal is to sort the numbers
in ascending order on stack A using the fewest operations possible.

The program includes four sorting strategies and an adaptive mode that selects
the optimal strategy based on the measured disorder of the input.

## Instructions

### Compilation

```
make        # builds push_swap
make bonus  # builds checker
make clean  # removes object files
make fclean # removes object files and binaries
make re     # full rebuild
```

### Usage

```
./push_swap [flags] <numbers>
```

**Flags** (optional):
- `--simple`   — force O(n^2) selection sort
- `--medium`   — force O(n*sqrt(n)) chunk-based sort
- `--complex`  — force O(n*log(n)) radix sort
- `--adaptive` — auto-select by disorder (default)
- `--bench`    — print disorder, strategy, and operation counts to stderr

**Examples:**
```
./push_swap 3 2 1
./push_swap --bench 5 4 3 2 1
./push_swap "3 2 1"
./push_swap 3 2 1 | ./checker 3 2 1
```

## Resources

- [Push_swap subject (42)](https://projects.intra.42.fr/push_swap)
- [Visualizer (push_swap tester)](https://github.com/o-reo/push_swap_visualizer)
- Claude AI (Anthropic) was used as an assisted coding tool during development

## Contributions

- **rmourey-**: Sorting algorithms (simple, medium, complex, adaptive), stack operations, parsing, bench mode, checker bonus
- **lcalvarro**: Testing, code review, algorithm optimization, documentation

## Disorder Metric

Disorder is defined as the ratio of inversions to the maximum possible:

    d = inversions(A) / (n * (n - 1) / 2)

where inversions(A) = |{(i, j) : i < j and A[i] > A[j]}|.

- d = 0 means A is sorted.
- d = 1 means A is reverse-sorted.
- A uniformly random permutation has expected d = 0.5.

Computed in O(n^2) CPU time (not counted as push_swap operations).

## Algorithm Explanations

All complexity bounds below count **push_swap operations** (the output
instructions), not CPU cycles. Space is always O(n) (the two stacks).

---

### Strategy: Simple — O(n^2)

Selection sort. Repeatedly finds the minimum element in A, rotates it to the
top, and pushes to B. After all elements are in B, pushes them back to A in
order. Each minimum-find is O(n), done n times: **O(n^2)**.

---

### Strategy: Medium — O(n * sqrt(n))

Chunk-based sort.

**Phase 1 — push chunks to B:** Divide the normalized range [0, n) into
chunks of size c = Theta(sqrt(n)). For each chunk [lo, hi], scan A and push
matching elements to B, using a midpoint split to keep larger values near the
top of B. There are n/c = O(sqrt(n)) chunks; each chunk scan traverses A
at most once (n rotations + c pushes). Total: O(n * sqrt(n)).

**Phase 2 — greedy push-back:** Repeatedly find the maximum in B, rotate it
to the top of B, and `pa`. Finding the max and rotating are each O(|B|), and
the sum of |B| over all n insertions telescopes to O(n^2 / c) = O(n * sqrt(n))
since elements are roughly grouped by chunk.

**Total: O(n * sqrt(n)). Space: O(n).**

---

### Strategy: Complex — O(n * log(n))

Binary radix sort on normalized values [0, n).

For each bit position b from LSB to MSB: scan all n elements in A; elements
with bit b = 0 go to B (`pb`), elements with bit b = 1 stay (`ra`). Then push
all of B back to A (`pa`).

Each pass processes n elements with at most 2n operations (n decisions +
up to n push-backs). There are ceil(log2(n)) bit passes.

**Total: O(n * log(n)). Space: O(n).**

---

### Strategy: Adaptive (default)

Measures the disorder d and dispatches to a sub-strategy:

| Condition           | Method                           | Complexity        |
|---------------------|----------------------------------|-------------------|
| Rotated sorted      | Rotate min to top                | O(n)              |
| d < 0.2             | Monotone extraction + reinsertion| O(n + k^2)        |
| 0.2 <= d < 0.5      | Chunk-based (medium)             | O(n * sqrt(n))    |
| d >= 0.5            | Binary radix (complex)           | O(n * log(n))     |

---

#### Low disorder (d < 0.2): Monotone extraction with sorted reinsertion

**Technique:**

1. **Rotation alignment** — Rotate A so the global minimum is on top.
   Cost: at most n/2 operations (ra or rra, whichever is shorter).

2. **Monotone scan** — Traverse A exactly once (n operations). Maintain a
   running maximum `last`. For each element at the top of A:
   - If top >= last: it extends the non-decreasing subsequence. Set
     last = top, do `ra`.
   - If top < last: it breaks monotonicity. Do `pb`.
   After this pass, A holds a sorted subsequence of length n - k, and B
   holds the k elements that broke monotonicity.

3. **Sorted reinsertion** — Reinsert B elements into A from **largest to
   smallest**:
   - Find the max in B (O(k) scan), rotate it to the top of B.
   - Find its insertion point in A (the position after the largest
     element smaller than it), rotate A there, `pa`.
   - Processing largest-first ensures successive insertion points move
     monotonically through A, so the **total A rotation across all
     reinsertions telescopes to O(n)**.
   - The B rotations cost O(k) per element, summing to O(k^2) total.

4. **Final alignment** — Rotate A so the minimum is on top. Cost: O(n).

**Complexity upper bound: O(n + k^2)** where k is the number of elements
pushed to B.

**Bound on k:** Each element pushed to B was strictly less than the running
maximum at the time of its push. That (element, running-max) pair constitutes
a unique inversion. Therefore k <= total inversions = d * n(n-1)/2.

For d < 0.2: k < n(n-1)/10. However, this is a worst-case algebraic bound;
in practice, for inputs that arise naturally with d < 0.2, k is much smaller:

- **Rotated sorted** (the most common low-d structure): caught by the
  is_rotated_sorted fast path, O(n).
- **A few outliers** (e.g., one element far from position): k = O(1),
  so O(n + 1) = O(n).
- **Scattered local swaps** (e.g., O(sqrt(n)) adjacent transpositions):
  k = O(sqrt(n)), so O(n + n) = O(n).

The method degrades gracefully: even in the pathological case where d < 0.2
but k = Theta(n) (e.g., all-pairs-swapped with d = O(1/n)), the O(k^2) term
dominates but k^2 < (0.1 * n^2)^2... in practice the dispatcher catches
these at d < 0.2 and the operation count remains well below the O(n * sqrt(n))
medium strategy.

---

#### Medium disorder (0.2 <= d < 0.5): Chunk-based sort

Delegates to the medium strategy (chunk decomposition). See the Medium
strategy section above.

**Complexity: O(n * sqrt(n)). Space: O(n).**

---

#### High disorder (d >= 0.5): Binary radix sort

Delegates to the complex strategy (binary radix sort). See the Complex
strategy section above.

**Complexity: O(n * log(n)). Space: O(n).**

---

## Threshold Rationale

**d = 0.2 (low/medium boundary):**
Below 20% disorder, most elements are already in relative order. The monotone
extraction scan pushes only elements that break the dominant ascending trend.
The reinsertion cost O(k^2) is dominated by the O(n) scan for typical
low-disorder inputs. Above 0.2, the number of breaks grows large enough that
the chunk-based strategy's O(n * sqrt(n)) worst-case guarantee becomes
preferable.

**d = 0.5 (medium/high boundary):**
A uniformly random permutation has expected disorder exactly 0.5. Below this
point, the input retains enough structure for the chunk-based sort to exploit
(elements within each chunk are partially concentrated, reducing scan cost).
Above 0.5, more than half of all element pairs are inverted; the input is
sufficiently shuffled that radix sort's uniform O(n * log(n)) behavior —
independent of input pattern — is preferred for its predictability and low
constant factor on heavily disordered inputs.
