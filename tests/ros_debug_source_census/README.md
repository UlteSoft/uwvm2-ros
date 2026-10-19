# Sealed ROS loader census for synchronous read-only DBG

This source cohort supplies concrete loader provenance and pointer-free function
DATA for the existing all-stop source-memory reader. It does not change that
reader, open a Host gate, stop a process, grant a ROS alias, issue a capture, read
memory on its own, or authorize checkpoint/cache/restore/replay/mutation. Every
candidate/generator/compiler/test/Guest/Native/SSH execution is zero. The actual
DBG entrance and real paused-frame tests are a separately owned integration.

## Actual production API

The actual `full_source_instance` has two new const queries:

```cpp
bool actual_debug_memory_provider_census(size_t actual_id,
    uint_least64_t actual_epoch, wasm_module_storage_t const* actual_module) const noexcept;
bool actual_builtin_ros_function(size_t actual_id,
    uint_least64_t actual_epoch, wasm_module_storage_t const* actual_module,
    size_t actual_import_index, builtin_ros_function_data& output) const noexcept;
```

These arguments are observed actual dense module/validation epoch/member/import
ordinal from the canonical Source's actual runtime registry, not user labels or
permissions. The existing actual validated-file check authenticates that member
before any supplied module is dereferenced. The caller must still retain the
same genuine maintenance/publication/closed-host/N/all-stop/generation/Source
owners across the complete synchronous borrow. A shared Source pointer or a
saved successful boolean is insufficient. Returned function DATA contains only
kind, finite function ordinal, interface revision/digest, counted i32/i64
parameter/result codes and fixed arrays. Failure preserves the output object.
No function pointer, module pointer, Guest address or resource owner is returned.

The new census checks the entire FINAL native vector's base and extent captured
by the actual initializer. Every remaining member must be the one authentic
WASIp1 instance or one of the seven factory-certified ROS implementations.
Canonical shared ownership, current selected factory object/control block,
original implementation pointer, bounded original ordinal and exactly one
`all_module` membership must agree. An unknown native object, copied wrapper,
name imitation, changed/relocated vector, stale factory or Source epoch fails.
The first read-only cohort refuses the threads provider as unknown; the existing
source-memory reader independently refuses shared/imported/ambiguous memory.
This does not imply that threaded shared memory can be inspected by this port.

Preloaded DL and weak symbols are refused both as permanently observed at the
actual Source seal and if present at query time. They can have received C ABI
preload/WASI tables and escaped mmap addresses: closing a current admission gate
or clearing an exposure option is not an adapter for them. Legal administration
must preserve the existing actual full-Source/provider retirement rules; no
foreign provider is removed, reconnected or relabeled by this code. The identity
never owns/destroys a native implementation or keeps it alive after its real
provider owner has retired. Existing drain/lease protection remains mandatory.

## The actual factory, not an import-name allowlist

`builtin_ros_loader.h` and its independent named module depend only on the real
Wasm type/storage graph and fast_io crypto. They do not import ROS Host or Runtime.
Their only identity issuer is the private `append_typed` friend invoked by
`sealed_debug_loader_factory`, defined beside the actual seven complete ROS
Concrete types in `host.h`. That native-only class is explicitly attached with
`extern "C++"` in both declaration and definition; it does not redeclare those
ROS types in another named module. A constrained public template admits only
those actual stateless Concrete types. It cannot accept an external implementation
pointer, supplied ABI table, guessed namespace, kind-ready flag or callable.

The issuer constructs the genuine `Concrete{}` itself exactly once. The ordinary
loader uses it for its original seven insertions. The existing threaded loader
uses argument-dependent lookup to delegate its ROS insertion to the same issuer,
then records that SAME object in its original threaded provenance table. There
is no second ROS implementation and no Runtime business/callback change.

Only an actual owned Source image causes tuple/hash record allocation; ordinary
unowned-image loading performs the original construction without extra tuple
hashing or debugging images. That path cannot later fabricate a debug identity.
The complete ordered known tuple is derived from the actual
`all_function_information_cache<Concrete::local_function_tuple, ...>`, including
module name, finite function indices, names and exact signature types. Counts
are bounded by64 functions,16 scalar arguments/results and1024 name bytes. These
are implementation ceilings, not a widening of the43/38 business catalog or
optional namespaces. Unknown future types/fields cannot pass by matching names.
The digest binds the entire actual interface with explicit LE64 lengths/counts,
revision and concrete kind. All returned signature fields come from that tuple.

An identity is provenance only. Namespace availability does not start a Gateway
or initialize native ROS. Ordinary Runtime caller/module/instance/source epoch,
explicit operation grants, native binding/QoS/quota and session checks still
apply to every real call. The new Source census does not replace them.

## Source audit: Guest aliases do not cross this closed Host boundary

The retained exact source observations cover these actual chains:

| Production boundary | Actual ownership behavior relevant to a stopped read |
|---|---|
| `wrapper::function::call` | Reads scalar values from the genuine local function frame; invokes the fixed function pointer from its compile-time type; writes the scalar result. It exposes no Guest pointer ABI. |
| `host::memory_range/read_bytes/write_u64` | Descriptor is a local value; range is checked, input is copied into a private vector and fixed output bytes are synchronous checked writes. |
| Runtime `call_local_imported_with_wasip1_env` | ROS calls choose the existing copy-mode preload context, not mmap delivery. The new census does not trust that name check as loader proof. |
| Runtime `preload_memory_read/write_host_api` | Actual bounded memory-copy path or actual threaded copy helper; snapshot/range checks remain next to memcpy. The copy descriptor's mmap pointer is null; no descriptor is retained by ROS. |
| ordinary publish/service/action/parameter operations | Payload vectors are private, then moved into the genuine synchronous IPC client. Response vectors are owned and checked before synchronous Guest writes. |
| event watch/poll | Long-lived watch rows store copied capability specifications, numeric source/request identities and an owned event-stream queue. Gateway callbacks enqueue owned bytes, never Guest addresses or a Guest callback closure. |
| owned RMW publisher | Allocates one complete private UMRE/business packet and copies canonical input before IPC; retained endpoint history contains native binding/actual QoS/GID DATA, no Guest storage. |
| owned RMW subscription | Reply owns UMRS/URMI/body bytes. Full association is checked; quota spans are lexical. A known borrow is canceled/retired on refusal, followed by one checked complete Guest copy. ACK is explicit. |
| owned RMW service | Private UMRC/body copy; temporary ticket prefix is restored before copy/RPC. Paired source/observer/responder leases remain real. Incoming URMI/body is owned, compensated on post-association exceptions, then copied once. Ticket/bool results use bounded stack bytes. |
| authorization/audit/retirement | Authorization retains actual capability state/lock and native identity, not message spans. Audit rows own copied text or fixed labels. Shutdown retires Host instances/queues/socket owners after actual execution/Host drain; no new lease was invented here. |

The audit concerns escaped Guest-memory aliases, not protocol atomicity or a
claim that every transport/native error has no effect. Existing late ACK,
unknown send, cleanup diagnostics and every original native command remain
unchanged. Native middleware, external file mappings and arbitrary DL/weak/C ABI
providers have no adapter in this Source and are refused by the whole census.
There is no independent untrusted API that can append a provenance identity.
Native code and the build toolchain themselves remain trusted boundaries.

## Preserve stricter old admissions

The whole original bodies of `actual_no_unadapted_native_memory_provider`,
`actual_cache_native_provider_census` and `actual_builtin_wasip1_function` remain
byte-for-byte. All existing checkpoint initializer/environment/capsule,
checkpoint effect, cache and mutation callers continue using those old methods.
Only the separately authored synchronous read-only DBG entrance may use the new
census and ROS signature query. A ROS process still cannot checkpoint/restore its
native endpoints or replay a physical effect. No change was made to Runtime
`default.cpp`, WSEA, init/management gates, cache/restorer or permission defaults.

## Real contracts and future qualification

`identity_contract.cc` directly uses these production headers/methods. A copied
same-name/same-tuple fake type cannot instantiate the real factory; identities
are not default-constructible/copyable. Under a genuine external Runtime harness,
`actual_ros_fixture` checks positive actual Source/tuple lookup, all finite
parameter/result codes, failed zero-epoch/null-member/out-of-range lookups and
unchanged failure destinations. It also requires the old generic/cache queries
to remain closed for that same actual ROS Source. `actual_refused_fixture` must
be called on separately constructed real foreign/vector/stale fixtures; it is
not a fake Native provider or a self-declared ready result.

`guest.c` is a real C11/WASI/DWARF source with one ordinary ROS ABI-version import
and an original typed global value. It is suitable for an actually admitted
compiler/debugger fixture. Volatile does not authorize cross-native-thread reads:
the real all-stop Host entrance must perform and retain its original admission.
The CMake switches are off by default. Contract compilation needs an actual
existing project's Runtime usage target; no fabricated feature header or target
is supplied. Guest compilation requires the actual WASI toolchain and a separate
build. Nothing configures/runs automatically, no native ROS process is launched,
and no test/generated/parser result is recorded in this cohort.

Remaining actual qualification includes header and named-module compile/link/
complete dependency closure, same-source ABI/import assertions, real Runtime
paused leaf/caller read with unshared memory, wrong epoch/module/function/cache
rejection, stale/relocated/duplicate/imitation/foreign-provider cases, an actual
active-Host call that prevents the all-stop read, all failure-output sentinels,
Source/provider retirement lifetime, and proof that unchanged checkpoint/cache/
mutation paths still refuse ROS. These are authored requirements, not completed
runs. Root is the only public Source integrator and future execution controller.

## R2: query-local complete declaration association

The query now independently proves the original actual-WF function import ordinal
through the actual import section's owned element, including a genuine retained
runtime import-reset copy. Its actual function pointer must resolve to the same
selected WF type-vector index. Runtime type begin/end/count must match that WF.
Modern owned signatures are checked by full numeric kind, all carrier codes,
exact arity and the actual owned carriers allocation/sentinel. Legacy ranges
must belong completely to the genuine immutable Source byte image before reads.
Same total bytes, wrong order, i32/f32 or i64/f64 are not equivalent. No opaque
provider callback or initializer import is used. Failure never changes output.
The contract returns immediately if a future admitted tuple regresses beyond its
fixed arrays, so reporting that regression does not itself read out of bounds.
Actual declaration-mismatch fixtures remain part of the separately admitted real
Runtime tests; no original source or metadata is forged into a successful query.
