# Franka selected ROS1 and ROS2 SDKs

This kit retains the complete custom interface graph from two immutable
official source selections and every reached original standard definition.
It provides complete C11, C++20, Rust, JavaScript, AssemblyScript and TinyGo
records, service/action facades, a whole-state reader and real Wasm command
examples. These are source-level development inputs; they do not implement
libfranka's native ABI or qualify every Franka model, FCI/robot software
combination, driver release or upstream official suite.

| Selection | Original commit | Definitions | Expanded records | Custom roots | Constants |
| --- | --- | ---: | ---: | ---: | ---: |
| ROS1 noetic-devel | 35e1f654426e04bc9f83b73af4ab68a3fb145c84 | 17 | 35 | 16 | 7 |
| ROS2 jazzy | 4acc05c01ebfcdf916ee6b3df966df0ad7b2f9ec | 47 | 73 | 30 | 11 |

The ROS1 graph includes franka_msgs, franka_gripper and the original example
controller's JointTorqueComparison. The ROS2 graph includes franka_msgs and
the nested franka_spine_msgs package. The source lock preserves each original
repository-relative path even when the package is nested. Branch inventories
record other available source branches; they are not compatibility test results.

Standard Jazzy originals are independently pinned to common_interfaces
a941f14bb318d8d904505ed935ccbb97f24a70a4 and rcl_interfaces
7aa3caf43377ea6ad615bc1040832e2c7566bfbe. ROS1 standard originals retain their
earlier independently verified acquisition provenance. Every original file
keeps its comments, fields, constants, defaults, Git blob SHA1 and SHA256.
Licenses and original driver implementation/documents remain under upstream/.

## Start with the controlled imports

Read sdk/include/roswasm/abi/ros1.h or ros2.h, then the corresponding C backend.
A Guest starts with no ROS authority. A nonempty argv alias selects only an
already configured policy resource. It cannot select a native IP, FCI endpoint,
credential, physical controller, module identity or Runtime permission.

Run the raw C entry before adding typed work. With no grants, open refuses13.
For a positive run independently select the exact module, backend, alias, role,
resource, complete type/native/canonical identities, QoS and finite quotas.
Generated Source supplies no Native identity, registry, installed driver or
grant. Runtime and separately compiled Gateway must use one coherent reviewed,
native-bound domain. Use the [A+B configuration guide](../../../../documents/ros/vendor-build-options.md)
to select only the required vendor/platform options; ros-vendor-franka defaults
OFF. An SDK source directory is not automatically linked into the Runtime.

The SDK exposes no updater, debugger configuration, credential selection,
policy mutation, native socket or GPU pointer. Physical control remains behind
the separately owned native Gateway and robot-side safety supervisor.

## Use complete values before model operations

Select exactly one franka-ros1-complete or franka-ros2-complete directory from
sdk/custom-types. Each contains all fourteen exact generated products and the
generation/preparation receipts. Use bounded_records in the selected language
and bounded_interfaces for every original service/action facade. Do not include
two independently generated domains in one native translation unit. Merge
compatible original declarations and regenerate one complete domain first.

include/state.h aliases the exact original full FrankaState (ROS1) or
FrankaRobotState (ROS2). Their layouts and fields are different; this SDK never
silently converts one to the other. ROS2 includes all nested joint, wrench,
inertia, transform, collision, error and accelerometer observations.
The finite capacities are part of the canonical identity: strings are bounded
to128 or256 UTF-8 bytes, dynamic joint/motion sequences to7 elements, and
declared fixed arrays retain their exact original lengths. A name list is not
implicitly reordered or expanded. Validate actual model joint order, lengths,
limits, freshness and mode before interpreting a received observation.

The caller owns nonoverlapping state and scratch storage. Scratch is exactly
2619 bytes for ROS1 or10169 bytes for ROS2. poll_state polls once, checks the
backend and decodes the complete value transactionally. Idle11 is not new data.
Error90 cannot trigger allocation, replay or an automatic larger read.

