# Bounded model proposal admission core

`proposal_admission.h` implements an isolated native core for inert model
proposals. It does not register `proposal.read` or `proposal.accept`; both
operations remain `planned_provider` in the semantic catalogue. It does not
invoke a model, send a ROS message, operate a device or claim industrial safety
qualification. Its purpose is to make the local authorization, freshness and
retry rules executable and testable before adding a guest codec or target
provider.

The proposed operations and record fields remain those in
[`interface-catalogue.json`](../../tools/ros_scene/interface-catalogue.json).
[`proposal_contract.h`](../../src/uwvm2/imported/ros/scene/proposal_contract.h)
contains semantic names, the closed target action list and existing receipt
words. It does not assign binary ordinals or add exports to `uwvm_ros_v2`.

## Trusted configuration and finite capacity

A default registry has no producer, reader, arbiter, tool or observation scope.
Trusted deployment calls `configure` once with an explicit producer actor,
proposal capability and finite runtime capacities. Each capacity is bounded by
the registry's compile-time storage limits. Zero allows no items or bytes of
that kind. Limits for proposal lifetime and acceptance deadline are explicit
steady-clock durations, checked using subtraction after ordering checks.

The default template has 16 proposal slots, at most 4096 argument bytes and
eight observation references per proposal, eight reader slots, eight tool
slots and 16 source slots. Deployment may reduce these bounds. Storage is
fixed; publication, read, admission, retry, attenuation and retirement do not
allocate container storage. This is a deliberately small per-scope component,
not an unbounded robot-wide registry.

Trusted management then binds the following records:

- A tool profile selects an exact action, target capability/resource,
  argument schema, frame, freshness bound and an explicit observation policy.
  An unconfigured or unknown observation policy cannot be bound. Canonical profile/schema
  identities and their digests remain owned by the integrating host registry.
- A reader grant binds a host actor to a particular proposal capability.
  An accepting arbiter additionally needs its own independent capability,
  exact target capability and explicit action bits. The producer actor cannot
  be an accepting arbiter, including through an incorrectly configured grant.
- Sources contain host-owned sealed-buffer references and provenance. Model
  output cannot create provenance by putting a source number in its arguments.

The management methods are not guest-callable imports. Numerical identities
alone provide no authority. The integration must still obtain the scene
authority lease for the caller and exact action before each operation. The
core's separate monotonic identity allocator is not yet integrated with the
VM's shared resource-handle allocator and kind/owner checks.

## Effect classification and three independent epochs

The eight permitted target actions are `device.submit`, `control.submit`,
`topic.publish`, `service.submit`, `action.submit`, `parameter.set`,
`parameter.set.atomic` and `lifecycle.submit`. No target action permits policy
management, generic sockets/filesystems, credential access or plugin loading.

An action name does not determine whether an effect is physical. A ROS topic,
service or parameter can cause the same actuator movement as a direct device
command. Every immutable tool profile must explicitly classify its target as
physical or nonphysical; an unclassified profile cannot be bound. A physical
profile requires a physical resource scope, controller lease epoch and at least
one observation. All eight action categories use the same checks. The trusted
target provider must substantiate the classification and enforce physical
limits; model text cannot set or change it.

The core distinguishes these host-managed generations:

- `authority_epoch` identifies the arbiter grant revision.
  `target_authority_epoch` identifies the target grant revision. Neither is
  model confidence or an observation timestamp.
- `provider_generation` fences target connection/restart identity.
- `controller_lease_epoch` fences ownership of the physical controller scope.

`fence_target` may advance any target epoch, with no decreases. Every proposal
created against the previous snapshot then fails admission, including one
whose validator is currently running. Tool schema, action and physical scope
remain immutable. Removing a tool prevents new publications and admissions.

These fields preserve the evidence needed for a later atomic target admission;
they do not themselves implement controller arbitration. The actual target
provider still needs owner-lease acquisition/renewal, priority rules, physical
limits, emergency-stop capacity and reconnect reconciliation.

