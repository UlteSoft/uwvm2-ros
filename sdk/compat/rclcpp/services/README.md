# Explicit AddTwoInts service source profile

This is an additive implementation over the separately opt-in rclcpp
String/topic profile. It implements the **complete field path of the unchanged
acquired Jazzy minimal_service/main.cpp**, and a real asynchronous AddTwoInts
client driven by `service_submit` plus its independently authorized observer.
Actual copied-source compilation and linking of that original service server
passed, as did all 26 local service/Future models and ten earlier topic
regressions. The first real builds exposed a missing byte helper and a model
response-length omission; reviewed corrections and all failed attempts are
preserved in the [dated checkpoint](../../../../documents/ros/sdk-compatibility-checkpoint-20261018.md).
Wasm production and native service delivery for this profile remain required.
Complete rclcpp APIs, native binary ABI and distribution/vendor acceptance need
further implementation and the full original suites.

The original official minimal_client/main.cpp is copied without any editing;
its complete execution/qualification remains **REQUIRED**. The early service/
Future increment refused95 at `wait_for_service(1s)`. The current separately
opt-in [C/C++ availability source contract](../../../../tests/ros_service_client_sdk/README.md)
implements `service_is_ready()` and finite `wait_for_service(duration)` with a
real independently granted `service.available` query owner and the optional
`uwvm_ros2_client_v1` imports. The original two-owner binding still refuses95;
the no-argument infinite overload also remains95. This is source implementation,
not evidence that the unchanged official client has run on the new route. Graph
rows or opening an alias do not prove service matching. The optional Gateway
query owns an extra retained native query client matched against the source;
it is not an attestation of the business path's same retained RMW client.
The explicit_ready_async_client.cc migration lesson remains separately named;
its first R is an external owner gate after actual native readiness evidence.

## Immutable official source and actual API scope

source-acquisition.json records the real downloaded files, exact commits, Git
blob SHA1s, byte extents and SHA256s. The examples commit is
07008852303f2a35a91c65d78046b274a35477ea (actual tree
cc1f17d44a88c740e4ed580be99ead013bcb1afd). The actual example_interfaces Jazzy
commit is 4ed406d1277df78be01a31005306d69e7592a993 and tree
e052b44c4ec48a966d71af9a5185c6549c67fc8e. The actual rclcpp Jazzy API commit is
53fa7aeea86e8ed484f9465fa14059faf13a71f1 and tree
8ba691f837b5c0368b0d6d8b3900e7b111d9257c. These are source revisions, not an
installed SDK version or a distribution compatibility result. Original package
metadata, main files, licenses, raw commit/tree observations and API headers
are retained under upstream/. No provenance row declares its own independent
review closed. The package peer/freeze/source lock are later independent steps.

Official API references were read from these actual immutable files. The server
uses Node::make_shared, create_service<AddTwoInts>, the three-shared-pointer
callback, spin, shutdown and PRId64 logging. The complete AddTwoInts request is
signed int64 a and b; the response is signed int64 sum. There is no Header,
string or omitted dynamic dependency in this .srv. The native Service identity
still includes the complete Request/Response/Event/ServiceEventInfo/Time graph;
a hash of three payload integers is never a native identity proof.

The rclcpp callback API normally supplies an actual rmw_request_id_t. This Guest
ABI exports only its own opaque gateway ticket, not the native requester GUID
or sequence. The exact selected official server ignores its header. It receives
a null shared pointer to a forward-declared opaque type; no fabricated GUID,
sequence, address or native object is created. Any application that dereferences
or inspects that native header is outside this source profile and must not be
qualified. Callback shapes beyond the selected three-pointer path are absent.

The adapter rejects signed a+b overflow90 before invoking the original callback.
The codec itself faithfully represents every signed i64 bit pattern. The
original example's unchecked C++ signed addition cannot define an overflow
result; no wrapping or saturating arithmetic is silently invented. The concrete
functional fixture is a41, b1, sum42. Independent codec tests also cover min/max
signed values without executing that undefined business arithmetic.

## Real SDK reuse and finite future contract

The only shared SDK extension is in roswasmcpp.hpp. ServiceClient and
ServiceServer retain their original Trigger/SetBool branches. An explicit
ServiceCodec specialization can supply complete response encode/decode; without
it the original Bool/message wire branch remains. Request/response scratch now
allocates before any corresponding capability open. A preallocated shared server
handler can be installed without allocation after the server opens. A documented
stable source-owner borrow allows the existing EventStream to observe the actual
ServiceClient; it does not open or grant an observer.

