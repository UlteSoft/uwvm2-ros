# Runnable C and C++ applications

These examples select one ROS backend at compilation. SDK objects are local
organizers; Runtime authority starts empty except basic stdin/stdout/stderr.
No Node label, factory, codec, Timer, Future, include file or example grants a
ROS operation. Each application attempts its selected alias, checks the actual
result and closes every acquired owner once. Denied13, unsupported95, pending11,
quota90 and timeout110 keep their meanings. An admitted native effect may survive
an error: never automatically resize/replay an RPC, publication or event poll.

`roswasm/roswasm.h` is the complete C umbrella; `roswasm/roswasm.hpp` is the
complete C++20 umbrella. `roswasmc.h` remains the smaller raw owner foundation.
Select `ros1.h` or `ros2.h` explicitly. Node/backend storage must outlive owners;
resources must outlive borrowed Executor registrations and deferred captures.
Remove registrations/cancel pending work before closing or moving those objects.
C storage/views are caller-owned; copy borrowed strings/arrays/events before
buffer reuse. C++ typed values own their decoded storage. Native pointer layouts
are never wire representations; ABI handles/length slots remain eight-byte LE
even in wasm32 memory.

## Build recipes

Use an installed WASI SDK. Keep all compiler outputs in a fresh `/tmp` folder and
build one translation unit at a time with `-O0 -g0`; no parallel compiler jobs are
needed. Example commands below name actual tool paths supplied by the caller:

```sh
task_out=$(mktemp -d /tmp/roswasm-example.XXXXXX)
"$WASI_CC" --sysroot="$WASI_SYSROOT" -std=c11 -O0 -g0 -I sdk/include \
  -DROSWASM_EXAMPLE_ROS_VERSION=2 -DROSWASM_EXAMPLE_CASE=0 \
  sdk/examples/c/04_service_action_resources.c -o "$task_out/client.wasm"
"$WASI_CXX" --sysroot="$WASI_SYSROOT" -std=c++20 -fno-exceptions -fno-rtti -O0 -g0 \
  -I sdk/include -DROSWASM_EXAMPLE_ROS_VERSION=2 -DROSWASM_EXAMPLE_CASE=0 \
  sdk/examples/cpp/05_service_action_servers.cpp -o "$task_out/server.wasm"
```

Use backend1 for ROS1 and backend2 for ROS2; never link both into one Guest to
guess the available backend. Target architecture and memory width must match
the selected Worker. The no-exception C++ profile requires nonthrowing callbacks;
standard-library allocation/termination failures are not numeric ROS errors.
The C caller-buffer layer is available when allocation must be avoided.

For actual held delivery examples use the direct basic-WASI entry and avoid CRT
argument/environment startup:

```sh
"$WASI_CC" --sysroot="$WASI_SYSROOT" -std=c11 -O0 -g0 -I sdk/include \
  -DROSWASM_EXAMPLE_ROS_VERSION=2 -nostartfiles -Wl,--entry=_start \
  sdk/examples/c/06_runtime_delivery.c -o "$task_out/delivery-c.wasm"
"$WASI_CXX" --sysroot="$WASI_SYSROOT" -std=c++20 -fno-exceptions -fno-rtti -O0 -g0 \
  -I sdk/include -DROSWASM_EXAMPLE_ROS_VERSION=2 -nostartfiles \
  -Wl,--entry=_start sdk/examples/cpp/06_runtime_delivery.cpp \
  -o "$task_out/delivery-cpp.wasm"
```

Compile a separate denial row with `-DROSWASM_EXAMPLE_DEFAULT_DENIED=1` and no
ROS grants. Inspect the final imports after compilation. The direct gate helper
uses only fd_read/fd_write/proc_exit, but the actual C++ toolchain may introduce
other WASI imports; record those and their exercised paths instead of silently
adding permissions. WASI libc allocation is not permission to use a clock,
random source, filesystem, environment, thread or socket. These are source
recipes; current actual local host models do not establish delivery builds.

## Exact case selection

