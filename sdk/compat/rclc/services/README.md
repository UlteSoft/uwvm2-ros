# Additive C/rclc service source profile

This opt-in Guest source adapter implements real asynchronous service admission,
observer completion and two-pointer service callbacks for the complete
AddTwoInts, Trigger and SetBool records. It is an authored source candidate. No
candidate, generator, compiler, model, Wasm module, SDK, native peer or ROS case
was executed while preparing it. Existing String/topic/timer source files and
examples stay byte-identical except the two documented additive core hook files.
This increment does not implement all of rcl, rclc, rmw or every upstream test.

## Exact supported source and record family

The immutable official rclc source is commit
45ef3de567588652c7f05017a91ea29921dd6862; rcl is
18d96c9c2b98e616324ae9a2c7701dd0b0886ff0; rosidl is
85fa592b698b0f665e3120f48fac0d35e2f7d8a4. Complete downloaded API files, source
licenses, actual commit/tree responses and all eleven acquired upstream test
translation units are preserved under upstream/. source-acquisition.json binds
actual Git blobs and original schema bytes. No branch label, unrelated digest,
Guest token or local test count is a native type identity.

The unchanged original example_service_node.c and example_client_node.c are
preserved whole, including the original client comment/include line and both
mains' literal node name add_twoints_client_rclc. They select /addtwoints. The
original server adds signed a+b; the adapter rejects a request whose mathematical
sum would overflow int64 before invoking that callback. It does not edit the
business source or rely on undefined arithmetic. The original printf narrows
request operands to int; an original-main demonstration must use24 and42 and
still require an independent native observer, not printf as its field oracle.
The checked lesson uses the entire int64 values and records66.

The public Guest structs retain all actual fields: AddTwoInts Request.a/b and
Response.sum; Trigger empty Request storage placeholder plus Response.success
and message; SetBool Request.data plus Response.success and message. The uint8
placeholder exists because the actual IDL adapter inserts storage for an empty
C struct. It is never serialized. All six records provide init/fini/create/
destroy/copy/are_equal and complete Sequence lifecycle APIs. These are authored
source-compatible functions, not claimed generated artifacts. Dynamic text and
Sequence copying build a full replacement before releasing the destination.
Strings use the existing real SDK String lifecycle, include a trailing C NUL,
and preserve embedded UTF8 NUL bytes using explicit size; C strcmp is not the
wire validator. Sequence capacity denotes initialized elements. Caller-owned
message objects must remain initialized and address-stable for registration.

The local type-support tokens retain the actual public struct/function
spellings and complete modern service_type_support_struct shape. Their
identifier explicitly says Guest source token, not native type support. No
get_type_hash/description pointer, RMW GUID, timestamp or native sequence is
invented. The Host/Gateway still independently qualifies the complete actual
native Service graph and canonical codec before granting any alias.

## Selection, authority and acquisition

uwvm_rclc_selection gains one trailing optional services pointer. NULL preserves
the original selection validity and topic/timer control paths. Each Service row
selects exact node, namespace, service spelling, type, role, complete QoS,
independent aliases, maximum canonical wire bytes and finite lifetime requests.
Names are exact source bindings, not implicit ROS remapping or a graph query.
The supplied example accepts default services QoS and the explicit best-effort
variant; other QoS is95 before opens. Native effective QoS remains separately
verified by the actual SDK/manifest. The initial source profile is ROS2 only.

Client rows require independent service.call and event.observe grants. Server
rows require service.advertise and service.respond grants. Aliases cannot collide
with each other, selected topics or the independent steady clock. Possessing a
backend, a source name, a type token or an observer does not confer authority.
The recipe starts with rights[] and no grants. Default denial is successful only
when the actual first source open returns13 and no live owner remains; an
unsupported95, missing ABI or logger/clock denial is reported by its actual role.
Nothing silently grants a clock, thread, graph query, network or native session.

