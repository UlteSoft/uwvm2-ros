# Spot bridge: complete selected ROS2 values and RPC owners

This directory installs the existing generated Spot bridge Source in the
canonical SDK layout. All fourteen product files and `generation.vendor.json`
are byte-identical to the originals in
`sdk/robots/mobile_manipulation/spot/protobuf/bridge-generated`.
`installation.vendor.json` records that copy; it does not claim a new generator
run or replace the original producer identities. The canonical layout makes the
real `../../assemblyscript` imports resolve correctly. Keep these files as one
coherent package and preserve the original nested generation.

The five project-authored ROS2 services expand into ten complete request and
response records in C11, C++20, Rust, JavaScript, AssemblyScript and TinyGo.
They are not official Boston Dynamics ROS definitions or an implementation of
every Spot SDK operation. The [native operator profile](../../robots/mobile_manipulation/spot/README.md)
fixes SDK 5.0.1 and separately limits native operations. Native credentials,
gRPC channels, real leases and robot-side safety remain outside the Guest.

| Service schema | Explicit resource roles | Complete response facts |
| --- | --- | --- |
| State | Telemetry only | Acquisition time, whole joint projection, power enum and complete RobotState protobuf |
| Lease | Acquire, retain and return are distinct aliases | Correlation, provider refusal, uncertainty and provider lease state |
| Command | Stand, sit and Stop are distinct aliases | Correlation, refusal, uncertainty, acceptance and native command ID |
| Velocity | One bounded velocity resource | Correlation, refusal, uncertainty, acceptance and native command ID |
| Feedback | One explicit feedback resource | Presence/status projection and complete RobotCommandFeedbackResponse protobuf |

State and Feedback retain up to 16,384 original protobuf bytes. Whole response
limits are 32,768 bytes. An oversized value fails; no field or prefix is silently
discarded. `native_proto` is owned Guest data, never a Host object or pointer.
The [full protobuf SDK](../../robots/mobile_manipulation/spot/protobuf/README.md)
provides genuine field access to all 1,319 production message descriptions in C
and C++. Other languages retain their complete canonical bridge values, but
this package does not claim a full protobuf field SDK for each language.

## Start at the raw import boundary

Read `sdk/include/roswasm/abi/ros2.h` and `roswasm/ros2.h` before the typed
wrappers. Selecting a backend creates no node, socket, thread, clock, credential
or permission. With no Runtime grant, opening the actual alias must return 13
and publish no owner. Use a fresh instance with separately reviewed exact
module/backend/alias/resource/type/native/canonical identities and finite limits
for a positive deployment. A research inventory or xmake switch is not that
authorization. No update, debugger, policy or credential-management API is
added by these services.

Every request requires nonzero token and nonce and an explicit finite native
budget. These are replay/correlation data in the shared native profile, not
authenticated Runtime identities or a per-Guest lease boundary. Before consuming
any response, check ROS transport success, matching token and nonce, provider
`error == 0` and `unknown == false`. A valid canonical record alone does not
establish those application conditions, native acceptance or physical safety.

## C and C++

Add this directory and the actual `sdk/include` to the selected Wasm compiler's
include paths. `bounded_records.h` supplies complete caller-owned records,
checked sizes, validation and whole-value encoders/decoders.
`bounded_interfaces.h` supplies typed service client/server owners. Start from
the generated complete default factory and retain all errors. Do not pass a
native ROS struct image or truncate the protobuf to fit a smaller buffer.

The maintained `spot/sdk.h` performs one checked C transport call, validates
correlation and only then assigns the full response. Its buffers must exist
before opening a capability. `spot/sdk.hpp` exposes the corresponding complete
C++ aliases and `uwvm_mobile::spot::Client`; it shares that actual checked C
transport path. `bounded_records.hpp` also supplies the matching
`roswasmcpp::Codec<T>` specializations. Include the matching RPC header when
using generated owners. Different generated domains must not be concatenated.

The read-only State examples in `spot/protobuf/examples/guest_state.c` and
`guest_state_banked.cc` use the genuine bounded C protobuf decoder. The C++20
banked example links that C SDK; it is explicitly separate from the protobuf
Lite C++ class API and its real threaded/EH WASI toolchain requirements.
Both read three little-endian u64 values from stdin (token, nonce, budget),
open one State alias, submit once, and report primary/transport/provider/decode
and cleanup status separately. Neither requests a lease nor sends motion.

The independent `spot/examples/guest_sequence.c` teaches the eight-role
telemetry/lease/command lifecycle. It requires all aliases and operator input
explicitly. Stop and return are separately selected operations, not inverse
rollback or a proof of physical stopping. A lost or uncertain request cannot
authorize retry, reacquisition, controller-mode change or replay.

## Rust, JavaScript, AssemblyScript and TinyGo

Rust declares `bounded_records` and `bounded_interfaces` as sibling modules and
uses the actual `sdk/rust` crate with explicit `alloc` and `ros2` features.
Values implement `roswasm::typed::Canonical`. The generated State client owns
only explicitly named roles and retains construction, rollback and cleanup
errors. Inspect `Result` and complete response fields before opening a decoder.

JavaScript imports complete `*Codec` classes from `bounded_records.mjs` and RPC
owners from `bounded_interfaces.mjs`. Keep token/nonce/budget values as BigInt.
Use the actual JavaScript-in-Wasm producer and core Session independently;
running a module in host Node is not a Wasm ROS test. A constructor refusal has
its own construction/cleanup diagnostics and must not be converted to success.

AssemblyScript imports the real records/`*Codec` classes and
`Ros2Uwvm_spot_bridgeSrvStateClient` from the matching modules. Inspect checked
encode/decode status and the service reply status before accessing a value.
Keep this package exactly one level below `sdk/custom-types`. No abort/trap can
promise cleanup after the Guest has terminated.

TinyGo keeps the two generated `.go` files together in a `customtypes` package,
resolves `uwvm2.local/roswasm` to the actual SDK module, and selects the genuine
Wasm target with `ros2`. Checked `WireSize`, `Encode`, `Decode*` and owner calls
return errors instead of partial values. Build tags select code, not rights.

Actual generation, native identities, compiler/final-link results, full Wasm
validation, Guest behavior, native ROS delivery and physical Spot qualification
are different facts. Consult the [platform status](../../../documents/ros/platform-sdk-status.md)
and retained per-lane receipts. This installation alone supplies no execution
or whole-vendor completion claim.
