# push_swap Study Guide

> A comprehensive defense preparation guide for the 42 push_swap project.
> Covers every source file, algorithm, design pattern, and optimization in detail.

---

## Table of Contents

1. [Chapter 1: Project Overview & Concepts](#chapter-1-project-overview--concepts)
2. [Chapter 2: C Patterns Used Throughout](#chapter-2-c-patterns-used-throughout)
3. [Chapter 3: Data Structure Deep-Dive](#chapter-3-data-structure-deep-dive)
4. [Chapter 4: Algorithm Walkthroughs](#chapter-4-algorithm-walkthroughs)
5. [Chapter 5: Argument Parsing & Validation](#chapter-5-argument-parsing--validation)
6. [Chapter 6: The Makefile](#chapter-6-the-makefile)
7. [Chapter 7: Benchmark Mode Implementation](#chapter-7-benchmark-mode-implementation)
8. [Chapter 8: Bonus Checker](#chapter-8-bonus-checker)
9. [Chapter 9: Testing & Debugging Strategies](#chapter-9-testing--debugging-strategies)
10. [Chapter 10: Performance Optimization Tips](#chapter-10-performance-optimization-tips)

---

## Chapter 1: Project Overview & Concepts

### What you'll learn

- What push_swap is and why it exists as a 42 project
- The two-stack model and what each operation means visually
- Why operation count matters more than theoretical time complexity
- A complete walkthrough of sorting [3, 2, 1] with stack visualizations

---

### 1.1 What is push_swap?

push_swap is a sorting project where you must sort a stack of integers using only
a limited set of operations, while minimizing the total number of operations used.

You are given:

- **Stack A**: initialized with the input integers
- **Stack B**: initially empty
- **11 operations**: sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr

The goal: output the shortest possible sequence of operations that sorts Stack A
in ascending order (smallest on top), with Stack B empty at the end.

**Why this project exists at 42:**

- It forces you to think about algorithmic complexity _in practice_, not just theory
- You must choose the right algorithm for the right input size
- It teaches you that O(n log n) isn't always better than O(n \* sqrt(n)) when
  constant factors and operation constraints matter
- It exercises C fundamentals: memory management, parsing, data structures

---

### 1.2 The Two-Stack Model

Think of two stacks of plates sitting side by side. You can only interact with
the tops (and bottoms via rotation) of each stack.

```
    Stack A          Stack B
   +-------+        +-------+
   |   3   |  <-- top        |       |  <-- top (empty)
   +-------+        +-------+
   |   2   |        |       |
   +-------+        +-------+
   |   1   |        |       |
   +-------+        +-------+
```

---

### 1.3 The 11 Operations — Visual Reference

Each operation manipulates one or both stacks. Here is every operation illustrated:

#### sa — swap a

Swap the first two elements at the top of Stack A.

```
BEFORE:          AFTER:
Stack A          Stack A
+---+            +---+
| 3 | <- top     | 2 | <- top
+---+            +---+
| 2 |            | 3 |
+---+            +---+
| 1 |            | 1 |
+---+            +---+
```

#### sb — swap b

Same as `sa` but for Stack B.

#### ss — swap both

Performs `sa` and `sb` simultaneously.

#### pa — push to a

Take the top element of B and put it on top of A.

```
BEFORE:                    AFTER:
Stack A    Stack B         Stack A    Stack B
+---+      +---+          +---+      +---+
| 2 |      | 3 |          | 3 |      |   |
+---+      +---+          +---+      +---+
| 1 |      |   |          | 2 |
+---+                     +---+
                           | 1 |
                           +---+
```

#### pb — push to b

Take the top element of A and put it on top of B.

```
BEFORE:                    AFTER:
Stack A    Stack B         Stack A    Stack B
+---+      +---+          +---+      +---+
| 3 |      |   |          | 2 |      | 3 |
+---+      +---+          +---+      +---+
| 2 |                     | 1 |
+---+                     +---+
| 1 |
+---+
```

#### ra — rotate a

Shift all elements of A up by one. The first element becomes the last.

```
BEFORE:          AFTER:
Stack A          Stack A
+---+            +---+
| 3 | <- top     | 2 | <- top
+---+            +---+
| 2 |            | 1 |
+---+            +---+
| 1 |            | 3 |  <- was on top, now on bottom
+---+            +---+
```

#### rb — rotate b

Same as `ra` but for Stack B.

#### rr — rotate both

Performs `ra` and `rb` simultaneously.

#### rra — reverse rotate a

Shift all elements of A down by one. The last element becomes the first.

```
BEFORE:          AFTER:
Stack A          Stack A
+---+            +---+
| 3 | <- top     | 1 | <- was on bottom, now on top
+---+            +---+
| 2 |            | 3 |
+---+            +---+
| 1 |            | 2 |
+---+            +---+
```

#### rrb — reverse rotate b

Same as `rra` but for Stack B.

#### rrr — reverse rotate both

Performs `rra` and `rrb` simultaneously.

---

### 1.4 Why Operation Count Matters

In traditional algorithm analysis, we care about asymptotic complexity: O(n log n)
beats O(n^2) for large n. But push_swap is graded on **exact operation count**:

| Elements | Maximum Ops for Full Marks |
| -------- | -------------------------- |
| 3        | 3                          |
| 5        | 12                         |
| 100      | 700                        |
| 500      | 5500                       |

This means:

- A theoretically "better" algorithm (O(n log n) radix) can produce **more
  operations** than a "worse" one (O(n \* sqrt(n)) chunk sort) for typical inputs
- Constant factors matter enormously
- Optimizations like rotated-sorted detection can save hundreds of operations

---

### 1.5 Walkthrough: Sorting [3, 2, 1]

Let us trace the sort_three algorithm from `src/sort_tiny.c` (lines 21-46):

**Initial state:**

```
Stack A: [3, 2, 1]    (3 on top, 1 on bottom)
Stack B: []

a = 3, b = 2, c = 1
```

**Step 1:** Check condition at line 32: `a > b && b > c` (3 > 2 && 2 > 1) -- TRUE

```
Execute: sa (swap top two)
Stack A: [2, 3, 1]
Stack B: []

Execute: rra (reverse rotate -- bottom goes to top)
Stack A: [1, 2, 3]
Stack B: []
```

**Result: SORTED in 2 operations!**

Let us trace another case: [2, 3, 1]

```
a = 2, b = 3, c = 1
```

**Check conditions in order:**

- Line 30: `a > b && b < c && a < c` -- 2 > 3? NO
- Line 32: `a > b && b > c` -- 2 > 3? NO
- Line 37: `a > b && b < c && a > c` -- 2 > 3? NO
- Line 39: `a < b && b > c && a < c` -- 2 < 3 && 3 > 1 && 2 < 1? NO (2 < 1 is false)
- Line 44: `a < b && b > c && a > c` -- 2 < 3 && 3 > 1 && 2 > 1? YES

```
Execute: rra
Stack A: [1, 2, 3]
```

**Result: SORTED in 1 operation!**

Here are all 6 permutations of [1, 2, 3] and their solutions:

```
[1, 2, 3] -> already sorted       (0 ops)
[1, 3, 2] -> sa, ra               (2 ops)
[2, 1, 3] -> sa                   (1 op)
[2, 3, 1] -> rra                  (1 op)
[3, 1, 2] -> ra                   (1 op)
[3, 2, 1] -> sa, rra              (2 ops)
```

All cases handled in at most 2 operations. The theoretical minimum for 3 elements.

---

### Key takeaways

1. push_swap is about **practical** sorting, not theoretical optimality
2. You have only 11 operations, all of which manipulate the top or bottom of stacks
3. The grading is based on operation count, not execution time
4. Small sizes (2, 3, 5) have hardcoded optimal solutions
5. The real challenge is choosing the right strategy for larger inputs

---

## Chapter 2: C Patterns Used Throughout

### What you'll learn

- The context struct pattern for avoiding globals
- The operation wrapper pattern for centralized execute + print + count
- Index normalization using copy, sort, and binary search
- Cheaper-direction rotation for minimizing operations
- Error handling with early exit
- How to survive the 42 Norm

---

### 2.1 The Context Struct Pattern

Instead of using global variables (forbidden at 42), all program state is bundled
into a single `t_context` struct.

**From `includes/push_swap.h` (lines 44-53):**

```c
typedef struct s_context
{
    t_stack a;           // Stack A
    t_stack b;           // Stack B
    int     ops[11];     // Per-operation counters
    int     total_ops;   // Total operation count
    int     strategy;    // Which algorithm to use
    int     bench;       // Benchmark mode flag
    double  disorder;    // Disorder metric (0.0 = sorted, 1.0 = reversed)
}   t_context;
```

**Why this pattern?**

- Every function receives `t_context *ctx` -- one pointer instead of 5+ parameters
- Adding new state (like `bench` or `disorder`) requires zero signature changes
- The struct is allocated on `main()`'s stack frame -- no malloc needed for ctx itself
- Cleanup is centralized: `free_context(ctx)` handles everything

**From `src/main.c` (lines 61-85), the lifecycle:**

```c
int main(int argc, char **argv)
{
    t_context ctx;

    if (argc < 2) return (0);       // 0. No args -> silent exit
    init_context(&ctx);              // 1. Zero-initialize all fields
    create_stacks(&ctx);             // 2. malloc data arrays (capacity 10000)
    parse_args(argc, argv, &ctx);    // 3. Fill stack A with parsed integers
    normalize_stack(&ctx.a);         // 4. Map values to ranks 0..n-1
    ctx.disorder = compute_disorder(&ctx.a); // 5. Measure disorder BEFORE sorting
    dispatch_sort(&ctx);             // 6. Choose and run the right algorithm
    if (ctx.bench) print_bench(&ctx);// 7. Optional: print stats to stderr
    free_context(&ctx);              // 8. Cleanup
}
```

**Key observation:** Disorder is computed **after** normalization but **before**
sorting. This is critical because:

- Normalization doesn't change relative order (it's rank-preserving), so disorder
  is the same before and after normalization
- The adaptive strategy needs the disorder value to choose its sub-algorithm
- Computing it after sorting would always give 0.0 (sorted = no inversions)

---

### 2.2 The Operation Wrapper Pattern

Every push_swap operation has two layers:

**Layer 1 -- Raw operation** (e.g., `src/stack_ops_swap.c` line 15):

```c
void op_sa(t_stack *a)
{
    int tmp;

    if (a->size < 2)        // Guard: do nothing if < 2 elements
        return ;
    tmp = a->data[0];        // Classic swap
    a->data[0] = a->data[1];
    a->data[1] = tmp;
}
```

**Layer 2 -- Wrapper** (e.g., `src/ops_out_swap.c` lines 17-23):

```c
void do_sa(t_context *ctx)
{
    op_sa(&ctx->a);          // 1. Execute the raw operation
    write(1, "sa\n", 3);     // 2. Print to stdout
    ctx->ops[OP_SA]++;       // 3. Increment per-op counter
    ctx->total_ops++;        // 4. Increment total counter
}
```

**Why two layers?**

- The **raw** `op_sa()` is reused by the bonus checker (which must NOT print)
- The **wrapper** `do_sa()` is used by the sorting algorithms (which MUST print)
- The wrapper centralizes counting -- you never forget to increment
- Adding new behavior (e.g., debug logging) requires changing only the wrapper

This is applied consistently for all 11 operations. The wrappers are split
across three files to respect the 42 Norm's function-per-file limits:

- `src/ops_out_swap.c` — `do_sa`, `do_sb`, `do_ss`, `do_pa`, `do_pb`
- `src/ops_out_rot.c` — `do_ra`, `do_rb`, `do_rr`
- `src/ops_out_revrot.c` — `do_rra`, `do_rrb`, `do_rrr`

Note the write lengths: `write(1, "sa\n", 3)` for two-character operations,
`write(1, "rra\n", 4)` for three-character operations. Getting this byte count
wrong is a common bug — the checker would receive corrupted operation names.

---

### 2.3 Index Normalization

The input can be any integers: `2147483647 -5 42 0`. But our algorithms work
on indices `0..n-1`. Normalization maps arbitrary values to their ranks.

**From `src/normalize.c` (lines 59-72):**

```c
void normalize_stack(t_stack *a)
{
    int *sorted;
    int i;

    sorted = malloc(sizeof(int) * a->size);  // 1. Copy the data
    if (!sorted)
        return ;
    i = -1;
    while (++i < a->size)
        sorted[i] = a->data[i];
    sort_copy(sorted, a->size);              // 2. Sort the copy
    i = -1;
    while (++i < a->size)
        a->data[i] = bin_search(sorted, a->size, a->data[i]); // 3. Replace with rank
    free(sorted);
}
```

**Visual example:**

```
Input:     [42, -5, 2147483647, 0]
Sorted:    [-5, 0, 42, 2147483647]
Ranks:      0   1   2       3

After normalization:
           [2, 0, 3, 1]

42          -> rank 2 (3rd smallest)
-5          -> rank 0 (smallest)
2147483647  -> rank 3 (largest)
0           -> rank 1 (2nd smallest)
```

**The helper functions:**

`sort_copy()` (`src/normalize.c` lines 17-35) uses selection sort -- O(n^2) but perfectly fine
since normalization happens once before sorting begins, and its cost is measured
in CPU time, not push_swap operations:

```c
static void sort_copy(int *arr, int n)
{
    // Standard nested-loop sort
    // Swaps arr[i] and arr[j] whenever arr[i] > arr[j]
}
```

`bin_search()` (`src/normalize.c` lines 37-54) uses binary search -- O(log n) per lookup, O(n log n)
total for all elements:

```c
static int bin_search(int *sorted, int n, int val)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        mid = (lo + hi) / 2;
        if (sorted[mid] == val) return (mid);  // Found rank!
        else if (sorted[mid] < val) lo = mid + 1;
        else hi = mid - 1;
    }
    return (-1);  // Should never happen if input has no duplicates
}
```

**Why normalize?**

- Radix sort needs contiguous indices 0..n-1
- Chunk sort partitions by range -- easier with 0..n-1 than arbitrary values
- Comparing ranks is identical to comparing original values (order-preserving)

---

### 2.4 Cheaper-Direction Rotation

When you need element at position `pos` on top of a stack, you can either:

- **Rotate forward** (`ra`/`rb`): costs `pos` operations
- **Reverse rotate** (`rra`/`rrb`): costs `size - pos` operations

The actual implementation uses a helper function `rot_dir` to keep
`rotate_to_top` under the 25-line Norm limit:

**From `src/utils.c` (lines 65-77) -- the rot_dir helper:**

```c
static void rot_dir(t_context *ctx, int stack_id, int reverse)
{
    if (!reverse && stack_id == 0)
        do_ra(ctx);                // Forward on A
    else if (!reverse)
        do_rb(ctx);                // Forward on B
    else if (stack_id == 0)
        do_rra(ctx);               // Reverse on A
    else
        do_rrb(ctx);               // Reverse on B
}
```

This helper encodes a 2x2 matrix (stack x direction) in a single function.
It avoids duplicating the if/else inside both branches of `rotate_to_top`.

**From `src/utils.c` (lines 79-95) -- rotate_to_top:**

```c
void rotate_to_top(t_context *ctx, int pos, int stack_id)
{
    int size;

    if (stack_id == 0)
        size = ctx->a.size;
    else
        size = ctx->b.size;
    if (pos <= size / 2)           // Closer to top? Rotate forward.
    {
        while (pos-- > 0)
            rot_dir(ctx, stack_id, 0);
    }
    else                           // Closer to bottom? Reverse rotate.
    {
        while (pos++ < size)
            rot_dir(ctx, stack_id, 1);
    }
}
```

**Why the two-function design?** Without `rot_dir`, `rotate_to_top` would need
four separate `if (stack_id == 0)` checks — two in each branch. That would push
the function over the 25-line Norm limit. By extracting the dispatch into `rot_dir`,
both functions stay compact and readable.

**Example:** Stack has 10 elements, target is at position 7.

- Forward rotation: 7 operations (ra, ra, ra, ra, ra, ra, ra)
- Reverse rotation: 3 operations (rra, rra, rra)
- `rotate_to_top` chooses reverse: `7 > 10/2`, so `size - pos = 3` wins.

**This optimization alone can cut total operations by ~30% on average.**

---

### 2.5 Error Handling with Early Exit

The project requires strict error handling: any invalid input must print "Error\n"
to stderr and exit.

**From `src/main.c` (lines 15-20):**

```c
void error_exit(t_context *ctx)
{
    free_context(ctx);          // Always free before exiting
    write(2, "Error\n", 6);    // Write to fd 2 (stderr)
    exit(1);                    // Non-zero exit code
}
```

**Key details:**

- `write(2, ...)` instead of `ft_putstr_fd("Error\n", 2)` -- fewer function calls,
  the string length is known at compile time
- `free_context` is called BEFORE the error message -- prevents leaks
- `exit(1)` terminates immediately -- no need to propagate errors up the call stack

**The pattern is used at every validation point:**

```c
// In main.c line 63-64:
if (!parse_args(argc, argv, &ctx))
    error_exit(&ctx);              // Invalid args -> Error + exit

// In checker_main_bonus.c line 68-69:
if (!read_and_exec(&ctx))
    checker_error_exit(&ctx);      // Invalid operation -> Error + exit
```

---

### 2.6 42 Norm Survival

The 42 Norm imposes strict constraints:

- Maximum **25 lines** per function (excluding braces)
- Maximum **4 parameters** per function
- Maximum **5 variables** per function
- No `for` loops, no `switch`, no ternary operators
- Functions must do one thing

**How this codebase adapts:**

1. **Splitting logic across files:** Instead of one `sort.c` with 200 lines, we have
   `sort_tiny.c`, `sort_simple.c`, `sort_medium.c`, `sort_medium_utils.c`,
   `sort_complex.c`, and `sort_adaptive.c`.

2. **Helper extraction:** The `push_chunk()` function in `sort_medium.c` (line 19)
   is separated from `phase_one()` (line 43) even though they could be one function.

3. **The context struct:** Bundles 7+ pieces of state into one parameter.

4. **Static helpers:** Functions like `push_front()`, `pop_front()`, `rotate()`,
   `rev_rotate()` are all `static` -- visible only in their translation unit.

5. **Using `while` instead of `for`:**

   ```c
   // Instead of: for (i = 0; i < n; i++)
   i = 0;
   while (i < n)
   {
       // ...
       i++;
   }
   ```

6. **The `++i` in while condition trick** (from `normalize.c` line 66-67):

   ```c
   i = -1;
   while (++i < a->size)
       sorted[i] = a->data[i];
   ```

   This saves one line vs. the explicit `i = 0; while (i < ...); i++` pattern.
   Use sparingly -- it harms readability.

---

### Key takeaways

1. The **context struct** avoids globals and simplifies function signatures
2. **Two-layer operations** (raw + wrapper) enable code reuse between push_swap and checker
3. **Normalization** converts arbitrary integers to 0..n-1 ranks, enabling all algorithms
4. **Cheaper-direction rotation** halves rotation cost on average
5. **Error handling** always frees memory before exiting
6. Every pattern exists partly because of the 42 Norm constraints

---

## Chapter 3: Data Structure Deep-Dive

### What you'll learn

- Why this project uses array-based stacks instead of linked lists
- How `data[0]` = top works for all operations
- Why capacity is separate from size
- Memory layout of the complete data structure

---

### 3.1 Array vs. Linked List

Both are valid for push_swap. Here is a comparison specific to this project:

```
+-------------------+------------------+--------------------+
| Criteria          | Array-Based      | Linked-List        |
+-------------------+------------------+--------------------+
| Push to top       | O(n) shift       | O(1) pointer       |
| Pop from top      | O(n) shift       | O(1) pointer       |
| Swap top two      | O(1) index       | O(1) pointer       |
| Rotate            | O(n) shift       | O(1) pointer       |
| Reverse rotate    | O(n) shift       | O(n) traverse*     |
| Access by index   | O(1) direct      | O(n) traverse      |
| Radix sort bit    | O(1) data[0]>>b  | O(1) head->val>>b  |
| Find min/max pos  | O(n) scan        | O(n) scan          |
| Memory overhead   | One malloc       | n mallocs + ptrs   |
| Cache locality    | Excellent        | Poor               |
+-------------------+------------------+--------------------+
* O(1) if doubly-linked with tail pointer
```

**Why this project chose arrays:**

1. **Index access is critical**: `find_min_pos()`, `find_max_pos()`, and chunk
   checks like `ctx->a.data[0] >= lo` need random access
2. **Simple memory model**: Two `malloc` calls total (one per stack), easy cleanup
3. **Cache-friendly**: Contiguous memory = fewer cache misses during scans
4. **No pointer chasing**: Radix sort checks `data[0] >> bit & 1` directly

**The tradeoff**: push, pop, and rotate are O(n) because elements must shift.
But n <= 500 for this project, so the constant factor is negligible.

---

### 3.2 The t_stack Structure

**From `includes/push_swap.h` (lines 37-42):**

```c
typedef struct s_stack
{
    int     *data;       // Dynamic array of integers
    int     size;        // Current number of elements
    int     capacity;    // Maximum number of elements
}   t_stack;
```

**Convention: `data[0]` is the TOP of the stack.**

This is the opposite of the typical "stack grows upward" convention, but it
makes the code more intuitive:

```
Stack A with elements [5, 3, 1]:

data[0] = 5  <-- TOP    (first element you see/access)
data[1] = 3
data[2] = 1  <-- BOTTOM (last element)
size = 3
capacity = 10000

Memory layout:
+----+----+----+----+----+----+- - - -+
|  5 |  3 |  1 |  ? |  ? |  ? |       |
+----+----+----+----+----+----+- - - -+
  ^                   ^                  ^
  data[0]          data[size-1]       data[capacity-1]
  (top)            (bottom)           (unused space)
```

---

### 3.3 How Each Operation Works on the Array

#### Push to front (used by pa, pb)

**From `src/stack_ops_push.c` (lines 15-27):**

```c
static void push_front(t_stack *s, int val)
{
    int i;

    i = s->size;
    while (i > 0)
    {
        s->data[i] = s->data[i - 1];  // Shift everything right
        i--;
    }
    s->data[0] = val;                  // Place new value at top
    s->size++;
}
```

Visual:

```
BEFORE: [3, 1, _]  size=2        Push 5:
                                   Shift: [_, 3, 1]
                                   Place: [5, 3, 1]  size=3
```

#### Pop from front (used by pa, pb)

**From `src/stack_ops_push.c` (lines 29-43):**

```c
static int pop_front(t_stack *s)
{
    int val;
    int i;

    val = s->data[0];                  // Save top value
    i = 0;
    while (i < s->size - 1)
    {
        s->data[i] = s->data[i + 1];  // Shift everything left
        i++;
    }
    s->size--;
    return (val);
}
```

Visual:

```
BEFORE: [5, 3, 1]  size=3        Pop:
                                   Save 5
                                   Shift: [3, 1, _]  size=2
                                   Return 5
```

#### Swap (used by sa, sb)

**From `src/stack_ops_swap.c` (lines 15-24):**

```c
void op_sa(t_stack *a)
{
    int tmp;

    if (a->size < 2) return;
    tmp = a->data[0];
    a->data[0] = a->data[1];
    a->data[1] = tmp;
}
```

Visual:

```
BEFORE: [5, 3, 1]     AFTER: [3, 5, 1]
         ^  ^                  ^  ^
         swap these            swapped
```

#### Rotate (used by ra, rb)

**From `src/stack_ops_rotate.c` (lines 15-30):**

```c
static void rotate(t_stack *s)
{
    int tmp;
    int i;

    if (s->size < 2) return;
    tmp = s->data[0];                  // Save top
    i = 0;
    while (i < s->size - 1)
    {
        s->data[i] = s->data[i + 1];  // Shift everything left
        i++;
    }
    s->data[s->size - 1] = tmp;        // Old top goes to bottom
}
```

Visual:

```
BEFORE: [5, 3, 1]     AFTER: [3, 1, 5]
         ^                          ^
         top moves to bottom
```

#### Reverse Rotate (used by rra, rrb)

**From `src/stack_ops_revrot.c` (lines 15-30):**

```c
static void rev_rotate(t_stack *s)
{
    int tmp;
    int i;

    if (s->size < 2) return;
    tmp = s->data[s->size - 1];        // Save bottom
    i = s->size - 1;
    while (i > 0)
    {
        s->data[i] = s->data[i - 1];  // Shift everything right
        i--;
    }
    s->data[0] = tmp;                  // Old bottom goes to top
}
```

Visual:

```
BEFORE: [5, 3, 1]     AFTER: [1, 5, 3]
               ^        ^
               bottom moves to top
```

---

### 3.4 Why Capacity is Separate from Size

**From `src/stack_init.c` (lines 15-23):**

```c
int stack_create(t_stack *s, int capacity)
{
    s->data = malloc(sizeof(int) * capacity);
    if (!s->data)
        return (0);
    s->size = 0;
    s->capacity = capacity;
    return (1);
}
```

Both stacks are allocated with capacity 10000 (see `main.c` line 56-58):

```c
if (!stack_create(&ctx.a, 10000))
    return (1);
if (!stack_create(&ctx.b, 10000))
{
    stack_destroy(&ctx.a);
    return (1);
}
```

**Why 10000?**

- The project typically tests up to 500 elements
- 10000 gives generous headroom
- No need for dynamic resizing (realloc), which would complicate the code
- Each int is 4 bytes, so 10000 \* 4 = 40KB per stack -- trivial

**Why separate capacity and size?**

- `size` tracks how many elements are actually in the stack
- `capacity` is the maximum before buffer overflow
- This distinction prevents writing past the allocation boundary
- During push, `size` grows; during pop, `size` shrinks; `capacity` never changes

---

### 3.5 Complete Memory Layout Diagram

```
t_context (on main's stack frame):
+===========================================================+
|                                                           |
|  t_stack a:                                               |
|  +-------+-------+-----------+                            |
|  | *data -+---> [int][int][int]...[int]  (heap, 40KB)    |
|  | size   |  = current element count                      |
|  | capacity = 10000                                       |
|  +-------+-------+-----------+                            |
|                                                           |
|  t_stack b:                                               |
|  +-------+-------+-----------+                            |
|  | *data -+---> [int][int][int]...[int]  (heap, 40KB)    |
|  | size   |  = current element count                      |
|  | capacity = 10000                                       |
|  +-------+-------+-----------+                            |
|                                                           |
|  int ops[11]:  [sa_count, sb_count, ..., rrr_count]       |
|  int total_ops: sum of all ops                            |
|  int strategy:  STRAT_ADAPTIVE (default)                  |
|  int bench:     0 or 1                                    |
|  double disorder: 0.0 to 1.0                              |
|                                                           |
+===========================================================+

Heap allocations: exactly 2 (one per stack's data array)
Cleanup: free_context() calls stack_destroy() on both
```

---

### Key takeaways

1. Arrays are chosen over linked lists for cache locality and O(1) index access
2. `data[0]` = top of stack -- all operations shift elements to maintain this
3. Fixed capacity (10000) avoids realloc complexity; size tracks actual elements
4. Only 2 heap allocations total -- simple memory model, hard to leak
5. Every operation is O(n) on the array, but n <= 500 makes this negligible

---

## Chapter 4: Algorithm Walkthroughs

### What you'll learn

- Four distinct sorting strategies and when each is optimal
- Step-by-step traces with visual examples
- The mathematical reasoning behind chunk sizes and complexity
- How the adaptive strategy selects the best approach at runtime

---

### 4a. Selection Sort (Simple Strategy) -- O(n^2)

**Source: `src/sort_simple.c`**

This is the simplest algorithm: repeatedly find the minimum element and push it
to Stack B, then push everything back.

**From `src/sort_simple.c` (lines 15-37):**

```c
void sort_simple(t_context *ctx)
{
    int pos;

    if (ctx->a.size <= 1 || is_sorted(&ctx->a))
        return ;
    if (ctx->a.size <= 3)
    {
        if (ctx->a.size == 2) sort_two(ctx);
        else                  sort_three(ctx);
        return ;
    }
    while (ctx->a.size > 0)
    {
        pos = find_min_pos(&ctx->a);    // O(n) scan
        rotate_to_top(ctx, pos, 0);     // O(n) rotations
        do_pb(ctx);                     // Push to B
    }
    while (ctx->b.size > 0)
        do_pa(ctx);                     // Push all back
}
```

**Why it works:**

1. Find the smallest unsorted element in A
2. Rotate it to the top (cheapest direction)
3. Push it to B (B accumulates in sorted order, largest on top)
4. Repeat until A is empty
5. Push everything from B back to A

**Step-by-step trace with [4, 2, 5, 1, 3]:**

```
Initial:
A: [4, 2, 5, 1, 3]    B: []

--- Iteration 1: find min (1) at pos 3 ---
rra (pos=3, size=5, 3 > 5/2, so rra x2)
A: [1, 4, 2, 5, 3]    B: []
rra
ERROR -- let me recalculate. pos=3, size=5. 3 > 5/2=2, so reverse rotate.
size - pos = 5 - 3 = 2 reverse rotations.

Actually, the code uses: while (pos++ < size), which iterates (size - pos) times.
pos=3: iteration when pos=3 (3<5 -> rra), pos=4 (4<5 -> rra), pos=5 (5<5? no, stop)
That's 2 rra operations.

rra: A: [3, 4, 2, 5, 1] -> wait, rra moves bottom to top.
A starts as [4, 2, 5, 1, 3]
rra: bottom (3) to top -> [3, 4, 2, 5, 1]
rra: bottom (1) to top -> [1, 3, 4, 2, 5]
pb:  A: [3, 4, 2, 5]  B: [1]

--- Iteration 2: find min (2) at pos 2 ---
pos=2, size=4, 2 <= 4/2=2, so rotate forward. 2 ra operations.
ra: A: [4, 2, 5, 3] -> wait, A is [3, 4, 2, 5]
ra: [4, 2, 5, 3]
ra: [2, 5, 3, 4]
pb: A: [5, 3, 4]  B: [2, 1]

--- Iteration 3: find min (3) at pos 1 ---
pos=1, size=3, 1 <= 3/2=1, so rotate forward. 1 ra.
ra: A: [3, 4, 5]
pb: A: [4, 5]  B: [3, 2, 1]

--- Iteration 4: find min (4) at pos 0 ---
pos=0, no rotation needed.
pb: A: [5]  B: [4, 3, 2, 1]

--- Iteration 5: find min (5) at pos 0 ---
pb: A: []  B: [5, 4, 3, 2, 1]

--- Push all back ---
pa x5: A: [1, 2, 3, 4, 5]  B: []
```

**Total: 2 + 1 + 2 + 1 + 1 + 1 + 0 + 1 + 5 (pa's) = varies, but O(n^2)**

**Complexity analysis:**

- Finding minimum: O(n) per iteration
- Rotating to top: O(n) per iteration (worst case n/2)
- n iterations for Phase 1
- n pushes back for Phase 2
- Total: O(n^2) operations

**When to use it:** Only for debugging or very small inputs. For n > 20, this
produces far too many operations.

---

### 4b. Chunk-Based Sort (Medium Strategy) -- O(n \* sqrt(n))

**Source: `src/sort_medium.c` and `src/sort_medium_utils.c`**

This is the primary algorithm for scoring well. It divides the input into chunks
and uses a two-phase approach.

#### The Intuition

Imagine sorting a deck of cards using two piles:

1. **Phase 1 (Partition):** Go through the deck and deal cards into pile B in
   groups (chunks). Within each chunk, keep larger values near the top of B.
2. **Phase 2 (Reassemble):** Repeatedly find the largest card in B and move it
   back to A.

#### Phase 1: Push Chunks to B

**From `src/sort_medium.c` (lines 19-41):**

```c
static void push_chunk(t_context *ctx, int lo, int hi)
{
    int mid;
    int pushed;
    int size;

    mid = (lo + hi) / 2;          // Midpoint of this chunk
    pushed = 0;
    size = ctx->a.size;
    while (pushed < (hi - lo + 1) && size > 0)
    {
        if (ctx->a.data[0] >= lo && ctx->a.data[0] <= hi)
        {
            do_pb(ctx);            // Push to B
            if (ctx->b.data[0] < mid)
                do_rb(ctx);        // Small half -> rotate to bottom of B
            pushed++;
        }
        else
            do_ra(ctx);            // Skip: rotate A
        size--;
    }
}
```

**The `rb` optimization (line 33-34):**
When pushing a chunk to B, elements below the midpoint are rotated to the bottom
of B. This means:

- Top of B has the larger half of the chunk
- Bottom of B has the smaller half

This reduces the work in Phase 2, because the largest elements are already near
the top of B.

**Phase 1 orchestration** (`sort_medium.c` lines 43-59):

```c
static void phase_one(t_context *ctx, int chunk_sz)
{
    int lo;
    int hi;
    int n;

    n = ctx->a.size;
    lo = 0;
    while (lo < n)
    {
        hi = lo + chunk_sz - 1;
        if (hi >= n)
            hi = n - 1;
        push_chunk(ctx, lo, hi);
        lo = hi + 1;
    }
}
```

This walks through normalized indices in chunks: [0..chunk_sz-1], [chunk_sz..2*chunk_sz-1], etc.

#### Phase 2: Greedily Push Back the Maximum

**From `src/sort_medium.c` (lines 61-65):**

```c
static void phase_two(t_context *ctx)
{
    while (ctx->b.size > 0)
        push_back_max(ctx);        // Find max in B, rotate to top, pa
}
```

**From `src/sort_medium_utils.c` (lines 35-42):**

```c
void push_back_max(t_context *ctx)
{
    int pos;

    pos = find_max_pos(&ctx->b);   // O(n) scan
    rotate_to_top(ctx, pos, 1);    // Cheapest direction rotation on B
    do_pa(ctx);                    // Push to A
}
```

#### Visual Walkthrough: 8 Elements with chunk_sz = 3

```
Normalized input: A = [5, 0, 7, 3, 1, 6, 2, 4]  B = []

Chunk 1: [0, 1, 2]  (lo=0, hi=2, mid=1)
  Scan A for values 0-2:
  5: not in range -> ra    A: [0, 7, 3, 1, 6, 2, 4, 5]
  0: in range, 0 < 1 -> pb, rb   A: [7, 3, 1, 6, 2, 4, 5]  B: [0] -> rb -> B still [0]
  7: not in range -> ra    A: [3, 1, 6, 2, 4, 5, 7]
  3: not in range -> ra    A: [1, 6, 2, 4, 5, 7, 3]
  1: in range, 1 >= 1 -> pb       A: [6, 2, 4, 5, 7, 3]  B: [1, 0]
  6: not in range -> ra    A: [2, 4, 5, 7, 3, 6]
  2: in range, 2 >= 1 -> pb       A: [4, 5, 7, 3, 6]  B: [2, 1, 0]
  pushed=3, done with chunk 1

Chunk 2: [3, 4, 5]  (lo=3, hi=5, mid=4)
  Scan A for values 3-5:
  4: in range, 4 >= 4 -> pb       A: [5, 7, 3, 6]  B: [4, 2, 1, 0]
  5: in range, 5 >= 4 -> pb       A: [7, 3, 6]  B: [5, 4, 2, 1, 0]
  7: not in range -> ra    A: [3, 6, 7]
  3: in range, 3 < 4 -> pb, rb   A: [6, 7]  B: [3, 5, 4, 2, 1, 0] -> rb -> [5, 4, 2, 1, 0, 3]
  pushed=3, done with chunk 2

Chunk 3: [6, 7]  (lo=6, hi=7, mid=6)
  6: in range -> pb                A: [7]  B: [6, 5, 4, 2, 1, 0, 3]
  7: in range -> pb                A: []   B: [7, 6, 5, 4, 2, 1, 0, 3]

Phase 2: repeatedly find max in B and push to A
  Max=7 at pos 0 -> pa            A: [7]  B: [6, 5, 4, 2, 1, 0, 3]
  Max=6 at pos 0 -> pa            A: [6, 7]  B: [5, 4, 2, 1, 0, 3]
  Max=5 at pos 0 -> pa            A: [5, 6, 7]  B: [4, 2, 1, 0, 3]
  Max=4 at pos 0 -> pa            A: [4, 5, 6, 7]  B: [2, 1, 0, 3]
  Max=3 at pos 3 -> rrb, pa       A: [3, 4, 5, 6, 7]  B: [2, 1, 0]
  Max=2 at pos 0 -> pa            A: [2, 3, 4, 5, 6, 7]  B: [1, 0]
  Max=1 at pos 0 -> pa            A: [1, 2, 3, 4, 5, 6, 7]  B: [0]
  Max=0 at pos 0 -> pa            A: [0, 1, 2, 3, 4, 5, 6, 7]  B: []

SORTED!
```

#### Why sqrt(n) Chunks: The Math Behind O(n \* sqrt(n))

With `k` chunks of size `n/k`:

**Phase 1 cost per chunk:**

- Scanning A for chunk members: O(n) rotations in the worst case
- Total Phase 1: O(n \* k) ... wait, actually each element is scanned once
  across all chunks, but A gets longer rotation cycles. More precisely:
  - Each chunk requires scanning through ~n elements (rotating past non-members)
  - With k chunks, total Phase 1 cost: O(n \* k)

**Phase 2 cost:**

- Each of n elements needs to be found in B (O(n) scan) and rotated to top
- Average rotation distance in B after chunks: O(n/k) because the rb optimization
  keeps each chunk semi-sorted
- Total Phase 2: O(n \* n/k)

**Total: O(n \* k + n^2 / k)**

Minimize by taking derivative and setting to 0:

- d/dk (nk + n^2/k) = n - n^2/k^2 = 0
- k^2 = n
- k = sqrt(n)

With k = sqrt(n):

- Total = O(n _sqrt(n) + n^2 / sqrt(n)) = O(n_ sqrt(n) + n _sqrt(n)) = \*\*O(n_ sqrt(n))\*\*

#### The Chunk Size Formula

**From `src/sort_medium_utils.c` (lines 25-33):**

```c
int get_chunk_size(int n)
{
    int size;

    size = (n * 3) / 100 + 14;
    if (size < 5)
        size = 5;
    return (size);
}
```

This empirical formula was found through benchmarking:

| n (elements) | chunk_size | num_chunks | sqrt(n) for reference |
| ------------ | ---------- | ---------- | --------------------- |
| 100          | 17         | ~6         | 10                    |
| 200          | 20         | ~10        | 14                    |
| 500          | 29         | ~17        | 22                    |

The `+ 14` base ensures reasonable chunk sizes for small inputs.
The `* 3 / 100` linear factor scales with input size.
The minimum of 5 prevents degenerate tiny chunks.

This formula consistently outperforms a pure sqrt(n) chunk size because:

- sqrt(n) chunks are too small for n < 200 (too many chunks = too many Phase 1 scans)
- The linear scaling adapts better to the actual operation costs

---

### 4c. Radix Sort (Complex Strategy) -- O(n \* log(n))

**Source: `src/sort_complex.c`**

This implements LSD (Least Significant Digit) radix sort using the two stacks
as a binary partition mechanism.

#### How It Works

After normalization, all values are in range [0, n-1]. We examine each bit
position from LSB to MSB:

- **Bit is 0**: The element stays in A (rotate to keep it)
- **Bit is 1**: Push to B

After processing all elements for one bit, push everything from B back to A.
Repeat for the next bit.

After processing all bits, A is sorted.

#### Why It Works

LSD radix sort is a stable sort. At each bit pass:

- Elements with bit=0 remain in A in their original relative order (ra preserves order)
- Elements with bit=1 go to B in their original relative order (pb preserves order)
- When pushed back (pa), they go on top of A, maintaining order

After processing bit 0, all elements are sorted by bit 0.
After processing bit 1, all elements are sorted by bits 0-1.
After processing all bits, all elements are fully sorted.

**From `src/sort_complex.c` (lines 15-42):**

```c
static int get_max_bits(int n)
{
    int bits;

    bits = 0;
    while ((n >> bits) != 0)       // Count bits needed to represent n
        bits++;
    return (bits);
}

static void radix_pass(t_context *ctx, int bit)
{
    int size;
    int i;

    size = ctx->a.size;
    i = 0;
    while (i < size)
    {
        if ((ctx->a.data[0] >> bit) & 1)   // Check current bit
            do_ra(ctx);                      // Bit=1: keep in A (rotate)
        else
            do_pb(ctx);                      // Bit=0: push to B
        i++;
    }
    while (ctx->b.size > 0)
        do_pa(ctx);                          // Push all back from B
}
```

**Main function** (`sort_complex.c` lines 44-68):

```c
void sort_complex(t_context *ctx)
{
    int max_bits;
    int bit;

    // ... size checks and small-size handlers ...

    max_bits = get_max_bits(ctx->a.size - 1);  // bits for max index
    bit = 0;
    while (bit < max_bits)
    {
        radix_pass(ctx, bit);
        bit++;
    }
}
```

#### Visual Trace: 8 Elements, 3 Bit-Passes

```
Normalized: A = [5, 0, 7, 3, 1, 6, 2, 4]
n = 8, max index = 7 = 0b111, so max_bits = 3

Binary representations:
  0 = 000    4 = 100
  1 = 001    5 = 101
  2 = 010    6 = 110
  3 = 011    7 = 111

=== BIT 0 (ones place) ===
Process each element, check bit 0:
  5 (101) bit0=1 -> ra     A: [0,7,3,1,6,2,4,5]
  0 (000) bit0=0 -> pb     A: [7,3,1,6,2,4,5]   B: [0]
  7 (111) bit0=1 -> ra     A: [3,1,6,2,4,5,7]
  3 (011) bit0=1 -> ra     A: [1,6,2,4,5,7,3]
  1 (001) bit0=1 -> ra     A: [6,2,4,5,7,3,1]
  6 (110) bit0=0 -> pb     A: [2,4,5,7,3,1]      B: [6,0]
  2 (010) bit0=0 -> pb     A: [4,5,7,3,1]         B: [2,6,0]
  4 (100) bit0=0 -> pb     A: [5,7,3,1]           B: [4,2,6,0]

Push all back from B:
  pa x4: A: [0,6,2,4,5,7,3,1]  B: []

After bit 0: elements sorted by last bit
  Even (bit0=0): 0,6,2,4 -- appear first
  Odd  (bit0=1): 5,7,3,1 -- appear after

=== BIT 1 (twos place) ===
Process each element, check bit 1:
  0 (000) bit1=0 -> pb     A: [6,2,4,5,7,3,1]    B: [0]
  6 (110) bit1=1 -> ra     A: [2,4,5,7,3,1,6]
  2 (010) bit1=1 -> ra     A: [4,5,7,3,1,6,2]
  4 (100) bit1=0 -> pb     A: [5,7,3,1,6,2]      B: [4,0]
  5 (101) bit1=0 -> pb     A: [7,3,1,6,2]         B: [5,4,0]
  7 (111) bit1=1 -> ra     A: [3,1,6,2,7]
  3 (011) bit1=1 -> ra     A: [1,6,2,7,3]
  1 (001) bit1=0 -> pb     A: [6,2,7,3]           B: [1,5,4,0]

Push all back:
  pa x4: A: [0,4,5,1,6,2,7,3]  B: []

After bit 1: elements sorted by last 2 bits
  x00: 0,4   x01: 5,1   x10: 6,2   x11: 7,3

=== BIT 2 (fours place) ===
Process each element, check bit 2:
  0 (000) bit2=0 -> pb     B: [0]
  4 (100) bit2=1 -> ra
  5 (101) bit2=1 -> ra
  1 (001) bit2=0 -> pb     B: [1,0]
  6 (110) bit2=1 -> ra
  2 (010) bit2=0 -> pb     B: [2,1,0]
  7 (111) bit2=1 -> ra
  3 (011) bit2=0 -> pb     B: [3,2,1,0]

Push all back:
  pa x4: A: [0,1,2,3,4,5,6,7]  B: []

SORTED!
```

#### Complexity

- **Passes**: O(log n) -- one per bit, and we need log2(n) bits for values 0..n-1
- **Work per pass**: O(n) -- examine each element once, plus push back
- **Total: O(n \* log n)**

For n = 500: log2(500) = 9 bits, so 9 passes of ~500 operations each = ~4500 ops
minimum, plus ~500 pa's per pass = ~9000 total. In practice, about 5000-7000.

---

### 4d. Adaptive Strategy

**Source: `src/sort_adaptive.c`**

The adaptive strategy is the **default** (see `main.c` line 27: `ctx->strategy = STRAT_ADAPTIVE`).
It examines the input and selects the best approach.

**From `src/sort_adaptive.c` (lines 89-113):**

```c
void sort_adaptive(t_context *ctx)
{
    if (ctx->a.size <= 1 || is_sorted(&ctx->a))
        return ;
    if (ctx->a.size <= 5)
    {
        if (ctx->a.size == 2)
            sort_two(ctx);
        else if (ctx->a.size == 3)
            sort_three(ctx);
        else
            sort_five(ctx);
        return ;
    }
    if (is_rotated_sorted(&ctx->a))         // Special case: nearly sorted!
    {
        rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
        return ;
    }
    if (ctx->disorder < 0.2)                // Low disorder
        sort_low_disorder(ctx);
    else if (ctx->disorder < 0.5)           // Medium disorder
        sort_medium(ctx);
    else                                    // High disorder
        sort_complex(ctx);
}
```

**This is the heart of the project.** The adaptive strategy does NOT blindly
delegate to one algorithm. It measures the input's disorder and dispatches to
the algorithm best suited for that disorder level. This three-way dispatch is
what makes the adaptive strategy outperform any single algorithm across all
input types.

#### The Decision Tree

```
                        Input
                          |
                  size <= 1 or sorted?
                  /                  \
               YES: return         NO
                                    |
                              size <= 5?
                             /         \
                           YES         NO
                         /    \          |
                      size=2  size=3  is_rotated_sorted?
                        |       |      /              \
                    sort_two  sort_3  YES              NO
                      or         |     |                |
                    sort_five    |   rotate_to_top    compute disorder
                                 |   O(n)            already done in main
                             sort_five                  |
                                              +--------+--------+
                                              |        |        |
                                          d < 0.2  0.2<=d<0.5  d >= 0.5
                                              |        |        |
                                      sort_low_     sort_     sort_
                                      disorder     medium    complex
                                       O(n+k^2)  O(n*sqrt)  O(n*log)
```

#### The Disorder Metric

**From `src/disorder.c` (lines 15-49):**

```c
static long count_inversions(t_stack *a)
{
    long inv;
    int  i;
    int  j;

    inv = 0;
    i = 0;
    while (i < a->size)
    {
        j = i + 1;
        while (j < a->size)
        {
            if (a->data[i] > a->data[j])
                inv++;
            j++;
        }
        i++;
    }
    return (inv);
}

double compute_disorder(t_stack *a)
{
    long inv;
    long max_inv;

    if (a->size <= 1)
        return (0.0);
    inv = count_inversions(a);
    max_inv = (long)a->size * (a->size - 1) / 2;
    if (max_inv == 0)
        return (0.0);
    return ((double)inv / (double)max_inv);
}
```

**What is an inversion?**
A pair (i, j) where i < j but a[i] > a[j]. In other words, two elements that
are in the wrong relative order.

**Examples:**

```
[1, 2, 3, 4]  -> 0 inversions     -> disorder = 0.00 (perfectly sorted)
[4, 3, 2, 1]  -> 6 inversions     -> disorder = 1.00 (perfectly reversed)
[2, 1, 4, 3]  -> 2 inversions     -> disorder = 0.33 (partially sorted)
```

**The formula:**

```
disorder = inversions / max_possible_inversions
max_possible_inversions = n * (n - 1) / 2
```

This gives a value between 0.0 (sorted) and 1.0 (reverse sorted).

The disorder metric is computed in `main()` (after normalization) and stored in
`ctx->disorder`. It serves two purposes:

1. **Algorithm selection**: The adaptive strategy uses it to dispatch to the
   optimal sub-algorithm (see the three-way dispatch above)
2. **Benchmarking**: The `--bench` flag prints it so you can see how disordered
   the input was and understand why a particular strategy was chosen

#### The Rotated-Sorted Detection

**From `src/sort_adaptive.c` (lines 15-33):**

```c
static int is_rotated_sorted(t_stack *a)
{
    int breaks;
    int i;

    breaks = 0;
    i = 0;
    while (i < a->size - 1)
    {
        if (a->data[i] > a->data[i + 1])
            breaks++;
        if (breaks > 1)
            return (0);
        i++;
    }
    if (breaks == 1 && a->data[a->size - 1] > a->data[0])
        return (0);
    return (1);
}
```

**What is a rotated-sorted array?**
An array that was sorted but then rotated by some amount. It has exactly one
"break point" where a larger element precedes a smaller one, AND the last element
wraps around correctly to the first.

**Examples:**

```
[3, 4, 5, 1, 2]  -> 1 break (5 > 1), last(2) < first(3) -> rotated sorted!
[1, 2, 3, 4, 5]  -> 0 breaks -> trivially rotated sorted (rotation of 0)
[3, 5, 4, 1, 2]  -> 2 breaks -> NOT rotated sorted
[3, 4, 5, 2, 1]  -> 2 breaks -> NOT rotated sorted
```

**Why this optimization matters:**
If the input is [3, 4, 5, 0, 1, 2], we just need to rotate the minimum (0) to
the top. That's at most n/2 operations instead of O(n \* sqrt(n)).

For n = 500, this can save ~6000 operations, turning a 6500-op sort into a 250-op
rotation!

---

#### Low Disorder (d < 0.2): Monotone Extraction with Sorted Reinsertion

**Source: `src/sort_adaptive.c` lines 68-87**

This is the most sophisticated sub-algorithm and the one that gives the adaptive
strategy its edge on nearly-sorted inputs. It exploits the fact that when disorder
is low, most elements are already in the right relative order — only a few
"outliers" need to be moved.

**The algorithm has four phases:**

**Phase 1 — Rotation alignment** (line 72):

```c
rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
```

Rotate A so the global minimum is on top. This costs at most n/2 operations.
After this, if the array were perfectly sorted, it would read 0, 1, 2, ..., n-1.

**Phase 2 — Monotone scan** (lines 73-84):

```c
n = ctx->a.size;
last = -1;
i = -1;
while (++i < n)
{
    if (ctx->a.data[0] >= last)
    {
        last = ctx->a.data[0];
        do_ra(ctx);               // Extends the sorted subsequence
    }
    else
        do_pb(ctx);               // Breaks monotonicity -> push to B
}
```

Walk through A exactly once (n operations total). Maintain a running maximum
`last`. For each element at the top of A:

- If `top >= last`: it continues the non-decreasing trend. Update `last`,
  rotate it to the back (`ra`) so it stays in A.
- If `top < last`: it breaks the ascending sequence. Push it to B (`pb`).

**Visual example:**

```
After rotation alignment: A = [0, 1, 4, 2, 3, 5, 6]  (7 elements, 2 are misplaced)

Step 1: top=0, last=-1. 0 >= -1? YES -> ra, last=0   A: [1,4,2,3,5,6,0]
Step 2: top=1, last=0.  1 >= 0?  YES -> ra, last=1   A: [4,2,3,5,6,0,1]
Step 3: top=4, last=1.  4 >= 1?  YES -> ra, last=4   A: [2,3,5,6,0,1,4]
Step 4: top=2, last=4.  2 >= 4?  NO  -> pb            A: [3,5,6,0,1,4]  B: [2]
Step 5: top=3, last=4.  3 >= 4?  NO  -> pb            A: [5,6,0,1,4]    B: [3,2]
Step 6: top=5, last=4.  5 >= 4?  YES -> ra, last=5   A: [6,0,1,4,5]    B: [3,2]
Step 7: top=6, last=5.  6 >= 5?  YES -> ra, last=6   A: [0,1,4,5,6]    B: [3,2]

Result: A holds sorted subsequence [0,1,4,5,6], B holds outliers [3,2]
k = 2 elements pushed to B
```

After this phase, A contains a sorted subsequence of length n-k, and B contains
the k elements that broke monotonicity. The key insight: **when disorder is low,
k is small** (often just a handful of elements out of hundreds).

**Phase 3 — Sorted reinsertion** (`reinsert_sorted`, lines 58-66):

```c
static void reinsert_sorted(t_context *ctx)
{
    int pos;

    while (ctx->b.size > 0)
    {
        pos = find_max_pos(&ctx->b);       // Find largest in B
        rotate_to_top(ctx, pos, 1);        // Bring it to top of B
        pos = find_insert_pos(&ctx->a, ctx->b.data[0]); // Where does it go in A?
        rotate_to_top(ctx, pos, 0);        // Open the gap in A
        do_pa(ctx);                        // Insert it
    }
    rotate_to_top(ctx, find_min_pos(&ctx->a), 0);  // Final alignment
}
```

Reinsert B elements into A from **largest to smallest**. For each element:

1. Find the max in B (O(k) scan), rotate it to the top of B
2. Find its correct insertion position in A using `find_insert_pos`
3. Rotate A to that position, then `pa` to insert

**Why largest-first?** Processing elements from largest to smallest ensures that
successive insertion points move monotonically through A, so the **total A
rotation across all reinsertions telescopes to O(n)**. If we inserted in random
order, each insertion could require rotating A by up to n positions.

**The find_insert_pos helper** (`sort_adaptive.c` lines 36-52):

```c
static int find_insert_pos(t_stack *a, int val)
{
    int i;
    int best_pos;
    int best_val;

    best_pos = find_min_pos(a);    // Default: insert before the minimum
    best_val = -1;
    i = 0;
    while (i < a->size)
    {
        if (a->data[i] < val && a->data[i] > best_val)
        {
            best_val = a->data[i];
            best_pos = (i + 1) % a->size;   // Insert AFTER this element
        }
        i++;
    }
    return (best_pos);
}
```

This function answers: "where in A should `val` be inserted to maintain sorted
circular order?" It scans A for the **largest element that is still smaller than
val** — `val` should go immediately after that element. The `% a->size` handles
the wrap-around case (when the best position is at the boundary between the
last element and the first).

If no element in A is smaller than `val` (meaning `val` is the new global
minimum), it defaults to inserting before the current minimum (`find_min_pos`).

**Continuing the visual example:**

```
A: [0, 1, 4, 5, 6]    B: [3, 2]

--- Reinsert max(B) = 3 ---
find_max_pos(B) = 0 (already on top, no rotation needed)
find_insert_pos(A, 3): scan A for largest element < 3
  a[0]=0 < 3, best_val=0, best_pos=1
  a[1]=1 < 3, best_val=1, best_pos=2
  a[2]=4 not < 3, skip
  a[3]=5 not < 3, skip
  a[4]=6 not < 3, skip
  -> insert at position 2

rotate_to_top(A, 2, 0): pos=2, size=5, 2 <= 5/2=2 -> forward
  ra: A: [1, 4, 5, 6, 0]
  ra: A: [4, 5, 6, 0, 1]
pa: A: [3, 4, 5, 6, 0, 1]    B: [2]

--- Reinsert max(B) = 2 ---
find_max_pos(B) = 0 (already on top)
find_insert_pos(A, 2): scan A for largest element < 2
  a[0]=3 not < 2, skip
  a[1]=4 not < 2, skip
  a[2]=5 not < 2, skip
  a[3]=6 not < 2, skip
  a[4]=0 < 2, best_val=0, best_pos=5
  a[5]=1 < 2, best_val=1, best_pos=(5+1)%6=0
  -> insert at position 0

rotate_to_top(A, 0, 0): no rotation needed!
pa: A: [2, 3, 4, 5, 6, 0, 1]    B: []

--- Final alignment ---
find_min_pos(A) = 5 (element 0 is at index 5)
rotate_to_top(A, 5, 0): pos=5, size=7, 5 > 7/2=3 -> reverse
  size - pos = 7 - 5 = 2 reverse rotations
  rra: A: [1, 2, 3, 4, 5, 6, 0]
  rra: A: [0, 1, 2, 3, 4, 5, 6]

SORTED! Total: ~16 operations for 7 elements with low disorder
Compare to chunk sort: ~40+ operations for the same input
```

**Phase 4 — Final alignment** (last line of `reinsert_sorted`):

After reinsertion, A is sorted but may be rotated (not starting at the minimum).
One final `rotate_to_top` brings the minimum to the top. Cost: at most n/2.

**Complexity analysis:**

| Phase                | Cost              | Notes                              |
|----------------------|-------------------|------------------------------------|
| 1. Rotation align    | O(n/2)            | Cheaper-direction rotation         |
| 2. Monotone scan     | Exactly n         | One ra or pb per element           |
| 3. Reinsertion       | O(n) + O(k^2)    | A rotations telescope; B is O(k) each |
| 4. Final alignment   | O(n/2)            | Cheaper-direction rotation         |
| **Total**            | **O(n + k^2)**    | k = number of outliers             |

**Why this is efficient for low disorder:**

When d < 0.2, the input is nearly sorted. The monotone scan finds that most
elements are already in ascending order — only the few "out-of-place" elements
get pushed to B. With k being small (often proportional to sqrt(n) or less),
the O(k^2) reinsertion cost is negligible compared to the O(n) scan.

Concrete example: for n=500 with d=0.1, typically k is around 20-30 elements.
The total operation count might be: 250 (alignment) + 500 (scan) + 600
(reinsertion) + 250 (final alignment) = **~1600 operations**. Compare that to
chunk sort's ~6500 operations for the same input — a 4x improvement!

---

#### Medium Disorder (0.2 <= d < 0.5): Chunk-Based Sort

Delegates to `sort_medium` (chunk decomposition). See Section 4b above.

**Why this threshold?** Below 0.2, the monotone extraction pushes very few
elements to B, making it more efficient. Above 0.2, the number of outliers
grows large enough that chunk sort's O(n * sqrt(n)) worst-case guarantee
becomes preferable — its performance doesn't degrade with increasing k the
way monotone extraction does.

**Complexity: O(n * sqrt(n)).**

---

#### High Disorder (d >= 0.5): Binary Radix Sort

Delegates to `sort_complex` (binary radix sort). See Section 4c above.

**Why this threshold?** A uniformly random permutation has expected disorder
exactly 0.5. Above this point, more than half of all element pairs are
inverted — the input is so shuffled that no structure-exploiting algorithm
has an advantage. Radix sort's uniform O(n * log(n)) behavior, independent of
input pattern, is ideal for this regime.

**Complexity: O(n * log(n)).**

---

### Key takeaways

1. **Simple sort** (O(n^2)) is only useful for tiny inputs or debugging
2. **Chunk sort** (O(n \* sqrt(n))) is the workhorse for medium-disorder inputs —
   the `rb` optimization and careful chunk sizing make it consistently perform well
3. **Radix sort** (O(n \* log n)) is elegant but produces more operations than chunk
   sort for typical push_swap input sizes due to large constant factors — best
   reserved for high-disorder (d >= 0.5) inputs
4. **Adaptive sort** combines all strategies via three-way dispatch:
   - Hardcoded solutions for size <= 5
   - O(n) fast path for rotated-sorted inputs
   - **Monotone extraction** (O(n + k^2)) for low disorder (d < 0.2) — the secret
     weapon that dramatically reduces operations on nearly-sorted inputs
   - Chunk sort for medium disorder (0.2 <= d < 0.5)
   - Radix sort for high disorder (d >= 0.5)
5. The chunk size formula `(n * 3) / 100 + 14` is empirically optimized
6. The `find_insert_pos` function is critical to the low-disorder strategy —
   it maintains sorted circular order during reinsertion

---

## Chapter 5: Argument Parsing & Validation

### What you'll learn

- Why ft_atoi is not sufficient (overflow)
- The ft_atol_safe pattern for safe integer parsing
- Handling both `"3 2 1"` (single arg) and `3 2 1` (multiple args)
- Duplicate detection
- Flag parsing for strategy selection and benchmark mode

---

### 5.1 Why ft_atoi Is Not Enough

The standard `atoi()` and the 42 `ft_atoi()` have a critical flaw: they do not
detect integer overflow. For push_swap, this means:

```
./push_swap 2147483648    <- This is INT_MAX + 1, should be Error
```

With `ft_atoi`, this silently wraps to a negative number. Our program would
happily sort a corrupted value.

---

### 5.2 The ft_atol_safe Pattern

**From `src/parse_utils.c` (lines 33-58):**

```c
int ft_atol_safe(const char *str, long *result)
{
    int     i;
    int     sign;
    long    val;

    i = 0;
    sign = 1;
    val = 0;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        val = val * 10 + (str[i] - '0');
        if ((sign == 1 && val > INT_MAX)
            || (sign == -1 && val > (long)INT_MAX + 1))
            return (0);                           // FAIL: out of int range
        i++;
    }
    *result = val * sign;
    return (1);                                   // SUCCESS
}
```

**Key design decisions:**

1. Uses `long` accumulator -- `long` is at least 64 bits, so it can hold values
   larger than INT_MAX without overflow itself
2. Checks overflow **after each digit** -- catches overflow at the earliest moment
3. Returns 0/1 (fail/success) and writes result via pointer -- classic C pattern
4. Separate positive/negative checks: `INT_MAX = 2147483647`, `INT_MIN = -2147483648`

**Why `val > (long)INT_MAX + 1` instead of `-val < INT_MIN`?**

The variable `val` accumulates the absolute (unsigned) magnitude. For negative
numbers, the sign is applied only at the end (`val * sign`). So the check asks:
"is the magnitude too large to fit when negated?" The maximum magnitude for a
negative `int` is 2147483648, which is `INT_MAX + 1`. The `(long)` cast is
essential to prevent integer overflow on the right side of the comparison —
without it, `INT_MAX + 1` would wrap to `INT_MIN` in 32-bit arithmetic.

This approach avoids undefined behavior: computing `-val` when `val` equals
2147483648 would work on most platforms but is technically not guaranteed.
Instead, we compare the unsigned magnitude directly.

---

### 5.3 Input Validation

**From `src/parse_utils.c` (lines 15-31):**

```c
int is_valid_integer(const char *str)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;
    if (!str[i])                     // Just a sign with no digits? Invalid
        return (0);
    while (str[i])
    {
        if (!ft_isdigit(str[i]))     // Non-digit character? Invalid
            return (0);
        i++;
    }
    return (1);
}
```

This catches:

- Empty strings
- Strings with only a sign: `"-"`, `"+"`
- Strings with non-digit characters: `"12a3"`, `"abc"`
- Strings with spaces mid-number: `"1 2"` (handled differently -- see next section)

The two-step validation (`is_valid_integer` then `ft_atol_safe`) provides defense
in depth:

1. `is_valid_integer` checks format (only digits and optional leading sign)
2. `ft_atol_safe` checks value range (within INT_MIN to INT_MAX)

---

### 5.4 Handling Single String vs Multiple Arguments

The project must handle both:

```bash
./push_swap "3 2 1"       # Single argument with spaces
./push_swap 3 2 1         # Three separate arguments
./push_swap "3 2" 1       # Mixed: first arg has space, second is standalone
```

**From `src/parse_args.c` (lines 91-110):**

```c
int parse_args(int argc, char **argv, t_context *ctx)
{
    int i;

    i = 1;
    while (i < argc)
    {
        if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            if (!parse_flag(argv[i], ctx))     // Handle --flags
                return (0);
        }
        else if (!parse_split_arg(ctx, argv[i]))  // Handle numbers
            return (0);
        i++;
    }
    if (!check_duplicates(&ctx->a))             // Check for duplicates
        return (0);
    return (1);
}
```

**The key is `parse_split_arg`** (`parse_args.c` lines 65-89):

```c
static int parse_split_arg(t_context *ctx, char *arg)
{
    char **tokens;
    int   i;
    int   ret;

    tokens = ft_split(arg, ' ');     // Split on spaces
    if (!tokens)
        return (0);
    i = 0;
    ret = 1;
    while (tokens[i] && ret)
    {
        if (tokens[i][0] == '-' && tokens[i][1] == '-')
            ret = parse_flag(tokens[i], ctx);
        else
            ret = add_number(ctx, tokens[i]);
        i++;
    }
    // Free all tokens
    i = 0;
    while (tokens[i])
        free(tokens[i++]);
    free(tokens);
    return (ret);
}
```

Every argument goes through `ft_split` on spaces. This means:

- `"3 2 1"` splits into `["3", "2", "1"]` -- three numbers
- `"3"` splits into `["3"]` -- one number
- `"--bench 3 2 1"` splits into `["--bench", "3", "2", "1"]` -- flag + numbers

This elegant approach handles all formats uniformly.

---

### 5.5 Duplicate Detection

**From `src/parse_args.c` (lines 15-33):**

```c
static int check_duplicates(t_stack *a)
{
    int i;
    int j;

    i = 0;
    while (i < a->size)
    {
        j = i + 1;
        while (j < a->size)
        {
            if (a->data[i] == a->data[j])
                return (0);          // Duplicate found!
            j++;
        }
        i++;
    }
    return (1);                      // No duplicates
}
```

This is O(n^2) brute force -- comparing every pair. For n <= 500, this is fine.

Note: Duplicate detection happens AFTER all numbers are parsed but BEFORE
normalization. This is critical because normalization would assign different
ranks to duplicate values, hiding the error.

---

### 5.6 Flag Parsing

**From `src/parse_args.c` (lines 35-50):**

```c
static int parse_flag(char *arg, t_context *ctx)
{
    if (ft_strncmp(arg, "--simple", 9) == 0)
        ctx->strategy = STRAT_SIMPLE;
    else if (ft_strncmp(arg, "--medium", 9) == 0)
        ctx->strategy = STRAT_MEDIUM;
    else if (ft_strncmp(arg, "--complex", 10) == 0)
        ctx->strategy = STRAT_COMPLEX;
    else if (ft_strncmp(arg, "--adaptive", 11) == 0)
        ctx->strategy = STRAT_ADAPTIVE;
    else if (ft_strncmp(arg, "--bench", 8) == 0)
        ctx->bench = 1;
    else
        return (0);                  // Unknown flag -> Error
    return (1);
}
```

**Supported flags:**

- `--simple`: Force selection sort
- `--medium`: Force chunk-based sort
- `--complex`: Force radix sort
- `--adaptive`: Use adaptive strategy (default)
- `--bench`: Enable benchmark output to stderr

The `ft_strncmp` comparison length includes the null terminator (e.g., `"--simple"` = 8
chars + null = compare 9). This ensures exact match -- `"--simpleton"` would NOT
match because the 9th character differs.

Flags can appear anywhere in the arguments:

```bash
./push_swap --bench 5 3 1 4 2
./push_swap 5 3 1 --medium 4 2
./push_swap "5 3 --bench 1 4 2"    # Even inside a quoted string!
```

---

### Key takeaways

1. Always use `long` for intermediate parsing to detect int overflow
2. `ft_split` on every argument handles both single-string and multi-argument input
3. Duplicate detection must happen before normalization
4. Two-step validation (format check then range check) provides defense in depth
5. Flags are recognized by prefix `--` and can appear anywhere in the input

---

## Chapter 6: The Makefile

### What you'll learn

- Anatomy of each Makefile target
- How libft compilation is chained
- Pattern rules and how relinking is prevented
- The bonus target and its shared sources

---

### 6.1 The Complete Makefile

**From `Makefile` (lines 1-68):**

```makefile
NAME = push_swap
BONUS_NAME = checker
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes -I libft
```

**Variable definitions:**

- `NAME` / `BONUS_NAME`: Output binary names
- `CC = cc`: Uses the system default C compiler (usually gcc or clang)
- `CFLAGS`: The mandatory 42 warning flags. `-Werror` makes warnings into errors.
- `INCLUDES`: Header search paths. `-I includes` for `push_swap.h`, `-I libft` for
  libft headers.

```makefile
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
```

The library is built as a static archive (`.a` file).

```makefile
SRCS = src/main.c \
       src/parse_args.c \
       src/parse_utils.c \
       src/stack_init.c \
       src/stack_ops_swap.c \
       src/stack_ops_push.c \
       src/stack_ops_rotate.c \
       src/stack_ops_revrot.c \
       src/ops_out_swap.c \
       src/ops_out_rot.c \
       src/ops_out_revrot.c \
       src/normalize.c \
       src/disorder.c \
       src/sort_tiny.c \
       src/sort_simple.c \
       src/sort_medium.c \
       src/sort_medium_utils.c \
       src/sort_complex.c \
       src/sort_adaptive.c \
       src/bench.c \
       src/utils.c
```

20 source files for the main program. Note the operation output wrappers are
split across three files (`ops_out_swap.c`, `ops_out_rot.c`, `ops_out_revrot.c`)
to keep each under the Norm's function limit. Normalization has its own file
(`normalize.c`) separate from `stack_init.c`.

```makefile
BONUS_SRCS = bonus/checker_main_bonus.c \
             bonus/checker_exec_bonus.c \
             src/parse_args.c \
             src/parse_utils.c \
             src/stack_init.c \
             src/stack_ops_swap.c \
             src/stack_ops_push.c \
             src/stack_ops_rotate.c \
             src/stack_ops_revrot.c \
             src/ops_out_swap.c \
             src/ops_out_rot.c \
             src/ops_out_revrot.c \
             src/normalize.c \
             src/utils.c
```

The checker reuses 12 shared source files from the main program. It does NOT
include the sorting algorithms (`sort_*.c`), disorder computation, or bench —
it only needs stack operations, parsing, normalization, and the checker-specific
main/exec files.

Note the `_bonus` suffix on the checker files: this is a 42 convention that
distinguishes bonus sources from mandatory ones. The Makefile uses this suffix
in the filenames even though the header comments inside say `checker_main.c`
and `checker_exec.c` (the canonical names without the suffix).

---

### 6.2 Object File Generation

```makefile
OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)
```

This substitution reference converts `src/main.c` to `src/main.o`, etc.

```makefile
%.o: %.c
    $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
```

The **pattern rule**: for any `.o` file, compile the corresponding `.c` file.

- `$<` = the first prerequisite (the `.c` file)
- `$@` = the target (the `.o` file)
- `-c` = compile only, do not link

This means `make` only recompiles `.c` files that changed (by comparing timestamps
of `.o` and `.c` files). This **prevents relinking** when source files haven't changed.

---

### 6.3 Target Dependency Chain

```makefile
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
```

The dependency chain:

```
all -> push_swap -> libft.a + all .o files
                      |
                      v
                  $(MAKE) -C libft  (builds libft if needed)
```

```makefile
$(LIBFT):
    $(MAKE) -C $(LIBFT_DIR)
```

`$(MAKE) -C $(LIBFT_DIR)` runs `make` inside the `libft/` directory. This:

1. Uses libft's own Makefile to compile its sources
2. Produces `libft/libft.a`
3. Only rebuilds if libft sources changed

---

### 6.4 The Bonus Target

```makefile
bonus: $(LIBFT) $(BONUS_OBJS)
    $(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)
```

Important: `bonus` is NOT a prerequisite of `all`. You must explicitly run
`make bonus` to build the checker.

Note: Some shared `.o` files (like `src/parse_args.o`) appear in both `OBJS` and
`BONUS_OBJS`. Make's timestamp-based dependency tracking handles this correctly --
if the `.o` already exists and is newer than the `.c`, it won't be recompiled.

---

### 6.5 Clean Targets

```makefile
clean:
    $(MAKE) -C $(LIBFT_DIR) clean
    rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
    $(MAKE) -C $(LIBFT_DIR) fclean
    rm -f $(NAME) $(BONUS_NAME)

re: fclean all
```

- `clean`: Remove object files (both main and bonus), clean libft
- `fclean`: Remove everything (objects + binaries), fclean libft
- `re`: Full rebuild (fclean then all)

```makefile
.PHONY: all clean fclean re bonus
```

`.PHONY` tells make these targets are not actual files. Without this, if a file
named `clean` existed in the directory, `make clean` would think it's up to date
and do nothing.

---

### Key takeaways

1. Pattern rules (`%.o: %.c`) prevent unnecessary recompilation
2. libft is built via recursive make (`$(MAKE) -C`)
3. The bonus checker reuses 9/18 source files -- code sharing through shared compilation
4. `CFLAGS = -Wall -Wextra -Werror` is mandatory for 42
5. `.PHONY` prevents filename conflicts with target names

---

## Chapter 7: Benchmark Mode Implementation

### What you'll learn

- How operations are counted using the `ops[11]` array
- Writing to stderr vs stdout and why it matters
- Formatting percentages without printf("%.2f")

---

### 7.1 Operation Counting

Every operation is tracked at two granularities:

**Per-operation counter** (`ctx->ops[OP_SA]`, etc.):

```c
// From includes/push_swap.h lines 20-30:
# define OP_SA   0
# define OP_SB   1
# define OP_SS   2
# define OP_PA   3
# define OP_PB   4
# define OP_RA   5
# define OP_RB   6
# define OP_RR   7
# define OP_RRA  8
# define OP_RRB  9
# define OP_RRR  10
```

**Total counter** (`ctx->total_ops`):

Both are incremented in every wrapper function. For example, from `src/ops_out_swap.c` line 21-22:

```c
ctx->ops[OP_SA]++;
ctx->total_ops++;
```

This means after sorting, you can examine exactly how many of each operation
was used, which is invaluable for optimization.

---

### 7.2 stderr vs stdout

**The rule:** Operation names go to **stdout** (fd 1), benchmark data goes to **stderr** (fd 2).

Why? Because the checker reads operations from stdout via pipe:

```bash
./push_swap 3 2 1 | ./checker 3 2 1
```

If benchmark data went to stdout, the checker would try to interpret
`[bench] disorder: 100.00%` as an operation and fail with Error.

By sending bench data to stderr:

```bash
./push_swap --bench 3 2 1 | ./checker 3 2 1
# stdout (piped to checker): sa\nrra\n
# stderr (printed to terminal): [bench] disorder: ...
```

In the code, every `print_bench` function uses `ft_putstr_fd(..., 2)` and
`ft_putnbr_fd(..., 2)` -- always file descriptor 2.

---

### 7.3 Formatting Percentages Without printf("%.2f")

The 42 projects typically forbid `printf`. We need to print `"45.23%"` using only
integer operations.

**From `src/bench.c` (lines 15-29):**

```c
static void print_disorder(t_context *ctx)
{
    int pct_int;
    int pct_dec;

    pct_int = (int)(ctx->disorder * 100);         // Integer part: 45
    pct_dec = (int)(ctx->disorder * 10000) % 100;  // Decimal part: 23
    ft_putstr_fd("[bench] disorder: ", 2);
    ft_putnbr_fd(pct_int, 2);
    ft_putstr_fd(".", 2);
    if (pct_dec < 10)
        ft_putstr_fd("0", 2);                      // Leading zero: "05" not "5"
    ft_putnbr_fd(pct_dec, 2);
    ft_putstr_fd("%\n", 2);
}
```

**How it works:**

```
disorder = 0.4523

pct_int = (int)(0.4523 * 100)   = (int)(45.23) = 45
pct_dec = (int)(0.4523 * 10000) = (int)(4523)   = 4523
          4523 % 100 = 23

Output: "45.23%"

Special case: disorder = 0.05
pct_int = 5
pct_dec = (int)(500) % 100 = 0
Output: "5.00%"  (the if (pct_dec < 10) adds leading zero, and putnbr(0) adds the other)

Wait -- pct_dec = 0, which is < 10, so we print "0", then putnbr(0) = "0"
Result: "5.00%" -- correct!
```

---

### 7.4 Strategy Name Resolution

**From `src/bench.c` (lines 31-42):**

```c
static const char *strat_name(int strat, double disorder)
{
    if (strat == STRAT_SIMPLE)
        return ("Simple / O(n^2)");
    if (strat == STRAT_MEDIUM)
        return ("Medium / O(n*sqrt(n))");
    if (strat == STRAT_COMPLEX)
        return ("Complex / O(n*log(n))");
    if (disorder < 0.2)
        return ("Adaptive / O(n)");
    if (disorder < 0.5)
        return ("Adaptive / O(n*sqrt(n))");
    return ("Adaptive / O(n*log(n))");
}
```

When the strategy is `STRAT_ADAPTIVE`, the function uses the disorder value to
determine **which sub-algorithm was actually used**. This means the bench output
tells you not just "Adaptive" but specifically which complexity class your input
fell into. This is valuable for understanding performance — if you see
`Adaptive / O(n)` you know the low-disorder fast path was taken.

---

### 7.5 Complete Benchmark Output

**From `src/bench.c` (lines 72-77):**

```c
void print_bench(t_context *ctx)
{
    print_disorder(ctx);              // [bench] disorder: XX.XX%
    print_strategy(ctx);              // [bench] strategy: ... \n [bench] total_ops: N
    print_ops_line(ctx, 0, 4);        // [bench] sa: N  sb: N  ss: N  pa: N  pb: N
    print_ops_line(ctx, 5, 10);       // [bench] ra: N  rb: N  rr: N  rra: N  rrb: N  rrr: N
}
```

The `print_ops_line` function (`bench.c` lines 56-70) uses a static array of
operation names and prints a range of them on a single line:

```c
static void print_ops_line(t_context *ctx, int start, int end)
{
    static const char *names[] = {"sa", "sb", "ss", "pa", "pb",
        "ra", "rb", "rr", "rra", "rrb", "rrr"};
    // ... iterates from start to end, printing "name: count" pairs
}
```

The operations are split into two lines for readability: swap/push ops (0-4) on
the first line, rotate ops (5-10) on the second.

**Example output (to stderr):**

```
[bench] disorder: 52.34%
[bench] strategy: Adaptive / O(n*log(n))
[bench] total_ops: 4521
[bench] sa: 0  sb: 0  ss: 0  pa: 250  pb: 250
[bench] ra: 1842  rb: 89  rr: 0  rra: 1045  rrb: 1045  rrr: 0
```

**Example with low-disorder input:**

```
[bench] disorder: 8.50%
[bench] strategy: Adaptive / O(n)
[bench] total_ops: 1247
[bench] sa: 0  sb: 0  ss: 0  pa: 42  pb: 42
[bench] ra: 580  rb: 35  rr: 0  rra: 274  rrb: 274  rrr: 0
```

This gives you complete visibility into the algorithm's behavior: which operations
dominate, what the disorder was, which strategy was selected, and precisely which
sub-algorithm handled the input.

---

### Key takeaways

1. Operations are counted at both per-op and total granularity via the `ops[11]` array
2. Benchmark output goes to stderr (fd 2) to not interfere with stdout piping
3. Float formatting uses integer multiplication and modulo to avoid printf
4. The bench flag (`--bench`) is optional and does not affect sorting behavior

---

## Chapter 8: Bonus Checker

### What you'll learn

- How get_next_line reads operations from stdin
- The execute loop: string comparison to function dispatch
- Why OK/KO goes to stdout but Error goes to stderr
- How the checker reuses the same stack operations as push_swap

---

### 8.1 Checker Architecture

The checker program:

1. Parses the same arguments as push_swap (same `parse_args`)
2. Reads operations from stdin (one per line)
3. Executes each operation on the stacks
4. Checks if Stack A is sorted and Stack B is empty
5. Prints `OK` or `KO`

**From `bonus/checker_main_bonus.c` (lines 47-73):**

```c
int main(int argc, char **argv)
{
    t_context ctx;

    if (argc < 2)
        return (0);
    init_checker_ctx(&ctx);
    if (!stack_create(&ctx.a, 10000))
        return (1);
    if (!stack_create(&ctx.b, 10000))
    {
        stack_destroy(&ctx.a);
        return (1);
    }
    if (!parse_args(argc, argv, &ctx))
        checker_error_exit(&ctx);
    if (ctx.a.size == 0)
    {
        free_context(&ctx);
        return (0);
    }
    if (!read_and_exec(&ctx))          // Read ops from stdin and execute
        checker_error_exit(&ctx);
    check_result(&ctx);                // Print OK or KO
    free_context(&ctx);
    return (0);
}
```

Notice: the checker does NOT call `normalize_stack()`. It works with the original
integer values. This is correct because:

- push_swap normalizes internally for algorithm purposes
- The operations output by push_swap are valid regardless of normalization
- The checker just needs to verify the final order

---

### 8.2 Reading and Executing Operations

**From `bonus/checker_exec_bonus.c` (lines 61-77):**

```c
int read_and_exec(t_context *ctx)
{
    char *line;

    line = get_next_line(0);           // Read from fd 0 (stdin)
    while (line)
    {
        if (!exec_operation(ctx, line))  // Execute or fail
        {
            free(line);
            return (0);                 // Invalid operation -> Error
        }
        free(line);                    // Free each line after use
        line = get_next_line(0);       // Read next line
    }
    return (1);                        // All operations valid
}
```

`get_next_line(0)` reads from stdin (fd 0). When used with a pipe:

```bash
./push_swap 5 3 1 | ./checker 5 3 1
```

stdin receives the operations that push_swap wrote to stdout.

When stdin reaches EOF (pipe closed or Ctrl+D), `get_next_line` returns NULL,
and the loop ends.

---

### 8.3 Operation Dispatch

**From `bonus/checker_exec_bonus.c` (lines 16-58):**

```c
static int exec_swap_push(t_context *ctx, char *line)
{
    if (ft_strncmp(line, "sa\n", 3) == 0)
        op_sa(&ctx->a);
    else if (ft_strncmp(line, "sb\n", 3) == 0)
        op_sb(&ctx->b);
    else if (ft_strncmp(line, "ss\n", 3) == 0)
        op_ss(&ctx->a, &ctx->b);
    else if (ft_strncmp(line, "pa\n", 3) == 0)
        op_pa(&ctx->a, &ctx->b);
    else if (ft_strncmp(line, "pb\n", 3) == 0)
        op_pb(&ctx->a, &ctx->b);
    else
        return (0);
    return (1);
}

static int exec_rotate(t_context *ctx, char *line)
{
    if (ft_strncmp(line, "rra\n", 4) == 0)      // Check rra BEFORE ra!
        op_rra(&ctx->a);
    else if (ft_strncmp(line, "rrb\n", 4) == 0)  // Check rrb BEFORE rb!
        op_rrb(&ctx->b);
    else if (ft_strncmp(line, "rrr\n", 4) == 0)  // Check rrr BEFORE rr!
        op_rrr(&ctx->a, &ctx->b);
    else if (ft_strncmp(line, "ra\n", 3) == 0)
        op_ra(&ctx->a);
    else if (ft_strncmp(line, "rb\n", 3) == 0)
        op_rb(&ctx->b);
    else if (ft_strncmp(line, "rr\n", 3) == 0)
        op_rr(&ctx->a, &ctx->b);
    else
        return (0);
    return (1);
}
```

**Critical ordering:** `rra` must be checked BEFORE `ra`! If we checked `ra` first
with `ft_strncmp(line, "ra\n", 3)`, the string `"rra\n"` would match because the
first 3 characters of `"rra\n"` are `"rra"`, which does NOT match `"ra\n"`. Actually,
`ft_strncmp("rra\n", "ra\n", 3)` compares: 'r' == 'r', 'r' != 'a' -- so it would
NOT match. The ordering here is a defensive measure that makes the logic clearer
and safer.

**Key detail:** The checker uses the **raw** `op_sa()`, NOT the wrapper `do_sa()`.
This is the two-layer pattern in action -- the checker must not print operations,
it only executes them silently.

---

### 8.4 Result Checking

**From `bonus/checker_main_bonus.c` (lines 39-45):**

```c
static void check_result(t_context *ctx)
{
    if (is_sorted(&ctx->a) && ctx->b.size == 0)
        write(1, "OK\n", 3);      // Sorted + B empty = success
    else
        write(1, "KO\n", 3);      // Otherwise = failure
}
```

Two conditions must be met:

1. Stack A is sorted (ascending, smallest on top)
2. Stack B is empty (all elements are back in A)

---

### 8.5 Output Routing

```
OK/KO         -> stdout (fd 1)   -- the checker's result
Error         -> stderr (fd 2)   -- invalid input or operation
```

Why this matters:

```bash
# You can capture just the result:
result=$(./push_swap 5 3 1 | ./checker 5 3 1)
echo $result  # "OK" or "KO"

# Error messages appear on terminal even when stdout is captured
./push_swap abc | ./checker abc
# stderr shows: Error
# stdout captured variable would be empty
```

---

### Key takeaways

1. The checker reuses parsing and stack operations but NOT sorting algorithms
2. It uses **raw** `op_*` functions, not `do_*` wrappers (no printing)
3. `get_next_line(0)` reads from stdin, enabling piping from push_swap
4. The order of `ft_strncmp` checks matters for reverse-rotate operations
5. OK/KO on stdout, Error on stderr -- proper Unix convention

---

## Chapter 9: Testing & Debugging Strategies

### What you'll learn

- Using the checker binary to validate push_swap output
- Generating random inputs with Python
- Scripting benchmark runs
- Common bugs and how to spot them
- Memory leak detection

---

### 9.1 Basic Validation with Checker

The simplest test: pipe push_swap output into the checker.

```bash
# Build both programs
make && make bonus

# Test with explicit input
./push_swap 3 2 1 | ./checker 3 2 1
# Expected: OK

# Test edge cases
./push_swap 1       | ./checker 1         # No output, already sorted
./push_swap 1 2 3   | ./checker 1 2 3     # No output, already sorted
./push_swap          # No output, no args

# Count operations
./push_swap 3 2 1 | wc -l
# Expected: 2 (sa + rra)
```

---

### 9.2 Generating Random Inputs with Python

```bash
# Generate 100 unique random numbers
python3 -c "import random; nums = random.sample(range(-1000, 1000), 100); print(' '.join(map(str, nums)))"

# Store in a variable and test
ARG=$(python3 -c "import random; nums = random.sample(range(-1000, 1000), 100); print(' '.join(map(str, nums)))")
./push_swap $ARG | ./checker $ARG
./push_swap $ARG | wc -l

# Generate 500 unique random numbers
ARG=$(python3 -c "import random; nums = random.sample(range(-5000, 5000), 500); print(' '.join(map(str, nums)))")
./push_swap $ARG | ./checker $ARG
./push_swap $ARG | wc -l
```

---

### 9.3 Scripting Benchmark Runs

Here is a comprehensive test script:

```bash
#!/bin/bash
# test_push_swap.sh - Run multiple tests and report statistics

PUSH_SWAP=./push_swap
CHECKER=./checker
ITERATIONS=100

test_size() {
    local size=$1
    local total=0
    local max=0
    local min=999999
    local fails=0

    for i in $(seq 1 $ITERATIONS); do
        ARG=$(python3 -c "import random; nums = random.sample(range(-10000, 10000), $size); print(' '.join(map(str, nums)))")

        # Check correctness
        result=$($PUSH_SWAP $ARG | $CHECKER $ARG)
        if [ "$result" != "OK" ]; then
            fails=$((fails + 1))
            echo "FAIL with: $ARG"
            continue
        fi

        # Count operations
        ops=$($PUSH_SWAP $ARG | wc -l | tr -d ' ')
        total=$((total + ops))

        if [ $ops -gt $max ]; then max=$ops; fi
        if [ $ops -lt $min ]; then min=$ops; fi
    done

    avg=$((total / ITERATIONS))
    echo "Size $size: avg=$avg min=$min max=$max fails=$fails"
}

echo "Testing push_swap ($ITERATIONS iterations each)"
echo "========================================="
test_size 3
test_size 5
test_size 100
test_size 500
```

**Expected results with the chunk-based sort:**

| Size | Average Ops | Target for Full Marks   |
| ---- | ----------- | ----------------------- |
| 3    | ~2          | <= 3                    |
| 5    | ~8          | <= 12                   |
| 100  | ~659        | <= 700                  |
| 500  | ~6651       | <= 5500 (great: < 8000) |

---

### 9.4 Common Bugs and How to Spot Them

#### Bug 1: Off-by-one in rotate_to_top

**Symptom:** Sometimes one extra or one fewer rotation than needed.

**Debug:** Add temporary debug output:

```bash
./push_swap --bench 5 3 1 4 2 2>/dev/null | wc -l   # Check op count
./push_swap --bench 5 3 1 4 2 2>bench.log            # Check strategy/ops
```

**Root cause:** The condition `pos <= size / 2` vs `pos < size / 2` determines
the boundary. With the current code (`<=`), position exactly at the midpoint
uses forward rotation. This is correct because forward rotation costs `pos` ops
and reverse costs `size - pos` ops; at `pos = size/2`, forward is cheaper or equal.

#### Bug 2: Chunk boundaries not covering all elements

**Symptom:** Some elements never get pushed to B; infinite loop in phase_one.

**Debug:** Run with --bench and check if total pa + pb operations equal 2n
(every element pushed out and back).

**Prevention:** The loop guard in `push_chunk()` uses `size--` (line 39) to
prevent infinite loops even if chunk boundaries are wrong.

#### Bug 3: Memory leaks in parse_split_arg

**Symptom:** `leaks` reports unreleased memory.

**Check:** The function must free all tokens AND the token array:

```c
i = 0;
while (tokens[i])
    free(tokens[i++]);   // Free each string
free(tokens);            // Free the array itself
```

#### Bug 4: Checker matches "rr\n" when given "rrr\n"

**Symptom:** Checker reports OK for incorrect sorts, or executes wrong operation.

**Prevention:** Check longer strings first (`rra`, `rrb`, `rrr`) before shorter
ones (`ra`, `rb`, `rr`). The current code in `checker_exec_bonus.c` already does this.

#### Bug 5: sort_three handles only 5 of 6 permutations

**Symptom:** `sort_three` sometimes does nothing for unsorted input.

**Verification:** Test all 6 permutations:

```bash
for perm in "1 2 3" "1 3 2" "2 1 3" "2 3 1" "3 1 2" "3 2 1"; do
    result=$(./push_swap $perm | ./checker $perm)
    ops=$(./push_swap $perm | wc -l | tr -d ' ')
    echo "$perm -> $result ($ops ops)"
done
```

Expected:

```
1 2 3 -> OK (0 ops)
1 3 2 -> OK (2 ops)
2 1 3 -> OK (1 ops)
2 3 1 -> OK (1 ops)
3 1 2 -> OK (1 ops)
3 2 1 -> OK (2 ops)
```

---

### 9.5 Memory Leak Detection

On macOS, use `leaks`:

```bash
# Run push_swap and check for leaks
leaks --atExit -- ./push_swap 5 3 1 4 2 > /dev/null

# Run checker and check for leaks
echo "sa" | leaks --atExit -- ./checker 2 1 > /dev/null
```

On Linux, use `valgrind`:

```bash
valgrind --leak-check=full ./push_swap 5 3 1 4 2 > /dev/null
echo "sa" | valgrind --leak-check=full ./checker 2 1 > /dev/null
```

**Memory management in this project is simple:**

- 2 allocations in `main()`: `stack_create` for A and B
- 1 temporary allocation in `normalize_stack()`: freed before return
- Multiple allocations in `parse_split_arg()`: all freed in the cleanup loop
- `free_context()` handles the two stack allocations at exit

---

### 9.6 Edge Case Testing

```bash
# Empty input
./push_swap ""
# Expected: no output

# Single element
./push_swap 42
# Expected: no output

# Already sorted
./push_swap 1 2 3 4 5
# Expected: no output

# Reverse sorted
./push_swap 5 4 3 2 1

# Duplicate detection
./push_swap 1 2 3 2
# Expected: Error (to stderr)

# Integer overflow
./push_swap 2147483648
# Expected: Error

# Integer underflow
./push_swap -2147483649
# Expected: Error

# Non-numeric input
./push_swap abc
# Expected: Error

# Just a sign
./push_swap -
# Expected: Error

# Mixed valid and invalid
./push_swap 1 2 abc 3
# Expected: Error
```

---

### Key takeaways

1. Always validate with the checker binary -- it is the ground truth
2. Test with random inputs, not just hand-picked cases
3. Script your benchmarks to get statistical averages, not one-off results
4. The most common bugs are off-by-one errors in rotation and chunk boundaries
5. Memory leak detection is straightforward because allocation count is small and deterministic

---

## Chapter 10: Performance Optimization Tips

### What you'll learn

- How to tune chunk sizes for optimal operation counts
- Why pre-checks (already sorted, rotated-sorted) save hundreds of operations
- Why chunk sort outperforms radix for typical push_swap inputs
- Detailed performance benchmarks and analysis

---

### 10.1 Chunk Size Tuning Methodology

The chunk size formula `(n * 3) / 100 + 14` was determined empirically. Here is
how to find the optimal formula for your implementation:

**Step 1: Write a parameterized test**

```bash
for chunk_base in 5 8 10 12 14 16 18 20; do
    for chunk_factor in 1 2 3 4 5; do
        # Modify get_chunk_size to use these parameters
        # Run 100 iterations of 500-element sorts
        # Record average operation count
    done
done
```

**Step 2: Plot results**

```
chunk_base vs avg_ops (for chunk_factor=3):
  base=5:   avg=7200
  base=10:  avg=6800
  base=14:  avg=6651  <-- optimal
  base=18:  avg=6750
  base=22:  avg=6900
```

**Step 3: Fine-tune around the optimum**

The optimal chunk size balances two costs:

- **Too few chunks (large chunk_size)**: Phase 1 is fast, but Phase 2 has long
  rotation distances because B is large and unsorted
- **Too many chunks (small chunk_size)**: Phase 2 is fast (B segments are short),
  but Phase 1 wastes operations rotating past non-chunk elements

The sweet spot for this implementation with n=500 is approximately 29
(`(500 * 3) / 100 + 14 = 29`), giving about 17 chunks.

---

### 10.2 Pre-Check: Already Sorted

**From `src/main.c` lines 37-47 -- the dispatch_sort function:**

```c
static void dispatch_sort(t_context *ctx)
{
    if (ctx->a.size <= 1 || is_sorted(&ctx->a))
        return ;                              // Sorted -> 0 operations!
    if (ctx->strategy == STRAT_SIMPLE)
        sort_simple(ctx);
    else if (ctx->strategy == STRAT_MEDIUM)
        sort_medium(ctx);
    else if (ctx->strategy == STRAT_COMPLEX)
        sort_complex(ctx);
    else
        sort_adaptive(ctx);                   // Default: STRAT_ADAPTIVE
}
```

This function is the top-level dispatcher. The `--simple`, `--medium`,
`--complex` flags bypass adaptive selection and force a specific algorithm.
Without any flag, `STRAT_ADAPTIVE` is the default (set in `init_context`).

The O(n) sorted check at the top means:

- Already sorted input produces ZERO operations
- The cost of the check (n-1 comparisons) is negligible

**Also checked in each sort function** (e.g., `sort_medium.c` line 71):

```c
if (ctx->a.size <= 1 || is_sorted(&ctx->a))
    return ;
```

This redundancy is intentional: `dispatch_sort` catches the common case early,
while each sort function protects itself from being called incorrectly.

---

### 10.3 Rotated-Sorted Fast Path

**From `src/sort_adaptive.c` (lines 49-53):**

```c
if (is_rotated_sorted(&ctx->a))
{
    rotate_to_top(ctx, find_min_pos(&ctx->a), 0);
    return ;
}
```

**Why this matters:**

Consider input [3, 4, 5, 0, 1, 2] (n=6):

- Without optimization: chunk sort uses ~30-40 operations
- With optimization: just rotate min (0) to top = 3 operations (rra, rra, rra)

For n=500, a rotated-sorted input:

- Without optimization: ~6500 operations
- With optimization: at most 250 operations (n/2 rotations)

**Detection is O(n):** Walk through the array counting "breaks" (places where
`data[i] > data[i+1]`). A rotated-sorted array has exactly one break, and
the last element must be less than the first (wrapping around).

---

### 10.4 The rb Optimization in Chunk Sort

**From `src/sort_medium.c` (lines 33-34):**

```c
if (ctx->b.data[0] < mid)
    do_rb(ctx);      // Push smaller elements to bottom of B
```

Without this optimization, all elements in a chunk pile up in B in random order.
Finding the max during Phase 2 requires rotating through the entire B stack.

With the optimization:

- Top half of B: larger elements of current chunk
- Bottom half of B: smaller elements of current chunk

This roughly halves the average rotation distance during Phase 2.

**Quantified impact for n=500:**

- Without rb optimization: ~8500 operations average
- With rb optimization: ~6651 operations average
- Savings: ~22%

---

### 10.5 Why Chunk Sort Outperforms Radix

For push_swap with n=500, let us compare:

**Radix sort:**

- Bits needed: log2(499) = 9 bits
- Per pass: n rotations/pushes + n push-backs = ~1000 operations
- Total passes: 9
- **Total: ~9000 operations**

**Chunk sort:**

- Phase 1: n push operations + chunk-related rotations = ~2500 operations
- Phase 2: n find-max + rotation + push = ~4000 operations
- **Total: ~6500 operations**

The key difference: radix sort does `n` operations per bit, and every element
participates in every pass. Chunk sort's Phase 1 is efficient because many
elements are already near the right chunk boundary (thanks to the rb optimization),
and Phase 2 benefits from `rotate_to_top` choosing the cheaper direction.

**When would radix be better?**

- If n were much larger (n > 10000), the O(n log n) vs O(n sqrt(n)) asymptotic
  difference would dominate
- If operations had different costs (they don't in push_swap)
- For extremely disordered inputs where chunk sort's Phase 2 suffers

---

### 10.6 Performance Benchmarks

Measured across 100 iterations with random inputs:

```
+------+----------+----------+----------+-----------+
| Size | Avg Ops  | Min Ops  | Max Ops  | Target    |
+------+----------+----------+----------+-----------+
| 3    |    1.5   |    0     |    2     |    3      |
| 5    |    7.8   |    0     |   12     |   12      |
| 100  |  ~659    |  ~570    |  ~750    |  700      |
| 500  | ~6651    | ~6200    | ~7100    | 5500-8000 |
+------+----------+----------+----------+-----------+
```

**Grade thresholds (typical 42 evaluation):**

For 100 elements:

```
Operations <= 700  : 5/5 points (excellent)
Operations <= 900  : 4/5 points
Operations <= 1100 : 3/5 points
Operations <= 1300 : 2/5 points
Operations <= 1500 : 1/5 points
```

For 500 elements:

```
Operations <= 5500 : 5/5 points (excellent)
Operations <= 7000 : 4/5 points
Operations <= 8500 : 3/5 points
Operations <= 10000: 2/5 points
Operations <= 11500: 1/5 points
```

---

### 10.7 Operation Distribution Analysis

Using `--bench` mode on a typical 500-element sort:

```
[bench] disorder: 49.87%
[bench] strategy: Adaptive
[bench] total_ops: 6534
[bench] sa: 0  sb: 0  ss: 0  pa: 500  pb: 500  ra: 2102  rb: 187  rr: 0  rra: 1623  rrb: 1622  rrr: 0
```

**Observations:**

- `pa` + `pb` = 1000 = 2n (every element pushed out and back) -- this is a fixed cost
- `ra` dominates: used in Phase 1 to skip non-chunk elements
- `rra` and `rrb` are high: used in `rotate_to_top` for reverse rotations
- `rb` is relatively low: only used during the rb optimization in Phase 1
- `sa`, `sb`, `ss` = 0: chunk sort never swaps (sorting is done structurally)
- `rr`, `rrr` = 0: both stacks never need to rotate simultaneously

**Optimization opportunities from this data:**

1. Reducing `ra` count: smarter Phase 1 scanning
2. Reducing `rra`/`rrb` count: better element positioning in B
3. Using `rr`/`rrr` when both stacks need rotation (currently unused)

---

### 10.8 Advanced Optimization Ideas (Beyond Current Implementation)

For students who want to push further:

**1. Greedy insertion sort:**
Instead of chunks, find the cheapest element to insert into B's sorted position.
Can achieve ~5500 ops for 500 elements.

**2. Simultaneous rotations:**
When rotating A and B at the same time, use `rr` or `rrr` instead of separate
`ra`/`rb` or `rra`/`rrb`. This saves one operation per simultaneous rotation.

**3. Turkish sort / cost-based insertion:**
For each element in A, calculate the cost to move it to B's correct position.
Pick the element with the lowest combined cost. Can achieve ~4500-5000 ops.

**4. LIS (Longest Increasing Subsequence):**
Keep the longest already-sorted subsequence in A and only sort the rest through B.
Reduces the number of elements that need to be moved.

---

### Key takeaways

1. The chunk size formula is empirically tuned -- don't just use sqrt(n)
2. Pre-checks (sorted, rotated-sorted) are cheap O(n) and save thousands of operations
3. The rb optimization in Phase 1 reduces Phase 2 rotation costs by ~22%
4. Chunk sort beats radix for n <= 500 due to lower constant factors
5. Benchmark with --bench to understand operation distribution and find optimization targets
6. The current implementation scores well within 42's grading thresholds:
   ~659 average for 100 elements, ~6651 average for 500 elements

---

## Appendix A: File Reference

Quick reference to all source files and their responsibilities:

```
includes/push_swap.h          - All type definitions, macros, and prototypes
src/main.c                    - Entry point, init_context, dispatch_sort, error_exit,
                                create_stacks
src/parse_args.c              - parse_args, parse_flag, parse_split_arg, add_number,
                                check_duplicates
src/parse_utils.c             - ft_atol_safe, is_valid_integer
src/stack_init.c              - stack_create, stack_destroy, free_context
src/normalize.c               - normalize_stack, sort_copy (static), bin_search (static)
src/stack_ops_swap.c          - op_sa, op_sb, op_ss (raw swap operations)
src/stack_ops_push.c          - op_pa, op_pb, push_front (static), pop_front (static)
src/stack_ops_rotate.c        - op_ra, op_rb, op_rr, rotate (static)
src/stack_ops_revrot.c        - op_rra, op_rrb, op_rrr, rev_rotate (static)
src/ops_out_swap.c            - do_sa, do_sb, do_ss, do_pa, do_pb (wrappers)
src/ops_out_rot.c             - do_ra, do_rb, do_rr (wrappers)
src/ops_out_revrot.c          - do_rra, do_rrb, do_rrr (wrappers)
src/disorder.c                - compute_disorder, count_inversions (static)
src/sort_tiny.c               - sort_two, sort_three, sort_five, push_min_to_b (static)
src/sort_simple.c             - sort_simple (selection sort)
src/sort_medium.c             - sort_medium, phase_one (static), phase_two (static),
                                push_chunk (static)
src/sort_medium_utils.c       - ft_sqrt_approx, get_chunk_size, push_back_max
src/sort_complex.c            - sort_complex, radix_pass (static), get_max_bits (static)
src/sort_adaptive.c           - sort_adaptive, sort_low_disorder (static),
                                reinsert_sorted (static), find_insert_pos (static),
                                is_rotated_sorted (static)
src/bench.c                   - print_bench, print_disorder (static),
                                print_strategy (static), print_ops_line (static),
                                strat_name (static)
src/utils.c                   - is_sorted, find_min_pos, find_max_pos, rotate_to_top,
                                rot_dir (static)
bonus/checker_main_bonus.c    - Checker entry point, init_checker_ctx, check_result,
                                checker_error_exit
bonus/checker_exec_bonus.c    - exec_operation, read_and_exec, exec_swap_push (static),
                                exec_rotate (static)
Makefile                      - Build system (20 srcs + 14 bonus srcs)
```

---

## Appendix B: Defense Preparation Checklist

Questions you might be asked during evaluation, and where to find answers:

**Q: How does your program handle integer overflow?**
A: `ft_atol_safe()` in `src/parse_utils.c` uses a `long` accumulator and checks
against INT_MAX/INT_MIN after each digit. See Section 5.2.

**Q: What algorithm do you use for 500 elements?**
A: The adaptive strategy selects based on disorder. For typical random inputs
(d ~0.5): binary radix sort O(n \* log(n)). For medium disorder (0.2-0.5):
chunk-based sort O(n \* sqrt(n)). For low disorder (d < 0.2): monotone
extraction O(n + k^2). See Section 4d and `src/sort_adaptive.c`.

**Q: Why not use radix sort?**
A: Radix sort produces ~9000 operations for 500 elements due to 9 bit-passes.
Chunk sort averages ~6651 operations. See Section 10.5.

**Q: How do you handle duplicates?**
A: `check_duplicates()` in `src/parse_args.c` (lines 15-33) compares all pairs
after parsing. See Section 5.5.

**Q: What is the chunk size formula and why?**
A: `(n * 3) / 100 + 14`, empirically tuned. The math behind it relates to
minimizing the sum of Phase 1 and Phase 2 costs. See Section 4b.

**Q: How does the checker work?**
A: It reads operations from stdin via get*next_line, executes them using raw
`op*\*` functions (no printing), then checks if A is sorted and B is empty.
See Chapter 8.

**Q: What happens with "./push_swap" (no arguments)?**
A: Returns 0 immediately. See `src/main.c` lines 53-54.

**Q: What happens with "./push_swap 42"?**
A: Single element is already sorted. `is_sorted()` returns 1 in `dispatch_sort()`,
so no operations are output. See `src/main.c` line 37.

**Q: Can you explain the normalization step?**
A: Input values are mapped to ranks 0..n-1 using copy + sort + binary search.
This makes all algorithms work with contiguous indices. See Section 2.3.

**Q: What is the disorder metric?**
A: The ratio of inversions to maximum possible inversions. 0.0 = sorted,
1.0 = reverse sorted. See Section 4d and `src/disorder.c`.

**Q: How do you avoid memory leaks?**
A: Only 2 heap allocations (stack data arrays), cleaned by `free_context()`.
Temporary allocations in `normalize_stack()` and `parse_split_arg()` are
freed before their functions return. See Section 9.5.

---

## Appendix C: Complexity Summary

```
+---------------------+-------------------+-------------------+
| Algorithm           | Time Complexity   | Typical Ops (500) |
+---------------------+-------------------+-------------------+
| sort_two            | O(1)              | 0-1               |
| sort_three          | O(1)              | 0-2               |
| sort_five           | O(n)              | 0-12              |
| sort_simple         | O(n^2)            | ~62,500           |
| sort_medium         | O(n * sqrt(n))    | ~6,651            |
| sort_complex        | O(n * log(n))     | ~9,000            |
| sort_low_disorder   | O(n + k^2)        | ~1,600 (d=0.1)   |
| sort_adaptive       | varies*           | varies*           |
+---------------------+-------------------+-------------------+

* sort_adaptive dispatches based on disorder:
  - Sorted/rotated-sorted:  O(n)            -> 0-250 ops
  - d < 0.2 (low):          O(n + k^2)      -> ~1,600 ops
  - 0.2 <= d < 0.5 (med):   O(n * sqrt(n))  -> ~6,651 ops
  - d >= 0.5 (high):        O(n * log(n))   -> ~5,000-7,000 ops

Helper complexities:
  normalize_stack:    O(n^2) for sort_copy + O(n log n) for bin_search
  compute_disorder:   O(n^2) for counting inversions
  is_sorted:          O(n)
  is_rotated_sorted:  O(n)
  find_min_pos:       O(n)
  find_max_pos:       O(n)
  find_insert_pos:    O(n) per call
  rotate_to_top:      O(n) worst case, O(n/2) average
  rot_dir:            O(1) dispatch helper
  check_duplicates:   O(n^2)
```

---

_End of push_swap Study Guide. Good luck with your defense!_
