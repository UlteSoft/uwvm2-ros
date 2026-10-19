# Original rclcpp default-Node control source

This explicit source profile preserves the original default-Node endpoints
while rebuilding original client libraries for a Wasm Guest. Canonical C/C++
source, mixed C/C++ Guest codecs, independently typed C++ Gateway codecs and
Host registration are separate implementation and qualification roles. The
current source integrations and their exact receipts are recorded in
[the implementation ledger](../required-work.json). No generation, SDK build,
Guest/native delivery, official suite or performance pass is implied here.

The original rclcpp commit is
`53fa7aeea86e8ed484f9465fa14059faf13a71f1`. Original NodeOptions remain unchanged.
The default closure has Log and ParameterEvent publishers, the TimeSource's
ParameterEvent subscription, six parameter Services and GetTypeDescription.
TimeSource attaches its ParameterEvent subscription even when use_sim_time is
false; omitting that dependency does not implement a default Node. The genuine
Guest rosout Log and GetTypeDescription objects are **C** objects; ParameterEvent
and the six parameter Services are **C++** objects. The separate Native Gateway
uses its own genuine C++ objects and inspectors for the corresponding endpoints.
Object language, layout and ownership cannot be inferred from a matching name.

`catalog.preparation.json` contains25 original source definitions for32 canonical
records. `original/` retains52 acquired source/license/context files, including
40 msg/srv definitions. Additional graph/logger source inventory does not select
endpoints. Logger Services are disabled by the original default NodeOptions;
other application-selected defaults, clocks and threads need their own complete
providers and explicit authorities. No setting is made quiet to pass a test.

## Exact source and scalar metadata

The explicit format is `uwvm-ros-original-node-control-c-cpp-v1`. It admits only
40 exact selected ROS2 source names with full source byte counts/SHA-256 values.
Source comments, scalar constants and declared defaults are retained. Changing
or erasing a constant/default/comment is a source-pin refusal, even when RIHS
excludes that difference. The source commit is
`614f3c4942e1a67a568546cb805ff3e0ff7777f5`, tree
`1fc8125d7a10587a81e083994142d313ad1903db`. This source selection is not a claim
that a currently installed native SDK was built from that commit.

The legal original field `type` stays `type`: this profile implements C/C++ only
and does not apply an unrelated Rust/Go/JS keyword union. Existing profiles keep
their original rules and outputs. No managed-language control source is emitted.
Actual source constants are emitted as type-prefixed C macros and retained in
the canonical/source IR. They are not instance fields and consume no wire bytes.
Examples include Log's five levels, ParameterType values, FieldType's 88
constants and ListParameters Request's DEPTH_RECURSIVE.

The actual original rosidl_adapter maps `.msg char` to IDL **uint8**, and `.msg
byte` to IDL **octet**. Both use unsigned one-byte canonical storage, but the
original spelling remains explicit `native_scalar` metadata and participates in
this profile's canonical identity. Both native adapter generators preserve and check
char/uint8 FieldType ID3 versus byte/octet FieldType ID16 and the corresponding
real introspection type. It must not project byte metadata to uint8 merely
because widths match. ServiceEventInfo's char[16] is actual IDL uint8[16].

All fields are retained, including inactive ParameterValue variant fields,
complete GetTypeDescription response sources/extra information and nested
references. A successful flag never permits dropping otherwise-undefined
fields. Scalar defaults are declared initialization only: ParameterDescriptor
false/false, FieldType.type_id=0 and GetTypeDescription.include_type_sources=true
are not constraints requiring those runtime values. Decoding requires every
field on the wire and never fills missing fields from a default.

## Complete values and finite resource admission

Canonical strings remain strict UTF8 with NUL forbidden, except the explicitly
selected `TypeSource.raw_file_contents` field. That field is counted raw bytes:
all bytes, including NUL and non-UTF8, are preserved. The pinned original
source getter declares a counted view whose last byte is NUL for these selected
ASCII/final-LF definitions. A strlen projection would lose a byte. Source-file
SHA and actual SDK getter view are separate facts and are independently checked.

The source profile preserves native declarations and all active/inactive fields.
Its explicit capacities include10 fields,11 references,12 source rows and6162
bytes per raw source. GetTypeDescription.Request.type_hash holds the complete
71-byte `RIHS01_` selector; an empty selector is also representable. All original
scalar defaults and constants remain unchanged. include_type_sources=true is a
declared initialization, not a constraint that every runtime value must be true.
The true-source native witness requests and compares the full11-reference and
12-source response instead of clearing that field to avoid the source payload.

TypeDescription and GetTypeDescription.Response have explicit32768-byte total
canonical wire quotas. Their complete Cartesian capacities are retained
separately. An overlarge combination refuses90; it is never truncated. Existing
node/depth/work/overflow admission remains in effect. A32768 wire quota is not
an inline sizeof, stack, Native allocator, DDS queue or process-memory bound.
The generated private full-record temporary needs actual target measurement.
Actual Native serialization must independently retain counted source bytes;
source review cannot establish that delivery property.

