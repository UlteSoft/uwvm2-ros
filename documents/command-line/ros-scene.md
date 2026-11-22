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

See the [authorization and usage limits](../runtime/ros-host-interfaces.md#authority-and-ownership) or the [Chinese permission guide](../runtime/ros-permissions.zh.md).

## Review a scene policy offline

Use `--ros-scene-dump-effective-policy` as the last option in a scene-only
command. The process validates the declarations and writes one JSON document to
stdout. It exits before guest loading, native provider preparation or journal
activation. Redirect stdout to save the inventory.

```sh
uwvm --ros-scene-create robot --ros-scene-bind robot main \
  --ros-scene-interface robot guard.trigger deny \
  --ros-scene-dump-effective-policy > robot-policy.json
```

The schema version is 1. To find a module’s policy, locate its exact
`moduleHex` in `memberships`, then inspect the matching `environments` entry.
Printable ASCII names also have a `module` display string. Unbound modules are
hidden. For members, an explicit `configuredImports` rule wins. Absent rules allow `abi` to return the public ABI major and
`permission.report` to report the caller’s own settings. All other imports
start hidden. Resource authority also requires a grant. Each grant includes its
purpose, resource and binding IDs, exact action set, platform owner, handle,
resource and wait limits, delivery policy, and call/byte window ceilings.
`platformSelections` reports explicit enable and disable choices. A bound
alias inherits its platform switch. `observationDenials` identifies reductions
to a permission report, with a null function for an entire family.

`sessions` gives each external driver, model or communication registration’s
logical owner, transport, expected PID/UID/GID, deployment, VM, scope, provider
and session IDs, pinned contract digest, absolute monotonic expiry and local
channel bounds. For a driver, the scope identifies the module and the provider
identifies the driver. The exact Wasm module, grant alias and schema traversal
limits also appear. Communication records include the authenticated startup
profile and worker limits. `communicationBindings` and `communicationPairs`
associate aliases and coupled workers with these sessions.

`callAuditRules` holds the configured global, group and single ceilings.
`payloadJournal` reports declared capture capacity. Per-route entries include
the client, server, schemas, limits, sender input erasure and selected capture
stages. Integer limits and timestamps are decimal strings so readers can
preserve all 64 bits. The literal `"unlimited"` distinguishes an unlimited
ceiling from a finite zero.

`validation: "declarations-only"` describes the evidence available before
launch. Provisioner descriptor numbers are checked for syntax and endpoint
reuse by number. The real launch must check kernel identity, duplicate kernel
objects, secret seals, transport availability and peer authentication.
Operator-specified hashed configuration definition files are read when their
CLI options require them. Artifact input paths, private journal files, TLS
identities and provider channels stay unopened. Paths, descriptor numbers,
key material and payloads have no JSON fields. Module names and logical IDs
are supplied by the operator and appear as declared.

The machine-readable `excluded` list identifies the rest of deployment policy
outside this inventory, including provider-specific definitions and aggregate
budgets, platform operation mappings, other host modules and live state.
The exporter accepts at most 4,096 variable rows and 4 MiB of JSON.
Invalid declarations, mixed host commands or either bound being exceeded
produce a failing exit status and empty stdout.

## Inspect a stopped scene archive

A full `dump wasm` at a cooperative stop can write a partial Wasm Core Dump
when executable state is unavailable. With debugger inspection permission,
that archive also records the first eight rows of each available process-owned
scene view (`authority`, `resources` and `audit`) when at least one grant,
resource or configured audit counter set is observed. Each page is sampled
independently while the guest stays stopped. The snapshot reader displays the
stop ID, logical labels, counters, total rows and the recorded page boundary:

```sh
python3 tools/debug/snapshot.py info guard.uwsnap
```

Inspect `sceneObservation.pages` in the JSON output. A page with `more: true`
has additional live rows beyond the saved first page. A busy or scan-limited
page carries an unavailable status. An absent observation means the file has
no recorded scene pages. Diagnostic-only dumps, typed-state captures and older
archives can all lack this optional record. The three pages exclude buffers,
credentials, descriptors, operation history and provider internals. They grant
no restore, replay or device authority.

The UWSNAP1 inspection record with id 0 and schema 1 still contains the Wasm
Core Dump. The optional partial inspection record has id 1 and schema
`0x52534331`. Its payload starts with the eight bytes `UWSCN1\0\0`, a
little-endian 64-bit diagnostic stop ID and three pages in the order above.
Each page is a little-endian 32-bit byte count followed by up to 65,536 bytes
of the existing ASCII scene debugger response. Readers can retain unknown
inspection schemas as opaque records.

## Current implementation boundary

The VM source registers 163 imports, including `abi`, `capability.open`, `capability.close`, `capability.revoke`, and
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
163 operations. The [robot binding guide](../runtime/ros-robot-bindings.md) lists
the exact registered device/control/media/bridge/component options, complete
mandatory limits, directional grants, recovery and platform boundaries. Their
[profile and deployment procedures](../runtime/ros-robot-provider-procedures.md)
also define the obligations for additional adapters. Only registered options
should be advertised by an IDE.

## Module observation and registered messages

Both bound queries support independent `allow`, `deny` and `hidden` rules.
For example, `--ros-scene-interface robot abi deny` makes the version call
return 13. With `hidden`, a module that imports it fails to link.

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
silently add a traffic rate. Optional rate limits are configured by
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

### Host audit of ROSv2 imports and call results

Use `--ros-scene-call-audit SCOPE LABEL MODE RECORDS BYTES RATE_HZ BURST` before the deployment is frozen. The default emits no records. This option changes host diagnostics only. It cannot expose an import, enroll a guest, grant a resource or poll a provider.

| Scope | Label | Charged events |
| --- | --- | --- |
| `global` | Literal `*` | Core ROSv2 call returns and hidden imports in the process |
| `group` | A previously created scene environment | Calls from a member of that environment |
| `single` | Exact loaded module label | Calls attributed to that module |

For example, use the following pair of options to observe refused calls for the `body` module with a shared process ceiling. Quote the asterisk in a shell.

```sh
--ros-scene-call-audit global '*' all 10000 4194304 1000 50
--ros-scene-call-audit single body denied 100 32768 10 4
```

All matching ceilings apply. A process-wide rule also selects calls outside the single module. To select only that module, use the single rule alone. The `denied` selection includes hidden imports at link time. At runtime it covers access denied (13), bad handle (9) and stale identity or generation (116). The `all` selection preserves the raw return value, including `abi` success (2). Every limit is an explicit finite decimal integer. Byte counts include the newline. Record and byte quotas last for the deployment. The rate budget refills against monotonic time with the configured burst capacity. Zero in any ceiling prevents delivery. Repeating the same scope and label is an error.

NDJSON records are emitted to host standard error. They contain the completed operation, raw result, admission sequence number, monotonic timestamp and the runtime caller's generation. The group and caller label use hexadecimal bytes. Caller labels exceeding 255 bytes are marked truncated. Diagnostic loss counters distinguish lock contention, admission limits, clock failure and sink failure. A sink failure consumes the admission. Concurrent outputs can arrive out of sequence. Each accepted record carries the earlier loss counters. At a cooperative debugger stop, use `info scene audit` to inspect the latest counters, including trailing losses. A regular-file sink can add I/O latency; the writer holds no scene authority lock while emitting. Observation never alters the underlying operation's result.

The loader records each observed visibility rejection as `phase="link"`, `reason="hidden"`, `generation=0` and `result=null`. The import name reflects host rewrites. A raw identifier outside the bounded ASCII operation alphabet is encoded as a hexadecimal prefix of up to 128 bytes with the original byte count and an explicit truncation field. Unbound callers can match process and single-module rules. Linking stops at the first fatal dependency error. A trap or process loss can prevent a normal return. The completed-call stream does not establish execution, cancellation or safe physical shutdown for any of those cases. It contains no arguments, request/response contents, native addresses, file descriptors or capability handles. Sensor values, model prompts and sealed cross-module messages remain outside this stream.

For explicitly authorized cross-Wasm message bytes, see [Private payload capture](../runtime/ros-module-buffer-audit.md) for the separate file registration, route and stage selection, and fixed capture budgets.


At a cooperative debugger stop, `info scene policy environment=robot
operation=permission.report` displays the configured and effective import
exposure for members of that environment. The debugger operator can inspect
this metadata with full or read-only inspection authority. See the
[scene inspector](../../tools/debug/README.scene.md) for pagination and
the distinction between interface exposure and resource grants.
