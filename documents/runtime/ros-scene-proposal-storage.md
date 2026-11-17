# Sealed proposal storage and model input evidence

`scene/proposals.h` integrates the bounded admission state machine with actual
sealed-buffer leases and model job receipts. `scene/proposal_codec.h` implements
the canonical `proposal.capture`, `proposal.publish`, `proposal.read` and
`proposal.accept` records. Central host/CLI registration now exposes these four
carriers through exact sensor/model/arbiter deployment profiles. Combined VM
qualification remains in progress, separately from the core receipts below.
The components themselves neither attach native sensors/models nor execute a
physical target. Acceptance produces inert
`admitted` / `locally_accepted` recovery data.

## Independent scopes and fixed storage

A provider owns one exact alias, one approved model alias and the original
producer/proposal capability identities. Tools, sources and reader/arbiter
grants require separate trusted binding. The producer cannot accept its own
proposals. Each immutable tool fixes its canonical profile and schema digests,
argument type/encoding/validator, exact target capability/action, effect class,
physical resource scope, frame and authority/provider/controller epochs.
Physical restrictions apply to topic, service, action, parameter and lifecycle
tools as well as device/control operations.

All capacities default to zero. Runtime bounds can reduce the fixed maxima of
16 proposals, 4096 argument bytes, eight observations, eight readers, eight tools
and 16 source slots. Production tool/source/proposal/receipt identities come from
the VM's resource allocator. A reader's request-key session is its already
validated arbiter capability handle, so a guest need not guess a private ID.
Capability and resource integer namespaces differ and can contain equal numbers;
typed ownership maps decide access. The original isolated core's allocator remains its default template
parameter for existing standalone tests. No external wrapper ID is invented or
silently recycled.

Publication first validates bounded arguments and model evidence, then checks a
free proposal slot before allocating retained storage. Allocations use the
recoverable strict fast_io adapters. Failed allocation consumes no core proposal,
ID result or source lease. Readers/validators retain a proposal while borrowing
it; explicit release returns `again` until those borrowers finish. Actual buffer
capacity remains charged for every surviving source/proposal/captured token,
including after its guest handle closes.

## Capturing observations before inference

A trusted source adapter binds host metadata to an actual sealed allocation.
It supplies source incarnation, sample sequence, current authority epoch,
clock/epoch, capture time, frame and calibration/transform generations. Buffer
contents and model text cannot assign these fields. The dispatcher checks the
guest's explicit buffer capability under current source authority before handing
over the lease; the storage component cannot infer a grant from a buffer handle.

The explicit capture operation retains an opaque snapshot token before model
submission. Tokens contain immutable provenance, the actual sealed lease and
its handle/digest evidence. Updating a source replaces only its current sample.
Existing tokens and proposals keep the original snapshot. This prevents a slow
model's old result from being relabeled with a newer sensor sample.

Publication requires a successfully completed, cleanup-confirmed model request
from the bound model alias. `models::registry::check_inputs` compares the exact
input count, order, handle and SHA256 to the captured tokens. Tokens from another
proposal provider are rejected. The host retains these tokens in the capture
resource until publication/cleanup; publication must precede explicit release
of the model job's evidence. A model
receipt alone does not supply live source permission.

The successful `check_inputs` comparison is the immutable evidence adoption
point. An already released request cannot be checked; a concurrent release
after successful comparison does not cancel publication that has adopted the
evidence and still holds its real snapshot pins. This is not a transaction
spanning both registries. Source/target authority and freshness are separately
checked at publication and local acceptance.

`latest_required` accepts only the exact current snapshot. `bounded_age` permits
a newer sample while the older captured snapshot remains within age/expiry and
the same source incarnation, authority, clock epoch, frame, calibration and
transform generations. Default/unrecognized policy denies. Source permission
fencing is separate from sampling and invalidates older proposals immediately.
After fencing, capture is denied until a trusted source update provides current
metadata. Revocation is permanent. Frame changes require a newer transform
generation and clock identity changes require a newer source incarnation, so
switching back cannot revive a previously invalid proposal.

