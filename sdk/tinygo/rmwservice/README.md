# TinyGo owned-native ROS2 Service SDK

This new optional package is source implemented and unexecuted. It uses the real
`uwvm_ros2_rmw_srv_v1` twelve-function numeric ABI. It does not route original
control services through the old `uwvm_ros2_v1` Service business functions, the
optional `uwvm_ros2_client_v1` query module, a native ABI/JNI bridge, or a mock.
The only old-module calls used here are explicit capability open/close/revoke.
Original81 declarations, old SDK owners and all existing examples are untouched.

Select `tinygo`, wasm32 WASI, exactly `ros2` (not `ros1`), and the additional
`rmw_service_v1` build tag. Missing raw imports must fail real artifact admission
or linking/instantiation; there is no weak stub. The raw twelve signatures and
all wire layouts are pinned to `sdk/abi/rmw-service-v1.json`,
`sdk/include/roswasm/rmw_service_v1.h`, and the actual IPC parsers. A successful
source build alone would not qualify the Runtime, optional import gate, Native
libraries, generated type exports, actual QoS, or a ROS exchange. All those
steps, including compilation of these authored contracts, remain unexecuted.

## Explicit owners and preallocation

`NewGroup(Options)` allocates bounded local state, opens no aliases and obtains
no authority. Options must explicitly select endpoint count1..256, lifetime
transaction slots1..256 per endpoint, separate request/response byte maxima,
actual native writer extent1..16, finite maximum wait0..60,000,000,000ns, and a
positive local storage allowance at most64MiB. Bounds0 permit logical Empty.
Each endpoint's UMRC packet, input scratch, fixed receipt/ticket/bool slots,
owner arrays, history and fixed cleanup diagnostics exist before any open.
The checked local byte allowance includes `unsafe.Sizeof` state and payload
arrays. Go allocator metadata, typed output values, Native DDS/RMW allocations
and process memory are additional; it is not a measured RSS or Native budget.
A finite process/cgroup/Source owner is still required. Fatal allocation failure,
Guest trap or forced process termination cannot promise execution of Go defers.

The Group is address bound. Keep its original pointer; copying its value is
refused by every effectful Group entry. Copying a Client/Server facade shares
one actual endpoint pointer. All state is for one Guest thread with no
concurrent goroutines. A whole-Group lease rejects recursive operations,
factory/close/revoke with17 during an encode/decode callback, before effects.
The SDK has no original-rclcpp executor or cross-upstream-callback lease claim.
`TakeRequest` returns after its ACK; application code later runs outside that
borrow, while the Group still strongly owns the endpoint.

`NewClient` reserves its wrapper/row and all owner registration before opening
independent call and event-observer aliases. A nonempty availability alias is a
third independent owner, opened before activation. It is never invented or
opened by `Available`. `NewServer` similarly owns advertise and a distinct
responder alias. Source/peer/query aliases cannot repeat within a Group's
lifetime, including failed rows. Factory/attempt rows are never recycled.
Aliases remain Host-configured policy references. Defaults grant nothing;
opening an ungranted alias returns the actual denial. No management/policy
mutation/debug function is imported.

The call/observer relation is the real Host `event-source-alias` pairing with
`event.observe`, service-response masks and its distinct event-envelope codec.
It is not a second service codec. The server responder is separately authorized
for the same selected full service/type/identity/canonical/QoS. New activation
does not call old `event_watch`, `service_submit` or `service_advertise`.

## Complete receipts and values

Pass two explicit normalized expected actual QoS profiles to each factory.
Every UMRE256 byte is validated: backend, operation, role, full64 source,
instance, runtime generation, session, native incarnation, both actual QoS
profiles (history/depth, reliability, durability, liveliness, deadline,
lifespan, lease, namespace-convention flag) and reserved bytes. Full returned
client GID extent0..32 is retained, including legitimate all-zero bytes. Server
GID extent must0. A client GID is not a request writer GUID. No truncation,
process counter or guessed native identity is used. The first valid actual Host
receipt binds the Group's scope; every later endpoint must have the same
instance/generation/session. This is an observed actual caller scope, not an
independent self-authenticating Native export. Inspection must match the original
receipt exactly except its operation byte, before each business operation.