The existing256 Context owner slots and admitted-address registry are reused.
State, pending slots, both bounded buffers, diagnostic storage, allocator and
two owner slots are reserved before the first corresponding native open. Custom
allocator callbacks run with the whole Context leased. Client acquisition opens
source then observer and watches the same source at correlation0 before submit;
server acquisition opens advertise then respond and activates advertise. A
failed step closes every entered owner in reverse order, once, preserving the
operation error and every cleanup status independently. Failed107/110 cleanup
is not retried or hidden. All new factories, fini, shutdown, executor mutation
and request removal refuse17 while any Context callback/driver pass is active.
Supported operations through retained owners remain usable. Raw SDK escape
calls still require the SDK's documented lifetime discipline.

## Actual async, complete records and terminal behavior

rcl_send_request encodes one complete canonical request into existing scratch,
then makes exactly one actual service_submit call through the existing event
backend. It is not service_call. Admission success is not completion. The
independently owned observer carries the Runtime's full64-bit opaque request
ID; the signed C sequence output preserves all64 bits with memcpy, including
UINT64_MAX. That output is NOT rmw_request_id_t.sequence_number. Backend errors
11/90/107/110 do not trigger resize, reopen, replay or synchronous fallback.
An explicit later application call after admission11 is a new decision, not an
automatic retry; ambiguous/terminal admission errors poison further submission.

Each executor pass performs the existing explicit driver step and original
registration-order read-then-dispatch phases. At most one record per service
source is consumed per pass. The existing SDK validates complete UWRE header64,
backend/source/queue sequence/instance/session/kind/correlation/flags and exact
extent. The response must match a pending local ID and its full typed decoder.
AddTwoInts is exactly two8-byte request fields and one8-byte response; Trigger
request is0bytes; SetBool request is a strict1byte boolean. Both text responses
are canonical boolean +u32 UTF8 byte length +entire String. Short, trailing,
invalid boolean/UTF8, over-bound, unknown ID, duplicate or foreign records fail.
No consumed bytes are replayed. Authored fixtures require every actual field.
Status3 records a complete decoded terminal response. If a later source gather
fails in the same read-then-dispatch pass, no callbacks from that failed pass
are replayed; the earlier decoded record remains terminal and its caller response
storage is retained. The caller must inspect the pass error and close the lane;
status3 alone is not proof the callback ran.

Local remove_pending_request records125 but does not cancel native work or
release its historical ID. A matching late response still receives the complete
decoder before it is discarded without touching expired user response storage.
Its original125 stays; duplicates thereafter refuse22. Request slots/IDs are
never reused and total submissions are bounded across the owner's lifetime,
not merely concurrent requests. A per-request RESOURCE_ERROR terminalizes the
matched pending request with the real code. Correlation0 or retired-source
metadata terminalizes all pending work and prevents new admission. CLOSED/ERROR
body/flag checks come from the actual SDK; effects-may-remain is not permission
to retry or a claim of native cancellation.

Server poll receives the actual8-byte Gateway-owned ticket followed by the
complete typed request. The ticket is retained privately and never exposed as
a forged native RMW header. On a successful callback, exactly one response is
attempted using the separately selected respond owner, ticket and complete
response codec. Successful RPC empty output must write the real length slot0;
the model explicitly does so, matching the actual SDK/Host contract. A C callback
can report an allocation/field failure with uwvm_rclc_service_callback_error;
that records the primary and prevents a response. There is no exception or
longjmp recovery promise for arbitrary C application callbacks. A request
consumed before a later gather failure is not silently retried on the next pass;
close is required. All initialized owners are independently retired at fini.

## Genuine sleep and unsupported metadata/availability

The original client calls void rclc_sleep_ms(2000), not wait_for_service. This
profile performs genuine elapsed waiting only with exactly one live selected
Context, an explicit steady-clock alias, finite maximum_sleep_ns and
maximum_sleep_steps, and a bounded caller driver. It reads actual granted steady
time before/after every step; R stdin credits are pacing, not nanoseconds. It
rejects time rollback, overflow, missing rights and exhausted budgets. Because
the official API is void, a failure is retained in Context.uwvm_sleep_error and
blocks the next request before any submit. Multiple live Contexts fail visibly
rather than selecting an arbitrary grant. No WASI/system clock, sleep no-op,
ambient thread or busy-spin duration is synthesized. A real owner must bound
blocking driver I/O independently. This elapsed delay does NOT prove DDS matching.

