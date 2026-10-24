# Six same-main ROS1 legacy commands

These six applications use the existing C, C++, Rust, TinyGo, AssemblyScript,
and JavaScript Guest SDKs with backend 1 and the complete official
`unitree_legged_msgs` family. They are source candidates, not completed build or
transport results. Their closed declarative build inventory is
`sdk/origin/robots/unitree/recipes/legacy-command-recipes.json`.

The JavaScript application is genuine QuickJS inside its Wasm command. It does
not use Node, a host-side ROS library, or a fake linear-memory object. Every
positive operation in every language executes in the actual main Wasm module.
A WAT root calling a ROS-privileged library would exercise different ancestry
and is not an accepted positive replacement. These commands do not change the
Runtime's cross-library denial rule, default grants, or optional WASI rights.

## Genuine native prerequisites

A future actual build must begin with a completed, authenticated amd64 Noetic
SDK acquisition and a fresh independent SDK copy. A failed or partial extracted
SDK is not admissible. The official legacy source is `unitree_ros_to_real` at
`b989870124913091fbe75e0dcfb047eb4ca00a09`, package
`unitree_legged_msgs`, containing eleven message definitions. Preserve the
actual upstream provenance, licenses, all eleven raw message bodies, complete
checkout catalog, and actual physical pins. Do not substitute the GO, HG, or API
ROS2 families, or a different repository with a similarly named package.

Compile the original catkin package without editing its CMake or message files.
Compare all eleven newly generated devel headers to the installed headers
before admitting the latter. The native stopped exporter must provide genuine
`ros::message_traits::DataType`, `MD5Sum`, and `Definition` for all eleven types.
Validate those definitions against the admitted source messages. Feed those
actual identities into the maintained bounded generator, retaining its exact
current source, shape lock, native binding, complete output modules and output
catalog. All six applications consume those complete newly native-bound
modules. An older guest-only Source33 generation with N/A native identities can
explain generated field spelling, but cannot prove Noetic compatibility or be
relabelled as a native provider.

The native counterpart is the separately compiled
`test/0073.ros-tests/support/source/ros_unitree/native_legacy_ros1_peer.cpp`, with the contracts and values in
its adjacent headers. Its CMake fixture uses real installed Noetic headers and
libraries. A future producer must qualify that binary, a complete legacy ROS1
Gateway, the ROS master and all selected SDK libraries with actual full
system-header dependency files, GNU Make working-directory evidence, complete
link trace, ELF facts, whole SDK/source/tool catalogs, and actual opened
`/proc/PID/map_files` byte identity. A file pathname or a raw maps device tuple
is not that physical proof. Keep main Worker, native SDK, Gateway, Guest
provider, and harness source/MF identities separately labelled.

## Values and wire observations

Every app constructs all fields of complete `LowState` and `LowCmd` values,
including 20 motors, IMU, BMS, all fixed arrays, reserved words, wireless bytes,
forces, tick and Cartesian values. No vendor struct is hand-written in these
apps. `values_c.h` shares only test-value equations between the actual generated
C values and their C++ aliases. Rust, Go, AssemblyScript and JavaScript use their
own complete generated value APIs. The native fixture constructs its expected
values independently using actual ROS message types.

The state has canonical length 1069 bytes and the command 734 bytes, both below
the explicit 1536-byte app capacity. Those lengths are an additional check,
not permission to serialize native object memory. The Guest decodes the entire
actual state, re-encodes it with the actual complete generated codec and compares
every byte to the complete expected state. C, Rust and Go also compare the
re-encoding to the received canonical input. Typed C++, AS and JS subscriptions
already validate/decode that entire exact input before dispatch. The parent
must compare the reported full state bytes with the native fixture's full
state bytes, not just the tick, checksum, CRC, or a selected motor field.

The command is created independently in every language and published exactly
once after the native fixture is armed. The native callback compares its
complete actual ROS1 serialization to its independently constructed command.
`LowState.crc=0x13572468` and `LowCmd.crc=0x89abcdef` are transported fields that
must remain equal. This does **not** authenticate a Unitree hardware CRC or
reuse a GO/HG firmware layout/algorithm. No robot, firmware address, simulation,
controller behavior, latency or performance result is claimed.

## Application owners and initialization

