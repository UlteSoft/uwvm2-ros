# uwvm2-ros operator and integrator whitepaper

Debugger permission levels are `full`, `read-only`, `limited` and `none`, selected only at launch. `-Rdbg` defaults to `full`; other launches default to `limited`. See [Debugger permissions](command-line/debug-permissions.md) for dump redaction, independent update authority and IDE negotiation.

## Purpose and qualification

uwvm2-ros runs robot application logic as WebAssembly with explicit host
authority. Native ROS Gateways remain the controlled middleware boundary.
A compiled host interface is neither an exposed guest import nor permission
to perform an operation. The same distinction applies to WASI sockets,
filesystem access, clocks and random input.

This whitepaper describes the current implementation and its operational
boundaries. It is not a certification of every SDK language, robot, simulator
or platform combination. Component tests, complete-VM tests, real IDE sessions,
native performance measurements and physical hardware tests establish different
evidence. QEMU execution establishes target behavior under emulation; it does
not measure native performance or prove a physical MTE processor's throughput.
The [MTE qualification record](runtime/memtag-extension.md), [native language
debugging record](runtime/debug-native-languages.md), and [production update
contract](runtime/production-update.md) retain their explicit limitations.

The current ROS product admits five Linux GNU ABIs: x86_64, i686, AArch64,
ARMv7 soft-float (arm), and ARMv7 hard-float (armhf). Other platforms retained
in upstream source are outside this product contract. Complete behavioral
parity must be assessed using matching source, compiler and runtime receipts,
rather than inferred from successful cross compilation.

## Build and dependency ownership

Use the [Linux build guide](how-to-build/linux.md) and
[Xmake option reference](xmake-options.md). Reuse installed compatible
toolchains; source configuration is not a request to reinstall them.
The host compiler needs the project's C++26 support. LLVM-enabled configuration
also needs CMake and Ninja.

The executable target is `uwvm2-ros`. A configuration retaining both execution
engines is:

```sh
xmake f -y -c -m release -p linux -a x86_64 -o build/ros \
  --use-llvm-compiler=y --use-cxx-module=n --march=none \
  --execution-int=uwvm-int --execution-jit=llvm \
  --llvm-build-jobs=1 --llvm-build-targets=Native \
  --import-ros1=y --import-ros2=y
xmake -j1 uwvm2-ros
xmake run uwvm2-ros --version
```

The mandatory [product gate](../xmake/utility/ros_product.lua) rejects removal
of WASI32/64, ROS1, ROS2 and socket implementations. It also requires the
LLVM execution backend, native unwind path, source-owned paired libc++ and
libunwind, thread-local storage, exceptions, WSEA and the mmap memory backend.
The interpreter can be omitted with `--execution-int=none`. Raw compiler flags
cannot override protected product contracts. A rejected configuration is not
silently converted into a reduced product.

The bundled runtime compiler is pinned LLVM 23.1.1 with reviewed downstream
changes. System `llvm-config` and `LLVM_CONFIG` do not replace it. Compiler
dependency information comes from CMake. Cross builds need a matching target
triple, sysroot, ABI, native libraries and `--llvm-cmake-toolchain` declaring
`LLVM_HOST_TRIPLE`. Build the LLVM code generator for the machine on which
the VM will execute. See [LLVM provenance and cross builds](../third-parties/llvm/README.md).

Limit aggregate build and test resources at the operating-system boundary.
`--llvm-build-jobs` and `xmake -j` limit parallelism, not process-tree memory.
Sanitizer and cross-platform qualification should use their own output
directories and reuse existing dependencies. Test artifacts, private keys,
credentials and transient compiler outputs do not belong in a source release.

## Command discovery and argument boundaries

`uwvm2-ros --help` lists command categories; `--help all` prints all advertised
commands. The [complete CLI catalog](command-line/cli-catalog.md) gives each
command's usage, aliases and description. It is generated from a real binary.
The [topic reference](command-line/README.md) documents argument parsing and
interactions. Build switches are separate from runtime commands.

