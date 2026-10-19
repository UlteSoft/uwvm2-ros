# From Wasm imports to application APIs

The SDK is a guest-side library. It uses the Runtime's existing `uwvm_ros1_v1`
and `uwvm_ros2_v1` import modules; it does not link native roscpp/rclcpp or open
ROS sockets. The native Gateway creates the ROS objects after the Runtime has
authenticated the caller and admitted its explicit capability.

## Layer 0: exact binary imports

Start with your language's `raw` or `abi` module. It declares every function
actually registered for that backend: 38 for ROS1 and 43 for ROS2, including
`abi_version`, `capability_open`, `capability_close` and `capability_revoke`.
Read [abi-v1.md](abi-v1.md) before writing your own bindings. The machine-readable
[ABI contract](../abi/abi-v1.json) records each name and Wasm signature, and the
source checker compares it with the Runtime registry.

For example, opening alias `pub` takes `memory:i32`, the UTF-8 alias offset and
byte length as two `i64` values, and an `i64` offset to an eight-byte output slot.
The returned `i32` is an error code. On success, decode the slot as little-endian
`u64`; it is an opaque Instance/backend-owned handle, not a native pointer.
Use that handle in `topic_publish` with a canonical typed message.

Do not replace an `i64` offset with an ordinary wasm32 pointer in the import
declaration. Pointer convenience wrappers widen the address to `u64`; the
underlying function still imports `i64`. Multi-memory users select a memory
index and provide explicit offsets in the raw API. Pointer convenience wrappers
operate only on their compiler's memory zero.

## Layer 1: wire values and resource ownership

Codecs convert application values to the canonical formats in
[wire-format.md](wire-format.md). They use explicit little-endian integers,
UTF-8 **byte** lengths, finite IEEE754 values and bounded arrays. This format is
neither ROS2 CDR nor an arbitrary ROS1 serialized message. Use the provided
codecs for supported builtins; an unknown native type still needs a trusted
Runtime/Gateway adapter and policy identity.

A Capability owner opens one explicit alias and releases its handle when the
language permits deterministic disposal. C and TinyGo owners require explicit
close calls; Rust Drop and C++ destructors provide best-effort cleanup. Report
explicit close errors when cleanup status matters. Revocation invalidates all
open siblings for that alias; cloning a local value never clones authority.

Each operation has its own grant. A publisher cannot subscribe, a service
advertiser cannot respond, and a send-goal grant does not authorize result
consumption, cancellation or any-goal cancellation. Servers poll request/goal
events and respond through separately granted aliases bound to the same
resource/type. IDs are returned by the Gateway and must never be fabricated.

## Layer 2: Node and resource factories

The language APIs provide Node, Publisher, Subscription, service and Action
resources, parameters, graph queries, TF, lifecycle, clocks and logging where
the selected Runtime backend implements them. Factories take **aliases**, not
arbitrary topic names. The trusted policy fixes the actual resource name,
generated type identity, QoS, rate, byte, queue and lifetime limits.

Node is an application-side grouping object. Constructing it does not create a
new native node, inherit an environment variable, perform discovery or request
new rights. The Gateway node and ROS connection settings remain trusted host
configuration. These APIs offer familiar ROS programming patterns; they are
not source-compatible replacements for native rclcpp/roscpp, and do not claim
native feature parity.

## Layer 3: a bounded polling executor

The Runtime never invokes a Wasm callback from a native ROS thread. A
Subscription's `poll` fetches a validated event; an Executor's `spin_once`
decodes it and invokes the registered callback synchronously on the calling
guest thread. A callback may inspect its borrowed message only for the lifetime
documented by its language binding; copy a value before retaining it.

Executors use an explicit finite work budget. An empty queue returns error 11
and causes no callback. Sleeping, clocks, file access, threading and networking
are not silently added by the SDK. Applications that need a timer must obtain
the appropriate independent clock authority and schedule the next bounded
poll. A result or timeout must not cause an automatic non-idempotent retry.

## Language embedding boundaries

C, C++, Rust, AssemblyScript and TinyGo compile to Wasm guests using their
documented targets. Rust and TinyGo select their backend explicitly; use the
matching Runtime build profile. AssemblyScript is distinct from JavaScript and
uses WebAssembly import declarations.

JavaScript's SDK is a BigInt-aware client of a supplied Wasm guest bridge and
its shared `WebAssembly.Memory`. The embedding must provide the authenticated
ROS import implementation and caller-reserved scratch memory. Merely passing a
plain JavaScript object to a generic engine does not implement uwvm's Instance,
capability, Gateway or cgroup guarantees. The SDK does not supply a permissive
ROS network replacement and does not make uwvm execute arbitrary JavaScript.
See the language guide for the exact bridge contract and an optional C bridge.

## Verification boundary

Static registry checks and language codec/mock tests establish source and wire
contracts. They do not prove ROS delivery or a valid ARM execution mapping.
The repository's integration work uses the existing 64 GiB, zero-swap cgroup on Linux,
actual compiler inputs, verified ELF183 targets and live QEMU process mappings.
New compiled guests must be inspected for exact import types and then tested
against the real Runtime/Gateway. Missing compilers or SSH access are recorded
as pending/blocking conditions rather than successful builds.