## Canonical delivery and local acceptance

`proposal.read` encodes the original model request, immutable tool profile,
validated typed arguments, captured observations and expiry. The present trusted
source path requires a stamp and frame; the codec preserves their original
values. Untrusted guest buffers cannot claim these sensor fields. The separate
model input path can use explicitly untrusted observations, but they do not
become physical-tool provenance through this storage layer.

Production acceptance also requires a nonzero host-only
`authorization_context{proposal_scope,target_scope}`. After complete group
admission, the broker resolves these environment/grant/generation-bound logical
identities from trusted mappings. They are not hashes of guest aliases or raw
integers. The original canonical capability fields remain unchanged in the
stored request and exact-retry comparison. Opening another capability handle
for the same granted scope therefore cannot disguise a changed retry request.
An unbound production scope is denied; zero-context fallback exists only in the
original isolated core test API. The arbiter's logical grant scope is also
separate from its raw capability/session handle.

The broker must admit the proposal reader or independent arbiter plus every
current source and target action required by the immutable tool. It holds these
short authority leases through the local transaction. A proposal record, pin,
confidence score or historical receipt cannot install a grant. The target
provider must independently check current authority, owner lease, fencing,
schema/physical bounds and immutable observation snapshot at its actual effect
admission point. This provider intentionally has no execution method.

Acceptance checks actual source leases before and after the bounded validator.
The validator runs outside wrapper and core locks and may not execute commands.
The core repeats deadline, snapshot lineage and arbiter/target epoch checks
before committing. Concurrent or reentrant acceptance returns `again`; source,
target or authority changes cannot consume the approval sequence or its optional
rate. Missing rate permits the granted scope; explicit zero denies new approval.
`attenuate_reader` can reduce rates and advance the authority epoch, never broaden
actor or target rights. An exact previous receipt remains inert recovery data.

Failed reads consume nothing. Failed output copying after acceptance uses exact
request retry, including after explicit proposal release; it never re-executes a
target. Retirement stops local admission and must be followed by joining native
callers. It does not assert that a controller stopped or provide a certified
watchdog, emergency stop or industrial safety certification.

`try_inspect` provides a nonblocking alias aggregate only. Proposal counts,
reserved source/reader/tool slots and provider-held snapshot references contain
no payload, digest, raw capability, retry key or pointer. Slot counts include
tombstones. External capture tokens are not counted as provider-held references;
actual retained bytes and lease capacity remain in buffer accounting. Inspection
does not read the clock, expire a proposal, invoke native I/O or advance state.

## Qualification

`tools/ros_scene/proposal_storage_checks.py` archives actual source/dependency
text and records hashes, gate inode/boot identity, memory/swap limits and events.
All compiler/test processes remain inside the existing 64 GiB, zero-swap cgroup
on CPUs 12–15. The production test covers real buffer/model/proposal lifetimes,
slow-model versus fast-sensor semantics, old/new and cross-provider input
pairing, immutable canonical provenance, source/target/authority fencing,
zero/default scope, explicit zero rate, exact historical recovery, concurrent
acceptance, fork denial and actual strict-allocator failure rollback. Existing
admission-core tests separately cover the full action matrix and ABA/freshness
policies. VM/CLI/debug and native provider integration need their own acceptance
evidence before these operations can be reported as registered implementations.

## Complete model-output adoption and dispatcher

`proposal_dispatch.h` adds the independent frontend for `proposal.read` and
`proposal.accept`. Deployment must bind a `model-proposals` alias and a separate
`proposal-arbiter` alias. `proposal.read` takes the existing one-u64 proposal
request; `proposal.accept` takes the existing seven-u64 request, with the arbiter
capability as `key.session`. Replies retain the catalogue record format. The
arbiter must independently possess the proposal-reader and exact target action.
A configured tool mask selects tools sharing one exact target alias. No import
is registered merely by compiling this dispatcher.

