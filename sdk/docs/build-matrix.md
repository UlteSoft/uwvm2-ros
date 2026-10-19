# SDK compiler matrix in the existing Linux cgroup

[`../tools/build_cgroup_matrix.py`](../tools/build_cgroup_matrix.py) prepares
recipes by default. It performs actual commands only with `--run`, on Linux,
after the existing repository [`../../tests/ros_cgroup/probe.py`](../../tests/ros_cgroup/probe.py)
proves the live process is in the current original cgroup with an exact leaf and
effective memory limit of 64 GiB (`68719476736` bytes). The caller must pin that
current leaf's absolute path, device, inode and kernel boot identifier. A
previous boot/incarnation's inode is not interchangeable with the current one.
The supplied environment also pins `memory.swap.max=0` and effective CPUs `0-31`;
explicit expected values are checked before/after and during commands. A previous
64 GiB receipt or a 64 GiB ancestor above a different leaf cannot qualify a run.
This driver never creates a cgroup, changes limits, creates binfmt entries,
installs/downloads a compiler, grants a capability or starts ROS.

Required tutorial/fixture filenames remain explicit even when a directory is
empty; missing entries become blocked rows and an empty matrix can never pass.

All compilation, binary inspection and full validation in an actual run occur
inside that pinned cgroup. The output directory must be absolute, fresh and
outside the SDK source tree. Existing directories are refused. The SDK's complete
file/symlink manifest is recorded before/after and rechecked after each row;
source mutation fails the matrix and preserves raw outputs. Freeze SDK source
only after all parallel source authors have finished.

## Explicit existing tools

Provide only toolchains already installed in the Linux environment:

| Option | Existing input | Recipes |
| --- | --- | --- |
| `--wasi-sdk` | Absolute WASI SDK directory containing bin/clang, bin/clang++ and share/wasi-sysroot | C11/C++20 examples, C JS bridge, and the explicit target C archive for every Rust Wasm row |
| `--cargo`, `--rustc` | Absolute actual ELF executables | Rust wasm32-wasip1 examples, exactly one ros1/ros2 feature; alloc only when needed |
| `--tinygo` | Absolute TinyGo ELF executable with installed Go/WASI dependencies | wasi target and exactly one ros1/ros2 tag |
| `--node`, `--asc-js` | Absolute Node ELF executable and installed AssemblyScript compiler script | Real AssemblyScript Wasm examples and prepared fixtures |
| `--wasm-tools` | Absolute validator ELF executable | Full validation of every compiled Wasm artifact |

Missing configuration or files produce a recorded BLOCKED row. Compiler/linker
errors retain their raw log and remain FAIL; a missing target/stdlib is not
silently replaced. The driver does not invoke npm, rustup target installation,
package installation or network fetching. Cargo uses offline mode and disables
automatic Rust toolchain installation. Go module/checksum proxies are disabled
and automatic Go toolchain selection is local only. These are compiler controls,
not permissions granted to a guest.

The actual requested tool executable is bound by ELF header, device/inode and
SHA256 before/after each command. An AssemblyScript compiler script and selected
Rust compiler are also bound before/after. This binds supplied compiler bytes;
it is not a full reproducible closure of every external system header/library.
The report explicitly records `toolchain_sdk_closure=UNQUALIFIED`. WASI sysroot,
Rust target standard library, TinyGo support files, global Cargo configuration
and AssemblyScript package dependencies require a separately verified immutable
closure before toolchain input reuse can be qualified. This matrix does not
upgrade supplied path/ELF hashes into that full qualification.

Rust compiler-wrapper environment keys are explicitly set empty, any inherited
encoded Rust-flags override is removed, and the pinned `RUSTC` is selected directly.
Every Rust Wasm row then supplies its own `CARGO_ENCODED_RUSTFLAGS` tokens for
one codegen unit, one linker thread, and the exact static archive
`WASI_SDK/share/wasi-sysroot/lib/wasm32-wasip1/libc.a`. The archive is an opened,
bounded regular input with before/after identity and SHA256 checks. Other WASI
targets, host `-lc` searches and custom memory-function replacements are not
fallbacks. Explicit tokens preserve a package path containing spaces. Coverage
keeps its separate export flag; builders with an explicit Rust sysroot keep it.

`no_std` omits Rust's standard library; it does not guarantee that code generation
needs no C compiler intrinsics. Slice comparisons can require `memcmp`, supplied
by this target archive. A standard-runtime Rust binary uses the same explicit
archive selection. Static linking grants no WASI authority. Import inspection
must still reject `env.memcmp` and every other unapproved non-ROS module; actual
startup imports and required permissions remain a separate binary/runtime check.
The local runner uses the same recipe helper for all ordinary/custom Rust rows
and leaves host Rust model builds untouched.
Relevant remaining compiler environment inputs are recorded by length/digest
without exposing inherited raw values; known explicit controls are recorded in
plain text. Global Cargo/toolchain configuration is still an unqualified input.
The caller supplies and records installed toolchain/sysroot provenance.

