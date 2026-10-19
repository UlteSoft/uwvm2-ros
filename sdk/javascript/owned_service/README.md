# Typed JavaScript owned ROS2 Services

This optional SDK implements the genuine twelve-function
`uwvm_ros2_rmw_srv_v1` channel. It never calls the legacy `service_call`,
`service_submit` or `event_poll` functions for these endpoints. That distinction
matters for the complete default-node control profile: its legacy business route
returns status95. Existing business ABI81 and optional client ABI3 are unchanged.

The implementation is source only. The new contracts, generator, C bridge,
JavaScript interpreter, final Wasm, Runtime and Native ROS interoperability still
need their separate unified execution. A local injected binding is a model, not
permission or Native evidence. ROS1 Services continue to use their separate
existing SDK and transport; this optional raw12 namespace is explicitly ROS2.

## Begin at the actual import boundary

Read [the builtin contract](../guest/OWNED_SERVICE_NATIVE.md) and
[the producer contract](../guest/OWNED_RMW_SERVICE.md) first. The closed builtin
`roswasm:rmw_service_native` exports one synchronous function:

```js
invoke(index, scalarArgs, inputUint8Array, outputUint8Array) // unsigned i32
```

The exact index order is `abi_version`, `client_activate`, `service_activate`,
`endpoint_inspect`, `client_submit`, `client_abandon`, `service_respond`,
`endpoint_peek`, `endpoint_commit`, `endpoint_cancel`, `client_available`,
`endpoint_retire`. Index0 returns ABI version1. All other functions return a
status, where0 is success. The C builtin owns memory zero, all offsets and bounded
scratch. JavaScript passes no address. Input/output may alias at this low-level
boundary because C captures input before writing output; the typed SDK uses
separate private buffers. Nonzero status leaves output unchanged.

Enable the builtin explicitly through the producer's backend2/owned-Service
catalog gate. Select the actual `sdk/abi/rmw-service-v1.json`, its complete SHA256,
actual authority/raw-header sources, original business imports and all twelve
optional Wasm signatures. The exact producer checks and arguments are documented
in the linked guide. Merely adding an allowed module name is insufficient.
Inspect the final Wasm's actual import signatures with that guide's checker.
Neither producer selection nor importing a module grants ROS authority.

The default Runtime policy grants no ROS alias, native networking, filesystem,
process, debug, update or Host-control authority. Open each alias only after the
user explicitly configured its command-line permission. A client requires its
separate `service.call` and `event.observe` aliases; a server requires separate
`service.advertise` and `service.respond` aliases. Availability needs another
`service.available` grant. All selected aliases must match the real same-session
source/peer and expected scope. Runtime checks those facts independently.

## Generate the complete record layer

Use the explicit records-only operator described in
[the managed record guide](../../rmw_wasm/default_control/managed/README.md).
Keep the original eight C/C++ products and `generation.json` unchanged. Supply
the captured-source SHA pair and the actual matching canonical/proof products.
The separate output contains `bounded_records.mjs`, its full Service root facts
and `generation.control-managed.json`. No generator runs by importing this SDK.

The full32 record set preserves all original field names, constants, defaults,
request/response graphs and complete64-bit values. Original canonical identities
are the entire `UWVMROS1_` plus64 lowercase hexadecimal characters. A Service root
identity is distinct from its Request and Response identities. Native identities
are copied only when actually supplied; an absent Native identity stays null.
The actual Runtime still validates native type, canonical codec and grants.

`TypeSource.raw_file_contents` is an owned `Uint8Array`, with its complete6162-byte
selected bound. Embedded NUL and invalid UTF8 are valid there. Other strings are
strict UTF8 without embedded NUL or lone UTF16 surrogates. The complete
TypeDescription/GetTypeDescription response keeps its32KiB aggregate bound in
addition to every nested selected capacity. Code22 means malformed input or a
field refusal; code90 means an aggregate/transport quota refusal. A logical wire
bound is not a JavaScript heap, C stack, DDS queue or process RSS measurement.

## Wrap a genuine pair and use typed records

A Wasm entry imports its selected generated namespace, the existing
`createGuestSession()`, and the optional builtin. It then constructs:

```js
import * as native from 'roswasm:rmw_service_native';
import { OwnedServiceBridge } from './bridge.mjs';
import { OwnedClient } from './owned.mjs';
import { bindControlServices } from './control.mjs';

const definitions = bindControlServices(records); // actual generated namespace
const endpoint = new OwnedClient(new OwnedServiceBridge(native), call, observer,
  definitions.getTypeDescription, {
    ownership: 'owned', availability: query, historyEntries: 64,
    expectedQos: [expectedRequestProfile80, expectedResponseProfile80]
  });
try {
  const activation = endpoint.activate();
  if (activation !== 0) return activation;
  const ready = endpoint.available(1000000000n); // actual finite Native wait
  if (ready.status !== 0 || !ready.available) return ready.status || 11;
  const admission = endpoint.submit(request); // original complete typed value
  if (admission.status !== 0) return admission.status;
  // The caller's explicit executor/supervisor later calls endpoint.take().
  // Call abandon(admission.ticket) if the caller stops waiting.
} finally {
  const cleanup = endpoint.close();
  // Preserve endpoint.facts(), including primary and every cleanup outcome.
}
```

