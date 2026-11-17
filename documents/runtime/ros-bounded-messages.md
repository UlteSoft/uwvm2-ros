# Bounded message schemas

## Purpose and authority

`bounded_message.h` implements owned type descriptions, complete message
validation and conversion between the same structural schema in two wire
layouts. It addresses a common portability requirement seen in the acquired
ROS package inventory: nested records, fixed arrays, bounded sequences, strings
and scalar values. It does not grant an endpoint, select a device, load a type
support library or grant permission to execute a command.

The communication `type` configuration accepts the `bounded-message` codec.
Its type identity, encoding and complete descriptor are included in the existing
logical profile digest and independently pinned configuration file. The
compiled validator owns its immutable descriptor across asynchronous reply
lifetimes. Validator equality compares exact complete descriptions; it cannot
match a native codec merely because their type names agree. The existing
compiled native registry still refuses unsupported types. This change does not
enable arbitrary generic middleware endpoints or guest-directed library loading.

## Explicit restrictions

Every description supplies a maximum complete message size, maximum traversal
work and maximum nesting depth. Every sequence and string supplies a finite
bound. An upstream unbounded field needs an explicit deployment restriction;
there is no automatic unlimited policy. A zero sequence/string bound permits
only the empty value. Permissions remain zero until explicitly granted, and
optional traffic attenuation remains a separate parent-authority policy.

Primitive nodes cover boolean, signed/unsigned 8/16/32/64-bit integers and IEEE
binary32/binary64. Records retain ordered named fields. Arrays and sequences
refer to one element node. Child indices always precede their parent; the last
node is the root record. Cycles, unreachable nodes, duplicate names, forward
references and unknown flags are rejected. Resource limits cap descriptions at
4 MiB, graphs at 4,096 nodes and 65,536 edges, complete messages at 16 MiB,
nesting at 64 and worst-case traversal work at 64 Mi units. These are bounded
message resource ceilings, not a fixed registry of available ROS modules.

The compiler proves a worst-case traversal bound, including arrays of empty
records that would otherwise permit large amounts of work without consuming
input. Narrow and wide strings count their maximum byte/code-unit scanning work.
Primitive arrays use one alignment and extent check. Boolean and finite-only
floating arrays validate every element with bounded integer reductions that the
compiler can vectorize. Unaligned reads use `memcpy`; both byte orders are
supported without FP arithmetic. After complete preflight of immutable input,
conversion's private second pass copies these ranges without repeating their
value scan. The public API has no unchecked validation or conversion mode.

`finite` is available only for floating scalars. Without that restriction all
IEEE bit patterns are preserved, including NaNs used by sensor messages.
`unicode` validates UTF-8 for narrow strings and UTF-16 for wide strings.
`no-nul` rejects embedded zero code units. These restrictions must be selected
for the actual field semantics; the type layer does not infer physical units,
coordinate frames, timestamps, acceptable motion or source trust.

## Wire layouts

`ros1-message` is the packed little-endian ROS1 message body, with uint32 string
and sequence lengths. It excludes transport framing and connection headers.
Wide strings are unsupported in that layout.

`cdr` accepts plain/final XCDR1 with a four-byte encapsulation header and either
byte order. Alignment is relative to the body after that header. Strings count
the final NUL in the wire length; interior NUL is refused. Wide strings carry a
uint32 count of 16-bit code units without a terminal code unit. Empty records
carry the ROS-generated one-byte zero dummy field. Nonzero encapsulation options,
parameter-list CDR, XCDR2, optional/mutable members and unknown encodings are
refused. Padding bytes are ignored on input and zeroed when writing a conversion.

