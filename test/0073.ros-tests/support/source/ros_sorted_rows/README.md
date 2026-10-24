# Borrowed lookup for sorted native text rows

`sorted_rows` keeps owning, unique, lexically sorted newline rows for ROS2 Graph
and parameter-list results. A borrowed four-fragment lookup key avoids allocating
an owning row for a duplicate or an aggregate-quota refusal. Accepted unique rows
still allocate their owning string and set node. The per-row full-quota check,
sticky errors, deferred UTF8 result validation, embedded NUL behavior, complete
wire framing, and duplicate budget accounting remain unchanged.

This common header must support C++17 native SDK compilers. Its borrowed standard
views and `char_traits` comparison do not require the vendored C++20 `fast_io`
concepts headers. There is no formatter, I/O sink, throughput, or ROS middleware
allocation claim here.

Run on Linux under the configured 64GiB cgroup using a fresh output directory:

```sh
python3 test/0073.ros-tests/support/source/ros_sorted_rows/run.py \
  --source /absolute/project-source \
  --out /absolute/new-results \
  --compiler /absolute/clang++ \
  --llvm-libs /absolute/llvm/lib:/absolute/llvm/lib/target
```

`--bounded-header /absolute/bounded_text.h` selects an explicit candidate header
while the other three shared prerequisites come from `--source`. The runner
freezes those inputs and the three test files, records actual compiler commands
and raw MMD dependencies, and samples the stopped actual ELF before hashing its
pinned executable FD. It builds C++17 with libstdc++ in normal and ASan/UBSan modes,
and libc++ with ASan/UBSan. The tests replace ordinary global `new` only for each
measured `add` call; fixture creation, old-oracle setup, `result()`, and printing
are outside the measured interval.

`before-bounded-text.inc` is the exact original header, SHA256
`f2020789263957389ef1b4e7f27866ba4dcd84b5fc3b6353a9ace9d7d8151320`.
The test changes its namespace only during preprocessing. It compares complete
wire bytes, acceptance, error, emptiness, and retained byte counts with this
reference, including delimiter collisions, empty fields, embedded NULs, UTF8,
invalid bytes, sticky quota failures, and a separate fixed wire golden.

Actual focused evidence under `/home/macromodel/.local/share/uwvm-ros-extension`:

- `20261013-sorted-rows-allocation-before-r1/results/report.json`, SHA256
  `1e498e8eb0c8e792d9317e4aa83c3f3ee470f043259c8a0a57325c474bb8d22a`:
  three CPP17 profiles confirm that four long duplicate/refusal calls each use
  one temporary allocation, requesting 65,537 bytes with libstdc++ and 65,544
  with libc++. A refused result has error90 and no payload.
- `20261013-sorted-rows-allocation-after-r1/results/report.json`, SHA256
  `3ed7b7e85a7c893ad3a0a6c2668c1801cb9e787e064355d171ce55d6f2b93a35`:
  each profile passes 2,182 differential cases and 6,571 checks. Those four
  duplicate/refusal calls allocate zero bytes. A fresh accepted long row still
  makes two ordinary allocations. All three actual ELF identities, source/MMD
  hashes, readable ancestor OOM deltas, and owned-child cleanup are recorded.

The focused report does not establish native Gateway runtime or full-SDK
sanitizer coverage. SDK compile/link and any later integration evidence have
separate reports and exact dependency bindings.
