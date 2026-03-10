# CODEMAP.md — Function-Level Reference Card

Quick-lookup companion to STUDYGUIDE.md.
Shows how all pieces connect, what every function does, and where to find it.

---

## 1. Call Graph

### push_swap (main program)

```
main(argc, argv)                                          src/main.c:61
 ├── init_context(ctx)                                    src/main.c:22
 ├── create_stacks(ctx)                                   src/main.c:49
 │    └── stack_create(s, capacity)  [x2]                 src/stack_init.c:15
 ├── parse_args(argc, argv, ctx)                          src/parse_args.c:91
 │    ├── parse_flag(arg, ctx)                            src/parse_args.c:35
 │    ├── parse_split_arg(ctx, arg)                       src/parse_args.c:65
 │    │    ├── parse_flag(arg, ctx)                       src/parse_args.c:35
 │    │    └── add_number(ctx, str)                       src/parse_args.c:52
 │    │         ├── is_valid_integer(str)                 src/parse_utils.c:15
 │    │         └── ft_atol_safe(str, result)             src/parse_utils.c:33
 │    └── check_duplicates(a)                             src/parse_args.c:15
 ├── error_exit(ctx)                                      src/main.c:15
 │    └── free_context(ctx)                               src/stack_init.c:34
 │         └── stack_destroy(s)  [x2]                     src/stack_init.c:25
 ├── normalize_stack(a)                                   src/normalize.c:60
 │    ├── sort_copy(arr, n)                               src/normalize.c:15
 │    └── bin_search(sorted, n, val)                      src/normalize.c:39
 ├── compute_disorder(a)                                  src/disorder.c:37
 │    └── count_inversions(a)                             src/disorder.c:15
 ├── dispatch_sort(ctx)                                   src/main.c:35
 │    ├── is_sorted(a)                                    src/utils.c:15
 │    ├── sort_simple(ctx)          [STRAT_SIMPLE]        src/sort_simple.c:15
 │    ├── sort_medium(ctx)          [STRAT_MEDIUM]        src/sort_medium.c:63
 │    ├── sort_complex(ctx)         [STRAT_COMPLEX]       src/sort_complex.c:44
 │    └── sort_adaptive(ctx)        [STRAT_ADAPTIVE]      src/sort_adaptive.c:94
 ├── print_bench(ctx)               [if --bench]          src/bench.c:76
 └── free_context(ctx)                                    src/stack_init.c:34
```

### sort_adaptive (default algorithm)

```
sort_adaptive(ctx)                                        src/sort_adaptive.c:94
 ├── is_sorted(a)                                         src/utils.c:15
 ├── sort_two / sort_three / sort_five  [n <= 5]          src/sort_tiny.c
 ├── is_rotated_sorted(a)                                 src/sort_adaptive.c:15
 │    └── rotate_to_top(ctx, pos, 0)                      src/utils.c:73
 ├── sort_low_disorder(ctx)         [disorder < 0.2]      src/sort_adaptive.c:71
 │    ├── rotate_to_top(ctx, find_min_pos(a), 0)
 │    ├── do_ra / do_pb              [scan loop]
 │    └── reinsert_sorted(ctx)                            src/sort_adaptive.c:56
 │         ├── find_max_pos(b) → rotate_to_top(b)
 │         ├── find_insert_pos(a, val)                    src/sort_adaptive.c:35
 │         ├── rotate_to_top(a) → do_pa
 │         └── rotate_to_top(find_min_pos(a), 0)  [final]
 ├── sort_medium(ctx)               [disorder < 0.5]      src/sort_medium.c:63
 └── sort_complex(ctx)              [disorder >= 0.5]     src/sort_complex.c:44
```

### sort_medium (chunk-based)

```
sort_medium(ctx)                                          src/sort_medium.c:63
 ├── sort_two / sort_three / sort_five  [n <= 5]
 ├── get_chunk_size(n)                                    src/sort_medium_utils.c:25
 ├── phase_one(ctx, chunk_sz)                             src/sort_medium.c:39
 │    └── push_chunk(ctx, lo, hi)                         src/sort_medium.c:15
 │         ├── do_pb  [val in range]
 │         ├── do_rb  [val < midpoint]
 │         └── do_ra  [val out of range]
 └── phase_two(ctx)                                       src/sort_medium.c:57
      └── push_back_max(ctx)                              src/sort_medium_utils.c:35
           ├── find_max_pos(b)
           ├── rotate_to_top(ctx, pos, 1)
           └── do_pa
```