| Application | Compile-time case | Actual flow and required aliases |
|---|---|---|
| C04 / C++04 |0 | Trigger call: `trigger-call` |
| C04 / C++04 |1 | SetBool true call: `set-bool-call` |
| C04 / C++04 |2 | Fibonacci order4 send: `goal-send` |
| C04 / C++04 |3 | Send then one feedback receive: `goal-send`, `goal-feedback` |
| C04 / C++04 |4 | Send then one result receive: `goal-send`, `goal-result` |
| C04 / C++04 |5 | Send then cancel owned goal: `goal-send`, `goal-cancel-own` |
| C04 / C++04 |6 | Cancel all, separate stronger grant: `goal-cancel-any` |
| C07 / C++05 |0 | Trigger server: `trigger-advertise`, `trigger-respond` |
| C07 / C++05 |1 | Fibonacci bounded order0..12 server: `fib-advertise`, `fib-respond` |
| C07 / C++05 |2 | SetBool server: `set-bool-advertise`, `set-bool-respond` |
| C05 / C++06 |0..8 | Parameter get/set/delete/exists/list/search/describe/declare/undeclare, respectively |
| C05 / C++06 |9 | One typed parameter event: `parameter-events` |
| C05 / C++06 |10..16 | Graph inspect topic/node/service/action, list topics/nodes/services, respectively |
| C05 / C++06 |17..20 | ROS/wall/system/steady time, respectively |
| C05 / C++06 |21 | `log-info` write |
| C05 / C++06 |22 /23 | `tf-lookup` / `tf-broadcast` |
| C05 / C++06 |24 /25 | `lifecycle-get` / `lifecycle-change` |
| C/C++08 | default | Real local Future/CallbackGroup/Timer execution; no ROS delivery claim |
| C/C++08 | `ROSWASM_EXAMPLE_DEFER_RPC=1` | Queue Trigger client call; caller spin executes it once |
| C/C++09 | default | Session open/pub/checked reverse close |
| C/C++09 | `ROSWASM_EXAMPLE_REVOKE=1` | Intentional alias revoke then reopen125; negative lifecycle check |

Parameter case aliases are `parameter-get`, `parameter-set`, `parameter-delete`,
`parameter-exists`, `parameter-list`, `parameter-search`, `parameter-describe`,
`parameter-declare`, `parameter-undeclare`. C uses Float64; C++ uses String.
Metadata operations use builtin String metadata independently of the value type.
Graph aliases are `graph-topic`, `graph-node`, `graph-service`, `graph-action`,
`graph-topics`, `graph-nodes`, `graph-services`. Time aliases are `ros-time`,
`wall-time`, `system-time`, `steady-time`. Selecting a version-absent case is an
explicit unsupported95 boundary and performs no alias open.

Servers expose only one bounded poll in the standalone application. Idle returns11;
service response errors and Action response errors are checked. A retained
application keeps owners alive across supervisor-paced passes, then removes its
registration and checks both role closes. No example claims discovery/delivery
from one empty pass or returns success merely because a recipe was exported.

## Complete explicit CLI policies

`advanced_policy.py` emits complete `--rosN-global-grant` and per-field
`--rosN-global-set` argv plus private Gateway configuration. It grants only the
individually repeated `--alias` selections. Review the generated JSON/config;
the generator performs no Runtime validation or launch. It uses the existing
SDK tutorial's connection serializer and refuses version-absent operations.

Example Trigger policy preparation (substitute the actual Worker birth PID,
trusted UIDs and private canonical socket; never guess those bindings):

```sh
PYTHONDONTWRITEBYTECODE=1 python3 -B sdk/examples/c/advanced_policy.py \
  --backend 2 --socket /tmp/roswasm-private/gateway.sock \
  --worker-uid 1000 --worker-pid 12345 --gateway-uid 1000 \
  --node-name sdk_gateway --namespace /uwvm_sdk_relay --domain-id 205 \
  --disabled-security-tutorial --alias trigger-call --out "$task_out/policy"
```

For ROS1 replace the domain/security opt-in with explicit
`--master-uri http://127.0.0.1:11384 --advertise-host 127.0.0.1`. For C parameter
get/set/declare/undeclare/events add `--parameter-kind float64`; C++ uses the
default String. ROS2 local declare/undeclare requires `--parameter-node` to equal
the actual configured Gateway node's absolute name. Remote mutation is not
silently simulated. For retained real relay use `--alias pub --alias sub`.
The complete argv in `Worker_policy_check_arguments` appends
`--rosN-check-policy`; run that exact array through the selected Worker to validate
policy before launching. This is a CLI policy check, not native readiness or
message acceptance. `Worker_arguments` contains only reviewed grants; append the
actual Guest after the Worker tool's argument delimiter. Gateway reads the emitted
`gateway.conf` with the actual matched Worker UID/PID and socket.