`--help debug` and `--help update` are separate categories. WASI, ROS1, ROS2,
Wasm features, runtime execution, logging and secure loading each have a help
category. `--secure help` explains the secure-loading subcommands.
`--version` reports the binary's build and backend information.

Put every host option before `--run`. After `--run MODULE.wasm`, all remaining
tokens are guest arguments, including tokens that look like host options.
`-r` and `--` are equivalent boundaries. An implicit trailing module path is
accepted, but an explicit boundary makes deployment commands easier to review.
`--wasm-set-main-module-name` chooses the policy identity; changing guest argv[0]
does not rename that identity.

`--mode validation` validates module imports and signatures without running a
guest start function. Explicit preopen configuration can still open an approved
host directory during command-line parsing. Validation is therefore not a
promise that an arbitrary host command line has no effects.

## Permission construction

A deployment must state all three interface conditions explicitly:

1. **hidden**: the import is absent and linking/validation rejects a module
   requiring it. This is the initial state.
2. **deny**: the exact ABI is exposed so the module can link, but calls are
   refused with an interface-specific diagnostic.
3. **allow**: the interface may be called, subject to operation grants, actual
   descriptor or ROS object rights, namespace policy, bounds checks and quotas.

Creating and enabling a namespace grants no operations. Exposure does not
create stdin, a directory, a socket or a ROS node. An operation grant does not
make hidden imports visible. Every imported API needs an exact interface state,
even if the guest never calls it at runtime.

For a module that imports only `fd_write` and writes to stdout:

```sh
uwvm2-ros --wasm-set-main-module-name main \
  --wasip1-single-create main --wasip1-single-enable main \
  --wasip1-single-interface main fd_write allow \
  --wasip1-single-grant main stdout.write \
  --run stdout-only.wasm
```

This command is deliberately specific to that import set. A libc program may
also import arguments, environment, metadata, clocks or `proc_exit`; those
imports require their own states and any operations they actually use require
their own grants. SDK test launchers enumerate the compiled module's imports;
they do not establish a production deployment policy.

Single policies bind exact module names. Group policies share only the resources
explicitly assigned to their members. ROS Global contracts require explicit
module bindings; there is no authority fallback for an unknown module.
Preloaded modules and imported memory aliases do not inherit another module's
permissions merely because they share a process or memory. Consult
[host permissions](command-line/host-permissions.md), [WASI](command-line/wasi.md),
[ROS1](command-line/ros1.md) and [ROS2](command-line/ros2.md) for exact API,
resource, group and operation configuration.

The host may preload a Wasm library or explicitly trust a native dynamic library.
A native library executes host code and is outside Wasm memory isolation.
It must not be presented as an untrusted guest merely because it participates
in the module graph.

## Limits, waiting and process exit

Granted WASI/ROS operations have no additional traffic quota unless the host
sets one. The independent finite WASI-thread resource contract is explicit:
`--wasi-threads` specifies active/spawn counts, guest/compiler stack storage,
per-child storage/code and shared-memory capacity. Granting a thread import
does not silently create those resources.

For operation quotas, `reject` returns the documented error before the
refused host effect. `wait` synchronously waits for replenishable window or
concurrency capacity. There is no asynchronous quota execution model.
Request and lifetime ceilings cannot replenish and do not accept wait.
Limits do not grant operations and a guest cannot erase the host's ledger
by creating another module or requesting an update.

Quota windows use monotonic time through `fast_io::posix_clock_gettime`;
Linux waiting uses absolute monotonic timer deadlines. Changing calendar time
does not replenish a window. Native failures and cancellation retain their
specified errors. Short I/O remains valid: callers must use the actual
transferred length, not assume the full request succeeded. A refusal is not
permission to crash the VM or fabricate a completed host operation.

The first authorized `proc_exit` ends the process by default, including when
it originates in a worker. Per-limit and native-wait exit policies can explicitly
retain an already admitted operation until it completes. New work cannot acquire
that exit hold afterwards. A native syscall already in progress is not falsely
reported as canceled. No guest can change these startup selections.