### sort_complex (radix sort)

```
sort_complex(ctx)                                         src/sort_complex.c:44
 ├── sort_two / sort_three / sort_five  [n <= 5]
 ├── get_max_bits(n - 1)                                  src/sort_complex.c:15
 └── radix_pass(ctx, bit)  [for each bit]                 src/sort_complex.c:25
      ├── do_ra  [bit is 1]
      ├── do_pb  [bit is 0]
      └── do_pa  [flush b back to a]
```

### do_* output layer (all identical pattern)

```
do_sa(ctx)  →  op_sa(&ctx->a)  +  write "sa\n"  +  ctx->ops[OP_SA]++  +  total_ops++
do_sb(ctx)  →  op_sb(&ctx->b)  +  write "sb\n"  +  ctx->ops[OP_SB]++  +  total_ops++
 ... (same pattern for all 11 operations)
```

### checker (bonus program)

```
main(argc, argv)                                          bonus/checker_main_bonus.c:47
 ├── init_checker_ctx(ctx)                                bonus/checker_main_bonus.c:26
 ├── stack_create(s, capacity)  [x2]                      src/stack_init.c:15
 ├── parse_args(argc, argv, ctx)                          src/parse_args.c:91
 ├── checker_error_exit(ctx)                              bonus/checker_main_bonus.c:19
 ├── read_and_exec(ctx)                                   bonus/checker_exec_bonus.c:61
 │    ├── get_next_line(0)                                libft/get_next_line.c
 │    └── exec_operation(ctx, line)                       bonus/checker_exec_bonus.c:52
 │         ├── exec_swap_push(ctx, line)                  bonus/checker_exec_bonus.c:16
 │         │    └── op_sa / op_sb / op_ss / op_pa / op_pb
 │         └── exec_rotate(ctx, line)                     bonus/checker_exec_bonus.c:33
 │              └── op_rra / op_rrb / op_rrr / op_ra / op_rb / op_rr
 ├── check_result(ctx)                                    bonus/checker_main_bonus.c:39
 │    └── is_sorted(a)
 └── free_context(ctx)                                    src/stack_init.c:34
```

---

## 2. Recommended Reading Order

### Layer 1 — Data Types
| File | Why |
|------|-----|
| `includes/push_swap.h` | All types (`t_stack`, `t_context`), operation defines, strategy defines, every prototype |

### Layer 2 — Infrastructure
| File | Why |
|------|-----|
| `src/stack_init.c` | Stack lifecycle: create, destroy, free context |
| `src/stack_ops_push.c` | Core `push_front`/`pop_front` — how elements move between stacks |
| `src/stack_ops_swap.c` | Swap top two elements |
| `src/stack_ops_rotate.c` | Rotate: first element goes to bottom |
| `src/stack_ops_revrot.c` | Reverse rotate: last element goes to top |
| `src/parse_utils.c` | Integer validation and safe long conversion |
| `src/parse_args.c` | Argument parsing, flag handling, duplicate checking |

### Layer 3 — Algorithm Support
| File | Why |
|------|-----|
| `src/normalize.c` | Replace raw values with rank indices (0..n-1) |
| `src/disorder.c` | Inversion count → disorder ratio for adaptive strategy |
| `src/utils.c` | `is_sorted`, `find_min_pos`, `find_max_pos`, `rotate_to_top` |

### Layer 4 — Algorithms (read in order of complexity)
| File | Why |
|------|-----|
| `src/sort_tiny.c` | Base cases: 2, 3, and 5 elements |
| `src/sort_simple.c` | Selection sort — push min to b repeatedly |
| `src/sort_medium_utils.c` | Chunk size formula, sqrt approx, push_back_max |
| `src/sort_medium.c` | Chunk-based sort — push ranges to b, greedy recall |
| `src/sort_complex.c` | Radix sort — bit-by-bit partitioning |
| `src/sort_adaptive.c` | Strategy selector using disorder metric |

### Layer 5 — Output
| File | Why |
|------|-----|
| `src/ops_out_swap.c` | `do_sa`..`do_pb` — execute + print + count |
| `src/ops_out_rot.c` | `do_ra`..`do_rr` |
| `src/ops_out_revrot.c` | `do_rra`..`do_rrr` |
| `src/bench.c` | `--bench` diagnostics: disorder%, strategy name, op counts |

