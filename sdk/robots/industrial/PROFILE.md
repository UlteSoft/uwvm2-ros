# Complete vendor source domain

`vendor_profile.py` is an independent, opt-in source-preparation API. It consumes the complete original `.msg`, `.srv` and `.action` definitions and captured, explicitly SHA-paired `sdk/tools/custom_types.py` and `sdk/custom-types/bounded_profile.py`. It does not modify either existing generator, change their old profiles, acquire a capability, start ROS, compile code or qualify hardware. The format is `uwvm-ros-vendor-complete-bounded-v1`. Existing bounded/control identities and generated products remain historical products of their own source revisions.

The public source contract is:

```python
api = API(parent_raw, bounded_raw,
    parent_path=actual_absolute_parent, bounded_path=actual_absolute_bounded,
    parent_sha256=reviewed_parent_sha, bounded_sha256=reviewed_bounded_sha,
    templates=all_four_captured_rpc_templates,
    adapter_raw=actual_adapter_bytes, adapter_sha256=reviewed_adapter_sha)
records, ordered, interfaces, input_pins = api.load_catalog(actual_catalog, False)
products, proof = api.products(actual_catalog, guest_only=True,
    helper_source=actual_captured_native_identity_source)
# A later independently admitted native export is DATA, never an identity oracle:
bound_document, binding = api.bind(actual_unbound_catalog, actual_native_json)
# Persist bound_document in a fresh source revision, then call products with
# guest_only=False on THAT exact catalog. Do not mutate the old preparation.
proof = api.generate(actual_catalog, fresh_absolute_output,
    guest_only=True, helper_source=actual_captured_native_identity_source)
```

`load_catalog` returns the actual existing Field/Record/Interface classes in an isolated function environment. `records()` emits all six record targets: C11, C++20, Rust, JavaScript, AssemblyScript and TinyGo. No generated product has been materialized by the authors of this source cohort. `products()` also emits the actual existing typed Service/Action interface templates, a stopped native metadata exporter source and typed constant-proof header. Bound generation adds the manifest and concrete native conversion header. Existing interface transports require the independently implemented complete-vendor Gateway admission; emitting a facade does not install that transport. Root owns the corresponding current Gateway integration. A default-denied Runtime remains denied.

`templates` has exactly these four relative keys, each mapped to the complete captured bytes:

- `interface_templates/bounded_interfaces.rs.in`
- `interface_templates/bounded_interfaces.mjs.in`
- `interface_templates/bounded_interfaces.go.in`
- `interface_templates/bounded_interfaces.ts.in`

The API is serial and not reentrant. Each instance owns private modules/function globals. It executes only the paired captured Python source when the operator is explicitly invoked; it does not resolve PYC, plugins or an ambient generator import. Every source pathname actually used is reopened and compared before a source receipt is published. The native exporter helper is an explicit captured source parameter and is recorded separately in the receipt. The caller must also retain the complete original source/lock/license closure and actual compiler/SDK dependency evidence. Root's `complete_prepare.py` is the pipeline integration; `prepare.py` is a standalone explicit source operator and does not modify the old vendor19 command.

## Original fields, constants and defaults

Original field order, names, types, array shape and native capacities are retained. Canonical capacities are explicit quotas, never alterations to a native variable-length declaration. Scalar and string constants remain class/module facts, consume no instance wire bytes, participate in this new canonical source identity and are checked against genuine compiled native C++ constants. No filtered definition is written. A constant-only message may be a dependency; it cannot silently become an empty registered Topic.

The legal original `type` field is kept in C/C++/JavaScript, mapped to `r#type` in Rust, `ros_type` in AssemblyScript and `Type` in TinyGo. `member_map` publishes every mapping. Identifier and generated-method collisions fail before output. Unsupported C/C++ keyword members, wide strings, preprocessing and array/nested declared defaults are explicitly refused rather than erased. A source bundle is one explicit ROS major and has at most512 original interfaces.

ROS1 `byte` is signed int8 and `char` unsigned uint8. ROS2 `byte` remains an IDL octet in native schema while `char` is IDL uint8; both use checked unsigned canonical storage. The source fact is retained as `native_scalar`, so a canonical storage match does not become a native type match. ROS1 `time` and `duration` use actual `ros::Time`/`ros::Duration` fields in native static checks. They are not invented ROS messages or MD5s.

