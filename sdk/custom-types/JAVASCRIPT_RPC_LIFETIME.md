# JavaScript generated RPC ownership and failure handling

This guide describes the current JavaScript bounded-interface template and its
public synchronous `Session`/`Capability` path. It is a source contract for typed
custom services and actions. The accompanying tests are unexecuted Node contract
fixtures. They do not establish a compiled Wasm engine, native ROS delivery,
debugger behavior, sandbox containment, ABI parity, or performance.

The template is
[`interface_templates/bounded_interfaces.mjs.in`](interface_templates/bounded_interfaces.mjs.in).
A real catalog emission supplies its concrete exported classes and complete
record codecs. Applications import the generated `bounded_interfaces.mjs`,
not the `.in` template. The reviewed cleanup template is a new input to fresh
products; an older vendor product does not change when the template changes.

## 1. Select the complete build and permission boundary

Use one explicit source freeze, catalog, generator and template revision. The
small teaching catalog is
[`interface-examples/catalog.preparation.json`](interface-examples/catalog.preparation.json).
Its ROS2 `demo_rpc/srv/Increment` has a complete `int32 value` request and response;
`demo_rpc/action/Count` has `uint32 count`, `total`, and `progress` components.
The generated names used below are `Ros2Demo_rpcSrvIncrementClient`,
`Ros2Demo_rpcSrvIncrementServer`, `Ros2Demo_rpcActionCountClient`, and
`Ros2Demo_rpcActionCountServer`. A vendor catalog has its own generated names
and component types; do not substitute the demo codec for a vendor schema.

The teaching catalog deliberately has no native identity. `--guest-only`
products can exercise complete canonical records and application structure.
Native ROS requires the real bound root and component identities, matching
Gateway registration, and independently granted aliases. Native layout, a
similar type name, an FNV value, or successful JavaScript decoding cannot supply
that identity. The [interface example instructions](interface-examples/README.md)
and [bounded format](BOUNDED_V2.md) describe the existing catalog/binder path.
Do not invent native hashes or grant a generic namespace to make a test pass.

The real same-Wasm engine uses `createGuestSession()` from
[`../javascript/guest.mjs`](../javascript/guest.mjs). Its C builtin and actual
ROS imports must be compiled into that same Wasm principal. A trusted external
embedder may supply the separately documented `Session` bridge. A Node fixture
with a `CopiedTransport` delegate is a local model of that interface, not an
implementation of ROS and not evidence of a same-Wasm identity. The generated
constructor requires a genuine selected `Session`; a duck-typed object is not
a supported substitute. Obtain every owner through `Session.open`; do not
construct or fabricate a `Capability` to work around a refused alias.

Every alias independently selects backend, operation, native type, resource,
QoS, quota, deadline and actual principal in Runtime policy:

| Facade | Required caller-supplied roles | Optional role |
| --- | --- | --- |
| Service client | service-call source | independent completion observer |
| Service server | advertise, respond | none |
| Action client | send, cancel-own, feedback, result | cancel-any |
| Action server | advertise, respond | none |

Aliases in one facade must be distinct, nonempty, NUL-free strings. Default
policy can refuse open with13. An imported SDK, generated type or local facade
is not a grant. Cancel-own never implies cancel-any; only the explicit latter
operation admits zero as cancel-all. Containers do not establish the Wasm
principal's finer resource permissions; actual Runtime/ROS grants still apply.
This facade is an API for this capability ABI. It is not a source-level port of
rcl/rclcpp/rclpy or a promise that an original native `.so`/`.a` can be loaded.
Source-port and rebuild native dependencies for the selected Wasm ABI, or keep
them in an explicitly configured trusted native adapter with its own audit.

## 2. Construction has an explicit outer failure object

Before its first open, the template reserves the owner array, per-role ledger,
open arguments, duplicate-handle refusal and a `RpcConstructionFailure` carrier.
Each returned genuine owner is stored before the next validation or open.
All roles are acquired before server advertisement. A later open failure,
service/action advertisement failure, or client `EventBinding` constructor
failure uses the same owned rollback route.