A `Definition[Q,S]` names the exact generated service root and its actual
`UWVMROS1_...` canonical identity. Bind the generator's SERVICE-root constant,
not a component constant. Request/Response Codec callbacks and maxima must
come from the actual corresponding complete generated records. UMRE has no
type/hash strings, so these local strings are source coupling facts, not proof
of Host policy or full Native TypeSupport. The operator must independently pair
these aliases with that full native type/canonical identity and genuinely
exported Native identities. The record generator reports absent identities as
absent; this package never manufactures them. Generated records retain every
field, declared default, constant, bounds and raw-source byte domain.

The selected `Codec.Decode` contract returns owned values and validates exact
extent/all fields; it must not alias the input scratch. The actual control Go
emitter copies raw bytes into new slices and converts text into owned strings,
with private nested/sequence construction. Arbitrary input-aliasing decoders
are unsupported. Typed values may allocate before ACK; owner registration never
waits until after an effect. User-provided encode/decode callbacks are trusted
Guest source, not permission or Native proof.

`Submit` encodes privately, checks the full bound, reserves a lifetime history
slot before the actual send, and accepts a ticket only from the eight-byte slot
written on success. Zero is invalid; UINT64_MAX is a valid opaque ticket. A
successful submission is not completion. Again11 spends its attempt row; no
automatic replay occurs. Unknown/duplicate/zero tickets terminalize the endpoint.
`Respond` requires one actually delivered ticket and marks it once-only before
calling raw respond. Any unknown send/respond outcome retires the endpoint;
calling again never replays that operation.

`Receive`/`TakeRequest` each perform one actual peek. They validate the complete
UMRC160 and same-take URMI192 before decoding: all bindings, exact record extent,
role/peer, source scope/incarnation, monotonically accepted sequence, independent
strictly fresh reservation nonce, full ticket, payload bytes, reserved fields,
actual kind/control flags/error and every required Native fact. Service data
requires actual source/receive timestamps and native writer/sequence presence.
Zero writer bytes and signed timestamp/sequence extremes are preserved exactly.
The writer extent must match the explicitly selected real1..16-byte ABI extent.
No fact is inferred from a missing metadata field. Current normalized control
records admit at most32768 bytes even though the protocol ceiling is4MiB; this
package never enlarges policy/canonical bounds to that ceiling.

Private full decoding precedes commit. Only commit0 publishes a typed value or
marks a server request delivered. A lost/late/failed ACK makes delivery unknown:
no output value, no replay, terminal Native retirement. Decoder failures cancel
this exact sequence+nonce and keep the original record. A decoder panic attempts
that same cancellation before propagating; no exception is swallowed. Raw
peek95/90/11 causes no invented ACK or automatic resize/retry. A successful peek
which lacks required facts is refused and its known borrow is explicitly
cancelled; the record is not popped. Cancel failure preserves the primary error,
records its independent code, and retires. A stale reservation is never ACKed.
A local code95/90 decoder refusal can be explicitly observed again, but its
later reservation must be fresh; there is no internal loop.

`Abandon` is local request abandonment, not DDS cancellation. Even a late value
for an abandoned ticket goes through the complete decoder before ACK/disposal;
it never becomes a successful caller value. Native controls carry real error,
retired/external-effects flags and correlation, with no invented request ID,
GID or timestamp. Control metadata and error are prepared before ACK. Valid controls are ACKed
as controls and returned with `HasValue=false` plus the actual code; after
commit0 only local history/poison bookkeeping runs, without another import or
fallible allocation. Terminal Native retirement is deferred to explicit
endpoint/Group cleanup, whose strong owners remain retained. Unknown correlations terminalize rather
than being silently dropped. Native writer/sequence duplicates on a server
are checked separately from opaque Gateway tickets.

