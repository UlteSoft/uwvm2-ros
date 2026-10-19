# Explicit native service-client availability

This cohort is a source-only implementation and unexecuted contract suite.
After independent full and focused source review, Root integrated the extension
and ran the source ABI association check plus eight ABI-preservation and nine
DAP-parser policies from copied files under `/tmp`; all passed. The C++ model,
selected-SDK native helper and actual Guest routes below remain unexecuted.
It adds one independent capability operation and two optional numeric import
namespaces. No compiler, test, generator, native client, Runtime, ROS server,
SSH command or Gateway was invoked by the author. Root must independently
review and run a coherent current-source generation before reporting results.
Old source/native reports remain historical; no retired ELF/object is required
or presumed available. Java, Python, C++, C# and other facade wait APIs must
adopt the actually integrated extension in separate reviewed work.

## Existing contract preservation

The complete `sdk/abi/abi-v1.json`, including its historical source hash, is
unchanged. The original two Host function tuples retain their exact textual
bytes, function order, numeric signatures and business/event/empty-request
semantics. This is a separate optional namespace, not an expansion or silent
reinterpretation of those81 business-v1 functions.

The existing ABI extractor now scopes its81 entries to the original two
module declarations. It accepts their unchanged historical authority hash only
when the separately closed optional catalog binds the current whole Host hash,
the unchanged complete old catalog hash, both unchanged declaration hashes,
and every numeric optional entry exactly. Missing/drifted association still
refuses; no source-hash check is dropped and no original catalog is rewritten.
`test_source_abi_preservation.py` authors eight real-filesystem source policies
for these associations. Neither that suite nor the extractor was called here.

The machine-readable extension catalog is
[`sdk/abi/service-client-v1.json`](../../sdk/abi/service-client-v1.json).
The C11/C++ raw contract is
[`sdk/include/roswasm/service_client_v1.h`](../../sdk/include/roswasm/service_client_v1.h).
It does not install a default service binding or select an SDK language.
Declaring either backend's imports grants nothing and creates no ROS entity.
A caller may explicitly import only its selected optional namespace and use
actual handles obtained through an independently admitted existing session;
this extension intentionally contains no open/grant/management import.

## Fixed numeric ABI

Each selected module, `uwvm_ros1_client_v1` or `uwvm_ros2_client_v1`, has exactly:

| Import | Parameters | Result/output on success |
|---|---|---|
| `abi_version` | none | i32 version1; no output or permission |
| `features` | availability:i64, source:i64, memory:i32, out8:i64 | i32 errno; exactly8 LE feature-bit bytes |
| `service_available` | availability:i64, source:i64, wait_ns:i64, memory:i32, out1:i64 | i32 errno; exactly1 canonical bool byte0/1 |

The first two values are genuine opaque full64-bit handles, not aliases,
resource names, guessed Runtime IDs or numeric native pointers. There is no
query length-slot, request body, arbitrary command selector or Guest retirement
function. Both outputs remain untouched on every nonzero return. Host verifies
the entire fixed memory extent before native effects and checks the live
extent again through the ordinary runtime copy primitive. It never returns a
partial result. Signed negative/infinite facade durations must be refused,
not converted or clamped into a wait; the raw u64 safety ceiling is60e9ns.
A zero wait still performs one genuine native query. Positive waits use a
finite explicit grant and include native setup/query overhead. A false answer
is a completed native observation; deadline expiry is110, never false success.
A true answer is a snapshot, not a future call or remote-peer-authenticity promise.

Features are exact finite masks: ROS1=11 (typed query, finite wait, typed MD5
header); ROS2=7 (typed query, finite wait, actual client QoS). Zero is reserved
as an explicit no-feature response; it is not a default answer on error.
Missing new Host imports fail linking. Old Gateways cannot attest the new
manifest/operation; no old reply, graph name or missing feature becomes ready.
The new default virtual adapter refuses95, and actual absent API/unsupported
address cases refuse95. Other authentication/manifest/transport failures retain
their truthful original errno rather than being relabeled native unsupported.

## Independent authority and quotas

A trusted target explicitly grants `service.available` under its own alias.
It must name one distinct present `service.call` alias in
`service-source-alias`. Both grants have identical canonical service resource,
type, actual trusted native identity, custom adapter-codec identity presence
and value, and all eight ROS2 QoS values. Source call authority alone is13 for
query; query authority alone cannot send a request or consume a completion.
Core CLI validation and the independent C++17 Gateway configuration parser
both enforce this pair. Existing native type/complete-schema qualifiers run
on the original call grant, not on a fabricated replacement capability.

