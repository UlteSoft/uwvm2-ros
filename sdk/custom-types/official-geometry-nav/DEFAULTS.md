# Complete ROS2 Quaternion defaults

This source implementation preserves the original selected ROS2 Quaternion
variant: four float64 fields in order x/y/z/w, declared0/0/0/1. It supports the
complete Message dependency family, including Pose, Transform, covariance36,
Odometry and Path. The first implementation emitted C, C++, Rust, JavaScript, AssemblyScript and
Go/TinyGo records. The maintained successor also supports optional real
Python/Java/C# record emission through [the shared managed record layer](../MANAGED_RECORDS.md).
MoonBit remains a separate language integration requirement; there is no
placeholder emitter or completed-language claim. General ROS2 bool, integer
and finite float scalar defaults, including complete default-bearing Service/
Action graphs, are described in [the scalar-default successor](../scalar-defaults/README.md).
The exact official Quaternion variant and its original identity domain remain
unchanged. This document's first-family provenance and historical execution
statements remain tied to that original cohort.

The author has not invoked the parser/generator, imported a candidate, compiled
any fixture, or run native/Guest/Gateway tests. Source review and code generation
are distinct from actual execution and all26-distro/native/hardware acceptance.
Default authority stays empty; no new import, grant, registration or endpoint
operation is introduced by a value initializer.

## Official representation and identity

The pinned original source lives in upstream/ros2/geometry_msgs/msg/Quaternion.msg
and is copied byte-for-byte into the operational catalog. ROS1's different
no-default declaration remains unchanged. ROS2 rosidl_adapter parses float64
literals as floating values, emits an IDL default annotation, and the official
TypeDescription generator serializes that annotation with str(value). The actual
normalized default text is therefore `0.0`, `0.0`, `0.0`, `1.0`. Both source spellings
0/1 and0.0/1.0 normalize to the same finite contract. Different values, missing or
partial defaults and changed field types/order are refused for this exact
official Quaternion. Other explicitly catalogued types use the scalar-default
successor; standard reserved namespaces still require their exact admitted
official shapes and variants. Arbitrary expressions, constants, declared
string/sequence defaults and nonfinite values remain refused.

Official RIHS calculation removes default_value before hashing. Consequently a
correct baked RIHS alone does not prove defaults. The canonical codec identity
adds a native_default key only for an explicitly declared default; a nested
Message includes the resulting child identity. All no-default canonical IR
keys and domains remain unchanged. Full payload encoders/decoders retain the
same complete field bytes: defaults are initialization semantics, not an
alternative encoding or a rule that Quaternion w must always equal1.

The actual introspection C++ template currently emits nullptr for every default
pointer, including fields with declared defaults. That is UNKNOWN, not evidence
of absence. The legacy matcher explicitly refuses any expected nonempty default
before deriving identity. Modern Gateway admission must obtain the actual baked
hash AND complete TypeDescription, compare every field and reference/default,
and admit only the exact selected variant. This originally covered Quaternion;
the successor compares the complete modern default-bearing protocol graph as well. No null pointer, generated zero
value, payload sample, synthetic metadata or historical SDK can substitute.

## Administrative bind and plan

The existing modern native exporter already preserves actual default_value
strings. It is not changed or replaced with a fake exporter. The direct binder
and the injected vendor planner share a literal-identical pure DATA validator:
complete original Message definitions, exact native field IDs/capacities/order,
whole root/reference graphs, normalized default text and actual exporter
root/component association must agree. The planner reopens original and retained
native JSON and compares its rederived proof to the binder receipt. This helper
does not derive a RIHS or qualify a native process. Its description SHA256 is a
DATA association digest, explicitly distinct from the actual native identity.

No-default binding receipts remain unchanged. For a default-bearing catalog the
new native_default_contract receipt is mandatory. It refuses derived legacy
metadata, default-erased descriptions, unknown fields/defaults and incomplete
references. The first default catalog was Message-only. The maintained
scalar-default successor supports complete Service/Action source and modern
protocol proof without inventing identities; native endpoint factories retain
their own independently reviewed admission and transport requirements.
The separately reviewed legacy Message/Service/Action exporters must reject a
non-None Field.native_default before emitting old empty-default metadata. Their
metadata_export.h helper remains unchanged by this cohort.

Use the real upstream-lock, matching installed modern SDK and original exporter
owner through the existing vendor prepare/export/bind flow. The geometry
catalog remains an unbound source template without fabricated native hashes.
The current examples' generator and direct binder usage in README.md applies;
select fresh owned output and retain all original source, raw metadata/full7,
whole SDK/source inventories and terminal preservation evidence.

## Value initialization APIs

| Output | Declared initialization | Ordinary zero storage |
| --- | --- | --- |
| C | `roswasmc_bounded_2_geometry_msgs_msg_Quaternion_default(&value)` | `{0}` stays w0 |
| C++ | `roswasmcpp::Codec<uwvm_custom::Ros2Geometry_msgsMsgQuaternion>::default_value()` | actual C-value alias `T{}` stays w0 |
| Rust | `Ros2Geometry_msgsMsgQuaternion::default()` | decoding still consumes all32 bytes |
| JavaScript | `Ros2Geometry_msgsMsgQuaternionCodec.defaultValue()` | manually supplied values stay explicit |
| AssemblyScript | `new Ros2Geometry_msgsMsgQuaternion()` | valid(bytes) must precede decode(bytes) |
| Go/TinyGo | `DefaultRos2Geometry_msgsMsgQuaternion()` | the Go zero value stays w0 |

The initializer propagates through nested fields and independently constructed
fixed-array elements. Empty dynamic sequences remain empty; callers initialize
each appended element explicitly. There is no unused-capacity default claim.
C's `_default` rejects nullptr, initializes a bounded local value and commits
once; it performs no allocation/I/O. Go adds an explicit function rather than
relabeling its language-defined zero value. Rust/JS/AS already expose declared
value initialization and now preserve the actual scalar literal.

All generated decoders remain strict: truncated input is refused, trailing bytes
are refused, and a valid complete all-zero Quaternion payload decodes to w0.
C/C++ transactional destinations remain unchanged on failure. AS callers must
use valid(bytes) before decode because that existing API returns a new default
value for malformed input; the default object is never an acceptance signal.

## Real validation sources and remaining runs

`tests/ros_custom_types/quaternion-defaults/test_defaults.py` supplies15 injected
source-policy methods plus a separate6-case native_suite requiring an admitted
REAL complete original native export as its positive input. It never invents a
passing native identity. Mutated copies only exercise negative DATA rejection.
The existing geometry family policy remains14 methods with complete30/27 ROS2
expectations. Native SDK/Gateway admission and native default constructors are
covered by native/native_defaults_contract.cc using real generated headers,
real type support, actual native descriptions and the real Gateway comparison.
The native target is modern-only and deliberately refuses legacy SDKs.

The Guest-only Bundle fixture exercises two independent fixed Quaternion values,
a nested Pose and an empty/explicitly appended Quaternion sequence in all six
current record outputs. Its source-only wrapper is not represented as an
installed native Message or assigned a fake identity. See the fixture README
for concrete serial compiler/runner shapes. A future original owner must record
the actual commands, full compiler dependencies, link trace, complete Wasm
validation, process/cgroup ownership and terminal normal Frame.finish. A source
policy pass cannot stand in for those actual builds/runs, native publication,
ROS network delivery, vendor official suites or performance counters.
