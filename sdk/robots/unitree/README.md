# Unitree complete-message SDK profiles

This directory prepares bounded canonical codecs and portable command checksum
helpers for explicitly selected Unitree dependencies. It does not install ROS,
contact a robot, grant a capability, or establish native interoperability.
Actual build, native identity/schema qualification, CRC oracle comparison,
Runtime delivery and simulation results require their own current receipts.
Source availability and preparation are never a `PASS` for those behaviors.

The installed SDK exports `roswasm::unitree` for these portable headers, linked
to `roswasm::c` without native ROS discovery or registration. C++ applications
link both `roswasm::cpp` and `roswasm::unitree`. Its relocated include directory
is `share/roswasm/sdk/robots/unitree/include`; explicit generated family codecs
remain a separate input. Installing/linking this target grants no authority.

| Profile | Backend | Complete definitions | Intended carrier |
| --- | --- | ---: | --- |
| `unitree_go` | ROS2 | 24 | GO low-level20-slot messages plus bounded auxiliary messages |
| `unitree_hg` | ROS2 | 11 | HG low-level35-slot messages, hands/BMS/board/pressure |
| `unitree_api` | ROS2 | 8 | Request/Response **topics**, including nested header/identity/policy |
| `unitree_legged_msgs` | ROS1 | 11 | External legacy complete definitions, preserving `Kp`, `Kd`, `SN`, camelCase |

All types in each locked family are retained. Native fields are never renamed,
projected away, or given a fabricated MD5/RIHS. Profiles are split to preserve the
existing128-type,4096-node/record,16384-expanded-node/catalog and32768-byte limits.
The generator remains responsible for enforcing those limits. The new guards
refuse keyword/method/exported-name/type-symbol collisions before emission.
Existing fixed-v1 spelling rules and canonical identity recipes remain intact.
The official HG `BmsState.fn` member is preserved exactly in canonical/native
schemas and C/C++/JS/AS (Go exports `Fn`). Only bounded Rust source uses the
standard raw identifier `r#fn` consistently in declarations, defaults and codec
reads/writes. This is a language spelling, not a renamed native field or codec
identity. Fixed-v1 still refuses `fn`; genuine other-language keywords and
exported-name collisions such as ROS1 `fn`/`Fn` remain errors.

The profile bounds deliberately select finite subsets of otherwise unbounded
native messages. For example, video arrays are at most1024 bytes each, audio2048,
height-map data2048 cells, Request/Response binary2048 and text4096 UTF-8 bytes.
These are not full video-frame or unbounded map transports. Oversized values
must be rejected, not truncated or fragmented without an independently defined
protocol. Hand sequence capacities do not identify a specific physical hand.

## External sources and native preparation

`upstream-lock.json` records official repository commits, paths and SHA256/size
facts. It contains references, not third-party definition bodies or a license
grant. The legacy `unitree_ros_to_real` repository has no root LICENSE in the
selected tree and `unitree_legged_msgs/package.xml` says `TODO` for its license.
Its definitions are intentionally external; the dependency owner must resolve
their permitted use. A neighboring repository's BSD license does not settle it.
Other selected repositories have their own BSD/Apache and third-party notices;
retain those when acquiring dependencies.

The Root acquisition/build controller supplies a pinned checkout or verified
snapshot. Run preparation only in a fresh owned directory, one family at a time:

```sh
python3 -B sdk/robots/unitree/materialize_catalog.py \
  --upstream-root /explicit/verified/unitree_ros2 \
  --profile sdk/robots/unitree/profiles/unitree_go.json \
  --lock sdk/robots/unitree/upstream-lock.json \
  --out /fresh/owned/go-preparation
python3 -B sdk/tools/custom_types.py \
  /fresh/owned/go-preparation/catalog.preparation.json \
  --guest-only --out /fresh/owned/go-guest-codecs
```

The first command verifies/copies the complete selected `.msg` files and writes
`catalog.preparation.json` plus `report.json: PREPARED_NOT_EXECUTED`. It performs
no download, subprocess, generator invocation or native hashing. The second
command is the existing trusted six-language source generator; it does not run
the generated code. The analogous ROS1 command selects the externally supplied
`unitree_ros_to_real` root and `unitree_legged_msgs.json`.

