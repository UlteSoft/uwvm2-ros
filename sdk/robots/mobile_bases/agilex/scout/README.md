# AgileX SCOUT: separate complete ROS1 and ROS2 SDK selections

This kit preserves every original custom `scout_msgs` message at two immutable
official driver revisions, plus the complete original Twist/Odometry dependency
graph. It provides generated C11, C++20, Rust, JavaScript, AssemblyScript and
TinyGo values, checked C/C++ operation helpers, and executable example sources.
It does not claim every AgileX model, every ROS release, complete native SDK
compatibility, controller acceptance or the official vendor test suite.

| Original selection | Commit | Definitions / expanded records |
| --- | --- | ---: |
| Official `scout_ros` ROS1 default branch | `01e07881cdc566c3a657e288c59a75577992d13e` | 16 / 17, including original ROS1 time |
| Official `scout_ros2` Humble branch | `bf0110ff44cc70cdda2931d69f0e2669a3e26b9d` | 15 / 15 |
| Selected original `ugv_sdk` SCOUT/protocol source | `f2704eacdc90357078cd93ec60aae08bb4baab35` | Source provenance, no native SDK build |

Both selected catalogs preserve ten original constants and every original
field. ROS1 includes BMS, motor, driver and stamped remote state; ROS2 includes
its different actuator and remote records. ROS1 `enable_cmd_light_control` and
ROS2 `cmd_ctrl_allowed` are distinct original declarations. Status fields,
remote signedness, time representation and light topic names also differ.
Neither source ABI is substituted for the other.

`catalogs/ros1` and `catalogs/ros2` retain unchanged original message bytes and
their individual source locks. `upstream` retains Git commit/tree/capture data,
licenses and selected original driver/SDK source. Each original file was
checked against the captured Git blob SHA1 and a separately measured SHA256.
The SCOUT ROS repositories were captured as bounded original archives; the
UGV whole archive exceeded the acquisition bound, so only selected original
SCOUT/protocol blobs were fetched. This is not a complete UGV SDK acquisition.

## Start with the raw import boundary

Read the actual [ROS1 ABI](../../../../include/roswasm/abi/ros1.h) or
[ROS2 ABI](../../../../include/roswasm/abi/ros2.h), then the selected backend
header. The C/C++ `raw ALIAS` example performs exactly one real capability open
and closes any returned owner. With default-zero ROS rights it must return 13
and create no subscription, native socket or CAN command. Basic stdin/stdout/
stderr are independent of ROS authority. The SDK exposes no debugger, updater,
policy, credential or native network-management operation.

An alias is a Runtime policy name, never a native topic string chosen by the
Guest. A positive operator must independently bind the complete native source
metadata/canonical identities and select each exact backend, module, role,
topic, QoS, resource and finite quota. A source receipt, build option or example
include does not provide those facts or a grant. Do not reuse unbound source
IDs as proof of an installed native ROS type.

## Complete values and checked operations

Select exactly one `UWVM_SCOUT_ROS_VERSION=1` or `2` and its matching generated
directory. Include `include/scout.h` for C or `include/scout.hpp` for C++20.
The C++ header exposes `roswasm_agilex::scout::{Status,RemoteState,Odometry,
LightCommand,MotionLimits,Client}` and the real generated `roswasmcpp::Codec<T>`
specializations. No constructor opens an owner. Caller-owned storage is fixed
at 1 KiB and must not overlap a typed output. Serialize access to one Client.

The generated SDKs live in
[ROS1](../../../../custom-types/agilex-scout-ros1-complete/README.md) and
[ROS2](../../../../custom-types/agilex-scout-ros2-complete/README.md).
Use their whole-value encoders/decoders. Header and child-frame text are bounded
to 128 UTF-8 bytes; strings reject NUL and invalid Unicode. All four motor/
actuator records and both 36-element odometry covariances remain present.
ROS2 Quaternion's original declared `w=1` is available through the generated
default factory; ordinary C aggregate zero initialization is still zero data.
`Twist` uses the existing builtin wire/type path and is never registered again
as a new vendor endpoint. Its complete source record remains in the catalog.

The SDK refuses a mismatched ROS backend before a dequeue or publication. It
polls one event and decodes the whole value before committing output.
Idle 11 remains idle; oversize/malformed/transport errors do not grow buffers,
discard fields or replay a consumed event. Publish helpers submit one complete
value; successful enqueue is not native command feedback. Cleanup is attempted
once after an acquired owner, with primary and cleanup outcomes kept separate.

