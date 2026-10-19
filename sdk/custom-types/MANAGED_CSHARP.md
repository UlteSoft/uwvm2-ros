# Optional typed C# canonical record codecs

`managed_profile.py --language csharp` adds one optional source emitter to the
existing bounded IR. It reads the same catalog once through the actual
`bounded_profile.load_catalog`, retains every record's existing canonical
identity, bounds, dependency order, complete-value semantics and declared
native defaults, and emits `csharp/BoundedRecords.cs`. It does not invoke or edit
the original C/C++/Rust/JavaScript/AssemblyScript/TinyGo emitters. Selecting only
Python/Java keeps both original source products byte-identical. The extensible
language registry is an explicit trusted-source list, not a total-language or
all-vendor compatibility claim and not a catalog-selected plugin path.

No generator, C# compiler, .NET process, Wasm module, ROS process or native type
exporter was run while authoring this candidate. The generated-source policies
and C# contracts below are authored execution inputs, not passing results.
Root's earlier C# SDK/.NET/Wasm bridge and Python/Java results are independent
historical evidence; they do not qualify this new optional emitter.

Subsequent validation on 2026-10-18 executed all twelve source policies and
generated eight actual catalogs. The real .NET 8.0.408 compiler/runtime then
passed all eight separately compiled roles: fifteen record cases/57 checks,
one case preserving all eight native keyword/metadata field names, and six
complete ROS1/ROS2 JointState, RobotMsg and CIOState wire roles. The Linux run
used serial commands inside the existing 64 GiB cgroup; its selected source
and read-only package-cache hashes remained unchanged. These are host codec
results. Wasm/native ROS interoperability still requires its separate route.
The [dated checkpoint](../../documents/ros/sdk-compatibility-checkpoint-20261018.md)
links the preserved actual receipts and their exact qualification boundaries.

## Actual source interface

The fixed adjacent `managed_csharp.py` exports
`emit_csharp(parent, bounded, records, ordered, interfaces)`. Only the registry
loads it, after pinning its source and the actual SDK `Codecs.cs` and `Result.cs`
(the latter declares `ICodec<T>`). The catalog cannot choose a module path. An
already occupied private module name is refused; a successfully loaded module
is removed from that private slot after emission. No arbitrary import hook,
ambient cache or managed/native ROS library discovery is added.

Generate into a fresh owned output outside the selected SDK source tree:

```sh
python3 sdk/custom-types/managed_profile.py /absolute/catalog.preparation.json \
  --language csharp --out /absolute/fresh-csharp-source-output
```

The operator must select the actual admitted Python and resource limits; the
command above is a documented future invocation. The existing
`--require-native-identity` requires the existing syntax and refuses preparation
catalogs without identities. It never discovers or verifies an installed native
identity. `generation.managed.json` is written last and retains full schema and
source/output hashes, `native_qualification=NOT_PERFORMED`, no native registry,
no implicit permissions and no RPC facade. C# selection adds only an explicit
`csharp_source_types` mapping. Native proof/registration still belongs to the
unchanged native exporter/binder/Gateway flow.

## Public types and exact native fields

All generated types are nested in `Uwvm.RosWasm.Generated.BoundedRecords`.
For an actual IR symbol `Ros2Xarm_msgsMsgRobotMsg`, the public value class is
`_Ros2Xarm_msgsMsgRobotMsg`; its independent codec class is
`Ros2Xarm_msgsMsgRobotMsgCodec`, with `Instance : ICodec<value-type>`. The proof
records this deterministic mapping for every actual resolved record.

The leading underscore cannot match an admitted native field, whose actual
ROS field grammar begins with a letter. Every field uses a verbatim C# identifier
`@<actual-native-name>`. Metadata and encode/decode methods live on the separate
codec class. Native names are never silently renamed, and C# selection adds no
keyword, generated-method or metadata-name refusal. Existing Python/Java name
refusals remain unchanged when those languages are also selected. Fields hiding
an `object` member explicitly use `new`. The separate names catalog demonstrates
C# keywords, the original IR symbol and `CODEC_IDENTITY` as actual byte fields.

`bool`, byte/sbyte, ushort/short, uint/int, ulong/long, float/double and string
are strongly typed. Every u64 bit, including bit63 and UINT64_MAX, is preserved.
Signed narrow values mask to their actual width before widening to the real
`Writer.Unsigned(ulong,width)`. Float32 uses actual `BitConverter` bit conversion
and the real four-byte unsigned reader/writer; it does not widen its wire to
Float64 or assume a new SDK API. All float values must be finite; signed zeros
retain their exact bits. Record fields are real typed nested value objects;
fixed arrays and bounded sequences are typed arrays without reflection.

Scalar native defaults are exactly those already admitted by the authoritative
IR. Constructors recursively initialize nested records and each fixed record
array element separately, including Quaternion w=1. Fixed strings start empty;
sequences start empty. A declared default never replaces missing wire bytes.
No unsupported defaults, scalar types, semantic rules or interface component
roles are silently admitted.

