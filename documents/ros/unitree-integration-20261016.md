# Unitree integration and current acceptance boundary

The latest restored-boot observations and current acceptance boundaries are in
[the third-admission checkpoint](validation-checkpoint-20261016-boot3.md).
[The earlier restart checkpoint](validation-checkpoint-20261016-restart.md)
retains its original boot and memory-aware parallel-build observations.
Earlier sections retain their original source/boot evidence scope.

This implementation extends the existing ROS1/ROS2 Runtime and six-language SDK
with complete, explicitly bounded Unitree message families, portable command
CRC helpers, actual typed Guest examples, native identity preparation, and a
separate simulation/performance lane. Source implementation, dependency input
acquisition, actual compilation, native transport, simulation and performance
are separate evidence scopes. The observed checks below cover source contracts,
SDK compilation and fixture preparation. Native ROS interoperability, actual
robot simulation and performance acceptance remain separate pending results.

## Implementation entry points

* `sdk/robots/unitree/` contains exact upstream source locks, four complete
  family profiles, external definition materialization, C/C++ CRC and Topic RPC
  helpers, native-compatible ROS1 field-name support, actual SDK main programs,
  and detailed English contracts. Generic generation supplies C, C++, Rust,
  JavaScript, AssemblyScript and Go/TinyGo canonical values.
* `sdk/integrations/unitree/` selects actual installed native message packages,
  builds an identity exporter, and provides source preparation for binding its
  output. ROS1 reads actual generated traits. ROS2 attests full actual C++
  introspection and uses actual baked hashes when present; older SDKs explicitly
  label independently recomputed RIHS01 from that actual introspection.
* `sdk/examples/robotics_modular/` supplies independent main, motion, vision and
  callback Wasm modules, explicit named-preload recipes, a negative ROS callback,
  shared math and language alternatives. Values crossing instances are scalars,
  never pointers into another instance's memory. Application generation tokens
  manage lifetime; Runtime identity separately controls authority.
* `tests/ros_unitree/` supplies real MuJoCo stepping, actual SDK2/ROS peers,
  direct Guest and matched native-controller protocols, independent witnesses,
  scoped benchmarks and conditional Intel VTune collection. It preserves each
  independent process and uses one owned execution lane.
* `tools/ros_distro/acquire_sdk.py` extracts authenticated complete `arm64` or
  `amd64` package closures into owned sysroots without host installation or
  package scripts. Its distribution list is input selection, not compatibility.

The SDK CMake package exports `roswasm::unitree` with a relocatable portable
header include directory and no native discovery, registry or grant. C++ users
also link `roswasm::cpp`. Both relocation checks verify this new installed
surface. Generated family/native providers remain explicit reviewed inputs.

## Authority and process scope

Default Guest ROS and optional WASIp1 rights remain empty. Only documented basic
standard streams are available until the administrator explicitly selects more
rights. A preloaded library receives no ROS authority from its caller. The
existing cross-module ROS ancestry refusal remains in force, including a call
through main, motion and callback. Root main may perform its own authorized ROS
operation after the library call unwinds.

SDK2 DDS `rt/lowcmd` corresponds to ROS2 `/lowcmd`; adapters must not add another
`rt/`. GO20 and HG35 commands have distinct full schemas and CRC images. The
Unitree API Request/Response pair is a topic protocol, not a fabricated Service
or Action. Reserved command fields are zero in the supported CRC profile. The
pinned upstream HG reserved-word discrepancy is retained in `PROTOCOL.md`.

Profiles preserve complete declared fields while imposing finite capacities.
An over-limit video/map/string value is refused, never silently truncated. Four
families need not fit in one registry: existing128-interface,4096-node/record,
16384-total-node and32768-byte limits remain enforced. Select complete families
or use separate Worker deployments; do not raise quotas or omit native fields
merely to make an aggregate catalog pass.

The simulator, Gateway, Worker and native witnesses remain separate processes.
The test supervisor is acceptance machinery. The execution Runtime does not
acquire artifact signing, WasmGit update tracking, multi-program deployment
configuration or WasmHub responsibilities. Optional vLLM deployment is not
required by the baseline CPU robotics simulation and is not started implicitly.

## Restored Linux input preparation

The current Linux boot is `af121220-8c6e-4302-9853-f08bf4c50dbb`. The task's
existing container was assigned64GiB memory and equal total memory-plus-swap
before restart; the actual admitted cgroup has `memory.max=68719476736` and
`memory.swap.max=0`. Its current leaf inode is18151, device29, CPUs0-31. Old-boot
cgroup identities and old physical mount pins are historical evidence.

All remote task commands enter through the current-boot64GiB guard. Tools,
installers, compilers, QEMU, simulators, profilers and their reporting children
must inherit that same leaf. Actual compilation is serial with `-j1`; bounded
owned RSS, time, output, disk reserves, pidfd ownership and cleanup remain
required. Matching a memory number in a configuration is insufficient proof.

The original sparse Btrfs artifact volume was remounted with its original UUID
`3fe7718a-8039-4f1c-b540-7c5cf05b65ca`; no format or filesystem repair was run.
Its current mount device is81 and loop device is `/dev/loop29`. Output and
ancestor filesystems are checked to protect the sparse image's backing space.

Six official source snapshots were acquired inside that leaf. Every Git tree
and tracked blob was verified against the exact official Git object identities:

| Official repository | Fixed commit |
| --- | --- |
| `unitreerobotics/unitree_ros2` | `668d1ec5a05d1c38d3306bdca7d59f2ba3581a88` |
| `unitreerobotics/unitree_sdk2` | `d1abf001220d3e5f59e00a0c4aaa595e00089a4a` |
| `unitreerobotics/unitree_sdk2_python` | `814556d15970dd2ecf1c9984e845ca02ab07e206` |
| `unitreerobotics/unitree_mujoco` | `1eb6642e3f3fdfb7fb13a9794fd6a2dd93ea0e7d` |
| `unitreerobotics/unitree_ros` | `5994d4faef0a9cadd3287f8de0199a67eeb2a259` |
| `unitreerobotics/unitree_ros_to_real` | `b989870124913091fbe75e0dcfb047eb4ca00a09` |

Gitlinks are recorded separately and never claimed materialized by an archive.
The legacy message package remains an external dependency with its unresolved
upstream license declaration; this integration does not vendor its definitions.
Official Python dependency metadata is retained separately. SDK2 Python's
`cyclonedds==0.10.2` requirement must not silently become a newer major release.
A native Humble/Jammy amd64 authenticated520-package closure has been extracted,
with a separately authenticated DDS-IDL generator add-on. Its package acquisition
runs no compiler, ROS program or simulator. Native builds consume an independent
SDK copy in a private mount namespace. A missing POSIX shell entry in the
extracted SDK is derived explicitly from its pinned Bash in that copy; original
package inputs and earlier failed attempts remain preserved.

## Observed checks and their limits

SourceB30 is identified by complete manifest SHA256
`aa84a4830941e28a56fd3a9de75b368d23d447171355397a0e53375eca395af6`.
The following observations were made in the current original64GiB cgroup.

| Observed scope | Actual result | Boundary |
| --- | --- | --- |
| Source and Linux process-cleanup contracts |99 checks passed | Includes actual high-numbered pidfd readiness/cleanup; not ROS behavior. |
| Complete official Unitree family materialization and six-language generation |54 definitions;12 serial commands passed | Source products and native export selections; native identities require their own actual exporter. |
| Complete SDK compiler/model regression |349 fresh Wasm compiler products and794 successful host command/model rows | Every Wasm product passed import inspection and full validation; no native ROS, Worker or physics execution. |
| Compiled SDK local scheduling models |12 cases passed using10 of the349 actual compiler products | Local scheduling only; TinyGo denied entropy and explicitly synthetic entropy cases remain separate. No Worker or native ROS execution. |
| Pure Unitree motion and callback providers |2 separately compiled Wasm libraries passed full dependencies, link-input checks and validation | No Wasm execution or preload/ROS authority qualification in this result. |
| Separate OSMesa dependency inputs |44 authenticated packages;303 serial commands passed | Safe extraction and actual dependency constraints only. No renderer, camera pixels, MuJoCo steps or robot behavior qualified. |

The SDK regression's final report SHA256 is
`3052c3e36a57bc94d5c75860512993173b423d5d4af10b94afff6670de6493d0`;
its full inner report is
`df32474b3855c04bf7d4646c8754005c7c3552fdff5f1d465e6e942613b359bd`.
All source/tool inputs and installed SDK inputs remained preserved. The owned
process-tree peak RSS was414642176bytes, cleanup completed without remaining
actors, and all ancestor OOM counters remained zero. The cgroup's larger
`memory.current` value also accounts for retained filesystem cache; it is not
this process-tree RSS measurement.

The compiled local scheduling report SHA256 is
`ca80ed73783cf7b63bd22ec47120261bb334a4feea120dccfd1229521dd0ee4b`.
It used the retained compiler outputs without compiling replacement fixtures,
preserved the complete349-product input catalog and source/tool inputs, and
finished with successful cleanup. Its owned process-tree peak RSS was
104005632bytes. These observations do not grant an entropy capability to a
Guest or substitute for actual Runtime/native transport tests.

The maintained two-provider preparation report is
`unitree-modular-pure-providers-20261016-r2/report.json`, SHA256
`858393c0f3de6d93e5b3ac6c3f53d914aa015fc5bba6c4ccdafe3c2b0d0b7d1a`.
The current artifact root is the restored
`/var/tmp/uwvm-ros-complete-20261015-r1/acceptance-storage-r1` volume. Earlier
failed preparation reports are retained rather than overwritten by these runs.

Existing JavaScript compiler coverage uses the explicit C bridge/relay with
external Node application logic. It does not establish isolation of JavaScript
user code inside Wasm. An embedded-engine Guest path is being implemented and
must receive independent actual compilation and execution evidence. Its
official QuickJS2026-06-04 source archive was acquired over validated HTTPS and
pinned as621500bytes, SHA256
`b376e839b322978313d929fd20663b11ba58b75df5a46c126dd19ea2fa70ad2a`.
That source acquisition executed no downloaded code and claims no upstream
signed-release digest or successful engine build.

The embedded JavaScript implementation now has30 explicit build variants,
including command programs, independent scalar Wasm providers, floating-point
boundaries and finite resource-limit fixtures. Its source peer is complete;
actual engine builds and Runtime/native execution remain pending. The separate
OSMesa input report is SHA256
`251ec9868b33bba922a6df930933c53814de1130257d941d3e1fd483fd5a4f0a`.
It preserved the original SDK and the extracted add-on, completed cleanup with
no remaining actors, and used105984000bytes peak owned process-tree RSS.

## Subsequent SourceB31 observations

The complete SourceB31 manifest is
`57908a62dec210ed286d47116cc0c9b1e07ed66f422fc0734cddde90c68c3e4b`.
Its SDK regression rebuilt349 Wasm products, validated their complete binaries
and imports, and passed820 host command/model rows. The supervisor report is
SHA256 `7407a7cc6b42ffab542c52f251228751ff3cccb0f29e2e2621d29cc476955fc0`;
peak owned process-tree RSS was411521024bytes, with preserved source/tools and
successful cleanup. This includes the embedded JavaScript source models, while
the compiler coverage still uses the existing C bridge/Node application path.
The separate two-provider SourceB31 preparation also passed, with inner report
SHA256 `b0d26a01ea4b5bc398d3ec9fb16ba9b4a64969005e8341bc58901eb0998eadff`.