Denied-interface warnings are normally visible. Quota warnings are opt-in.
`--enable-warning`, `--disable-warning` and `--warning-as-error` accept
family-wide and API-specific selectors such as `all_host_denied_wasip1`,
`all_host_limited_ros2` and `host_denied_wasip1.fd_write`.
Use the [warning contract](command-line/host-permissions.md) for the exact
selector grammar. A missing import produces an error identifying the namespace
or interface and the command needed to expose it; enabling logging is not a
permission change.

## Files, sockets and ROS boundaries

A directory preopen binds a guest path to an explicitly selected host directory.
Initial base and inheriting rights are separate from the preopen and remain zero
unless granted. Child rights cannot exceed the approved inheriting rights.
Path length is governed by the ABI, memory validity and actual filesystem
behavior; the VM does not impose a special fixed 4096-byte guest path ceiling.

Socket support is implemented and compiled. A preopened socket starts with no
guest socket rights. The host chooses its endpoint and then grants the individual
read, write, accept, shutdown, readiness, flag or metadata rights that are needed.
Accepted-child rights are separate; descriptor renumbering must retain object
identity and authority. A global socket-rights record is not a general WASI
namespace grant or authorization for another module.

`--wasip1-socket-abi standard|wasix` is an immutable process choice for the
memory32 `sock_accept` signature. Standard is the default three-argument ABI;
the supported WASIX variant includes a peer-address argument. The switch does
not implement all WASIX APIs or authorize networking. No heuristic protocol
classifier decides whether arbitrary bytes are ROS traffic.

ROS calls cross the VM's native ROS interface and an explicitly selected
Gateway. Resource names, schemas, operations and quotas are independently
controlled. A general network grant can carry a user-implemented application
protocol; therefore deployment authority must control which endpoints are
preopened. The sandbox does not claim to recognize or forbid every possible
ROS protocol hidden in arbitrary network traffic.

## Execution, exceptions and experimental memory tagging

The interpreter and full-module LLVM backend must agree on guest results,
bounds errors, traps and permissions. `--runtime-int` selects the interpreter
when compiled; `--runtime-aot` selects LLVM. Native source/Wasm/assembly debugging
requires LLVM. Lazy/tiered execution is outside the ROS product.

LLVM guest exceptions use the owned native unwind metadata on all five
architectures. Instruction-based exception dispatch is unavailable. A compile
success alone is insufficient: nested exceptions, host transitions, traps and
replacement generations need execution tests with the matching unwind provider.

WasmMTE is a nonstandard extension. Parsing is enabled by default;
`--wasm-feature-disable-memtag-extension` rejects it. Execution is independently
disabled with `--runtime-disable-memtag`, which retains required zeroing/data
effects while omitting tag checks. These choices remain immutable across
replacement and checkpoint compilation.

The reference-compatible implementation uses tagged linear-memory pointers.
It does not prove protection for every language allocator, GC object,
continuation or concurrent allocation lifetime. Finite tags can collide and
same-granule errors can retain a matching tag. Software check followed by a
data access is not an atomic lifetime transaction: concurrent retagging needs
an explicit retirement/access protocol. The successor design and its semantic
model are not yet a production compiler/SDK ABI. Mandatory instrumentation of
every SDK language remains contingent on that qualification.

Where usable, hardware MTE requires the actual OS/thread policy, tagged mappings
and strict synchronous checking. CPU feature detection alone is insufficient.
Apple's synchronous EMTE/MIE policy must not be confused with Linux asymmetric
checking, whose writes can fault asynchronously. Deferred diagnostic export
would not authorize a mismatched instruction to execute. See the
[detailed MTE contract](runtime/memtag-extension.md).

Native host protection is separate: `--host-mte=off|required` is a build-time
provider gate. No complete native host provider is currently qualified, so
`required` rejects rather than labeling a partially instrumented VM protected.
See [native host MTE](toolchain/native-host-mte.md).

## Debugger authority, observation and IDE setup

RELEASE is the default management profile and permanently seals interactive
inspection and mutation. `--debug-permissions full` must be selected at launch
for guest debugging. Guest imports, reset, reconnect and replacement cannot
turn RELEASE into DEV. Debugger control is carried by an authenticated,
host-owned inherited endpoint, not a guest socket.