### Layer 6 — Bonus
| File | Why |
|------|-----|
| `bonus/checker_main_bonus.c` | Checker entry point — parse, read ops from stdin, verify |
| `bonus/checker_exec_bonus.c` | Map operation strings to op_* calls |

---

## 3. File-by-File Function Index

### includes/push_swap.h
```
Types:
  t_stack                             — int *data, int size, int capacity
  t_context                           — t_stack a/b, int ops[11], total_ops, strategy, bench, double disorder

Defines:
  OP_SA..OP_RRR  (0..10)             — Operation indices for ops[] counter array
  STRAT_SIMPLE..STRAT_ADAPTIVE (0..3) — Strategy selector constants
```

### src/main.c
```
  error_exit(ctx)                     — free_context + "Error\n" to stderr + exit(1)
  init_context(ctx)            [static] Zero-init all t_context fields; default STRAT_ADAPTIVE
  dispatch_sort(ctx)           [static] Route to sort algorithm based on ctx->strategy
  create_stacks(ctx)           [static] malloc both stacks with capacity 10000; cleanup on fail
  main(argc, argv)                    — Entry: parse → normalize → disorder → sort → bench → free
```

### src/stack_init.c
```
  stack_create(s, capacity)           — malloc data array, set size=0 and capacity; return 0/1
  stack_destroy(s)                    — free data, null out fields
  free_context(ctx)                   — destroy both stacks a and b
```

### src/parse_args.c
```
  check_duplicates(a)          [static] O(n^2) scan for duplicate values in stack; return 0/1
  parse_flag(arg, ctx)         [static] Match "--simple/--medium/--complex/--adaptive/--bench"
  add_number(ctx, str)         [static] Validate + convert str → int, append to ctx->a
  parse_split_arg(ctx, arg)    [static] ft_split on space, parse each token as flag or number
  parse_args(argc, argv, ctx)         — Iterate argv: flags direct, numbers via parse_split_arg
```

### src/parse_utils.c
```
  is_valid_integer(str)               — Check optional sign + all digits, non-empty
  ft_atol_safe(str, result)           — Convert to long with INT_MIN/INT_MAX overflow check
```

### src/normalize.c
```
  sort_copy(arr, n)            [static] In-place selection sort on a copied array
  bin_search(sorted, n, val)   [static] Binary search returning index of val in sorted array
  normalize_stack(a)                  — Replace raw values with rank indices 0..n-1
```

### src/disorder.c
```
  count_inversions(a)          [static] O(n^2) count pairs where a[i] > a[j] for i < j
  compute_disorder(a)                 — inversions / max_possible → double in [0.0, 1.0]
```

### src/utils.c
```
  is_sorted(a)                        — Return 1 if a->data is ascending, 0 otherwise
  find_min_pos(s)                     — Index of minimum value in stack
  find_max_pos(s)                     — Index of maximum value in stack
  rot_dir(ctx, stack_id, rev)  [static] Call do_ra/do_rb or do_rra/do_rrb based on params
  rotate_to_top(ctx, pos, id)         — Rotate stack until position pos is at index 0
```

### src/sort_tiny.c
```
  sort_two(ctx)                       — Single sa if first > second
  sort_three(ctx)                     — 5 conditional branches covering all 6 permutations
  push_min_to_b(ctx)           [static] find_min_pos → rotate_to_top → pb
  sort_five(ctx)                      — Push smallest to b until 3 remain, sort_three, pa back
```

### src/sort_simple.c
```
  sort_simple(ctx)                    — Selection sort: repeatedly push min to b, then pa all back
```

### src/sort_medium_utils.c
```
  ft_sqrt_approx(n)                   — Integer ceiling of sqrt(n) via linear scan
  get_chunk_size(n)                   — Formula: (n * 3) / 100 + 14, minimum 5
  push_back_max(ctx)                  — find_max_pos(b) → rotate_to_top(b) → pa
```

### src/sort_medium.c
```
  push_chunk(ctx, lo, hi)     [static] Push values in [lo,hi] to b; rb if below midpoint
  phase_one(ctx, chunk_sz)    [static] Iterate chunks from 0 to n, push each chunk to b
  phase_two(ctx)              [static] Repeatedly push_back_max until b is empty
  sort_medium(ctx)                    — Chunk-based sort: phase_one pushes to b, phase_two recalls
```

