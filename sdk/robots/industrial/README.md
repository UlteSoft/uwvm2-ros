# Complete industrial ROS source SDKs

This source cohort provides complete original-message catalogs, typed C/C++
bindings, a caller-driven FollowJointTrajectory owner, C service/topic reactors,
and a genuine ROS2 synthetic action opponent for FANUC, KUKA, Yaskawa and ABB.
Every capability starts absent. Source preparation, successful encoding and a
Gateway acknowledgement do not establish delivery to a robot. The explicitly
scoped [FANUC r29 record](../../../documents/ros/continuation-20261018-evidence/fanuc-original-sdk-and-native-values-r29/README.md)
now captures original interface-package builds, Humble amd64 metadata/value
contracts and actual C/C++ Wasm builds with default-no-grant executions. Other
vendor instantiations and hardware scenarios remain unqualified by that record.

The five catalogs contain274 complete original definitions: FANUC ROS2(80),
KUKA ROS2(26), Yaskawa ROS2(52), ABB ROS1(53) and ABB ROS2(63). Definitions of
messages, services and actions retain every field, constant, scalar/string
default, native spelling and dependency. `source-map.json` records the actual
upstream commit/tree/path/Git blob and counted SHA256 for every definition.
`source-lock.json` additionally retains whole license/package declarations and
capture references. These are Source/DATA observations, not installed SDK proof.
The original repositories' license notices remain in each catalog's `licenses/`.
A missing upstream standalone license body is explicitly recorded; a neighboring
repository's notice is not substituted for it.

## One coherent build domain

Use the separate `uwvm-ros-vendor-complete-bounded-v1` adapter. The old vendor19
operator and old fixed/bounded generator behavior remain intact. A jointly
selected catalog must satisfy the actual128-type,32KiB record wire, depth16,
4096 expanded nodes/record and16384 nodes/catalog limits. Bounds represent a
finite accepted subset of native values; oversized values are rejected without
truncation. They do not imply a specific robot configuration. Legal fields such
as `type` remain original native/schema names, with explicit language member
maps where a language requires escaping. ROS1 byte/char and ROS2 byte/char facts
remain distinct. Constants consume no wire bytes and are checked against genuine
native typed constants before native entity creation. Declared defaults retain
presence and actual canonical value. TypeDescription alone cannot distinguish
an explicitly empty string default from an absent default. The separately
selected [installed-source Message proof](pre_rihs/README.md) can establish this
distinction on genuine older SDKs; the ordinary modern metadata path continues
to refuse that ambiguous declaration. This optional path requires actual SDK
source and typed native evidence. Complete Service/Action catalogs use the
separate [installed protocol proof](pre_rihs/PROTOCOLS.md), including the actual
legacy request/response graph and all five Action type-support associations.
Neither proof path creates an endpoint or grants Guest authority.

One physical Runtime/Gateway/C++ build domain selects **one merged, deduplicated
catalog with common reviewed bounds**, one manifest, one records revision and
one matching native-header revision in every translation unit. Independently
generated bundles are not concatenatable: native-name-based C symbols, layouts
and native traits can conflict even when constant namespaces differ. A single
Wasm can use several vendors through separate aliases in one jointly admitted
domain. Type count or aggregate limits still apply to that joint catalog.

The C/C++ aliases in `binding_ros*.hpp` refer to actual generated portable values,
never host ROS object layouts. `bounded_records.h` exposes full C structs with
counted text/arrays, `_valid`, `_wire_size`, `_encode` and `_decode` functions.
`bounded_records.hpp` supplies C++ aliases and `roswasmcpp::Codec<T>`. Generated
`bounded_interfaces.*` preserve all selected service/action components. The
complete adapter also implements the existing six record/interface targets
(C, C++, Rust, JavaScript, AssemblyScript, TinyGo); this kit's concrete controller
examples are C/C++. Other-language robot applications must use those actual
products and their actual SDK transport, not opaque bytes or a NativeNode in a
Guest. The separate27-language effort supplies additional compilers/SDKs; this
cohort does not claim those executables or qualifications.

## Prepare, export and bind

Perform these stages only under the controller's explicit implementation/build
authorization, in fresh owned output directories. They are not run by this
source package. From an exact SDK Source revision that contains the independently
reviewed shared adapter, prepare one original catalog:

