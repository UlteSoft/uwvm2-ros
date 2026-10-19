# Clearpath complete selected platform SDK

This kit preserves complete original custom interface declarations from four
source snapshots. Noetic additionally includes unchanged legacy Husky and Jackal
message packages. It supplies full bounded C, C++, Rust, JavaScript,
AssemblyScript and TinyGo records and service/action facades for those domains.
Original source comments, constants, component fields and dependencies remain
unchanged. Main is a development snapshot, not a released ROS distribution.

| Complete source domain | Original definitions | Complete records | Original custom roots | Constants |
| --- | ---: | ---: | ---: | ---: |
| noetic | 225 | 371 | 208 | 239 |
| humble | 17 | 18 | 14 | 85 |
| jazzy | 35 | 42 | 32 | 256 |
| main | 13 | 14 | 10 | 76 |

Each `catalogs/DOMAIN/source-lock.json` records exact original message,
driver and standard dependency commits, whole bodies/blob hashes and all bounded
quotas. The Noetic source includes the OutdoorNav configuration, control,
docking, logging, localization, mission management/scheduling, navigation,
safety and camera packages as well as platform, Husky and Jackal records.
The original `q62AreaZoom` service retains its lowercase ROS1 spelling.
Both the original Mission message and Mission Action remain present. Their
ROS/native names are unchanged; only our original-exporter JSON association
uses `message:PACKAGE/Mission` and `action:PACKAGE/Mission` when needed
to keep those distinct root kinds from overwriting one another.

These source domains do not certify every Clearpath robot, sensor stack,
robot.yaml, firmware, ROS release, controller or native API. Match original
installed middleware and model evidence independently before a positive run.

## Explicit application and model selection

Six mutually exclusive application selections are provided. They do not
autodetect a robot or substitute a similar-looking status message.

| Selection | Complete source bundle | Whole observed original type |
| --- | --- | --- |
| noetic | clearpath-noetic-complete | clearpath_platform_msgs/Status |
| noetic_husky | clearpath-noetic-complete | husky_msgs/HuskyStatus |
| noetic_jackal | clearpath-noetic-complete | jackal_msgs/Status |
| humble | clearpath-humble-complete | clearpath_platform_msgs/msg/Status |
| jazzy | clearpath-jazzy-complete | clearpath_platform_msgs/msg/Status |
| main | clearpath-main-complete | clearpath_platform_msgs/msg/Status |

The three Noetic selections deliberately use one complete original graph.
The selected application's state type is explicit. The full bundle still
includes all original records and service/action methods, rather than a copied
prefix of the status structure. A legacy Husky velocity command uses its actual
Twist endpoint; a platform Drive value is not silently substituted for that API.

## Start at the raw controlled import boundary

Read `sdk/include/roswasm/abi/ros1.h` or `abi/ros2.h` and the matching
`roswasm/ros1.h` or `ros2.h`. The C entry has explicit `raw ALIAS`
open/close and `state ALIAS` subscribe/poll operations. An operator alias
is required. Importing, constructing or compiling anything here grants no ROS
authority. Default ungranted open must refuse13 without publishing a handle.

The operator must independently bind the actual module/backend, endpoint,
direction, original native/canonical identities, operations, QoS and finite
limits. The SDK cannot infer grants, discover credentials, enable a native
socket/serial/CAN device, install robot.yaml, start services or change a host
debugger, updater, process environment or permission policy. Configuration,
camera, mission and watchdog records are data, not grants to administer them.
External robot safety supervision remains necessary for physical operation.

`ros-vendor-clearpath` defaults OFF independently of ROS1/ROS2 import
options. A positive native build requires the actually bound coherent manifest
and separately selected native Gateway dependencies. Use the repository
A+B configuration printer to clear all other vendor switches. Merely adding
these SDK source directories does not link every robot SDK into uwvm.

## Progress to full typed values and operations

C `include/state.h` borrows an already explicit capability and caller scratch
for a full transactional state decode. Keep scratch/output disjoint and serialize
access to the capability. C++ `state.hpp` adds a borrowed Reader and the
complete generated service/action facades. Include `bounded_interfaces.h`
or `.hpp` for original typed requests, responses, goals, feedback and terminal
results. They keep acquisition, advertisement, reply, cancel, result and cleanup
as separate operations. A result/acknowledgement is not physical completion.

`uwvm_clearpath_make_wheel_velocity` constructs the original platform
Drive message in wheel radians/second, with an explicit reviewed model limit.
It checks finite inputs and the rounded float32 values, retains the original
velocity mode and both wheels, and preserves the destination on refusal.
It never sends, acknowledges, retries, resumes or performs an emergency stop.
It does not infer model limits or convert that value to legacy Husky Twist.