Every new grant explicitly supplies `timeout-ms`, `max-wait-ns`,
`max-native-clients`, `max-binding-history`, `max-inflight`,
`max-request-bytes`, `max-response-bytes`, rate, burst, bandwidth and audit
limits, plus all eight ROS2 QoS fields/custom codec where applicable.
`max-inflight` is exactly1, so the current manager also admits one open
availability handle per alias. Request and response quotas are exactly64, the
trusted tagged metadata envelope; they do not authorize a64-byte Service body.
Bandwidth accounts both request and response envelopes, independently in Host
and Gateway. A small explicit bandwidth can therefore reject output11 after a
native observation; it does not grant more bandwidth or weaken failure atomicity.
A busy existing Host backend returns11 immediately instead of waiting behind
a long business call; it never produces a false readiness value.
Source `service.call` contributes a second live authorization lease but query
never spends its business-request operation/byte quota.

Wait grants are positive and at most60e9ns. Their ceil-millisecond value plus1
must fit the explicit operation timeout (at most60000ms), reserving truthful
false-result headroom. Native clients are bounded1..1024 and no more than the
explicit lifetime history bound1..65536. The history also has a fixed65536-row
whole-session safety ceiling. Tombstones never evict or regain lifetime credit.
Exhaustion11 precedes native entity creation. Closing does not erase history;
reopening requires a fresh genuine capability generation and remaining credit.

## Full binding, trust and retirement

The new64-byte wire envelope carries fixed version/command/backend tags,
private retirement scope/reason, full availability and source handles, actual
capability-instance label, true Runtime lifetime generation, wait/result, and
sixteen reserved zero bytes. Every offset, tag, reserved byte, generation,
command-specific range and reply echo is checked. Native session credentials
remain in the already authenticated existing IPC frame; the session-local
ledger additionally stamps the actual random native session for each binding.
Different source/instance/runtime-generation/native-session associations,
known retired full handles and cross-role full-handle reuse refuse. Same alias
alone cannot rebind a stale association.

Host identity comes only from the existing trusted runtime caller API, and both
capability leases survive the query/copy. The backend lock serializes native
query, close, revoke and management retirement. Gateway validates its own
immutable configured pair and known tombstones. Its UID/PID/session-authenticated
transport is a **Host trust boundary**: Gateway does not claim independent
visibility into unseen Host handles or authenticate Guest-supplied generations.
Guest never directly constructs this metadata, socket frame or pairing.

True Core close/revoke retires the local handle/alias **first**. A private
Host-only IPC operation then retires matching native ownership, independent of
rate/bandwidth/revocation. There is no public private-command import. Source
close still propagates through historical rows after every availability handle
closed. Availability close affects its full handle only; ordinary alias close
does not revoke a future generation. Alias revoke is permanent. A private
cleanup failure closes the entire native session and locally retires its rows;
no old handle transparently rebinds another session. Only a fresh explicit
availability handle, with both real source authorities still live, may bind a
later session. Management retirement seals the layer. Native registries own
copied values/shared native owners; no raw authorization/Guest/native pointer
survives the lease or session. Gateway session exit explicitly clears query clients before waiting for
unrelated asynchronous Service tasks; existing Gateway destruction owns remaining
cleanup after transport loss; its trusted external process owner remains the
hard bound for a stuck native client library/finalizer.

## Native implementations and honest limitations

ROS2 creates a separate retained real `rclcpp::Client<Service>` under the new
observation authority, with its request publisher/response subscription. It is
not the old `service.call` ephemeral client and does not issue a request,
callback, event subscription or executor wait. Actual selected RMW controls
participant/entity mapping; no fixed participant count is asserted. Copied ownership and the map node are reserved before native creation; a
transaction erases the pending slot and any created client on refusal/exception,
then best-effort notifies the same still-live node so executor wait-set references
can retire. The transaction is local to that observe call and cannot retain its
node argument after return. Both actual
request-publisher and response-subscription QoS getters must exist and match
all configured values. The real `service_is_ready` API is expression-probed;
missing APIs are95 before creation. Readiness itself invokes actual rcl/rmw
service-server availability via the selected real client, not graph-name lists.
Elapsed-time checks after setup/readiness calls do not preempt synchronous
RMW internals. Host IPC timeout bounds Guest waiting; a separate pidfd-owned
Gateway process deadline and actual cleanup proof are still mandatory.
Retirement erases owned clients and wakes the existing executor so wait-set
references can retire. No extra observation thread/process is created.

ROS1 performs a real Master `lookupService` on an owned nonblocking socket,
then a typed TCPROS header handshake with actual official Service MD5/DataType.
It closes before sending **any** service request length/body. DNS resolution,
XMLRPC connect/send/read, TCPROS connect/send/read share one absolute query
budget. Only numeric IPv4/IPv6 and explicit localhost mapping are implemented;
unknown hostnames return95 without blocking `getaddrinfo`. Response XML/header
bytes, nesting and field counts are bounded before native parsing. Arbitrary
URI methods, XML DTD/entities/commands, a wildcard-MD5 name-only probe, and a
business call with an empty body are not substitutes. The exact rosmaster
`lookupService` absence response (-1, "no provider", empty URI) is false; internal
status0 is5, other errors/faults/malformed responses are22. No failure status is
a fabricated success. Unknown Master implementations need an independently
reviewed absence contract. HTTP short header/body and bounded timeout also fail.