Initial argument/backend/capacity refusal before acquisition can still be an
ordinary exception. Once this owned construction path fails, the thrown object
is the preallocated **outer** `RpcConstructionFailure`. It is not the original
thrown value. Compatibility code that formerly compared the caught object to
an original error must inspect `carrier.primary` instead. The original is
never decorated, unfrozen, coerced to a status, or used as a truthiness test.

| Carrier member | Meaning |
| --- | --- |
| `primaryKnown` | Whether construction recorded an actual primary throw |
| `primary` | Exact original value, including `undefined`, `null`, `false`, `0`, `-0`, `0n`, `NaN`, a Symbol, or a frozen object |
| `cleanupAttempted`, `cleanupCalls` | Group-cleanup invocation facts, including explicit later retries |
| `cleanup` | Last fully returned cleanup report, or initially `null` |
| `cleanupReturned` | A cleanup report has returned; cumulative, not a proof that all historical cleanup succeeded |
| `cleanupThrew`, `cleanupException` | Cleanup itself threw; the first exact value is retained independently |
| `diagnostics()` | Copied frozen role facts, without exposing capabilities or granting authority |
| `retryCleanup()` | Explicit cleanup of still-retained owners only; no open, advertisement, request, goal, or submission replay |

`cleanupReturned` and `cleanupThrew` may both become true over several calls.
A **delegate throw caught into a returned cleanup report** sets
`report.cleanupThrew`; it need not set `carrier.cleanupThrew`. The latter
records a failure escaping the cleanup/report operation itself. Preserve both.
Do not write `if (failure.primary)` or `if (report.cleanupException)`: those
checks lose valid falsey thrown values. `primaryKnown` and `cleanupThrew` are
independent boolean facts.

An open that throws without returning an owner is recorded as
`openInvoked=true`, `openThrew=true`, `openReturned=false`, `acquired=false`.
The template cannot invent its full handle, close an unknown owner, or prove
that the underlying bridge caused no effect. This successor does not repair
all unknown-after-effect behavior of a general backend or `Session.open`.

## 3. Checked cleanup preserves every role and every failure

Use `closeChecked(primary)` for normal release or explicit
`revokeChecked(primary)` when revocation is intended. They return a frozen
report. Construction rollback uses close, not revocation. JavaScript GC does
not call either operation.

Every acquired distinct owner is visited in reverse order, even if an earlier
cleanup delegate throws `undefined`, another primitive, or a frozen object.
All report rows exist before the first cleanup delegate. A later report-copy
failure does not erase the private owner ledger; a construction carrier can
still expose diagnostics and explicit cleanup of a known retained owner.
This is not a promise of recovery from terminal engine OOM or process failure.

| Report member | Interpretation |
| --- | --- |
| `primary` | Unchanged caller-supplied primary diagnostic value |
| `blocked` | Local callback/cleanup guard refused the group; no delegate in this nested pass |
| `invoked` | Number of actual Capability cleanup method invocations in this pass |
| `attempted` | Invoked owners whose genuine SDK-private lifetime reached its captured delegate attempt |
| `retained` | Acquired owners whose SDK lifetime still permits explicit cleanup |
| `failed` | Roles with a sticky first cleanup failure, including earlier passes |
| `cleanupError` | Numeric nonzero summary for failure/refusal; exactly0 alone is insufficient if other evidence is inconsistent |
| `cleanupThrew`, `cleanupException` | Exact first delegate-throw fact/value retained in the report |
| `statuses` | Complete per-role facts, frozen with the report |

Each status row retains the original role/alias/full BigInt handle, acquisition
and open facts, actual call count, `inFlight`, `returned`/`status`, `threw` and
exact `exception`, `attempted`/`retained`, external retirement and first-failure
facts. A snapshot taken during synchronous delegate reentry can legitimately
show `inFlight=true`, `attempted=true`, `returned=false`, `status=null`. Do not
turn it into a completed status or issue another close.

