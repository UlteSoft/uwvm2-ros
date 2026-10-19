# Explicit Wasm libraries and guest callbacks

These sources demonstrate a **main → motion → callback** function-import chain,
a separately preloaded vision computation, and root-only SDK access. The C and
C++ variants are complete entry programs. Rust and AssemblyScript supply
alternative scalar vision modules. TinyGo supplies a conditional reactor
source with an explicit initialization adapter. The additive `*_guest.mjs`
variants compile the pinned embedded engine and their actual JavaScript logic
into separately preloaded Wasm libraries; the original external SDK host path
remains separate.

Everything here is **SOURCE_ONLY / NOT_EXECUTED** until the actual compiler,
validator, Worker and independent native observations have been retained. A
recipe or exported function is not evidence of a working robot, simulator,
ROS connection, callback delivery, performance gain or compatibility with a
particular toolchain. The [scope record](GAPS_AND_SCOPE.md) lists the source
anchors and deliberate limits.

## Process and memory boundaries

One Worker process instantiates four Wasm modules, named exactly `callbacks`,
`vision`, `motion`, and `main`. Each compiler-generated module owns its own
linear memory. Imports pass only `i32` scalars; no pointer, SDK handle, native
function pointer, C++ object or managed-language object crosses a module.
The Runtime's named import resolver calls the selected function in its owning
instance. The modules are libraries inside one process, not four OS processes.

For the positive ROS extension, retain the separate processes: Worker,
Gateway, independent ROS participant, and the ROS1 Master where applicable.
The supervisor controls their actual identities, cgroup and lifetime. This
example does not collapse them into a shared address space, replace IPC with a
direct native call, or authorize a library by lending it main's ROS handle.
The local graph is not a replacement for a distributed vision/motion pipeline;
separate Worker processes must communicate through separately reviewed real
ROS capabilities, not cross-process Wasm pointers.

`--wasm-preload-library PATH NAME` is the actual loader syntax. Preload
`callbacks`, then `vision`, then `motion`, and run main with
`--wasm-set-main-module-name main`. The C/C++ libraries have no Wasm start
section or libc initializer. Actual Wasm start sections run during graph
startup; a library export merely named `_start` or `_initialize` does not
become an automatic initializer. Main explicitly calls `vision_initialize`
and `callback_begin` after graph startup.

## Callback ownership and refusal

`callback_begin(0x4d4f544e)` returns a nonzero generation. Main passes the pair
to motion, which imports `callbacks.on_motion`. A generation is active until
the matching checked `callback_end`. Reopening advances it; it never wraps.
Wrong owner tags, inactive generations and stale generations are refused with
the sentinel `INT32_MIN+9`. Invalid numerical inputs use `INT32_MIN+22`.
Valid commands are in `[-100,100]`, so these errors cannot look like commands.
The ordinary lesson checks two callbacks, refusal without count mutation,
close, stale use, reopen, one further callback, and checked retirement.

This owner tag/generation is **application lifetime bookkeeping**. It does not
prove the Runtime caller's identity and is not an unforgeable authorization
token. A module importing the exports can call them. Runtime ROS authorization
separately uses the trusted instance plus Runtime generation and rejects ROS
host calls under *any* cross-module ancestry. An explicit library grant does
not delegate authority along a Wasm import chain.

`callback_ros_denied.c` is an explicitly selected negative variant. Its accepted
callback path tries the actual selected ROS `capability_open` import, requires
13, and checks all eight output bytes remain `0xa5`. Build main with
`ROBOT_EXPECT_CHAIN_DENIALS=1`; it requires exactly three such refusals. Default
denial also follows from the callback module's empty policy. To distinguish the
strong ancestry rule, an independent negative test may explicitly configure
the callback module's own identical alias/connection, then still require 13
when reached through main→motion→callback. That is a different reviewed
profile, requires a live Gateway for preflight, and grants nothing implicitly.
Never invoke that callback as root and then call its direct-root authority a
cross-module test. Guest refusal alone does not establish native-effect or
audit counters; preserve the independent native and trusted-host evidence.

The optional `funcref_callback.wat` / `funcref_main.wat` graph additionally puts
the callback adapter's actual `funcref` into main's table. Provider and main
have different functions at index2; losing the owning instance produces a
wrong result. Its start section checks a successful call and a stale call.
It has no host IO or ROS imports. This demonstrates function-reference identity,
not a native ROS thread calling into guest code. Native ROS callbacks still
enqueue; SDK executors poll and call guest handlers on the calling thread.

## Real SDK root programs

