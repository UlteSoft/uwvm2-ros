# Core 3.0 engine comparison

[简体中文](core3-engine-comparison.zh.md)

Use the same core Wasm bytes to compare UWVM's full interpreter and LLVM
backend with Wasmtime and WABT. The source generator supplies an independent
scalar checksum for each workload. Modules carry no WASI imports. The
comparison disables the VM's private WasmMTE extension and Wasmtime's
component model.

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py \
  /usr/bin/taskset -c 4 /usr/bin/python3 -B \
  test/0017.runtime/run_core3_engine_comparison.py \
  --runtime "$VM" --baseline "$BASELINE_VM" \
  --wasmtime "$WASMTIME" --wasm-tools "$WASM_TOOLS" \
  --wabt "$WASM_INTERP" --wabt-validator "$WASM_VALIDATE" \
  --output "$NEW_RESULTS"
```

The runner uses the existing 64 GiB, zero-swap gate. Serialize the runs with
other workload tests. Use builds with optimization and debug information for
performance attribution. The fixed matrix covers Memory64, multiple memories,
Table64, extended constants, typed references, tail calls, exception handling,
GC, relaxed SIMD and custom annotations. Choose `--seconds 900 --seed 1935627
--iterations 4096` and a fresh output directory for a 15-minute randomized
run of the same arithmetic and control-flow invariants.

The fixed runner times process creation, validation, compilation, the guest
workload and exit. For execution alone, pass the same `--runtime`,
`--baseline`, `--wasmtime`, `--wasm-tools` and a fresh `--output` to
`run_core3_kernel_performance.py`. That runner measures the interval between
two guest monotonic clock reads, before formatting and printing the result.
The only grants it uses are that clock and stdout.

Both tools disable the Wasmtime CLI cache and the LLVM JIT cache. The kernel
runner explicitly selects UWVM's maximum LLVM optimization level and
Wasmtime's Cranelift compiler with its speed optimization policy. The ratio
between two VM binaries addresses deployment regression. A ratio against
another engine describes the given workload and compilation policy.

A WABT validator rejection is retained with its diagnostic. The runner
executes WABT where its validator accepts the module and requires the exported
entry to report an actual execution result. Coverage varies by WABT version.
The executed WABT count, compiler versions and fixture digests are in the
receipt. Random trials vary the inputs to each checksum oracle. For official
specification assertions, run the pinned upstream WAST with the reference
tools separately. [Core tests](https://github.com/WebAssembly/spec/tree/main/test/core)
· [WABT test tools](https://github.com/WebAssembly/wabt/blob/main/test/README.md)
· [Wasmtime WAST command](https://docs.wasmtime.dev/cli-options.html#wast)

The first x86-64 packaging comparison executed 593 trials across 20 cases.
WABT 1.0.36 accepted nine cases, including the valid module that traps at
execution. The compact VM's AOT median was between 0.886 and 1.015 times the
original AOT median over successful cases. The exception payload and
exception-reference rethrow cases took 27.2 and 25.0 times the corresponding
Wasmtime 52.0.0-dev medians. These two workloads repeat genuine throws 100,000
times. The original VM also shows the gap.

The seeded 900-second run completed 11,110 cases with eight input values
per case and no mismatched executions. WABT 1.0.36 executed 4,660 of
those cases. All pinned binaries and the workload generator retained their
original digests.

The fixed matrix also passed against the official Wasmtime 50.0.2 and WABT
1.0.42 releases. WABT executed 13 of the 20 cases. In the 19 successful
kernel-only cases, the compact VM took 29.16 μs for Memory64, 136.48 μs
for indirect tail calls and 20.14 μs for relaxed swizzle. The corresponding
Cranelift medians were 70.72, 443.98 and 40.47 μs. Cross-engine results for
heavy allocation and actual exception propagation favored Cranelift.
[Wasmtime 50.0.2](https://github.com/bytecodealliance/wasmtime/releases/tag/v50.0.2)
· [WABT 1.0.42](https://github.com/WebAssembly/wabt/releases/tag/1.0.42)

The multi-memory copy case was repeated with sixteen iterations of the
kernel per process and 15 alternating samples. The compact and original
LLVM binaries took 8.323 ms and 8.373 ms, respectively. Cranelift took
0.827 ms on the same module.

An independent pass over 21 pinned upstream WAST files supplied 573
distinct binaries. Wasmtime 50.0.1 passed 18 scripts with strict diagnostics,
and three more with text matching relaxed. WABT 1.0.42 executed 5,420
assertions in 15 scripts it could translate. The VM compared the binary
validation outcomes and reattached registered providers for 22 cases.
Both VM engines instantiated 12 of those modules successfully and rejected
the other 10 at the import type check, as expected by the WAST.
[Pinned upstream WAST suite](https://github.com/WebAssembly/spec/tree/2e44bf79e68cc4fe689f43eea8e7d25b6fdfbce1/test/core)
