# ROBOTIS TurtleBot3 complete selected interface SDK

This kit provides unchanged original custom messages, services and actions from
four explicitly selected TurtleBot3 source snapshots. It provides complete
bounded codecs and service/action facades for C, C++, Rust, JavaScript,
AssemblyScript and TinyGo. It is a source integration SDK. It does not certify
every TurtleBot3 model, firmware, historical release, native driver or controller.

| Profile | ROS | Original definitions | Complete records | Custom roots |
| --- | --- | --- | --- | --- |
| noetic | 1 | 23 | 28 | 4 |
| humble | 2 | 27 | 34 | 7 |
| jazzy | 2 | 27 | 34 | 7 |
| main | 2 development snapshot | 27 | 34 | 7 |

Each `catalogs/PROFILE/source-lock.json` records exact message, driver and
standard dependency commits, original blob hashes, field bounds and source
provenance. Main is a development snapshot, not a released distribution.
Original comments, constants, defaults and all components are retained.
Noetic includes the original driver `Turtlebot3.action`; ROS2 includes
`Patrol.action` and `Dqn`, `Goal` and `Sound` services.
ROS1 Sound is a topic message. The ROS2 Sound service is a distinct interface;
a service response reports its original success/message, not physical completion.

## Authority and native integration

All imports and constructors start with zero ROS authority. An operator must
explicitly configure the selected native ROS backend, exact source-bound type
registry, endpoint, direction, operations, limits and resource alias. The
example never invents an alias, role, robot IP, ROS domain, device access or
inherited environment. Importing the SDK grants no socket, serial, file, process,
clock, update, credential, debugger or policy authority. Runtime vendor option
`ros-vendor-robotis` is also off by default; compiling it is not a grant.
See the repository vendor build guide for independent A+B option selection.

The checked-in complete bundles were prepared without original native middleware
metadata. Their native registry is deliberately absent. To bind real endpoints,
use `sdk/robots/industrial/prepare.py --mode bind` with independently collected
original native metadata and original owner report matching the selected
catalog and exact installed ROS middleware. Do not fabricate IDs or substitute
source hashes for native type identity. A builtin standard endpoint keeps its
existing registered identity; additional standard records here are data-only.

C `state.h` owns no native resource. `uwvm_turtlebot3_poll_state` borrows
an explicitly opened capability and caller scratch, checks the backend, and
commits a complete SensorState only after full decode. C++ `state.hpp`
adds a borrowed Reader and the complete generated service/action facades.
`uwvm_turtlebot3_make_sound` constructs one full original message for codes
0..5 and preserves the destination on invalid input; it never publishes.

## Complete bounds and errors

Every field is represented, including nested headers, all SensorState flags,
encoders, battery and version strings. Standard Imu, MagneticField, LaserScan,
JointState, BatteryState, Odometry, Twist, TwistStamped and TransformStamped
records and Trigger/SetBool service data are included with their dependencies.
These standard roots are not additional native registrations.

Frame/name strings have a 128-byte UTF-8 quota; other strings have 256 bytes.
Each LaserScan ranges/intensities sequence has at most 1,024 items. JointState
sequences have at most two items; battery cell arrays have at most 16 items;
Dqn state has at most 128 floats. These are explicit bounded SDK profiles, not
claims that the original unbounded ROS types have those limits. Larger input is
refused; it is never silently truncated. Adjust a catalog quota only by preparing
and qualifying a new complete graph within the 32 KiB/4,096-node ceilings.

Error13 means permission denied, 11 means no event, 22 means invalid value and
90 means the supplied bounded storage cannot hold the value. Preserve primary
and cleanup status separately; close does not acknowledge, cancel or stop motion.
Do not replay a command or resume a robot automatically after any error.
A successful Patrol result is application data, not an independent safety proof.

## Prepare and compile C/C++

Set absolute `SRC`, `KIT=$SRC/sdk/robots/mobile_bases/robotis/turtlebot3`,
`PROFILE=noetic|humble|jazzy|main` and
`BUNDLE=$SRC/sdk/custom-types/robotis-turtlebot3-$PROFILE-complete`.
Use actual SHA256 values of the current generator sources when regenerating:

```sh
python3 -I -S -B "$SRC/sdk/robots/industrial/prepare.py" \
  --source-root "$SRC" --catalog "$KIT/catalogs/$PROFILE/catalog.json" \
  --out /tmp/turtlebot3-prepared --mode prepare \
  --parent-sha256 "$PARENT_SHA256" --bounded-sha256 "$BOUNDED_SHA256" \
  --adapter-sha256 "$ADAPTER_SHA256"
cmake -S "$KIT" -B /tmp/turtlebot3-c \
  -DUWVM_TURTLEBOT3_BUILD_CONTRACTS=ON \
  -DUWVM_TURTLEBOT3_PROFILE="$PROFILE" \
  -DUWVM_TURTLEBOT3_SOURCE_ROOT="$SRC" \
  -DUWVM_TURTLEBOT3_GENERATED="$BUNDLE"
cmake --build /tmp/turtlebot3-c --parallel 1
```

Preparation emits no native registry. For a newly prepared bundle, preserve its
preparation receipt and create a reviewed profile-selection.json from the exact
source-lock, catalog and product hashes using `tools/select_profile.py`.
The checked-in bundles already include this source-only selection record.
The selection tool also supports an actually bound 16-product bundle: its
preparation must be bind, and the exact manifest/native adapter must be present.
This permits rebuilding Guest examples after genuine native identity binding;
it does not qualify or simulate that independent native step.
CMake compares the full original bodies, profile lock and all exact products;
a catalog with identical paths is insufficient to choose another distribution.
Both contract and Guest targets are OFF unless selected explicitly.

For genuine WASI entries add the actual WASI SDK toolchain,
`-DUWVM_TURTLEBOT3_BUILD_WASI_EXAMPLES=ON` and
`-DCMAKE_EXE_LINKER_FLAGS=-Wl,--threads=1` at initial configure time.
Run a built `turtlebot3_guest_c.wasm` or `turtlebot3_guest_cpp.wasm`
with `raw ALIAS` to exercise explicit open/close, or `state ALIAS`
to subscribe and read once. On success C/C++ write the complete canonical
SensorState bytes to basic stdout. Without an operator grant the expected
operation status is 13. Never add a broad grant simply to make this example pass.

## Rust, JavaScript, AssemblyScript and TinyGo

Rust: from `examples/rust` run
`cargo build --offline --release --target wasm32-wasip1 --no-default-features --features PROFILE -j 1`.
Exactly one of noetic/humble/jazzy/main must be chosen. The crate compiles the
full record and service/action modules and calls the real controlled backend.
Run `turtlebot3-state.wasm ALIAS`. Outcome retains complete value, primary
and cleanup separately; the command returns primary first, then cleanup.

AssemblyScript: compile `examples/main_PROFILE.ts` with the actual
AssemblyScript compiler and `--use abort=sdk/assemblyscript/api/abortGuest`.
Use incremental runtime, an explicit memory ceiling and the repository's actual
WASI entry settings. Supply basic stdin as one nonzero u8 byte count followed
by exactly that many strict UTF-8 alias bytes. A main module selects one backend
and full codec; `lastValue()` changes only on a successful complete decode.
The exported cleanupStatus remains distinct from primary.

JavaScript: `examples/javascript-PROFILE.json` is a closed build recipe
for `sdk/javascript/guest/prepare.py` with actual original QuickJS sources,
WASI compiler, libc, builtins and validator. Execute its recorded compile/link
plan to produce a genuine QuickJS-in-Wasm command, not a Node-host substitute.
The same counted alias is basic stdin. Basic stdout contains primary and cleanup
as two little-endian u32 values. Engine exit0 does not mean ROS success.
The recipe includes complete generated records and interface facades; it never
loads native robot modules or opens a host JavaScript networking API.

TinyGo: from `examples/go` run
`tinygo build -target=wasi -tags=ros2,tb3_humble -scheduler=none -gc=leaking -panic=trap -p=1 -o /tmp/turtlebot3-go.wasm .`.
For Noetic use `ros1,tb3_noetic`; Jazzy/main use
`ros2,tb3_jazzy` or `ros2,tb3_main`. Missing, duplicate and mismatched
profile/backend tags must not select a state. Run with one explicit alias.
Copied full generated Go records/interfaces are byte-checked against the selected
canonical bundle. Do not hand-edit a generated package to bypass identity checks.

## Validation scope

`python3 -I -S -B "$KIT/tests/validate_sources.py" --source "$SRC"` checks
every original custom declaration against captured Git trees, complete member
and constant maps in all six languages, all exact products, and source selection.
C/C++ contracts execute every complete record codec, all truncation positions,
trailing-data refusal, undersized-output preservation and Sound construction.
These are authored SDK contracts, not ROBOTIS's complete official test suite.
Native ROS interoperation, firmware/model compatibility, simulator, real robot,
hardware counter hotspots and microarchitecture performance require separate
original evidence. See the delivered qualification report for actual executions.