### src/sort_complex.c
```
  get_max_bits(n)              [static] Count bits needed to represent n: floor(log2(n)) + 1
  radix_pass(ctx, bit)         [static] Partition a by bit: 1-bits stay (ra), 0-bits go to b (pb)
  sort_complex(ctx)                   — LSD radix sort over normalized values, bit by bit
```

### src/sort_adaptive.c
```
  is_rotated_sorted(a)         [static] True if at most 1 descending break (circular sorted)
  find_insert_pos(a, val)      [static] Find position in a where val maintains sorted order
  reinsert_sorted(ctx)         [static] Pop b's max, insert at correct position in a, repeat
  sort_low_disorder(ctx)       [static] Rotate to min, keep sorted elements, pb outliers, reinsert
  sort_adaptive(ctx)                  — Dispatch: rotated→rotate, low→sort_low, med→medium, hi→complex
```

### src/bench.c
```
  print_disorder(ctx)          [static] Format disorder as "XX.XX%" to stderr
  strat_name(strat, disorder)  [static] Return human-readable strategy string
  print_strategy(ctx)          [static] Print strategy name + total_ops to stderr
  print_ops_line(ctx, s, e)    [static] Print count of ops[s..e] on one line to stderr
  print_bench(ctx)                    — Print full benchmark: disorder, strategy, all op counts
```

### src/stack_ops_swap.c
```
  op_sa(a)                            — Swap a->data[0] and a->data[1]
  op_sb(b)                            — Swap b->data[0] and b->data[1]
  op_ss(a, b)                         — op_sa + op_sb
```

### src/stack_ops_push.c
```
  push_front(s, val)           [static] Shift all elements right, insert val at index 0
  pop_front(s)                 [static] Remove and return s->data[0], shift all left
  op_pa(a, b)                         — Pop front of b, push front of a
  op_pb(a, b)                         — Pop front of a, push front of b
```

### src/stack_ops_rotate.c
```
  rotate(s)                    [static] Save data[0], shift all left, place saved at end
  op_ra(a)                            — Rotate stack a (first → last)
  op_rb(b)                            — Rotate stack b (first → last)
  op_rr(a, b)                         — Rotate both a and b simultaneously
```

### src/stack_ops_revrot.c
```
  rev_rotate(s)                [static] Save data[size-1], shift all right, place saved at front
  op_rra(a)                           — Reverse rotate a (last → first)
  op_rrb(b)                           — Reverse rotate b (last → first)
  op_rrr(a, b)                        — Reverse rotate both a and b simultaneously
```

### src/ops_out_swap.c
```
  do_sa(ctx)                          — op_sa + write "sa\n" + count
  do_sb(ctx)                          — op_sb + write "sb\n" + count
  do_ss(ctx)                          — op_ss + write "ss\n" + count
  do_pa(ctx)                          — op_pa + write "pa\n" + count
  do_pb(ctx)                          — op_pb + write "pb\n" + count
```

### src/ops_out_rot.c
```
  do_ra(ctx)                          — op_ra + write "ra\n" + count
  do_rb(ctx)                          — op_rb + write "rb\n" + count
  do_rr(ctx)                          — op_rr + write "rr\n" + count
```

### src/ops_out_revrot.c
```
  do_rra(ctx)                         — op_rra + write "rra\n" + count
  do_rrb(ctx)                         — op_rrb + write "rrb\n" + count
  do_rrr(ctx)                         — op_rrr + write "rrr\n" + count
```

### bonus/checker_main_bonus.c
```
  checker_error_exit(ctx)      [static] free_context + "Error\n" to stderr + exit(1)
  init_checker_ctx(ctx)        [static] Zero-init context (same as init_context)
  check_result(ctx)            [static] Print "OK\n" if sorted and b empty, else "KO\n"
  main(argc, argv)                    — Entry: parse → read_and_exec → check_result → free
```

### bonus/checker_exec_bonus.c
```
  exec_swap_push(ctx, line)    [static] Match "sa\n"/"sb\n"/"ss\n"/"pa\n"/"pb\n" → call op_*
  exec_rotate(ctx, line)       [static] Match "rra\n"/"rrb\n"/"rrr\n"/"ra\n"/"rb\n"/"rr\n" → op_*
  exec_operation(ctx, line)           — Try exec_swap_push, then exec_rotate; return 0 if unknown
  read_and_exec(ctx)                  — Loop get_next_line(0) → exec_operation; return 0 on bad op
```