The two expected QoS values are independently selected exact normalized
protocol80-byte profiles, not receipt bytes discovered by activating this same
endpoint. The SDK validates/copies them before activation and compares them with
the actual same-object request/response QoS in UMRE256. Every inspection checks the
full original receipt, including Native incarnation, binding, GID tail, QoS and
reserved bytes. Only the inspection operation byte may differ. Receipt GID and
Native Service request writer GUID are different fields; no conversion is made.

Constructors allocate finite lifetime request history, cleanup rows and every
receipt/packet/ticket buffer before Native activation. `historyEntries` is
explicit in[1,65536], is never reset, and counts every client submit attempt,
including status11. It is separate from Native QoS history and max-inflight.
No timer, implicit spinning, hidden worker, Promise job or automatic resend is
created. `take()` polls one record; status11 may indicate empty or rate limited.
The application/supervisor owns finite scheduling and external process deadlines.

`bindControlServices(records)` binds all seven roots: DescribeParameters,
GetParameterTypes, GetParameters, ListParameters, SetParameters,
SetParametersAtomically and GetTypeDescription. For another generated Service,
use `serviceDefinition(identity, requestCodec, responseCodec)` and pass its result
to `OwnedClient` or `OwnedServer`. The factory captures codec delegates once.

## Ownership, delivery and failures

Use explicit `ownership:'owned'` to transfer the supplied genuine SDK capability
lifetimes to the endpoint. `close()` retires Native ownership once, then attempts
every independent actual SDK capability close in reverse order, even if another
close fails or throws. A thrown cleanup is preserved and reported as a nonzero
close result; actual retirement is never misreported as success0. The endpoint
closing guard also spans all genuine raw owner closes: synchronous adapted
delegate reentry returns17 without relabelling its own in-flight attempt as
external cleanup. A separate `cleanupThrew` flag records every thrown JavaScript
value, including null, undefined, false and0; a null/truthiness sentinel cannot
turn an attempted failed close into success on a later cleanup call. Partial
application rollback rows likewise use an independent `threw` flag. Genuine SDK
private entry tickets decide whether a close
was attempted. A local preflight refusal retains that owner for a later explicit
cleanup; an attempted Host close is never blindly retried. Keep the endpoint
object and `facts()` through failures.

Use `ownership:'borrowed'` only while the caller retains and later closes all
supplied capabilities. The endpoint still retires its Native endpoint once, but
never closes those borrowed owners. Its facts explicitly report the remaining
caller cleanup duty. The SDK's existing private polling leases span every
operation; direct Capability/Node cleanup cannot retire the same owner during
that operation. Do not mutate a supplied Capability's handle or Session.

A successful peek is a borrow, not delivery. The SDK checks complete UMRC160,
same-record URMI192, full owner/session/incarnation, role, peer, extents, quota,
strictly fresh nonce and stream sequence. Client sequences are contiguous. A
complete associated refusal cancels the exact original sequence+nonce. An
unassociated header supplies no safe cancellation token and retires the known
endpoint instead. An uncertain commit never triggers cancellation or replay.

Full typed decode, duplicate request-ID checks, copied signed64 timestamps,
signed64 Native request sequence, complete16-byte writer GUID and the result
object are prepared before one ACK. After business ACK success only already
prepared local state/return work remains. UINT64_MAX is a valid unsigned Runtime
ticket; zero is invalid. Native request sequence zero/negative values and an
all-zero16-byte Native writer GUID remain legal exact values. Neither is a local
Runtime ticket. A late abandoned response is still fully decoded before ACK and
returns `discarded:true` with no successful application delivery. Its lifetime
tombstone prevents a second success.

Control records preserve actual reason, correlation and cleanup/external-effect
flags without invented Native metadata. Primary, peek, cancel, commit, Native
retirement and each raw owner cleanup remain separate diagnostics. A handler
exception occurs after take/ACK: close the server explicitly and report it. Never
turn it into an empty successful response.

## Complete caller-driven applications and validation scope

[The description client](../../examples/javascript_owned_service/control_client.mjs)
opens the three explicit aliases, queries actual availability, submits the full
original request with `include_type_sources:true`, takes the full typed response,
abandons explicitly, and closes every partial acquisition. [The parameter-type
server](../../examples/javascript_owned_service/control_server.mjs) opens the two
independent aliases, handles one exact-ID typed request at a time and closes on a
callback failure. Their shared [setup owner](../../examples/javascript_owned_service/setup.mjs)
allocates its failure carrier, partial-owner diagnostics and activation refusal
before the first alias open. If setup fails, the thrown `SetupFailure` retains
`primary` by reference, `cleanupStatus`, `cleanupException` and `application`.
This works with a frozen primary Error and never relies on attaching properties
to it. Call `applicationCleanupFacts(failure.application)` for every partial
owner's actual attempted/result/error/retained state; an existing endpoint also
remains reachable for its facts and explicit later local-preflight cleanup.
All independent rollback closes are attempted before throwing the carrier. Their
inputs are explicit injected selection values; neither reads a host
path/environment or fabricates Native QoS.

The authored [local model contracts](../../../tests/ros_javascript_owned_service/model_contract.mjs)
cover twenty-three independent lifecycle/wire scenarios. They have not run. The
producer's real-builtin rejection fixture, real Wasm/import/layout checks,
Runtime denied-policy tests, same-actual-node Native/DDS interoperability,
shutdown, distribution/vendor/architecture matrices and physical memory/VTune
measurements remain separate required qualifications. Source review and model
assertions cannot substitute for them.
