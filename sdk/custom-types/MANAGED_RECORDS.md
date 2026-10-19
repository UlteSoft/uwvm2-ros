# Optional complete Python and Java record codecs

This layer implements source emission over the existing bounded schema IR. It
does not replace the original generator, change any of its six record outputs,
recompute a canonical identity, export native metadata, register a type or grant
an alias. The new module, its test sources and these examples are unexecuted
until a separately recorded actual run. A generated source receipt is also not
a compiler, Wasm, native SDK, ROS delivery, vendor, hardware or VTune result.

## Use the same admitted catalog

`managed_profile.py` accepts exactly the complete catalogs already admitted by
`bounded_profile.load_catalog`. Its programmatic entry is:

```python
generate(parent, bounded, catalog, output,
         languages=('python', 'java'), require_native_identity=False)
```

The trusted caller supplies the actual coherent `sdk/tools/custom_types.py` and
`sdk/custom-types/bounded_profile.py` modules. There is one authoritative catalog
load per generation. The optional emitters consume its resolved Field/Record
objects, dependency order, capacities, complete-value semantics, declared
defaults and exact `codec_identity` strings. There is no second `.msg` parser or
language-specific hash domain. Every original definition and generator/SDK
foundation source is reopened before the first output write.

The CLI loads those two fixed SDK-relative sources; a catalog cannot select a
module, Python provider or plugin. Choose a fresh output **outside the SDK source
tree**, in a caller-owned ordinary temporary directory. Preserve the complete
coherent source snapshot and the original `generation.json`/six output products.
For example, after actual source admission and review:

```sh
"$PYTHON" -I -B "$SOURCE/sdk/custom-types/managed_profile.py" \
  "$SOURCE/tests/ros_custom_types/official-standard/ros2/catalog.preparation.json" \
  --out "$OWNED_OUT/joint-state-managed" --language python --language java
```

An existing output or dangling output symlink is refused, not overwritten. The
outer owner must admit the output's physical parents and prevent aliases/races;
this source generator is not a process/cgroup/whole-root owner. It writes the
actual selected sources and `generation.managed.json` last. A failed write may
leave a partial failed revision; never promote that directory to a successful
receipt. Total emitted text is capped at64MiB. The profile's existing32KiB wire,
depth16, per-record4096 expanded-node, total16384-node and128-interface limits
remain authoritative; emission does not lift them.

Preparation catalogs intentionally contain no actual exported native IDs.
`--require-native-identity` asks the existing parser to require their syntax;
it does not obtain, invent or qualify those IDs. `NATIVE_IDENTITY` remains
`None`/`null` when absent. For native use, first run the existing actual exporter,
binder/native-schema/Gateway pipeline and use the exact resulting bound catalog.
An actual native root/component identity, default-bearing TypeDescription proof,
whole SDK closure and explicit Host operation/resource/type/QoS grants remain
mandatory. No alternative legacy default-pointer interpretation is introduced.

## Actual output APIs

Python produces `bounded_records.py`. Each complete record is a dataclass with
the exact resolved public symbol and native field spellings, an `encode(self)`
method and a whole-value `decode(bytes)` classmethod. The class itself satisfies
the existing Python Node codec calls:

```python
from bounded_records import Ros2Sensor_msgsMsgJointState as JointState

value = JointState()
value.header.frame_id = 'robot'
value.name = ['joint1']
value.position = [1.25]
wire = value.encode()
same_value = JointState.decode(wire)
# A real separately authorized publisher may use JointState as its codec.
```

It uses the current real `roswasm.codecs.Writer`/`Reader`, including f32/f64 and
exact signed/unsigned integer methods. Bool is not an integer field. Python ints
retain all64 bits within their exact signed/unsigned range. Fixed arrays are
lists of exact length; sequences are initially empty lists with their existing
maximum count. Tuple array inputs are also accepted. Iteration uses a captured
bounded count, not an unbounded iterator that can grow while being encoded.
Nested and fixed-array default objects are independently constructed. A legal
native field named `list` cannot replace the generated default factory. The
`classmethod` decorator and postponed builtin/dependency type annotations use
private leading-underscore aliases; neither actual ROS field grammar admits
that prefix, so a legal field such as `classmethod`, `list`, `str` or a ROS1
dependency-symbol spelling cannot shadow those helpers. Fields keep their
original spelling and wire order.