## Publication and provenance

`publish` is a trusted provider handoff after model output has passed the
canonical argument-schema validator. It requires a host-owned model request
identity, resolves the immutable tool binding and source bindings, and copies
the bounded arguments and observations into a new proposal. It checks expiry,
frame, host capture age, duplicate source references and all extents before
publishing the identity. A failed publication changes no caller output.

This core does not parse the canonical message, calculate a schema digest,
verify tensor layouts or pin the actual buffer. The model-provider adapter
must do those operations before publication and retain sealed input leases for
the proposal's lifetime. The core cannot establish that an arbitrary host
caller truthfully labelled unvalidated bytes as a trusted proposal.

Observation reuse is a closed, explicit tool-profile decision. There is no
default policy. `latest_required` requires exact equality with the current
source snapshot. A new sample makes an earlier proposal stale, including while
its validator is running. This is appropriate when the tool requires the latest
available input at local admission.

`bounded_age` allows a slower model to use its original immutable sample while
new samples arrive. It requires the same source capability, source incarnation,
source authority epoch, clock identity/epoch, frame, calibration generation and
transform generation as the current source. The current sample sequence may
advance, but may not precede the retained sequence. The proposal's original host
capture time supplies the age bound; newer samples never refresh its age or
expiry. This policy does not reinterpret an old buffer using a newer calibration
or transform.

`source_generation` retains the catalogue meaning of source incarnation.
The core adds host-only `sample_sequence`, `calibration_generation`,
`transform_generation` and `source_authority_epoch`; these are not new canonical
`proposed_record` fields. New samples strictly advance the sample sequence
within an incarnation. Restart requires a higher source generation and may
reset the sample sequence. Calibration/transform/authority revisions cannot
decrease, including across a source restart. A capability change requires a newer
source authority epoch. Changing frame must strictly advance transform
generation; changing clock identity must strictly advance source incarnation.
For the same clock identity its epoch cannot decrease. These enforced rules
prevent an A-to-B-to-A frame or clock switch from making an old snapshot valid
again. Every snapshot also retains its original source
timestamp and host capture time. A model-supplied time cannot extend freshness.

`fence_source_authority` advances a permission revision immediately without
requiring a new sensor sample. Thus an authorization withdrawal or rate limit
invalidates old proposals under both policies, independently of sampling rate.
Source revocation, restart, clock/frame change, calibration/transform change and
target provider fencing likewise reject an old proposal under both policies.
Trusted adapters must report these events before further admission; comparing
metadata cannot detect an unreported external change.

The adapter must retain the **original** sealed-buffer lease for a bounded-age
proposal and separately check that source capability and buffer authority are
still live and permit this use. This core stores provenance metadata; it does
not hold that lease, implement general information-flow tracking or prove that
the referenced buffer still exists. Allowing sample advancement never waives
source permission checks. Execution still requires an atomic target admission
against the current authority, physical state and selected observation policy.

## Read, local admission and delivery recovery

`read` verifies the exact reader actor and proposal capability, copies a
detached record and invokes its output sink without holding the registry
mutex. A false or throwing sink consumes nothing. The integrating carrier must
preflight the complete guest output range and hold its disclosure authority
lease; the sink must not retain a reference to the temporary record.

`accept` follows this sequence:

1. Resolve the arbiter's owned request-key session and exact readable proposal
   capability. Reject producer self-approval, wrong target/action, stale
   bindings, expired deadlines and exhausted optional rates.
2. Require the next request sequence. Reserve both the arbiter and proposal;
   concurrent or reentrant acceptance receives `again` without consuming work.
3. Copy the proposal and release the mutex. Run a bounded, nonblocking,
   side-effect-free validator against the target schema and arguments. This
   callback must never publish a command or perform an external effect.
