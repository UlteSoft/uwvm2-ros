# Robot scene policy and local scheduling

`uwvm_ros_v2` is an independent scene ABI. Its major version appears only at
the module boundary. Commands use `--ros-scene-*`; `--help rosv2` lists them.
Platform configuration is nested under `--ros-scene ENV platform_extension ...`
and `--ros-scene ENV platform_extension.bind ...`. These child switches cannot
create common device authority or register a driver. See the
[platform extension contract](../runtime/ros-platform-extensions.md) for exact
grammar, default-deny behavior and the current registration boundary.
The [advertised command catalog](ros-scene-cli-catalog.md) contains exact usage
for the registered scene options. WASIp1 and the deprecated ROS1
v1/ROS2 v1 paths retain independent policy. Their grants do not enable a scene
import. ROS v1 enablement emits the configurable `deprecated-ros` warning;
see [logging controls](logging.md). New robot deployments should select the
specific qualified ROSv2 provider for each required capability. A planned
provider name does not establish a working replacement for a legacy operation.

## Current implementation boundary

The VM source registers 160 imports: `abi`, `capability.open`, `capability.close`, `capability.revoke`, and
the 16 scheduling operations, `artifact.open/read/describe`, eight volatile
cache operations, six managed lifecycle operations and five bounded-buffer
operations, plus five model, two shared request, nine transform and two private
log/diagnostic, seven recording/replay, three sensor and four proposal carriers,
plus 28 communication, receive-reservation and asynchronous creation carriers, 12 parameter carriers, seven action carriers, two graph carriers, three readiness stream carriers and 22 device/control/media/bridge/component carriers,
six self/foreign permission and module observations, and six registered cross-Wasm message operations
documented below. Each new binary's execution qualification is
recorded separately from this source registration inventory.
Guard conditions, three clock kinds, timers and waitsets are implemented by the
VM. Linux immutable inputs use a verified sealed snapshot prepared before launch.
Disk-backed transactional outputs remain backend-only. An explicit
`volatile-output` cache binding implements bounded session output.
An explicitly configured isolated CPU worker backs model execution. Local TF
uses bounded VM-owned frame history with an explicit clock capability. Native
ROS node, Topic, Service and Action deployment now supports explicitly sealed Linux
worker pairs with a closed set of canonical message codecs. Generic ROSIDL,
additional native platforms and additional device/bridge adapters remain separate
integration work. Queued receive
providers use `topic.reserve`/`service.reserve` and separate acknowledgement;
the older direct-take dispatcher path does not consume from these providers.
The CLI rejects unsupported purposes and provider bindings.

The [complete design reference](../runtime/ros-operation-reference.md) covers all
160 operations. The [robot binding guide](../runtime/ros-robot-bindings.md) lists
the exact registered device/control/media/bridge/component options, complete
mandatory limits, directional grants, recovery and platform boundaries. Their
[profile and deployment procedures](../runtime/ros-robot-provider-procedures.md)
also define the obligations for additional adapters. Only registered options
should be advertised by an IDE.

## Module observation and registered messages

`permission.report` is a read-only self query. Binding a module to a ROSv2
environment exposes this query by default; it does not enable resource use.
It reports the immediate caller's identity, its group name, import exposure and
available permission metadata. It never lists other group members. Unsupported
families are distinguished from supported but disabled families. WASIp1 rows
describe initially approved descriptor rights; legacy ROS rows describe
configured policy, not a complete snapshot of live handles.

Deployers may reduce observation with
`--ros-scene ENV permission.report FAMILY deny` or the exact
`FAMILY.FUNCTION` selector. An exact function denial also refuses an aggregate
query that would disclose that function. `--ros-scene-interface ENV
permission.report deny` refuses the whole operation; `hidden` removes the import.
Denied queries write neither the result nor its length.

Foreign observation starts with no authority. Expose the required
`module.describe`, `module.imports`, `module.importers`, `module.exports`, `module.group` or
`module.permission.report` operation, and grant the exact target with purpose
`module`, binding `metadata`, and that action. The guest supplies the approved
grant alias. Group membership needs its separate `module.group` action.
These operations cannot invoke foreign exports, inspect foreign memory,
alter another policy, start providers or consume execution budgets. Legacy
preloaddl observation grants no isolation or control over that library.

