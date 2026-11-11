# Runtime Commands

The uwvm2-ros product runs on its qualified Linux GNU targets and uses eager,
full-module LLVM AOT compilation. `--runtime-aot` (`-Raot`) selects that backend;
it is also the product default. Native EH, CFI, the real per-process generated
unwind witness and crash stack recovery are mandatory and cannot be disabled.

Interpreter-enabled builds also accept `--runtime-int` (`-Rint`). Both backends
use the same host namespace, exact interface and operation/resource policy.
The interpreter does not provide the LLVM source/Wasm/assembly debugger. LLVM
AOT rejects unsupported lowering instead of silently falling back to the
interpreter or instruction-based exception propagation.

Source focus:

- `src/uwvm2/uwvm/cmdline/params/runtime_*.h`
- `src/uwvm2/uwvm/cmdline/callback/runtime_*.h`
- `src/uwvm2/uwvm/runtime/runtime_mode/storage.h`
- `src/uwvm2/uwvm/run/run.h`
- `src/uwvm2/runtime/compiler/**/compile_all_from_uwvm/`

## Command Table

| Command | Alias | Arguments | Availability | Behavior |
| --- | --- | --- | --- | --- |
| `--runtime-aot` | `-Raot` | None | LLVM backend | Select full-module LLVM AOT compilation. |
| `--runtime-int` | `-Rint` | None | Interpreter-enabled builds | Select the interpreter with the same host policy. |
| `--runtime-compiler-log` | `-Rclog` | `[out\|err\|file <path>]` | Runtime backend support | Route compiler diagnostics. |
| `--runtime-compile-threads` | `-Rct` | `[default\|aggressive\|<count>]` | Runtime backend support | Configure full-module translation workers. |
| `--runtime-scheduling-policy` | `-Rsp` | `[func_count <count>\|code_size <bytes>]` | Runtime backend support | Split full-module translation work. |
| `--runtime-llvm-jit-policy` | `-Rllvm-policy` | `[debug\|default\|fast-compile\|balanced\|max]` | LLVM backend | Select an AOT optimization preset. |
| `--runtime-llvm-jit-full-policy` | `-Rllvm-full-policy` | `[auto\|debug\|legacy-light\|pb-o1\|pb-o2\|pb-o3]` | LLVM backend | Select the concrete full-module optimizer pipeline. |
| `--runtime-llvm-jit-cache-path` | `-Rllvm-cache-path` | `[disable\|default\|path <path>]` | LLVM backend | Configure the AOT object cache. |

The independent WASI Threads execution profile has its own backend admission.
Its LLVM implementation is currently limited to x86_64; ordinary non-threaded
interpreter/AOT results do not qualify threaded AOT on other architectures.

## Runtime Selection

`runtime_mode_t` contains only `full_compile`. Runtime selection therefore chooses a backend, not a compilation lifecycle:

| Shortcut | Backend | Preparation rule |
| --- | --- | --- |
| `--runtime-aot` | LLVM AOT | Generate, optimize, and materialize the complete module before entering it. |
| `--runtime-int` | Interpreter | Translate the module for interpreter execution. |

The ROS build requires the LLVM AOT backend. Runtime shortcuts reject duplicate or conflicting selections.

Examples:

```bash
uwvm2-ros --runtime-aot --run app.wasm
```

Removed commands such as `--runtime-jit`, `--runtime-tiered`, `--runtime-custom-mode`, and `--runtime-custom-compiler` are command-line errors rather than aliases for AOT.

## Compiler Log

```bash
uwvm2-ros --runtime-compiler-log err --runtime-aot --run app.wasm
uwvm2-ros --runtime-compiler-log file compiler.log --runtime-aot --run app.wasm
```

The compiler log is independent of the main diagnostic destination selected by `--log-output`. The file form requires the literal `file` argument.

## LLVM AOT Policies

The retained high-level presets are:

- `debug`: no LLVM optimization.
- `default`: use the normal full-module default.
- `fast-compile`: favor compilation latency.
- `balanced`: use the tuned PassBuilder O1 path.
- `max`: use the tuned PassBuilder O3 path and aggressive code generation.

The scoped full-module policies map directly to `auto`, `debug`, `legacy-light`, `pb-o1`, `pb-o2`, and `pb-o3`. The high-level and scoped policy commands are mutually exclusive.

```bash
uwvm2-ros --runtime-aot --runtime-llvm-jit-policy max --run app.wasm
uwvm2-ros --runtime-aot --runtime-llvm-jit-full-policy pb-o2 --run app.wasm
```

All generated Wasm functions carry LLVM `NoInline`, including under `max`/`pb-o3`. Optimizers may still transform code inside each function, but they do not merge one Wasm function body into another. This keeps patchable AOT boundaries and native stack identities stable.

## LLVM AOT Call Stacks

Native unwind and crash stack recovery are mandatory. The former
`--runtime-llvm-jit-call-stack` / `-Rllvm-call-stack` option is removed; every old
value is a command-line error. No unchecked or instruction fallback is exposed.

