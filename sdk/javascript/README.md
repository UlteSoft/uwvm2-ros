# JavaScript roswasm SDK

This directory provides canonical codecs, the complete ROS 1/ROS 2 ABI v1
inventory, a validating raw adapter, and familiar client-library objects. Ordinary
JavaScript source alone is **not an executable uwvm guest**. Choose either the
existing explicitly supplied bridge/command-relay path, or the additive
[embedded Wasm engine](guest/README.md). The latter compiles pinned QuickJS,
closed ESM source and copied C imports into the application's actual Wasm, so
user logic and callbacks execute there. Both paths reuse `Session`/`Node` and
the same resource facade. Neither SDK objects nor imports grant sandbox rights.
The new embedding remains source-prepared until actual build and Runtime
evidence is retained; external Node models do not qualify its execution.

## Learn in four steps

1. Read [the raw example](../examples/javascript/01-raw.mjs). It shows exact import
   arguments, UTF-8 byte offsets and an eight-byte output slot.
2. Run `node sdk/examples/javascript/02-codecs.mjs` from the repository root. This
   pure example explains canonical String, Twist, service and parameter bytes.
3. Read [the Node example](../examples/javascript/03-node.mjs). Its caller supplies
   the trusted bridge and reserved scratch range. Callbacks run only during
   `Executor.spinOnce()`.
4. Read [the resource example](../examples/javascript/04-resources.mjs), then the
   [shared wire specification](../docs/wire-format.md) for typed services,
   parameters, actions, TF, lifecycle and clocks.

## The bridge boundary

This section describes the original external-host bridge. For JavaScript
inside Wasm use [`guest.mjs`](guest.mjs), the explicit copied transport and
[progressive guest lessons](../examples/javascript_guest/README.md). That path
does not fake `WebAssembly.Memory` or require a host JS function to implement ROS.

The adapter accepts `{memory, functions}`. `memory` is the exact
`WebAssembly.Memory` whose bytes the bridged guest imports read and write.
`functions` contains **every** name in the selected ABI table, with exactly the
documented parameter order. A bridge might expose thin Wasm guest exports around
its imports and map them into this table. All handles, offsets, lengths and
capacities cross JavaScript as `BigInt`; memory indices and returned status codes
use unsigned `Number` integers. Never convert a handle through `Number`.

The embedder must establish the guest identity, preserve uwvm authorization, and
reserve scratch memory against that guest's allocator. Neither an arbitrary
`WebAssembly.instantiate()` import object nor a JavaScript implementation that
directly talks to ROS establishes this property. A forged host bridge can bypass
policy, so it is outside the SDK's trusted execution claim. The additive
[command relay](RELAY.md) now provides source for a persistent real Wasm command
and copied asynchronous SDK; its compilation and actual delivery remain pending.
It runs JavaScript in an external Node driver and introduces no JS engine inside
Wasm. It neither forges a Memory/Instance nor connects Node directly to ROS.

For example, an established embedder can call:

```js
import { Session, Node, StringMessage } from './sdk/javascript/roswasm.mjs';

// trustedBridge and reservedScratch come from the authority-aware embedder.
// reservedScratch = {offset: 4096n, inputCapacity: 4096, outputCapacity: 4096}
// is valid only if the embedder has exclusively reserved that entire region.
const node = new Node(new Session(2, trustedBridge, reservedScratch), 'talker');
try {
  const publisher = node.createPublisher('greeting-pub', StringMessage);
  const status = publisher.publish('hello');
  if (status !== 0) handleRuntimeStatus(status);
} finally {
  node.close();
}
```

This snippet deliberately leaves bridge creation to the trusted embedding layer.
It is not a standalone command-line uwvm input. Emscripten/Javy or another JS
engine can be integrated only when its generated Wasm imports and bindings
preserve the same ABI and policy; a toolchain name alone does not add that support.
Use AssemblyScript, C/C++, Rust or TinyGo for the SDK's direct compiled-guest path.

The repository also contains [bridge.c](bridge.c), a selectable ROS 1/ROS 2 Wasm
guest forwarding bridge with a statically reserved scratch region. The accompanying
bridge loader still requires the authority-aware embedder to supply actual ROS
imports and authenticated guest identity. It never implements those imports in
JavaScript. Compilation and integration of this new bridge are separately pending.