```sh
python3 -B sdk/robots/industrial/prepare.py \
  --source-root /absolute/reviewed/uwvm2-ros \
  --catalog /absolute/reviewed/uwvm2-ros/sdk/robots/industrial/fanuc/catalogs/ros2/catalog.preparation.json \
  --parent-sha256 1a73d6a87624e563bbb6be2624e77b51d7dba0f0f2fb524cda4079f493381826 \
  --bounded-sha256 40e30d542283cecad4ca210599dbc6d96259f774a1c778050920b32dbe4a0bf4 \
  --adapter-sha256 ACTUAL_REVIEWED_ADAPTER_SOURCE_SHA256 \
  --mode prepare --out /fresh/owned/fanuc-source
```

The SHA placeholder must be the actual independently reviewed adapter file;
it is not an identity to paste unchanged. The operator admits captured source
bytes, actual four interface templates, complete original definitions and a
fresh output. It never downloads, configures, compiles, starts a process or grants
an alias. `generation.vendor.json` is Source provenance. Its preparation manifest
has no fabricated native MD5/RIHS. The Root `vendors/complete_prepare.py` source
API can additionally admit the multi-repository `source-lock-v1` through its
explicit controller-provided read/load functions; it has no invented standalone
CLI in this guide.

Build the emitted `native_identity.cc` against every genuine selected native
package and matching complete-vendor native-domain provider. It emits the exact
`uwvm-ros-industrial-native-v1` schema, full native type/field/default metadata,
constant schema and actual identities. The original installed generated headers,
type-support libraries, compiler/link options, complete dependency files, process
owner, stopped executable/library maps and bounded exporter output must be
independently captured. The exporter Source is not that evidence. Then use the
same original catalog with `--mode bind --native-metadata /actual/export.json
--native-owner-report /actual/owner.json` and a different fresh `--out`. It copies
real identities only after complete association checks; an old ROS1 refusal or
old generated manifest is not relabeled as a successful new-domain product.

All native-side consumers use that one bound manifest and generated native
headers. The matching Gateway complete-vendor descriptor/Codec/Native trait
admission is necessary; a generated header alone does not install a provider.
The ordinary modern ROS2 metadata path requires the actual TypeDescription API.
An older SDK requires the separately selected installed-source/protocol proof,
actual generated SDK and complete typed/default/association witnesses. Missing
or broken modern metadata never triggers that legacy path implicitly. ROS1 uses
the complete original Definition/MD5 path plus genuine typed constants. Actual
native link/field/value/refusal witnesses remain required in both cases.

## Build the real Wasm examples

`examples/CMakeLists.txt` is deliberately inactive by default. Select
`UWVM_INDUSTRIAL_BUILD_GUEST=ON`, the actual WASI C/C++ toolchain/sysroot, one
vendor, ROS major and the fresh same-source generation directory. A representative
future command is:

```sh
cmake -S sdk/robots/industrial/examples -B /fresh/owned/industrial-wasm \
  -DCMAKE_TOOLCHAIN_FILE=/actual/wasi-sdk/share/cmake/wasi-sdk.cmake \
  -DUWVM_SOURCE=/absolute/reviewed/uwvm2-ros \
  -DUWVM_INDUSTRIAL_VENDOR=fanuc -DUWVM_INDUSTRIAL_ROS_VERSION=2 \
  -DUWVM_INDUSTRIAL_GENERATION=/fresh/owned/fanuc-bound/generated \
  -DUWVM_INDUSTRIAL_BUILD_GUEST=ON
cmake --build /fresh/owned/industrial-wasm --parallel 1
```

This emits `industrial_c.wasm` and, for ROS2, `industrial_trajectory.wasm`.
The C++ WASI profile explicitly disables exceptions and RTTI; numeric ROS errors
remain available, while allocation exhaustion follows libc++ termination. The
shared fixture uses checked counted-value copies between distinct generated
field types. It propagates capacity failures without modifying the destination
and rejects insufficient two-joint/two-point capacities during compilation.
Both reactors reserve a 1 MiB stack, start with 2 MiB linear memory, have a 32 MiB
linear-memory ceiling and use a serial linker. They use memory0, fixed SDK raw imports and portable owned canonical
values. No native pointer/ABI is passed to a Guest. Root owns actual toolchain,
constructor/entry wrapper, stack/linear-memory reservations and import/signature/
maximum-memory checks. The `--no-entry` products export explicit reactor
functions; they are not a fabricated `uwvm2 --reactor` CLI. An actual admitted
Runtime embedding must initialize C++ constructors and call these exports under
the real caller/module/source epoch. Its driver must preserve Wasm storage and
Host callback lifetimes through the final close.

