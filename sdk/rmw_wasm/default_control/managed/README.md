# Complete original control records in four additional Guest languages

This explicit source operator implements all32 original control records for
Rust, JavaScript, AssemblyScript and TinyGo. It preserves every field, constant,
scalar default, native scalar spelling and canonical identity of the existing
C/C++ domain. All seven Service roots also have explicit identity facts, distinct
from their Request/Response identities. No native identity is calculated.

This revision deliberately emits **records only**. The actual full-control
Gateway refuses95 on legacy business `service_call`, `service_submit`/observer,
advertise/respond and availability paths for this family. Consequently the old
four-language RPC templates are not emitted here. Real Service owners require
the existing independent `uwvm_ros2_rmw_srv_v1` raw12 protocol and are being
implemented as separate owned transports. A local record roundtrip is not a
Service SDK, native client ABI or successful default Node. Default rights are
empty, and no Guest raw import, engine, Runtime or SDK core is modified here.

## Separate source operators and independent fresh revisions

Freeze a coherent reviewed source tree containing the actual parent
`sdk/tools/custom_types.py`, extended `sdk/custom-types/bounded_profile.py`, this
operator, the control catalog and every original definition. Preserve actual
hashes and file observations. Use a private staged SDK and `/tmp` scratch/output
for later local generation and tests; do not write into a shared public SDK.

The historical entry point remains unchanged (the catalog is positional):

```text
/absolute/python3 /absolute/reviewed/sdk/tools/custom_types.py \
  /absolute/reviewed/sdk/rmw_wasm/default_control/catalog.preparation.json \
  --guest-only --out /tmp/unique-control-canonical
```

It still emits exactly seven source products plus `generation.json`, with
`implemented_record_outputs=["c","cpp"]`. Its old metadata describes that old
command and remains unchanged, including its historical other-language string.
All old generator source is an exact prefix of the extended file; only new
function definitions are appended. The natural bounded-generator SHA changes,
so regenerate the canonical revision using the same reviewed extended source.
No old custom/standard/fixed profile or C/C++ output logic is changed.

Invoke the explicit records operator next:

```text
/absolute/python3 /absolute/reviewed/sdk/rmw_wasm/default_control/managed/generate_managed.py \
  --source-root /absolute/reviewed \
  --catalog /absolute/reviewed/sdk/rmw_wasm/default_control/catalog.preparation.json \
  --canonical /tmp/unique-control-canonical \
  --parent-sha256 ACTUAL_REVIEWED_64_LOWERCASE_HEX \
  --bounded-sha256 ACTUAL_REVIEWED_64_LOWERCASE_HEX \
  --out /tmp/unique-staged-tree/sdk/custom-types/generated-control-managed \
  --records-only
```

The capitalized values are required actual externally reviewed inputs, not
ready-to-use hashes. The operator loads only captured source bytes matching
those explicit hashes; no catalog plugin or ambient provider is loaded. It
admits bounded ordinary files with `O_NONBLOCK|O_NOFOLLOW` before reading,
checks stable FD/name identity and exact extent, requires the exact selected32
record names and two-Topic/seven-Service root family, and pairs the separate
C/C++ canonical proof and every product. Inputs are reopened before any output
is written. Each source/product is at most8 MiB; the whole new source revision
is at most32 MiB. The fresh output must be separate from the canonical input.
The proof is written last. Filesystem failure leaves an incomplete fresh
revision; it is not resumed, overwritten, cleaned up or accepted as complete.
No subprocess, compiler, exporter, Native getter, ROS entity or test is run.

The new files are `bounded_records.rs`, `.mjs`, `.go`, `.ts` and
`generation.control-managed.json`. That independent proof lists all six source
languages, identifies this revision's four implemented record outputs, pairs
its original C/C++ source revision, and records source/constant/default/member
maps and original canonical/native identity facts. Preparation native IDs stay
absent. The proof explicitly lists no implemented Service transport.

## Complete native values and target-language spellings

