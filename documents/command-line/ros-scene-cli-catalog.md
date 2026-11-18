# ROSv2 advertised CLI catalog

Generated from an actual Linux integration binary's `--help all`.
This catalog contains every advertised primary command matching `--ros-scene-`, its aliases in
the usage line, and the binary's own description. Availability and
supported transports still follow the ROS product policy. A registered
platform-specific option is not proof that its backend is available.

See the [operator whitepaper](../user-whitepaper.md) for build, permissions,
debugging, update boundaries and exit status, and the
[topic references](README.md) for semantic details and subcommands.

Regenerate with `tools/documentation/generate_cli_reference.py --vm
/absolute/path/to/uwvm2-ros --prefix=--ros-scene- --output documents/command-line/ros-scene-cli-catalog.md`.
An existing exact help capture can instead be supplied with `--help-file`.
The generator does not run a Guest or confer any permissions.

Advertised primary commands: **87**.

## `--ros-scene`

```text
[--ros-scene] <environment:str> <platform_extension|platform_extension.bind|permission.report> <module-or-function:str> <enable|disable|device-alias|deny>
```

Configure a ROSv2 child or reduce read-only permission reports; platform enablement grants no authority.

## `--ros-scene-bind`

```text
[--ros-scene-bind] <environment:str> <module:str>
```

Bind one exact module; no fallback to any other host policy.

## `--ros-scene-bridge-budget`

```text
[--ros-scene-bridge-budget] <environment:str> <bindings:u64> <storage-bytes:u64> <supervisor-stack-bytes:u64> <poll-ns:u64>
```

Reserve finite directed topic bridge providers; creates no input, output or egress authority.

## `--ros-scene-bridge-definition`