## Generate canonical source explicitly

These commands are authored instructions for the later unified test phase.
Admit the real interpreter, parent/bounded generator source and complete input
closure first. Use a fresh owned output. Local test/build outputs belong in
`/tmp`; Linux work must stay inside the owning64GiB, swap0 cgroup and its real
backing-space/deadline budgets. No tool is run by reading these instructions.

```sh
python3 -I -B sdk/tools/custom_types.py \
  sdk/rmw_wasm/default_control/catalog.preparation.json --check
python3 -I -B sdk/tools/custom_types.py \
  sdk/rmw_wasm/default_control/catalog.preparation.json --guest-only \
  --out /tmp/OWNED-FRESH-default-control-canonical
```

For this extended profile the canonical stage emits8 files:
`bounded_records.h/.hpp`, `bounded_interfaces.h/.hpp`, `control_wire.hpp`,
`control_manifest.hpp`, `control-schema.json` and `generation.json`. It emits
neither real SDK objects nor a Native Registry. Native emission through this
canonical command remains refused; use the separate actual-header adapter
operators. Managed control emitters remain required implementation work.

Preparation identities are blank and the common control table is deliberately
unavailable. For an available table, supply the separately acquired genuine
Native identities through the documented explicit source-generation selection.
The generated C++ common manifest uses `UWVM_ROS_ORIGINAL_CONTROL_MANIFEST` and
its own namespace. A separately selected vendor manifest can coexist under the
original macro. Complete combined count/node/duplicate/identity validation
poisons the whole lookup on any invalid row. The control marker is trusted source
metadata, never a Guest grant or substitute for actual Native proof.

The full ParameterEvent canonical topic is selected only for topic.publish and
topic.subscribe. The existing param.* event projection retains its separate
complete schema, identity and codec. Names alone must not route those different
wire domains into one another. See the actual
[common-registry contracts](../../../tests/ros_control_common_registry/README.md).

## Compose the real Guest and Native sides

[The Guest native-adapter guide](native/README.md) documents the separate
SHA-paired generator, real C/C++ metadata/layout/default/source checks,
independent Native exporter/freezers, address-aware codec composer and original
Node example. The old pure-C Registry is not silently replaced. The mixed
Registry commits Source-lifetime retention when its descriptor pointers become
public; a published Registry cannot be destroyed merely because a callback
counter reached zero. Actual Source/Group/native-owner retirement is required.

[The Gateway control guide](../../../gateways/ros2/default_control/README.md)
documents the separate all-C++ proof and real typed publisher/subscriber/service
factories. The [Host profile guide](../../../gateways/ros2/README.default_control_profile.md)
describes independent frozen inputs, one complete linked catalog environment,
explicit storage selection and operation-aware owned routing. Those source
packages have their own integration/review status in the ledger; a link in this
guide does not confer build or execution qualification.

Original ParametersQoS and ParameterEventsQoS retain Native DDS depth1000.
Canonical service lifetime history stays independently bounded at1..256 and
must cover max-inflight and its full owned byte/observer budgets. The admitted
numeric1000 Native-depth ceiling is not a Native receipt or physical memory
proof. Actual queried same-object QoS/GID and independently owned middleware
memory must still be verified. Old fixed/vendor storage behavior stays coupled
unless its own complete factory selects the new bounded model.

The original-rclcpp build uses the explicit
[composite source entry](../original_build/README.md), with the real threaded
static WASI core/platform/dispatch, genuine C lifecycle helper and exact fixed
original ports. Its build option defaults OFF. A source marker or successful
configure does not establish full linking or unchanged default Node operation.
Signal mapping, Host debugger admission, producer/import closure and physical
retirement remain independent runtime requirements. The Guest cannot update
modules, configure debugging or change Host permissions through these APIs.

## Required qualification

[STAGE2_REQUIRED.md](STAGE2_REQUIRED.md) separates implemented source obligations
from actual acceptance. [The complete source-domain contracts](../../../tests/ros_control_source_domain/README.md)
cover raw bytes and aggregate quotas; `contracts/type_hash_capacity.c` covers
empty/full71-byte selectors and transactional malformed/short/quota refusal.
The Native object/default-Node/true-source fixtures are genuine SDK consumers,
not fake TypeSupport providers. Every authored contract is unexecuted until a
real owned harness produces a fresh result against the exact current source.

Regenerate all canonical/native/Guest/Gateway consumers together after any IR
or codec identity change. Preserve real compiler/link dependencies, actual
independent exports, full messages and source bytes, default denial, granted
operations, timeout/cancel/retirement behavior and exact resource cleanup.
Full requested distributions/vendors/architectures, official upstream suites,
QEMU simulation and native/VTune performance remain separate open obligations.