The SDK's read-only `Capability.closed` mirror becomes sticky at the captured
JavaScript delegate attempt. It does not prove that the original Host import
ran, that a robot command completed, or that external effects were undone.
Returned107/110 or a throw after that point never authorizes replay. A local
preparation/busy/callee-capture failure before entry leaves the genuine owner
retained. An application may explicitly retry cleanup after resolving that
specific local preflight condition. Failure classes or errno alone cannot
identify which side of this boundary occurred. Historical failures remain in
the later report even when that explicit cleanup finally succeeds.

A synchronous nested close/revoke during the template's cleanup returns
Conflict17 with `blocked=true`, without another delegate. Capability-backed
facade operations refuse17 during this local cleanup guard. An already-entered
individual `Capability` itself reports closed/BadHandle9, including from its
own close delegate. The outer pass still attempts all remaining owners.

A backend returning the same full handle for two aliases violates distinct
owner expectations. The template records `duplicateOf`, closes the unique
original once and retains the duplicate wrapper as conflicted/suppressed. It
does not write the read-only closed mirror or manufacture a second ambiguous
close. Repeating `retryCleanup()` does not repair this anomaly; preserve the
facts for the backend/Runtime owner instead of looping or inventing cancellation.

## 4. A complete client lifetime with exact error retention

This reusable application step uses a caller-supplied genuine session and
explicit policy aliases. Keep the returned evidence strongly reachable until
the controller has recorded it. Reporting may fail independently; it must not
change the business/cleanup result. This is application code to adapt to the
actual generated component, not a new transport or permission source.

```js
import {RpcConstructionFailure, Ros2Demo_rpcSrvIncrementClient}
  from './generated-rpc/bounded_interfaces.mjs';

export function incrementOnce(session, callAlias, input) {
  // Reserve the evidence carrier before the first constructor/open.
  const evidence = {primaryKnown: false, primary: undefined,
    construction: null, cleanup: null, cleanupThrew: false,
    cleanupException: undefined, value: null, ok: false};
  let client = null;
  try {
    client = new Ros2Demo_rpcSrvIncrementClient(session, callAlias);
    evidence.value = client.call({value: input}); // one actual call, full decode
  } catch (error) {
    evidence.primaryKnown = true;
    evidence.primary = error;
    if (error instanceof RpcConstructionFailure) {
      evidence.construction = error; // preserves exact error.primary separately
    }
  } finally {
    if (client !== null) {
      try { evidence.cleanup = client.closeChecked(evidence.primary); }
      catch (error) {
        evidence.cleanupThrew = true;
        evidence.cleanupException = error;
      }
    }
  }
  const r = evidence.cleanup;
  evidence.ok = !evidence.primaryKnown && !evidence.cleanupThrew &&
    r !== null && !r.blocked && r.cleanupError === 0 && !r.cleanupThrew &&
    r.failed === 0 && r.retained === 0;
  return evidence; // process/controller success is permitted only when ok === true
}
```

A command entry must return/process-exit nonzero when `ok` is false, including
business success followed by cleanup failure. Keep an original thrown `false`
or frozen error beside cleanup diagnostics; logging a warning and returning0
is not checked cleanup. Do not use JSON serialization as the only retention
mechanism: BigInt, Symbol, cyclic and primitive throws require the exact live
evidence object. In a constructor catch, inspect `construction.primaryKnown`,
`construction.primary`, its cumulative cleanup flags, and `diagnostics()`.
Only an explicit application/operator decision may call `retryCleanup()`; do
not put it in an automatic retry loop. There is no automatic RPC replay.

For asynchronous service completion, construct with both `callAlias` and a
fresh independently granted observer alias. `submit({value})` calls the real
`service_submit` once and returns a nonzero full-width BigInt admission ID.
The caller advances `pollCompletion()` in bounded steps. The existing envelope
decoder checks backend/source, instance/session, next sequence and correlation,
then the generated response decoder accepts the entire component. A numeric
terminal error is a completion failure, not permission to resubmit. Again11
may be an idle poll or rate limit; arrange another explicit caller-driven poll
without replaying submit. Keep the facade through the completion lifetime and
always check final cleanup.