## Complete values and bounded failure

Encoding captures each typed field and array reference, checks the exact fixed
count or sequence bound, every nested value, strict UTF8 byte bound/no NUL,
integer width and finite float, then enforces normalized Time/Duration subsecond
ranges and JointState optional equal-length arrays on the actual captured
values. The real Writer enforces the resolved total maximum. The final complete
extent is checked before the only returned wire array. Encoding does not decode
its own result or allocate a second nested object tree. Callers must not mutate
shared value objects or array contents concurrently with encoding.

Decoding first checks the complete input extent and clones caller-owned bytes,
then parses every field using the actual SDK Reader. Sequence counts are checked
against both the IR bound and remaining bytes before allocation. Nested framing,
Bool0/1, UTF8, finite floats, normalized time, JointState array lengths and exact
end consumption remain mandatory. Returned strings/arrays/records do not retain
borrowed slices into caller input. All allocations are bounded by the original
IR node/depth/wire limits; this layer does not change those limits.

Message, Service request/response and Action goal/result/feedback records all use
the same complete-value emitter. Generating component codecs creates no Service
or Action transport, callback, future, protocol root, native identity, grant or
cancellation authority. Registration and aliases remain explicit trusted inputs.

## Actual execution recipes still required

`tests/ros_custom_types/managed-csharp/test_managed_csharp.py` provides the
injected `suite(managed,csharp,parent,bounded,*,scratch,source_root,guard)` with
12 authored source-generation policies. The caller must bind actual coherent
source modules and private fresh scratch outside that source. It creates no
process, compiler, transport or ROS object. The suite checks the existing actual
IR, all eight maintained preparation catalogs, language isolation, real source
pins, refusal behavior and module retirement. Its source-shape assertions are
supplementary; they do not stand in for the real C# cases.

`ManagedContract.csproj` compiles exactly one generated source, one explicit
contract and the actual selected SDK `Codecs.cs`/`Result.cs`. It has no NuGet
PackageReference, reflection, fake transport or native hook. The Root executor
must use actual admitted .NET8, offline/fixed SDK settings, new owned intermediate
and output directories and bounded serial commands. Do not run restore/build
from the source checkout with default obj/bin paths. For example, after actual
source generation, copy the project to a fresh owned build directory and use:

```sh
dotnet build /fresh/build/ManagedContract.csproj --no-restore \
  -p:GeneratedRecords=/fresh/generated/csharp/BoundedRecords.cs \
  -p:ContractSource=/admitted/source/tests/ros_custom_types/managed-csharp/RecordContract.cs \
  -p:RosWasmSdkRoot=/admitted/source/sdk/csharp/RosWasm \
  -p:BaseIntermediateOutputPath=/fresh/build/obj/ -p:OutputPath=/fresh/build/bin/
```

`--no-restore` requires that Root has first performed a genuine bounded restore
into that same owned tree with a closed offline source configuration. No assets
file is fabricated. Root must record selected SDK/compiler/runtime, actual full
source/output hashes, return codes and raw logs; commands here are not observed
successes. A sensible initial host-only lane is one command at a time, <=1GiB
RSS and <=60s per restore/build/run with a separately bounded outer owner and
normal final preservation. Root decides actual admission; no remote actor is
started by this source package.

Use the complete existing managed-records fixture for `RecordContract.cs`:
15 authored real typed cases cover the 49-byte scalar golden, signed narrow and
full u64 values, strict text and floats, malformed/trailing input, complete
nested/fixed/sequence values, actual defaults, explicit Service/Action component
records, time normalization, input ownership and legal native field names.
Generate the separate names catalog for `NamesContract.cs` (one case/eight
fields), rather than merging it into or weakening Python/Java's fixtures.

Compile `FamilyContract.cs` six times, each against its separately generated
actual catalog. Set `-p:FamilyDefine=JOINT1`, `JOINT2`, `ROBOT1`, `ROBOT2`, `CIO1`
or `CIO2`; multiple or absent roles are compile-time errors. The roles map to
`official-standard/ros1`, `official-standard/ros2`, `sdk/robots/xarm/catalogs/ros1`,
`ros2`, and `catalogs/cio_state/ros1`, `ros2`. Every role independently builds
complete Header/time/string and every family field, compares the full canonical
wire against explicit little-endian primitives, round-trips it, checks real
metadata absence and rejects a bad subsecond. JointState also refuses sibling
length mismatch. These are host codec cases, not native CDR, Gateway, Wasm or
ROS delivery credit. Geometry/default closure is additionally generated by the
12 policies and exercised in the main actual typed default/nested cases.

`ManagedRobotExample.cs` accepts an already selected real Session plus an
explicit publisher alias. It demonstrates this generated ICodec with the actual
SDK Node API. The example creates no default selection, grants, backend fallback,
automatic retry, native fixture or native identity. Root must first qualify the
chosen generated/native catalog and actual selected Wasm/native route before
claiming interoperability. A complete catalog generated for an additional vendor
is not by itself a validated vendor SDK installation or permission to publish.
