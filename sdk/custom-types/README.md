# Trusted custom type profiles

The current [bounded-v2 profile](BOUNDED_V2.md) adds complete finite nested
strings/sequences and custom Service/Action components, native adapters, typed
client/server compositions and six-language runnable applications. Its new
[official source A/B workflow](../../tests/ros_custom_types/bounded-v2/README.md)
retains this fixed-v1 profile physically in the final composite registry.
Registration grants no rights; actual native acceptance is separately required.

The SDK also delivers an exact twelve-file
[bounded fixture codec/interface copy](generated-bounded-v2.README.md) for the
official Noetic/Jazzy fixture package. Its original generation receipt remains
unchanged. Applications using their own interfaces must generate their own
reviewed bounded profile; this fixture copy supplies no default ROS authority.

## Retained fixed-v1 topic profile

This directory prepares custom ROS topic types for the existing capability-based
ABI. It does not enable a topic, open a capability, generate a native ROS type,
or establish a successful native ROS test. The default runtime registry remains
empty. Every publication and subscription still requires an explicit operation
alias, resource, ROS version, native type identity, codec identity and all the
normal byte, rate, queue, transport/QoS and audit limits.

The first profile is `uwvm-ros-custom-fixed-v1`. It supports `bool`, explicitly
signed or unsigned 8/16/32/64-bit integers, finite IEEE-754 `float32` and
`float64`, fixed arrays and explicitly supplied nested fixed records. Fields are
encoded in declaration order without C/C++ padding. Integers use little endian;
signed integers preserve their two's-complement bits; booleans are exactly zero
or one; floats retain their finite IEEE-754 bits. Fixed arrays carry no length
prefix. No native TCPROS serialization or ROS2 CDR is forwarded to a guest.

Each record is limited to 4,096 expanded scalar cells, 32,768 canonical bytes and
16 nested records. A catalog contains at most 128 descriptors and 16,384 expanded
cells in total, bounding source-generation work as well as runtime validation.
Package names are restricted to lowercase identifiers of at most 63 characters;
the complete native type name is at most 255 characters. Record names are upper
camel case alphanumeric names. Member names follow the ROS lowercase underscore
convention and must not collide with reserved names in any generated language.

In the retained fixed-v1 profile, strings, sequences, variable arrays, recursion, ambiguous `byte`/`char`/`octet` aliases,
implicit `Header`, implicit time/duration types, constants, default values,
unions, inheritance, typedefs, IDL preprocessing/includes, annotations, wide
characters and unsupported syntax are refused with a specific diagnostic.
Dependencies must appear in the catalog explicitly. Native Service and Action
registration is unsupported in this retained fixed-v1 profile: `.srv` and `.action` inputs are
refused rather than presented as working adapters. Their existing builtin
adapters continue to use their established framing. Existing builtin adapter
packages cannot be shadowed by this generator.

## Inspect a schema without creating a registry

Run the pure source check from the repository root:

```sh
python3 sdk/tools/custom_types.py sdk/custom-types/examples/catalog.json --check
```

The example catalog deliberately contains no fabricated native identities.
`--check` reports `SCHEMA_PREPARATION_ONLY`, resolves fixed layouts and computes
canonical schema identities. It writes no output, creates no native entity and
provides no native or WebAssembly execution evidence. The Packet example has 23
canonical bytes; the IDL FixedSample example has 45 bytes.

The current checked-in `generated-r3/` example files are the six guest source codecs only.
They were produced without native identities using `--guest-only`. Their
`generation.json` explicitly records `GENERATED_GUEST_SOURCE_ONLY` and
`native_registry_emitted=false`; there is no native registry or provider header
in that directory. Earlier `generated/` and `generated-r2/` files remain immutable
as previous source-generation revisions. Reproduce current files into a fresh directory with:

```sh
python3 sdk/tools/custom_types.py sdk/custom-types/examples/catalog.json \
  --guest-only --out /absolute/fresh/guest-codecs
```