```text
[--ros-scene-bridge-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read a bounded authenticated bridge definition; never loads codec libraries.

## `--ros-scene-bridge-definition-hex`

```text
[--ros-scene-bridge-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply a canonical authenticated directed topic bridge definition without file access.

## `--ros-scene-bridge-limit`

```text
[--ros-scene-bridge-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Assign one mandatory bridge capacity; optional rates remain separate.

## `--ros-scene-bridge-profile`

```text
[--ros-scene-bridge-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select an exact digest-bound directed topic bridge definition under an existing bridge grant.

## `--ros-scene-buffer`

```text
[--ros-scene-buffer] <environment:str> <alias:str> <schema:str> <sha256:hex> <bytes:u64> <objects:u64> <object-bytes:u64> <transfer:u64> <workspace:u64> <leases:u64> <rank:u64> <elements:u64> <dtype-mask:u64> <order-mask:u64> <color-mask:u64>
```

Bind an exact buffer schema, finite capacities and closed layout masks; allocates no payload.

## `--ros-scene-buffer-budget`

```text
[--ros-scene-buffer-budget] <environment:str> <bytes:u64> <objects:u64> <workspace:u64> <leases:u64>
```

Reserve aggregate buffer bytes, object slots, read workspace and sealed leases; defaults are zero.

## `--ros-scene-call`

```text
[--ros-scene-call] <environment:str> <route:str> <caller:str> <callee:str> <operation:str> <request-schema:str> <response-schema:str> <request-bytes:u64> <response-bytes:u64> <slots:u64> <deadline-ns:u64>
```

Register an exact caller, callee and bounded message contract; no arbitrary export invocation.

## `--ros-scene-call-budget`

```text
[--ros-scene-call-budget] <environment:str> <bytes:u64> <routes:u64>
```

Reserve cross-Wasm message storage; creates no authority.

## `--ros-scene-call-clear-input`

```text
[--ros-scene-call-clear-input] <environment:str> <route:str> <clear-after-copy:bool>
```

Set copied-input erasure for one registered route (default true); false risks retained sender data.

## `--ros-scene-check`

```text
[--ros-scene-check]
```

Validate and freeze scene policy without executing guests or activating native providers.

## `--ros-scene-communication-bind`

```text
[--ros-scene-communication-bind] <environment:str> <alias:str> <worker:str> <profile:str>
```

Select one exact worker profile for an existing native-communication grant.

## `--ros-scene-communication-budget`

```text
[--ros-scene-communication-budget] <environment:str> <name:str> <value:u64>
```

Reserve finite communication registries, carriers, worker images and pair supervisor resources; every named field is explicit.

## `--ros-scene-communication-channel`

```text
[--ros-scene-communication-channel] <environment:str> <worker:str> <control-fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <request-write:u32> <response-read:u32> <diagnostic-read:u32> <discovery-write:u32> <discovery-read:u32> <user-write:u32> <user-read:u32> <deployment:hex128> <vm:hex128> <scope:hex128> <provider:hex128> <session:hex128> <contract:sha256> <startup-ns:u64> <session-until-ns:u64> <stderr-bytes:u64>
```

Attach a deployment-owned ROS provider through authenticated control and seven directional pipes; no native lifecycle control.

## `--ros-scene-communication-pair`

```text
[--ros-scene-communication-pair] <environment:str> <pair:str> <left-worker:str> <right-worker:str> <retirement-ns:u64>
```

Bind two isolated workers to one explicit transport and cleanup failure domain.

## `--ros-scene-communication-worker`

```text
[--ros-scene-communication-worker] <environment:str> <worker:str> <profile:str> <ros-prefix:str> <absolute-path:str> <bytes:u64> <sha256:hex>
```

Authenticate a bounded startup definition; grants no capability and starts no native code.

## `--ros-scene-communication-worker-limit`

```text
[--ros-scene-communication-worker-limit] <environment:str> <worker:str> <name:str> <value:u64>
```

Set a mandatory local RPC or adapter reservation; explicit zero never becomes unlimited.

## `--ros-scene-control-definition`

```text
[--ros-scene-control-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read a bounded authenticated control definition; performs no native device I/O.

## `--ros-scene-control-definition-hex`

```text
[--ros-scene-control-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply a canonical authenticated closed scalar control definition without file access.

## `--ros-scene-control-limit`

```text
[--ros-scene-control-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Assign one mandatory control capacity; optional rates remain separate.

## `--ros-scene-control-profile`

```text
[--ros-scene-control-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select an exact digest-bound closed scalar control definition under an existing control grant.

## `--ros-scene-create`

```text
[--ros-scene-create] <environment:str>
```

Create an empty independent environment; grants no interfaces or resources.

## `--ros-scene-device-budget`

```text
[--ros-scene-device-budget] <environment:str> <devices:u64> <controllers:u64> <retained-bytes:u64> <supervisor-stack-bytes:u64> <poll-ns:u64> <shutdown-ns:u64>
```

Reserve device/control records, an inherited-placement supervisor and bounded shutdown; grants remain separate.

## `--ros-scene-device-definition`

```text
[--ros-scene-device-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read a bounded authenticated device definition; performs no native device I/O.

## `--ros-scene-device-definition-hex`

```text
[--ros-scene-device-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply a canonical authenticated closed scalar device definition without file access.

## `--ros-scene-device-endpoints`

```text
[--ros-scene-device-endpoints] <environment:str> <alias:str> <command-tx:u64> <command-ack-rx:u64> <stop-tx:u64> <stop-ack-rx:u64>
```

Bind explicit directional command and independent stop channels; raw mode and channel identity are checked only on activation.

## `--ros-scene-device-limit`

```text
[--ros-scene-device-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Assign one mandatory device capacity; optional rates remain separate.

## `--ros-scene-device-profile`

```text
[--ros-scene-device-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select an exact digest-bound closed scalar device definition under an existing device grant.

## `--ros-scene-diagnostic-limit`

```text
[--ros-scene-diagnostic-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory scope, field mask, capacity or lifetime for cached diagnostic pages.

## `--ros-scene-diagnostic-source`

```text
[--ros-scene-diagnostic-source] <environment:str> <alias:str> <source-alias:str>
```

Select one exact private-log source; every read also needs that source's resource.observe capability.

## `--ros-scene-driver-budget`

```text
[--ros-scene-driver-budget] <environment:str> <bindings:u64> <bytes:u64>
```

Reserve external-driver bindings and retained bytes; no process authority.

## `--ros-scene-driver-channel`

```text
[--ros-scene-driver-channel] <environment:str> <alias:str> <module:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <module:hex128> <driver:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <request-ns:u64> <session-until-ns:u64> <identity-fd:u32> <peer-time-namespace-fd:u32>
```

Require pinned mutual TLS 1.3 and provisioned peer clock evidence. Read the secret only from a sealed identity descriptor.

## `--ros-scene-driver-schema`

```text
[--ros-scene-driver-schema] <environment:str> <alias:str> <operation:str> <nodes:u64> <elements:u64> <depth:u64>
```

Require a compiled canonical operation shape for both driver request and reply.

## `--ros-scene-event-stream`

```text
[--ros-scene-event-stream] <environment:str> <alias:str> <domain:str> <profile:str> <sha256:str> <streams:u64> <sources:u64> <events:u64> <bytes:u64> <workspace:u64> <deadline_ns:u64> <fields:u64>
```

Bind a bounded host-local readiness stream; its source capabilities remain independently granted.

## `--ros-scene-grant`

```text
[--ros-scene-grant] <environment:str> <alias:str> <purpose:str> <resource:str> <binding:str> <actions:csv>
```

Grant exact purpose, resource, trusted binding and action set. Opening an alias never elevates it.

## `--ros-scene-input`

```text
[--ros-scene-input] <environment:str> <alias:str> <absolute-path:str> <sha256:str> <generation:u64> <bytes:u64> <transfer:u64>
```

Pin an existing sealed-input grant to an exact SHA256 generation and length; activation occurs only before launch.

## `--ros-scene-input-budget`

```text
[--ros-scene-input-budget] <environment:str> <bytes:u64> <objects:u64>
```

Reserve finite retained immutable-input bytes and objects; both default to zero.

## `--ros-scene-interface`

```text
[--ros-scene-interface] <environment:str> <function:str> <hidden|deny|allow>
```

Configure an exact uwvm_ros_v2 function; exposure and resource authority are independent.

## `--ros-scene-lifecycle`

```text
[--ros-scene-lifecycle] <environment:str> <alias:str> <handles:u64> <deadline-ns:u64> <transition-mask:u64> <role-mask:u64>
```

Bind a finite local lifecycle graph; no native node or device authority.

## `--ros-scene-limit`

```text
[--ros-scene-limit] <environment:str> <alias:str> <field:str> <value:str>
```

Set a capacity or explicit attenuation field; finite window budgets require delivery=may-reject.

## `--ros-scene-log-diagnostic-budget`

```text
[--ros-scene-log-diagnostic-budget] <environment:str> <name:str> <value:u64>
```

Set one mandatory finite reservation for private logs and cached diagnostic pages.

## `--ros-scene-log-limit`

```text
[--ros-scene-log-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory record, byte or closed severity bound for an existing private-log grant.

## `--ros-scene-log-logger`

```text
[--ros-scene-log-logger] <environment:str> <alias:str> <logger:str>
```

Allow one exact logger identity; no console, filesystem or network appender is selected.

## `--ros-scene-log-loss`

```text
[--ros-scene-log-loss] <environment:str> <alias:str> <mode:str>
```

Explicitly choose reject or drop-oldest behavior when the finite log is full.

## `--ros-scene-media-budget`

```text
[--ros-scene-media-budget] <environment:str> <bindings:u64> <storage-bytes:u64>
```

Reserve finite immutable-buffer media providers; creates no input, output or egress authority.

## `--ros-scene-media-definition`

```text
[--ros-scene-media-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read a bounded authenticated media definition; never loads codec libraries.

## `--ros-scene-media-definition-hex`

```text
[--ros-scene-media-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply a canonical authenticated raw media definition without file access.

## `--ros-scene-media-limit`

```text
[--ros-scene-media-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Assign one mandatory media capacity; optional rates remain separate.

## `--ros-scene-media-profile`

```text
[--ros-scene-media-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select an exact digest-bound raw media definition under an existing media grant.

## `--ros-scene-model`

```text
[--ros-scene-model] <environment:str> <alias:str> <model:str> <model-sha256:hex> <task:str> <task-sha256:hex> <parameter-type:str> <output-type:str> <text.utf8|bytes> <stream|terminal>
```

Bind immutable model/task profiles and transport codec; native worker preparation is independently required.

## `--ros-scene-model-budget`

```text
[--ros-scene-model-budget] <environment:str> <sessions:u64> <requests:u64> <input-bytes:u64> <output-bytes:u64> <output-items:u64> <workspace:u64>
```

Reserve aggregate model sessions, requests, pinned inputs, outputs and workspace; all default to zero.

## `--ros-scene-model-capacity`

```text
[--ros-scene-model-capacity] <environment:str> <alias:str> <sessions:u64> <requests:u64> <input-bytes:u64> <input-count:u64> <parameter-bytes:u64> <output-bytes:u64> <output-items:u64> <workspace:u64> <deadline-ns:u64>
```

Set one model binding's finite reservations and deadline; independent of optional rate attenuation.

## `--ros-scene-model-channel`

```text
[--ros-scene-model-channel] <environment:str> <alias:str> <fd:u32> <peer-pid:u32> <peer-uid:u32> <peer-gid:u32> <deployment:hex128> <vm:hex128> <scope:hex128> <provider:hex128> <session:hex128> <contract:sha256> <request-bytes:u64> <response-bytes:u64> <startup-ns:u64> <poll-ns:u64> <supervisor-bytes:u64> <session-until-ns:u64> <identity-fd:u32> <peer-time-namespace-fd:u32>
```

Require pinned mutual TLS 1.3 and provisioned peer clock evidence. Read the secret only from a sealed identity descriptor.

## `--ros-scene-output`

```text
[--ros-scene-output] <environment:str> <alias:str> <bytes:u64> <objects:u64> <object-bytes:u64> <transfer:u64>
```

Configure an explicit cache grant as a bounded session-local output; no durable storage or host path.

## `--ros-scene-output-budget`

```text
[--ros-scene-output-budget] <environment:str> <bytes:u64> <objects:u64> <receipt-bytes:u64>
```

Reserve finite volatile output bytes, objects and last-request recovery bytes; defaults are zero.

## `--ros-scene-parameter-budget`

```text
[--ros-scene-parameter-budget] <environment:str> <name:str> <value:u64>
```

Set one mandatory finite aggregate parameter reservation; grants no node or parameter key.

## `--ros-scene-parameter-definition`

```text
[--ros-scene-parameter-definition] <environment:str> <alias:str> <path:path> <bytes:u64> <sha256:hex>
```

Read an exact bounded authenticated regular parameter definition file without starting native work.

## `--ros-scene-parameter-definition-hex`

```text
[--ros-scene-parameter-definition-hex] <environment:str> <alias:str> <definition:hex>
```

Parse one bounded canonical parameter definition after explicit profile and capacity configuration.

## `--ros-scene-parameter-limit`

```text
[--ros-scene-parameter-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one explicit parameter storage, parser, cursor or validation bound; zero is literal.

## `--ros-scene-parameter-profile`

```text
[--ros-scene-parameter-profile] <environment:str> <alias:str> <node-alias:str> <profile:str> <sha256:hex> <node-profile:str> <node-sha256:hex> <algebra:ros1|ros2>
```

Bind local parameter semantics to one exact node grant and immutable profiles.

## `--ros-scene-proposal-arbiter`

```text
[--ros-scene-proposal-arbiter] <environment:str> <alias:str> <proposal-alias:str> <target-alias:str> <tool-mask:u64>
```

Bind one existing arbiter grant to exact proposal and target aliases plus a finite tool mask; acceptance never executes.

## `--ros-scene-proposal-budget`

```text
[--ros-scene-proposal-budget] <environment:str> <providers:u64> <arbiters:u64> <producer-sessions:u64> <capture-handles:u64> <retained-bytes:u64>
```

Reserve exact finite proposal/arbiter bindings and producer/capture recovery tables; zero remains zero.

## `--ros-scene-proposal-definition`

```text
[--ros-scene-proposal-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Load one bounded exact-hash source/tool definition; current target adapters accept local nonphysical lifecycle only.

## `--ros-scene-proposal-definition-hex`

```text
[--ros-scene-proposal-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply canonical proposal definition bytes as bounded lowercase hex.

## `--ros-scene-proposal-limit`

```text
[--ros-scene-proposal-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory finite proposal capacity; capture, publish, read and accept rights remain independent.

## `--ros-scene-proposal-profile`

```text
[--ros-scene-proposal-profile] <environment:str> <alias:str> <identity:str> <sha256:hex> <model-alias:str>
```

Select exact proposal semantics and an already configured model/task pair; no implicit source, model or target permission.

## `--ros-scene-publication`

```text
[--ros-scene-publication] <environment:str> <alias:str> <scratch-bytes:u64> <scratch-calls:u64>
```

Bind finite publication scratch to an existing output plan; does not grant any action.

## `--ros-scene-publication-budget`

```text
[--ros-scene-publication-budget] <environment:str> <bytes:u64> <calls:u64>
```

Reserve aggregate complete-publication carrier bytes and in-flight calls; defaults are zero.

## `--ros-scene-recording-budget`

```text
[--ros-scene-recording-budget] <environment:str> <name:str> <value:u64>
```

Set one mandatory finite reservation for private recordings and immutable replay cursors.

## `--ros-scene-recording-definition`

```text
[--ros-scene-recording-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read one bounded authenticated regular configuration file after grant and capacity checks; grants no guest filesystem access.

## `--ros-scene-recording-definition-hex`

```text
[--ros-scene-recording-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Decode bounded lowercase canonical channel bytes using the compiled schema registry and configured logical digest.

## `--ros-scene-recording-limit`

```text
[--ros-scene-recording-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory finite recording storage, message or definition capacity; zero remains zero.

## `--ros-scene-recording-profile`

```text
[--ros-scene-recording-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select an exact hashed ROS channel definition under an existing private-recording grant.

## `--ros-scene-replay`

```text
[--ros-scene-replay] <environment:str> <alias:str> <source-alias:str> <identity:str> <sha256:hex> <handles:u64>
```

Bind immutable replay to one exact recording source, profile and finite handle count; no paths or native ROS transport.

## `--ros-scene-sensor-budget`

```text
[--ros-scene-sensor-budget] <environment:str> <bindings:u64> <storage-bytes:u64>
```

Reserve finite receive-only sensor bindings and native retained storage; grants no read authority.

## `--ros-scene-sensor-definition`

```text
[--ros-scene-sensor-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Load one bounded exact-hash sensor definition; no native sensor access during configuration.

## `--ros-scene-sensor-definition-hex`

```text
[--ros-scene-sensor-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply authenticated sensor definition bytes as lowercase hex without reading a configuration file.

## `--ros-scene-sensor-fd`

```text
[--ros-scene-sensor-fd] <environment:str> <alias:str> <descriptor:u64>
```

Select a trusted inherited descriptor; activation requires a nonblocking read-only pipe or raw terminal, and check never probes it.

## `--ros-scene-sensor-limit`

```text
[--ros-scene-sensor-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory finite sensor capacity after its definition; does not add rate attenuation.

## `--ros-scene-sensor-profile`

```text
[--ros-scene-sensor-profile] <environment:str> <alias:str> <identity:str> <sha256:hex> <schema:str>
```

Select an authenticated sensor definition and fixed compiled schema octets; no validator address or guest endpoint.

## `--ros-scene-transform-budget`

```text
[--ros-scene-transform-budget] <environment:str> <sessions:u64> <queries:u64> <data-bytes:u64> <workspace-bytes:u64> <query-bytes:u64> <carrier-bytes:u64>
```

Reserve aggregate transform sessions, queries, data, scratch, query records and carrier copies; no implicit capacity.

## `--ros-scene-transform-definition`

```text
[--ros-scene-transform-definition] <environment:str> <alias:str> <host-path:str> <bytes:u64> <sha256:hex>
```

Read one bounded regular host configuration file and verify its SHA256 before freezing policy; no guest file authority.

## `--ros-scene-transform-definition-hex`

```text
[--ros-scene-transform-definition-hex] <environment:str> <alias:str> <canonical-bytes:hex>
```

Supply an owned canonical frame definition without filesystem access; same digest and schema checks.

## `--ros-scene-transform-limit`

```text
[--ros-scene-transform-limit] <environment:str> <alias:str> <name:str> <value:u64>
```

Set one mandatory finite transform bound by semantic name; zero is literal.

## `--ros-scene-transform-profile`

```text
[--ros-scene-transform-profile] <environment:str> <alias:str> <identity:str> <sha256:hex>
```

Select one exact canonical frame definition under a local-transform grant.