The first actual embedded QuickJS build compiled its13 C objects but failed
at its first Wasm link because ELF archive-group flags are unsupported there.
The original failed report remains SHA256
`db8a385c24496bddc8d08a9f8a020593c2111f1557e6cbc673bdf8c023104688`.
All input catalogs remained unchanged, cleanup passed, and peak owned RSS was
330584064bytes. The corrected recipe passes archives directly and selects the
explicit Wasm linker alias through the target driver's `-fuse-ld` option.
This source correction requires a fresh build and does not revise that failure.

An actual stopped-exporter diagnostic established that the trusted chroot
keeps a waiting parent, while the exporter is an owned descendant. It also
established that raw Btrfs mapping device numbers differ from opened file
`st_dev` values. The diagnostic opened all73 file-backed mappings and matched
14 distinct full physical file identities/hashes through `map_files`; it sent
neither a resume signal nor the exporter input gate. Native preparation must
use those actual descriptor facts, retaining raw map fields separately. This
diagnostic alone qualifies no native type identity, ROS behavior or simulation.

The subsequent actual three-family native preparation completed all40 commands
and passed for `unitree_go`, `unitree_hg` and `unitree_api`, including original
package compile evidence, actual mapped exporters, native type identities,
bound catalogs and six-language source generation. Its inner report is SHA256
`c52bc5ed98e52904f52e1f8dac7031cd65a53f94d5128375d8de2eea23ae4d73`;
the supervisor report is SHA256
`09ef9e540eb6fb959a6614283d9852cc01928e2aa06c87865b7c2788a48cd350`.
Peak owned process-tree RSS was395415552bytes; source, original inputs,
execution SDK and derived products remained preserved. All owned actors and
private bind mounts were cleaned up. ROS endpoints and physics remain separate
pending execution requirements.

The official Rust1.90.0 `wasm32-unknown-unknown` standard library was obtained
and extracted into a new independent SDK copy, preserving the existing SDK.
Its input-only report is SHA256
`8f6d29f5c836bbd4dce244d07f4b347a2e51bb178456ec546e779dbed0e29c0c`.
No Rust compiler or target program ran during this acquisition.

## Subsequent SourceB32 diagnostics and native inputs

SourceB32 has complete manifest SHA256
`5cad2945ed8d26a14ee7235bc84c82c1116ea05ac2690ec9364ba034252af25e`.
Its first embedded JavaScript command binary linked successfully, but strict
import admission refused the additional WASI `fd_close` and `fd_seek` imports.
A separately compiled pure JavaScript provider also retained those imports and
`fd_write`, so expanding the command allowlist would not solve the boundary.
Neither failed binary was accepted or executed. The actual one-link extraction
diagnostic, report SHA256
`4022ee66c66d270891049a8fdfcf08a8815d3f4cf1f0ce476f28ff9839cd545b`,
relinked the retained objects to exactly the original binary bytes and recorded
the libc archive dependency chain. It made no compiler translation, ABI,
full-validation, Guest, ROS or performance acceptance claim.

The independent native CPython 3.10 dependency preparation verified all 19
selected wheels, their platform/Python tags, metadata, dependency closure and
file records, then extracted a separate offline prefix. Its report SHA256 is
`4f21f666e8ef2d99354aa97696fed105809bb549c1f334eeeb4d5587ab066192`.
No installed wheel package code, native CRC library, MuJoCo model or renderer
ran in that preparation. Loading those libraries and observing actual model
structure and finite physics steps require the separate native preflights.

The actual generated complete GO, HG and API registries charge 9916, 2353 and
4142 expanded nodes respectively. Their aggregate 16411 exceeds the unchanged
16384 ceiling. The proposed native interpreter qualification therefore uses
the complete GO+API provider (14058 nodes) and an independent complete HG
provider (2353 nodes). Each matching Worker/Gateway pair must use identical
common registry bytes. This deployment split neither drops family members nor
qualifies the still-pending legacy ROS1 family or retained fixture acceptance.

## Actual SourceB33 compilation and native preflights

SourceB33 has complete manifest SHA256
`0ff8c2bc605c0acd24b4b8c641c2ceb503a53966e12054c6f2bc61b77b5f7a99`.
All execution below inherited the original boot-bound 64 GiB, zero-swap
cgroup. Every recorded ancestor OOM counter remained zero. Successful cleanup
and unchanged original source, complete input roots and retained products are
prerequisites of these results.

Four fresh x86-64 interpreter Workers were built for the `none`, `ros1`,
`ros2` and `dual` profiles, with twelve independent translation units and four
links. The ROS2 profile contains the complete GO+API registry; the dual
profile contains the separate complete HG registry. The inner build report is
SHA256 `7fc7508b4a620f928b15801f15cf21843d7a1f4266ec61ea6eed57e27002939b`.
Peak owned process-tree RSS was 4190830592 bytes. This build did not execute a
Worker or qualify ROS exchange, permissions, debugging or physics.

Eleven separate native Unitree preflights completed. Real MuJoCo 3.16.0 loaded
Go2, G1, H1, H1_2, B2, B2w, Go2w, H2 and R1, checked their actual actuator,
joint, carrier-slot, sensor and IMU maps, and advanced eight zero-control steps
per model. These 72 steps had finite numeric state. A separate one-step Go2
OSMesa job produced actual 160 by 120 RGB bytes. That 57600-byte renderer
output exceeds the unchanged 32768-byte ROS wire ceiling; it qualifies native
rendering only. A future ROS Image case must explicitly select a smaller image
and retain all pixels. No truncation or larger wire quota is implied.