The built-in debugger owns only current Wasm-generated code and authenticated
guest state. It must not step into VM, libc, ROS middleware, native plugins or
another VM's code. Source, Wasm and native instruction views share the same
owner, generation and pause proof. The interpreter has no native debugger.
Native thread unwind information does not authorize arbitrary host memory
inspection. See [three-level debugging](runtime/debug-jit-three-level.md),
[data controls](runtime/debug-data-controls.md) and
[host observations](runtime/debug-host-observations.md).

The importer `tools/debug/import_ide.py` generates local and SSH profiles for
Code, Cursor, Neovim and the supported JetBrains/NetBeans bridges. It preserves
the exact host launch arguments without inferring permissions from a language.
Use an owner-only profile outside every guest preopen:

```sh
python3 tools/debug/import_ide.py validate --profile /absolute/private/robot.json
python3 tools/debug/import_ide.py import --profile /absolute/private/robot.json --open code
```

The [import guide](../tools/debug/README.ide-import.md) gives the complete JSON
schema, remote host-key configuration, launch authority and editor-specific
commands. An attach-only profile requires an already authorized broker.
Imported source mappings identify compiler paths; they do not grant arbitrary
host file reads. IDE UI success, language source stepping and all debugger
features are separate qualification dimensions. AOT/interpreted languages may
need runtime-specific metadata; selecting a language name is not evidence that
its frames or values are available.

Initial debugger connection waits on actual broker/VM events without an
implicit JIT compilation deadline. An operator may explicitly choose
`startupTimeoutSeconds` in the import/DAP profile or `--startup-timeout SECONDS`
on the Python broker command line, before its `--` boundary. These are debugger
tool options, not native uwvm executable parameters. Authentication and later
command deadlines remain separate. Canceling an already forwarded first
command retires the original launch so a late reply cannot be reused by another
connection. See the import guide for cancellation and remote behavior.

WASI/ROS observation, host-call content and JSON export require explicit
observation configuration. Disabled observation must retain the ordinary
execution path. Saved traces may contain application inputs or outputs and
remain host-owned data; do not expose them through guest preopens. Coverage and
performance evidence are in the linked observation and performance records.

## Crash policy and explicit dumps

Omit `--debug` for normal fault reporting and termination. The only crash
selection is `--debug pause`: report the failure and pause for the host
supervisor. No automatic dump, debugger attachment, instruction retry,
checkpoint recovery or hot update occurs.

An existing authorized DEV LLVM controller provides a cooperative terminal
stop and explicit debugger dump commands. Ordinary Linux execution uses
process `SIGSTOP`; the parent observes the stopped wait status. `SIGCONT`
finishes terminal handling. A process-wide stop does not itself grant an
in-process debugger or unblock a stopped management thread.

The host receives a `uwvm.crash.v1` JSON record on stderr with category,
reason, status, available numeric Wasm coordinates and MTE evidence. Unknown
locations remain unavailable. The stdout/stderr trust boundary and complete
pause behavior are specified in [crash policy](command-line/crash-policy.md).
Dump collection retains the existing ownership and coherent-stop checks.

## Hot updates and RELEASE diagnostics

`update` is independent of `debug`. RELEASE update authority comes from a
host launcher, an authenticated inherited channel, exact deployment scope,
approved permission contract and Ed25519 key. A guest has no update import.
Passing an endpoint to another process does not transfer the original
management grant. Launch and transaction credentials, source ownership,
generation and lifetime are revalidated.

The [production update specification](runtime/production-update.md) describes
function, numeric configuration/global, same-layout module and limited policy
transactions. Preparation is private; validation/compilation failure does not
publish a partial replacement. Publication requires the supported pause/drain
and ownership proof. A successful update is not an implicit permission reset,
resource reopen, quota refund or debugger enablement.

Current RELEASE admission is deliberately narrower than general Wasm
execution: one immutable main image, no preload graph, shared/imported state,
Wasm start section, checkpoint profile or unqualified native provider.
Synchronous builtin WASI has a qualified subset; arbitrary ROS/native
writer handoff and multi-module replacement are not complete. Current candidate,
scope, image and migration limits are documented in the specification.
They are implementation boundaries, not a claim of unlimited hot-update support.

