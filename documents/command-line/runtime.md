# Runtime Commands

The uwvm2-ros product runs on its qualified Linux GNU targets and uses eager,
full-module LLVM AOT compilation. `--runtime-aot` (`-Raot`) selects that backend;
it is also the product default. Native EH, CFI, the real per-process generated
unwind witness and crash stack recovery are mandatory and cannot be disabled.

Interpreter-enabled builds also accept `--runtime-int` (`-Rint`). Both backends
use the same host namespace, exact interface and operation/resource policy.
Live debugging requires LLVM AOT. The interpreter rejects `-Rdbg`,
`--runtime-debug`, `-m debug` and `--mode debug` at every debugger permission
level. It exits with status 126 before starting the guest. An inherited debugger
control endpoint also requires LLVM AOT, including with `--wasi-threads`.
LLVM AOT rejects unsupported lowering instead of silently falling back to the
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
| `--runtime-llvm-jit-cache-path` | `-Rllvm-cache-path` | `[disable\|default\|path <path>]` | LLVM backend | Configure the AOT object cache. |

The independent WASI Threads execution profile has its own backend admission.
Its LLVM implementation is currently limited to x86_64; ordinary non-threaded
interpreter/AOT results do not qualify threaded AOT on other architectures.

## Linear memory reservations

Use `--runtime-memory-reservation` before `--run`. Sizes are decimal bytes.
The process pool accounts for mappings, including inaccessible guard pages.
The initial committed data size comes from the memory's minimum in Wasm pages.

```sh
uwvm2-ros --runtime-memory-reservation pool 17592186044416 --run app.wasm
uwvm2-ros --runtime-memory-reservation participants 300 --run app.wasm
uwvm2-ros --runtime-memory-reservation memory app 0 16777216 --run app.wasm
```

The default pool is 16 TiB on 64-bit hosts and 2 GiB on 32-bit hosts. On a
64-bit host the pool accepts the inclusive endpoint `18446744073709551616`.
The OS must still be able to map each selected window. Each local memory
definition has one owner. All its imports and re-exports share the same
reservation. A stable participant count allows for future owners when
calculating the common power-of-two share. The `memory` selector uses the
defining module name and a zero-based index among its local memory definitions.

With a full-permission LLVM debugger and automatic participant sizing, the
initial graph can use up to half the free pool. The paused graph keeps its
mappings while `dump restore` prepares its replacement. On i686 with the
default 2 GiB pool and one debugger session, each graph has a 1 GiB budget
across all its owners and guards, including on subsequent restores. Other
live mappings reduce the budget available to the pair. Initialization fails
if the required data quotas and minimum guard windows exceed a graph's budget.
An explicit `participants` count selects the participant-based plan. The private
WASI Threads driver and sessions with preloaded native drivers use the ordinary
plan.

On a 64-bit host an ordinary memory32 owner keeps a fixed full guard window.
A manual reservation for that owner is an initialization error. A memory32
owner with ignored address bits can use a manual quota. For memory64, request a
power-of-two data quota. A request above a declared maximum is clipped with a
warning. A quota below a finite declared maximum also produces a warning. The
planner must cover every declared minimum, the per-owner guard and the host
mapping floor. For ADM it prefers a full protection window even for a small
manual data quota. A partial window preserves the accepted data quota when the
process pool or OS cannot provide the full window. Use `--verbose` to see
each owner's data quota and mapping size.

## Passing pointers between modules with different MTE compiler settings

An instrumented producer can pass an object pointer to a separately compiled
ordinary consumer. Both modules must address the same memory instance with the
same pointer width. The call, return, or pointer-sized slot used to transfer
the pointer carries all its bits. A consumer with comparisons enabled checks its tag against the owning memory when it dereferences the pointer. The consumer can use its
ordinary load and store instructions for this purpose.

A protocol that transmits a linear byte offset discards any pointer tag. For
example, a consumer that independently computes offset `0x1000` gets a
pointer with tag zero, even if its producer allocated a nonzero-tagged object
there. On a checked owner the consumer can dereference that offset only when
the corresponding granule has tag zero. A bare-offset interface needs storage
kept at tag zero for the entire exposure period, including every granule of the
buffer. A pointer-passing interface can instead carry the complete pointer
produced for the current lifetime of the object.

Use `runtime.memtag-checks off` on a module to skip its ordinary tag comparisons. Tag queries, allocation operations, zeroing, address masking and bounds checks still run. Other modules can use checked accesses to the same software owner. [Module policies](wasm-module-policy.md)

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

The scoped full-module policies map directly to `auto`, `debug`, `legacy-light`, `pb-o1`, `pb-o2`, and `pb-o3`. A concrete full policy takes precedence over the high-level preset.

```bash
uwvm2-ros --runtime-aot --wasm-policy-global runtime.llvm.policy max --run app.wasm
uwvm2-ros --runtime-aot --wasm-policy-global runtime.llvm.full-policy pb-o2 --run app.wasm
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

Use `runtime.memtag-backend` on the defining module or `--wasm-policy-memory-backend` on one local memory. Available values are `software`, `auto`, `arm-mte` and `x86-mtt`. Explicit hardware selection requires a supported checked mapping. Set process delivery with `--runtime-memtag-mode sync|async`. The defaults are **software, sync and crash**.
[Module and individual owner overrides](wasm-module-policy.md)

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

Use `--wasm-policy-global parser.memtag off` to reject WasmMTE during validation.
`--wasm-policy-global runtime.memtag-checks off` sets the global default for ordinary access comparisons
to off. The owner still supports tag operations, address masking, bounds checks
and required zeroing. Explicit hardware selection requires a real checked
mapping. Select software or auto for precise, continuing silent-log reports.

Old `silent`/`log` values, misspellings, mixed backend/timing values, worker
controls and dedicated report-output switches are rejected. There are no
compatibility aliases. See the [extension contract](../runtime/memtag-extension.md).
