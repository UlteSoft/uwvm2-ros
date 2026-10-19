# Bounded custom Message, Service and Action profile

`uwvm-ros-custom-bounded-v2` implements a finite, trusted source profile alongside
`uwvm-ros-custom-fixed-v1`. This document describes implementation and the
required acceptance workflow. A checked schema, generated codec, compiled Guest
or prepared native probe does not establish native delivery. Actual reports from
the unified lane remain the acceptance authority.

Registration grants no rights. A default Instance has only basic stdin/stdout/
stderr. Every custom Topic, Service and Action role needs its own explicit CLI
alias, resource, official native identity, canonical fingerprint and limits.
Service advertise/respond and Action advertise/respond are separate owners.
Cancel-own and optional cancel-any are independent permissions; receiving a Goal
ID never authorizes cancellation of another owner's Goal.

## Supported source and canonical values

The generator accepts complete `.msg`, `.srv` and `.action` definitions with
explicit scalar widths, Bool, finite IEEE-754 values, fixed arrays, bounded UTF-8
strings, bounded sequences and catalog-declared nested message dependencies.
Fields retain declaration order. Little-endian scalars have no native padding;
strings contain a uint32 byte count and UTF-8 bytes; sequences contain a uint32
element count; fixed arrays and nested records have no extra envelope. Bool is
exactly0 or1. UTF-8 rejects NUL, overlong sequences, surrogates, truncation and
values above U+10FFFF. Nonfinite float bit patterns are invalid in both directions.

ROS1 uses official `string` and `Type[]` spelling with separately trusted catalog
bounds. ROS2 `string<=N` and `Type[<=N]` preserve the actual native declarations;
a catalog may narrow their canonical bounds. Bounds never expand an official
native bound. A maximum of128 registered interfaces,4096 expanded schema nodes
per component,16384 aggregate nodes,16 nested levels and32768 canonical bytes per
component bounds parsing, source generation and payload traversal. Runtime
capability quotas may select smaller active message sizes. Native encoders
validate and measure active fields before allocating the exact canonical size.
They do not reserve the maximum layout for every small message.

Empty Service and Action components have zero canonical bytes and still require
their normal nonzero ticket/Goal envelopes. ROS2's generated native empty-member
placeholder is independently checked and initialized to0. Empty native Topics
are refused. Unknown dependencies, recursive layouts, constants/defaults,
ambiguous byte/char aliases, implicit Header/time/duration, wide strings,
unions/inheritance and unreviewed IDL preprocessing/annotations/aliases are
refused. The older fixed-v1 profile retains its documented closed fixed IDL
subset. This finite profile does not claim arbitrary runtime-discovered types.

A Service call carries raw Request and returns raw Response. An incoming Service
event is ticketLE64 plus Request; a response is ticketLE64 plus Response. An
Action send carries raw Goal and returns a nonzero GoalIDLE64. Incoming Goal and
feedback records carry IDLE64 plus the corresponding component. A result carries
IDLE64, kind2..4, then Result. An Action response carries IDLE64, kind1 for feedback
or2..4 for terminal output, followed by the selected complete component. These
are Runtime envelopes, separate from native ROS1 Actionlib wrappers and ROS2 CDR.

## Official identity and native schema qualification

Native identity is copied from the actual official generated traits/type support.
ROS1 uses exact generated message/service MD5 conventions. In particular a
Service Request/Response trait may have the Service's MD5; the generator does not
invent a standalone component MD5. Modern ROS2 uses actual version1 RIHS
root/component hashes and full TypeDescription field/dependency metadata.
On an SDK whose typesupport struct predates native hashes, every actual payload
component must instead match its full official C++ introspection metadata. The
Gateway then recomputes RIHS01 with the target OpenSSL Crypto library from the
complete trusted canonical declaration. Service roots include event messages;
Action roots include both protocol services, their events, feedback envelopes,
UUID and Time dependencies. This is an official-schema projection, never a
claim that an old SDK contains a baked RIHS hash. Missing introspection, Crypto,
complete protocol traits, or any mismatching field fails admission. A malformed
hash on a hash-capable SDK never falls back to this path. Earlier generated
providers with no complete protocol trait can retain their modern native-hash
path; regenerate them before selecting a legacy SDK. Definition-file SHA,
canonical-codec SHA and native type identity are distinct values.

