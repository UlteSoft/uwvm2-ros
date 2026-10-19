# Progressive JavaScript Wasm guest examples

These applications use the real embedded engine in
[`sdk/javascript/guest`](../../javascript/guest/README.md). Their user logic and
callbacks are JavaScript interpreted **inside the compiled Wasm**, with actual
C ROS imports in the same principal. They do not run through an external Node
process. The existing `../javascript` bridge/relay lessons remain useful for
their separate host integration boundary.

Everything in this directory is source-prepared until actual artifact/import,
Runtime and independent native evidence is retained. An ordinary Node run, a
fake bridge model, a preparation report or a success-looking source marker does
not establish Wasm execution or native delivery. Do not invoke these entry
modules with Node: their restricted `roswasm:guest_native` builtin belongs to
the actual embedded C engine.

## Learn from imports to owned application resources

Each lesson exports a bounded `main()`. The selected command entry executes it,
pumps finite guest Promise jobs and accepts only integer zero after cleanup.
Create no native ROS headers, sockets, host callback pointers or implicit grant.
Names below are exact keys in `../../javascript/guest/recipes.json`.

| Lesson | Build selections | What the application demonstrates |
| --- | --- | --- |
| `00-raw.mjs` | `00-raw-ros1`, `00-raw-ros2` | Exact raw version import, copied C scratch, real default alias denial13 and protected slot/output |
| `01-codecs.mjs` | `01-codecs-ros1`, `01-codecs-ros2` | Canonical String/parameter records, uint64 BigInt, UTF-8/BOM, malformed text refusal and non-splitting encodeInto |
| `02-node.mjs` | `02-node-ros1`, `02-node-ros2` | Actual Node publisher/subscription, guest callback, bounded executor, explicit native-match gate and stale close refusal |
| `03-application.mjs` | `03-application-ros1`, `03-application-ros2` | Existing synchronous facade: service/Action handlers, parameters/events, TF, graph, clock, logger and ROS2 lifecycle |
| `04-async-application.mjs` | `04-async-application-ros1`, `04-async-application-ros2` | Existing Promise-facing facade over same-Wasm copied imports and finite job pump |
| `05-ownership-scheduling.mjs` | `05-ownership-scheduling-ros1`, `05-ownership-scheduling-ros2` | Actual JS closures, Timer, CallbackGroup, Future, deferred queue and closed Context refusal without ROS aliases |
| `06-custom-codec.mjs` | `06-custom-codec-ros1`, `06-custom-codec-ros2` | Actual generated ROS1 Packet/ROS2 FixedSample record codecs, plus explicit native-publish mode |
| `07-complete-denied-abi.mjs` | `07-complete-denied-abi-ros1`, `07-complete-denied-abi-ros2` | Enter all38/43 actual imports using valid copied forms and ungranted aliases/zero handles; no native success claim |
| `08-unified-events.mjs` | `08-unified-events-ros1`, `08-unified-events-ros2` | Separate source/observer capability, exact event envelope, readiness and resource_closed observation |
| `09-boundary-refusals.mjs` | `09-boundary-refusals-ros1`, `09-boundary-refusals-ros2` | Strict C integer/BigInt/typed-array checks, closed dynamic-import rejection and final real default denial |
| `10-floating-modules.mjs` | `10-floating-modules` plus pure `float_guest` | Actual separate f32/f64 Wasm imports/exports, precision, overflow, NaN/Infinity/negative-zero and Number-only refusal |

Read every source comment before selecting a positive profile. Lessons03/04
reuse the existing full application helper rather than duplicate ROS resource
semantics. Their `config.mjs` starts with **empty aliases/actions**: the default
case demonstrates local construction and checked cleanup, not successful
service, Action, parameter or event interoperability. Those cases require
explicit source-selected alias/action plans and corresponding trusted Runtime
grants, quotas and native type identity. A local label is never native identity.
The facade implements the documented Runtime operations; it does not claim
complete `rclcpp`, `rclpy` or arbitrary ROS package source compatibility.

## Admit only the intended native operations

`config.mjs` documents the alias plan, bounded action list, passes, optional
stdin gates, event source/observer aliases and explicit custom-publish switch.
Change this file only in a separately identified frozen build input. The
closed recipe embeds exact bytes, so the resulting artifact pin changes. Do
not mutate config after preparation, infer aliases from a Node name, or make
the runtime default permissive. Each service advertise/respond, Action
send/result/feedback/cancel, clock/read, parameter operation or event observer
keeps its own grant.

For lesson02, provision only `pub` and `sub` with the existing canonical String
contract. Start Worker, Gateway and independent native peer as separate guarded
processes. Wait for `JS_GUEST_ENDPOINTS_CREATED\n`; independently establish
native endpoint matching, then send exactly `P`. Require the peer to observe
`roswasm-js-relay-out` and reply `roswasm-js-relay-in` through actual ROS.
Send one `R` per permitted executor poll, at most200. Empty passes emit
`JS_GUEST_POLL_AGAIN\n`; successful guest receipt/cleanup emits
`PASS_JS_GUEST_DELIVERY\n`. A warmup String is `warmup-js-relay`. A guest
publication marker alone is not proof of peer delivery or callback receipt.

For lesson06, the default checks both complete records without ROS IO.
`customPublish=true` explicitly selects actual `packet-pub` or `sample-pub`
publication for the selected backend. The corresponding generated native
adapter/identity and independent native field oracle are required. A custom
codec success marker in default mode does not qualify native custom types.
Lesson08 similarly requires actual source/observer aliases; its empty default
must refuse rather than silently skip a requested native event test.

## Build and verify the selected artifact

Follow the [explicit preparation CLI](../../javascript/guest/README.md#prepare-explicit-serial-build-jobs)
with one exact variant ID and all pinned engine/tool/archive paths. Preparation
starts no compiler. The owner runs its ordered jobs serially, inspects the
actual38/43 ROS signatures, basic WASIp1 imports, memory limits, exports and
closed source pins, then executes the actual Worker under empty or narrowly
selected permissions. Keep all local builds/tests below `/tmp`, one job at a
time, with memory reserve and a supervisor deadline. Remote execution stays
inside the owner-selected64GiB/swap-zero cgroup and its descendant guards.

The complete [acceptance plan](../../javascript/guest/acceptance.json) separates
local semantics, actual denied imports, positive native IO and deliberate limit
refusals. All current Node suites must be rerun separately because both SDK
storage paths share the resource facade; Node results remain host models.

## Test engine refusal, then retain evidence

`limit-interrupt`, `limit-heap` and `limit-jobs` are deliberately failing command
variants. Each writes `JS_GUEST_LIMIT_ARMED\n` before consuming its selected
budget. Require actual lifecycle LIMIT/exit3 before the external deadline, with
no ROS grant or native effect. A missing armed marker, startup allocation
failure, arbitrary trap, compiler failure or supervisor timeout is not a
successful budget test. These limits do not turn an arbitrary admitted native
blocking call into preemptible JavaScript.

The separate [robotics modular examples](../robotics_modular/README.md) provide
actual JS vision, motion and callback reactor modules plus fixed and retained
session benchmark commands. The pure libraries compile with backend zero and
no ROS/WASI imports. Preload by exact module names, initialize explicitly and
keep every library ROS grant empty. Their synthetic decision checksum is not
Unitree physics, robot safety, image inference or a native performance result.
The floating lesson instead preloads `float_guest` as module `floating` with
empty library permissions. Its typed C trampolines are part of the actual
provider and root binaries, not an external host Number simulation. The build
and acceptance inventories retain all original28 variants and add these two.