DEV function replacement has separate actual-VM tests for enabled/disabled
MTE, memory32/64, stale generations, rejected tag mismatches and required zeroing.
That evidence does not automatically qualify RELEASE signed deployment or a
complete checkpoint world restore.

`--diagnostic-control-fd` provides a distinct authenticated RELEASE observer.
It cannot mutate or pause the guest and cannot reopen DEV. See
[RELEASE diagnostics](runtime/release-diagnostics.md) and
[management authority](runtime/management-authority.md).

## Exit status and supervision

On the Linux ROS executable:

- **0** is successful completion, including immediate successful help/version.
- **120** reports an executed `unreachable`.
- **121** reports other managed Wasm execution faults or uncaught exceptions.
- **122** reports a rejected memory-tag access.
- **123** reports a detected internal runtime failure.
- **126** is used for invalid launch parameters.
- **127** is used for module loading and validation/check failures.
- **75** reports a missing required authenticated production-update launch
  handshake in that startup path.
- An authorized WASI `proc_exit` preserves the low eight bits of the first
  accepted guest exit code. Guest-selected codes can overlap the values above.

A supervisor must inspect the OS wait result to distinguish normal exit from
signal termination. A shell's conventional `128 + signal` representation does
not reserve that numeric range against explicit guest exit codes. A debugger
may retain its authorized terminal session until closed; a paused process has
not exited. Do not use a single numeric status as proof that no code ran or
that an update committed.

Internal host APIs can return a structured guest-fault state separately from
a native function return. Embedding callers must inspect that state and must
not interpret an unchanged result buffer as successful execution.

## Testing and performance acceptance

Maintain separate checks for binary validation, source policy, actual execution,
host effects and cross-module isolation. Every negative permission test must
verify the missing effect, not only an error string. Each backend needs valid
and invalid accesses, cross-granule ranges, alias ownership, growth, traps and
concurrent lifecycle cases.

ThreadSanitizer supplements those checks on supported native hosts. First prove
that the selected runtime reports a known race and accepts a correctly
synchronized control. Instrument all relevant host translation units.
Dynamically generated LLVM code needs its own TSan instrumentation and exact
helper bindings; a sanitized launcher alone does not instrument generated code.
Document any unsanitized external libraries. Host atomic lowering may conceal
a guest-language race from TSan, so a clean TSan log is not a whole-Wasm
thread-safety proof. Retain deterministic check/retag/access tests independently.

TSan is a test configuration, not a release dependency or performance baseline.
The [official Clang TSan runtime](https://clang.llvm.org/docs/ThreadSanitizer.html)
does not support i686/ARM32 as equivalent test targets.
Those architectures still require deterministic real-worker tests; QEMU runs
must not be labeled native TSan or native performance passes.

Measure release performance with real native workloads and installed profiling
tools. Compare checking enabled/disabled, observation enabled/disabled and
ordinary execution using identical artifacts. Preserve PMU sampling quality,
CPU affinity, compiler flags, instruction counts and elapsed distributions.
Do not extrapolate a tight memory loop to every ROS workload.
[The MTE benchmark record](../benchmark/0006.memtag/README.md) reports the
remaining software-check overhead; near-zero-overhead SDK-wide protection
has not been established.

Official vendor simulators or official simulator robot models are the SDK
integration acceptance target. Replacing their behavior with a mock does not
qualify a robot. Full SDK acceptance still requires actual language runtimes,
the real VM path, required Gateways and the declared simulator deployment.

## Reference set

The [complete CLI catalog](command-line/cli-catalog.md) covers every advertised
command in the recorded integration binary. The [command-line reference index](command-line/README.md)
links the semantic specifications for its categories. The [build reference](xmake-options.md)
covers project configuration and the mandatory product gate.

The runtime, compiler, SDK and simulator qualification records must accompany
a release. Updating this document or enabling an option does not replace the
outstanding execution, lifecycle and performance evidence.