---

## 4. Who Calls Whom

For each function: **callers** (who calls it) and **callees** (what it calls).

### Core Infrastructure

| Function | Callers | Callees |
|----------|---------|---------|
| `stack_create` | `create_stacks`, checker `main` | `malloc` |
| `stack_destroy` | `free_context`, `create_stacks` (on fail) | `free` |
| `free_context` | `error_exit`, `main` (push_swap), checker `main`, `checker_error_exit` | `stack_destroy` x2 |
| `error_exit` | `main` (push_swap) | `free_context`, `write`, `exit` |

### Parsing

| Function | Callers | Callees |
|----------|---------|---------|
| `is_valid_integer` | `add_number` | `ft_isdigit` |
| `ft_atol_safe` | `add_number` | — |
| `add_number` | `parse_split_arg` | `is_valid_integer`, `ft_atol_safe` |
| `parse_flag` | `parse_args`, `parse_split_arg` | `ft_strncmp` |
| `parse_split_arg` | `parse_args` | `ft_split`, `parse_flag`, `add_number`, `free` |
| `check_duplicates` | `parse_args` | — |
| `parse_args` | `main` (both programs) | `parse_flag`, `parse_split_arg`, `check_duplicates` |

### Normalization & Analysis

| Function | Callers | Callees |
|----------|---------|---------|
| `sort_copy` | `normalize_stack` | — |
| `bin_search` | `normalize_stack` | — |
| `normalize_stack` | `main` | `malloc`, `sort_copy`, `bin_search`, `free` |
| `count_inversions` | `compute_disorder` | — |
| `compute_disorder` | `main` | `count_inversions` |

### Utilities

| Function | Callers | Callees |
|----------|---------|---------|
| `is_sorted` | `dispatch_sort`, `sort_simple`, `sort_medium`, `sort_complex`, `sort_adaptive`, `check_result` | — |
| `find_min_pos` | `push_min_to_b`, `sort_low_disorder`, `reinsert_sorted`, `find_insert_pos` | — |
| `find_max_pos` | `push_back_max`, `reinsert_sorted` | — |
| `rot_dir` | `rotate_to_top` | `do_ra`, `do_rb`, `do_rra`, `do_rrb` |
| `rotate_to_top` | `push_min_to_b`, `push_back_max`, `sort_low_disorder`, `reinsert_sorted`, `sort_adaptive` | `rot_dir` |

### Stack Operations (op_*)

| Function | Callers | Callees |
|----------|---------|---------|
| `op_sa` | `do_sa`, `op_ss`, checker `exec_swap_push` | — |
| `op_sb` | `do_sb`, `op_ss`, checker `exec_swap_push` | — |
| `op_ss` | `do_ss`, checker `exec_swap_push` | `op_sa`, `op_sb` |
| `push_front` | `op_pa`, `op_pb` | — |
| `pop_front` | `op_pa`, `op_pb` | — |
| `op_pa` | `do_pa`, checker `exec_swap_push` | `push_front`, `pop_front` |
| `op_pb` | `do_pb`, checker `exec_swap_push` | `push_front`, `pop_front` |
| `rotate` | `op_ra`, `op_rb`, `op_rr` | — |
| `op_ra` | `do_ra`, checker `exec_rotate` | `rotate` |
| `op_rb` | `do_rb`, checker `exec_rotate` | `rotate` |
| `op_rr` | `do_rr`, checker `exec_rotate` | `rotate` x2 |
| `rev_rotate` | `op_rra`, `op_rrb`, `op_rrr` | — |
| `op_rra` | `do_rra`, checker `exec_rotate` | `rev_rotate` |
| `op_rrb` | `do_rrb`, checker `exec_rotate` | `rev_rotate` |
| `op_rrr` | `do_rrr`, checker `exec_rotate` | `rev_rotate` x2 |

### Output Operations (do_*)