Motion requires explicit finite nonzero forward/yaw maxima, a nonnegative
lateral maximum and one explicit model: SCOUT, SCOUT Mini or SCOUT Mini Omni.
Zero-initialized limits refuse. Non-Omni models refuse any lateral command or
lateral allowance instead of depending on a driver to ignore it. The constructor
preserves the entire destination on refusal. These are operator application
limits, not independently established safe physical/controller maxima.

`stop` publishes one ordinary zero-velocity Twist under separately admitted
publish authority. It is not physical E-Stop, cancellation acknowledgement or
rollback. Closing a publisher does not stop the robot. The native operator must
own the controller watchdog, command deadline, external stop supervision and
uncertain-outcome handling. An error/timeout does not authorize retry.

## Pinned native endpoint differences

| Role | ROS1 original driver | ROS2 original driver |
| --- | --- | --- |
| Complete status | `/scout_status` | `/scout_status` |
| Remote state | `/rs_status`, stamped `ScoutRsStatus` | `/rc_status`, `ScoutRCState` |
| Lights | `/scout_light_control` | `/light_control` |
| Motion / ordinary zero command | `/cmd_vel` | `/cmd_vel` |
| Odometry | Explicit native `odom_topic_name` | Explicit native `odom_topic_name` |
| BMS | `/BMS_status`, original mapping is commented out | No BMS message in this selected tree |

The pinned ROS1 driver publishes a default BMS object while its assignment
lines are commented out. A decodable BMS packet is therefore not evidence of
real battery state and must not drive a battery/safety decision. Protocol V1
also leaves some ROS1 status members unpopulated. The complete SDK preserves
those declarations; it does not invent validity, a source sample or a sensor
value. Exact firmware/protocol/model and native driver behavior need separate
acceptance. The selected Mini Omni paths forward lateral command values, but
ROS1 odometry explicitly writes lateral twist zero; do not infer accurate
Omni lateral telemetry from its generated field presence.

## C and C++ build configuration

Every SDK CMake build lane defaults OFF. Existing Runtime option
`ros-vendor-agilex` also remains default-off. Shipping generated SDK source does
not add all vendor code to `uwvm2-ros`. Use the
[Runtime A+B build guide](../../../../../documents/ros/vendor-build-options.md)
to select AgileX with another vendor and explicitly disable all others. A
positive combined build still needs one coherent, independently native-bound
catalog union; an xmake option alone cannot create one.

For each ROS major, configure a fresh absolute directory with the matching
complete bundle and actual source root:

```sh
cmake -S sdk/robots/mobile_bases/agilex/scout -B /absolute/scout-native-ros2 \
  -DCMAKE_BUILD_TYPE=Release -DUWVM_SCOUT_ROS_VERSION=2 \
  -DUWVM_SCOUT_BUILD_CONTRACTS=ON \
  -DUWVM_SCOUT_SOURCE_ROOT=/absolute/uwvm2-ros \
  -DUWVM_SCOUT_GENERATED=/absolute/uwvm2-ros/sdk/custom-types/agilex-scout-ros2-complete
cmake --build /absolute/scout-native-ros2 --parallel 1
/absolute/scout-native-ros2/scout_contract_c
/absolute/scout-native-ros2/scout_contract_cpp
```

For real WASI Preview1 add its actual SDK toolchain and enable the examples:

```sh
cmake -S sdk/robots/mobile_bases/agilex/scout -B /absolute/scout-wasi-ros2 \
  -DCMAKE_TOOLCHAIN_FILE=/absolute/wasi-sdk-33/share/cmake/wasi-sdk-p1.cmake \
  -DCMAKE_BUILD_TYPE=Release -DUWVM_SCOUT_ROS_VERSION=2 \
  -DCMAKE_EXE_LINKER_FLAGS='-Wl,--gc-sections,--initial-memory=8388608,--max-memory=33554432,-z,stack-size=1048576' \
  -DUWVM_SCOUT_BUILD_CONTRACTS=ON -DUWVM_SCOUT_BUILD_WASI_EXAMPLES=ON \
  -DUWVM_SCOUT_SOURCE_ROOT=/absolute/uwvm2-ros \
  -DUWVM_SCOUT_GENERATED=/absolute/uwvm2-ros/sdk/custom-types/agilex-scout-ros2-complete
cmake --build /absolute/scout-wasi-ros2 --parallel 1
```

C++ ordinary WASI examples use the real `-fno-exceptions` profile, not substitute
exception or pthread symbols. Genuine final links, full Wasm validation and
actual import admission remain required. Cross targets need their own original
sysroot, float ABI and loader; changing an executable name is not an arch test.