The input JSON contains exactly `format` and `types`. Each type entry has
`ros_version`, `type`, `file` and, for registry generation, `native_identity`:

```json
{
  "format": "uwvm-ros-custom-fixed-v1",
  "types": [
    {
      "ros_version": 1,
      "type": "example_interfaces_custom/Packet",
      "file": "Packet.msg"
    }
  ]
}
```

Definition paths are relative to the catalog directory and cannot escape it,
including through symlinks. Inputs must be stable bounded regular UTF-8 files.
Duplicate JSON keys, duplicate types and duplicate fields are refused. A `.msg`
record contains only `type member` or `type[N] member` declarations and comments.
The ROS2 `.idl` subset contains `package::msg` modules and fixed struct members;
the example shows its supported spellings. Array bounds are positive integer
literals. Native ROS1/ROS2 definitions must independently pass their official
generation tools; this bounded parser is not a replacement for those tools.

## Obtain native identities and generate source

First generate and build the exact native interface package with official ROS1
`message_generation`/`genmsg` or ROS2 `rosidl_default_generators`. Preserve its
definition inputs and dependency closure. Export the ROS1 message MD5 or ROS2
version-1 type hash from the resulting official traits/type support. In this
project, official native compilation, identity probes and native ROS acceptance belong
inside the verified Linux 64 GiB cgroup with zero swap. Guest-only SDK examples
can also use the [serial local `/tmp` compiler](../docs/local-testing.md) after
Linux SSH was closed. That compiler includes the five handwritten C/C++/Rust/
TinyGo/AssemblyScript ROS1 entries and the JavaScript pure roundtrip. It supplies
the documented local Rust/Go package layout with the bundled generated-r3
codecs; no native identity is required merely to compile those guest sources.
No successful compilation or native
delivery is implied by the commands or artifacts described here.

Add the exported identity to the catalog as `native_identity`: exactly 32
lowercase hexadecimal characters for ROS1 or `RIHS01_` followed by 64 lowercase
hexadecimal characters for ROS2. Do not substitute the definition file SHA-256,
the canonical schema hash, a guessed value, or a hash derived with a new local
algorithm. The generator checks the identity's format; the Gateway checks it
against the official concrete native message at runtime before creating a
custom topic entity. Native definition/schema correspondence remains part of
the trusted build and native acceptance procedure.

Then run, using a new directory each time:

```sh
python3 sdk/tools/custom_types.py path/to/catalog-with-official-identities.json \
  --out /absolute/fresh/qualified-generation
```

The generator refuses an existing output directory. Keep earlier output and
reports immutable; choose a revision directory or a separate build directory
for another generation. It emits:

| File | Contract |
| --- | --- |
| `manifest.hpp` | C++17 immutable common descriptor provider, without ROS includes |
| `native_ros1.hpp`, `native_ros2.hpp` | Official native includes, fixed converters and explicit typed X-macro rows |
| `fixed_records.h` | Standalone C11 caller-buffer records and transactional encode/decode |
| `fixed_records.hpp` | C++20 `roswasmcpp::Codec<Record>` specializations using existing SDK publishers/subscriptions |
| `fixed_records.rs` | Rust records, caller-buffer encoding and `roswasm::typed::Canonical` implementations |
| `fixed_records.mjs` | JavaScript encode/decode objects accepted by existing Node factories |
| `fixed_records.ts` | AssemblyScript records and existing `Codec<T>` implementations |
| `fixed_records.go` | TinyGo/Go records implementing the existing SDK `Message` interface and checked decoders |
| `generation.json` | Source-only status, source hashes, package lists, sizes, identities and output hashes |

Generated codecs perform no I/O. C/native scalar copies use compiler memory-copy
intrinsics with a portable freestanding fallback. Outputs are bounded and
floats are validated before output allocation or writes. Native C++ applications
that add logging or file access should use fast_io; no logging or file access is
hidden inside these codecs.