The remaining preflight compared the official SDK2 Python and native CRC
implementations on five word vectors and eight message cases. Actual loaded
MuJoCo, NumPy, OSMesa and CRC library identities were opened through kernel
mapping descriptors. The inner native-preflight report is SHA256
`8c281ba2eec9ea5f7e83ae4a6eb8e1721f9f184c66b227e88894eb9b73905697`;
peak owned job RSS was 855818240 bytes. All temporary mounts and actors were
removed. Zero-control physics and CRC agreement do not establish balancing,
locomotion, ROS control delivery or native/Wasm equivalence.

All thirty actual embedded QuickJS Wasm variants subsequently compiled and
passed full Wasm validation and exact binary-boundary checks. Pure libraries
have zero imports; command modules retain only the three selected basic WASI
imports alongside the exact ROS1 or ROS2 ABI where applicable. The selected
official WASI libc full long-double formatter removes the diagnostic stdio
dependency without an ABI stub, disabled assertions or a wider allowlist. The
inner report is SHA256
`0431413a6fa274abc4617d2e31aec9614dd117e78cf25268db4ca3289af270e7`;
the supervisor report is SHA256
`a6c17f64d1e9aef604141f0f5c398e01872b68a93443186b9a7b2facd6fb2d86`.
Peak supervised owned RSS was 337018880 bytes. This result executes compilers
and validators, without executing the resulting JavaScript or ROS programs.

The maintained motion and callback pure Wasm providers were also freshly
compiled from SourceB33 with full dependencies, retained link traces and full
validation. Their preparation report is SHA256
`fa5d31508de802e7cc93a3fdbc0e72afc6bd140365a219be1c716d91b85c879d`.
The original 36 modular/passive example selections and the additional 18
complete native-bound family plus six legacy guest-only codec selections have
separate producers and require their own actual reports. Legacy guest-codec
compilation never supplies a native ROS1 identity or interoperability result.

## LLVM ROS ancestry implementation awaiting fresh qualification

The subsequent implementation supplies real LLVM function-entry ancestry,
authenticated tail-frame retirement and direct Host-import frames. Tail
transitions retain an already-crossed module boundary until the actual call
returns. ROS admission requires the effective full LLVM execution path with
the instruction call-stack policy; unwind and absent tracking remain denied.
Default rights, quotas and the interpreter authority rules remain unchanged.
The actual CLI recipes use `--runtime-int` or `--runtime-aot` and `-Rct 0`.

This implementation changes actual Runtime compilation inputs. The SourceB33
Workers therefore cannot be relabeled as qualification of this newer source.
A new complete freeze, four fresh interpreter builds, the genuine patched LLVM
dependency and fresh LLVM/debugger integrations remain necessary. Source review
and an applied patch do not establish the execution semantics of this bridge.

## Unified acceptance still required

After the implementation freeze, run the complete existing acceptance plus new
Unitree cells. Required evidence includes all six language codecs and negative
bounds, actual native generation/identity/schema/CRC oracles, mapped library
qualification, default denial with independently quiet native effect, granted
actual LowState/LowCmd delivery, deterministic real MuJoCo steps, distinct GO/HG
model support, matched compiled native/Wasm observations, and cleanup/resource
checks. Source/model manifests start as `NOT_RUN` and are updated only by their
actual retained observations.

The original debugger, broad distribution, SDK, correctness/security and
multiarchitecture QEMU obligations continue. QEMU measurements describe that
emulated workload; Intel host PMU counters do not measure an emulated guest
microarchitecture. Hardware Hotspots and Microarchitecture Exploration follow
an actual measured bottleneck and official collector qualification. Preserve
uninstrumented timings and raw failed runs across optimization and retesting.

## Source34 actual build and first execution results

Source34 contains17,858 files and214,145,656 bytes, with manifest
`cef92bcb49d3fe39635e1f0f0f3532ff491b20fb0488fcd210ae05bb202af12b`.
The complete remote tree was independently copied and every opened file was
rehashed. Only the local duplicate uses a sparse22-file delta; it is explicitly
labelled metadata plus delta and is not represented as a complete local copy.
All Linux phases below used the original64GiB/swap0 cgroup, serial child jobs,
finite descendant limits and the unchanged source/input preservation checks.

Four fresh native interpreter Workers completed actual compilation and linking.
The outer report is `source34-native-four-runtime-build-r1/report.json`, SHA256
`03e1db016145b3c655168b95de9ceef0fab7eb8ee886ff25930a2ab1953c8e9d`;
its inner `workers/report.json` is
`60e1194317d31cb4448e88db059eb7c2d59dd3c34b2da62c59b8fb109279cc5e`.
The18 actual commands include12 fresh translation units, four links and two
tool version observations. Outer peak owned RSS was4,247,244,800 bytes. This is
build preparation only, not an execution result. Two additional JavaScript
native-event binaries separately completed full compilation, validation and
strict import inspection; their report is
`source34-embedded-js-additional-event2-build-r1/report.json`, SHA256
`f55071d1399b5572a6f01221fd303d285989c873b2d0324976ed93e4bb85772a`.
Their actual native delivery remains untested.