| Function | Callers | Callees |
|----------|---------|---------|
| `do_sa` | `sort_two`, `sort_three` | `op_sa`, `write` |
| `do_sb` | (unused in current algorithms) | `op_sb`, `write` |
| `do_ss` | (unused in current algorithms) | `op_ss`, `write` |
| `do_pa` | `sort_five`, `sort_simple`, `push_back_max`, `radix_pass`, `reinsert_sorted` | `op_pa`, `write` |
| `do_pb` | `push_min_to_b`, `sort_simple`, `push_chunk`, `radix_pass`, `sort_low_disorder` | `op_pb`, `write` |
| `do_ra` | `rot_dir`, `sort_three`, `push_chunk`, `radix_pass`, `sort_low_disorder` | `op_ra`, `write` |
| `do_rb` | `rot_dir`, `push_chunk` | `op_rb`, `write` |
| `do_rr` | (unused in current algorithms) | `op_rr`, `write` |
| `do_rra` | `rot_dir`, `sort_three` | `op_rra`, `write` |
| `do_rrb` | `rot_dir` | `op_rrb`, `write` |
| `do_rrr` | (unused in current algorithms) | `op_rrr`, `write` |

### Sorting Algorithms

| Function | Callers | Callees |
|----------|---------|---------|
| `sort_two` | `sort_simple`, `sort_medium`, `sort_complex`, `sort_adaptive` | `do_sa` |
| `sort_three` | `sort_five`, `sort_simple`, `sort_medium`, `sort_complex`, `sort_adaptive` | `do_sa`, `do_ra`, `do_rra` |
| `push_min_to_b` | `sort_five` | `find_min_pos`, `rotate_to_top`, `do_pb` |
| `sort_five` | `sort_medium`, `sort_complex`, `sort_adaptive` | `push_min_to_b`, `sort_three`, `do_pa` |
| `sort_simple` | `dispatch_sort` | `is_sorted`, `sort_two`, `sort_three`, `find_min_pos`, `rotate_to_top`, `do_pb`, `do_pa` |
| `get_chunk_size` | `sort_medium` | — |
| `ft_sqrt_approx` | (available but unused in current code) | — |
| `push_back_max` | `phase_two` | `find_max_pos`, `rotate_to_top`, `do_pa` |
| `push_chunk` | `phase_one` | `do_pb`, `do_rb`, `do_ra` |
| `phase_one` | `sort_medium` | `push_chunk` |
| `phase_two` | `sort_medium` | `push_back_max` |
| `sort_medium` | `dispatch_sort`, `sort_adaptive` | `is_sorted`, `sort_two`, `sort_three`, `sort_five`, `get_chunk_size`, `phase_one`, `phase_two` |
| `get_max_bits` | `sort_complex` | — |
| `radix_pass` | `sort_complex` | `do_ra`, `do_pb`, `do_pa` |
| `sort_complex` | `dispatch_sort`, `sort_adaptive` | `is_sorted`, `sort_two`, `sort_three`, `sort_five`, `get_max_bits`, `radix_pass` |
| `is_rotated_sorted` | `sort_adaptive` | — |
| `find_insert_pos` | `reinsert_sorted` | `find_min_pos` |
| `reinsert_sorted` | `sort_low_disorder` | `find_max_pos`, `rotate_to_top`, `find_insert_pos`, `do_pa`, `find_min_pos` |
| `sort_low_disorder` | `sort_adaptive` | `rotate_to_top`, `find_min_pos`, `do_ra`, `do_pb`, `reinsert_sorted` |
| `sort_adaptive` | `dispatch_sort` | `is_sorted`, `sort_two`, `sort_three`, `sort_five`, `is_rotated_sorted`, `rotate_to_top`, `find_min_pos`, `sort_low_disorder`, `sort_medium`, `sort_complex` |

### Bench

| Function | Callers | Callees |
|----------|---------|---------|
| `print_disorder` | `print_bench` | `ft_putstr_fd`, `ft_putnbr_fd` |
| `strat_name` | `print_strategy` | — |
| `print_strategy` | `print_bench` | `strat_name`, `ft_putstr_fd`, `ft_putnbr_fd` |
| `print_ops_line` | `print_bench` | `ft_putstr_fd`, `ft_putnbr_fd` |
| `print_bench` | `main` | `print_disorder`, `print_strategy`, `print_ops_line` |

### Checker Bonus

| Function | Callers | Callees |
|----------|---------|---------|
| `checker_error_exit` | checker `main` | `free_context`, `write`, `exit` |
| `init_checker_ctx` | checker `main` | — |
| `check_result` | checker `main` | `is_sorted` |
| `exec_swap_push` | `exec_operation` | `op_sa`, `op_sb`, `op_ss`, `op_pa`, `op_pb`, `ft_strncmp` |
| `exec_rotate` | `exec_operation` | `op_rra`, `op_rrb`, `op_rrr`, `op_ra`, `op_rb`, `op_rr`, `ft_strncmp` |
| `exec_operation` | `read_and_exec` | `exec_swap_push`, `exec_rotate` |
| `read_and_exec` | checker `main` | `get_next_line`, `exec_operation`, `free` |