Java produces the fixed package
`org/uwvm/roswasm/generated/BoundedRecords.java`. It contains final nested
complete-value classes with actual public fields, constructors and a real
`Codec<T> CODEC` for each record. Primitive fixed arrays and sequences are typed
Java arrays; nested/string array elements are fully checked, not skipped when
null. Native field names are preserved. `uint8`/`uint16` use int, `uint32` uses a
long numeric0..0xffffffff, and **uint64 uses all long bits**. A negative long is
a valid uint64 bit pattern, not a refusal or floating-point conversion. Use
`Long.toUnsignedString` when displaying it. Signed widths use byte/short/int/long;
floating fields use float/double. The current real `Codecs.Writer`, Reader and
`Codec<T>` interfaces are used directly, with no replacement foundation writer.
Fixed string/record array initialization explicitly addresses `this.<field>`; a
legal native field `i` therefore cannot be replaced by the loop's local `i`.
JLS17 [identifier rules](https://docs.oracle.com/javase/specs/jls/se17/html/jls-3.html#jls-3.8)
and [field grammar](https://docs.oracle.com/javase/specs/jls/se17/html/jls-8.html#jls-8.3)
allow contextual `record`, `sealed` and `permits` as field identifiers. They
remain unchanged public fields here; generated type names have a `RosN` prefix.

Java decode first checks the resolved full wire extent and clones the caller's
mutable byte array before parsing. Decoded fields own their arrays/objects.
Python decode requires an immutable actual bytes object. Both encoders validate
every actually written scalar, strict string, array extent and nested record,
including normalized time and JointState constraints, then check the final owned
wire against its complete minimum/maximum extent before returning it. No partial
wire escapes on failure. Encoding does not decode and allocate a second complete
object tree. Each Python array element is captured once; write-side time checks
use the encoded captured subsecond scalar and JointState checks use the captured
encoded array counts. Every nested writer applies the same complete checks.
This is not an atomic snapshot promise for an arbitrarily shared application
object: use exclusive application ownership while encoding. Java decode retains
its bounded input clone. No benchmark or measured speedup is claimed. All work
and allocation are finite under the resolved profile and precede a Node effect.

Python/Java reserved names and collisions with generated metadata/Java CODEC
are explicit optional-language refusals, never silent field renaming. A refusal
for one optional language does not modify the original six-language parser or
its outputs. Python additionally refuses `encode`/`decode` method collisions as
a defensive emitter policy, but the current base parser already reserves both;
these are not newly admitted catalog fields. A future base-parser relaxation
would still require a separate codec-object design to support them. Likewise
Python soft keywords currently refused by the parent are not newly reachable
optional-emitter bugs. The source registry initially implements Python and Java and has
no fixed claim about the project's total number of languages. A future reviewed
C#/MoonBit emitter must also add its exact output names, foundation-source pins,
name validation and real API contract; no ambient plugin loading is supported.

One generated Python module and Java container describe one complete catalog.
To use multiple families in one application, select an actual complete composed
catalog through the existing reviewed composition/binding pipeline where
applicable, then generate once. Do not overwrite a revision, concatenate two
different `BoundedRecords.java` containers or present a partial family merge as
a complete catalog. Existing type/node/wire limits still apply to composition.

## Exact canonical behavior

Every current IR scalar is implemented: bool, signed/unsigned8/16/32/64,
binary32/binary64 and counted strict UTF-8. Fields, nested records and fixed
arrays remain in original order. A dynamic array carries the same u32 count.
Strings carry their UTF-8 **byte** count and explicit bound. This bounded
profile forbids NUL; builtin SDK String intentionally preserves it. A leading
BOM remains data. Invalid UTF-8, unpaired Java/Python surrogates, NaN/Infinity,
binary32 overflow, invalid Bool, wrong fixed length, excessive count, truncation
and trailing bytes are refused. No bytes are padded or discarded.

The same authoritative `standard_semantics` rules remain active. ROS1 native
time/duration primitives are complete values without invented Message hashes.
Subsecond fields are normalized to0..999999999. JointState permits empty
optional arrays but requires every nonempty name/position/velocity/effort array
to have the same length. Unknown future semantics/scalars are refused until an
actual emitter supports them.

Declared ROS2 bool, integer and finite float scalar defaults now use the
[complete scalar-default contract](scalar-defaults/README.md). C, C++, Rust,
JavaScript, AssemblyScript, Go/TinyGo and optional Python/Java/C# record outputs
preserve the original normalized annotation separately from each language's
typed initializer. This includes the previously admitted official Quaternion
x/y/z=0,w=1 variant. Default construction propagates through nested records and
each fixed-array element; dynamic sequences remain empty until explicitly filled.
Decode always reads every wire field: a complete w=0 payload stays0, and a
31-byte Quaternion is not completed using a default. Declared array/string or
nested-object defaults, constants and unsupported schema syntax remain refused.
Complete default-bearing Service/Action record graphs can be emitted; native
admission still requires the actual modern full root/reference/default proof.
The separately owned native service factory has its own explicit admission
boundary; record emission alone does not qualify that factory or transport.

Message records and complete Service/Action component records are emitted from
the same IR. An empty Service request encodes exactly zero bytes. Runtime-owned
tickets/Action envelopes, RPC facade/lifecycle methods and native protocol proof
are not fabricated by this record emitter. Existing SDK clients can use actual
generated complete request/response/component codecs with their existing
ownership APIs; automatic generated RPC facades remain separate work.

## Concrete catalogs and real test sources

The current source matrix includes actual ROS1/ROS2 Header/JointState,
RobotMsg and CIOState preparation catalogs, the ROS2 geometry/nav/default
closure, and the maintained nested/fixed/dynamic Quaternion fixture. The same
admissible schema/profile used for an official/vendor family can select these
optional outputs. This is not a claim that all19 vendor inventories, historical
native variants or platform SDKs have been generated, compiled or exercised.

`tests/ros_custom_types/managed-records/fixture` adds an explicitly local catalog
covering every scalar, nested/string/fixed/sequence values, actual same-byte
maintained Quaternion/Point/Pose source, ROS1 primitive time/duration and complete
Service/Action components. It supplies no native IDs, type-support stubs,
generated records or mock metadata. The source-copy association is retained in
the author package. The real Java contract uses an independent49-byte scalar
golden, full uint64 bits, strict strings/floats/bounds/ownership, complete nested
arrays/defaults and empty components, legal contextual fields and fixed `i`
arrays. It invokes14 named cases; actual assertion counts are reported only when
a real run occurs. The Python suite has29 authored
methods, including all actual catalog/IR identity comparisons and isolated
changed-input/keyword/collision refusals, real class-scope field roundtrips,
resolved type hints, encode-with-decoder-disabled and fixed `i` array wire
contracts. None was invoked by the source author.
The two XArm methods compare complete RobotMsg and CIOState bytes, every scalar,
array element and Header field, against independent struct layouts in both ROS
versions. `FamilyContract.java` does the corresponding complete Header/JointState,
RobotMsg and CIOState comparison for six explicit catalog/version roles. It uses
reflection only in this JVM test driver to select actual generated classes; it
does not imply TeaVM/WASI reflection support or substitute a native type.

After coherent source/foundation review and a fresh actual snapshot, Root may
run these shapes in private scratch, serially, under independently admitted
source/tool/cgroup/process/memory/disk/watchdog/finalization boundaries:

```sh
# Actual Python codec tests; scratch exists, is empty and is outside SOURCE.
PYTHONDONTWRITEBYTECODE=1 "$PYTHON" -B \
  "$SOURCE/tests/ros_custom_types/managed-records/test_managed_profile.py" \
  --source-root "$SOURCE" --scratch "$OWNED_OUT/python-cases"

# Real source generation, then Java compilation/execution; separate evidence.
"$PYTHON" -I -B "$SOURCE/sdk/custom-types/managed_profile.py" \
  "$SOURCE/tests/ros_custom_types/managed-records/fixture/catalog.preparation.json" \
  --out "$OWNED_OUT/fixture-managed" --language python --language java
mkdir -m 700 "$OWNED_OUT/java-classes"
"$JAVAC" -J-Xms16m -J-Xmx512m -d "$OWNED_OUT/java-classes" \
  "$SOURCE/sdk/java/src/main/java/org/uwvm/roswasm/Codec.java" \
  "$SOURCE/sdk/java/src/main/java/org/uwvm/roswasm/Codecs.java" \
  "$OWNED_OUT/fixture-managed/org/uwvm/roswasm/generated/BoundedRecords.java" \
  "$SOURCE/tests/ros_custom_types/managed-records/ManagedContract.java"
"$JAVA" -Xms16m -Xmx256m -cp "$OWNED_OUT/java-classes" ManagedContract
```

For the family driver, repeat the same generation and three-source Java compile
into **distinct fresh directories per catalog**, replacing ManagedContract.java
with FamilyContract.java. Select these exact preparation catalog/driver pairs:

| Catalog under SOURCE | Driver argument |
| --- | --- |
| `tests/ros_custom_types/official-standard/ros1/catalog.preparation.json` | `joint1` |
| `tests/ros_custom_types/official-standard/ros2/catalog.preparation.json` | `joint2` |
| `sdk/robots/xarm/catalogs/ros1/catalog.preparation.json` | `robot1` |
| `sdk/robots/xarm/catalogs/ros2/catalog.preparation.json` | `robot2` |
| `sdk/robots/xarm/catalogs/cio_state/ros1/catalog.preparation.json` | `cio1` |
| `sdk/robots/xarm/catalogs/cio_state/ros2/catalog.preparation.json` | `cio2` |

Run `"$JAVA" -Xms16m -Xmx256m -cp "$ACTUAL_CLASSES" FamilyContract robot1`
only with the matching actually generated/compiled catalog. Each complete
expected wire uses a separate ByteBuffer layout, including every Header field,
all7 RobotMsg angles/pose6/offset6 or all40 CIOState fixed elements. Complete
JointState arrays are also compared. Class metadata/codec identities still come
from the actual generator; no expected native hash is invented by this test.

Use actual admitted native Python/JDK tools, not a fabricated future path/pin.
Suggested isolated bounds are512MiB/120s for the Python source/codec suite and
1GiB/120s per serial Java compile/run, with a longer separately bounded source
admission and normal final preservation. These are future limits, not measured
performance. Preserve raw logs, actual counts, sources/inputs, compiler outputs
and failures. Source-policy checks are not Java compilation; JVM codec checks
are not TeaVM/WASI or ROS delivery. No source test needs a Backend or process
spawn; Java's compiler/runtime are separately counted real owned commands.

`managed-examples/joint_state.py` and `JointStateExample.java` use the actual SDK
Node/Session/Publisher/Subscription APIs and explicit R/Q stdin scheduling. They
must be compiled/frozen with the exact selected generated module and actual
SDK. The Python interpreter route needs the real target CPython codec/stdlib
closure. The Java route needs the real TeaVM WASIp1/linear-memory producer and
its native import checks, not a JVM-only result. See the existing language SDK
build/embedding documentation. Neither example grants WASI stdin nor a ROS
operation. Root must supply both aliases, full actual native type/default proof,
independent native source/observer barriers, a bounded stdin owner and cleanup.
An identical callback/print without that evidence is not native delivery proof.