## Matrix contents

Every selected backend compiles all C and C++ examples, every declared Rust
example executable, every TinyGo example package, and all numbered
AssemblyScript lessons. ROS2 parameter-event/lifecycle entries compile only for
ROS2. Local scheduling lessons deliberately import no ROS operation.
The five handwritten `custom-types/examples/use_generated` guest applications
also compile under their explicit ROS1 profile. Rust/TinyGo use fresh local
package manifests and copied `generated-r3` codecs; no native registry or
official identity is invented. JavaScript's sixth example is a bridge client
with a separate pure codec exercise.

C and C++ examples also export callable educational helpers and `example_backend()`,
which supplies the selected immutable dispatch table for a guest SDK context.
The Wasm export annotations keep those functions reachable under linking. The
pointer/reference arguments require initialized guest objects; these exports
do not define a native/JavaScript marshalling API. One C and one C++ recipe row for each
backend requires all38/all43 imports, in addition to the raw coverage fixtures. It
returns the immutable table and performs no native operation. Headers remain
complete; linkage is checked from the emitted Wasm binary, not assumed from a
source declaration inventory.

Only the WASIp1 C++ example compilation recipes add `-fno-exceptions -fno-rtti`. Ordinary
SDK/ROS refusals remain numeric, callbacks must not throw, and the examples do
not depend on a Wasm exception ABI. Standard-library allocation failure follows
the target's termination policy; it is not translated into a recoverable ROS
error. Choose the C caller-buffer or freestanding C++ API for allocation-free
work. Native model compilation/execution keeps its original flags, RTTI and
callback unwinding semantics.

Rust/TinyGo local packages and example packages are copied into a fresh working
subtree with the SDK license. Cargo.lock/go.sum/cache writes therefore cannot
mutate original SDK inputs. Cargo targets and Go caches are separate output
paths. Features/tags select exactly one backend. The standard-library Rust
examples require the installed wasm32-wasip1 target; the SDK's core library
alone does not establish availability of that target or linker.

AssemblyScript uses its formal ROS1 variants and existing ROS2 entries. The
ROS1 resource entry selects the implemented int32 parameter type. The abort
mapping is explicitly `--use abort=sdk/assemblyscript/api/abortGuest`; it is a
prepared recipe whose emitted imports must be inspected. An unhandled `env`
import is recorded as incompatible/blocked, not admitted by a blanket env
allowlist. The two raw coverage fixtures require every registered ROS1/ROS2
import. High-level/codec/adapter/factory/executor/resource-role/Unicode model fixtures are compiled
and fully validated separately, expect no ROS imports, and are never executed
by this driver. Their successful compilation is not an actual model-test run.

JavaScript source is host-side application/embedding code, not a Wasm guest.
The JS rows compile the concrete `javascript/bridge.c` guest for each backend
and require all38/all43 actual raw imports. Each backend also compiles the
freestanding `javascript/relay.c` command, retaining that complete ROS inventory
and exactly `fd_read`, `fd_write`, and `proc_exit` with their WASIp1 signatures.
The `.mjs` layer needs a separately
qualified authenticated embedding. A permissive JS mock is not that embedding;
compiling this bridge does not cause the uwvm CLI to execute JavaScript.

Rust's `coverage.rs` is compiled with the exported `roswasm_abi_coverage`
linker symbol; TinyGo's `coverage` package requires actual compiler support for
its `//go:wasmexport` directive. Both inventory rows require all38/all43 binary
imports after compilation. The probe accepts zero placeholder arguments and
must never be executed against real grants. A missing directive, export or
pruned import produces a failed/blocked row, never a source-only full-coverage
claim. The compiler environment is bound by a digest without exposing inherited
values; the explicit Rust token list, coverage export and target archive binding
are recorded separately.

## Observed process and memory guards

Each compiler/validator command starts in a fresh owned session. A same-PID
Python launcher stops before exec; the driver verifies its parent, birth,
process group, actual membership and live PID in the pinned leaf's cgroup.procs
before continuing it. It observes descendant births, membership, executable
inode and RSS every20ms. A live observed departure fails. Confirmed raced exits
are excluded from positive execution evidence. Very short internal children or
validator executions can finish between samples; `target_executable_observed`
records that honestly. This sampling is not exhaustive instruction tracing and
is not aarch64 execution qualification.