For the separate complete-vendor domain, explicitly pass
`--catalog-format uwvm-ros-vendor-complete-bounded-v1` to the materializer, then
use [`industrial/prepare.py`](../industrial/prepare.py) with `--mode prepare`
and the exact current parent/bounded/adapter Source SHA256 inputs described in
[`COMPLETE_VENDOR_SDK.md`](../../custom-types/COMPLETE_VENDOR_SDK.md). This
preserves the same whole54 definitions across the four individually selected
families and the same finite bounds. It emits the complete-domain constants,
Native exporter and six-language records. It does not convert old Native
metadata into complete-domain evidence: new binding requires the full original
definition/constant/default proof and the actual selected SDK. The existing
bounded-v2 command above remains an explicit, separate compatibility route.

For native registries, install/build the actual official message packages and
export their actual generated identities and complete schemas. The integration
entry is [`sdk/integrations/unitree`](../../integrations/unitree/README.md), owned
separately from this directory. Its exporter must bind the exact complete type
inventory to the pinned definitions, compiler, official headers, type support,
libraries, ELF and actual execution provenance. Hash absence on an older ROS2
distribution requires full actual introspection/schema attestation, rather than
inventing a catalog RIHS or declaring a package compatible by name.

The existing binding step consumes the official exporter format:

```sh
python3 -B tests/ros_custom_types/bounded-v2/bind_catalog.py \
  --template /fresh/owned/go-preparation/catalog.preparation.json \
  --native-metadata /qualified/current/go-native-metadata.json \
  --output /another/fresh/owned/go-bound/catalog.native.json
python3 -B sdk/tools/custom_types.py \
  /another/fresh/owned/go-bound/catalog.native.json \
  --out /another/fresh/owned/go-native-codecs
```

The export format is `uwvm-ros-official-bounded-native-v2` with exact backend and
type inventory, each message's native identity and `components.message` metadata.
Binding/source generation alone still establishes no native registration. Build
the same immutable common manifest into the selected Worker and matching native
provider into the Gateway, then qualify the real native schema/identity before
entity creation. Each runtime operation additionally needs its own explicit
alias, complete codec identity, quotas, QoS and authenticated Worker/session.

`recipes/six-language-codecs.json` lists all six generated surfaces: C, C++,
Rust, JavaScript, TinyGo and AssemblyScript. Use the normal SDK for each language;
there is no extra robot-native import or fake `Instance`/memory. Generated
JS/AS SDK imports are relative: stage the generated family beside the existing
SDK layout or rebase only those import paths in a pinned owned build-work copy.
Do not overwrite historical generated artifacts. The new source tests do not
modify historical receipts or infer current results from older SDK runs.

## Actual Guest examples

The new complete-domain families are provided directly in
[`unitree-go-complete`](../../custom-types/unitree-go-complete/README.md),
[`unitree-hg-complete`](../../custom-types/unitree-hg-complete/README.md),
[`unitree-api-complete`](../../custom-types/unitree-api-complete/README.md) and
[`unitree-ros1-complete`](../../custom-types/unitree-ros1-complete/README.md).
Select one coherent directory; its Native identities are still unbound.
`examples/complete_telemetry.c` progresses from raw imports to one complete
copied value. `examples/complete_telemetry.cpp` uses the genuine typed
Subscription/Codec facade. Set `UWVM_UNITREE_COMPLETE_FAMILY` to1/2/3/4 for
GO/HG/API/legacyROS1. Both default to verifying denial13. Only an explicitly
compiled `UWVM_UNITREE_COMPLETE_RECEIVE=1` application with independently granted
authority polls once. An idle11 is not successful delivery. The API variant
receives Response topics; it neither submits a request nor invents a service.

Linux validation of this revision includes25 Source/parser contracts,
six optimized/UBSan local RPC-lifetime programs, and eight actual C11/C++20
complete-codec programs covering54 original messages at three input sizes
per language (324 scenarios). The telemetry examples have16 actual object
compilations across four families, two languages and denial/receive modes.
The same324 scenarios additionally passed under QEMU on each of x86_64, i686,
aarch64 and armhf. Eight new Wasm codec programs passed324 scenarios on the
retained immutable R10 ROS Runtime;16 real Wasm telemetry programs verified
default rejection (eight denial-mode exits0, eight ungranted receive-mode
exits13). This does not qualify latest Runtime Source, native ROS delivery,
official suites, a simulator/controller, or hardware performance. Original
compiler and execution failures remain retained separately.
The independent codec oracle in `tests/full_family_codec_contract.py` reads
the retained original `.msg` grammar without importing the production parser
or emitter, populates every field and constructs exact canonical bytes with
Python `struct`. It checks output preservation on truncation, trailing bytes,
insufficient capacity and malformed dynamic sizes/booleans where present.

