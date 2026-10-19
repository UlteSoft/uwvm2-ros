# AssemblyScript API, executable entries and verification boundaries

| Layer / concept | Actual public APIs | Complete entry |
| --- | --- | --- |
| Exact ABI | ros1/raw.ts38 imports; ros2/raw.ts43 imports |01-raw; raw coverage fixtures are intentionally inert |
| Typed canonical wire | Codec, strict Reader/Writer and builtin values |02-codecs; independent wire/unicode vectors |
| Ownership | Capability close/revoke/rawHandle; Session node/close; Node close |03-node,04-resources; adapters/IDs are read-only |
| Pub/sub and polling | Publisher, Subscription, bounded Executor add/remove/spinOnce/close |03-node; callbacks are Guest-local |
| Client / server | Client trigger/setBool/response; Service spinOnce |04-resources,05-servers; exact owned request ID |
| Action client/server | send/own-any-all cancel/feedback/result; validated sequence decoders; ActionServer spinOnce |05-servers,08-action-parameters |
| Parameters/events | typed scalar/arrays and exact get/set/delete/exists/list/search/describe/declareValue/undeclare/event selectors |04,06,08; events ROS2-only |
| Graph / TF / clock/logger | filtered Graph, exact configured Tf, u64 Clock, explicit Logger |04,06; logger requires a separate alias |
| Lifecycle | exact get/change node and host-fixed transition | ROS2 lesson06; absent in ROS1 |
| Deferred result / Context | copied Reply Future, bounded queue, caller spinOnce/shutdown |07-scheduling; no native async RPC |
| Timer / CallbackGroup | explicit first/period/now, overflow/backwards-time checks, mutual exclusion |07-scheduling; no implicit clock/thread |

All application entries contain real `_start` imports/typed operations and
checked cleanup. Inspect lastStatus and require0 for acceptance; command exit0
alone does not inspect a void entry's diagnostic. Native peers/discovery are
prerequisites for positive delivery, and one event poll may legitimately be idle.
The compiler-only raw coverage entries and highlevel_coverage fake fixture are
separate from runnable native applications. Default authority remains empty;
Node/Session/Context labels do not create native identity, connection or rights.
Read the [example guide](../examples/assemblyscript/README.md) for exact aliases,
CLI identities, limits, native type/QoS and default13 expectations.

Close/revoke retire before native notification; group close attempts all live
handles once and returns every status. Retained owners and later factories are9.
Factory acquisition rollback preserves first failure. Server recursive spin and
callback-time close/revoke is17 before another dequeue/import. Direct
Subscription.spinOnce also holds its capability lease through the handler.
Public Capability.poll refuses17 while that lease is held; typed dispatch's
first poll requires its exact private identity token. A nested caller cannot
acquire/release the active token. Ordinary response requests remain permitted.
Node/Session cleanup preflights all live owners before any retirement;17 keeps
factories/owners open for cleanup after dispatch. Idle/error returns release
the lease, and subsequent checked cleanup is still required. Executor nested
spin/remove/close is17 and additions wait until the next outer pass. No native
callback enters managed code. A Wasm trap is an unrecoverable application error,
not a ROS status or guaranteed cleanup. No auto-resize/replay after90 occurs.

Future completion stores a copied bounded Reply; pending/running11, taken9 and
not-started cancellation local125 do not change host authority. A deferred
Client remains synchronous in its later step; cancel/shutdown cannot interrupt
a started native call or undo an admitted effect. Context's1..1024 combined
queue/timer budget is local, not a native quota. Timers use exact caller u64 time,
positive periods and no catch-up. Function references are used; JavaScript
closure support is not presumed. Node/Session close is separate from Context
shutdown and Executor unregistering because Pollable is a local dispatch API.

ROS1 has no ROS2 events/declare/undeclare/describe, Action graph inspection,
system/steady clock or lifecycle. ROS2 has no ROS1 delete/exists/search or wall
clock. Those exact absent selectors stay95. Existing builtin adapters/trusted
generated custom topics do not imply arbitrary native services/Actions, dynamic
IDL/CDR loading, native Context construction or binary rclcpp/rclc compatibility.

Prepared executable fixtures include factory_transactions75, local_schedule47
and server_callbacks60 assertions, plus all retained wire/Unicode/adapter/role/
executor tests. They use local fake Api/counters, not ROS authentication or DDS.
Root owns actual serial `/tmp` compiler/Wasm/model runs. Each report must bind
this source revision; source/fixture preparation is not a native behavior PASS.
