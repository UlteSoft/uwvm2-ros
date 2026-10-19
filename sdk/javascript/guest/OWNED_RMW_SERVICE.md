# Explicit embedded-JavaScript owned-RMW service builds

This additive producer path is **Source only and unexecuted**. It plans an
embedded QuickJS Wasm build containing the separately reviewed copied C bridge
`native_rmw_service.c`. The existing business81 and optional client3 catalogs,
raw declarations and original checkers are unchanged. It does not install a
provider, open a capability or establish native ROS delivery.

The bridge is off by default. Select ROS2 in the closed manifest, then supply
`--rmw-service-enabled 1 --rmw-service-catalog-sha256 <exact-raw-SHA256>` to
`prepare.py`. Explicit0 behaves as the default and takes no catalog digest.
ROS1 and pure-library backends refuse this option. The raw digest must identify
the complete bytes of `sdk/abi/rmw-service-v1.json`, not reserialized JSON.

The producer verifies all twelve names, ordering, parameter/result widths and
the complete output contract. The catalog must bind the actual current Host,
implementation and raw header bytes, plus the unchanged original business
catalog. These hashes are provenance DATA and require an externally admitted
Source snapshot; they do not authenticate their own origin.

Only this explicit selection adds `-DUWVM_JS_RMW_SERVICE=1`, compiles
`native_rmw_service.c` and allows its twelve exact undefined C symbols. The
engine exposes `roswasm:rmw_service_native` only through its checked builtin
registration. Generic scalar imports may not select the exact reserved
`uwvm_ros2_rmw_srv_v1` namespace. Other existing generic namespace behavior is
unchanged by this delta.

The native builtin exports only
`invoke(index, scalarArgs, inputUint8Array, outputUint8Array) -> u32`.
Indices follow catalog order0..11. Its scalar vector contains the non-memory,
non-buffer fields of that operation: handles, incarnation, ticket, sequence,
reservation and wait values are unsigned full-width BigInt; expected-writer
extent is an unsigned32-bit Number. Root's C bridge owns all numeric offsets
and private scratch. The pinned engine's staged C API is also corrected to
report the current element extent of a length-tracking resizable/growable
buffer. Upstream `ta->length` retains its initial extent after a resize;
current `p->u.array.count` times the genuine element size matches actual JS
`byteLength`. The preceding upstream detached/OOB check stays intact, and no
user-overridable property getter participates. The producer requires exactly
one original release match, retains the unchanged official source digest, and
records the actual patched compiler input separately. This correction applies
to all builds using this producer, including its existing copied business
bridge. Actual emitted-engine behavior remains unexecuted. The separate
`owned_service_native_refusals.mjs` fixture covers detached and OOB views,
tracking growth/shrink, and an explicit fixed-length view. Requests4/6 take input; other requests require empty input.
Output is exactly256 for activation/inspection,8 for submit,1 for availability,
352..configured output bound for peek, and empty for all other operations.
Nonzero errors preserve caller output. Successful peek remains a borrow until
the independent exact commit/cancel ACK; no retry or cancellation is invented.

| Index | Operation | Scalar arguments in order | Input | Output |
|---|---|---|---|---|
|0|abi_version|none|empty|empty|
|1|client_activate|source, observer|empty|256|
|2|service_activate|advertise, responder|empty|256|
|3|endpoint_inspect|source, incarnation|empty|256|
|4|client_submit|source, incarnation|canonical request|8|
|5|client_abandon|source, incarnation, ticket|empty|empty|
|6|service_respond|advertise, incarnation, responder, ticket|canonical response|empty|
|7|endpoint_peek|source, incarnation, expected_writer|empty|352..bound|
|8|endpoint_commit|source, incarnation, sequence, reservation|empty|empty|
|9|endpoint_cancel|source, incarnation, sequence, reservation|empty|empty|
|10|client_available|source, incarnation, availability, wait_ns|empty|1|
|11|endpoint_retire|source, incarnation|empty|empty|

Input/output scratch use the existing `UWVM_JS_INPUT_BYTES` and
`UWVM_JS_OUTPUT_BYTES`. The producer requires output at least352 and retains
the original conservative4193786 upper bounds. Raw ABI's4194304 peek ceiling
does not enlarge every existing JS scratch allocation.

For enabled builds the report contains the complete expected import rows:
business43, optional12, exact WASIp1 fields, actual generic scalar signatures,
and the existing explicit pure Unitree modular fields when selected. Duplicate
module/name keys refuse during planning. The new serial
`full-binary-import-contract` job has **no `--allow-module` escape**. It captures
all pinned Source dependencies and contract bytes, loads the original
`check_wasm_imports.py` inspector from the same captured hash-checked bytes,
and compares every actual module/name/kind/parameter/result exactly once.
Unexpected, missing, duplicate and non-function imports fail. This checks only
type/import syntax; the original independent full validator job stays mandatory.
The legacy `expected_ros_imports` field remains the business inventory count;
`expected_rmw_service_imports` and `expected_total_ros_imports` distinguish the
optional12 and total55. The complete row contract remains authoritative.

The owner must admit the actual Linux Python entry, interpreter, stdlib,
extensions, startup and complete staged Source closure before execution, and
run every job serially within the existing64GiB cgroup with bounded output,
RSS/deadline and owned-process controls. `-I -S -B` is not interpreter
hermeticity and `-B` does not disable reading `.pyc`. The inspector uses no
SourceFileLoader or local-module import, so ambient `.pyc` and aliases cannot
replace its captured bytes. Its private context is removed after each call.
The entry's later self-hash is consistency evidence, not retrospective startup
authentication. Keep full compiler `-MD` closure, linker trace and matching
final artifact digests across validation/inspection; Source pins alone do not
cover all native toolchain assets.

`sdk/tests/javascript_rmw_service_models.py` supplies meaningful Source and
synthetic binary fixtures: complete inventory, exact WASIp1/scalar widths,
catalog/output mutations, missing/duplicate/unexpected/non-function imports,
wrong signatures, reserved generic injection and default-off/ROS1 refusal.
The owner supplies all four exact Source/catalog digests to this model runner.
These fixtures were authored but **not run** in this packet. They instantiate
no Wasm, engine, Runtime or native ROS object. Actual C compilation, full Wasm
validation, real Guest/native service/metadata, error paths, DBG, platform and
performance qualification are separate pending work.