The explicit native-ROS2 oracle additionally passed129 complete-value CDR
round trips for all43 GO/HG/API messages against the retained genuine Humble
amd64 SDK and FastRTPS type-support libraries. All three programs use the
current complete C/C++ records. Their independent test bridge copies every
original field into actual ROS2 values, checks native serialization equality
and compares the restored canonical bytes. Actual stopped direct-child ELF,
kernel `map_files` objects, full compiler dependencies and original SDK/package
closure are retained in the [r25 packet](../../../documents/ros/continuation-20261018-evidence/unitree-native-cdr-values-r25/integration.json).
This is message-value/serialization evidence; production Native converters,
complete-domain identity binding and network delivery remain separate work.
See [the native witness instructions](tests/README.md#compare-complete-values-with-genuine-ros2-cdr).

`examples/passive_command.c` and `.cpp` are real main entries over the C/C++
ROS2 SDK. Compile with `UWVM_UNITREE_FAMILY=1` for GO or2 for HG and the selected
generated header directory. Their default build proves both a13 refusal and no
live capability. Only an explicitly compiled `UWVM_UNITREE_SEND=1` version opens
`unitree-lowcmd`, publishes one zero-mode command with checked CRC, and checks
close. The controller must already have an independent ready SIM participant.
Publication acknowledgement is not independent native delivery evidence.

`examples/direct_go2.cpp` is a direct Runtime controller: complete generated
LowState subscription → shared Go2 fixture kernel → checked CRC → complete typed
LowCmd publication. It does not use an SDK2 relay. The default build verifies13
for both aliases with no poll/publication. Select `UWVM_UNITREE_DIRECT_ENABLED=1`
only in the controlled simulation lane. The SDK ships the authoritative
`include/roswasm_unitree/simulation_control.h`; the native comparison's test
header forwards to that same source. No repository test header is required by
an installed/copied SDK tree.
Required include roots are the generated GO directory,
`sdk/include`, `sdk/robots/unitree/include` and the project's existing fast_io
include closure; build from the preserved source layout. Compile C++20 with
matched `-O3 -fno-fast-math -ffp-contract=off`, and explicit standard-WASI basic
stdin/stdout rights. Root owns compiler/sysroot/link recipes, memory, RSS and
deadlines; these instructions are not a claim that a command ran.

`examples/direct_go2.c` implements the same complete control protocol with the
ordinary C11 capability API and full generated C codec. Link its small
`direct_go2_output.cpp` formatter into that same Wasm; it uses fast_io and owns
no ROS handle. `examples/javascript/direct_go2.mjs` is the corresponding real
embedded-JavaScript main. Its optional `javascript/control_kernel.c` module
compiles the exact shared C controller and official CRC in the same Wasm and
returns copied bytes to JavaScript. It supplies no native ROS bridge or rights.
The complete generated GO JavaScript module is embedded as source, alongside
the real SDK and pinned QuickJS engine.

The maintained six-product compiler and controlled-physics instructions are
in [`tests/ros_unitree/DIRECT_PHYSICS.md`](../../../tests/ros_unitree/DIRECT_PHYSICS.md).
That recipe compiles each C/C++ application with its real renamed main and the
separate `direct_main_entry.cpp` constructor/exit wrapper; do not rename the
wrapper's own `_start`. Every standalone entry first writes
`READY_UNITREE_MAIN` and waits for `G` before language initialization. This
trusted test barrier permits actual Runtime mapping checks and PID-bound
Gateway setup. It grants no capability. The ordinary `UCK1` protocol below
begins only after that gate. These source recipes and binary receipts do not
qualify DDS delivery, physics or performance until the real controlled run.

Positive direct stdin begins with12 bytes: ASCII `UCK1`, little-endian count1..32,
little-endian timestep100000..20000000 nanoseconds. The Guest creates both actual
endpoints and writes `READY_UNITREE_DIRECT_V1\n`. Each `R` permits exactly one
bounded poll (at most4096 total); idle11 yields `UNITREE_AGAIN\n` and no command.
Real state tickN must arrive before commandN. After an actual publication:

```text
UNITREE_STEP <N> <tick> <command-crc> <state-crc>
UNITREE_STATE <N> <tick> <q-bits> <dq-bits> <tau-est-bits> ... (12 slots)
```

The36 bit-pattern values are decimal unsigned32 representations of the actual
binary32 observations, avoiding float-text rounding. After exactly count steps,
`Q` permits checked owner cleanup followed by `PASS_UNITREE_DIRECT steps=N\n`.
An early/unknown gate, repeated/wrong tick, nonfinite scalar, budget exhaustion,
CRC or publication error, or cleanup failure prevents that final marker.
Native observation, state CRC verification and real physics progression are
independent obligations in `tests/ros_unitree`, not inferred from these markers.

### Explicit model-map sampled hold

`examples/direct_hold.cpp` supplies an additional complete typed Guest path for
GO20 or HG35 carriers, including G1/H1-family models only when their exact
actuator map/family is independently admitted. It samples the current delivered
q again on every step; it does not hold a persistent initial target and makes no
walking, balance, gravity-compensation, firmware or physical safety claim.
The shared `include/roswasm_unitree/simulation_hold.h` is used by both native and
Wasm builds. Select `UWVM_UNITREE_HOLD_FAMILY=1` (GO) or2 (HG) and explicitly
`UWVM_UNITREE_HOLD_ENABLED=0|1`. The default0 opens both aliases only to require13
and no live capability, then writes `PASS_UNITREE_HOLD_DEFAULT_DENIED`; it reads
no positive configuration and performs no poll/publication.

Positive stdin begins with exact59-byte `UHL1`:

| Offset | Encoding | Meaning |
| ---: | --- | --- |
| 0 | 4 bytes | ASCII UHL1 |
| 4 | LE32 | steps1..32 |
| 8 | LE32 | dt_ns100000..20000000 |
| 12 | u8 | carrier20 or35, matching the compiled family |
| 13 | u8 | active actuator count1..carrier |
| 14 | u8 | explicit mode_pr |
| 15 | u8 | explicit mode_machine |
| 16 | IEEE binary32 LE | finite kp>0,<=1000 |
| 20 | IEEE binary32 LE | finite kd>0,<=100 |
| 24 | 35 bytes | ordered actual actuator→carrier slots; unique,<carrier; unused255 |

GO requires both mode bytes0 because those fields are absent from its command;
no input is silently discarded. HG also requires actual incoming state
mode_machine to equal the explicitly selected value. Each active slot gets
mode1, the current state's exact q, dq/tau0 and selected gains. Inactive GO
slots use mode1/q2.146e9/dq16000/gains0/tau0; inactive HG slots are allzero/mode0.
Every reserved command field remains0 before the checked full-family CRC.
These numerical fixture bounds are not manufacturer hardware limits.

The same R/Q/4096-poll schedule applies. Ready/completion markers are
`READY_UNITREE_HOLD_V1` and `PASS_UNITREE_HOLD steps=N`. STEP grammar is unchanged;
STATE emits **all** carrier triples, GO60 or HG105 decimal uint32 bit patterns.
Malformed setup, duplicate/out-of-range slot, nonfinite state/gain, wrong tick or
HG operating mode fails before publishing that command. The independent native
observer checks actual state CRC, complete values and command receipt/physics.

### Compiled native and real preload comparisons

The separate `tests/ros_unitree/native_ros2_controller.cpp` is actual rclcpp with
official generated `unitree_go` or `unitree_hg` types. Its CMake source is
`native_ros2_CMakeLists.txt`; Root stages that exact file as CMakeLists.txt in a
fresh owned source directory and sets `UWVM_SOURCE` to the frozen complete tree.
Explicit `UWVM_UNITREE_NATIVE_FAMILY=unitree_go|unitree_hg` and
`UWVM_UNITREE_NATIVE_MODE=go2-stand|sampled-hold` produce numeric definitions
`UWVM_UNITREE_NATIVE_HG=0|1` and `UWVM_UNITREE_NATIVE_HOLD=0|1` in actual argv.
HG requires sampled-hold. C++17/20 is selected by an actual compile+link probe;
C++20 uses fast_io, while C++17 uses the existing checked native_io shim.
It consumes the identical binary setup/gates and authoritative shared math,
with native stand markers `READY_UNITREE_NATIVE_V1`/`PASS_UNITREE_NATIVE steps=N`
and hold markers `READY_UNITREE_HOLD_NATIVE_V1`/`PASS_UNITREE_HOLD_NATIVE steps=N`.
Native OS ROS access is not a Guest capability/default-denial proof. Root must
retain real full-MD dependencies, link trace, ELF/maps and matching independent
measurements rather than label Python or a relayed mathematical result native.

The stand Guest also has an explicit `UWVM_UNITREE_MODULAR_MOTION=1` build,
using the independent `callbacks` then `motion` modules from
[`sdk/examples/robotics_modular`](../../examples/robotics_modular/unitree-preload-recipes.json).
Inline positive uses exact definitions DIRECT_ENABLED1/MODULAR_MOTION0;
modular positive uses1/1; the common denied build uses0/0. The scalar ABI carries
generation/slot/field uint32 values, never pointers, ROS owners or capabilities.
The motion module runs the same SDK kernel and makes twelve actual ordered
cross-module callback calls. Root main checks status after begin and each of
20×6 returned words and always ends a nonzero generation. Only after every
library call unwinds and checked retirement succeeds may it CRC/publish. Both
providers must have zero ROS/WASI rights and separately pinned real artifacts.
No modular variant is implied for the sampled-hold example.

## Checksum and Topic-RPC helpers

`include/roswasm_unitree/crc.h` is C11/C++ compatible and allocates nothing.
`crc.hpp` adapts complete generated/native ROS2 field values without reading
object padding. GO and HG layouts are different; they are not legacy ROS1
layouts. Every named reserved command field must be zero. On22/90 all output
storage, length and CRC remain unchanged. [`PROTOCOL.md`](PROTOCOL.md) gives
exact offsets, algorithms, the upstream discrepancy and its bounded contract.

`topic_rpc.h/.hpp` supplies a16-slot local correlation ledger with a lifetime
ID watermark. Register a positive signed64 request/API identity before
exactly-one topic publication. Request IDs must strictly increase, including
after completion, expiry or `forget`; reused/out-of-order IDs return17. This
rule prevents a late response from consuming a newer request in the same local
tracker. Retiring a slot does not exhaust the16-slot concurrency limit.
`begin_next` allocates/registers the next ID and returns75 at signed64 exhaustion;
it changes its output only on success and never wraps. No allocator, clock or
random import is needed. Reinitialization is not an ID reset protocol: distinct
trackers/Guest instances need disjoint operator-assigned ID ranges or an old
response channel which has actually been retired and drained.

A publication error may leave delivery **unknown**. Retain its request until
explicit local retirement; `forget` does not prove non-delivery, cancel a robot
operation, or make that ID reusable. Decode/validate an entire Response before
`accept`; wrong API/ID and duplicate replies return9 without consuming another
request. Caller-provided monotonic ticks drive finite expiry;
no automatic clock, thread, retransmit, remote cancel, ROS Service or Action is
created. Close those separate topic owners with the existing checked SDK APIs.

The final independent oracle requires three distinct payloads per original
message. This strengthens the earlier corpus, whose fixed-only fields repeated
the same values across modes. The new corpus passed all eight Native programs,
all32 x86/Arm user-QEMU programs and eight actual Wasm programs. Its Source and
original results are retained separately as r23; these are local canonical
contracts, not official upstream suite counts. See [test recipes](tests/README.md).

## Validation entry points and scope

For complete GO/HG/API Message binding on genuine older ROS2 SDKs, use the
explicit [installed-source proof workflow](../industrial/pre_rihs/README.md).
It combines original installed IDL, actual native constants and constructor
defaults, complete introspection and derived RIHS with the production binding.
The generated proof wrapper must be selected explicitly. The same bound
converter without that wrapper remains refused. Modern SDKs retain their
existing hash contract; services, actions, ROS traffic and robot qualification
remain separate. The [test guide](tests/README.md) distinguishes independent
field-copy CDR witnesses from complete production converter witnesses.

Root may run the pure Python tests at `tests/source_contracts.py` and compile/run
`tests/crc_contract.c`, `tests/topic_rpc_contract.c` and `tests/hold_contract.c` as
native and Wasm models. Hold source checks explicit non-contiguous maps,
GO/HG inactive behavior, configuration boundaries and unchanged outputs on
failure; they do not establish native/physics behavior.
`recipes/compile-models.json` gives explicit source/model/direct-Guest command
templates; every physical tool, sysroot, generated input and output placeholder
requires Root's actual pinned selection. It never installs tools or adds grants.
These mathematical/parser tests are not official native or robot acceptance.
The CRC source checks include asymmetrical words, explicit offsets/padding,
all reserved refusal paths, finite-float checks and unchanged-output cases.
Independent actual official C++ **and** SDK2 Python CRC oracles must agree for
the supported zero-reserved command space before CRC interoperability is claimed.

The official SDK2 binary tree currently supplies x86_64/AArch64 libraries; its
Python CRC Linux loader also selects those two architectures. Other Runtime
targets can use a separately qualified ROS Gateway/native peer; availability of
a Wasm codec does not manufacture a target SDK2 library. MuJoCo and IsaacLab are
separate optional native dependencies with model, physics, GPU/display and
license requirements. No build, test or hardware run was performed by this
directory's source author. The current Root lane's actual reports are decisive.
