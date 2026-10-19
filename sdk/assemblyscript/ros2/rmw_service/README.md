# Explicit retained-native AssemblyScript services

This optional source module calls the real twelve functions in
`uwvm_ros2_rmw_srv_v1`. It leaves `ros2/raw.ts`, the original business81 catalog,
the older service facade and all existing emitters unchanged. Selecting a module
is not a grant. Missing imports fail actual module admission; this module has no
weak import, old service-call fallback, network, clock or management operation.

This revision is authored source, not an AssemblyScript compiler result, Wasm
validator/import result, Runtime run or Native ROS result. No candidate generator,
compiler, test, model, Guest or Native program was run while preparing it.
Original SDK/Host/Gateway and selected real RMW qualification remain separate.

## Open or borrow exact owners

Import the selected business adapter and the optional adapter explicitly:

```ts
import {NativeApi} from '../../roswasm';
import {NativeServiceImports} from '../imports';
import {OwnedService,Limits} from '../owned';

const endpoint = OwnedService.openClient(new NativeApi(),
  new NativeServiceImports(), 'explicit.call', 'explicit.observe',
  new Limits(4096, 32768, 64, 16));
if (endpoint.status != 0) { /* retain status and diagnostic snapshot */ }
```

The paths above are relative to the included `examples/` directory. A client
requires independently opened `service.call` and `event.observe` owners. A server
requires `service.advertise` and a distinct `service.respond` owner. Actual source
resource/type/canonical identity/QoS/masks/quotas and all native graph/default
proof remain independently enforced by Host and Gateway. A Guest `Limits`, codec
object or copied receipt cannot grant authority or prove Native compatibility.

`openClient`/`openServer` construct and own exactly their two raw capabilities.
Permanent receipt, packet, ticket, availability, diagnostics and finite history
storage is allocated before opening or activating those owners. Failed second
open closes the first actual owner. Failed activation independently closes all
genuinely owned raw owners, even if no valid Native receipt can be recovered.
Do not invent an incarnation from a malformed receipt. Host source retirement
must clean up unknown native activation effects.

`borrowClient`/`borrowServer` instead borrow two already-open `Capability` objects
from the same actual `Api`. They never close, revoke or transfer those raw owners,
including on failed setup or Native retirement. Keep them alive and unchanged
until endpoint retirement, then close **both** yourself and retain both results:

```ts
const endpoint = OwnedService.borrowClient(imports, call, observer, limits);
// ... actual operations ...
const nativeResult = endpoint.close();
const observerResult = observer.close();
const callResult = call.close();
```

For factory-owned endpoints `close()` once-retires Native, then attempts the two
actual raw closes independently even when Native retirement fails. Its return is
the first cleanup result in Native/peer/source order; `diagnostics()` retains each
component separately. Repeated close returns9 without a second retirement. A
caller cannot mutate internal state through the copied diagnostics or receipt.
Raw revocation/Source reset still has independent Host retirement semantics.

## Checked values and exact transactions

`CheckedCodec<T>` uses explicit `EncodeResult(status,bytes)` and
`DecodeResult<T>(status,value)`. `T` is a managed record class. Error22 and bound90
remain distinct; success with a logical empty body is not an error. No sender
encodes and then decodes its complete tree merely to repeat validation.

The full-control record operator has its own concrete `ControlEncoded` and
`ControlDecoded<T>` classes. The included GetTypeDescription helper explicitly
maps those results into this module's result classes. It does not assume nominal
class assignment between separately emitted modules. Generate its actual complete
`bounded_records.ts` and proof separately; no handwritten generated substitute is
provided. It uses the complete raw-byte TypeSource representation and original
aggregate32768 refusal, not a text codec or old business service facade.

`submit` succeeds with one full-u64 Gateway ticket, including UINT64_MAX; only0
is invalid. A ticket is not a Native signed sequence, writer GUID, client GID,
capability, completion or permission. Every entered submission uses a permanent
local history slot. Failed/abandoned/completed slots never recycle. Reaching the
configured lifetime bound returns90 before another submission or server take.
The local1..256 limit defaults64; creating another Guest object cannot reset Host
source/pair lifetime quota or native incarnation history.

`take` does one real `endpoint_peek`. It checks the whole UMRC160 echo against the
exact source, peer, instance, runtime generation, native session, incarnation,
role, expected writer extent, capacity, sequence and reservation nonce. It then
checks the SAME record's complete URMI192 and canonical body. Stream sequences
must be contiguous after successful commits; reservation nonces must increase
even after canceling the same record. Zero timestamps, a signed zero/negative
request sequence and an explicitly written all-zero writer GUID remain real
values. A client GID is never substituted for a request writer GUID.