Build `main.c` or `main.cpp` with exactly `ROSWASM_EXAMPLE_ROS_VERSION=1` or `2`.
The C program uses the existing allocation-free Node and String codec helpers.
The C++ program uses the actual `roswasmcpp::Node` and typed String Publisher.
Both finish the imported graph before making a ROS host call, so no delegated
library stack is involved. No module receives another instance's capability.
The graph commits the actual imported decision into main's own output variable
only after successful checked retirement. This fixed vector must return60
before main may publish the corresponding fixed String; it is not a general
numeric-text formatter or a substitute for the Unitree message codecs.

`ROBOT_MODULAR_PUBLISH=0` is the default. It checks ABI1, actual denied open13,
the untouched output sentinel, and writes exactly
`PASS_MODULAR_DEFAULT_DENIED\n` before exit0. This local refusal path needs no
Gateway and makes no native-delivery claim. A nonzero exit or unexpected marker
fails the lesson; do not treat13 itself as an implemented ROS operation.

`ROBOT_MODULAR_PUBLISH=1` is a separate compile selection. Main alone opens
alias `modular-out`, publishes the exact String `robotics-modular:warmup`,
writes `MODULAR_ENDPOINT_CREATED\n`, and waits for exactly one stdin byte `P`.
The supervisor supplies P **only after the independent subscriber actually
matches** the Gateway publisher. Main then publishes exactly one String
`robotics-modular:command=60`, checks close, writes `MODULAR_PUBLISHED\n`, and
exits0. Writes/reads check errno and exact count; EOF/short IO is an error,
with no implicit retry. The host controls deadlines; the guest reads no clock,
randomness, environment or CLI arguments. There is no motor-command publisher.

The independent peer must check the final String once, warmup zero or once,
unexpected messages zero, matching native type/identity/QoS and a host-measured
quiet interval after checked shutdown. `MODULAR_PUBLISHED` means the actual SDK
publication was acknowledged, **not** that a native subscriber received it.
The Gateway/peer launch and physical Unitree/simulator wiring are separate
acceptance work; these examples do not fabricate their implementation.

## Explicit build and launch data

[recipes.json](recipes.json) contains 28 compiler/assembler templates. It is
data, not an executable success report. Every `{...}` value must be replaced by
a reviewed physical input or finite value; reject remaining placeholders.
Use a fresh owned output, immutable source/SDK copies, explicit installed tools,
one compiler at a time, finite600s command deadlines and actual512MiB compiler
tree limits. Retain raw stdout/stderr, exit code, complete dependency/tool/SDK
catalogs and before/after source pins. The outer provided Linux cgroup must be
the actual64GiB/swap0 leaf with its identity and reserve preserved; a matching
number in JSON does not prove containment. No agent has run these recipes.

C libraries and C main use freestanding Clang wasm32 with bounded memory and
no libc. C++ libraries are likewise freestanding. C++ main uses an explicitly
selected WASIp1 sysroot/libc++ with `-nostartfiles`, custom `_start`, no RTTI or
exceptions. The six WASIp1 `main-cpp-*` recipes also require the explicit
[SDK33 C++ termination object](cpp_termination/README.md), prepared from the
locked unchanged official source. Bind `CPP_TERMINATION_OBJECT` only after
actual compile/full-MD qualification; there is no default SDK diagnostic-stdio
fallback. The Guest TUs retain `-UNDEBUG` and the header-only verbose trap;
only the separate official no-assert TU uses its documented private NDEBUG
policy. Actual link trace/why-extract and strict binary-import checks remain
mandatory. Do not link a second CRT `_start`. `--allow-undefined` permits the
annotated import graph at link time; the subsequent **closed actual binary
import/signature inspection** must reject every unexpected namespace/function.
It is not permission to fabricate an `env` resolver. The root SDK backend table
may retain additional selected-version ABI imports; only the explicit alias
can authorize an operation. Compare every retained signature to
`sdk/abi/abi-v1.json`. Do not enable the other ROS version to make a build pass.

For every actual Wasm artifact run the full validator and inspect imports,
exports, memory limits/start section and tool-generated startup. The JSON gives
the exact `wasm-tools validate ARTIFACT` template and role contracts. Compiler
success and validation remain distinct from Runtime/native behavior. Pin the
whole installed SDK layout when copying: public forwarding headers depend on
`sdk/common`, `sdk/ros1` and `sdk/ros2`, as well as `sdk/include`.

[launch-templates.json](launch-templates.json) supplies exact structured argv
for both ROS versions and complete explicit String grant/limit data. It starts
with Global ROS/WASI disabled and creates each named Single target. Only main
enables basic WASIp1 stdio/process exit; all eight additional permission bits
are explicitly denied, host API hidden, environment inheritance disabled,
fd-limit3 and forced argc0. There are no mounts or sockets. Pure preloads keep
WASI and ROS disabled. Positive ROS replaces main's disabled target with its
own explicit connection/grant; it does not turn on Global or library rights.