For `trajectory.cpp`, call `initialize_case(0)` first in an independent denied
instance. Require13 at the attempted first open, `denied_verified()==1`, no live
owner, and zero independent Native requests/commands. The positive instance uses
`initialize_case(1)`, `submit_once()` exactly once, bounded `step_once()` calls,
optional `cancel_once()` for this owned goal, getters and `close_case()`.
`goal_low/high` preserve all64 bits; a reused/truncated ID is rejected. Complete
Goal, Feedback, Result and JointState values are validated. A cancel ACK is not
terminal completion. Canceled result uses local125 while retaining real
controller `error_code`, terminal kind and complete error string. Unknown submit,
transport failure or timeout never triggers replay. `step_once` has a4096-pass
work budget, not a nanosecond clock. Physical timeout requires a real external
steady-clock deadline and a true Host cancellation/drain contract.

`include/action_owner.hpp` preallocates the caller's goal history and five owner
slots before the first open. It retains actual constructor rollback and all
five cleanup statuses. `trajectory_lifecycle.hpp` keeps primary/cleanup/status
and uncertainty separate. Unwinding/close does not revoke another process or
certify physical stop. Each C query/status reactor likewise opens only its
explicit alias, sends/observes once and retains primary/cleanup separately.

## Permission CLI and Native evidence

Each vendor's `permissions.fixture.json` contains literal current single-module
CLI argument vectors. Default authority is `[]`. The denied vector configures
transport only, with zero grants. The positive vector uses explicit
`--ros2-single-grant main ALIAS OPERATION RESOURCE TYPE TYPE_IDENTITY` and
`--ros2-single-set main ALIAS KEY VALUE`; type/codec placeholders must be replaced
by the actual same-bound products. They are intentionally not a permissive
ready-to-run permission file. Actual main-module identity, source generation,
authenticated Gateway socket/peer UID, resource, codec, quota, queue and all8 QoS
properties are required. Clock/log/graph/env rights are never implied.

The five Action channels use real independently configured reliable/volatile/
keep-last/depth8 profiles with infinite deadline/lifespan/lease and automatic
liveliness. This fixture configuration is not an attestation of the controller's
actual middleware profile. Physical driver QoS must be observed and explicitly
selected; Yaskawa's real sensor-data status stream is best-effort, for example.

Build the separate synthetic native fixture with
`UWVM_INDUSTRIAL_BUILD_NATIVE_FIXTURE=ON` in a genuine modern ROS2 host build,
`UWVM_FAST_IO_INCLUDE`, the same bound generation directory,
`UWVM_INDUSTRIAL_NATIVE_PACKAGES` listing every actual generated package, and
`UWVM_INDUSTRIAL_NATIVE_PACKAGE_TARGETS` listing its actual imported type-support/
introspection/generator targets. CMake finds declared packages and checks real
target existence; it does not invent an SDK or run the fixture. The fixture
always addresses `/uwvm_industrial_fixture/*`, has no robot IP, and explicitly
selects native quiet Node options including `start_type_description_service=false`.
It stops itself before Node creation. A trusted controller admits its original
process then sends the existing `I` gate, `M` to allow traffic and `F` to finish.
Modes are `success`, `cancel` and `denied`. Full native values and exact once-only
goal/cancel/feedback/terminal counts are checked. A marker is not proof: retain
actual Native callbacks, values, DDS/source owner evidence and denial counts.

Debug observation must use the actual Runtime DBG admission/all-stop/typed-memory
APIs, with the exact current source/module/epoch and real owner lifetime. The
Guest exports counters and complete portable values, never a debug authority
setter. Read-only metadata observations are not pause/checkpoint/replay. At this
cohort, genuine ROS-provider DBG typed-memory admission is a separately assigned
implementation gap; neither `-Rdbg` nor a textual snapshot claim closes it. A
future independently reviewed ROS sealed loader census is required before such
an observation is claimed. Native stop/cancel/denial evidence is independent of
that debug feature. Never enable checkpoint/cache/restorer for ROS just to make
a debugger read succeed.

Hardware qualification is a separate controlled run with the exact controller,
firmware, model, joint order/units, limits, installation and stop supervision.
Never send the synthetic two-joint/MultiDOF fixture to a physical controller. Use
actual official model/controller configuration and complete current JointState;
change the typed Goal under review, regenerate the coherent permission catalog,
and capture native acceptance/result and independently witnessed effects.

## Source contracts and remaining qualification

`tests/ros_industrial_vendors/trajectory_contract.cc` exercises actual local
Journal behavior: full64 IDs, duplicate submit, cancel ACK vs terminal,
controller failure, uncertainty, timeout and independent cleanup results. It
imports no Native ROS model. CMake adds no tests or auto-run step. All these
contracts are authored/unexecuted; they do not replace Wasm construction, original
SDK full-header/link proof, whole dependency files, native communication,
permission refusal, sanitizer/resource, timing or hardware qualification.
Per-vendor READMEs identify real controller and recovery boundaries.
