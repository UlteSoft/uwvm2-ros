# Platform SDK and example delivery status

All requested platform SDKs/examples are **not complete**. A default-off xmake
option establishes build selection, not a driver, full client-library migration,
complete SDK, executable example or accepted controller. Keep source coverage
and actual execution results separate.

The current shipped-source audit checks 38 generated families, covering selected
interfaces for 16 of the 28 inventoried vendors plus three NVIDIA data layers.
Those 16 vendors are not declared completely supported: the selected model/API,
ROS distribution, native metadata and test scope still constrain each family.
The other 12 vendors lack a complete generated six-language family in this
specific audit; some have narrower historical/handwritten/protobuf kits.

| Vendor | Shipped source scope | Remaining whole-platform work |
| --- | --- | --- |
| Unitree | GO, HG, API and ROS1 complete selected record families | All requested model/release/driver and positive Guest qualification |
| AgileX | Separate complete selected SCOUT ROS1/ROS2 catalogs; six-language Wasm examples actually default-denied; complete C/C++ value contracts and x86/Arm user-QEMU contracts | Positive native driver/CAN/model/protocol acceptance, other AgileX products/releases, official suites and performance |
| Elephant Robotics | Six complete original Kinetic/Melodic/Noetic/Foxy/Galactic/Humble source families, thirty explicit model selections, six-language Wasm examples and independent full-field x86/Arm contracts | Positive native ROS/pymycobot/driver/model/firmware acceptance, other releases, original official suites, simulation and hardware performance |
| UFACTORY | Separate historical xArm kit | Complete selected-source/native closure and current qualification |
| DOBOT | Research inventory and shared standard examples | Complete vendor SDK and execution |
| Flexiv | Research inventory and shared standard examples | Complete vendor SDK and execution |
| Leju | Historical ROS1 interface evidence | Complete ROS1 SDK/execution; independent evidence before claiming ROS2 |
| DEEP Robotics | ROS2 research evidence | Complete selected SDK/execution; independent evidence before claiming ROS1 |
| RealMan | Research inventory and shared standard examples | Complete vendor SDK and execution |
| Clearpath | Four complete selected original source graphs (Noetic plus legacy Husky/Jackal, Humble, Jazzy, main), six-language denied applications and independent full-field canonical contracts | Positive native ROS/driver/controller/model qualification, other releases, official suites, simulation and hardware performance |
| ROBOTIS | Complete selected TurtleBot3 Noetic/Humble/Jazzy/main source graphs, six-language default-denied Wasm examples, whole C/C++ contracts and x86/Arm user-QEMU contracts | Positive native ROS/firmware/model/release qualification, other ROBOTIS products, official suites and performance |
| Universal Robots | Three installed complete selected ROS1/ROS2/PolyScope X source families, six-language Wasm examples and x86/Arm user-QEMU codec contracts | Positive native/controller/release acceptance, other requested releases, official suites and performance |
| Franka | Complete selected Noetic/Jazzy records and six-language Wasm examples, full C/C++ value contracts and x86/Arm user-QEMU contracts | Native FCI/controller/model/release qualification, upstream suites and performance |
| PAL Robotics | Research inventory and shared standard examples | Complete model-specific SDK and execution |
| Doosan | Research inventory and shared standard examples | Complete controller/version SDK and execution |
| Trossen / Interbotix | Research inventory and shared standard examples | Complete model-specific SDK and execution |
| AgiBot | ROS2 research evidence | Complete selected SDK/execution; independent evidence before claiming ROS1 |
| UBTECH | ROS2 research evidence | Complete selected SDK/execution; independent evidence before claiming ROS1 |
| Fourier | Research candidate | Verified public driver/interface closure before a concrete support claim |
| FANUC | Complete selected ROS2 records and C/C++ examples; separately qualified native interface lane | Positive Guest delivery, complete controller/driver/releases and other-language example execution |
| KUKA | Complete selected ROS2 records and C/C++ examples; separately qualified native interface lane | Positive Guest delivery, complete controller/driver/releases and other-language example execution |
| Yaskawa | Complete selected ROS2 records and C/C++ examples; separately qualified native interface lane | Positive Guest delivery, complete controller/Agent/releases and other-language example execution |
| Boston Dynamics / Spot | Complete selected five-service bridge records in six languages; genuine full-field C/C++ protobuf SDK; native C/C++ and Wasm C contracts; x86/Arm C codec QEMU runs | Remaining operation/language examples, positive bridge/robot delivery, full release/official-suite/performance qualification |
| Kinova | Complete selected Humble JointState/FJT records in six languages, model wrapper and C/C++ Wasm examples | Positive Guest/driver/model qualification; complete remaining Kortex/gripper functions and language execution |
| ABB | Complete selected ROS1/ROS2 records and C/C++ example Source | Original native SDK, executable examples and full controller/release qualification |
| Booster Robotics | Complete selected ROS2 and authored native-bridge records | Native bridge, actual Guest/driver/model and language example qualification |
| LimX Dynamics | Complete selected authored native-bridge records | Native bridge, actual Guest/driver/model and language example qualification |
| Hello Robot / Stretch | Complete selected Humble JointState/FJT records in six languages, model wrapper and C/C++ Wasm examples | Positive Guest/driver/model qualification; remaining mode/stop/navigation functions and language execution |