The AddTwoInts specialization implements real complete16-byte request and8-byte
response encodings, exact extents and transactional decodes. It is not a stub or
native serialization. No native RIHS, adapter identity, generated output hash or
Gateway registration is provided. The unbound catalog uses the exact acquired
.srv bytes. Root must export actual installed root AND component identities,
bind the complete source, generate with the real current producer and prove the
complete native graph and byte agreement using native/codec_contract.cc.

Client opens the real service.call source and a DIFFERENT event.observe alias,
then calls actual EventStream watch with correlation0. Each async_send_request
allocates/retains its result and fixed-table slot before exactly one real
service_submit. The returned nonzero u64 ID is the Runtime's opaque ID, including
UINT64_MAX. It is not narrowed to a signed native sequence or fabricated locally.
There is no synchronous service_call, timer thread or deferred synchronous RPC
under the async name.

The move-only FutureAndRequestId has a .future member and the get/valid forwarding
needed by the selected original source shape. It is a finite Guest future, not
std::future binary compatibility or its full blocking API: pending get refuses11
instead of blocking/pumping an implicit loop. get consumes a completed value or
throws the exact completed error; a second get refuses22. completion_flags keeps
actual external-effects/retirement metadata even after an error is consumed.
No wait/wait_until, general shared_future, promise, callback overload, native
request header, signed request-sequence API or broad executor overload is
advertised. Those remain explicit full-API work items.

The finite spin_until_future_complete retains the actual Node and snapshots its
existing entities. Existing timers receive the same actual already-granted Clock
reading as the original topic spin, rather than a fabricated zero timestamp. One selected Host step releases one bounded pass. Each client
poll consumes at most one actual observer record. Backend/source/instance/session
and the observer's complete queue sequence are validated by the current SDK.
The complete response decoder and a locally admitted correlation must match.
A per-request RESOURCE_ERROR completes only that request with its actual code;
a correlation0 or source-retired error/closed event completes all affected local
pending work. Explicit source-retired metadata also prevents any new submit
locally, using the actual recorded error before another Host call. The flags are preserved, not interpreted as cancellation of an
external robot/native effect. Unknown IDs, duplicate replies, foreign source,
wrong sequence or malformed bytes fail22 and cannot complete an unrelated future.

Host admission11, poll idle11, error90 required-size semantics, 107 and110 retain
their actual meanings. None triggers a replay, resize, reopen or synchronous
fallback. remove_pending_request marks a local125 tombstone. It does not issue
native cancellation, revoke an alias or release the slot until a late matching
terminal event or client closure. Even a matching response to such a local
tombstone is fully decoded before retirement: a short/trailing payload refuses22
without changing the locally recorded125 or pretending to cancel native work. It cannot remove a future belonging to another
Node/client. A finite spin budget exhaustion returns TIMEOUT with diagnostic110;
this does not pretend to implement a native infinite wait set or cancel an
already admitted effect. The explicit owner must still finish/close the lane.

## Authority, allocation and teardown

Selection::services defaults null. Old six-field topic Selection aggregate
initializers retain that default. No topic source/example or source acquisition
file is overwritten. The new ServiceBinding table maps exact node/service/type,
role, declared service QoS token and local pending capacity to independently
selected aliases. Source names never choose an endpoint or confer grants. The
Host must independently bind each alias to the actual native type, identity,
canonical identity, resource, effective native QoS, quotas and principal. A source
QoS token is not measured native QoS. Unknown type/QoS/row fails before opening.

The server owns separate advertise/respond capabilities. An INFO log owner is
prepared separately on its Node before the first callback. An absent or denied
log grant can therefore be the first13 under wholly empty authority, with no
service entity; a specific service-denial test must grant only the log role and
withhold the service roles. This distinction must appear in actual Host audits.
The logger remains Node-owned if later factory setup fails, and is closed by
Node teardown; it is not an untracked server owner. There is no ERROR-severity
alias mapping in this increment. RCLCPP_ERROR explicitly refuses95 instead of
silently using an INFO grant. The original client failure logging remains work.

Registry capacity, shared facade/callback ownership and request completion
storage are reserved before the corresponding effects. The stable Core/context
outlives every SDK borrow. Fresh shared Request/Response objects are allocated
before consuming each service request, so a callback-retained pointer is never
reused as a later request. Before consuming an event, the full service callback lease reserves at most
16x64 strong facade references from a fixed weak Node registry. Application
release of a final shared pointer during that callback therefore cannot bypass
the busy gate through a capability destructor. The references retire only after
the lease flag is restored. The group rejects cross-Node factories/shutdown,
nested spin, cold lazy clock setup and facade close/remove operations17; it always releases on
ordinary exception unwinding. It introduces no native thread. Unsupported raw
SDK escape usage still requires its documented borrow discipline.