4. Reacquire the mutex and recheck grant retirement/revision, tool/target
   epochs, observation policy/lineage/freshness, host time and optional rate. Changed state,
   revocation, timeout, a failed validator or an exception releases reservations
   without consuming the request sequence, proposal or rate.
5. Commit an inert local admission receipt and mark the proposal admitted.
   Only this step charges an optional rate. The next successful sequence
   acknowledges the previous retained receipt.

The receipt uses the catalogue's `admitted` state and `locally_accepted` effect.
It does not mean a physical effect was queued, executed or completed. Internal
epoch evidence is host metadata, not an addition to the proposed wire record.
There is deliberately no `execute` method.

The last identical accepted request returns the same receipt without another
validation, effect or charge. A differing request with that key, an older key
or a skipped sequence fails. A failure while copying an already committed
receipt into guest memory does **not** roll admission back: the guest retries
the same request to recover its result. Historical receipt recovery may still
succeed after proposal expiry or retirement, provided the arbiter's current
read/accept authority remains. That history cannot authorize execution.

Before any real target effect, the target provider must atomically recheck
current caller/target authority, physical resource ownership and controller
lease epoch, provider generation, arguments and physical limits, observation
requirements and deadline at the same point where it admits the effect.
Enqueuing an already approved command and checking only its old receipt later
is expressly insufficient. A native provider with an unknown execution outcome
also needs its own deduplication/reconciliation receipt; this core makes no
exactly-once claim across native ROS or hardware connections.

## Authority followed by explicit usage limits

An ordinary grant omits `calls`, allowing stable execution within its exact
scope. `attenuate_reader` can later install a finite window limit under a
strictly newer authority epoch. The transition is zero authority, explicit
grant, stable rights, then explicit usage limits. A zero call allowance rejects
every new admission; waiting for another window does not turn zero into
unlimited execution.

Once a finite rate exists, later updates may only lower its call count
and lengthen its window. It retains the consumed count and original window
start; changing configuration cannot refund use. Revoked reader slots remain
tombstones, so reopening cannot reset rate or request history. Capacity limits
remain independent of these optional rates. No call is silently delayed,
dropped, partially executed or converted into a physical stop on quota failure.

`retire` and per-reader/source/tool revocation stop local admission only.
They do not supply an autonomous watchdog, interrupt an arbitrary provider
callback or stop a robot. Teardown retires the core, joins callers and only
then destroys its fixed storage. Forked children fail before acquiring
inherited mutexes.

## Qualification scope

[`proposal_admission.cpp`](../../test/0084.ros-runtime/proposal_admission.cpp)
tests the core's exact authority, immutable extents, all eight physical target
action classifications, receipt recovery, stale observations, independent
epoch fences, callback reentry/revocation, three rate windows, explicit zero,
progressively tighter rate limits, concurrent acceptance and fork isolation. The two
observation policies additionally cover fast-sensor/slow-model reuse, expiry of
the original sample's age, calibration/transform/clock/incarnation fencing,
source authority changes without new samples, frame/clock A-to-B-to-A rejection,
and the two policies' distinct behavior when sampling advances during validation. A
test-time allocation trap exercises the normal publication/read/admission/
retry/retirement path without `operator new`; TSan retains its own allocator.

[`proposal_admission_checks.py`](../../tools/ros_scene/proposal_admission_checks.py)
records compiler/run results, source hashes, CPU affinity, cgroup identity and
boot identity. New runs save the actual header/test/runner source text in
`output/source` and compile using that snapshot, checking both the snapshot and
workspace hashes afterwards. The older `proposal-admission-core-complete`
receipt predates this change and is historical hash-only evidence; its original
source text was not archived and must not be represented as a reproducible
source snapshot. The runner requires the existing 64 GiB, zero-swap gate and optionally
executes AArch64, ARM32 and i686 cross builds. It records this as isolated native
core evidence. No core result increases the registered import count, qualifies
the guest memory boundary or establishes robot safety or model correctness.