The default aggregate sampled compiler-RSS guard is 2 GiB (`2048` MiB) and the
whole cgroup `memory.current` guard is 60 GiB (`61440` MiB), both below the hard
64 GiB (`65536` MiB) limit. Per-command and whole-run `memory.events` deltas
include OOM/OOM-kill counters. Leaf incarnation
and boot must remain consistent. Any new OOM, counter reset, source mutation,
process departure or deadline failure prevents a successful matrix status.

A child is added to the owned-birth set immediately after ancestry/birth
discovery, before membership checks. A first-observed departure therefore stays
covered by cleanup.

On failure, cleanup signals only recorded owned births through checked pidfds,
including observed children whose compiler parent already exited. It never
signals an arbitrary reused process group. Cleanup must verify no observed live
owned child remains. A failed process/memory/cleanup guard aborts the matrix after preserving its
row evidence; no further compiler starts while observed owned children remain.
A failure to re-prove cgroup scope, or a launched command exception during
cleanup/after-proof/tool binding/evidence persistence, produces a typed fatal
guard capsule with available facts and aborts. An unavailable after/cleanup
proof is never relabeled successful. Preflight missing executable configuration
before launch can remain a normal blocked row.
Secondary asc.js/Rust compiler rebinding and row source/report finalization also
abort on failure. Available command facts are retained in a separate fatal row
fallback if the normal report cannot be saved. If both writes fail, the failure
and a best-effort stderr capsule remain explicit; on-disk persistence is not
claimed. Detected secondary tool-input mutation also prevents another command.


Ordinary missing inputs, compiler errors and unsupported import modules are
recorded and independent rows can continue. The deadline and guards can be explicitly lowered; guards
must satisfy `0 < RSS <= memory-current < 65536 MiB`. Actual runs require Linux/Python pidfd
support for this cleanup contract.

## Invocation and evidence scope

Prepare recipes without running a compiler or creating output directories:

```sh
python3 -B sdk/tools/build_cgroup_matrix.py --out /absolute/new/sdk-build-run \
  --languages c cpp rust tinygo assemblyscript javascript_bridge --backends 1 2
```

For an actual run, execute the driver through the existing authorized cgroup
launcher. Supply current leaf/device/inode/boot from its verified Linux incarnation
and each installed toolchain's absolute paths:

```sh
python3 -B sdk/tools/build_cgroup_matrix.py --run --out /absolute/new/sdk-build-run \
  --expected-leaf /sys/fs/cgroup/current-authorized-leaf \
  --expected-device CURRENT_DEVICE --expected-inode CURRENT_INODE \
  --expected-boot CURRENT_BOOT_UUID --expected-memory-max 68719476736 \
  --expected-swap-max 0 --expected-cpus 0-31 \
  --rss-limit-mib 2048 --cgroup-current-limit-mib 61440 \
  --wasi-sdk /absolute/wasi-sdk --cargo /absolute/cargo --rustc /absolute/rustc \
  --tinygo /absolute/tinygo --node /absolute/node \
  --asc-js /absolute/assemblyscript/bin/asc.js --wasm-tools /absolute/wasm-tools
```

The leaf/device/inode/boot and tool paths above are placeholders to be resolved
from the live Linux environment, not remembered or invented values. The memory,
swap, CPU and default guard values are the current explicit acceptance contract.
`--expected-boot` is required for an actual compiler run. `report.json` separates
compilation, actual binary import checking, full validation and guard evidence.
A bounded artifact read supplies both the recorded SHA256 and binary import
inspection. Its device/inode/size/timestamps/bytes must match immediately before
and after full validation; output-path replacement or mutation fails and aborts
the matrix.

Every row explicitly records `guest_execution` and `ros_execution` as
`NOT_PERFORMED`. A compiler matrix PASS is limited to recorded commands, produced binaries and
those three preparation steps; it does not qualify complete compiler inputs for reuse and leaves `task_complete=false`.

Actual native C/C++ mock fixtures, Rust/TinyGo unit fixtures and AssemblyScript
model execution remain required separately in the same cgroup. Native ARM model
compilation/execution needs the mainline's exact cross configuration, actual
QEMU argv, mapped executable proof and source/dependency binding. This driver
has no fake native-test mode and runs no bare uwvm command. Actual ROS behavior
must use the independently qualified ARM Worker/Gateway mainline with explicit
grants, default-deny cases, native side-effect witnesses, deadlines, cleanup and
current cgroup/OOM evidence. Those runtime results are not inferred from Wasm
compiler output, import signatures or full-module validation.

This driver and its recipes are prepared while Linux SSH is unavailable. No new
compiler, validator, Wasm model, Worker or native ROS execution result is claimed.