Isaac ROS, Isaac Sim and GR00T each have a separate generated Source family.
Jetson has an explicit deployment admission profile, not a generated robot API
or direct Guest CUDA access. Their simulation, GPU-node/model integration and
hardware/release acceptance remain separate. Tesla Optimus remains research
pending a verified public control interface; no SDK is fabricated for it.

## Reproduce the source audit

```sh
python3 -I -S -B sdk/tools/audit_ros_platform_sdks.py > /absolute/sdk-source-audit.json
```

The report verifies every `generation.vendor.json` output byte/hash and all six
record-language products. It never changes the historical research inventory or
adopts a Source receipt's old `NOT_RUN` field as a current execution result.
`--require-all` deliberately fails while any requested vendor lacks a selected
complete generated family. File/hash coverage does not mean all methods,
examples, original rcl APIs, ROS releases or official upstream tests pass.

The [r35 original results](continuation-20261018-evidence/platform-build-size-and-mobile-sdk-r35/README.md)
add the mobile Humble FJT generation, actual C/C++ Wasm builds, full validation,
two executable codec contracts and default-denied Gen3 application execution on
the newly built frozen-source dual-ROS interpreter. No positive ROS traffic,
physical controller effect or non-C/C++ Guest result is claimed by that lane.
Earlier FANUC/KUKA/Yaskawa native and Guest lanes retain their exact original
scope and Runtime/source pins; they are not relabeled as current whole-project
or all-vendor acceptance.

The [r36 Spot results](continuation-20261018-evidence/spot-full-field-sdk-and-wasm-r36/README.md)
install the already-generated Spot bridge package in the canonical SDK layout;
this corrects an audit omission and the AssemblyScript relative import layout,
not a newly invented upstream service. They also fix the actual nanopb C keyword
generation failure, compile both full-field native SDKs, exercise all 1,319 C
message bindings natively/in Wasm/on x86/Arm user QEMU, and execute three
default-denied C/C++ applications. A successful empty message plus a refused
malformed tag for each type is not exhaustive field-value or official-suite
coverage. No positive native ROS or physical Spot result is inferred.

The [r37 SCOUT results](continuation-20261018-evidence/agilex-scout-six-language-sdk-r37/README.md)
retain separate original ROS1/ROS2 message closures and genuine final Wasm
products for C, C++, Rust, JavaScript, AssemblyScript and TinyGo. They exercise
default-zero opens, whole-value C/C++ contracts and 16 actual user-QEMU contract
runs across x86_64, i686, AArch64 and ARM hard-float. The JavaScript engine's
successful exit is recorded separately from its refused ROS operation. Failed
intermediate builds and harness attempts remain in the archive. These SDK
contracts do not qualify native ROS/CAN delivery, a cross-host Runtime, other
AgileX models/releases, official upstream suites or hardware performance.

## Build and deployment obligations