The first actual25-case JavaScript Runtime attempt failed its first pure-library
lifecycle assertion; zero cases were accepted. Preserve
`source34-js-local25-runtime-r1/report.json`, SHA256
`c0a91d7d0adaecca9187953e991fc7cf780ab480cdd72f8f1a0f5eb0fb392ca4`.
A separate diagnostic retained the exact original assertions and once-only
import calls, adding only bounded scalar return-bit records to the main's
already admitted stdout. It observed `vision.roswasm_js_init` returning2
(EXCEPTION) and then the first assertion failing. Its report is
`source34-js-pure-value-diagnostic-r1/report.json`, SHA256
`7b9ff5deba78e399d6c9357e1ef886123347b6a51b4c3da8829eb3817d03910e`.
Successful diagnostic capture does not qualify the failed program. All pure
library imports and permissions stayed empty.

The first three-Gateway native preparation also failed before target builds.
Its actual Humble configure selected C++17 and detected OpenSSL, but its
quiet-node probe could not call the reference-returning
`get_notify_guard_condition()`. The retained report is
`source34-native-gateways-peer-r1/report.json`, SHA256
`7843dd47ea459e7afcb90624b0b963ba438607a1618bb6524e5ff997af3c23ef`.
The next source revision adds an expression-selected reference notification
path while preserving direct, pointer and locked raw-guard paths. This source
fix requires fresh native configure/build evidence; the required quiet-node
feature has not been waived. Both failed attempts completed owned cleanup and
preserved their original inputs, source and outputs; no OOM occurred.

Two additional preparation results remain deliberately narrow. One complete
GO-family TinyGo diagnostic compiled and fully validated with zero imports and
an explicit initializer after selecting the installed `wasm-unknown` target
and opt1. It did not execute codec bodies, and its original binary lacks a
declared memory maximum; bounded-memory artifacts require separate builds.
Its report is `source33-tinygo-one-pure-reactor-diagnostic-r1/report.json`,
SHA256 `5df2e172f081a9e24de908d15c815bf33658b224389e3fd4e44c94cd37b8ad03`.
Separately, the exact SDK-matched official libc++abi termination source compiled
to an ordinary Wasm object with complete dependencies and local-only diagnostic
definitions. Its report is `source34-official-cpp-overlay-object-r1/report.json`,
SHA256 `aa500a1b3f01015b7b6e33e39c20508e3dba08ca6f8b84778a2734cf909e93bc`.
This has not yet established a successful C++ Guest link, removal of unexpected
imports, or a maintained user-facing recipe fix. Earlier36/18-case failures
remain failed and are not replaced by these diagnostic results.

The subsequent complete-family build qualified all18 separate GO/HG/API
artifacts: all three full message families in C, C++, Rust, JavaScript,
AssemblyScript and TinyGo. The actual report is
`unitree-source33-complete-native-codecs18-r3/report.json`, SHA256
`da6bc8f5195d3391321710e88cf0c2e68dfd379f81c4c687a9f6d78a1ccb4935`.
The TinyGo artifacts use the complete installed pure target with only a finite
64MiB memory maximum appended; their actual import and initializer tables were
inspected. A separate corrected legacy-only build then qualified all six
languages against all11 legacy message definitions, preserving ROS1's exact
`package/Type` identifiers. Its report is
`unitree-source33-complete-legacy-codecs6-r4/report.json`, SHA256
`4ea517b5420f8f62207bed12997d3ac922e149967602959cb2581a112eb0b030`.
These24 products establish compilation and full binary validation. They do not
establish initializer execution, codec round trips, DDS/TCPROS interoperability,
or a native ROS1 MD5 identity. The original legacy name-validation refusal is
retained separately; the successful native18 products were not rebuilt or
relabeled by that legacy retry. Both completed owned cleanup and preservation.

A terminal-only copied-engine diagnostic subsequently captured the actual
first JavaScript exception: `stack overflow` at
`sdk/javascript/guest/bootstrap.mjs:1:1`, during bootstrap compilation. Its
report is `source34-js-terminal-exception-diagnostic-r1/report.json`, SHA256
`6a0b75323379b0b29e20ab8d4fa68f3b59f57acd4b01c10d9d2ceed6ebc0a3ac`.
The diagnostic library kept zero imports, separate bounded memory and the
original failed initialization assertion. It consumed the pending exception
only after sticky failure and did not resume or qualify the application.
Source inspection found that subtracting the entire low-address Wasm C-stack
reservation from an already lower in-function stack pointer can wrap QuickJS's
unsigned limit. The next revision keeps that reservation unchanged, supplies
a separate nonzero half-reserve evaluation budget, checks entry headroom, and
refreshes the stack top at creation and later entries. This fix, the public
JavaScript Status reexport, and the new vendor examples still require fresh
current-source compilation and actual execution before acceptance.

Source35 then freshly compiled and fully validated all30 embedded JavaScript
artifacts with the stack-budget and public Status fixes. Its build report is
`source35-embedded-js-all30-binaries-r1/report.json`, SHA256
`c02c0ec0ef8643c8a08a06a448a9cec95f57bc2a8ba3e8431d6a40eb79bab49a`.
The next actual Runtime attempt passed the independent pure lifecycle oracle
for vision, callbacks, motion and floating modules, including their real
cross-library calls, with zero ROS/WASI library imports. The subsequent first
ROS1 raw command exited2 before its expected marker, so the whole25-case run
remains failed after one accepted case. Preserve its actual report
`source35-js-local25-runtime-r2/report.json`, SHA256
`28f4ba4571433676a5d79f7e1b4876b4bb238fd841a1a9678ff08e15d3c7c164`.
Those Workers retain their actual Source34 provenance; every compiled source
dependency was independently compared with the current Source35 counterpart.
They were not relabeled as freshly built Source35 executables. Source, full
pins, complete inputs and owned cleanup passed; no native ROS effect or
whole-suite PASS follows from the pure-library result.