---

## 5. Key Constants & Magic Numbers

| Constant / Expression | Value | Where | Why |
|----------------------|-------|-------|-----|
| Stack capacity | `10000` | `main.c:51`, checker `main` | Max elements supported |
| `OP_SA`..`OP_RRR` | `0`..`10` | `push_swap.h:19-29` | Index into `ctx->ops[11]` counter array |
| `STRAT_SIMPLE`..`STRAT_ADAPTIVE` | `0`..`3` | `push_swap.h:31-34` | Strategy selector for `dispatch_sort` |
| Chunk size formula | `(n * 3) / 100 + 14` | `sort_medium_utils.c:29` | Empirically tuned for ~5500 ops on 500 elements |
| Chunk minimum | `5` | `sort_medium_utils.c:31` | Floor to avoid degenerate tiny chunks |
| Chunk midpoint | `(lo + hi) / 2` | `sort_medium.c:21` | Split chunk in b: small values sink via rb |
| Disorder threshold (low) | `0.2` | `sort_adaptive.c:113` | Below this → `sort_low_disorder` (near-sorted path) |
| Disorder threshold (medium) | `0.5` | `sort_adaptive.c:115` | Below this → `sort_medium` (chunk-based) |
| Disorder = 1.0 | fully reversed | `disorder.c:48` | `inversions / max_inversions` |
| Max inversions | `n * (n-1) / 2` | `disorder.c:45` | Denominator for disorder ratio |
| Radix bit count | `get_max_bits(n-1)` | `sort_complex.c:61` | Number of passes = bits in largest normalized value |
| sort_three cases | 5 branches | `sort_tiny.c:30-46` | Covers all 6 permutations (sorted case = no-op) |
| INT_MAX overflow | `(long)INT_MAX + 1` | `parse_utils.c:52` | Allows -2147483648 (INT_MIN) |

---

## 6. Tricky Code Patterns

### Pattern 1: Backward rotation loop
```c
// utils.c:88 — rotate_to_top reverse direction
while (pos++ < size)
    rot_dir(ctx, stack_id, 1);
```
`pos` starts past midpoint. `pos++` increments *after* comparison, so the loop runs
`size - pos` times (the distance from pos to the end), which is the shorter reverse path.

### Pattern 2: Wrap-around insertion position
```c
// sort_adaptive.c:49 — find_insert_pos
best_pos = (i + 1) % a->size;
```
After finding the largest value smaller than `val`, the insertion point is one position
ahead. The modulo wraps index `a->size` back to `0`, handling the circular boundary.

### Pattern 3: Radix bit extraction
```c
// sort_complex.c:34 — radix_pass
if ((ctx->a.data[0] >> bit) & 1)
    do_ra(ctx);    // bit is 1 → keep in a
else
    do_pb(ctx);    // bit is 0 → move to b
```
Right-shift isolates the target bit, `& 1` extracts it. LSD radix processes bit 0 first,
then bit 1, etc. Stable partitioning preserves order from previous passes.

### Pattern 4: Float formatting without printf
```c
// bench.c:20-21 — print_disorder
pct_int = (int)(ctx->disorder * 100);
pct_dec = (int)(ctx->disorder * 10000) % 100;
```
Extracts integer and fractional parts of a percentage. For disorder = 0.4567:
`pct_int = 45`, `pct_dec = 67`. The `% 100` strips the already-printed integer part.
Leading zero padded manually (`if (pct_dec < 10)`).

### Pattern 5: Check longer ops before shorter
```c
// checker_exec_bonus.c:35-46 — exec_rotate
if (ft_strncmp(line, "rra\n", 4) == 0)      // check rra BEFORE ra
    ...
else if (ft_strncmp(line, "ra\n", 3) == 0)   // ra checked after rra
```
"rra\n" starts with "ra", so `ft_strncmp(line, "ra\n", 3)` would match "rra\n" too.
Checking the longer string first prevents false matches.

### Pattern 6: Norm-style pre-increment loop
```c
// normalize.c:68-70
i = -1;
while (++i < a->size)
    sorted[i] = a->data[i];
```
42 Norm limits to 25 lines per function and forbids `for` loops. `i = -1` + `++i`
simulates `for (i = 0; i < n; i++)` in one fewer line.

