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

The execution timings include process creation, validation, compilation, the
guest workload and exit. They measure end-to-end cost at a fixed workload size.
Use `run_debug_core3_feature_performance.py` for the separate guest-clock
measurements. The Wasmtime CLI cache and LLVM JIT cache are disabled. The
ratio between two VM binaries addresses deployment regression. A ratio
against Wasmtime or WABT answers a separate engine comparison.

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