`scout_guest_c.wasm` and `scout_guest_cpp.wasm` take `OPERATION ALIAS`:
`raw`, `status`, `odom`, `remote`, `light`, `motion`, `stop`, and ROS1-only `bms`.
Telemetry performs one poll; on success stdout contains the complete canonical
value. Light stdin is five bytes: enabled, front mode, front custom value, rear
mode, rear custom value. Motion stdin is 52 bytes: model u32LE followed by six
f64LE values (forward/lateral/yaw limits, then forward/lateral/yaw command).
All other modes require no stdin payload. The supplied operator values do not
alter Runtime policy. A default-denied open returns13 before any command input
is used or a native effect occurs.

## Rust, JavaScript, AssemblyScript and TinyGo examples

`examples/rust` is a complete local Cargo crate. Enable exactly one `ros1` or
`ros2` feature and build `--target wasm32-wasip1 --offline`. The real core crate
uses `alloc` and the selected backend. `main.rs` takes one explicit alias;
`read_once` returns the complete generated Status plus primary/cleanup codes.
It does not call a native ROS Rust binding.

`examples/telemetry.mjs` accepts an actual copied JavaScript-in-Wasm Session and
explicit matching ROS major. `examples/guest.mjs` uses the real SDK native
engine module, strict counted-alias stdin and two u32LE stdout status fields
(primary, cleanup). A successful engine/script exit is independent of the ROS
status; primary13 still means default refusal. Preserve unexpected thrown
values instead of manufacturing a ROS errno or treating engine success as
delivery. The closed ESM producer map must include the complete current SDK,
bootstrap, selected record modules and these exact entries. A host Node run is
not a Wasm test.

`examples/main_ros1.ts` and `main_ros2.ts` are real AssemblyScript `_start`
entries using the matching genuine NativeApi. Basic stdin is a nonzero u8 alias
length followed by exactly that many strict UTF-8 bytes. Compile one entry with
the actual SDK `abortGuest` binding, finite linear memory and the real AS runtime.
The reader uses `decodeChecked`; a default or partial object never stands in
for an unsuccessful decode. `lastValue` is updated only on complete success.

`examples/go` is a complete TinyGo module with the actual core module replacement
and two byte-identical generated record copies selected by original backend
build tags. Build the real `wasi` target with exactly one `ros1` or `ros2` tag.
The application takes one explicit alias, polls into a fixed generated bound,
checks the complete typed decode, and reports primary/cleanup independently.
No native-host Go SDK or fabricated ABI layout is used.

Source generation, compiler/final-link results, actual Wasm behavior, original
native metadata and physical controller acceptance are different stages. See
the retained per-lane results and the [platform status](../../../../../documents/ros/platform-sdk-status.md).
Positive Gateway/native driver traffic, physical/simulator/controller testing,
all releases, remaining AgileX products, genuine upstream suites and equivalent-
work hardware VTune Hotspots/Microarchitecture remain separate obligations.

## Recorded execution and product sizes

The [r37 execution record](../../../../../documents/ros/continuation-20261018-evidence/agilex-scout-six-language-sdk-r37/README.md)
contains the actual compiler/link commands, original-source checks, full Wasm
validation/import checks, default-zero results and retained failures. Each of
the six languages produced and executed a genuine Wasm application for each
ROS major. C/C++ value contracts also ran natively, in Wasm, and under x86_64,
i686, AArch64 and ARM hard-float user QEMU. This is SDK contract coverage;
native ROS/CAN communication and upstream official suites were not executed.

| Actual application | ROS1 bytes | ROS2 bytes |
| --- | ---: | ---: |
| C | 92,895 | 89,011 |
| C++ | 94,496 | 90,336 |
| Rust | 74,034 | 73,381 |
| JavaScript inside original QuickJS/Wasm | 1,531,388 | 1,531,971 |
| AssemblyScript | 19,287 | 17,643 |
| TinyGo | 203,073 | 200,578 |

These are exact retained unstripped application file sizes for the recorded
producers and profiles, not native Runtime size deltas or speed measurements.
The unchanged frozen dual-ROS interpreter used here is 6,573,776 bytes; its
original configuration and independent binary-size comparisons belong to the
[r35 build record](../../../../../documents/ros/continuation-20261018-evidence/platform-build-size-and-mobile-sdk-r35/README.md).
The SCOUT SDK source is selected separately and does not force extra vendor
adapters into that Runtime. Use explicit backend and `ros-vendor-agilex` build
selection plus the linked A+B guide for a different Runtime build.