`module.importers` answers which registered Wasm modules declare imports from
the exact granted target. Each row contains the importing module name and the
declared symbol. This is a direct declaration relationship: reexports are not
expanded, and native dynamic-linker dependencies are not inspected. The operation
requires its own grant even when asking who imports the caller itself, because
the answer reveals other modules. Every visited module and import, including a
nonmatch, consumes the bounded scan work; exhaustion returns no partial page.

`vm.describe` has a separate purpose and exact resource. For example:

```text
--ros-scene-interface observer vm.describe allow
--ros-scene-grant observer vm-view vm runtime metadata vm.describe
```

The guest sends `vm-view` and a row cursor using the common observation carrier
with capability zero. The result contains compiled support, selected compiler,
the resolved compile-thread ceiling and counts of registered declaration kinds.
These counts are not live instance or thread counts. No module names, host paths,
process IDs, addresses or credentials are included. The default self report
cannot query this VM state, and a `module` grant cannot authorize it. Hiding or
denying the import prevents access; revoking its live grant prevents further
delivery. Queries neither start providers nor change or debit execution policy.
See the [observation implementation contract](../runtime/ros-module-observation.md)
for bounded scans, publication, temporary erasure and native comparison scope.

For messages, reserve bounded storage with
`--ros-scene-call-budget ENV BYTES ROUTES`, then register each exact route:

```text
--ros-scene-call ENV ROUTE CLIENT SERVER SEMANTIC REQUEST_SCHEMA RESPONSE_SCHEMA REQUEST_BYTES RESPONSE_BYTES SLOTS DEADLINE_NS
```

Registration fixes endpoint names, semantic operation, opaque schema identities,
byte capacities, outstanding slots and a finite lifetime. It grants no authority.
Expose and grant only the required `module.call.submit`, `.take`, `.respond`,
`.result`, `.acknowledge` and `.cancel` actions using purpose `module.call`, route
as the resource, and binding `registered-message`. Endpoint role and runtime
incarnation are checked independently of the grant. The server's first
authorized `take` registers its current incarnation, even if no message is queued.
An explicitly bound group member may receive a capability from another member
of that same environment. Its first authorized message call performs fallible
enrollment; later calls reuse membership without weakening current authority
checks. A foreign environment's handle, retired incarnation or denied import
remains invalid. This does not permit arbitrary modules to join a group.
Schema labels identify an agreed contract; this opaque-byte broker does not
validate application-specific payload semantics or invoke arbitrary exports.

Inputs and delivery buffers use an owned, unexported shared Wasm memory.
Imported memories, memory exports, nonshared heaps, active threaded host calls
and resumable/replay profiles are refused. The shared declaration permits
atomic cleanup; it does not expose the memory to a peer. Input carriers are
copied and overwritten before dispatch by default. Response, acknowledgement, cancellation,
observed expiry, revocation and retirement erase the corresponding retained
regions and host buffers. Failed cleanup quarantines the slot. Expiry is checked
when the router observes it; it is not a hard real-time erasure timer.

The trusted deployment can change input clearing for one registered route:

```text
--ros-scene-call-clear-input ENV ROUTE true
--ros-scene-call-clear-input ENV ROUTE false
```

The Boolean defaults to `true`; only the exact spellings `true` and `false` are
accepted. Place this option after the route's `--ros-scene-call` declaration and
before activation. The equivalent native deployment API is
`module_call_input_clear(environment, route, clear_after_copy)`; direct native
route plans also expose `bool clear_after_copy{true}`. Activation freezes this
policy. A Wasm request cannot override it or weaken another route's policy.

`false` omits overwriting the **input carrier**, including its failure cleanup.
Copied request headers and payloads remain in the sender's memory until that
sender overwrites them. This is an explicit data-remanence tradeoff, not an
authorization grant or a guarantee of faster execution. Full bounds, tags,
ownership, endpoint, action and revocation checks still apply. Broker storage,
delivered request/response regions and their length words are always cleared
according to their existing lifecycle; this option cannot disable that cleanup.

Selecting `false` emits the `retained-ros-input` warning. Use
`--warning-as-error retained-ros-input` before the option to reject it before
policy mutation. Warning suppression follows the existing logging policy and
does not alter permissions. Debugger `scene resources` and DAP/IDE metadata show
the effective `clear_after_copy` Boolean without reading message bytes.

