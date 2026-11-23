# Focused AOT bookkeeping regressions

These are small checks for compilation bookkeeping. They do not replace full
Wasm conformance, debugger, hot-update or multi-target qualification.

## Parallel native-object partitions

Compile `partition_layout.cc` as C++23 or later with the repository `src`
include directory. It checks nonempty, complete partition coverage, requested
worker limits, and arithmetic near `size_t` maximum without allocating a list
of that size. There is no new function-count limit.

`run_partition_remainder.py` requires an existing VM and `wasm-tools` binary.
Run it under the project's resource cgroup, with a fresh scratch output:

```sh
python3 -B test/0072.core3_codegen_cross/run_partition_remainder.py \
  --uwvm /absolute/path/to/uwvm2-ros \
  --wasm-tools /absolute/path/to/wasm-tools \
  --output .tmp/aot-partition-smoke
```

It executes four GC modules with serial emission and two parallel worker
settings (12 cold executions). Each module verifies the value passed through
all functions. Three of the seven-extra-worker cases previously accessed a
function index beyond the list after rounding the partition size upward.
Only nonempty partitions are now scheduled. Function-owner and exposed-symbol
indexes are immutable and shared until the worker batch has joined; their
storage is used only during compilation.

## LLVM register-liveness storage

Compile `live_variables_storage.cc` against the **rebuilt paired** LLVM headers
and libraries with the same C++ runtime/RTTI settings as the provider. It checks
that the single-kill case uses inline storage, that 4,096 records can grow
without an artificial ceiling, and that copy, move, clear and erase preserve
records. No opaque instruction identity is dereferenced.

`liveness_codegen.ll` exercises branching, two distinct kill paths, loop PHIs,
volatile stores and calls. Emit it with the before/after target machines and
compare the complete objects. The analysis container is private compilation
state, but its C++ representation changes: all dependents of the header must be
rebuilt. The bundled source inventory and archive closure identify that rebuild.

See `README.legacy-analysis-bindings.md` for the separate analysis-resolution
and last-use-list regression. None of these optimizations changes the selected
optimization level or adds a runtime JIT option.