The [r38 UR record](continuation-20261018-evidence/universal-robots-complete-sdk-r38/README.md)
adds an installed and durably retained ROS1/ROS2/PolyScope X SDK: 146 original
definitions across three families, 214 expanded records, 18 genuine six-language
Wasm applications and 24 x86/Arm user-QEMU codec contracts. The current source
audit at that installation checked 22 families / 12 vendors. The installation receipt verifies all
484 owned source files against the tested archive; earlier ENOSPC attempts did
not partially overwrite vendor directories. The independently qualified fixed
floating-array default producer extension is also installed. No positive native ROS,
controller, official-suite or hardware performance result is inferred.

The [r40 Franka record](continuation-20261018-evidence/franka-complete-sdk-r40/README.md)
adds two installed complete selected Noetic/Jazzy families: 64 original
definitions, 46 custom roots, 108 expanded records and 18 constants. Twelve
actual six-language Wasm applications, four native and four Wasm C/C++ contracts,
and 16 x86/Arm user-QEMU codec runs passed. The Linux post-installation check
verifies 24 families / 13 vendors from the current workspace. Native ROS/FCI,
controller, other-release, official-suite and hardware performance qualification
remain separate and unrun in that lane.

The [r42 TurtleBot3 record](continuation-20261018-evidence/robotis-turtlebot3-complete-sdk-r42/README.md)
adds four installed complete selected Noetic/Humble/Jazzy/main development
snapshot families: 104 original definitions, 25 custom roots and 130 records.
Twenty-four genuine six-language Wasm applications, eight native and eight Wasm
C/C++ whole-value contracts, and 32 x86/Arm user-QEMU runs passed. Final source
profile/definition/product checks also reject cross-distribution substitution.
The actual Linux post-installation audit checks 28 families / 14 vendors;
all-vendor require-all remains an expected refusal. Native ROS/firmware/model,
other-release, official-suite, simulator and VTune qualification remain unrun.

The [r43 Clearpath record](continuation-20261018-evidence/clearpath-complete-sdk-r43/README.md)
adds four complete selected original graphs: 290 definitions, 264 original
custom roots, 445 expanded records and 656 constants. Six explicit application
selections distinguish platform status, legacy Husky and Jackal. Thirty-six
genuine six-language Wasm applications remain default-denied. Independent
nonzero vectors cover all 445 records and 5,689 scalar/string field instances
in C/C++ and each managed language, including 192 actual C/C++ program runs
and 64 actual Rust/AssemblyScript/TinyGo/QuickJS-in-Wasm runs. The separate
whole-value lane adds 48 x86/Arm user-QEMU C/C++ contracts. A real generated
AssemblyScript native-name collision was fixed and all 126 products across nine
previous original domains remain byte-exact. These are authored SDK/data tests;
positive native delivery, original official suites, physical command validity,
simulation, BE/soft-float ROS targets and VTune remain unqualified in this lane.
The post-installation audit checks 32 families / 15 vendors; require-all remains
an expected refusal while the other 13 vendors lack a selected complete family.

Use the [explicit A+B configuration](vendor-build-options.md) and
[complete generated SDK guide](../../sdk/custom-types/COMPLETE_VENDOR_SDK.md).
The one coherent registry rule applies across the Runtime, Guest SDK and native
Gateway. The catalog merger refuses conflicting shared definitions/bounds; a
source union still needs original native metadata and independent binding.

Every new vendor/model requires its full original source/schema closure,
complete typed conversion and lifecycle/timeout/cancel/stop semantics, narrowly
granted positive traffic, independently observed native values, malformed and
unauthorized cases, genuine official-suite coverage and equivalent-work
performance measurements. Requested ROS x86/ARM/QEMU and VTune hardware Hotspots
and Microarchitecture qualification remain open beyond the retained scoped
results. Guest default authority stays empty throughout this work.

The [r44 Elephant record](continuation-20261018-evidence/elephant-mycobot-complete-sdk-r44/README.md)
adds six exact original distribution snapshots: 369 declarations and 555
expanded records, with thirty explicit model/source selections. Genuine C,
C++, Rust, AssemblyScript, TinyGo and QuickJS-Wasm examples retain zero ROS
authority. Independent nonzero vectors cover 1,863 scalar/string field instances
in every record language. A combined Clearpath/Elephant Noetic source domain
also retains all 334 original declarations and 534 complete records. This is
selected SDK/data/default-denial qualification; it does not qualify native ROS,
firmware, all client-library APIs, all releases, official suites or performance.