The fresh Source35 native Gateway configure passed the real Humble quiet-node
probe with the reference notification adapter. Its first actual Gateway build
was stopped by the private2GiB owned-command budget at2147700736 bytes; this
was not an OOM or a completed target build. Preserve
`source35-native-gateways-peer-r1/report.json`, SHA256
`e4eaa3809a4488ef61fd876b7ee0102905645d8c93217b0780d080e190d55222`.
The original64GiB/swap0 cgroup stayed unchanged and all owned mounts/processes
were retired. A later serial retry may use a separately explicit3GiB native
compile budget; the old failed result remains separate.

The seven genuine matched-C fixture artifacts were also independently
qualified without recompilation. The actual retained-product report is
`source34-math7-retained-readonly-r1/report.json`, SHA256
`18805ff6c211b13d05946bb5d318191e07b8a12d9d11c7c4c8bfc62f0b3357f2`.
It reopens the original seven compiles, seven links and five validators,
complete dependencies, native ELF and full Wasm tables. Only benchmark roots
select their actual six compute imports; the ordinary main retains its
original seven-import contract. This qualification starts no compiler,
validator, fixture or Runtime. Mathematical execution, native/Wasm comparison
and transport are still untested by that report.

The next source revision additionally corrects TinyGo source-selection
predicates for the official pure Wasm target's `tinygo.wasm` tag, without
injecting a conflicting Go architecture tag. It also supplies an independent
actual-rclcpp GO/HG/API typed witness and its finite native protocol in
`tests/ros_unitree/native_typed_ros2_peer.cpp` and
`tests/ros_unitree/typed-native-protocol.md`. This is source implementation,
not a claim that the new native witness, fourteen typed Guest products or
twenty-eight denied/positive transport cases have run. API discovery respects
the Gateway's lazy first-publication publisher creation; local capability-open
still creates no native publisher and grants no additional right.


## Continuation: full36 binary preparation and measured runtime failures

The actual Source34 full36 producer completed all 28 modular and eight passive
Unitree Wasm selections. Inner report SHA256
`4f0f5695bcee1ff655b121963159bedfa106a8f103fb168e3d3b6f95627687c8`,
outer `bdbee83da82cf36ab6c0410ab074c9d101d1c3415d245362a84a9b9979915787`.
Both retained the original complete source/tool/SDK input inventories and
successful owned cleanup. This is compilation and closed-binary qualification;
it does not qualify Wasm initialization, native ROS delivery, simulation or
performance. The portable SDK33 official termination-object preparation
instructions and six C++ main recipe dependencies now deliver the same explicit
policy; the newly maintained preparer remains unexecuted.

The Source35 raw JavaScript failure was reproduced with the same policy and
original application assertions. The bounded terminal recorder observed
`invalid length` at native `encode`, bootstrap line10, raw example line12.
Diagnostic inner report
`d2ce4abe1dac8f40661df194746bbb7466850ed7340b619f45999bae2aad79f1`,
outer `4ba029d1dcff9db3943e6691c625596f6c4c5a527cba41b264bf0773e3151d20`,
retained actual run_main/Worker return2 and successful cleanup. The diagnostic
success is only successful exception capture; the original application remains
a failed correctness case. Inspection of the pinned QuickJS2026-06-04 source
showed that its public typed-array C entry point forwards an unpadded argument
vector to a constructor that reads argv[0..2]. The maintained embedding now
supplies three real slots for both the empty class probe and copied byte-array
construction. These source changes need a fresh build and original-case rerun.

Native Gateway Release/O3 compilation reached the real Humble SDK. The builtin
Gateway compiled with about2.23GB peak owned RSS. The complete GO24+API8 custom
Gateway exceeded separate3GiB and5GiB command limits; the latter report inner
`93a47c3b4f512991fabc184fa0c30034bcedd99c2eb3580e76d5d96ed0fecbd3`,
outer `9448d05b4d0792be2f5a5904f603f811303288f1a92b49d0d424b6520ac3beb8`.
All attempts retained original64GiB/swap0 cgroup limits and zero OOM counters;
all owned mounts were cleaned. These failures motivate splitting typed native
publisher/subscriber instantiation across separately bounded translation units.
Neither an incomplete custom Gateway nor a budget failure is ROS compatibility
qualification. Release optimization and complete generated type coverage must
remain intact in the next build.

The actual official Noetic distribution-content lock and authenticated APT spec
are prepared: report
`248edf19d750121638655a726347d4fb0aad8a9d184dfe99c9f094a042c3eb39`,
lock `abf52a699cadb95332277605506cbd4db076736afc55b57a2dfcd9879ae1d8b5`,
spec `9d7c94ca3154f2c57e05167f87a34edf2747ce95d6694d0d60fb03934f6d2f89`.
This closes the real metadata/signature-selection prerequisite only. SDK
acquisition, native ROS1 MD5 identities and actual TCPROS interoperability still
require their own completed results.


## Source37: bounded native template compilation delivery

The custom ROS2 adapter now keeps its actual rclcpp publisher/subscriber
implementation in generated shard translation units. The default32 partitions
cover the complete selected native type table, including all32 GO+API Messages
or all11 HG Messages; partitioning never projects or drops definitions. Main
and the dispatcher see declarations, while every selected native operation is
implemented by an out-of-line shard. A missing object is a link error. The
builtin-empty target remains one main TU. The shared CMake setup also serves
the native fixture and ARM cross-build acceptance entry point; both now retain
the true multiple-source/object contract. See
[the public build contract](../../gateways/ros2/custom_topics_build.md).