Native rmw_request_id_t.writer_guid, native sequence and service timestamps are
absent from the existing business ABI. rcl_take_request/response(_with_info),
rcl_send_response with an RMW header and request-ID callback registrations
therefore refuse95 before poll or output mutation. The original two-pointer
service/client callbacks are supported. Genuine matched native availability is
also95 here until the independently reviewed optional client namespace and C
provider are coherently integrated; no graph list, elapsed2s or source alias is
used as a substitute. The separate current optional availability candidate is
not imported, duplicated or forecast as implemented by this cohort.

## Unexecuted build, binding and complete native recipe

All following commands are instructions for a future independently admitted
Root owner after coherent integration/source freeze. They were not run. SOURCE,
OUT, CC, CXX, WASI_CC and PYTHON must be actual selected physical inputs with
whole tool/SDK inventories, strict current source/MD/link/ELF/maps and resource
admission. Use fresh OUT, serial jobs and the actual original cgroup/CPU/memory/
backing gates. Every command has a whole-tree deadline, retained pidfd ownership,
all raw stdout/stderr/MD, bounded outputs and independent cleanup/Frame finish.
The commands do not authorize the author or a caller to bypass those gates.

```sh
INC="$SOURCE/sdk/compat/rclc/include"
SDK="$SOURCE/sdk/include"
BASE="$SOURCE/sdk/compat/rclc"
LESSON="$BASE/services"
# Local bridge model: genuine adapter, explicitly fake raw transport. No ROS.
"$CC" -std=c11 -O0 -UNDEBUG -MD -MF "$OUT/model.d" -I"$INC" -I"$SDK" \
  "$BASE/src/compat.c" "$LESSON/tests/contract.c" -o "$OUT/service-model"
# Actual owner may subsequently run service-model; authored model counts alone
# have no native, Guest, official-suite or ROS credit.
# Compile the adapter and selection ONCE into separate Wasm objects.
"$WASI_CC" -std=c11 -O0 -MD -MF "$OUT/adapter.d" -I"$INC" -I"$SDK" \
  -c "$BASE/src/compat.c" -o "$OUT/adapter.o"
"$WASI_CC" -std=c11 -O0 -MD -MF "$OUT/selection.d" -I"$INC" -I"$SDK" \
  -c "$LESSON/examples/selection.c" -o "$OUT/selection.o"
"$WASI_CC" -std=c11 -O0 -MD -MF "$OUT/checked.d" -I"$INC" -I"$SDK" \
  -c "$LESSON/examples/checked_service.c" -o "$OUT/checked.o"
"$WASI_CC" "$OUT/adapter.o" "$OUT/selection.o" "$OUT/checked.o" \
  -o "$OUT/checked-services.wasm"
# Each original main is compiled unmodified as its own separate command module.
"$WASI_CC" -std=c11 -O0 -MD -MF "$OUT/original-server.d" -I"$INC" -I"$SDK" \
  -c "$LESSON/upstream/rclc_examples/src/example_service_node.c" -o "$OUT/original-server.o"
"$WASI_CC" "$OUT/adapter.o" "$OUT/selection.o" "$OUT/original-server.o" \
  -o "$OUT/original-server.wasm"
"$WASI_CC" -std=c11 -O0 -MD -MF "$OUT/original-client.d" -I"$INC" -I"$SDK" \
  -c "$LESSON/upstream/rclc_examples/src/example_client_node.c" -o "$OUT/original-client.o"
"$WASI_CC" "$OUT/adapter.o" "$OUT/selection.o" "$OUT/original-client.o" \
  -o "$OUT/original-client.wasm"
```

Do not force-include selected_service_binding.h into every translation unit:
that would define the selector repeatedly. selection.c is its single owner.
Validate the entire actual Wasm and every observed import/signature/entry with
the current ABI validator. A backend table or an authored header is no such
proof. Preserve all previous topic String/timer sources and rerun their actual
local regression group independently after integration; this candidate claims
no inherited actual result for changed adapter bytes.