Service closes respond then advertise through the existing checked pair. Client
closes observer then source, attempting both even after failure. Per-role cleanup
status arrays and aggregate attempted/failed/first cleanup error are retained.
Destructor cleanup is best effort and never replaces a primary exception;
107/110 close is not retried. The external owner must inspect global diagnostic,
per-resource and Host/Frame cleanup evidence; a killed Guest is not clean teardown.
No-exception Wasm abort/allocation exhaustion needs owned external cleanup and
cannot be claimed as in-process exception recovery. Actual asynchronous native
work may remain after a local tombstone or an error carrying effects-may-remain.

## Source-only preparation and native binding recipe

The native binding and independent-peer commands in this section remain
UNEXECUTED for this profile. Apply/review a coherent full source first, stage a new owned working source and actual SDK/tool
inputs, and run serially under Root's current actual cgroup/CPU/memory policy.
The native identity exporter is modern ROS2 only; it never downgrades to legacy
metadata if any modern root/component function is missing. The helper namespace
is byte-identical to the existing reviewed native_identity.cc; its specialized
main emits one valid complete Service inventory. Real support/hash/description
functions are warmed before actual SIGSTOP, followed by the explicit I gate.

```sh
cmake -S "$SOURCE/sdk/compat/rclcpp/services/native" -B "$OUT/native-export" \
  -DUWVM_SOURCE_ROOT="$SOURCE" -DUWVM_BUILD_BOUND_CONTRACT=OFF
cmake --build "$OUT/native-export" --target identity_export service_peer --parallel 1
# Root's original-pidfd owner runs identity_export, proves the actual stopped
# native maps and full SDK/MD/link closure, then sends I and saves actual JSON.
"$PYTHON" -B "$SOURCE/tests/ros_custom_types/bounded-v2/bind_catalog.py" \
  --template "$SOURCE/sdk/compat/rclcpp/services/catalogs/catalog.preparation.json" \
  --native-metadata "$ACTUAL_NATIVE_EXPORT" --output "$OUT/bound/catalog.json"
"$PYTHON" -B "$SOURCE/sdk/tools/custom_types.py" "$OUT/bound/catalog.json" \
  --out "$OUT/generated-add-two-ints"
cmake -S "$SOURCE/sdk/compat/rclcpp/services/native" -B "$OUT/native-contract" \
  -DUWVM_SOURCE_ROOT="$SOURCE" -DUWVM_BUILD_BOUND_CONTRACT=ON \
  -DUWVM_BOUND_CATALOG="$OUT/bound/catalog.json" \
  -DUWVM_BOUND_GENERATED="$OUT/generated-add-two-ints" \
  -DPython3_EXECUTABLE="$PYTHON"
cmake --build "$OUT/native-contract" --target codec_contract --parallel 1
```

Bound output parents must be fresh as required by the actual binder. Retain its
original export/template/definition copies/receipt and every generation.json
output. Reopen the actual complete Service protocol Root/Event/Time graph and
both native typed Request/Response schemas, not just root-hash text. The real
codec oracle lets the Gateway include the actual selected native_ros2.hpp after
its paired common manifest, with real installed headers. Its own held SIGSTOP/I
gate precedes contract output. Markers, authored
checks or a codec-only pass are not delivery. Older distributions require their
separate explicit complete native metadata workflow, not these modern targets.

Model build uses the real adapter and a clearly fake raw backend. It earns zero
native/ROS/official credit. The original server main is renamed only as a model
link symbol; its entire original file remains byte-exact:

```sh
"$CXX" -std=c++20 -O0 -UNDEBUG -MD -MF "$OUT/official-server.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/compat/rclcpp" \
  -I "$SOURCE/sdk/include" -I "$SOURCE/third-parties/fast_io/include" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"services/tests/model_binding.hpp"' \
  -Dmain=official_service_main -c "$SOURCE/sdk/compat/rclcpp/services/upstream/examples/rclcpp/services/minimal_service/main.cpp" -o "$OUT/server.o"
"$CXX" -std=c++20 -O0 -UNDEBUG -MD -MF "$OUT/model.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/compat/rclcpp" \
  -I "$SOURCE/sdk/include" -I "$SOURCE/third-parties/fast_io/include" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"services/tests/model_binding.hpp"' \
  "$SOURCE/sdk/compat/rclcpp/services/tests/service_contract.cc" "$OUT/server.o" -o "$OUT/service-model"
# Only the independently owned test lane may run the resulting model later.
```