Scalar defaults use the existing exact width/range/finite parser and normalized bool/int/float metadata text. String defaults use the pinned original `rosidl_adapter.parse_primitive_value_string`: remove matching outer quotes and unescape only that quote. Backslash-n is two semantic bytes, not a Python newline escape. The pinned `serialize_field` records `str(annotation.value)`, without the source quotes. The original literal and normalized declaration are distinct facts. Native field tables compare the complete counted default text; RIHS excludes defaults and cannot replace that comparison. Every declared default sets the explicit `has_declared_default` source-layout tail in the paired current custom field descriptor.

An explicitly empty string default is retained in records, native source schema and canonical identity. The official `full__description.c.em` maps both absent and declared-empty values to `{NULL,0,0}`; pointer/capacity and RIHS cannot distinguish them. Consequently this source API refuses native **binding** for that case until an independent original native IDL/source-proof protocol is implemented. It never silently treats the declaration as absent. The selected industrial original sources do not need that unsupported native proof case; retain their exact inventories to verify the statement for the selected revision.

## Wire and source ownership

Every record is complete, little endian and bounded. The existing32KiB wire ceiling, uint64 Cartesian arithmetic, expanded4096 nodes per record, total16384 nodes and depth16 remain enforced. `wire_limits` may separately bound the logical aggregate; it does not shrink inline C storage or Cartesian shape. Strings are counted strict Unicode-scalar UTF-8 with embedded NUL refused in this industrial text domain. No control raw-byte exception is implicitly applied. `encode` validates and measures the entire value before writing; `decode` validates a private complete value and publishes once. Errors preserve caller outputs and the existing22 versus90 distinctions where the target API exposes statuses. No codec reads Native pointers from Guest memory.

C public symbols follow the actual Record property: `roswasmc_bounded_<major>_<native_name_with_underscores>`. Public functions are `_valid`, `_wire_size`, `_encode`, `_decode`, and `_default` when there are declared defaults. C++ `uwvm_custom::Ros2Control_msgsActionFollowJointTrajectory_Goal` is an alias of that owned C record; its codec is `roswasmcpp::Codec<T>`, not a fabricated native class. Rust uses `Canonical`, JS a complete owned codec object, AS the checked `VendorCodec` result and TinyGo `Encode`/`Decode<Symbol>` from the actual generated source. The existing raw SDKs and their actual language producer/import/linear-memory gates remain separate dependencies.

## Genuine native boundary

No native identity is computed from type spelling or canonical hash. `bind()` accepts only a whole independent stopped-exporter document in `uwvm-ros-industrial-native-v1`, exact source profile, exact constant-schema source hash, complete root/component inventory and actual identities. Modern ROS2 requires the complete original type description fields/defaults and exact referenced graph; no metadata/hash fallback fabricates missing API. The Action exporter also checks the genuine five-link TypeSupport association before its original SIGSTOP/I gate. ROS1 compares complete Definition sections including constants and every dependency; the actual SDK supplies the root/component MD5s.

New manifests set descriptor tail `{official_standard=false, original_control=false, complete_vendor=true}`. New native headers specialize `complete_vendor_native_traits<ActualNative>` and mark each codec `complete_vendor_profile=true`, calling a catalog-hash-specific typed constant proof. These are a source compilation seam requiring the paired Root implementation and a complete consumer rebuild; they are not a stable binary ABI. Standard dependencies do not automatically become Topics. The current Gateway must independently check this domain, payload/protocol graph and actual constants before creating a Node/endpoint. The old generic constant refusal is retained in its old domain. A matching JSON document alone proves neither execution, permission, middleware behavior nor physical robot safety.

The exporter source includes only real generated type headers and the actual bounded native protocol helper. It creates no Node or business endpoint. Qualification must separately retain the real generated headers/TypeSupport libraries, full compiler MD/link closure, current owner process/maps, original SDK values, release gates and Native communication/effect counters. All authored source contracts, CMake targets and hardware recipes in this cohort are unrun.

## Source correction in revision2