## Bind the compiled registry and native providers

The generated common header defines
`uwvm2::imported::ros::ipc::custom_topic_entries()` after the shared descriptor
types. Supply its absolute path through `UWVM_ROS_CUSTOM_TOPIC_MANIFEST`; the
Gateway CMake option `UWVM_ROS_CUSTOM_REGISTRY_HEADER` maps to that macro. Supply
the corresponding absolute native header through `UWVM_ROS_CUSTOM_ROS1_HEADER`
or `UWVM_ROS_CUSTOM_ROS2_HEADER` and list the exact native dependency packages in
the Gateway's custom package option. Follow the native fixture guides for the
current CMake option names and Linux build invocation:
[ROS1 fixtures](../../tests/ros_custom_types/ros1/README.md) and
[ROS2 fixtures](../../tests/ros_custom_types/ros2/README.md).

Both Worker and Gateway must compile the same immutable descriptor table. Each
descriptor contains ROS version, exact native topic type, official native
identity, canonical codec identity, fixed byte count and a run-length encoded
scalar layout. Both processes validate the table and reject malformed tables,
duplicate types, builtin shadowing and identity/layout inconsistencies. An
empty table grants no custom type support.

The native headers define `UWVM_ROS_CUSTOM_ROS1_TYPES(M)` and
`UWVM_ROS_CUSTOM_ROS2_TYPES(M)`. Each row is
`M(NativeType, name, native_identity, codec_identity, wire_bytes, Codec)`.
`Codec::valid(Native const&) noexcept` performs allocation-free full value checks;
`encode(Native const&, uint8_t*, size_t)` checks the entire native value before
writing; `decode(uint8_t const*, size_t, Native&)` validates exact canonical
length, all boolean/float values and commits a complete temporary native value.
The generated optional vector encode overload preserves its destination on
failure. A ROS1 native boolean byte greater than one is refused, never normalized
to true.

The generated native header also asserts every declared native scalar type,
signed width, fixed array length and fixed array element type at compile time.
ROS1 booleans require the official byte representation and ROS2 booleans require
`bool`. These checks prevent widening/narrowing, partial arrays and a dynamic
native container from silently becoming a fixed canonical field. They do not
prove that a supplied schema covers every native field.

Full schema admission therefore remains mandatory before creating an entity.
`Codec::definitions[]` and `definition_count` expose normalized ROS1 root and
transitive dependency declarations for comparison with the official expanded
`ros::message_traits::Definition<Native>` text. `Codec::native_schema()` exposes
the complete root/dependency closure, ordered member names/types and exact array
shapes for ROS2 official TypeDescription comparison. Native metadata with any
missing/extra field, unsupported default, dependency, differing field order,
scalar type or array shape is refused. ROS1 expanded definitions also expose
constants, which this fixed profile refuses. ROS2 TypeDescription field matching
does not attest the absence of native constants; unsupported constants in the
catalog itself are rejected by the parser. A genuine native MD5/RIHS identity paired with
a different catalog schema is insufficient to enable a custom adapter.