The explicit native producer for the next actual build requires one builtin
TU, two complete34-TU custom Gateways, and four independent peer/gate/identity/
typed-witness TUs:73 in total. CMake File API sources, compile database,
configured shard template bytes, actual verbose compiler commands, every
full-MD receipt/object and final linker inputs must agree. Every object uses
one real generated Make rule with its own finite budget, followed by the
actual CMake link. Complete header closures are retained in separately pinned
per-TU receipts to bound the main report without omitting dependencies.
Release/O3, assertions, strict floating-point flags, lazy entity admission,
QoS verification and per-session ownership remain unchanged.

These are implemented and independently reviewed source contracts. No actual
Source37 Gateway, four-profile Worker or typed-native build has completed at
this checkpoint. The actual earlier compile failures and their cleanup remain
separate evidence. Fresh Worker compilation is required because Source36's
fast_io host/policy/IPC headers changed; Source34 binaries will not be relabelled
as Source37. The fresh JavaScript build must incorporate both the previously
observed stack correction and the three-slot C typed-array argument correction.

## Source38: Noetic extraction memory and native compiler compatibility

The first actual Noetic amd64 acquisition failed while decoding package297,
`libflang-18-dev`. Its command exceeded the512MiB budget; the terminal outer
report is
`3370f5e44c68f33f24387a35f51b5fa1f3300a6cda0d2430a4658a6d5a889d7d`.
Owned cleanup and source/input preservation passed, with no cgroup OOM.
The inner acquisition report remained a partial `RUNNING` record and its
extracted directory is not a qualified SDK. Neither record is resumed or
promoted to success.

An independent actual diagnostic decoded that same authenticated package with
`DPKG_DEB_THREADS_MAX=1`, keeping the same512MiB command limit. Decoding and
the complete tar safety preflight passed at64,634,880-byte peak owned RSS,
compared with572,628,992 bytes in the failed command. Diagnostic report
`eacb6d758b7f7108364078349973de2af0b31c67b69c5a678643367161f684e8`
qualifies only this one-package operation. The maintained acquisition tool now
requests one decoder thread in its explicit environment and records the
request in each real command receipt. It does not infer an observed thread
count. Signed origins, package checks, extraction checks, serial execution and
the original64GiB/swap0 cgroup limits remain unchanged. A fresh complete
acquisition, native MD5 export and real TCPROS tests are still required.

The native String witness and descriptor-based execution gate now also have
an explicit ROS1 C++17 path using the maintained fixed-capacity native I/O
adapter. Their existing C++20 fast_io implementations are retained. This
supports selecting an actual Noetic compiler without changing ROS calls,
the native handshake or authority. It remains source compatibility work;
compiler success and native transport require separate actual evidence.

The positive typed-example harness must execute its SDK calls within the
actual main Wasm module. A WAT caller that imports a preloaded application
introduces cross-module ancestry and is correctly refused by the existing
ROS authority guard, even when both modules have matching alias grants.
Keep that path as a denial fixture. Same-main command entries must retain
the original complete typed SDK code and language initialization, with only
basic stdin/stdout control imports. This test design does not justify
weakening the Runtime's cross-module guard or count as a completed transport
test.

The first actual Source37 four-profile Worker build subsequently completed
the none/ROS1/ROS2 products, then failed during the dual profile's main TU.
The selected Clang23 process returned SIGSEGV at4,359,835,648-byte peak owned
RSS, below its5GiB budget, with no cgroup OOM. Its actual stack names
`DeadMachineInstructionElim` and `LiveRegUnits::stepBackward` while generating
`detect_cycles`. Inner report
`391e7bd238af36043388f5b4d6011ce350ddf164dd532363df684e0d2b8e00f0`,
outer `1aafadc86ed43a770a8471621333f0a0e1daf1cdefd2b58718ae03253619b784`,
preserve the failed command, diagnostic sources and successful owned cleanup.
The four-profile build remains failed; three completed products do not qualify
that complete set. Any host-compiler workaround must be explicit in actual
per-TU argv and rerun the complete selected build before it supplies four
Workers to the next Runtime acceptance stage.

The retained Source34 modular local correctness suite has now completed all
21 actual cases and58 process jobs. It exercised C/C++/Rust/AssemblyScript/
TinyGo vision providers, Wasm funcref initialization/callbacks, the selected
preload chains, ROS1/ROS2/dual default denials, and matching native/Wasm integer
checksums in one-shot and persistent sessions. Its terminal independent owner
report is
`5b67a377b2966ffd5ea03ee655a327e15426753884940e1256d3d46da923b090`,
coordinator `167ec221badf5e76926ac53c06d19947f9ea410c151d49e5ebb7cdb3d933b3df`,
actual case report
`14087b2fa038c1f96bcb8deeca3f8b809da6c02c488210dca0d9c84416c8b4f7`.
Owned cleanup, complete selected input inventories and original source/product
preservation passed. This is the declared21-case subset using its actual
Source34 artifacts; it is not all36 products' Runtime coverage, JavaScript
embedding coverage, a Source37 Worker result, native ROS transport, controlled
physics, PMU collection or a speed comparison. No optional Guest ROS rights
were granted by this suite.

## Latest completed execution checkpoints

The following results supersede earlier pending statements for their stated
scope. All actual jobs ran serially in the original current-boot64GiB/swap0
cgroup. Successful cleanup and preserved source/input catalogs were checked
after each terminal job; ancestor OOM counters remained zero.