All six generated record modules expose every original member and constant.
The generated service/action modules use the actual controlled ABI, retain
owned lifetimes and separate primary/cleanup/terminal outcomes. Rust uses the
real SDK Backend/Capability/Canonical; JavaScript uses an actual copied Session;
AssemblyScript uses the selected NativeApi; TinyGo uses the selected Wasm ABI.
No host-native ROS language package replaces a Wasm import.

## Complete bounded data and explicit refusals

Frame/name strings have a 128-byte UTF-8 bound; other strings use 256 bytes,
or the smaller original declared bound. Variable sequences normally hold two
items; motor driver sequences use four and GNSS cn0 uses 64. Fixed original
arrays and constants remain exact. These are explicit bounded SDK profiles,
not changes to the unbounded ROS types. Excess is refused without truncation.
Review and regenerate a different finite domain when a larger mission is needed.

StorageState has an additional aggregate canonical wire cap of 32,768 bytes.
All its fields remain represented. Its full inline/cartesian storage is distinct
from its accepted wire quota; the largest cartesian record in the Noetic domain
is 71,156 bytes. The complete domain has 13,263 expanded nodes, below the
unchanged 16,384 total ceiling. Per-record 4,096-node, depth16 and 32 KiB wire
limits remain. The paired industrial preparer admits at most 512 interfaces;
this complete source has 225. The host cgroup and compiler limits are separate.

The original Noetic PlatformConfig.msg contains only a placeholder comment and
no fields. Its unchanged empty record is retained as data; it is not promoted to
an operational native topic. This upstream placeholder is explicitly recorded
in the lock. No configuration API or missing fields are invented for it.

Error11 is idle, 13 is permission denied, 22 is invalid input and 90 is bounded
storage refusal. An error does not authorize replay, resize, recovery or resume.
Keep primary and cleanup separately; closing a resource is not stopping motion.

## Prepare original source and bind native identities independently

Set absolute `SRC`, `KIT=$SRC/sdk/robots/mobile_bases/clearpath/platform`,
`DOMAIN=noetic|humble|jazzy|main` and a fresh output outside the source tree.
Use actual SHA256 values of the reviewed producer sources:

```sh
python3 -I -S -B "$SRC/sdk/robots/industrial/prepare.py" \
  --source-root "$SRC" --catalog "$KIT/catalogs/$DOMAIN/catalog.json" \
  --out /tmp/clearpath-prepared --mode prepare \
  --parent-sha256 "$PARENT_SHA256" --bounded-sha256 "$BOUNDED_SHA256" \
  --adapter-sha256 "$ADAPTER_SHA256"
```

Preparation grants nothing and emits no native registry. To bind real endpoints,
retain original independent SDK headers, definitions, complete native metadata,
owner reports and dependencies and use the actual `--mode bind` workflow.
Do not fabricate native identities or substitute source hashes for native MD5,
TypeDescription or source/owner proof. Existing builtin standard identities are
not registered again as new topics. Source/native records must form one coherent
domain across Guest, Runtime, native Gateway and selected installed middleware.

Use `tools/select_profile.py --kit "$KIT" --bundle "$BUNDLE" --profile "$DOMAIN"`
on an exact newly prepared/bound bundle without an existing selection record.
It verifies the full original definitions and all exact products. It associates
source, not authority or test evidence. Prepared bundles have 14 products;
actually bound bundles have 16 including the real manifest/native adapter.
The checked-in source bundles already contain exact profile-selection records.

## Genuine C/C++ applications

Choose `APP` from the six explicit selections above and its corresponding
`DOMAIN`. Set `BUNDLE=$SRC/sdk/custom-types/clearpath-$DOMAIN-complete`.

```sh
cmake -S "$KIT" -B /tmp/clearpath-c \
  -DUWVM_CLEARPATH_BUILD_CONTRACTS=ON \
  -DUWVM_CLEARPATH_PROFILE="$APP" \
  -DUWVM_CLEARPATH_SOURCE_ROOT="$SRC" \
  -DUWVM_CLEARPATH_GENERATED="$BUNDLE"
cmake --build /tmp/clearpath-c --parallel 1
```

All build targets are OFF by default. Missing/mismatched application, complete
source bodies, original lock or product bytes are refused. For real WASI entries
add the actual WASI toolchain, `-DUWVM_CLEARPATH_BUILD_WASI_EXAMPLES=ON` and
`-DCMAKE_EXE_LINKER_FLAGS=-Wl,--threads=1` at initial configure time, including
compiler identification. Run `clearpath_guest_c.wasm` or `_cpp.wasm`
with `raw ALIAS` or `state ALIAS`. Success writes the whole canonical
observation to basic stdout; an ungranted alias must refuse13.