Each executing process compiles and runs a real recursive generated chain using
its actual target ABI and paired unwinder. A failed witness stops execution.
Cache reuse does not authorize skipping this process-local check. Native
measurements record `elapsed_ns`; 10 ms is an optimization target, never a
forced timeout or a reason to accept an incomplete witness. QEMU establishes
correctness and ABI behavior, not native startup performance.

The runtime uses repository-owned LLVM libunwind headers and matching paired
libc++/libc++abi/libunwind archives. Native CFI preserves recursive frame
multiplicity without per-function logical stack push/pop operations. It does
not scan arbitrary stack memory or manufacture frames from guessed code ranges.
Compiler diagnostics report `call_stack=unwind`, `unwind_check=live`, and omitted
logical frames; a crash test must verify the complete ordered Wasm chain.

## LLVM IR Verification and Cache

LLVM IR verification is mandatory in this reduced runtime and cannot be disabled from the command line. This is independent of the earlier Wasm validation step; both must succeed.

The full-module backend can read and write native object-cache entries. Cache path selection is `default`, `disable`, or `path <directory>`. When caching is enabled, every newly written object is signed and every loaded object must have a valid signature. There are no command-line overrides for signing or verification.

Persistent native-object caching requires a clean Git commit identity or a verified complete-source manifest ID. Source-archive packagers can pass `--build-source-id=sha256:<64 lowercase hex digits>`; the value must be regenerated whenever any packaged source input changes. Official release archives should carry the stable hash of a normalized, complete source manifest rather than a tag, version string, timestamp, path, or build artifact.

Dirty Git worktrees and builds with neither identity fail closed even when an explicit cache path is requested. A Wasm-derived LLVM module hash does not cover host bridge/runtime/unwind semantics, and the cache signature provides integrity and context binding rather than source provenance. Developers may separately accept these risks with `UWVM2_ALLOW_UNSAFE_DIRTY_LLVM_JIT_CACHE` or `UWVM2_ALLOW_UNSAFE_UNPROVENANCED_LLVM_JIT_CACHE`; release builds must not define either macro.

The signing identity is deterministically derived from the local execution context. It detects damaged entries, incompatible-context substitution, and accidental cache reuse; it is not a secret-key trust boundary and does not protect against an attacker who can act as the same OS account. Put the cache in an account-private directory, and use `disable` whenever that ownership boundary is not sufficient.

```bash
uwvm2-ros --runtime-aot --runtime-llvm-jit-cache-path default --run app.wasm
uwvm2-ros --runtime-aot --runtime-llvm-jit-cache-path path ./uwvm-cache --run app.wasm
```

## Compile Threads and Scheduling

`--runtime-compile-threads` accepts:

- `default`: an adaptive worker count based on hardware concurrency.
- `aggressive`: roughly two thirds of detected hardware threads.
- `0`: serial translation.
- positive `N`: request exactly `N` extra workers, subject to useful task counts.
- negative `-K`: detected hardware threads minus `K`.

`--runtime-scheduling-policy` groups full-module tasks either by function count or cumulative Wasm code-body bytes. The default is `code_size 4096`. Worker counts can be reduced when the module does not contain enough useful task groups.

```bash
uwvm \
  --runtime-aot \
  --runtime-compile-threads aggressive \
  --runtime-scheduling-policy code_size 8192 \
  --run app.wasm

uwvm \
  --runtime-int \
  --runtime-compile-threads 4 \
  --runtime-scheduling-policy func_count 16 \
  --run app.wasm
```

## MTE execution and independent violation reports

WasmMTE exposes five controls: parser disable, runtime disable, backend, timing
and violation action. Backend and timing remain independent:
`--runtime-memtag-backend software|hardware` and
`--runtime-memtag-mode sync|async`. Defaults are **software + sync + crash**.

The action grammar is:

```text
--runtime-memtag-action crash
--runtime-memtag-action silent-log out
--runtime-memtag-action silent-log err
--runtime-memtag-action silent-log path <PATH>
```

`silent-log` records every detected tag violation and continues the access.
A destination is mandatory. `out` and `err` own separate duplicates of stdout
and stderr; `path` opens a dedicated host file and appends complete JSON records.
This report stream has its own handle and serialization mutex. `--log-output`
controls ordinary UWVM diagnostics and never rebinds the MTE stream, regardless
of option order. Explicitly selecting the same stdout/stderr endpoint for both
streams naturally puts both kinds of output on that endpoint.

Ordinary sandbox bounds, owner identity and host permissions remain enforced.
Opening a report destination can fail at configuration time (exit126); a report
write failure is a controlled runtime error (exit123), never silent record loss.
The report descriptor is not inserted into a guest FD table or granted as a
WASI/ROS capability. Crash mode opens no report stream.

Use `--wasm-feature-disable-memtag-extension` to reject the extension at parse
validation, or `--runtime-disable-memtag` to disable execution while retaining
required zeroing. Unsupported explicit hardware selection is a launch error.
Hardware + silent-log requires a supported host but uses precise software
checking to enumerate all violations and safely continue, rather than claiming
physical fault replay or hardware diagnostic acceleration.

Old `silent`/`log` values, misspellings, mixed backend/timing values, worker
controls and dedicated report-output switches are rejected. There are no
compatibility aliases. See the [extension contract](../runtime/memtag-extension.md).