## Query and cleanup

`Available(waitNanoseconds)` uses this retained client and the independently
opened third owner. Missing query owner returns95 before querying. Zero asks
one real observation; a positive value must be within explicit local and actual
Host/native bounds. The Host's admitted clock/GraphDomain/stop observation owns
time: this SDK does not read ambient clocks, sleep, count iterations as time,
construct a probe client or substitute graph lists. Query success privately
validates byte0/1 before returning a bool. Malformed success is22 and retirement;
nonzero raw errors preserve the caller result. Positive Native feature support
remains an actual SDK/Runtime qualification prerequisite.

`Close` once-retires the same admitted native incarnation before closing raw
query, peer and source owners independently in reverse order. Every Attempted
bit and code is retained; cleanup keeps going after failures. `Revoke` first
retires the Native object, then locally retires each selected raw owner BEFORE
its actual revoke attempt, matching the genuine Host/TinyGo core contract even
when a remote error follows retirement. It retains each independent revoke
outcome and closes only owners whose revoke was never attempted. An
attempted-revoked handle is not closed again to invent a BadHandle9 cleanup
failure. This does not mutate Host policy. Repeated
close/revoke never repeats an import. `Diagnostic` returns a fixed value copy:
primary, activation/inspect validation and original bytes, each ACK/cancel/
retire result, and each raw close/revoke are separate. Cleanup errors never
replace the original business error. A malformed receipt with a trustworthy
actual fixed binding can be used only to retire; otherwise raw owner close
invokes actual Host cleanup without guessing an incarnation. Unknown/trapped
Host effects require the external finite Source/session owner, not a fabricated
local cleanup success.

## Future build and real examples

See `recipe.json` and the complete `sdk/examples/tinygo/owned_rmw_service` main.
It uses the real record operator, separate actual `customtypes` package and
root constants. `gtd-client` preserves the actual GetTypeDescription true
include-sources default and real caller-provided71-byte RIHS string; a complete
response is re-encoded/decoded without dropping any raw source byte. The
`types-server` implements explicit no-set-parameter business behavior with one
PARAMETER_NOT_SET result per actual name, retaining ordering and full length.
It is a Service example, not a replacement for the original rclcpp Node API.

Its CLI requires TWO explicit normalized actual QoS observations, each twelve
comma-separated unsigned decimals in this order:
`history,reliability,durability,liveliness,depth,deadline_sec,deadline_nsec,lifespan_sec,lifespan_nsec,lease_sec,lease_nsec,avoid_namespace`.
Do not guess actual middleware-default resolution from a profile's name.
Aliases and independently exported Native type/hash inputs follow these two
profiles. `gtd-client Q0 Q1 CALL OBSERVER QUERY TYPE_NAME ACTUAL_RIHS71` and
`types-server Q0 Q1 ADVERTISE RESPONDER` are complete argv shapes. The examples'
64-iteration budget is a finite poll/quota budget, not a wait guarantee. Budget
exhaustion returns90 and retires; no timer, clock right or retransmission is
hidden. The actual external counterpart can be the independently built original
Node default GetTypeDescription server and a real rclcpp GetParameterTypes
client. Native constructors/options, independent grants and every original
callback/lifecycle dependency must remain intact.

There are10 pure wire contract functions and20 tagged private-transport contract
functions, authored but unexecuted. They exercise full-u64/zero/signed metadata,
receipt domains, malformed/tombstone decoding, nonce cancel/ACK ambiguity,
whole-Group reentry, real-owner cleanup bookkeeping and independent availability.
Synthetic fixtures authenticate no Native type or permission. Actual TinyGo
compiler/stdlib support, optional raw12 artifact admission, real default Node,
Native opposite-peer whole values, rejection gates and all fault outcomes need
separate builds/execution after independent source review. The existing source
producer's older allowed namespace must not be silently bypassed; Root must
explicitly admit exact raw12 catalog/signatures for the future artifact.