## Safe ownership and polling

`Session` uses only memory 0 and an exclusive scratch layout of input capacity,
an eight-byte length/handle slot, then output capacity. It recreates views after
calls because Wasm memory growth can detach old buffers. It rejects reentrant
scratch use. The raw layer remains available for explicitly selected memory
indices, but the runtime is responsible for enforcing each call's memory bounds.

Read `Reply.required` only when non-null. Code 90 may leave the slot untouched,
or may write a required length after consuming an event. The SDK uses a
`UINT64_MAX` sentinel and never automatically grows, retries, or replays a call.
Code 11 means an empty queue or quota refusal; an executor polls each entity at
most once in a snapshot of the registrations present at pass entry. A callback
may add entities, but they run only in the next outer pass. Nested `spinOnce`
returns `[Status.CONFLICT]` without polling. Callback exceptions propagate to the
caller while releasing the busy guard in `finally`; no partially completed pass
is retried automatically. The application explicitly schedules another iteration.

Close each owned handle explicitly. Local validation, a busy copied transport or
a busy relay can refuse before attempting its underlying delegate; that owner
stays live for a later explicit cleanup. Once the captured bridge/native/write
delegate is attempted, the wrapper stays retired even if it throws or returns
107/110. Never infer safe replay from an error class or errno. This boundary
proves a JavaScript delegate attempt, not actual Host import entry or successful
native cleanup. JavaScript GC never invokes imports. Timeouts do not establish
that an admitted native side effect was rolled back.

Revoke uses the same preparation/attempt rule. `Capability.closed` and
`AsyncCapability.closed` are read-only diagnostic getters; assigning `false`
cannot revive an attempted owner. `session` and `handle` remain writable API
properties, and internal preparation exports do not authenticate same-realm
application code. Host policy remains authoritative regardless of these wrappers.
An ActionClient requires only send authority; cancel-own, feedback, result and
privileged cancel-any handles are optional. `cancelAll()` passes zero only through
the explicit cancel-any capability. It never borrows own-goal authority or opens
another alias automatically. Ordinary goal IDs must remain nonzero.

Multi-alias factories acquire all requested handles before activating a service
or Action advertiser. On any requested open failure they attempt cleanup of every
earlier acquisition and preserve the original failure. A private Node ledger
retains any owner whose cleanup was refused before entry, even when construction
mutates its array or the primary error is frozen or primitive. Optional Action aliases
are optional to request, but a requested alias failure rolls back the whole factory
instead of silently weakening its contract. Retrying a failed factory starts a
fresh acquisition; cleanup cannot guarantee rollback of previously admitted native effects.

