# Complete JointState lesson in six Guest languages

These are concrete source-only C, C++, Rust, JavaScript, AssemblyScript and TinyGo applications over the actual ROS Guest SDK and the complete generated official-standard JointState record. They are not a generated type implementation, a native publisher, or a vendor delivery receipt. No generator, compiler, Guest, native ROS process or test has run for this package. Java and C# are outside this family.

The two actual ROS types are `sensor_msgs/JointState` and `sensor_msgs/msg/JointState`. Header remains complete. ROS1 preserves `seq` and unsigned `stamp.sec`/`stamp.nsec`; ROS2 preserves signed `stamp.sec`/`stamp.nanosec` and has no `seq`. A negative ROS2 second and an empty optional velocity sequence are deliberate fixture boundaries. Nothing treats a native struct as a wire buffer or invents a Header/time message identity.

## Read the lesson

The application opens only the literal alias `joint-state`. The operator must configure its actual resource, type, native identity, complete canonical codec identity, bounds, connection and version-specific limits. This source supplies none of those identities and grants no permissions.

Each backend has two independent fresh Guest builds/instances. Mode0 is the default and requires ABI1 followed by actual open error13 and no acquired handle. Error11, unsupported95, invalid22, disconnected107 or a successful open is not a denied-case success. Every unexpected acquired handle is still closed. Mode1 must be explicitly selected in a separate new instance and have one actual topic.subscribe grant. It calls subscribe once, then receives one sample with checked full-value decode.

Every ordinary command poll is released by one actual `R` byte on basic stdin, at most64 credits. Idle11 consumes that credit and delivers nothing. The caller owns wall time, process/Runtime/Gateway cleanup, resource budgets and native discovery timing; the Guest creates no timer, thread, goroutine, callback from ROS, reopen loop or automatic resize-and-retry. After a complete value arrives, the program stops polling and closes every actual owner before its success marker. A trap, kill or interpreter crash cannot promise Guest cleanup; the external retained process owner remains mandatory.

The exact observer fixture is `observer-fixture.json` and matches the separately authored official-standard native codec roundtrip fixture:

- ROS1: Header seq19, stamp sec42/nsec300000000; ROS2: stamp sec-2/nanosec300000000.
- Header frame_id `base`; name [`left`, `right`].
- position [1.25, -2.5]; velocity **empty**; effort [0.5, -0.25].

Each language checks all fields and exact logical lengths after the full generated decoder accepts the bytes. The numeric fractions have exact binary f64 representations. No field projection, tolerance, native pointer, guessed offset or string-prefix match is used. The real decoder remains responsible for UTF-8, container bounds, finite numbers, canonical complete input and JointState length semantics.

## Actual generated products are prerequisites

Use the reviewed explicit `uwvm-ros-official-standard-bounded-v1` generator mode and actual current official source/bind receipts. Header and ROS2 Time are dependency-only; only the intended JointState interface is explicitly registered. The original normal custom/builtin-shadow restrictions remain in force outside that mode.

The generator must be run later by the authorized owner. This package ships no `bounded_records.*`, codec implementation, constant wire size, native hash, `manifest.hpp`, fake generated output or placeholder binding body. Put the entire actual output bundle at `sdk/custom-types/generated-official-standard` in a fresh owned SDK layout. Check every `generation.json.outputs` bytes/SHA and retain its actual catalog/definitions/generator/native identity receipts. Native registry/trait qualification is an independent prerequisite; a guest-only generated bundle is insufficient to authorize a real native entity.

Per-ROS generated SDK layouts are supported. A combined bundle may be used only if its real producer/binder accepts the combined official-standard source. JavaScript uses a namespace import and selects one fixed version-specific codec, so it does not demand nonexistent unselected exports. The other languages select types through preprocessing, cfg, build tags or a selected AssemblyScript module.

All buffer bounds and codec fingerprints come from actual public generated constants. C uses the uppercase `_MAX_WIRE_BYTES` macro; C++ uses `roswasmcpp::Codec<Message>::max_wire_bytes`; Rust uses `Message::MAX_WIRE_BYTES`; JS/AS use `Codec.maxWireBytes`; Go uses `...MaxWireBytes`. There is an additional lesson ceiling32768, with no fallback to a projected or clipped buffer. A refusal is an error, not permission to substitute another record.

## Source and staging graph

`recipe.json` contains explicit source staging and tool argv shapes, not executable shell strings or observed commands. Root must bind actual compiler/engine/archive/sysroot/SDK identities, original cgroup/boot/leaf and complete source manifest, then create fresh disjoint outputs. Keep all ordinary source inputs stable through compilation and execution.

C and C++ reuse the unchanged current `../telemetry.c` and `../telemetry.hpp` infrastructure. The C entry's `vendor_binding.h` supplies the complete concrete generated type, bound, actual decode function and full-value observer. It is not an SDK type stub. The C++ entry passes the real generated type/Codec specialization and the same complete inline C-layout value check. Both build with `UWVM_JOINT_STATE_ROS_VERSION=1/2` and default `UWVM_JOINT_STATE_MODE=0`; mode1 requires explicit `=1`. C also needs this lesson directory as an actual include root because the common component resolves `vendor_binding.h` through the compiler include search. The component never derives native identity from an alias or SDK node label.

