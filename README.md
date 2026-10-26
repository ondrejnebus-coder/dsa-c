# Dynamic Set Data Structures in C

Four standalone C programs that implement and compare dynamic-set data structures:

- a **red-black tree**,
- a **treap** (binary search tree + heap priority),
- a **hash table with double hashing and rehashing**,
- a **reduced ordered binary decision diagram (BDD)** for Boolean expressions.

Each program has an interactive menu for inserting, searching and deleting keys. The
three set structures can also replay a file of `INSERT`, `SEARCH` and `DELETE`
commands, which was used for the benchmark in the project report.

This was a Data Structures and Algorithms course project at FIIT STU in Bratislava.

## Project layout

```text
src/
  red_black_tree.c   red-black tree with lazy deletion
  treap.c            treap with rotations
  hash_table.c       open addressing, double hashing, rehashing at 70 % load
  bdd.c              BDD construction, reduction and Boolean evaluation
test_input.txt       100 sample commands for the file-replay mode
docs/DSA-report.pdf  project report with design notes and benchmark results
CMakeLists.txt
```

## Build

Requirements: CMake 3.16+ and a C11 compiler.

```bash
cmake -S . -B build
cmake --build build
```

This produces four executables:

```text
build/red_black_tree
build/treap
build/hash_table
build/bdd
```

On Windows the executables are placed in `build/Debug/` or `build/Release/`
depending on the generator.

## Run

Start a program from the repository root so that it can find `test_input.txt`:

```bash
./build/red_black_tree
./build/treap
./build/hash_table
./build/bdd
```

Use the menu to insert, search, delete or run the file-based test. The sample file
contains 100 operations. The BDD program runs a small built-in correctness test.

## Benchmark results

The report compares all three set implementations on command files with 50,000 to
1,000,000 operations. Times are in seconds:

| Structure | 50k | 250k | 500k | 1M |
|---|---:|---:|---:|---:|
| Hash table (double hashing) | 0.107 | 0.537 | 0.777 | 3.030 |
| Treap | 0.037 | 0.235 | 0.360 | 3.329 |
| Red-black tree | 0.037 | 0.208 | 0.253 | 1.012 |

The red-black tree was the most consistent on the largest workload. The full
discussion, including the lazy-deletion trade-off and the effect of rehashing, is in
[`docs/DSA-report.pdf`](docs/DSA-report.pdf).

## Notes

- The red-black tree marks deleted nodes instead of performing a full delete fix-up.
  This keeps the implementation compact; the report explains the trade-off.
- The hash table grows through a fixed list of prime sizes and rehashes when the load
  factor exceeds 0.7.
- The BDD program supports up to 17 variables and uses a node pool with reduction of
  identical low/high branches.

## Author

Ondrej Nebus - FIIT STU, Bratislava.