`capture` observes only an existing typed source through a trusted adapter. It
checks `resource.observe`, the exact source profile and frame, and the actual
sealed-buffer alias/profile under `buffer.read`. Each host-only `source_reference`
explicitly supplies the source capability, source resource and buffer capability.
The native source supplies the actual sealed allocation and provenance; a caller's
capability supplies only independently checked authority. Capture never opens a
capability implicitly or trusts a capability number in deployment configuration.
After all adapter callbacks it rechecks the complete source group and real pins
before publishing its opaque token. Opaque captures retain those
actual allocations; model submission must use the same ordered buffers. Capture
records hold an explicit slot and workspace reservation until their last shared
reference is destroyed. Updating a cached source is an observation of current
host state; it may remain cached if a later capture item fails. Such a failed
call publishes no capture token and does not submit a model job.

A successful model job must return one nonempty, complete final byte record with
output type `ros.proposal` and encoding `bytes`. Its canonical fields are the
text magic `uwvm.ros.model.proposal`, tool profile, schema profile, target alias,
target profile, actual target resource, action text, frame text, argument type,
encoding, and argument blob. Profiles are identity text followed by a 32-byte
SHA256 blob. Text/blob lengths and integers use the existing little-endian u64
wire encoding. The complete record is at most 8192 bytes, and arguments at most
4096 bytes. Unknown actions, invalid UTF-8, NUL, trailing bytes, zero targets and
inexact immutable outer fields fail closed.

`model_dispatch::with_completed_output` verifies the final length and SHA256,
successful terminal outcome, confirmed cleanup, exact ordered input handles and
digests, original caller, and the still-current original model/source authority
group. It provides a private, noncopyable token only for a synchronous callback.
A delivered or lost `model.take` does not erase this evidence. Explicit job
release or session close denies adoption. A late cancellation cannot rewrite
historical terminal success. Failure, cancellation, unknown worker outcome,
streaming-only chunks and supervisor `reconcile(succeeded)` without actual final
bytes do not qualify. The whole envelope is authenticated, so retaining an
argument digest cannot authorize a substituted tool, frame, schema or target.

The dispatcher resolves actual target environment/owner/generation/profile,
resource and authority/controller epochs from a trusted typed provider. Static
policy contains none of those runtime identities. Validators are registered
host functions, not symbols from configuration or model text. Validation and
allocation run outside the proposal storage mutex; authority and source checks
are repeated after callbacks, then the storage commit rechecks current pins and
tool epochs. Source/target callbacks must be bounded, observational and keep
native objects alive for each invocation. They cannot execute a robot task.

Acceptance produces only the historical `admitted` / `locally_accepted` receipt.
It has no physical effect and cannot report physical completion or grant rights.
Actual execution always requires a separate current target transaction. Exact
accept retries recover the previous receipt even after proposal release or
source/target revocation, provided the independent arbiter and proposal-reader
rights remain valid. Changed bytes at the same key conflict. `request.describe`
and `request.release` require their own arbiter actions; release hides ordinary
receipt lookup while retaining the finite exact-retry tombstone. A later
successful sequence replaces the previous receipt. Each arbiter's revocation
fences only its own reader slots.

## Immutable configuration and capacity

`proposals::configuration` accepts `profile(selected, model_alias, model, task)`,
`limit(name, value)`, `definition(bytes)` and finally `freeze(plan)`. Each of the
11 limits is mandatory exactly once: `proposals`, `argument.bytes`,
`observations`, `readers`, `tools`, `sources`, `lifetime.ns`,
`accept.deadline.ns`, `captures`, `workspace.bytes`, and `workspace.calls`.
Zero is literal zero; missing values never imply unlimited storage. Capacities
are independent from optional authority rates. Array maxima are 16 proposals,
4096 argument bytes, 8 observations/readers/tools, 16 sources, 1024 capture
records and 1024 workspace calls. The frontend also caps bound proposal and
arbiter aliases at 32 each. Deployments must impose their aggregate budget when
selecting these finite per-alias limits.