The new unbound catalog contains ONLY example_interfaces/srv/AddTwoInts. The
current builtin Trigger/SetBool native identities/codecs are reused; do not
shadow-register std_srvs as custom. For genuine AddTwoInts native binding reuse
the already integrated sdk/compat/rclcpp/services/native identity_export and
codec_contract targets and its complete source recipe. That real exporter warms
modern Service root/Request/Response/Event/Time metadata before stopped-image I;
missing modern root/component APIs must refuse, never legacy auto-downgrade.

```sh
cmake -S "$SOURCE/sdk/compat/rclcpp/services/native" -B "$OUT/export" \
  -DUWVM_SOURCE_ROOT="$SOURCE" -DUWVM_BUILD_BOUND_CONTRACT=OFF
cmake --build "$OUT/export" --target identity_export --parallel 1
# An actual admitted owner runs identity_export, proves stopped native FD/maps,
# all SDK/MD/ELF inputs and sends its I gate; retain the exact resulting JSON.
"$PYTHON" -B "$SOURCE/tests/ros_custom_types/bounded-v2/bind_catalog.py" \
  --template "$LESSON/catalogs/catalog.preparation.json" \
  --native-metadata "$ACTUAL_NATIVE_EXPORT" --output "$OUT/bound/catalog.json"
"$PYTHON" -B "$SOURCE/sdk/tools/custom_types.py" "$OUT/bound/catalog.json" \
  --out "$OUT/generated-add-two-ints"
cmake -S "$SOURCE/sdk/compat/rclcpp/services/native" -B "$OUT/codec" \
  -DUWVM_SOURCE_ROOT="$SOURCE" -DUWVM_BUILD_BOUND_CONTRACT=ON \
  -DUWVM_BOUND_GENERATED="$OUT/generated-add-two-ints"
cmake --build "$OUT/codec" --target codec_contract --parallel 1
# Native C/rclc independent peer: REAL installed headers and generated C types.
cmake -S "$LESSON/native" -B "$OUT/native"
cmake --build "$OUT/native" --target rclc_service_native_peer --parallel 1
```

The native C peer must never include sdk/compat/rclc/include or link compat.c or
services.inc. It uses actual installed rcl/rclc and all three generated Service
types; full native MD must prove that separation. It constructs every entity
and prepares its actual executor before SIGSTOP. The external owner proves its
actual selected SDK/source/definition/typed native identities, whole mappings
and same-birth executable FD before SIGCONT and I then M. In client mode it
queries actual native rcl_service_server_is_available under bounded native
spins before a single request per Service. Server mode observes every request
field and emits all complete response fields. After real three-family full-value
checks, owner P permits the observation row, F/C permit final close stages. EOF,
unknown gate or any primary/cleanup error fails. Native counters/markers do not
prove Guest exchange. All native resources and all initialized messages are
independently attempted once at teardown, with primary/cleanup/entered slots
recorded separately. Root must compare the actual checked Guest and native raw
fields in both directions, default denial, all actual ownership/normal finish,
source/SDK before/after preservation and authority audits. No expected hash,
RIHS, codec ID, future ELF, future boot or success receipt is baked into this
source. Generate/bind outputs only from actual observed full native inputs.

The original upstream mains omit executor/support fini and have early-return
macros. Their exact bytes remain the source-compatibility subjects. A process
exit does not retroactively prove normal Context teardown: the external owner
must retire the process/native Session independently and preserve that narrower
scope. checked_service.c is the separate complete-cleanup lesson; it does not
replace the originals' official source or hide their missing fini calls.

## Required work and honest test boundary

remaining-work.json and upstream-test-inventory.json keep every unimplemented
API/official test family required. All eleven actual original test units are
retained; none was compiled or run here. They depend on broader test_msgs,
Actions, native wait sets and field layouts outside this profile, and are not
silently retagged as local model tests. The local bridge models exercise
allocation/rollback, group leases, all64-bit tickets, complete/tombstone/error
records, typed server fields, genuine-clock-interface failure, request lifetime
bounds and every independent cleanup path explicitly covered by their source.
They do not emulate a native rmw GUID, SDK identity, availability, middleware
matching, ROS delivery or RMW ABI. Actual counts remain0 until a separately
admitted owner writes new raw results. Java/C#/Python/QuickJS/MoonBit and the
broader rcl/rclc surface remain separate required work, not implied by this
three-Service C family.