Lyrical's unbounded uint8 sequence may use the official `rosidl::Buffer` type.
The native provider recognizes that exact two-parameter container, retains all
canonical bounds, and checks its actual CPU storage before noexcept validation
reads elements. It never triggers an implicit GPU-to-CPU copy to validate an
unbounded buffer. Other scalar/sequence representations retain their exact
native compile-time checks. See the [identity regression fixtures](../../tests/ros2_compat/README.md)
for independent comparisons to complete official native exports; source
availability is separate from actual distribution or architecture acceptance.

The official fixture packages are under
[bounded-v2](../../tests/ros_custom_types/bounded-v2/README.md). They contain Point
and Envelope messages, Echo and empty-component Empty services, and Digest Action.
The native identity exporter creates no ROS Node/entity. The Gateway checks
root and every component identity, scalar/container type, native bound and full
nested definition/schema before creating an endpoint. Duplicate native names
across Topic/Service/Action and across fixed/bounded profiles are refused.

ROS1 custom subscriptions, incoming Service requests and Action wrappers install
an official callback-helper proxy that validates their complete finite serialized
field grammar before the official helper allocates typed dynamic fields. It then
uses official deserialization and a separate native-to-canonical conversion.
Native transport/RMW buffers exist before these helpers; their heap/network
allocation is outside canonical queue accounting. Deployment cgroup/network/RMW
budgets remain independent host constraints. There is no claim of a transitive
middleware heap or dynamic-library dependency graph attestation.

## Six-language client and server layers

The generated products contain C/C++/Rust/JavaScript/TinyGo/AssemblyScript complete
record codecs. C `bounded_interfaces.h` adds typed Service call/poll/respond and
Action send/feedback/result/poll/respond functions over explicitly acquired C
capabilities. Its caller-owned existing Action role bundle tracks matching live
Goal IDs, bounded slots, independent cancellation and checked reverse cleanup;
the builtin int32/Fibonacci payload helpers are not used for custom components.

C++ `bounded_interfaces.hpp` adds nonmoving typed Service/Action Client and Server
owners. Constructors preflight roles/ABI/aliases and roll back all acquired
owners after failure. Service and Action servers guard an entire poll/decode/
callback/respond pass with intrinsic RAII busy state. Nested poll and grouped
close/revoke return17 before imports; same-functor shared ownership makes callback
replacement safe without resetting mutable capture state. Respond operations are
allowed inside the correct handler. `poll_once()` is a caller-driven Executor
entry, with11 returned unchanged. The caller must retain every registered owner
through the entire executor pass; moving/destroying an active object is a lifetime
error. Destructors provide one best-effort cleanup, while explicit checked close/
revoke exposes errors. No thread, clock, permission or retry is created.

Rust and TinyGo examples compose existing Capability ownership with generated
complete typed components and private typed handlers. AssemblyScript does the
same through its selected NativeApi and checked direct WASI stdio. JavaScript
examples use AsyncSession over the actual retained compiled Wasm relay; copies
are transport results, not fabricated WebAssembly.Memory. Its server leases both
role owners across decoding, handlers and responses. These compositions cover
custom interfaces and do not reinterpret builtin Trigger/Fibonacci factories.

See the [complete runnable examples](bounded-examples/README.md) and the
[source A/B preparation and actual acceptance protocol](../../tests/ros_custom_types/bounded-v2/README.md).
Historical fixed-v1 generated directories and acceptance receipts remain intact.
The final providers physically contain both profiles in a newly frozen SourceB.