The at-most-32768-byte definition contains magic
`uwvm.ros.proposal.definition`, selected identity, model alias/model/task
profiles, a counted source array, then a counted tool array. Each source carries
alias, source profile, buffer alias, buffer profile and frame. Each tool carries
tool/schema/target profiles, target alias, action, effect (`physical` or
`non_physical`), observation policy (`latest_required` or `bounded_age`), argument
type, encoding, frame, maximum observation age and minimum observation count.
Physical tools require observations. The selected SHA256 covers the entire
canonical definition, which contains no copy of its own digest. Parsing does
not open paths, perform I/O, allocate storage or activate a provider. Trusted CLI
file loading and independent grants remain separate integration steps.

Persistent containers use recoverable fast_io allocation. Temporary payloads
use bounded owned storage; small payloads use the existing stack scratch buffer.
Capture reservations remain charged through source-pin destruction, including
retirement. Workspace call and byte reservations fail before owned allocation.
Reader/source/tool slots and retry tombstones are fixed and are not silently
recycled into another authority scope.

`proposal_dispatch_checks.py` qualifies the dispatcher, strict configuration,
actual allocator failures, cancellation/reentry and concurrent reads using
`-O3 -g` with Clang ASan/UBSan, GCC and TSan in the existing 64 GiB, zero-swap
cgroup. `proposal_core_checks.py` repeats the existing six storage groups under
the same optimized/debug build flags after the authenticated-publication
changes. These focused receipts are separate from actual VM carrier, CLI,
debugger, native target adapter and industrial deployment qualification.

The current focused receipts are
`../uwvm2-ros-sdk/.tmp/ros-scene-security/proposal-dispatch-qualified/results.json`
(12 groups, nine successful commands) and
`../uwvm2-ros-sdk/.tmp/ros-scene-security/proposal-core-authenticated-final/results.json`
(six groups, nine successful commands). Final model-output adoption is separately
qualified by `model-completion-second/results.json`. Each receipt retains the
actual source closure and confirms unchanged inputs and snapshots. The dispatcher
suite injects two real capture-allocation failures and one publication-allocation
failure, then verifies rollback and successful retry. The shared cgroup's
`memory.events.max` increased during core qualification; OOM and OOM-kill counters
remained zero. These receipts do not imply that no shared memory pressure occurred.

After the cross-provider cancellation audit, the latest dispatcher cohort is
`proposal-dispatch-final-model-authority/results.json` (nine successful commands,
12 groups). It includes the model callback fix qualified separately by
`model-cancellation-qualified/results.json` (nine successful commands, the seven
existing model dispatch groups plus four adversarial cancellation groups). A
cancellation callback returning false can still revoke authority; fresh model
submit and output copy now repeat the original complete source group after that
callback, and output consumption repeats it after delivery. Historical cleanup
and exact receipt recovery remain available after source revocation, with a fresh
primary-authority check before disclosure. The proposal common receipt path also
rechecks its current arbiter after the last cancellation callback and before copy.

The subsequent `proposal-read-source-cancellation/results.json` cohort contains
13 groups and nine successful ASan/UBSan, GCC and TSan commands, with 415 stable
source files. `proposal.read` now repeats its complete source authority and typed
source validation after the last cancellation callback, checks the actual retained
sealed pins, then checks primary authority before copying any bytes. Regressions
revoke source or buffer authority, retire a typed source or buffer registry, change
source generation/authority epoch, and revoke primary authority from the source
validator. Every case refuses before disclosure. Historical receipt recovery
keeps its existing primary-only semantics.