Replace all connection placeholders with actual Worker PID/UID, Gateway UID,
private socket, selected ROS1 Master/advertise address or ROS2 domain/RMW/
security/keystore/enclave. Review Enforce or an explicitly admitted test-security
profile; there is no automatic security downgrade. Policy and Gateway manifest
must agree on native String identity and every limit. Run the production policy
check before admitting a positive Guest. The native feature/type provider of
the selected ROS distribution must actually support the contract; distribution
names or headers alone do not establish that. No optional clock/random/
filesystem permission is granted on a compiler or startup failure.

## Additional languages and initialization

| Language | Source and actual composition scope |
| --- | --- |
| C | All three library providers, root Node/typed-codec program, negative callback, benchmark root |
| C++ | All three providers and root C++ Node/typed Publisher; C linkage keeps the module ABI scalar |
| Rust | `vision.rs`: no_std/allocator-free vision replacement, wasm32-unknown-unknown; no claimed Rust Node factory coverage |
| AssemblyScript | `vision.ts`: integer-only vision replacement, local abort trap, bounded stub runtime; no managed value crosses modules |
| TinyGo | `tinygo/vision.go`, the explicit v0.43.0 pure target and `tinygo_adapter.wat`: actual reactor/export inspection and explicit `_initialize`; startup must first qualify at zero rights |
| JavaScript | `vision_guest.mjs`, `callback_guest.mjs`, `motion_guest.mjs`: actual closed-source embedded-engine reactors; fixed/session JS benchmark roots use real scalar Wasm imports. Existing external SDK/bridge remains separate. Actual builds/execution require their own evidence. |

The Rust vision build requires the actual matching target core/sysroot and
linker. AssemblyScript `--use abort=.../vision/abortGuest` binds the local trap
instead of assuming an `env.abort` host. TinyGo's **reactor-only** recipe uses
[`tinygo/wasm-unknown-v0.43.0-bounded.json`](tinygo/wasm-unknown-v0.43.0-bounded.json)
with `-buildmode=c-shared -scheduler=none -opt=1 -p 1`. It does not use the
command/WASIp1 target. The complete TinyGo and Go installations remain explicit
immutable compiler inputs; `TINYGOROOT` resolves the target's compiler-rt,
wasmbuiltins and `src/runtime/asm_tinygowasm.S` inside that installed package.
The custom JSON is self-contained: it does not inherit another target. Do not
replace it with the Go-variable `-ldflags` option or mutate an installed target.