### Pattern 7: Chunk midpoint split
```c
// sort_medium.c:28-31 — push_chunk
do_pb(ctx);
if (ctx->b.data[0] < mid)
    do_rb(ctx);       // sink small values to bottom of b
```
After pushing to b, values below the chunk midpoint get rotated to the bottom.
This pre-sorts b into two halves, reducing the work in phase_two when recalling
elements by max value.

### Pattern 8: Rotated-sorted detection
```c
// sort_adaptive.c:15-33 — is_rotated_sorted
breaks = 0;
while (i < a->size - 1)
    if (a->data[i] > a->data[i + 1])
        breaks++;
if (breaks == 1 && a->data[a->size - 1] > a->data[0])
    return (0);
```
A sorted array rotated by k positions has exactly 1 "break" (descending pair), and
the last element must be less than the first (wrap-around). If both conditions hold,
a single `rotate_to_top(find_min_pos)` solves it in O(n) ops.

---

## 7. Architecture Summary

```
┌─────────────────────────────────────────────────────────────┐
│                        push_swap.h                          │
│  t_stack { data, size, capacity }                           │
│  t_context { a, b, ops[11], total_ops, strategy, bench,     │
│              disorder }                                     │
└─────────────────────┬───────────────────────────────────────┘
                      │
         ┌────────────┼────────────┐
         ▼            ▼            ▼
   ┌──────────┐ ┌──────────┐ ┌──────────────┐
   │  parse   │ │  stack   │ │  normalize   │
   │  layer   │ │  layer   │ │  & analysis  │
   ├──────────┤ ├──────────┤ ├──────────────┤
   │parse_args│ │stack_init│ │normalize.c   │
   │parse_util│ │ops_swap  │ │disorder.c    │
   │          │ │ops_push  │ │utils.c       │
   │          │ │ops_rotate│ │              │
   │          │ │ops_revrot│ │              │
   └────┬─────┘ └────┬─────┘ └──────┬───────┘
        │             │              │
        ▼             ▼              ▼
   ┌──────────────────────────────────────────┐
   │             Output Layer                  │
   │  ops_out_swap / ops_out_rot / ops_out_rev │
   │  do_* = op_* + write + count              │
   └─────────────────┬────────────────────────┘
                     │
                     ▼
   ┌──────────────────────────────────────────┐
   │           Algorithm Layer                 │
   ├──────────┬───────────┬───────────────────┤
   │sort_tiny │sort_simple│ sort_medium       │
   │ (2,3,5)  │ O(n^2)   │ O(n*sqrt(n))     │
   ├──────────┴───────────┼───────────────────┤
   │ sort_complex          │ sort_adaptive     │
   │ O(n*log(n)) radix     │ dispatcher using  │
   │                       │ disorder metric   │
   └───────────────────────┴───────────────────┘
                     │
                     ▼
   ┌──────────────────────────────────────────┐
   │  bench.c — optional diagnostics          │
   │  --bench flag prints to stderr           │
   └──────────────────────────────────────────┘

   ┌──────────────────────────────────────────┐
   │  BONUS: checker                           │
   │  checker_main → parse → read_and_exec    │
   │  exec_operation → op_* (no do_*, no      │
   │  output) → check_result: "OK" / "KO"    │
   └──────────────────────────────────────────┘
```

**Key design rule:** The `op_*` functions mutate stacks silently. The `do_*` wrappers
add output and counting. The checker uses `op_*` directly (no output needed).

---

## File Count Summary

| Category | Files | Functions (public + static) |
|----------|-------|---------------------------|
| Header | 1 | — (types + prototypes) |
| Main / entry | 1 | 2 + 3 = 5 |
| Parsing | 2 | 3 + 4 = 7 |
| Stack lifecycle | 1 | 3 + 0 = 3 |
| Stack ops (raw) | 4 | 11 + 4 = 15 |
| Output ops | 3 | 11 + 0 = 11 |
| Normalization | 1 | 1 + 2 = 3 |
| Analysis | 2 | 5 + 2 = 7 |
| Algorithms | 5 | 10 + 10 = 20 |
| Bench | 1 | 1 + 4 = 5 |
| Bonus | 2 | 3 + 5 = 8 |
| **Total** | **23 + 1 header** | **50 public + 34 static = 84** |