`source_reference` now explicitly contains `{index, capability, resource,
buffer_capability}`. A source adapter supplies the actual sealed allocation and
its provenance; it cannot supply the caller's buffer authority or implicitly open
a capability. The dispatcher checks the explicit buffer capability's exact
`sealed-buffer` binding, alias, profile and read action independently of the
source capability, including after reentrant source validation. The resumed
`proposal-explicit-buffer-resumed/results.json` cohort contains fourteen groups,
nine successful sanitizer/compiler commands and 415 stable source files. The
earlier `proposal-explicit-source-buffer` run stopped after seven successful
commands and remains incomplete.

The typed sensor source described in [sensor/device/control boundaries](ros-sensor-device-control.md)
provides a real `source_access` adapter using its private ingress queue and finite
sealed history. A separately implemented `proposal_lifecycle_target.h` adapter
supports only the actual `local-lifecycle` graph. Its strict argument schema is
`ros.lifecycle.submit` / `le64`, containing the five existing submit words. Target
profile hashes cover the exact alias and all lifecycle limits. Cached client
observation supplies the actual state generation, next key, transition mask and
limits without expiring state or executing callbacks. Validation checks current
target authority and exact arguments; an admission receipt is still inert.

This lifecycle tool is explicitly `non_physical`, with frame `logical`. It cannot
be repurposed as a physical ROS lifecycle bridge, device controller, watchdog or
native node adapter. Those adapters need their own actual owner/fencing/source
contracts. Qualification of the real sensor → completed model → local lifecycle
proposal chain is recorded separately; no core/facade receipt registers a guest
import or qualifies a native robot effect.

`proposal-lifecycle-actual-second/results.json` qualifies that actual chain with
417 retained source files and nine successful `-O3 -g` ASan/UBSan, GCC13 and TSan
commands in the existing 64 GiB zero-swap cgroup. Six groups exercise real private
sensor bytes, actual buffer pins, completed model output and the real local
lifecycle registry. Acceptance and lost-receipt retry leave lifecycle state and
next key unchanged and produce no lifecycle callback. New sensor data, changed
lifecycle state, source/target/buffer retirement, forged schema/profile/role/key,
invalid transition and elapsed deadlines are refused. Cached observation does
not expire pending state; a reentrant clock callback can revoke authority or
retire the adapter without deadlocking or admitting a target. Validation performs
no heap allocation, and forked instances refuse inherited resources.

The fixed sensor calibration/transform epoch fields identify the unchanged raw
source binding. This adapter performs neither external calibration nor a TF
lookup. A nonidentity transformation requires a separately bound provider that
can authenticate its actual calibration/transform provenance. These receipts do
not supply such an adapter. The first lifecycle chain cohort failed a strict
unused-variable fixture check before execution and remains preserved separately.

## Explicit guest capture and publication

The guest facade is `proposal_guest_dispatch` in `proposal_guest.h`. The semantic
operation set contains `proposal.capture`, `proposal.publish`, `proposal.read`
and `proposal.accept`. Registering these names still requires the VM carrier and
an activated typed deployment; a descriptor or a host-only test does not supply
a native sensor driver.

`proposal.capture` requires its own current proposal capability and an exact
immutable proposal profile. Its canonical request is the request-key pair,
profile, source count, ordered source references and absolute steady expiry.
Each source reference contains four unsigned 64-bit words: configured source
index, source capability, source resource handle and buffer capability. No source
timestamp, source identity, authority epoch or caller-selected buffer is accepted
as provenance. The response contains capture handle, count and ordered
`(buffer_capability, buffer_handle)` pairs. Counts are at most eight; all words
use the existing little-endian 64-bit carrier and profiles retain their exact
identity/digest encoding.

