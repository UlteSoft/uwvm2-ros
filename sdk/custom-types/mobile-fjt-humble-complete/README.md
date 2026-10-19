# Kinova Gen3 and Stretch: complete Humble trajectory values

This is one actually generated Source domain for the selected standard ROS2
interfaces used by the [Gen3 kit](../../robots/mobile_manipulation/kinova/README.md)
and [Stretch kit](../../robots/mobile_manipulation/stretch/README.md). It contains
15 whole original definitions, expanded into 17 complete records, and registers
only JointState and FollowJointTrajectory. The Goal, Feedback and Result retain
all scalar/MultiDOF trajectories, tolerance arrays, timestamps, constants and
original defaults. Finite canonical capacities are explicit in the original
`catalog-fjt-humble.json`; no field is removed to satisfy the generator.

The complete C11, C++20, Rust, JavaScript, AssemblyScript and TinyGo records and
RPC owners were emitted together. `generation.vendor.json` binds every output
byte and the actual producer/template/source identities;
`preparation.vendor.json` preserves the preparation inputs. Keep both receipts
unchanged. This is Guest-only preparation: no native registry, native type
identity, firmware compatibility, full driver or physical robot acceptance is
created by these files. The two vendors share standard interfaces, not native
controller sessions, model configuration or permissions.

## Begin with raw imports and no authority

Read `sdk/include/roswasm/abi/ros2.h`, then `roswasm/ros2.h`. The backend object
selects checked Wasm imports; constructing it starts no node, socket, thread,
lease or controller. Open an explicit alias with no Runtime grant and expect
permission error 13. A positive application needs separate exact aliases for
telemetry subscription, goal send, feedback, result and own-goal cancellation,
including actual type/native/canonical identities, resource names and finite
limits. The SDK cannot grant rights, choose credentials or configure Runtime
debugging, updates or policy.

For C, include `bounded_records.h` and `bounded_interfaces.h` from this directory
plus the real `sdk/include`. Each counted string/sequence owns its declared
capacity. Start complete defaults with the generated `_default` factory, then
use `_valid`, `_wire_size`, `_encode` and `_decode`. A decoder commits a complete
value; an invalid length, normalized-time violation, trailing data or capacity
refusal cannot publish a prefix. Keep record, byte buffer and length-output
storage separate. The shared `fjt_goal.h` constructs explicit six/seven-joint
Gen3 or single-lift Stretch convenience goals; the complete record remains
available for other reviewed fields. These examples do not validate collisions,
physical joint limits, actuator readiness or tool calibration.

For C++, include `bounded_records.hpp` and `bounded_interfaces.hpp`. The actual
`uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal` and corresponding
Feedback/Result aliases use `roswasmcpp::Codec<T>`. The maintained
`fjt_client.hpp` borrows four independently opened owners and preserves owned
goal IDs and uncertain outcomes. The ordinary WASI libc++ recipe uses
`-fno-exceptions`: ROS refusals remain numeric, while allocation failure follows
that target library's termination policy. An exception-enabled application needs
a separately qualified producer/runtime profile.

## Use the matching generated language domain

Rust declares `bounded_records` and `bounded_interfaces` as sibling modules and
uses the actual `sdk/rust` crate with explicit `alloc` and `ros2` features.
Generated values implement `roswasm::typed::Canonical`; validate checked sizes
and preserve `Result` errors. The RPC owners require actual distinct aliases and
retain construction/rollback/cleanup diagnostics. A native Rust ROS process is
not a replacement for those Wasm imports.

JavaScript imports the matching `*Codec` from `bounded_records.mjs` and the
corresponding RPC classes from `bounded_interfaces.mjs`. Construct complete
defaults, call `encode(value)` and decode one whole `Uint8Array`. Keep u64/i64
identities as BigInt. Select the actual JavaScript-inside-Wasm producer and core
SDK Session separately. Running this codec in host Node does not qualify a
Wasm-to-ROS route.

AssemblyScript imports the matching records/`*Codec` classes and RPC owners.
Inspect the returned status of `encodeChecked`/`decodeChecked` before publishing
a result. Keep this directory one level below `sdk/custom-types`, so the real
`../../assemblyscript` imports retain their SDK context. Backend selection and
compiler target remain explicit; no host JavaScript object is a native pointer.

TinyGo keeps both generated `.go` files in one `customtypes` package. Resolve
`uwvm2.local/roswasm` to the real `sdk/tinygo` module, compile with the actual
Wasm target and `ros2` build tag, and preserve every checked `WireSize`, `Encode`
and `Decode*` error. The generated build constraints prevent accidental ROS1 or
ordinary host-Go compilation from masquerading as a TinyGo ROS2 Guest.

## Build only the required example family

The mobile CMake entry has separate default-off FJT and Spot switches. A Gen3
or Stretch FJT build needs no Spot generated bundle or native Spot SDK:

```sh
cmake -S "$UWVM_SOURCE/sdk/robots/mobile_manipulation" -B "$FRESH_BUILD" \
  -DCMAKE_TOOLCHAIN_FILE="$WASI_SDK/share/cmake/wasi-sdk.cmake" \
  -DCMAKE_C_COMPILER_TARGET=wasm32-wasip1 \
  -DCMAKE_CXX_COMPILER_TARGET=wasm32-wasip1 \
  -DUWVM_MOBILE_BUILD_FJT_GUESTS=ON \
  -DUWVM_SOURCE_ROOT="$UWVM_SOURCE" \
  -DUWVM_MOBILE_FJT_PRODUCT="$UWVM_SOURCE/sdk/custom-types/mobile-fjt-humble-complete" \
  -DCMAKE_EXE_LINKER_FLAGS="-Wl,--initial-memory=2097152,--max-memory=33554432,-z,stack-size=1048576"
cmake --build "$FRESH_BUILD" --parallel 1
```

Set the toolchain's finite initial/maximum memory and stack for the actual
embedding. All three products have `.wasm` suffixes. The C/C++ contract programs
exercise complete value codecs; `uwvm_mobile_fjt_guest.wasm` is the real WASI
application. Its arguments are `gen3-6dof|gen3-7dof|stretch-lift` followed by the
five explicit telemetry/send/feedback/result/cancel aliases. Standard input
starts with the selected joint count of little-endian f64 positions, followed
by explicit application credits (`T`, `S`, `F`, `R`, `C`). Credits bound work;
they do not measure time or readiness. No implicit native clock is available.

The application opens telemetry before the four Action roles, consumes a whole
JointState, submits once, and records primary/cleanup errors separately.
Cancellation acceptance is not terminal result or physical stop. Timeout or a
lost response after possible submission remains unknown and cannot authorize
replay, reconnection or an automatic controller-mode change. Native driver,
controller safety and the real stop procedure remain independently supervised.

The [platform delivery report](../../../documents/ros/platform-sdk-status.md)
distinguishes generated Source, actual C/C++ Wasm compilation and default-denial
checks from the still-required positive Guest/native/controller qualification.