Every original instance field is serialized, including inactive ParameterValue
storage members. Native IR and identity retain the field `type`: Rust spells it
`r#type`, Go exports `Type`, JavaScript uses the exact quoted name, and AS spells
it `ros_type`. AS Log's `function` is `ros_function`. All member mappings are
published and checked for collisions. Original byte/octet versus char/uint8
facts remain in IR. Constants are associated Rust constants, frozen JS codec
constants, typed Go constants and AS static readonly constants. They consume no
instance wire bytes. The GetTypeDescription request default keeps
`include_type_sources=true`. Decode requires every wire field; defaults never
repair a missing/truncated field.

Ordinary strings are strict UTF-8 without NUL. JS rejects lone UTF-16 surrogates
before TextEncoder can replace them and uses a fatal decoder preserving BOM.
AS first checks actual SDK `validUnicode` and `validUtf8`. The official
[counted String API](https://www.assemblyscript.org/stdlib/string.html) is used
after validation, without replacement or zero-terminated acceptance.

Only `TypeSource.raw_file_contents` is raw counted bytes: Rust `Vec<u8>`, JS/AS
`Uint8Array`, Go `[]byte`. It preserves arbitrary bytes, invalid UTF-8 and every
NUL including the final NUL in original source getter views. No strlen, text
conversion or terminator trimming occurs, and the real native String type is
unchanged. Other fields retain strict text rules.

Capacities include ten fields, eleven referenced descriptions, twelve sources
and6162 raw bytes per source. Logical TypeDescription/GTD response has a
separate32768 canonical-byte cap. Its complete cartesian capacity is larger:
twelve maximal source bodies cannot fit simultaneously. Checked accumulation
and per-record measure reject aggregate/output capacity with90 and malformed
shape/domain with22 before output allocation/publication. Only active logical
values are allocated; no6162-times12 cartesian storage is reserved. Native DDS
memory, allocator capacity, stack, object headers and RSS are separate budgets.

Rust validates before a caller-buffer write and returns an owned full decode.
Go validates before one exact wire allocation and returns zero value plus error
on decode failure. They require caller-exclusive ordinary values: concurrent
slice mutation/data races are not supported. JS captures every bounded field
and container element once, copies actual typed-array extent without user
length/species, and sizes/writes only private state. AS measures before one exact
private typed buffer. Its `encodeChecked` returns status/bytes, `decodeChecked`
returns status/value-or-null, preserving90/22 and never accepting a partial value.
Compatibility `Codec` methods retain the SDK valid/decode convention; checked
methods are the direct detailed interface. Terminal Wasm trap/OOM/process abort
is not a recoverable cleanup guarantee.

## Real Service-root facts and the owned transport follow-up

For each of seven Services, Go exports `<ServiceSymbol>CodecIdentity`,
`NativeType`, `NativeIdentity` and `HasNativeIdentity`. Rust exports an explicit
`<ServiceSymbol>Identity` value, JS a frozen identity object and AS an identity
class with static readonly facts. The root canonical ID is the actual original
Interface.codec_identity, not a Request/Response ID. Preparation native identity
is absent (`None`/null or separate false-presence), never an invented hash.
The proof's services table pairs root identity and exact component names/IDs/
maximum wire sizes for the independent owned transport authors.

Those facts are data, not authority or proof of Native support. The real
follow-up must open distinct service.call+event.observe (or advertise+respond)
owners, bind UMRE256 to the same actual retained incarnation and both queried
QoS profiles, retain u64 tickets separately from signed native sequence, and
fully validate UMRC160+URMI192+body before explicit sequence+nonce commit.
Known borrow rejection requires real cancel or terminal source retirement;
ambiguous ACKs are not replayed. Primary/cancel/commit/retire/raw-close outcomes
remain separate. Availability needs a third independent grant on the same
retained client. No legacy95 fallback, graph-list inference, fake metadata,
synchronous-as-asynchronous operation or empty successful response is allowed.

## Record-only usage examples

These snippets run no ROS operation. Their named modules/types are exactly the
operator output; stage the relative source layout before a later real build.
For Rust, import `roswasm::typed::Canonical` and the generated module:

```rust
let mut source = Ros2Type_description_interfacesMsgTypeSource::default();
source.type_name = "rcl_interfaces/msg/Log".into();
source.encoding = "msg".into();
source.raw_file_contents = vec![0, 255, 0];
let wire = source.encode()?;
let owned = Ros2Type_description_interfacesMsgTypeSource::decode(&wire)?;
```

For JavaScript, import the generated `.mjs` namespace as `records`:

```javascript
const codec = records.Ros2Type_description_interfacesMsgTypeSourceCodec;
const value = codec.defaultValue();
value.raw_file_contents = new Uint8Array([0, 255, 0]);
const owned = codec.decode(codec.encode(value));
```

For AssemblyScript, import the generated module as `records`. Use the checked
result before taking its value; an error is never an empty successful record:

```typescript
const value = new records.Ros2Type_description_interfacesMsgTypeSource();
value.raw_file_contents = new Uint8Array(3);
value.raw_file_contents[0] = 0;
value.raw_file_contents[1] = 255;
value.raw_file_contents[2] = 0;
const codec = new records.Ros2Type_description_interfacesMsgTypeSourceCodec();
const encoded = codec.encodeChecked(value);
if (encoded.status) return encoded.status;
const decoded = codec.decodeChecked(encoded.bytes);
if (decoded.status) return decoded.status;
// decoded.value now holds the complete privately decoded record.
```

For TinyGo, import the staged generated package as `records`:

```go
value := records.Ros2Type_description_interfacesMsgTypeSource{
    Raw_file_contents: []byte{0, 255, 0},
}
wire, err := value.Encode()
if err != nil { return err }
owned, err := records.DecodeRos2Type_description_interfacesMsgTypeSource(wire)
```

These are function-body fragments: the caller retains the exact language error
return type and ordinary variable use. They acquire no transport or authority.

## Actual SDK use and unexecuted contracts

In a private coherent staged tree, retain output layout
`sdk/custom-types/generated-control-managed` for AS's actual relative SDK
imports. Rust declares the generated module beside the caller and enables the
real roswasm `alloc,ros2` features. TinyGo copies the generated file to its
module's customtypes package, uses the genuine `sdk/tinygo` module and selects
exactly TinyGo/Wasm/ROS2. It retains full uint64/int64 values; JS uses BigInt.
AS only imports copied linear byte arrays, never managed object pointers.
No language acquires aliases, clocks, graph access, logging, threads or policy.

Use the existing actual pinned producer for each SDK. Do not substitute Javy,
QuickJS, Node or a different IR producer for another. Later language builds
must retain compiler/sysroot/helpers/source closure, private outputs/caches,
full Wasm import/signature/memory/layout validation and actual Host default-deny
results. Producer or Native success is not claimed by this source revision.

`tests/ros_original_control_managed` contains fourteen unexecuted operator/source
policies and four actual codec contract sources. They cover whole old C/C++
regression, strict source admission, raw bytes, all ParameterValue members,
defaults/constants, aggregate90, malformed22, private output ownership and exact
fixed/u64 domains. `recipes.json` supplies the actual source-operator argv;
compiler/artifact/Guest/Native qualification remains a later unified step after
the genuine owned Service layer and optional producer namespace are complete.

## Captured-source proof association correction

The records operator now requires the actual resolved IR catalog extent/hash to
match its initially captured catalog, not merely the later canonical proof and
terminal path bytes. It also requires the generated proof's two source hashes
to equal the captured parent/bounded bytes that actually executed, and publishes
that exact captured pair. A catalog or ambient provenance-read A→B→A race cannot
produce a self-contradictory proof. Two additional unexecuted policies stage their
own private original definitions/source copies and genuinely exercise those
association boundaries. None rewrites a shared SDK or executes source B. All
four record emitter bodies and the historical C/C++ command remain unchanged.