The guest submits those unchanged ordered buffer references through the ordinary
`model.submit` interface, with native source/stamp metadata absent. After taking
the complete successful terminal output through `model.take`, it calls
`proposal.publish` with request-key pair, capture handle, actual model request,
complete terminal envelope blob and absolute proposal expiry. The response is
the proposal handle. Publication verifies original caller, model/task/profile,
input order, actual allocation identities and hashes, complete final bytes,
terminal success and confirmed model cleanup. Neither operation creates source,
buffer, model or target authority. `read` and `accept` keep their existing records.

Capture and publication maintain independent strict key sequences per proposal
capability. The first key is one, successful next keys advance by one, and an
exact last replay recovers the same receipt. Changed bytes, old keys and gaps
conflict; a refused admission consumes no key. Successful commitment precedes
receipt copy, so a lost copy retains the object for recovery. An exact publication
receipt remains historical data after capture/source closure and requires current
publication authority; it grants no permission to read, admit or execute the
proposal. Capture retry requires the original live capture and source witness.

`resource.close` independently closes a capture. A different current capability
for the same private alias and caller can close persistent captures after the
creating capability is closed. Capability close itself does not silently destroy
resources. Published proposals and completed model jobs retain their independent
actual buffer pins; closing a capture does not refund storage still pinned by
those owners. Expired captures need explicit cleanup and are never silently
replaced by a newer source observation.

Deployment configures three additional finite facade capacities before prepare:
`producer.sessions`, `capture.handles` and `retained.bytes`. Each is explicitly
assigned once; missing/zero capacity does not enable the capture/publication
path. Producer sessions are limited to 32 and captures to 1024. Successful session
receipts remain bounded tombstones; closing a capability does not recycle a key
namespace. `required_storage` accounts for both fixed arrays and the actual
fast_io array prefixes. Underlying proposal storage and per-alias workspace are
separate limits. The carrier reserves copied-request workspace after operation
preflight and before allocating owned input. Recoverable fast_io allocation
failure returns an error and preserves retry semantics; ordinary allocation
budgets may charge an admitted attempt, but refused preflight does not charge.

Construction order is shared authority/buffers/model and actual source/target
providers, immutable bindings, facade `configure`, `bind` / `bind_arbiter`, then
`prepare` with their borrowed typed adapters. Teardown retires and joins active
callers before destroying any borrowed provider. A source-free nonphysical tool
still requires an explicitly configured policy permitting zero observations;
it does not qualify a fabricated sensor or physical target.

`proposal-guest-final/results.json` qualifies eight actual guest-flow security
groups and nine successful Clang ASan/UBSan, GCC13 and TSan commands, all with
`-O3 -g` inside the existing 64 GiB zero-swap cgroup. Its 419 retained source files
and working inputs remained unchanged. The cohort verifies the real model.take
terminal envelope, independent operation permissions, short-copy preflight,
exact lost-receipt recovery, no duplicate publication budget charge, malformed
carriers, unrelated/changed completed output, capture close with surviving
proposal pins, expiry, callback revocation, concurrent retirement, recoverable
fast_io capture/publication/configuration failures and inherited-process refusal.
Lifecycle acceptance remains inert. Native driver and VM import qualification
remain separate.

Earlier guest cohorts are preserved as incomplete: the first found a strict
initializer warning, the second a GCC test indentation warning, and the third
exceeded the test stack after optimizer inlining of multiple large fixed-table
fixtures. Fixtures now use recoverable fast_io ownership instead of increasing
the stack or bypassing the cgroup. The fourth passed all nine commands against
unchanged retained sources but its live authority header changed concurrently;
it is not the final exact-workspace receipt. The final cohort's cgroup memory
event counters were unchanged and recorded no OOM/OOM-kill event.

The original fourteen dispatcher/configuration/recovery groups were also rerun
against that same frozen production closure in
`proposal-dispatch-guest-final/results.json`: all nine Clang ASan/UBSan, GCC13 and
TSan commands passed with unchanged inputs and retained snapshots. This second
cohort preserves the existing authority, whole-output proof, lost-delivery,
allocation rollback and revocation regressions alongside the new guest flow.
