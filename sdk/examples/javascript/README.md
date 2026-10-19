# JavaScript lessons

Read these English-commented lessons in order. `01-raw.mjs` explains exact ABI
arguments and eight-byte little-endian slots. `02-codecs.mjs` runs as a pure Node.js
codec exercise. `03-node.mjs` introduces publishers, subscriptions and a bounded
polling executor. `04-resources.mjs` shows typed services, parameters, TF, lifecycle
and clocks with independently authorized aliases.

Continue with [05-runtime-relay.mjs](05-runtime-relay.mjs), the supervised copied
binary driver. [06-async-application.mjs](06-async-application.mjs) and
[07-sync-application.mjs](07-sync-application.mjs) assemble every supported family
with Context, explicit Executor/Future/CallbackGroup/Timer and parameter hooks.
[08-custom-codecs.mjs](08-custom-codecs.mjs) applies a generated bounded custom
topic codec. Both full application plans default to empty and open no authority.

Lesson 2 is a pure standalone exercise. Lesson5 is a host driver command and
requires a separately supervised real Worker; it is not JS inside Wasm. Other lessons export functions for an
explicitly supplied authority-aware Wasm bridge and reserved memory. They neither
instantiate native ROS nor provide a JS engine inside uwvm. Follow the
[bridge contract](../../javascript/README.md) before calling them. Every alias
needs an explicit runtime CLI grant; default-deny 13 is expected without one.

Run the codec exercise from repository root:

```sh
node sdk/examples/javascript/02-codecs.mjs
```

The printed bytes prove serialization only. Actual Wasm guest/native ROS tests
are separately recorded in the authorized cgroup matrix.

Lesson5 explicitly selects backend1/2 and positive/absent mode, reads Worker
binary stdout from fd3 and writes Worker binary stdin to fd4. Driver control
stdin and marker stdout stay separate from those binary pipes and audit stderr.
Send P only after independent native MATCHED, then at most200 single-byte R
gates, each causing one bounded poll. Its positive markers are
SDK_ENDPOINTS_CREATED, SDK_FINAL_SENT, repeated SDK_POLL_AGAIN when11,
SDK_DELIVERED and PASS_SDK_DELIVERY. Its exact Strings are warmup-js-relay,
roswasm-js-relay-out and roswasm-js-relay-in (canonical19/24/23 bytes). Explicit
pub/sub aliases fix `/uwvm_sdk_relay/pub` and `/uwvm_sdk_relay/sub`; an ungranted
alias must return13 with protected slot/output. After exact delivery the driver
checks closes/stale9 and shutdown acknowledgement. Native final Q needs actual
Workerexit0 as well. The absent row prints PASS_SDK_DEFAULT_DENIED after three
alias refusals; independent zero native effects remain an external requirement.
The task owner's serial receipts have qualified actual ROS1/ROS2 relay C
compilation/import signatures and80 cases executing the real compiled Wasm
dispatcher with mathematical ROS replies (`/tmp/uwvm-sdk-local-js-wasm-20261014-r4`).
Those guest-local models do not authenticate a Runtime/Worker/Gateway session.
Independent native ROS relay delivery remains pending.

The owner runs current Node tests serially in fresh `/tmp` snapshots with low
memory. Deferred service calls still execute the original bounded synchronous
RPC when the caller explicitly advances a pass. Timers require caller-supplied
readiness and optional already-granted Clock; no thread or implicit clock is
created. Inspect every Context cleanup item. Cancellation/timeouts do not prove
rollback of an admitted native effect. Read [the complete API guide](../../javascript/API.md)
and [relay launch/authority contract](../../javascript/RELAY.md).

Subscription/Service/ActionServer direct `spinOnce` calls hold a local ownership
lease through callback or awaited handler and response. Nested poll, close or
revoke is17 before any import, including retirement attempted through the same
Capability or Node/Context. A refused group cleanup leaves its owners available
for checked cleanup after the pass. Callbacks retain the same callable and the
resource as `this`; reassignment affects the next pass. Exceptions/rejections
release leases in finally. An independently owned resource and an exact-ID
responder request remain callable after copied poll completion.

Action feedback/result validate the requested full-width BigInt goal ID. A
canonical reply for another ID returns local22/value:null and `runtimeStatus:0`,
preserving copied bytes/length metadata for diagnosis without replay. A completed
terminal result retires the native goal; subsequent own cancellation can be9.
Cancel before consuming the result or branch on pending/terminal state instead
of interpreting stale9 as a successful false cancellation Bool.

## Complete command entry

[run.mjs](run.mjs) provides actual invocation of the exported lessons. It accepts
only explicit inputs and never creates fake imports/memory or starts actors:

```sh
node sdk/examples/javascript/run.mjs sync 2 /absolute/plan.json /absolute/trusted-setup.mjs
node sdk/examples/javascript/run.mjs relay 2 /absolute/plan.json
```

The trusted synchronous setup exports `createAdmittedInstance(version)` returning
the real `WebAssembly.Instance`; the runner derives the exact bridge/scratch and
creates Session. This is an embedding interface, not an implementation of native
imports. Its host module has ordinary Node privileges, so select it explicitly
and pin/trust it as part of the supervisor's launch setup. Relay uses fd3/fd4
already connected to the authenticated Worker. No environment inheritance or
Runtime flags are inferred by the command.

`plan-empty.json` demonstrates application Context/Node/executor/shutdown with
zero ROS grants. `plan-ros1-resources.json` and `plan-ros2-resources.json` explicitly
request every supported resource family; these are opt-in examples, not launch
defaults or prepared Runtime policies. Every named alias requires its complete
trusted native identity/QoS/limits contract. Native Trigger/Action/parameter/
TF/lifecycle providers must be independently running at those configured exact
resources. A client must not rely on this same caller-driven executor to answer
its own synchronous RPC; use an independent native/provider actor and preserve
the bounded timeout result. Missing providers and grants remain visible statuses.

The plan selects `raw`, `node`, `resources`, `application` or `custom`. Legacy
node/resources recipes require explicit synchronous ROS2 setup; application
supports both backends/transports. Raw selects one explicit `rawAlias` and closes
its successful handle. Application accepts an alias map, at most64 closed public
method actions and at most200 explicit executor passes. Signed/unsigned64-bit
JSON arguments use exact `{"$i64":"..."}` / `{"$u64":"..."}` decimal tags.
Generic exact-operation Resource inputs use `{"$bytes":[0,1,...]}` with at most
4080 explicit unsigned bytes. The supplied method is selected from the current
ABI table before alias opening and still needs its own grant.
`callDeferred` reports a local pending Future and requires a subsequent explicit
spin action/pass to complete. Timer readiness is an explicit plan boolean.
Custom requires an explicit generated module exporting `codec` and separate
`aliases.custom.publish/subscribe`; the registered native/codec identity is
still an external prerequisite. No unsupported resource is silently substituted.

The runner reports each action status and every close/shutdown result. Nonzero
Runtime action/cleanup statuses set a failing exit; refused factories report
public13/95/etc while cleaning all earlier acquisitions. Output is application
data, not Runtime audit evidence, and no operation success alone claims delivery.

## Explicit unified events

The next runnable lesson uses an independently opened Source and `subEvents`
observer, checks a single poll, closes the Source, verifies its explicitly
permitted closed event and checks observer cleanup. See
[unified event v1](../../docs/unified-events.md) for exact aliases, header,
limits, backend entries, native peer prerequisites and policy preparation.
Metadata-only observation grants no payload permission. New38/43 inventories
and event source fixtures await the unified serial test stage; historical
receipts keep their original revision and narrower scope.