Real Wasm builds use the actual selected WASI C++ sysroot/archive pair, full
validator/import inspection and original sources. No model header or fake backend
is allowed in a real artifact:

```sh
"$WASI_CXX" --target=wasm32-wasi --sysroot="$ACTUAL_WASI_SYSROOT" \
  -std=c++20 -O1 -fno-exceptions -fno-rtti -MD -MF "$OUT/service.d" \
  -I "$SOURCE/sdk/compat/rclcpp/include" -I "$SOURCE/sdk/compat/rclcpp" \
  -I "$SOURCE/sdk/include" -I "$SOURCE/third-parties/fast_io/include" \
  -DUWVM_RCLCPP_COMPAT_SELECTION_HEADER='"services/examples/selected_service_binding.hpp"' \
  "$SOURCE/sdk/compat/rclcpp/services/upstream/examples/rclcpp/services/minimal_service/main.cpp" \
  -Wl,-t -o "$OUT/official-service.wasm"
```

Build explicit_ready_async_client.cc with that same actual selection, sources
and SDK; do not label it official minimal_client. For each direction Root must
retain actual immutable source/artifact identity, whole SDK catalog, strict full
MD/link/ELF and map_files FD/hash evidence, the genuine Gateway custom Service
registry, actual module/principal grants, full independent native request and
response observations, all26 authored source-model outcomes when run, and normal
Frame finish/zero residual owners. Actual native peer M uses the real native
availability predicate. Any optional Guest query is separately admitted and is
not inferred from these peer credits. I creates
only the selected isolated endpoint; P never sends the same native request twice;
F requires full values and closure. All resource/deadline/output admissions and
pidfd ownership belong to a separately reviewed future execution owner.

## Required next work

remaining-work.json enumerates explicit incompatibilities and the actual public
header inventory from the acquired rclcpp tree. Its early availability gap is
now partly implemented by the separately opt-in source contract linked above:
zero/finite queries use a genuine native observation with independent authority,
source/epoch/type/QoS association and exact errno/output admission. Its physical
finite Host query is not scheduling credit or a fake AddTwoInts request. Default
bindings and infinite waits retain their explicit unsupported boundaries.
The actual optional-import artifact, Runtime/Gateway/native pipeline and whole
unchanged official client still require separate qualification; ERROR logging
and a genuine shutdown-aware infinite wait contract remain implementation work.
Callback-form client APIs, broad Future/executor/blocking behavior, native request
header queries, general QoS/options, parameters, graph/remaps, actions and all
other languages/vendors remain required independent work. This increment does
not reduce the user's broader SDK/test/vendor obligations.


## Current native source selection and paired headers

The standalone Native codec contract requires an actual fresh native-bound
catalog AND all generated outputs from the selected current parent/bounded
source. A previous delivered header or a copied RIHS literal is not enough:
current Service admission also needs the complete emitted root/Event/Time
`custom_native_interface_schema` trait. Keep all historical outputs unchanged.
Follow `sdk/custom-types/native_current/recipes.json` in an admitted private tree.
Generation is an explicit implementation prerequisite, never run by CMake.

For the contract configure command supply both
`-DUWVM_BOUND_CATALOG=/absolute/fresh-bound/catalog.json` and
`-DUWVM_BOUND_GENERATED=/absolute/fresh-generated`, plus the actual source root
and selected Python interpreter. The real CMake helper checks source/product
pairing, selects `manifest.hpp` and `native_ros2.hpp` together, supplies actual
project include roots and concrete installed ROS imported targets, then links
an EXECUTABLE modern Service/Request/Response probe. An outer STATIC_LIBRARY
try-compile setting cannot replace that link. A probe link is not an executed
Native contract. The actual bounded SIGSTOP/I owner protocol remains required.

`codec_contract.cc` intentionally does not include `native_ros2.hpp` first.
The Gateway loads the common manifest before its selected provider; predefining
the type table is still a hard error. Every root/schema/component/value assertion
remains, with additional comparisons against the exact paired common Service
and Request/Response descriptors. Existing unsupported/modern refusal paths
are preserved; no header, Native identity, dependency target or success is faked.
All source selection/build/probe/native commands in this successor are unrun.