include/state.hpp supplies a borrowed serialized Reader and all generated C++
service/action facades. Construction opens nothing. No native rclcpp or
libfranka pointer enters Guest memory. Ordinary C zero initialization is zero
data; call the generated default factory for original declared defaults.
The ROS2 GraspEpsilon inner/outer defaults remain0.005; Jazzy Quaternion defaults
are also retained. Pure uwvm_franka_make_grasp takes explicit width, speed,
force, tolerances and a reviewed model maximum width. It validates finite
necessary input constraints, preserves the output on refusal and sends nothing.
Its checks do not establish model-specific force/speed safety.

## Build C and C++ with explicit source selection

All CMake build lanes default OFF. Set absolute source, output and compiler
paths. The examples below assume an existing genuine WASI Preview1 SDK;
substitute native C/C++ compilers and omit the Guest lane for native contracts.

    export UWVM_SOURCE=/absolute/uwvm2-ros
    export UWVM_OUT=/absolute/fresh-franka-ros2-build
    export WASI_SDK=/absolute/wasi-sdk
    cmake -S "$UWVM_SOURCE/sdk/robots/industrial/franka" -B "$UWVM_OUT" \
      -DCMAKE_TOOLCHAIN_FILE="$WASI_SDK/share/cmake/wasi-sdk-p1.cmake" \
      -DCMAKE_BUILD_TYPE=Release \
      -DUWVM_FRANKA_VARIANT=ros2 \
      -DUWVM_FRANKA_SOURCE_ROOT="$UWVM_SOURCE" \
      -DUWVM_FRANKA_GENERATED="$UWVM_SOURCE/sdk/custom-types/franka-ros2-complete" \
      -DUWVM_FRANKA_BUILD_CONTRACTS=ON \
      -DUWVM_FRANKA_BUILD_WASI_EXAMPLES=ON \
      -DCMAKE_EXE_LINKER_FLAGS='-Wl,--gc-sections,--threads=1,--initial-memory=8388608,--max-memory=33554432,-z,stack-size=1048576'
    cmake --build "$UWVM_OUT" --parallel 1

Use ros1 and its matching bundle in a fresh directory for ROS1. Configuration
checks the exact original catalog and every generated product byte/hash.
Wasm C++ uses no exceptions; LLD is explicitly limited to one worker.
franka_guest_c.wasm and franka_guest_cpp.wasm take raw|state ALIAS.
raw only opens/closes; state subscribes, polls once, then writes the complete
canonical value to stdout on successful whole decode. Closing is performed once.

With a selected dual/ROS2 Runtime and no ROS grants:

    "$UWVM" -Rint --wasip1-global-noinherit-system-environment \
      --run "$UWVM_OUT/franka_guest_c.wasm" raw franka-state

Expected exit status is13. Basic standard streams are independent of ROS rights.

## Build Rust, AssemblyScript and TinyGo

Use genuine producer installations and a fresh writable build/cache directory.
Select one backend only. These are real compilation commands, not a host model.

    export CARGO_TARGET_DIR=/absolute/franka-rust-target
    export CARGO_NET_OFFLINE=true
    export CARGO_BUILD_JOBS=1
    export RUSTFLAGS='-C opt-level=z -C codegen-units=1 -C debuginfo=0 -C link-arg=--threads=1 -C link-arg=--initial-memory=8388608 -C link-arg=--max-memory=33554432 -C link-arg=-z -C link-arg=stack-size=1048576'
    cd "$UWVM_SOURCE/sdk/robots/industrial/franka/examples/rust"
    "$CARGO" build --offline --release --target wasm32-wasip1 \
      --no-default-features --features ros2 -j1

The output is wasm32-wasip1/release/franka-state.wasm below CARGO_TARGET_DIR.
The matching rustc target must already be installed; do not substitute a host
executable. read_once returns primary, cleanup and an optional complete State.
Invoke the Wasm command with one nonempty explicit alias.

AssemblyScript uses the genuine backend NativeApi and complete generated Codec:

    cd "$UWVM_SOURCE"
    "$NODE" --max-old-space-size=256 --disable-wasm-trap-handler "$ASC" \
      sdk/robots/industrial/franka/examples/main_ros2.ts \
      --outFile /absolute/franka-as-ros2.wasm --runtime incremental \
      --initialMemory 128 --maximumMemory 512 --optimizeLevel 2 --shrinkLevel 1 \
      --use abort=sdk/assemblyscript/api/abortGuest