| Scope | Actual result | Remaining boundary |
| --- | --- | --- |
| Fresh Source37 four-profile Worker build, revision2 | All12 TUs and four links passed | The Clang23 `-mllvm -disable-machine-dce` workaround is explicit on each main TU only. This is not the separately required patched LLVM provider. |
| Fresh Source37 embedded JavaScript build | All30 variants passed compilation, binary validation and import inspection | Build success alone does not qualify native ROS transport or performance. |
| Source37 embedded JavaScript local Runtime suite | All25 cases and26 variants passed | Covers actual JavaScript execution inside Wasm, both raw ROS codec paths, lifecycle, ownership, default denials, scalar modules and resource boundaries. Native ROS peers and physics are separate. |
| Source37 complete native74 build attempt | Failed at the portable String peer after all69 Gateway TUs and three links passed | The full batch is failed. A C++20 fast_io character-pointer adapter is corrected in Source39 and requires a fresh complete build. |
| Source38 same-main typed14 build attempt | Failed after one C product; the Rust compiler's successful linker stdout was absent from its diagnostic stream | A genuine opened-linker stdout/argv receipt and complete temporary-object accounting are implemented for the fresh retry. No native typed transport is qualified by this attempt. |
| Fresh Noetic amd64 acquisition, revision2 | Failed at package32 with official snapshot-server HTTP500 over IPv6 | The partial SDK remains unqualified. A fresh retry retains the signed inputs and package hashes, adds bounded network retries and explicitly selects IPv4 for this recovery only. |

The completed Worker build reports are
`c51e28251007812f1a4d5720865cdb5f0f1236136934378964cce0fe55f2786e`
(owner) and `18932cb8abc7d9998eb7431f751d6d1feea687cb380f1b247eef12750de0fb64`
(inner). The embedded JavaScript build reports are
`affd62ad5d1a36fbc9f5d6891cf44b979f2ad4df1bf98842624b4544f53a8bbf`
and `23e3c76a5ca83358599eb3cd8fcc36323e2d9d790cd9d1b3b84109d501be98e3`.
Its actual local Runtime reports are
`c206a8c5c246c789c1094eba7d58b7a6c4fcfb2e7cbe12515172c6ee04892b22`
and `000a512856611b6eb33da2a1850467a07035cb84b98eb4bc7c3493d0d7d13991`.

The failed native74 owner/inner reports remain
`d02e0d2918bf4d8cff481cee3fbb80298bc86b43d692d11a66c981f6e6860d3b`
and `7a108e08f4803caf9cc64bbdffb49565ea2dc88c8bf61c59b292eaf23d515cc1`.
The failed typed14 reports remain
`39fcffb488e32b10133260e38205cefd14ce70959c1833380a70fac0e096440d`
and `9e40152ff6bee24947f892b74fe22a2562d9b6a95d2fde7dac3e3dbc3c20b20f`.
The failed Noetic revision2 reports remain
`4a73fd29e0baaade7027441c1b8fe182c6c350e0b4c15a7534bfd404c5d6d292`
and `7cd45dab97cc16649cd3193e3338e10244865fbd92c5378531547186f168d4fa`.
Retaining these failures prevents a successful subcommand or later source fix
from being presented as a completed batch.

Source39 also adds the full native legacy ROS1 Unitree witness and its
documented handshake. It covers complete LowState/LowCmd field values, actual
generated native serialization/MD5 traits and explicit positive/denied modes.
These are source contracts pending the complete Noetic SDK, fresh compilation
and real TCPROS execution; ROS2 message identities cannot substitute for them.
Controlled DDS/MuJoCo behavior, matched native/Wasm timing, hardware profiling,
the multi-architecture matrix and broad distribution acceptance remain pending.

The next actual Source38 typed14 retry completed62 compiler/validation commands
before the GO AssemblyScript command hit a compiler assertion. Seven products
were observed, including the three Rust commands with their genuine linker
stdout/input receipts; the entire14-product batch remains failed. Its owner is
`a5edbb24d8fcd10958b2b8f257035bef9b5104c5d5a7851e2b2673af7548e9b8`
and inner report is
`e9f1a27ea7dc83f1ccad33943ba6680f50a4df8cafd66876effc82d147bfaad0`.
The actual installed AssemblyScript0.28.20 source map is identical to the
retained official package, SHA256
`0abd083398629da3a7578e905033be4115f6d609803d9c7d52452f7a1d4966a6`.
Its failing inferred-global type-resolution branch matches the GO/HG reader
initializer's callback dependency on that same reader. Source39 explicitly
annotates those two reader types before their initializers are compiled.
Callback bodies, initialization order, exports and authority remain unchanged;
a fresh complete14-product build is required to verify the fix.

Source39 now includes six genuine legacy ROS1 main applications in C, C++,
Rust, JavaScript inside Wasm, AssemblyScript and TinyGo, with full typed value
helpers, six declarative compiler recipes, language startup entries, a closed
JavaScript module manifest and detailed English usage/protocol guides. Every
positive app activates its real subscription before the native discovery
barrier. The full native witness and fresh negative/positive cases remain
mandatory; these source additions are not TCPROS acceptance results.

Noetic acquisition revision3 passed the earlier package32 download but failed
at package85 after its three real retries, again with an official snapshot
HTTP500, this time over IPv4. Its preserved owner and inner reports are
`06b87cba147b5201d75d4b021b255dcc6cf676d560717aab57a3a22fdfc862d2`
and `65a30ec0418432840202a3fe7340e533f43c0c2fe8cbd3c1e86576b8353fdf71`.
Cleanup and input preservation passed with no OOM; no complete SDK was
qualified. A subsequent acquisition may reuse retained deb bytes only after
checking them against freshly authenticated exact package versions, sizes and
SHA-256 digests. It must extract and qualify a complete fresh SDK rather than
resume or promote any of these failed partial SDK trees.