Use `Node.pendingCleanup()` or `AsyncNode.pendingCleanup()` to inspect retained
full-BigInt handles without effects. Its nonenumerable `factory` property retains
the latest failed factory's primary and cleanup-attempt snapshots. Batch cleanup
also exposes nonenumerable `attempts` rows; see [the complete cleanup contract](API.md#ownership-context-and-cleanup)
for returned statuses, thrown values and Context shutdown behavior.

Graph and Clock validate their named read-operation family in both factories and
direct constructors, and recheck it before dispatch. A caller cannot accidentally
execute a parameter mutation through `Clock.now()` or `Graph.read()`, even after
changing their operation property. Those mismatches return/throw Invalid (22)
before a host request. `Resource` remains the intentionally generic byte-level API.

## Included layers

| Layer | File | Purpose |
| --- | --- | --- |
| Exact inventory | `abi.mjs` | 38 ROS 1 and 43 ROS 2 imports with immutable signatures |
| Raw calls | `raw.mjs` | BigInt validation, import inspection and bounded memory helpers |
| Wire codecs | `codecs.mjs` | String/Bool/Twist/Image/Transform, services, Fibonacci, scalar/array parameters, legacy/typed events, lifecycle/time |
| Client concepts | `roswasm.mjs` | Node, Publisher, Subscription, Executor, Client, Service, ActionClient/Server, Parameters, Graph, Clock, Logger, Tf, Lifecycle |
| Copied asynchronous objects | `async.mjs` | Matching AsyncSession/Context/Node and every supported resource family |
| Real command relay | `relay.c`, `relay_protocol.h`, `relay.mjs` | Bounded complete38/43 dispatcher, three basic WASI imports, exclusive binary pipes; JS remains outside Wasm |
| Embedded guest | `guest.mjs`, `copied_transport.mjs`, `guest/` | Same resource facade over bounded C copies inside a pinned engine/Wasm principal; closed ESM, finite heap/jobs and pure-library flavor |
| Local scheduling | `scheduling.mjs` | Future, CallbackGroup, explicit-ready Timer, bounded deferred queue and parameter notifications |

Read the complete [API and application guide](API.md) for every factory/method,
Context lifetime, error reporting, version limits and local scheduling. Both
synchronous and asynchronous application lessons use an empty alias plan by
default and open no ROS authority. The main package export stays synchronous;
`./async`, `./relay` and `./scheduling` are additive explicit entrypoints.
`./guest` requires the embedded engine's builtin and intentionally cannot load
in ordinary Node. Pure preloaded JS libraries use closed module maps without
that ROS facade and compile zero ROS/WASI imports.

Aliases always select native resource, type identity, QoS and quotas through
trusted runtime configuration. A local Node label has no effect on native
identity. Each service advertise/respond or Action operation uses a separate
explicit capability. The API offers useful client-library concepts; it does not
claim complete `rclcpp` behavior or unrestricted native resource construction.

## Tests and current validation

Run `node sdk/javascript/tests/test.mjs`. It compares all 57 positive and 11
negative language-neutral golden vectors and verifies precision, bounds, unknown
length errors, close ownership, memory growth and polling. Its in-memory bridge
fake provides no ROS implementation; this is a pure JavaScript specification
test, not uwvm/ROS integration validation. The actual direct-guest and native ROS
test matrix remains a separate cgroup execution task.

Run `node sdk/javascript/tests/factory_transactions.mjs` for failed second/third
open, close-backend-error, retry, activation-error and successful commit models.
These tests also use only an in-memory fake and create no native ROS endpoint.

Run `node sdk/javascript/tests/executor_contracts.mjs` for self-registration,
deferred entities, registration-array mutation, nested-spin refusal and callback
exception recovery. These are ordinary JavaScript Pollables and use no ROS bridge
or Wasm execution. `npm test` in this directory runs all three pure suites.

Canonical String decoding preserves literal leading U+FEFF. It is part of the
length-framed UTF-8 payload, not a byte-order marker to strip. Private regression
cases cover message/parameter Strings, String arrays, service responses and typed
or legacy event text without changing the shared golden-vector set.

The task owner schedules all current tests serially in fresh `/tmp` snapshots
with low memory. Do not run parallel language compiler/model matrices or
overwrite historical JSON receipts. New
`tests/ros_sdk_relay/javascript_protocol_models.mjs` previously passed824 pure
stream checks; changed current library bytes require a fresh owner-run receipt.
`javascript_api_models.mjs` declares36 serial API/codec cases and
`javascript_source_contracts.py` performs source/schema checks. Future Node runs
use `--max-old-space-size=96`. No current-source Node rerun, new command
compilation or real relay/native delivery is claimed by the source checkpoint.

`tests/cleanup_entry_contracts.mjs` adds16 authored, unexecuted lifetime models.
It uses actual SDK classes with explicit fixture delegates, including pre-entry
busy refusal, post-attempt exceptions, mixed batches, full handles and retained
factory/Context cleanup. Schedule it with the existing suites only on the SSH
Linux host inside the existing64GiB cgroup. Rebuild exact embedded module tables
and Wasm artifacts before testing actual copied/relay/native behavior; a Node
model result alone cannot qualify those paths or their performance.

## Additive unified event source contract

[Unified event v1](../docs/unified-events.md) specifies the separately granted
observer, complete64-byte envelope, seven kinds, Source/observer lease checks,
capacity-retaining poll, finite readiness and real bounded `service_submit`.
The new APIs are additive; local deferred tasks still advance synchronously
when explicitly scheduled, and do not gain native asynchronous authority.
No implicit alias, clock, thread, retry or stronger cancellation grant is added.
The six runnable unified-event lessons and source fixtures are prepared for the
unified serial test stage; dated historical34/39 receipts certify their original
revisions only.
