# Optional complete MoonBit JointState

The original authoring checkpoint was **SOURCE_ONLY_UNEXECUTED**. The subsequent
[actual Linux qualification](../../../../../test/0050.high_level_languages/results/native-budget/language-moonbit-records-jointstate-20261019/README.md)
now records nine emitter-policy cases, fourteen real Wasm codec test executions,
sixteen application builds/layout checks and sixteen default-disabled UWVM
outcomes. The four numeric test receiver spellings were fixed after preserving
the first failed build. No native JointState publisher or positive delivery was
run. The source-plan descriptions below retain their original staging scope;
actual command/source/product identities are in the linked checkpoint.

The same complete ROS1/ROS2 codec bodies have now also executed inside actual
UWVM interpreter products, as part of the separate [three-repository codec
matrix](../../../../../test/0050.high_level_languages/results/native-budget/language-moonbit-codec-uwvm-20261019/README.md).
This removes the earlier codec execution's dependency on the Node test host.
The [standalone recipe](../../../../../tests/ros_custom_types/moonbit-records/README.md)
retains every assertion and malformed input and checks deliberate golden-byte
failure. These applications perform codec computation without any ROS import;
positive native JointState middleware delivery is still a separate requirement.

The immutable three-language managed r1 is retained separately. Its statement
that the unchanged managed emitter registry has no MoonBit entry remains true:
this revision adds a separate explicit producer, not a replacement registry.

## Actual typed interface

`sdk/custom-types/moonbit_profile.py` consumes the actual resolved bounded IR
from `bounded_profile.py` and emits every field of every admitted record. Its
only target is explicit `--language moonbit`; original six emitters,
`managed_profile.py` and `managed_csharp.py` are untouched. It does not generate
native support, identities, permissions, RPC factories or successful providers.
`generation.moonbit.json` records source-only output identity; it cannot qualify
native transport. The native-identity option only requires the existing syntax.

For each actual record `R`, output contains a complete mutable typed `R`,
`R::new()` with actual admitted native scalar defaults, and `RCodec::new()`.
The codec exposes the actual IR's ROS version, native type/identity, canonical
codec identity and minimum/maximum wire bytes. `encode` returns
`Result[Bytes, Int]`; `decode` returns `Result[R, Int]`. The selected lessons use
`Ros1Sensor_msgsJointStateCodec` or `Ros2Sensor_msgsMsgJointStateCodec`.
There is no invented generic Codec trait: the existing SDK provides a real
`Node.subscription` resource and `Executor.add(Resource, (Bytes)->Int)`.
The callback invokes this generated complete decoder directly.

All eleven authoritative scalar kinds are represented. UInt64 retains its full
64-bit pattern; Int64 and Int32 use actual pinned reinterpretation APIs.
Int-backed 8/16-bit fields reject out-of-range encoding instead of narrowing.
Float32 and Float64 preserve finite bits, including negative zero, and refuse
all infinity/NaN exponent patterns. UTF-16 input is checked by the real SDK
bounded UTF-8 encoder; strict core decoding preserves BOM and rejects malformed
UTF-8. This complete-record profile rejects embedded NUL. String limits count
UTF-8 bytes, not managed character count. A bounded writer checks every output
byte; the private reader validates counts against both IR bounds and remaining
bytes. Decode owns a private input copy/cursor and returns a value only after
all nested fields, semantic rules and exact end-of-input pass. No partial
record or mutated input escapes a returned decode error. Allocation failure or
trap recovery is not claimed.

Fixed arrays, bounded sequences, nested records and complete empty Service or
Action components are emitted. Empty records use a typed singleton and zero
wire bytes, not an invented field. Each nested/fixed-array default is created
independently. Actual native finite scalar default bits are retained, including
Quaternion's declared w=1. ROS1 Header includes unsigned seq and primitive
unsigned sec/nsec; ROS2 Header has signed sec and unsigned nanosec, with no seq.
Normalized time/duration nanoseconds must be below one billion. JointState's
nonempty optional arrays must agree in length; empty velocity is valid.