## Genuine Rust, AssemblyScript, TinyGo and JavaScript applications

Rust: from `examples/rust` build
`cargo build --offline --release --target wasm32-wasip1 --no-default-features --features APP -j 1`.
Exactly one application feature is required. The full matching record and
service/action modules are compiled. Run `clearpath-state.wasm ALIAS`.
Outcome retains the full selected value, primary and cleanup independently.

AssemblyScript: compile `examples/main_APP.ts` with the actual compiler,
incremental runtime, explicit memory limits and
`--use abort=sdk/assemblyscript/api/abortGuest`. Basic stdin contains one
nonzero u8 byte count followed by that many strict UTF-8 alias bytes. The selected
main module chooses an actual backend and whole original codec. lastValue changes
only on successful full decode; cleanupStatus stays separate.

TinyGo: from `examples/go` build
`tinygo build -target=wasi -tags=ros2,clearpath_jazzy -scheduler=none -gc=leaking -panic=trap -p=1 -o /tmp/clearpath-go.wasm .`.
Use `ros1,clearpath_noetic`, `ros1,clearpath_noetic_husky` or
`ros1,clearpath_noetic_jackal` for the respective original Noetic state;
Humble/main use `ros2,clearpath_humble` or `ros2,clearpath_main`.
Missing, duplicate and backend-mismatched tags must refuse selection.
Run with one explicit alias. Copied complete records/interfaces are verified
against their canonical four generated bundles.

JavaScript: use the closed `examples/javascript-APP.json` recipe with
`sdk/javascript/guest/prepare.py`, actual original QuickJS sources, WASI
compiler/libc/builtins and validator. Execute its recorded compile/link plan
to obtain an actual QuickJS-in-Wasm command. It receives the same counted alias;
basic stdout contains primary and cleanup as two little-endian u32 values.
Engine exit0 is not ROS success. Host Node/networking modules are not substitutes.

## Validation scope

`python3 -I -S -B "$KIT/tests/validate_sources.py" --source "$SRC"`
checks every original custom declaration, blob, component field, constant,
six-language map, product and exact profile association. Authored C/C++ whole-value
contracts cover every complete record, all truncation positions, trailing-data
refusal, undersized-output preservation and pure wheel-value construction.
These are not the upstream official suite. Keep genuine Guest results, narrowly
granted native observations, model/firmware/controller/simulator qualification,
requested x86/ARM Runtime tests and equivalent-work VTune hardware measurements
separate. See delivered qualification metadata for what actually ran.


## Reproduce independent full-field data checks

The test generator deliberately does not import the codec generator or its
field IR. It parses unchanged original declarations, selects deterministic
nonzero field values, and packs separate little-endian reference bytes with
Python struct. Every named decoded field is checked before re-encoding in the
managed tests; inverse encoder/decoder mistakes cannot hide behind a round trip.
These synthetic values are never sent to a robot and do not establish physical
command validity, native identity or upstream official-suite acceptance.

Within an admitted Linux test workspace, freeze the actual source at
`$WORK/source` and set `SRC=$WORK/source`. Generate each original
domain, then create bounded compilation units:

```sh
for DOMAIN in noetic humble jazzy main; do
  python3 -I -S -B "$KIT/tests/generate_golden_vectors.py" \
    --source "$SRC" --profile "$DOMAIN" \
    --out "$WORK/golden-vectors-$DOMAIN"
done
python3 -I -S -B "$KIT/tests/generate_managed_golden.py" \
  --source "$SRC" --vectors-root "$WORK" --out "$WORK/managed"
python3 -I -S -B "$KIT/tests/split_managed_golden.py" \
  --inputs "$WORK/managed" --out "$WORK/batches"
```

The independent C/C++ batch files include the real selected bounded_records.h.
Compile with actual C11/C++20 native or WASI tools and run assertions enabled.
The generated Rust programs use the actual Canonical.decode and zero-argument
Canonical.encode Vec API. Go uses the actual DecodeSymbol and value.Encode API.
Each AS batch re-exports the genuine SDK abortGuest; use an explicit common
`--baseDir "$WORK"` and
`--use abort=source/sdk/assemblyscript/api/abortGuest`. Preserve the computed
relative SDK imports when moving a batch; the supplied splitter relocates them.
JavaScript batches must be embedded with the actual copied source producer,
closed manifest, headers and import-checker closure, then executed in QuickJS
inside Wasm. A Node-host test is not an equivalent qualification.

Use one compiler job and retain the hard cgroup and per-process limits. The
32-record batches cover the whole graph while avoiding a monolithic Rust LLVM
optimization job exceeding the 2 GiB compiler virtual-address limit. Do not
disable assertions or lift resource guards to make a failing test pass. Retain
original failed attempts separately from actual successful executions.