Live debugger value observation remains supported. Scene inspection returns
detached counters without payloads and does not consume messages. Ordinary
debugger memory reads require their own debugger permission. Guest-made copies
and authorized debugger copies are outside the broker's erasure guarantee.
See [message qualification and profiling](../runtime/ros-module-exchange-performance.md)
for real VM, debugger, sampling and performance evidence.

## Isolated communication deployment

The communication configuration separates four decisions: executable images,
immutable ROS semantic profiles, exact resource grants and worker pairing.
None implies the others. Each worker owns a private registry. Repeating a type
name, profile name or resource label in another worker does not share a handle,
node or authority. Worker definitions use the closed format emitted by
`tools/ros_scene/make_communication_worker_definition.py`; image closure is
prepared with `tools/ros_scene/pack_communication_worker.py`. These tools create
deployment inputs, not permissions or proof of successful execution.

The configuration order is:

1. Create the scene environment and expose the exact required imports.
2. Assign every communication aggregate field with
   `--ros-scene-communication-budget ENV NAME VALUE`.
3. Load each startup definition with
   `--ros-scene-communication-worker ENV WORKER PROFILE ROS_PREFIX PATH BYTES SHA256`.
   This explicitly requested configuration read verifies the exact regular-file
   size, digest and bounded definition. It does not start a worker. `WORKER` and
   `PROFILE` are logical identifiers; `ROS_PREFIX` is the immutable installation
   label for the provisioned provider. The startup digest is bound into its handshake.
4. Set every local RPC/adapter reservation with
   `--ros-scene-communication-worker-limit ENV WORKER NAME VALUE`.
5. Register the provisioned control endpoint, peer credentials, seven directional
   pipes, immutable session identity and expiry with
   `--ros-scene-communication-channel`. See the complete argument and wire contract
   in [external communication attachment](../runtime/ros-communication-attachment.md).
6. Configure native process images, sandboxing, CPU/NUMA placement and cgroups
   externally. The VM has no worker launcher, image or placement option.
7. Create exact grants with binding `native-communication` (or `native-action`
   for an action), and associate aliases using
   `--ros-scene-communication-bind ENV ALIAS WORKER PROFILE`.
8. Pair reciprocal participant definitions with
   `--ros-scene-communication-pair ENV PAIR LEFT RIGHT RETIRE_NS`.
   Each peer belongs to one explicit pair. Pairing grants no guest socket.
9. Use `--ros-scene-check` to validate and freeze the policy. Preparation duplicates
   existing endpoints. Activation requires current authority on both sides.

All numeric values are exact unsigned decimal integers. Digests are exactly
32 bytes in lowercase hexadecimal. Unknown names, duplicate assignments,
missing fields, arithmetic overflow and unsupported families fail closed.
An explicit zero remains zero. Capacity settings never issue a capability or
silently add a traffic rate. Optional rate attenuation remains controlled by
the separate exact grant policy.

### Communication capacities

Every aggregate field is mandatory, including fields whose selected workload
uses zero capacity:

- `workers`, `pairs`, `contexts`, `nodes`, `types`, `topics`, `services`, `actions` and
  `bindings` bound configured objects and registry entries.
- `queue-bytes` and `receipt-bytes` bound retained communication data and
  completion bookkeeping. `carrier-bytes` and `output-bytes` bound one guest
  request and reply.
- `workspace-bytes` and `workspace-calls` bound concurrent request copies and
  encoders. Deployment partitions these reservations between private worker
  registries; each share must hold a complete permitted input and output.
- `node-deadline-ns` bounds asynchronous node creation.
- `action-workspace-bytes` and `action-workspace-calls` separately reserve
  concurrent action carrier copies and encoders. They are divided between
  workers and must cover a complete request and output in each enabled share.
  Set all three action aggregate fields to zero when no action is selected.
- `creation-bindings`, `creation-workspace-bytes` and
  `creation-workspace-calls` reserve independent creation control capacity.
- `definition-bytes` bounds owned startup definitions. `provider-bytes` is a
  conservative aggregate admission bound for configured images, provider
  buffers, worker address limits, supervision and retained requests. It is not
  a measured RSS promise or a replacement for an OS memory controller.
- `supervisor-stack-bytes` reserves one guarded stack per pair;
  `supervisor-poll-ns` sets the polling interval. A smaller interval may reduce
  queueing delay while consuming more CPU. It establishes no real-time bound.

