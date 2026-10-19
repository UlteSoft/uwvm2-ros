# Official bounded custom type preparation and real Runtime acceptance

This directory implements the new finite custom Message/Service/Action workflow.
Its source-prepared controllers are executed only by the root's unified serial
Linux lane after the complete implementation freeze. Source review or generated
code is not a test result. Earlier fixed-v1 packages/products, raw failures and
receipts stay unchanged. The final Worker/Gateway must include both profiles.

The shared `tests/ros_acceptance/lane.py` requires Linux root inside a separate
mount namespace under the original64GiB cgroup with zero swap, current boot/leaf
identity, current QEMU/binfmt lease, ancestor OOM counters and explicit container
StartedAt. It preserves the complete frozen source and original SDK, owns exact
pidfd process births, checks finite RSS/log/deadline bounds, proves actual stopped
ARM mappings under the pinned host QEMU and cleans all owned processes/mounts/
sockets on success and failure. No controller changes an original SDK or accepts
historical64GiB evidence as current proof. Invoke each stage serially under the
parent lease; no controller installs/downloads an SDK or compiler.

## SourceA: official package, identity and schema derivation

First freeze the complete implementation as SourceA. Its official definitions
are in `ros1/uwvm_ros_bounded_v2` and `ros2/uwvm_ros_bounded_v2`: Point/Envelope
messages, Echo/Empty services and Digest Action. The Noetic package uses official
catkin/message_generation; Jazzy uses official rosidl_default_generators. Separate
catalog templates contain bounds and no invented native identities.

The preparation entry is:

```sh
python3 -B SourceA/tests/ros_custom_types/bounded-v2/prepare.py \
  --mode derive --source /absolute/SourceA --manifest /absolute/SourceA-manifest.json \
  --sdk-workspace /absolute/original-sdk --out /absolute/fresh/derive \
  --qemu /absolute/pinned/qemu-aarch64 --binfmt-evidence /absolute/current-lease.json \
  --container-started-at EXACT_CURRENT_STARTED_AT \
  --fixed-provider /absolute/SourceA/tests/ros_custom_types/deployment-r1/generated
```

`derive` copies each existing `/opt/uwvm-custom-types` prefix into its own output
and binds that copy into its private native root. The original root is unchanged.
Both official packages configure/build/install serially at the same explicit
logical prefix. The compiler/CMake SDK tools, raw configure/build/install logs
and whole original SDK inputs are bound. The sibling CMake project then builds
`bounded-native-identities` and `bounded-native-peer` with C++17, explicit thread
linkage and original full-MD/final-MF object recipes. Its original verbose compile
and link commands, generated headers, scalar link inputs and actual tool mappings
are retained. Compiler command lines alone are not architecture proof.

The exporter stops before reading control `I`; the controller proves its actual
ARM mapping and current process birth before releasing it. It exports exact
root/component official identity and complete definition/type-description data.
It creates no ROS Node or entity. This output is not a replacement for the
official generator. ROS1 Service component MD5 values follow actual generated
traits. ROS2 empty components keep their actual dummy native field; their
canonical component remains empty.

The closed pure derivation commands, invoked by `prepare.py` after qualification,
are the following. Every output directory is new; `bind_catalog.py` requires its
output parent not to exist.

```sh
python3 -B SourceA/tests/ros_custom_types/bounded-v2/bind_catalog.py \
  --template SourceA/tests/ros_custom_types/bounded-v2/ros1/catalog.preparation.json \
  --native-metadata /qualified/ros1-export.stdout --output /fresh/bound1/catalog.native.json
python3 -B SourceA/tests/ros_custom_types/bounded-v2/bind_catalog.py \
  --template SourceA/tests/ros_custom_types/bounded-v2/ros2/catalog.preparation.json \
  --native-metadata /qualified/ros2-export.stdout --output /fresh/bound2/catalog.native.json
python3 -B SourceA/sdk/custom-types/combine_catalogs.py \
  --input /fresh/bound1/catalog.native.json --input /fresh/bound2/catalog.native.json \
  --output /fresh/combined
python3 -B SourceA/sdk/tools/custom_types.py /fresh/combined/catalog.json --out /fresh/bounded
python3 -B SourceA/sdk/custom-types/compose_profiles.py \
  --fixed SourceA/tests/ros_custom_types/deployment-r1/generated \
  --bounded /fresh/bounded --output /fresh/composite
```