The layout follows the applicable ROS interface mapping and upstream Fast-CDR
serialization. See [ROS interface definitions](https://design.ros2.org/articles/legacy_interface_definition.html),
[ROS wide strings](https://design.ros2.org/articles/wide_strings.html) and
[Fast-CDR serialization](https://github.com/eProsima/Fast-CDR/blob/master/src/cpp/Cdr.cpp).
Native parity must be qualified against an exact retained implementation;
these links alone do not establish it.

Conversion validates the entire input, measures the exact output and checks
its byte budget before writing. Insufficient output capacity returns the exact
required size without modifying storage. Input and output must not overlap;
input must remain immutable for the call. Validation/conversion allocate no
memory, retain no guest pointer and do not execute floating-point operations.
Preparation uses recoverable fast_io allocation and leaves the prior state
intact on failure. The deployment definition budget includes the compiled
graph's logical retained storage; allocator metadata is outside that accounting.

Structural conversion is not a ROS1/ROS2 semantic bridge. In particular,
ROS1 `Header.seq`, signed time representations, type hashes and generated action
protocols require separately reviewed mappings. Safe command admission still
requires the common controller, physical bounds, source evidence, deadline,
authority generation and reserved stop path.

## Configuration and inspection

`tools/ros_scene/bounded_message_definition.py` compiles explicit JSON into the
canonical binary descriptor. A node has exactly `kind`, `bound`, `flags` and
`members`; a member has exactly `name` and `type`. The schema document has exactly
`layout`, `message_bytes`, `work`, `depth` and `nodes`. Collection members are
named `element`. Unknown/duplicate keys and implicit defaults are refused.

The existing communication definition compiler accepts a type definition with
`codec: "bounded-message"`, an exact `type` identity and the explicit `schema`
document. It uses the normal `canonical-schema` provider and existing type
limits. No peer CLI permission family or platform enablement switch is added.
The existing type-description operation returns the pinned descriptor and
encoding. Debugger inspection retains the existing capability/type identity
and never parses a descriptor to load code or confer authority.

## Qualification boundary

Core tests cover nested layouts, both byte orders, full-input consumption,
hostile lengths, cyclic/deep schemas, UTF-8/UTF-16, NaN preservation, exact
conversion extents, alias rejection, immutable ownership and recoverable
allocation failure. Qualification receipts separately identify sanitizer,
compiler, architecture and native-codec evidence. A core validator receipt does
not qualify a general native adapter, complete ROS package migration, hardware
actuation, industrial certification or whole-runtime native performance.

## Measured implementation cost

The `bounded-message-performance-baseline`, `bounded-message-performance-bulk`
and `bounded-message-performance-integer` receipts retain identical serialized
inputs, preallocated buffers, compiler-derived source snapshots and an exact
Fast-CDR 2.2.8 static archive. Both implementations use `-O3 -g`; collections use
the official `/opt/intel/oneapi/vtune/latest/bin64/vtune`. Execution stays on CPU 8
inside the existing shared 64 GiB, zero-swap cgroup. Batch order alternates across
31 samples in each of three rounds. Batch quantiles are not request latency
quantiles. No NUMA isolation, realtime scheduling or quiet-machine claim is made.

For 1,024 finite binary64 values in one sequence, median conversion decreased
from 14,113.64 ns to 199.13 ns. The same final cohort measured a Fast-CDR native
deserialize/serialize round trip at 161.02 ns without finite-value validation,
or 358.87 ns with that validation. Six values measured 44.63 ns in ROSv2 versus
51.14/55.02 ns in those native variants. A 1 MiB byte sequence measured
23,687.50 ns in ROSv2 versus 55,747.38 ns in raw native. The native paths
materialize an intermediate object buffer; ROSv2 performs complete wire
preflight followed by direct copying. Native's raw path accepts nonfinite values
and does not provide ROSv2's complete invalid-input output-atomicity contract.

Hardware Hotspots identified per-element traversal and byte assembly in
`schema::visit`. Microarchitecture analysis located those operations in sampled
assembly. The optimization validates contiguous numeric values using integer
masks/reductions, performs one bulk copy, and initializes only reachable
traversal frames. Integer exponent addition avoids synthesized 64-bit comparisons
on baseline SSE2. No FP arithmetic, ISA-specific inline assembly or new allocator
is required by the production scan. The instruction receipts independently
join sampled addresses through DWARF to exact frozen source hashes: 242 joined
instructions before optimization and 262 in the final cohort.

These measurements qualify a bounded CDR-processing subset. They do not measure
Wasm dispatch, DDS discovery, IPC scheduling, native device latency or full ROS
application throughput, and do not establish whole-runtime native parity.

## Acquired-source and VM evidence

The expanded acquired source inventory produced 15,958 distinct bounded descriptions and
31,916 concrete structural cases. Successful closures referenced 42,132 source
files, whose hashes are retained. The corpus uses explicit test-only sequence
and string limits; these are not production defaults. Ambiguous or missing
dependencies, unsupported IDL, legacy time aliases and encoding/parser failures
retain separate refusal reasons. Corpus coverage does not establish package
semantics or a successful migration of every package.

`tools/ros_ecosystem/idl_shapes.py` uses a pinned upstream `rosidl_parser` grammar
and hashed Lark wheel. It never follows include locators or executes source
expressions. It extracted 231 structures from 184 ROS IDL files; 870 files retain
explicit scope/feature refusals and 999 retain parser/encoding review results.
These counts include fixtures and non-ROS IDL in acquired repositories. Unknown
layout annotations, enums, forward declarations, namespace/owner mismatches,
unresolved aliases and unsupported scalar forms do not become accepted schemas.
Each input has a byte/time bound, and all parser execution remains in the shared
test cgroup. The resulting corpus is independently validated by the C++ core.

An independent retained ROS1 serializer and Fast-CDR 2.2.8 build check 516 nested
and UTF-16 round trips. Actual VM `type.describe` tests cover Wasm32/Wasm64,
memory indices zero/one, interpreter/AOT, absent grants, closed handles, cursor
conflicts, exact output sizing and overlapping spans. These inspection tests
expose no endpoint operation; process tracing verifies that they launch no
worker executable. The separate native worker remains limited to its explicitly
registered factories. [Bounded generated topic adapters](ros-bounded-native-topics.md)
add native metadata and type-hash verification; an identical type name alone
still cannot impersonate a compiled codec or install a native factory.