## 5. Service, action and vendor application flow

A service server acquires distinct advertise/respond roles and advertises
once. In each caller-driven step, `await server.spinOnce(handler)` performs one
poll, complete request decoding, callback and response. Its lease spans the
`await`. A handler returns the one response; it must not also respond manually
to that service ticket. A thrown handler value releases the local lease in
`finally` while retaining the delivered ticket for an explicit manual response.
Nested poll/cleanup refuses before dequeue or retirement. Application scheduling,
deadline and final checked cleanup remain explicit.

An action client acquires four roles, then `sendGoal` once. Retain the exact
nonzero BigInt goal ID. Feedback/result envelopes must match that ID. Only a
fully decoded kind2/3/4 terminal result retires the local goal; feedback and a
cancel acknowledgement do not. Stronger `cancelAny` needs its own fifth grant.
An action server's handler receives a copied typed goal and ID. It may call
`respondFeedback(id, value)` and then `respondResult(id, 2|3|4, value)` for that
exact delivered goal. A successful feedback retains it; terminal response
retires it. Failed encoding/refusal does not imply successful native delivery.

For Unitree, Booster, LimX or another catalog, retain all original constants,
defaults, nested fields, bounds, canonical identity and metadata in the vendor's
actual complete generated records. Use the vendor generator's actual exported
facade/component names and real official/native bindings. Distinct aliases
select the intended service/action channels, each QoS and the correct physical
or simulation policy. A simulation label is not hardware authority. Follow the
vendor's actual deployment tutorial and use an independent native peer to
verify typed requests, feedback/results, denial and cleanup. A passing demo
Node fixture gives that vendor product no automatic generation/native coverage.
The separate same-Wasm owned-RMW API has its own raw12/receipt/ACK contracts;
these legacy custom RPC facades do not replace or qualify that API.

## 6. Fresh Linux preparation and the contract fixture

Generating proposed Source in a fresh private temporary directory is an
implementation step and may take place locally. Preserve its actual tool,
inputs, outputs and provenance; generation is not a test result. All test
execution, including Node model fixtures, compiler/runtime validation, Guest,
ROS, DBG and performance work, belongs to the coordinated SSH Linux lane in the
existing64GiB/swap-zero cgroup. Admit the complete generated Source cohort again
before testing it. The test owner must bind that actual directory/device/inode and process membership,
apply the current low aggregate resource budget and deadline, bound/drain
stdout/stderr, retain pidfd/descendant/EOF/exit evidence, and remove only owned
temporary files. These instructions do not create a new cgroup or authorize
another heavy window. The fixture starts no subprocess or network service.

Within a fresh complete source freeze, first have the authorized generation
owner run the actual selected generator against the real teaching catalog:

```sh
/actual/pinned/python3 -B /actual/frozen/source/sdk/tools/custom_types.py \
  /actual/frozen/source/sdk/custom-types/interface-examples/catalog.preparation.json \
  --guest-only --out /actual/fresh-owned/generated-rpc
```

This is an invocation shape, not a statement that generation ran. The generator,
helper loader and tool/stdlib execution-byte provenance must be independently
admitted by that owner; `-B` alone disables pyc writes, not ambient pyc reads.
Check `generation.json` output hashes and its exact
`source_inputs.facade_templates`, catalog and generator bindings. In particular
the JavaScript template must be the current reviewed cleanup successor. Do not
patch a generated file to make a failed test appear current.

Preserve source-relative imports: place those exact generated products at
`/actual/stage/sdk/custom-types/generated-rpc/` beside the actual complete
`/actual/stage/sdk/javascript/` tree. The test itself is a new file under
`/actual/stage/sdk/custom-types/tests/`. Run serially under the same Linux owner:

```sh
/actual/pinned/node \
  /actual/stage/sdk/custom-types/tests/rpc_cleanup_real_session_fixtures.mjs \
  /actual/stage/sdk/custom-types/generated-rpc/bounded_interfaces.mjs
```