The final composite has26 regular files: unchanged fixed10, unchanged bounded12,
three materialized provider/manifest wrappers and composition.json. Generation
hash inventories are checked before copying. Topic arrays are renamed/combined;
Service/Action tables remain complete. Native macro rows are physically parsed
and materialized, preventing delayed macro overwrite. No alias grants result.

## New complete SourceB and separately derived complete SDK

The trusted parent copies the entire derived26-member composite to the NEW
SourceB path `tests/ros_custom_types/deployment-final/generated`. It also appends
the exact native-bound catalogs, source definitions, actual metadata exports and
preparation/derivation receipts under a NEW closed derivation subtree. The parent
must record the SourceA manifest and derivative file hashes and freeze the full
SourceB with `tests/ros_acceptance/freeze_source.py --extra ...`. A mutable output
header named only by path is not accepted. Original SourceA remains immutable.

For each backend the parent separately copies the whole original SDK to a NEW
SDK workspace and overlays only the exact `derived_prefix[version].root` at
`/opt/uwvm-custom-types`. The complete prefix inventory is in the derivation
report. All old official fixed-v1 package headers/libraries remain in that prefix.
The final complete derived SDK is inventoried afresh before builds. No generated
provider is silently substituted into an old fixed-only Worker/Gateway.

Re-run `prepare.py --mode native` with SourceB/its manifest and the NEW complete
SDK workspace. This fresh stage rebuilds both probes and exports official
metadata against SourceB. SourceA `derive` products cannot qualify behavior.
Then build all four fresh Workers and both strict Gateways using the exact
SourceB composite manifest/native_rosV headers. The Gateway custom package list
must contain both retained fixed-v1 packages and `uwvm_ros_bounded_v2`; its prefix
is exactly `/opt/uwvm-custom-types`. Actual final dependency evidence must include
all original/new provider inputs. Native runtime setup checks the pinned base setup with `source ... && exec ...`,
then explicitly sets the selected custom/base CMake, ament and ROS package paths.
It prepends only the exact custom library directory to validated nonempty base
loader entries; paths cannot come from the ambient host environment. An optional
colcon aggregate setup script is not assumed for standalone CMake installs.

`build_guests.py` prepares24 language/version/role selections with fresh full
validation and exact import signatures, source/dependency copies and whole
compiler/standard-library package pins. Its CLI has common Lane arguments and
explicit `--wasi-sdk --cargo --rustc --rust-sysroot --tinygo --go --go-root --node
--asc-js --wasm-tools` paths plus repeated `--toolchain-root`. The six complete
[client/server applications](../../../sdk/custom-types/bounded-examples/README.md)
use the frozen composite's `bounded` subtree. They do not regenerate codecs.
JavaScript uses actual relay Wasm plus the separately pinned host entry; a host
model cannot replace a Runtime execution. Startup WASI rights require explicit
reviewed choices associated with imports; none is inferred from a failure.

## Exact actual behavior inventory

Each invocation of `run_custom_bounded.py` is one serial owned actual row:

```sh
python3 -B SourceB/tests/ros_custom_types/bounded-v2/run_custom_bounded.py \
  --source /absolute/SourceB --manifest /absolute/SourceB-manifest.json \
  --sdk-workspace /absolute/derived-sdk --out /absolute/fresh/row \
  --qemu /absolute/pinned/qemu-aarch64 --binfmt-evidence /absolute/current-lease.json \
  --container-started-at EXACT_CURRENT_STARTED_AT \
  --worker /fresh/dual-worker --worker-report /fresh/workers/report.json \
  --worker-config /fresh/worker-config.json \
  --ros1-gateway /fresh/ros1-gateway --ros1-gateway-report /fresh/ros1/report.json \
  --ros2-gateway /fresh/ros2-gateway --ros2-gateway-report /fresh/ros2/report.json \
  --prepared-native /fresh/sourceB-native/report.json --wat2wasm /pinned/wat2wasm \
  --version 1 --case client --language c --products /fresh/guests/report.json
```

Repeat client and server for C/C++/Rust/TinyGo/AssemblyScript/JavaScript and both
backends:24 rows. JavaScript additionally selects `--node /pinned/node`. Then run
`--case topics`, `--case denied`, `--case rejection` on each backend, omitting
language/products:6 separate real-import WAT rows. These30 rows are additive to
all original fixed-v1 acceptance rows; they do not replace them.