The selected tutorial limits are visible: synchronous RPC timeout3000ms,
inflight1, rate10Hz/burst4, bandwidth65536B/s; response/Action payload512B,
parameter/graph/TF/log1024B, topic64B/queue256B, event queue depth4/bytes4096;
all aliases have explicit bounded audit settings. ROS2 reliable/volatile/
keep-last4/automatic/infinite duration fields are emitted explicitly; Action
sets every8-field contract on goal/cancel/result services and feedback/status
topics (40 total). ROS1 sets TCPROS/depth4/latchfalse where applicable. TF fixes
map→base_link, translation bound100, quaternion tolerance0.001 and cache10000ms.
Fibonacci fixes order<=12 and cancel own/any/not-applicable per exact operation.
Lifecycle change fixes configure; the Guest cannot select another transition.
These are tutorial choices, not defaults or production recommendations. ROS2
disabled-test opt-in is separate from signed Enforce/SROS2 validation.

## Native prerequisites and actual delivery gates

Before running client cases, start the matching native Trigger/SetBool service
or Fibonacci server at `/uwvm_sdk_relay/trigger`, `/set_bool` or `/fibonacci`.
Server cases require a native client sending an actual request/goal while the
Guest owners remain open. ROS1 needs the explicitly selected Master/TCPROS peer;
ROS2 needs domain205 and a compatible explicitly configured RMW/QoS peer. Install
the exact trusted generated types with identity matching the selected backend.
Parameter queries require `/uwvm_sdk_relay/sdk_parameter_node` and `value` of the
matching type; ROS1 uses `/uwvm_sdk_relay/value`. Graph results are prefix-filtered
views, not unrestricted discovery. TF lookup requires map→base_link in the
native buffer; broadcast sends zero-stamped identity rotation for those fixed
frames. Lifecycle needs a managed node at `/uwvm_sdk_relay/lifecycle_node` and a
valid configure transition. Merely compiling these applications proves none of
those native prerequisites.

The dedicated C/C++06 delivery applications use the external protocol in
[`../../../tests/ros_sdk_relay/NATIVE.md`](../../../tests/ros_sdk_relay/NATIVE.md).
They open pub/sub, check unrelated alias denial and protected receive sentinels,
send one warmup, and emit `SDK_ENDPOINTS_CREATED`. The supervisor supplies P only
after independently recorded actual native match. Final output is sent once;
each R token permits exactly one poll, at most200. Exactly one canonical reply
and typed callback are required before `SDK_DELIVERED`; both closes and raw stale9
sentinels are checked before `PASS_SDK_DELIVERY`. Native final delivery, native
reply publication, quiet-window counters, Worker/Gateway birth, process cleanup
and source bindings are independent supervisor evidence. A Guest marker alone
cannot prove them.

## Objective boundaries

The SDK covers every registered ABI operation; its high-level interfaces do not
provide native rclcpp/rclc source/ABI compatibility, arbitrary custom IDL inference,
automatic native context/thread discovery, unbounded futures or implicit timers.
Version-absent operations are95. ROS2 remote declare/undeclare, unsupported QoS
history/backpressure/custom service or Action adapters remain real Runtime
limitations. SDK registration/policy success does not establish full40-field
native RMW adoption. CallbackGroup and Context are local scheduling/lifetime
concepts. Deferred RPCs remain synchronously bounded on caller spin; Timer needs
explicit caller time or a separately granted Clock. Context shutdown stops local
dispatch, leaves existing owners closeable, and sends no native lifecycle change
or alias revoke. C Session/Executor associate this local context explicitly;
C Node is an independent stable dispatch organizer.

Current host-model results are separately reported by the serial local test
coordinator. New example source and generated policy are source-prepared until
actual toolchain/Runtime/native tests complete. The complete73-row binding and
example mapping is in [`API_COVERAGE.json`](API_COVERAGE.json); scope there is
source coverage, not an assertion that all native behaviors passed.

## Explicit Action client bundle and direct polling safety

`10_action_client_bundle.c/.cpp` is a runnable Node-style four-role application.
Supply `goal-send`, `goal-cancel-own`, `goal-feedback` and `goal-result` explicitly;
all names must be distinct and every role must be registered before any open.
The optional fifth `goal-cancel-any` is supplied only with
`ROSWASM_EXAMPLE_CANCEL_ANY=1`. It is an independent stronger right, never a
consequence of cancel-own, a Future, construction or shutdown. Generate only
these required aliases with `advanced_policy.py`; add the fifth only after an
explicit intentional selection. Every ROS2 Action role receives the exact40
QoS fields in the Runtime schema: goal/cancel/result-service and feedback/status
prefixes. A valid CLI policy does not establish complete native RMW adoption.