The test imports the real SDK and generated module; it does not generate,
rewrite, eval, substring-test or substitute either one. Its recording binding
implements the public `describe()/invoke()` shape and owns exact-size copied
snapshots. Real `Session.open` constructs every capability, and the actual raw
preparation/entry protocol determines its read-only closed state. Failures
throw and give Node a nonzero exit; the final model marker appears only after
all assertions complete. The recorded Node execution completed all 34 cases
with `RPC_REAL_SESSION_CONTRACT_MODEL_PASS`; NativeROS/Wasm/DBG remain NOT_RUN.
See the [actual Linux model execution record](../../documents/ros/continuation-20261018-evidence/js-rpc-real-session34-actual-linux-root-20261018-r1/README.md).
That marker must not be used as a native or sandbox acceptance gate.

The authored cases inspect actual call counts, order, full IDs and returned
facts for falsey throws (including -0,0n and NaN), frozen primary/cleanup identity,
reverse independent cleanup, explicit retained cleanup without open/RPC replay,
real synchronous transport-busy refusal, same-class post-entry throw, closed
read-only behavior, reentry guards, duplicate handles, both advertisement
constructors, full service submit/completion, awaited handler failure and
manual exact-ID response. Two scoped local JS fault injections additionally
exercise the actual `EventBinding` constructor (an inherited source setter)
and report assembly (`Object.assign`). Each restores the original object in
`finally`. They are explicit model faults, not claims of ordinary native
malformation, allocator OOM, or runtime hardening against arbitrary mutated
builtins. No fixture writes a capability's `closed` mirror.

## 7. Build the real Wasm application and qualify it separately

The [closed RPC preparation recipe](interface-examples/build-recipes.json)
and [`interface-examples/prepare.py`](interface-examples/prepare.py) describe the
existing fresh staging/tool selection. Embedded JS must include the actual
application, generated records/interfaces and every SDK import in the closed
QuickJS manifest. Use the same-Wasm session builtin, exact matching C imports,
selected ROS backend, bounded heap/stack/jobs/memory and actual final validator.
The [guest producer guide](../javascript/guest/README.md) details the fixed
engine source lock, explicit compiler/sysroot/archive paths and serial jobs.
A preparer's report is not compilation. The existing reusable `example.mjs`
contains older short error-handling examples; use the explicit carrier/lifetime
handling above for exact falsey-primary and independent-cleanup retention.

After an actual build, retain final complete Wasm validation/import signatures,
memory/start/entry checks, source and dependency closure, real native registry,
finite aliases and independent native positive/default-denied interactions.
Verify that a refusal does not cause native effects through an independent
witness. Validate cleanup errors and stale handles with actual Runtime results;
Node model live-set deletion cannot establish them. DBG requires actual current
stop, source lines/frames/typed values and caller source bindings; a section or
symbol name alone is insufficient. Hardware timing, VTune hotspots/counters/
microarchitecture and cross-architecture QEMU remain separate future test rows.

Use the current [WSEA procedure](../../documents/security/wsea.md) for actual
artifact admission. Finish linking, debug/custom-section changes and any strip
or embedding before signing the **entire final Wasm** with the operator's
existing trusted keys/policy. Do not create keys/CA or trust roots for this
lesson. This tutorial explicitly selects ES256. Existing six-field module
rules permit ES256 only. Other profiles require an explicit local per-module
algorithm allowlist and qualification for the actual current product, build
and backend; no automatic fallback or downgrade is implied. New Source or a
new profile cannot inherit the historical ES256-only VM/verifier test results.
`--secure module <path> <policy>` selects exactly one rule;
shared policies/multiple modules require explicit
`--secure named-module <path> <policy> <expected-id>` for each actual module,
with all VM options before `--run`. Signed capability ceilings must fit the
local rule; Runtime/ROS resource grants remain independent. A changed artifact
or DBG reload requires the actual verified-load procedure and fresh signature/
identity binding. A valid old signature, warm cache or a matching pathname
cannot substitute for current final-byte admission. Consult the current WSEA
CLI rather than assuming a future algorithm, policy or reload interface.