`initialize_case(1,4,0)` opens `legacy-lowcmd` and creates a real subscription on
`legacy-lowstate`. Each language calls the actual `topic_subscribe` path before
it reports initialization. Capability open alone is not sufficient: the ROS1
Gateway attaches a custom subscription only when subscribe is requested.
C explicitly calls `roswasmc_subscribe`; C++, Rust, Go, AS and JS use the actual
SDK subscription constructor/factory that performs that call.

The SDK organizer names are local labels. They do not confer a native node
identity or select a new principal. Only the authenticated actual main receives
explicit isolated fixture grants. The complete native identity and
adapter-codec-identity must come from the selected actual provider.

C/C++/Rust export a finite scalar application interface and link the real
`entry.c` into the same final command module. Rust includes the complete generated
module with `#[path]`, preserving its inner documentation. Go compiles its real
`main` together with the application, values and protocol in one package. AS
executes its actual module/global initialization through the explicit void
`legacy_command_entry` export. JavaScript uses the embedded engine's ordinary
command pipeline to evaluate its closed module graph and invoke actual
`export function main()`; there is no substitute scalar trampoline.

For a fresh negative instance `initialize_case(0,4,0)` probes both alias opens,
requires status 13 and no owner, records exactly two denials, and closes its
organizer. A successful open or another status fails the case. It does not skip
the second alias because the first was denied. Closed poll/submit/close calls
must then refuse with status 9. No ROS aliases are granted to the negative
instance. Basic stdio remains necessary for the finite test protocol.

## Actual command protocol

The driver and native witness are independent actors. The driver retains every
original Popen, birth, pidfd, cgroup and executable proof. Use a private loopback
network namespace and a real owned ROS master; prove that only `lo` is up, all
SDK mappings are admitted, and the configured namespace/node names are exact.
A control byte, PID printed by a process, alias label, or environment variable
is not an authentication proof. Observe native and Worker readiness, then use
the existing owned executable gates before sending either protocol setup.

1. The actual Guest command emits `READY_LEGACY_MAIN` after its language
   initialization and before opening any ROS alias. Parent writes exactly
   16 bytes: ASCII `ULG1`, little-endian family 4, mode 0 or 1, reserved 0.
   The Guest initializes and emits `LEGACY_INITIALIZED MODE`.
2. Negative mode emits `LEGACY_DENIED 2`, accepts only `F`, prints
   `PASS_LEGACY_CASE`, and exits 0. The native denied counterpart must confirm
   zero Gateway publishers/subscribers, zero callbacks, and its full quiet
   interval; a Guest line alone is insufficient.
3. Positive mode uses only `/uwvm_unitree_legacy/lowstate` and
   `/uwvm_unitree_legacy/lowcmd` in the isolated fixture. Native `M` must observe
   two LowState subscribers (native self plus Gateway) and **zero** LowCmd
   publishers. ROS1 publish aliases are lazy: opening `legacy-lowcmd` does not
   advertise the publisher. Do not require publisher 1 at this earlier stage.
4. Send native `P` once. After its full state publication/self-observation,
   send Guest `R` to perform one actual SDK poll. `LEGACY_AGAIN` allows another
   explicit parent `R`, bounded to 4096 total polls; no hidden retry/thread or
   clock is added inside the app. On success the Guest emits
   `LEGACY_GUEST_STATE LENGTH HEX`, with all canonical state bytes. Compare them
   to the native state and require exactly one received state.
5. Send native `A` and wait for `LEGACY_ARMED_COMMAND` **before** Guest `S`.
   The Guest publishes one complete command, checks a repeated submit refuses
   with 9, and emits `LEGACY_SUBMITTED`. The actual positive LowCmd grant uses
   `latch=true`, `queue-depth=1`, `burst=1`. This isolated explicit policy
   prevents the one publish from being lost before TCPROS connection setup;
   it is neither a global latch default nor a relaxed queue invariant. Require
   the native full command observation, publisher count 1, and callback count 1.
6. Send Guest `C`; it closes all actual owners after callbacks/leases unwind,
   checks repeated close/poll/submit refusal, and emits `LEGACY_CLOSED`.
   Send `F`, require `PASS_LEGACY_CASE` and actual Guest exit 0. Native `Z` then
   proves the full quiet interval and only its own state subscription remains;
   native `F` closes ROS and exits 0. The negative native control uses `Q`.