This target copies the field values of the official TinyGo **v0.43.0**
[`targets/wasm-unknown.json`](https://github.com/tinygo-org/tinygo/blob/v0.43.0/targets/wasm-unknown.json),
appending only `--max-memory=67108864` to its existing linker flags. Formatting
is project-local. Its `goos=linux`/`goarch=arm` fields are TinyGo's upstream
compiler configuration; they do not label the resulting Wasm as a native ARM
binary. The retained `emulator` string is upstream metadata and is never
executed by this `tinygo build` recipe. This is one explicit supported compiler
profile, not a claim about every TinyGo release. Its provenance/license notice
is in [`tinygo/LICENSE.reactor-profile`](tinygo/LICENSE.reactor-profile).
TinyGo's [actual target loader](https://github.com/tinygo-org/tinygo/blob/v0.43.0/compileopts/target.go)
accepts the absolute `.json` path used by the expanded recipe.

Inspect the real artifact with the selected full validator: imports must be
empty, no Wasm start section or `_start` may exist, `_initialize` must be a
zero-argument/zero-result function, `vision_clearance_mm` must be exactly four
i32 arguments returning i32, and one unimported memory must have maximum1024
64KiB pages. A compile, source profile or another codec's successful build does
not establish this vision artifact's startup. The zero-right preload remains
**unqualified** until its actual initialization is observed. A clock, entropy,
filesystem or other unexpected import refuses this variant; retain the failure
and use a separately qualified provider. Never auto-grant or turn refusal into
success. Compiler commands remain512MiB/600seconds, serial `-p 1`, with
`GOMAXPROCS=1`/`GOMEMLIMIT=192MiB`; the Go heap hint is not the native LLVM RSS cap.
The adapter explicitly calls `_initialize` once after graph startup; exporting
an initializer does not execute it. Command examples retain their original
startup target and are not changed by this reactor profile.

Primary compiler references:
[TinyGo reactor example](https://github.com/tinygo-org/tinygo/blob/dev/README.md),
[TinyGo Wasm exports](https://tinygo.org/docs/guides/webassembly/wasm/), and
[AssemblyScript compiler/runtime options](https://www.assemblyscript.org/compiler.html).
Use the exact installed tool package and retained bytes for an actual build;
these documentation links are not package-version evidence.

## Matched synthetic computation for profiling

`robotics_math.h` is portable C/C++ with no IO, Wasm or ROS dependency. It is
the shared bounded-integer oracle. It generates four synthetic distances and a
requested command with a specified uint32 LCG; nearest-distance gating returns
the requested command for clearance>=400mm, otherwise0. All unsigned overflow
is intentional modulo arithmetic. `robot_math_batch(seed,iterations)` returns
the uint64 checksum for1..1000000 iterations; invalid bounds returnUINT64_MAX.

`benchmark_main.c` performs the same sample/checksum sequence while actually
calling the separate vision module and motion→callback chain every iteration.
The pure callback provider is mandatory. `ROBOT_BENCHMARK_SEED` (default1) and
`ROBOT_BENCHMARK_ITERATIONS` (default10000) are compile-time artifact inputs;
the exact chosen values must match the independent native fixture. Stdout is
exactly eight little-endian checksum bytes followed by exit0; any nonzero exit,
missing/extra byte or checksum mismatch is failure. It uses basic stdout and
proc_exit only, with no guest clock or ROS import. All native timing, repeated
sampling, hardware counters and hotspot attribution belong to the external
supervisor/profiler, with actual tool/CPU/permissions evidence.

A native fused `robot_math_batch` baseline performs the same numerical work
without Wasm cross-module dispatch or registration. Label this distinction in
every report; it cannot establish native ROS equivalence, a speedup in physical
control, image inference, simulator dynamics, real Unitree middleware, or a
hardware bottleneck. Compare checksums before using time/counter measurements.
Do not infer VTune hardware-counter results from source or wall-clock timing.
Trap/abort is terminal failure: checked normal cleanup does not promise to run
after a trap, and the supervisor must still prove owned-process cleanup.

The separate `benchmark_session_main.c` keeps one actual Worker/module graph
alive. After `READY\n`, each single-byte `W` or `P` runs the identical reset-seed
batch, including a fresh checked callback generation, and emits exactly8LE
checksum bytes. `Q` exits0. The protocol allows at most256 command bytes; EOF,
an invalid byte, short IO or exhaustion without Q fails. Vision initializes
once before READY. The host sends explicit W warmups, then measures P-write to
complete8-byte receipt, checks every checksum and finally Q/exit0. Match the
native session's framing, input values and timing boundaries. This moves Worker
instantiation outside measured intervals, while retaining protocol/dispatch/
registration work inside; it does not measure an isolated arithmetic kernel.
The supervisor still owns an actual deadline for blocking stdin. Neither
session nor one-shot entry has been run during authoring.

## Embedded JavaScript provider and root alternatives

The [embedded engine guide](../../javascript/guest/README.md) and
[`recipes.json`](../../javascript/guest/recipes.json) select `reactor-vision`,
`reactor-callback`, `reactor-motion`, `benchmark_guest` and
`benchmark_session_guest`. These are additive recipes, not changes to the C
recipe inventory. Each provider's JS functions are called by real compiled
scalar exports. Motion's JS calls `roswasm:library_imports.on_motion`, whose
C trampoline enters the actual `callbacks.on_motion` Wasm import. Each engine
owns its separate heap, closed source table and JS callback lifetime. No host
Node callback or JS object crosses module boundaries.

Preload the resulting callback, vision and motion artifacts under the exact
names `callbacks`, `vision`, `motion`, then select the chosen root artifact.
The pure backend-zero providers must have zero ROS and WASI imports; motion
has only its declared scalar callback import. They need no filesystem, clock,
randomness, environment, socket or ROS grant. Each declared first-call
initializer is explicit; an export name alone never means automatic startup.
The callback generation/owner semantics match the scalar lesson above and
remain application bookkeeping. Never lend main's ROS handles to a library.

The JS benchmark uses seed1 and10000 iterations, exact modulo integer math and
uint64 BigInt checksum. Fixed mode emits exactly8LE bytes then exits0. Session
mode has the same `READY\n`, W/P eight-byte reply, Q and256-command bound as
the C session. Compare the independent checksum before timing. Retain warmup,
process-start and warmed-instance scopes separately. Interpretation, JS value
conversion and cross-module engine dispatch are included; no native speed,
Unitree controller, simulator dynamics or vision inference claim follows.
The JS benchmark command currently retains the selected full ROS dispatcher
despite making no ROS call; it is not a zero-ROS-import root. All provider ROS
permissions remain empty and all actual type/import/runtime gates still apply.