Compile `ROSWASM_EXAMPLE_CASE=0` for send/one feedback/one terminal result; case1
for send/own-cancel/one result; case2 for cancel-all; case3 for a deferred local
send followed by feedback/result. Case4 sends then uses a nonzero cancel-any
selector through the fifth role and receives one result; the grouped API also
permits an explicitly selected nonlocal goal, with final authority enforced by
the Runtime. Zero is refused22 by cancel_any; cancel_all is separately explicit.
Cases2/4 without the fifth alias return13 locally.
Accepted cancellation is checked, and the supplied order4/native Fibonacci
server must match the independently configured resource/type/limits. This
one-shot main reports pending11 or timeout110 and performs checked cleanup; it
does not return a retained client object. A long-lived application must keep the
bundle alive and externally drive bounded feedback/result receives before
closing. Send/cancel are never automatically retried, and neither application
busy-waits, obtains a clock or replays an admitted operation.
`ROSWASM_EXAMPLE_REVOKE=1` deliberately revokes each acquired alias after the
selected flow; it is a distinct destructive policy action, not ordinary close.

The C API is `action_bundle.h`, available through the complete C Node umbrella.
Supply a zero-initialized bundle, stable selected context, explicit role alias
views and caller-owned goal-ID storage. The C++ `ActionClientAliases` and Node
`create_action_client(aliases, maximum_goals, maximum_reply_bytes)` overload
provide the same role grouping; the original single-role overload remains
compatible and gains explicit revoke. `ActionClientBundle` is noncopying and
nonmoving, with its own stable context and fixed ID storage. The local table is
bounded1..1024 (at most8KiB of IDs); host larger quotas cannot enlarge it.
`async_send_goal` queues exactly one existing bounded synchronous send, with an
owned Future result; cancel pending captures before destroying their bundle.

Only a valid nonzero unique ID returned by this bundle's send is tracked.
Unknown/stale IDs are refused9 before import; malformed/mismatched replies and
idle/errors preserve the table and caller output. Feedback retains ownership;
only a matching successful terminal2..4 result retires its local ID. Cancellation
acceptance never pretends a terminal result already arrived. The Runtime remains
authoritative for Instance/backend/grant/resource/goal provenance, so local
bookkeeping cannot confer authority. The C result preserves error90 written or
unwritten length; C++ exposes `last_result()` without resize/replay. Checked
close/revoke attempts every live role once in reverse order, reports each slot
in acquisition order, keeps primary/cleanup errors separate and clears IDs even
after notification failure. Failed acquisition rolls back already opened roles;
no half-constructed client proceeds. Destruction performs ordinary best-effort
close only; explicit checked close is needed to observe cleanup failures.

`Subscription`, `ServiceServer` and `ActionServer` protect direct polling even
without an Executor. Nested poll/close/revoke during a handler returns17 before
imports or buffer mutation; checked cleanup returns attempted0 and preserves
status slots. Their callable snapshot shares the exact registered function
object, preserving mutable capture state across polls while keeping it alive
through reassignment. Replacement affects only the next poll. Action.respond
remains valid inside its handler. RAII releases the intrinsic lease on normal
return or an exception; no-exception Wasm handlers must not throw. An actively
borrowed object must not be moved/destroyed by user code. Closed pollable facades
report9 consistently; setup/activation failures keep their original error.
Servers also provide explicit checked pair revocation, never automatic shutdown.

New fake-import fixtures cover these stateful/reentrant/exception/rollback/goal
contracts and must be executed by the serial local test coordinator. They do
not prove native action effects, signed policy, complete QoS adoption or real
Guest delivery. Compilation and Runtime/native acceptance remain separate.

The intrinsic17 lease protects only methods on the active typed facade.
Pollable capabilities remain private; there is no public mutable capability()
accessor. Node keeps no shared owner registry or grouped close; Session cleanup
owns only its independently opened slots, not typed Node factory resources.
Public numeric handle access is a deliberate raw-ABI escape: direct import or
backend calls bypass the facade lease and must not close/revoke/poll active
borrowed handles. A separate owner/admin can intentionally revoke the same alias
and the Runtime may retire siblings; the SDK does not promise an alias-wide
lock, native thread synchronization or rollback protection. Moving/destroying
an active resource remains unsupported caller lifetime misuse. These boundaries
prevent local borrow-safety claims from becoming a false Runtime security claim.