The native setup is independently 16-byte `ULN1`, mode, sequence 42 and zero
reserved word, as specified in `NATIVE_LEGACY_ROS1.md`. Never swap the native and
Guest setup layouts. Parent owns deadlines, finite output budgets and final
terminal cleanup. A trap, nonzero exit, unexpected line, extra callback, missing
connection, failure to close, changed input, or unproven descendant fails the
case. Do not retry a failed case in place or count a previous partial run as a
new successful instance.

## Reproducible compiler inputs

The JSON recipe is deliberately a declarative input contract for the existing
serial generic producer, not a second build framework. Each token must be
resolved to an actual absolute operator-selected input with full physical pin
and complete package/root coverage. All output paths are fresh. The per-job
`{OBJECT}` and `{DEP}` tokens are distinct for entry and application TUs.
Preserve compiler/linker invocation aliases as well as physical ELF pins.

C uses C11 and C++ uses C++20. Compile the example and C entry independently
with `-MD -MF -MT` (including system headers), `-UNDEBUG`, no fast math, and
`-ffp-contract=off`. Link serially with the actual Wasm selector `-fuse-ld`,
`--threads=1`, full trace and a 64 MiB memory maximum. C++ uses the maintained
`sdk/origin/examples/robotics_modular/cpp_termination` official-source profile: actual
SDK VERSION and LLVM commit must match its exact source lock, the operator pins
its platform compiler/alias, and the separately compiled unchanged official
abort object precedes implicit C++ archives. Only that official TU has its
reviewed NDEBUG/BAREMETAL scope; all Guest TUs keep UNDEBUG and the documented
header verbose-abort trap. Require real why-extract evidence rejecting default
`abort_message.cpp.o` and `verbose_abort.cpp.o`. No stderr ABI stub is allowed.

Rust uses the actual selected target-equipped SDK, `ros1+alloc`, one codegen
unit, dep-info files and the same freshly compiled C entry. Actual successful
linker stdout must be retained outside Rustc's diagnostic pipe. Reuse the
reviewed generic command builder's same-PID real-linker wrapper: every temporary
object has genuine pre-exec opened-byte evidence and every raw trace entry is
accounted for. Retired objects stay historical facts; selected rlibs, SDK,
entry, tool and trace remain ordinary current pins. A missing-file skip is not
link closure.

TinyGo is scoped to selected v0.43.0. Preserve every field of its actual
`targets/wasm-unknown.json` in a fresh owned target, appending only
`--max-memory=67108864` to ldflags. Use `-buildmode=default`, `-scheduler=none`,
`-opt=1`, `-p 1`, and the real `ros1` tag. The platform predicate accepts
`tinygo.wasm`; do not add a manual `wasm` tag or compile a c-shared reactor as a
command. Preserve complete TinyGo/Go packages, actual offline module graph,
explicit selected Go in PATH and owned caches. Do not mutate the SDK target.

AS preserves the complete SDK and generated-module relative paths. Its actual
exported initializer is `legacy_command_entry`; require its signature `()->()`
and absence of implicit start. Use the existing SDK `abortGuest` trap, never an
`env.abort` allowance. Complete Node/AS package roots replace invented C depfiles.

JS uses `javascript-manifest.json`, the complete SDK graph and freshly generated
legacy provider. The actual backend1 command preparation has thirteen C TUs:
five QuickJS, engine/scalars/utf8, native_ros/stdio, command.c and two generated
TUs. Execute and retain that exact preparation's real jobs, all full-MD files,
source/object pins, actual trace, official full printscan-before-libc selection,
full validator and binary table. The source manifest's `exports: []` means the
command invokes the real JS main, not an FFI implementation. Limits, integer
and float semantics come from the maintained current engine.

Every final module requires full actual `wasm-tools validate`, one owned memory
with maximum 1024 pages, no implicit start, no imported memory/global/table,
exact used ROS1 ABI signatures and only basic WASIp1 `fd_read`, `fd_write`,
`proc_exit`. Unexpected imports fail; a compiler pass never expands that list.
All twelve fresh default-denied/positive transport cases, owner preservation and
terminal cleanup remain future actual work. None is implied by these recipes.