`GAPS.md` lists the remaining hostname, selected-SDK, facade and actual test
work. In particular this is **not complete ROS1 address compatibility** and
neither source presence nor these finite API guards proves26-distribution,
all19-vendor or native/Guest interoperability.

## Authenticated read-only debugger linkage

`info ros services` includes `service.available` by its existing operation
family. `trace ros function=client.features op=service.available` and
`function=client.service_available` use the existing bounded filters. Trace
rows add configured source alias, local successful-connect ordinal and, only
for successful feature returns, actual validated feature bits. This ordinal
is not an IPC credential; no raw handle, native pointer or session token enters
the debugger DTO. Session rows show lifetime binding count and that ordinal
only after bindings exist. Native state is never refreshed by a debugger read.
The strict DAP parser admits only these complete optional metadata groups with
correct operation/function/row/backend/mask associations. Every other unknown
field/mutation selector still refuses. Original rows render/parse unchanged.
Gateway audit additionally records the configured source alias beside its
existing authenticated session/operation/alias. All audit quotas remain active.

## Unexecuted verification inputs

* `wire_history_contract.cc`: genuine wire/history header model; synthetic
  labels are explicitly local data, not future Runtime identities.
* `core_contract.cc`: actual capability manager, independent authority, quotas,
  stale/foreign handles, source revoke and real concurrent lease-drain model.
* `server_contract.cc`: real AF_UNIX peer credentials, immutable manifest,
  protocol and private cleanup with an explicitly fake native adapter. Zero ROS.
* `debug_contract.cc`: actual parser/fast_io renderer, optional fields and
  escaped maximum-page bound; zero debugger/native interaction.
* `test_debug_protocol.py::cases(dap)`: inject the actual admitted DAP parser;
  optional metadata does not bypass strict closed schema/fences. Unexecuted.
* `native_contract.cc`: actual selected SDK Trigger helper/quiet Node/server,
  real installed XmlRpc parsing and owned socket status/fault/short-header/body/timeout
  boundary cases (ROS1), then actual absence→presence→removal and callback counter0
  across metadata queries,
  plus exactly one separately explicit trusted native business-call control.
  It requires an unused canonical service name and real selected SDK/network;
  unsupported quiet/identity/QoS APIs refuse95. It is **not** a Wasm/Host test.
* `guest.c`: actual optional ABI calls using real Host-opened handles; default
  denial, a separate call-only authority refusal, invalid output/memory boundaries,
  wrong-operation unchanged output, a Root-selected actual readiness
  expectation, and source-first close/stale refusal. Requires a newly built
  coherent Runtime/Gateway/native fixture; never uses old retired products.
* `check_service_client_imports.py`: actual binary signatures only. Separate
  entry leaves the original inspector/81 catalog unchanged; full validation
  and actual Guest/ROS execution remain independent mandatory gates.

Root owns all invocations. Build models and native SDK targets in separate
fresh physical outputs with explicitly selected compilers/standard/toolchain
and native feature inputs; `CMakeLists.txt` defaults both groups off. Keep
serial `-j1`, the original64GiB/swap0 controller, real ancestor/backing/reserve
admission, pidfd-owned process cleanup and whole source/SDK pins. Suggested
individual ceilings are8GiB/600s for compilation,512MiB/20s for local models,
and1.5GiB/60s for a native witness, under a separately reviewed finite Root
owner. These are required future invocation limits, not observed measurements
or a ready execution owner. DAP models must run under a current-MF injected
Frame outside native/ROS credit. Actual native setup/queries have library
synchronous calls; the trusted outer owned process bound remains necessary.

## Exception configuration

Shared new wire/history/Host code preserves the Worker's existing optional
`fno-exceptions` build path; exception handlers are compiled only when
`__cpp_exceptions` is present. Exception-enabled allocation failures return12
where caught. A no-exception Worker keeps its existing process allocation-
failure policy; source presence does not promise a recoverable heap failure
that its standard library cannot provide. Native Gateways keep their existing
exception-enabled client-library/build contract. Both actual compiler modes
need source-current compilation by Root; neither was compiled here.

## Deferred native reuse optimization

The existing business call still creates its own ephemeral native client. The
new observation client has separate authority and owned retained lifetime. A
future real handle-scoped reuse optimization must prove matching actual source
identity/QoS/generation, cancellation, lease retirement and executor behavior,
then use actual VTune/latency evidence. This source cohort claims no measured
speedup and does not silently merge the two native owners.

## r2 source-review corrections

The independent focused review found a missing rollback wake after an attempted
ROS2 client creation; rollback now matches ordinary native retire/clear with a
best-effort executor notification. It also found that the independent Gateway
parser relied on a broader generic integer ceiling. The new operation now
explicitly requires timeout-ms<=60000, preserving all old operation validation.
The server contract authors both-backend60000 acceptance/60001 refusal and the
ceil-wait-plus-one relation boundaries. All are source-only and unexecuted.
The complete r1 package and its original raw ABI/Host/catalog remain preserved.