Revision1 is preserved with a confirmed isolated-environment defect: the cloned `_member` function could not find the new `C_KEYWORDS` constant. Revision2 injects a finite, explicit set of the source functions' module/constants into their isolated globals, preserving the actual old bounded classes and emitters. Its modern metadata comparison also uses strict canonical JSON bytes, so Python's `True == 1` and `1.0 == 1` cannot admit a changed metadata field type. New authored contracts exercise the genuine parser/emitter path and both root/reference numeric-type substitutions; no contract has been run.


## Full unsigned-byte sequences without artificial node expansion

In this explicit complete-vendor profile, an array or sequence whose canonical
element is `uint8` costs two schema nodes: its bounded container and its scalar
byte storage. It does not allocate one object per byte. All byte elements, the
original native scalar spelling, native bounds and canonical sequence count are
retained. ROS1 signed `byte` remains `int8`; ROS2 `byte` and `char` retain their
original distinct native metadata even though their canonical elements are
unsigned bytes. No text interpretation, UTF-8 conversion or NUL refusal is
introduced for byte sequences. Other scalar and message arrays keep their
existing Cartesian node/depth accounting.

The existing32KiB complete-value wire ceiling and per-field capacity still
apply before output/commit. uint64 Cartesian extent, aggregate response budget,
4096 schema-node/record,16384 schema-node/catalog, depth16,512 interfaces and
operator file/byte budgets are independent. A byte capacity of16384 therefore
does not consume16385 object-graph nodes or silently increase the wire budget.
Canonical byte order, size, per-byte values and codec identity are unchanged
for previously supported definitions; the identity still binds the full byte
capacity and every field. Native hashes, complete TypeDescriptions and typed
constant proofs are not inferred from this accounting rule.

This is needed for complete protobuf RobotState payloads and image/tensor
values. A chosen16384-byte protobuf quota is a finite deployment profile, not a
claim that every possible upstream message fits. An overbudget value is refused
whole and never truncated, split or replaced by a typed projection. The actual
C/C++ protobuf SDK must parse the complete owned payload with its independent
resource/unknown-field policy. Native controller communication still passes
through the existing explicit ROS service alias and one permission boundary.

`tests/ros_industrial_vendors/fixtures/byte-sequence` is an explicitly synthetic
complete message, not an upstream/robot identity. Prepare it through this exact
adapter in a fresh admitted `/tmp` directory, then compile the real
`byte_sequence_contract.cc` against that emitted `bounded_records.h` and the
actual SDK includes. The C codec contract checks all16384 bytes, a full-width
trailing scalar, late truncation and oversized-count destination preservation.
It is authored and unexecuted here. It grants no compiler, Native, Wasm or
performance qualification.


## Complete ROS1 root categories and original spelling

The paired complete-vendor adapter and standalone preparer now admit up to512
original interfaces. Wire32KiB, depth16,4096 nodes per record,16384 aggregate
nodes and captured source byte limits remain independent and unchanged. The
older parent/bounded profile retains its own128-interface ceiling.

ROS1 permits an original Foo.msg alongside Foo.action, with actual native Foo.h
and FooAction.h headers. The complete profile keeps both original root names,
all components and separate kind-specific descriptors. Duplicate declarations
in one category, other same-name category collisions and generated public-symbol
collisions remain refused. Original ROS1 names may start with a lowercase letter,
as the real Clearpath q62AreaZoom service does; ROS2 keeps its existing uppercase
root/category spelling rule.

For a same-name ROS1 Message/Action pair only, our independently compiled native
exporter JSON uses message:PACKAGE/Foo and action:PACKAGE/Foo as association keys.
The binder derives these keys from the complete selected source and matches
catalog rows by both original type and source category. Noncolliding domains
retain their previous exporter keys and generated product bytes. Native type
names, MD5s, full Definition graphs, constant proofs and independent owner
evidence remain original and mandatory. This key is not a native identity or
a way for a Guest to select a new authority.

The staged Clearpath lane retains complete acquired originals and independent
nonzero vectors. Its exact original-source/product/native/Guest/performance
qualification must be read from the final delivered report, not inferred from
this profile description or from a source-only receipt.