Rust reuses the real `../telemetry.rs` executor/session ownership component and the actual generated `Canonical` implementation. `Cargo.toml` explicitly enables the SDK alloc feature, no default backend and exactly ros1/ros2. Mode1 is feature `positive`; omitted means mode0. Use an isolated one-job offline workspace and the actual WASIp1 target/libc/linker closure from the maintained Rust recipe. Do not modify or fabricate the generated `.rs` file to silence an API or link error.

The JavaScript command is `entry.mjs`, not a host Node program. It calls current `sdk/javascript/guest.mjs::createGuestSession()`, whose real `roswasm:guest_native` module and CopiedTransport run inside the actual compiled QuickJS-in-Wasm principal. Basic gate/marker adapters call original `readGuestByte`/`writeGuestOutput`. The application checks the real Session/CopiedTransport source classes, complete codec and explicit scratch bound; those checks alone are not native execution attestation. The maintained JS producer must stage the complete canonical ESM graph and engine/tool/header/relay closure, compile every actual job and validate the complete Wasm. Stage one exact `mode_default.mjs`/`mode_positive.mjs` as `mode.mjs`. Do not allow `os`, filesystem/network modules, an `env` mock or a host WebAssembly.Memory/Instance substitute. Actual whole artifact/maps/import proof remains external.

AssemblyScript keeps the actual generated import `../../assemblyscript/codecs` unchanged by retaining the logical SDK tree. Stage one exact binding_ros1/2.ts as `binding.ts`, matching header_ros1/2.ts as `header.ts`, and the selected mode source as `mode.ts`; record both original and staged full7s and byte equality. The selected header helper avoids referencing nonexistent ROS2 seq. `lesson.ts` exports `_start` and actual initialize/poll/close/diagnostic reactor functions. The command `_start` performs the same finite stdin-driven flow. `wasi_stdio.ts` uses only exact basic fd_read/fd_write/proc_exit with live managed memory and checked lengths; there are no additional ROS or management imports. The generated AS decoder must pass `valid(bytes)` before its decoded value can be exposed. The selected real SDK abort override remains required; never allow `env.abort` to hide a build error.

TinyGo lives in the separate `go/` directory so C/C++ lesson files are not incorrectly passed through the Go package loader. Copy the actual generated `bounded_records.go` byte-exact into `go/generated/bounded_records.go`, preserving the original complete generator bundle and separately recording the staged copy pin. Its package remains `customtypes`; the lesson imports it through its own module subdirectory. No generated Go source or package declaration is rewritten. Exactly ros1/ros2 build tags select their real type/decoder/Header layout. `joint_state_positive` is opt-in mode1; absent is mode0. Use the actual TinyGo `wasi` target, scheduler none, one job and offline matching Go/TinyGo roots. Original runtime entropy/startup imports can still make a binary unsupported under empty WASI permissions; no automatic random grant or entropy fallback is introduced.

## Source-only build matrix and independent acceptance

The authored matrix is six languages × two ROS backends × two fresh modes, 24 source recipe entries. It is not24 compiled or delivered cases. There are no actual command, vendor, robot, performance, network or official case credits in this package.

Do actual preparation/generation first, admit/freeze the resulting SDK layout as its own coherent source, then compile fresh Guests. Bind actual same-SDK C++ archive layout through the reviewed selector; do not assume flat WASIp1 libc++ paths. Preserve every compiler input, full dependency rule, link trace, native/Wasm validator product, SDK inventory and final source pin. Both generated complete codecs and standard native traits/identities need independent qualification.

For real delivery the operator must separately own an actual matching native publisher and Gateway registered with the full generated bounded record. That publisher sets all fixture fields and confirms its actual publish. The native_roundtrip.cc fixture is a codec/native-trait oracle; it is not a ROS publisher. The successful Guest marker and count1 show only that the Guest checked its received value. Original runtime/native target ELF/full-MD/link/maps, lease/process/cgroup/SDK proof, actual topic/type/native identity/QoS and source generation must still be qualified together by a finite current owner.

Default-denied and positive runs may use basic WASIp1 startup/stdin/stdout/exit as actually retained by their toolchains. Inspect all emitted imports and explicit permissions. A real Runtime/profile mismatch, unknown import, missing decoder, malformed body, failed close, disconnected Gateway, startup requirement, stale generation or unsupported compiler is a refusal. Do not replace the official runtime, grant debug/update/management authority, enlarge Guest rights, shrink fields or treat a local mathematical model as transport acceptance.


## Explicit C command exports, narrow r2

The C command recipe now passes seven literal linker options, one per existing
actual source symbol: `joint_state_initialize`, `joint_state_poll_once`,
`joint_state_close`, `received_count`, `denied_verified`, `case_status`, and
`cleanup_error`. They are supplied as individual `-Wl,--export=NAME` argv tokens;
none is accepted from catalog data. The ordinary command main/_start and every
application/source/permission/poll/cleanup path are unchanged. C++ remains a
command entry; AssemblyScript already has explicit source exports.

These extra exports alone do not define a WASI reactor startup ABI. No
`_initialize` export, command-to-reactor conversion, automatic constructor call,
basic-WASI permission, or actual Host invocation is assumed. An independently
reviewed caller must establish the selected actual compiler/runtime startup and
fresh-instance contract before using the extra C entrypoints. Do not invoke
reactor steps on an instance already consumed/closed by command main. Actual
Wasm validation must inspect all seven exported signatures and every retained
ROS/WASI import; link flags or symbol names never prove execution or grant
control/management rights. The complete original r1 is preserved in before-r1
and at its original path. This is a source-only recipe/docs correction, not a
compiler or behavioral result.
