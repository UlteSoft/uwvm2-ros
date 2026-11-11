# Numeric GC code-generation contracts

The fixtures check exact i64, f64 (including signaling NaN and negative zero) and v128 bits, null and index traps, canonical foreign-module ownership, subtype fallback, aliases, hash-bucket collisions and live collection. The existing scalar and packed suites cover the smaller numeric types. Borrow-boundary fixtures exercise local reassignment, branches, loops, calls, allocations, reference-valued helpers, changing array indices and late publication. Calls that collect retain the original object and verify it after collection.

Run both the interpreter and LLVM JIT with ../run_numeric_codegen.py. The runner requires an existing VM and a GC-capable assembler, writes all outputs to the explicit output directory, accepts an existing QEMU argv prefix, and never installs tools. The wat-to-wasm assembler style accepts a converter with positional input/output arguments. The entry field is a local-defined function index, not an import-inclusive index. foreign-provider.wat is a real preloaded Wasm module.

These are regression contracts, not a claim of exhaustive Core 3 conformance. The separate Core 3 specification suite remains necessary. They deliberately isolate standard GC semantics from the nonstandard memory-tag extension.