Each worker requires `image-bytes`, `address-bytes`, `cpu-seconds`,
`file-bytes`, `open-files`, `scratch-bytes`, `scratch-slots`, `request-bytes`,
`response-bytes`, `stderr-bytes`, `startup-ns`, `poll-ns`, `supervisor-bytes`
and `threads`. `profile-bytes` is set by the authenticated startup definition
and cannot be assigned again. The additional fields are `rpc-retained-bytes`,
`rpc-stderr-bytes`, `rpc-deadline-ns`, `rpc-poll-ns`, `topic-carrier-bytes`,
`topic-bytes`, `service-carrier-bytes`, `service-bytes`, `action-carrier-bytes`
and `action-bytes`. An unused endpoint family requires both of its worker
reservations to be explicitly zero. The immutable
definition also reserves native queue capacity and explicit directional
transport buffers; these independent bounds must all be consistent.

### Process, latency and recovery boundaries

One joinable supervisor serves each configured pair. Native requests and
creation queues are advanced outside authority and registry locks. A pair's
bounded cleanup cannot occupy another pair's supervisor. Supervisor threads
inherit the preparing thread's CPU and NUMA constraints; child worker placement
is configured independently. Pinning provides neither exclusive cores nor
real-time scheduling. Deployers must reserve CPU, memory bandwidth and NUMA
capacity for the VM, supervisors, native DDS threads and downstream controllers.
Moving data through two processes introduces scheduling and transport costs;
measure end-to-end latency and tail behavior on the selected topology.

The [native driver lifecycle design](../runtime/ros-native-driver-lifecycle.md)
specifies the remaining general vendor-host admission, durable recovery,
replacement and independent-stop requirements. The current closed worker flags
do not provide an arbitrary vendor launcher or automatic crash recovery.

The former component process-lifecycle imports and launcher options are removed.
Native driver lifetime belongs to the external deployment. See the
[driver attachment protocol](../runtime/ros-driver-attachment.md) for inherited
channels, fixed canonical schemas, exact module grants and detached status.

### Host audit of completed ROSv2 imports

Use `--ros-scene-call-audit SCOPE LABEL MODE RECORDS BYTES RATE_HZ BURST` before the deployment is frozen. The default emits no records. This option changes host diagnostics only. It cannot expose an import, enroll a guest, grant a resource or poll a provider.

| Scope | Label | Charged events |
| --- | --- | --- |
| `global` | Literal `*` | Each completed registered core ROSv2 import in the process |
| `group` | A previously created scene environment | Calls from a member of that environment |
| `single` | Exact loaded module label | Calls attributed to that module |

For example, use the following pair of options to observe refused calls for the `body` module with a shared process ceiling. Quote the asterisk in a shell.

```sh
--ros-scene-call-audit global '*' all 10000 4194304 1000 50
--ros-scene-call-audit single body denied 100 32768 10 4
```

All matching ceilings apply. A process-wide rule also selects calls outside the single module. To select only that module, use the single rule alone. The `denied` selection covers access denied (13), bad handle (9) and stale identity or generation (116). The `all` selection preserves the raw return value, including `abi` success (2). Every limit is an explicit finite decimal integer. Byte counts include the newline. Record and byte quotas last for the deployment. The rate budget refills against monotonic time with the configured burst capacity. Zero in any ceiling prevents delivery. Repeating the same scope and label is an error.

NDJSON records are emitted to host standard error. They contain the completed operation, raw result, admission sequence number, monotonic timestamp and the runtime caller's generation. The group and caller label use hexadecimal bytes. Caller labels exceeding 255 bytes are marked truncated. Diagnostic loss counters distinguish lock contention, admission limits, clock failure and sink failure. A sink failure consumes the admission. Concurrent outputs can arrive out of sequence. Counters are reported on subsequent accepted records, so a saturated or disconnected sink can leave the tail unobserved. A regular-file sink can add I/O latency; the writer holds no scene authority lock while emitting. Observation never alters the underlying operation's result.

A hidden import is rejected during linking. A trap or process loss can prevent a normal return. The completed-call stream does not establish execution, cancellation or safe physical shutdown for any of those cases. It contains no arguments, request/response contents, native addresses, file descriptors or capability handles. Sensor values, model prompts and sealed cross-module messages remain outside this stream.

For explicitly authorized cross-Wasm message bytes, see [Private payload capture](../runtime/ros-module-buffer-audit.md) for the separate file registration, route and stage selection, and fixed capture budgets.