Every output object, token ledger and typed value is ready before commit. Typed
decode runs once over an owned body copy. Only an exact successful ACK publishes
the typed value and copied metadata to the returned result. The success tail
does not allocate, invoke a codec/callback or call another import. Application
handlers run later, after the completed copy/ACK; a handler failure cannot replay
the consumed record. Controls are returned with `nativePresent=false`, no value,
and their actual error/flags/correlation; Native metadata is not invented.

If a complete associated UMRC header is valid but metadata/typed decode fails,
the SDK cancels that exact sequence+nonce. Decode90 leaves the record retained
when cancellation succeeds; an explicit later take can borrow it with a fresh
nonce. There is no automatic resize or retry. Bad unassociated headers do not
authorize guessing cancellation tokens: they poison and once-retire the source.
Commit errors are unknown outcomes: never cancel or replay a commit, submission,
response or abandonment. A normal error result contains no typed value or
published metadata. The copied fixed refused prefix remains diagnostic DATA only.

`respond` accepts only a locally committed server request ticket, once. The SDK
also retains the actual native writer+signed sequence to reject a replay under a
different local ticket. `abandon` marks one pending local client request for
late-response disposal; it does not claim native/DDS request cancellation.
A response may already be queued when abandonment succeeds. Such a late body
passes the same complete checked decoder before exact ACK. After commit0,
`take` returns status125, `abandoned=true`, `committed=true`, the genuine metadata,
and no typed value. This intentional disposal leaves other pending requests live.
The transaction keeps a separate consumed-discard tombstone, so a duplicate body
cannot later become a success. A malformed late body follows the usual checked
cancel/poison path; decode90 retains the abandoned state for a later explicit
borrow, while an unknown commit never publishes a completed disposal. The
post-ACK disposal tail contains only local fixed assignments, without an import,
allocation, decoder call or callback.

## Availability, lifetime and errors

`available(owner, waitNs)` needs a third distinct actual `service.available`
capability from the same API. Both original paired owners and this third owner
remain leased through the query. The maximum wait is60,000,000,000ns. Zero means
one same-retained-client query; positive means the actual independently admitted
native deadline/graph-event route. No probe client, sleep counter, graph list or
ambient clock is supplied. A canonical one-byte0/1 success is required; every
error preserves the public result's default false plus its nonzero status.

One local endpoint pass and both actual capability leases cover encode, inspect,
peek, complete decode and ACK. Reentrant endpoint/Node/Session/capability cleanup
therefore returns17 before another retirement; availability also leases its third
owner. This is single-dispatch Guest SDK logic, not an atomic shared-memory
AssemblyScript collector or multi-worker guarantee. Do not share these mutable
SDK objects between Wasm threads without a separately qualified ownership model.

Raw effect errors poison the endpoint and once-retire Native; owned raw cleanup
still follows independently. Peek11 is idle, while peek90/95 preserve no visible
borrow under the raw contract. They do not publish a typed result. No numeric
error is generalized into proof that all native effects were absent. Diagnostic
115 denotes an entered call with no returned observation yet. Primary, commit,
cancel, retire and both raw closes have independent slots. Any fatal Wasm trap,
allocation abort or terminal Source transition requires actual external Group/
instance drain and physical retirement; normal source cleanup is not that proof.

## Complete source examples and future admission

`examples/empty_client.ts` exports `begin`, `step`, `completed`, `finish` for an
actual externally bounded driver. It calls the separate raw12 route, preserves
logical empty bodies and reports11 without calling an unfinished request done.
`examples/add_two_ints_server.ts` exports caller-driven server stages, uses exact
signed64 canonical fields and rejects application arithmetic overflow explicitly.
The compiled alias strings must be provisioned independently; no default grant
exists. Neither example is a claimed completed Native roundtrip.

`examples/control_client.ts` is an actual typed application helper over the
separately generated full-control GetTypeDescription classes. The real caller
supplies actual requested type/hash DATA and explicit aliases, repeatedly drives
only authorized finite passes, and closes the endpoint at the real end of its
lifetime. The helper sets original `include_type_sources=true`; arbitrary raw
source bytes and aggregate90 are preserved. A preparation catalog without actual
Native identities remains preparation, never a production Native profile.

`recipe.json` lists unexecuted serial producer/import/validator stages. Use the
actual admitted AssemblyScript producer, heap/process budget and private staged
source. The optional service import checker is required; the original81 checker
cannot attest this namespace. Raw inventory, local byte/lifecycle models, full
Wasm validation, actual Runtime default13/positive values, complete selected SDK/
Native metadata and real peer roundtrips are separate gates. All remain unrun.