ASC is the actual assemblyscript/bin/asc.js. The real abort binding traps inside
the Guest; it is not an env.abort host callback. Basic stdin is one nonzero u8
UTF-8 alias length followed by exactly those bytes. readOnce returns the primary
operation; cleanupStatus remains separate. The command exit prefers primary,
then cleanup. lastValue changes only after a whole successful decode.

TinyGo uses the actual WASI target and local core module:

    export GOMAXPROCS=1 GOMEMLIMIT=512MiB
    export GOCACHE=/absolute/franka-go-cache
    export GOPROXY=off GOSUMDB=off GOTOOLCHAIN=local
    cd "$UWVM_SOURCE/sdk/robots/industrial/franka/examples/go"
    "$TINYGO" build -target=wasi -tags=ros2 -scheduler=none -gc=leaking \
      -panic=trap -p=1 -o /absolute/franka-tinygo-ros2.wasm .

Select ros1 for ROS1. Each generated Go product remains byte-identical in its
own package. The selected backend imports one package only. Invoke with one
explicit argv alias. Outcome preserves the primary, cleanup and complete value.

## Build the actual embedded JavaScript command

examples/javascript-ros1.json and javascript-ros2.json are closed recipes.
Select variant selection with sdk/javascript/guest/prepare.py, a pinned original
QuickJS tree, actual WASI Clang/linker/sysroot/resource directory, libc/libm/
printscan/builtins archives and a full Wasm validator. See the
[engine build instructions](../../../javascript/guest/README.md) for every
producer option. Preparation writes exact serial jobs in report.json; execute
each compiler/final-link/validator argv and retain each actual result.
A PREPARED report or Node execution is not a compiled Wasm result.

Each recipe includes only the selected complete generated module graph. Its
copied output scratch admits the entire2619/10169-byte state. main opens nothing
during import and starts work only when called by the genuine engine.
Counted stdin matches AssemblyScript. Stdout is exactly two u32LE statuses:
primary, cleanup. Default refusal is 0d00000000000000 with engine exit0.
Engine success does not mean a robot operation succeeded.

## Stop, cancel, timeout and uncertain outcomes

Explicit action-send, own-goal cancel, feedback and result authorities are
independent. Preserve the owner and goal correlation until the matching
terminal outcome. Transport admission or an action acknowledgement is not
physical completion. Closing a handle is not stop, cancellation acknowledgement
or rollback. Do not resend an unacknowledged command automatically.

ROS1 retains the original Stop action. ROS2 retains its actual selected source
interfaces, including Spine Halt and mode/fault services; they are not aliases
for a certified emergency stop. Select the exact native driver resource and
review its firmware/model semantics before granting it. Never automatically
invoke ErrorRecovery, FaultReset, SwitchOn, homing or resume after a timeout.
The native watchdog, safety supervisor and explicit unknown-outcome/recovery
procedure remain required outside the Guest.

## Source checks and qualification

Run the read-only source checker on Linux inside the admitted hard64GiB/swap0
cgroup:

    python3 -I -S -B sdk/robots/industrial/franka/tests/validate_sources.py \
      --source "$UWVM_SOURCE"

It independently checks Git commit/tree/blob associations, every original
custom declaration, all component member/constant names in all six maps, every
generated product and unchanged Go example copies. File integrity is separate
from compilation and actual traffic. tests/contract_ros1.c and contract_ros2.c
exercise complete record defaults, truncation/trailing refusal, destination
preservation, short encode buffers and pure goal validation. They are authored
SDK contracts, not the original vendor test suite.

Retain actual selected source/toolchain/runtime hashes, full imports,
compilation/execution logs, QEMU ELF/loader/sysroot identities, cgroup membership,
limits and memory events with every result. No native ROS, controller, simulator,
all-release, upstream official-suite or hardware VTune acceptance is inferred
from SDK source or default-denied Wasm examples.