| Case | Native role | Guest requirements | Independent native effect oracle |
|---|---|---|---|
| client, six languages | server | Echo1, Empty1, send1, matching feedback1 then delivery-gated terminal1, checked cleanup/exit0 | Service1, Empty1, Goal1, feedback1, result1, mismatch0 |
| server, six languages | client | Advertise two endpoints; typed Service reply1, feedback1 then independently delivery-gated result1 | Service1, Goal1, feedback1, result1 with full independent native values |
| topics | topics | Full61-byte Envelope roundtrip, trailing sentinels, unauthorized13, stale9 | Native received1/sent1, exact fields, counterpart counts1/1 |
| denied | absent | Eleven independent alias-open13/output-slot protections, no handles | Every Topic/Service/Action counter0 |
| rejection | absent | Five malformed Bool/count/string/UTF-8/trailing payloads through each Topic/Service/Action path; empty Service trailing refusal;16 actual22 refusals, protected outputs, stale9 | Every Topic/Service/Action counter0 |

Service/Action clients use independent native servers. SDK servers use independent
native clients. Native peers never connect Gateway IPC, acquire Guest handles or
include generated canonical codecs/custom adapters. They stop before ROS init;
`I` releases initialization only after exact ARM mapping. In native-server mode,
`F` follows the actual Guest Goal marker and permits one feedback publication.
After `SDK_BOUNDED_FEEDBACK_DELIVERED` confirms complete Guest decoding, native
`E` permits one terminal result and `RESULT_SENT_BOUNDED_NATIVE` permits Guest
`E` before result polling. In Guest-server mode, `SDK_BOUNDED_FEEDBACK_SENT`
precedes an independent native matching feedback callback and
`FEEDBACK_DELIVERED_BOUNDED_NATIVE`; only then does Guest `E` permit its terminal
response. Both directions require exactly one feedback and one result, without
assuming ordering between their independent DDS channels. SDK server `D` follows
actual independent native DELIVERED and precedes endpoint cleanup. `Q` follows
exact Guest transcript and actual Worker exit0, then a native quiet window of at
least2seconds and30 samples verifies no late/forbidden effect. The controller
owns an external absolute deadline for the fixture's ordinary synchronous ROS1
ServiceClient. Production custom Service RPCs use the separate bounded TCPROS
transport and one conversion+transport+encode deadline.

READY/MATCHED/FEEDBACK_SENT/RESULT_SENT/FEEDBACK_DELIVERED/DELIVERED records have
exact ordered `backend,pid,mode` fields.
PASS additionally has `services,empty,goals,feedback,result,mismatch,topic_received,
topic_sent,quiet_samples,quiet_ns`. The typed sample and transport context are
specified in the SDK examples guide. Actor stdout/stderr, full terminal return
codes, source/SDK/product preservation, architecture and cleanup remain raw
report evidence. A negative PASS qualifies refusal and zero effects; it is not
native delivery. `G01_complete` remains false in individual row receipts until
the parent qualifies the entire required original/additive inventory.

## Pure regression sources

`wire_contract.cc` is a synthetic canonical/envelope model (expected67 checks).
`typed_facade_contract.cpp` exercises actual generated C++ owners over an explicit
fake backend, including mutable handlers, replacement lifetime, nested17,
exception reset, all cleanup, empty Service, owned Goal bounds and optional
cancel-any denial. `sdk/custom-types/tests/bounded_profile_models.py` has17 named
stdlib-only generation/adversarial cases. Root compiles/runs these later in its
serial temporary model lane. Their identity placeholders are synthetic syntax
fixtures and never qualify native registration. They do not launch native ROS or
replace any real Wasm/native acceptance row.

`feedback_protocol_models.py` adds nine pure causal-gate/adversarial cases. It
loads only selected function ASTs from the actual controller, uses synthetic
actors/logs/time, and checks single feedback admission, refusal of terminal-before-
feedback, duplicate/missing/reordered observations, the unchanged200-pass bound
and the exact Gateway-after-Worker0 rule. Root runs it with
`python3 -B tests/ros_custom_types/bounded-v2/feedback_protocol_models.py` after
freeze. It creates no process and is not an actual DDS communication result.