Older ROS2 SDKs are qualified through complete C++ introspection metadata and an
independent canonical RIHS01 recomputation, as documented in the
[bounded profile](BOUNDED_V2.md#official-identity-and-native-schema-qualification).
Service/Action roots require their complete official protocol/event closure;
partial payload-only descriptions fail. Missing legacy metadata or Crypto
refuses that adapter, and invalid modern hashes never enable a legacy fallback.
No fallback changes the canonical codec fingerprint or grants authority.

## Configure one custom operation explicitly

Read `adapter_codec_identity` from `generation.json`. It has the form
`UWVMROS1_` plus a SHA-256 of the normalized canonical IR. The IR includes the
profile, ROS version, exact type, declared members and nested schemas, scalar
signs/widths and mandatory boolean/finite-float rules. Comments, whitespace,
native identity strings and absolute filesystem paths do not alter this hash;
wire-relevant schema changes do.

For every custom topic grant, set `adapter-codec-identity` to that exact value in
addition to the official native `type-identity`, resource and all existing
required topic limits. Set `validator=none`: the generated structural validator
still runs unconditionally. This field belongs only to registered custom topic
operations; it does not relax builtin validators. The binding manifest carries
it to the Gateway, which independently compares it with its compiled registry.
An independently built Worker or Gateway with a mismatched codec refuses the
operation. Generated metadata and SDK codecs carry no capability authority.

Publication consumes precisely `wire_bytes` data bytes. Subscription polling
returns that exact record payload; it does not prepend a new topic envelope.
Resource names and types come from the host alias, never from guest bytes.
Choose buffer capacities explicitly and pass only successful initialized
prefixes to imports. Encoding failures must not be retried as empty messages.
Decode failures must not replace an application's prior accepted value.

## Use generated records in six guest languages

The source examples in `examples/use_generated.*` demonstrate Packet using the
checked-in guest-only sources in `sdk/custom-types/generated-r3`. Native delivery
additionally requires a separate generation using actual official identities
for the example package. The guest examples are preparation sources, not compiled
Wasm artifacts. Adapt only build/module paths when using another fresh output
directory; do not alter generated field order, wire bytes or codec identity.

C uses a separate fixed output buffer and passes successful bytes to an existing
capability. C++ uses the existing RAII publisher template and generated Codec
specialization. Rust needs the SDK `alloc` feature for `Canonical`; its
`encode_into` method supplies a caller-buffer alternative. Rust arrays use
`core::array::from_fn` initialization and require a Rust toolchain supporting it.
JavaScript integers of width 64 remain BigInt, with no precision-losing Number
conversion. AssemblyScript has managed record values but imports still receive
linear-memory byte offsets; call `Codec.valid` before directly exposing decoded
values. Go fixed arrays retain their native lengths and generated `Encode`
implements `roswasm.Message`; polling can use the existing byte API followed by
the generated checked decoder. No language inherits an endpoint or operation.

## Source and model validation scope

```sh
python3 sdk/custom-types/tests/source_models.py \
  --report /tmp/uwvm-custom-fixed-source-models-fresh.json
```

The pure models cover parser refusals, explicit dependency resolution, recursive
and cached-depth rejection, scalar/byte ceilings, stable schema identities,
source pins, fresh output preservation, descriptor/provider/native header shapes
and generated JavaScript round trips and malformed vectors. Native identity
strings inside tests are explicitly synthetic. JavaScript comparisons use an
independent little-endian golden vector. No native compiler, Wasm engine, ROS
node or middleware executes in these models.

Actual acceptance must separately compile all six guest outputs, compile both
Gateway providers against official native interfaces, verify current compiled
identities, and exercise native publication/subscription plus refusal side-effect
absence under the verified Linux cgroup. Test wrong native/codec identities,
cross-version aliases, truncated/trailing records, boolean two, nonfinite f32/f64,
close/revoke and QoS/transport admission. Source/model passes cannot mark those
requirements complete. Broader custom dynamic, Service and Action support
remains explicitly unsupported.

The supported source subset is informed by the official
[ROS2 interface definitions](https://design.ros2.org/articles/legacy_interface_definition.html)
and [ROS2 IDL mapping](https://design.ros2.org/articles/idl_interface_definition.html),
with native ROS1 identity generation delegated to
[genmsg](https://docs.ros.org/en/hydro/api/genmsg/html/python_api.html).

## Complete vendor catalogs and bridges

The actual generated complete-value vendor and NVIDIA Source directories are
listed in [the complete vendor SDK guide](COMPLETE_VENDOR_SDK.md). Read its
raw-import to typed-operation sequence, coherent build-domain rule, explicit
permissions and Native qualification requirements before selecting a bundle.
These Source outputs do not confer Native identities or robot acceptance.