For these preparation/JointState profiles, a schema with an unrepresentable
original MoonBit field name, reserved word,
unknown semantic/scalar, unsupported declared default, or any original bounded
IR refusal is rejected **as a whole**, before output creation. No rename,
projection, field omission or guessed future grammar is allowed. In particular,
the original managed-records catalog contains `sealed` and is deliberately
refused by this conservative policy. The new test fixture selects nine complete
existing interfaces and copies their definitions byte-for-byte; it does not
claim acceptance of the original larger catalog. Lexical policy was checked
against the [official lexical reference](https://docs.moonbitlang.com/en/latest/language/lexical-conventions.html)
and [method reference](https://docs.moonbitlang.com/en/stable/language/methods.html).
Those current documents supplement, rather than replace, the saved exact
compiler/core API source pins; they are not an executed compiler result.
The separate original-node-control profile has explicit source spellings for
three original native `type` members. Those narrowly selected control bindings
are outside this phase's compiled catalog set and are not covered by its
reserved-name refusal result.

## Actual Guest and ownership

`moonbit-recipe.json` retains the original producer plan. The linked r97/r98
records supply the actual selected builds and default-disabled runs. Build a fresh
project for each ROS backend, mode, target and profile. Stage exactly one
observer as `guest/observe.mbt`, one mode as `guest/mode.mbt`, and the matching
package manifest. Do not build the source-variant authoring directory directly.
Copy the actual SDK into the declared local workspace, both generated record
files into `records`, and the original SDK ASCII stdout helper plus the new
stdin helper into `stdio`. Preserve exact source/output/toolchain receipts.

Mode0 performs the actual SDK ABI/arena check, opens only `joint-state` with
TopicSubscribe, and accepts only real errno13 with no acquired owner. Mode1
uses that single subscription alias and the actual SDK Executor. Each complete
stdin byte `R` admits one spin, at most64 credits; a native idle poll and an
application callback failure remain distinct. The decoder checks the complete
shared observer fixture: ROS1 seq19/sec42/nsec300000000 or ROS2 sec-2/nanosec300000000,
frame_id base, names left/right, position1.25/-2.5, empty velocity, and effort
0.5/-0.25. The application never publishes a subset as a successful observation.

The actual Session is retained before opening an alias. Executor.close and
Session.shutdown occur before either success marker. Primary and cleanup
codes remain distinct; every close outcome must be completed with errno0.
Unexpectedly acquired default-mode owners are still handled by the actual
Session ledger and cause failure. The retained Session retains its actual
cleanup history/retirement diagnostics. This source does not turn OOM, fatal
trap or arbitrary asynchronous termination into a recoverable close guarantee.

## Linear-memory and qualification boundaries

Both `wasm` and `wasm-gc` use the SDK's real exported ordinary memory0, numeric
Wasm ABI and copying linear arena. There is no cast from a managed GC object to
a linear pointer. ROS input/output/slot bounds and heap8519680 are unchanged.
The new one-byte fd_read reserves descriptor/result320..332 and byte352..353;
this reservation is **additional** to the original stdout0..289 and ROS65536+
reservation. The actual r98 gate checks all sixteen produced applications for
segments, heap/static layout, limits, entry, types and imports, including this
added scratch. Repeat that check for every future rebuild. Each target keeps
min256/max1024 pages; GC JS builtin strings remain disabled.

The recipe permits only selected reachable ROS backend ABI imports and genuine
WASIp1 fd_read/fd_write. It adds no clock, environment, preopen, random, native,
management or debugging authority. Unknown imports fail instead of receiving
success stubs. Native generation/type identity/source/vendor/platform/QoS and
real delivery/cleanup need independent evidence for the same instance.
`adapter_plan.plan(..., moonbit=selection)` binds exact generation, complete
outputs, foundation/SDK/lesson sources and explicit real producer/toolchain/core
inputs; it reads source data and never runs the producer or authorizes effects.
When `moonbit` is omitted, the previous six plus explicit three-managed path
is unchanged. A source record or success marker is not native qualification.

Seven typed codec test methods and nine emitter/refusal roles were originally
authored as SOURCE. They now have the separate actual results linked above:
seven methods per backend, fourteen executions, plus nine source-policy cases.
They include the independent full-width scalar
wire vector, finite/UTF8/count/trailing failures, empty component, independent
nested defaults and both complete Header layouts. Stage matching test sources
only with their matching actual generated catalogs. All future generation,
compilation, Guest and native tests belong in the original SSH Linux64GiB,
zero-swap cgroup under the same process/resource ownership. Threaded Runtime support,
universal schema/vendor/platform compatibility and unchanged measured
performance are not asserted here.
