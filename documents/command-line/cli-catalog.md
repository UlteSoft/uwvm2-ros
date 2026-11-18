# Complete advertised CLI catalog

Generated from an actual Linux integration binary's `--help all`.
This catalog contains every advertised primary command, its aliases in
the usage line, and the binary's own description. Availability and
supported transports still follow the ROS product policy. A registered
platform-specific option is not proof that its backend is available.

See the [operator whitepaper](../user-whitepaper.md) for build, permissions,
debugging, update boundaries and exit status, and the
[topic references](README.md) for semantic details and subcommands.

Regenerate with `tools/documentation/generate_cli_reference.py --vm
/absolute/path/to/uwvm2-ros --output documents/command-line/cli-catalog.md`.
An existing exact help capture can instead be supplied with `--help-file`.
The generator does not run a Guest or confer any permissions.

Advertised primary commands: **290**.

## `--debug-dump-force`

```text
[--debug-dump-force] <none|diagnostic|full>
```

Force every dump to this disclosure level; none disables all dumps. Cannot be changed by a debug session. Mutually exclusive with --debug-dump-max; never grants debugger or restore authority.

## `--debug-dump-max`

```text
[--debug-dump-max] <diagnostic|full>
```

Launch-only dump disclosure ceiling (default: full); each export may select less. Diagnostic includes static Wasm and fault locations, not dynamic sensitive state. Mutually exclusive with --debug-dump-force; debugger permissions still apply.

## `--debug-fault-action`

```text
[--debug-fault-action] <crash|pause|exit>
```

Unhandled Wasm fault: crash (default; OS-configured core dumps), pause for authorized inspection, or exit with stable status 120/121/122. Internal VM failures always crash; no automatic dump, restart or recovery.

## `--debug-jit-control-fd`

```text
[--debug-jit-control-fd] <connected-fd>
```

Opt in to late LLVM-full debugger commands from a trusted inherited Linux packet or macOS framed Unix endpoint.

## `--debug-jit-control-handle`

```text
[--debug-jit-control-handle] <connected-decimal-handle>
```

Windows only: opt in to LLVM-full late debugger commands from a direct-parent inherited private pipe HANDLE.

## `--debug-permissions`

```text
[--debug-permissions] <full|read-only|limited|none>
```

Debugger launch permissions only: full (inspect/modify/control execution), read-only (inspect/full dump; no execution control), limited (non-sensitive diagnostics/dump only; default), none (no debugger or dump). -Rdbg and -m debug default to full; other modes default to limited; explicit selection wins. Does not grant imports, drivers or updates.

## `--debug-thread-history`

```text
[--debug-thread-history] <record|replay> <path> <control-storage-bytes>
```

Record a complete private WASI Threads execution to a unified snapshot, or replay it without real WASI/ROS I/O. Requires full debug and full dump permissions. Original source must match. Control storage is an explicit native resource budget; history has no implicit quota.

## `--diagnostic-control-fd`

```text
[--diagnostic-control-fd] <fd>
```

Non-sensitive read-only diagnostics on a separate authenticated Unix seqpacket socket.

## `--disable-warning`

```text
[--disable-warning] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|vm|untrusted-dl|deprecated-ros|retained-ros-input|depend|runtime|runtime-compile-threads]
```

Disable selected warning categories.

## `--enable-warning`

```text
[--enable-warning] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|parser|wasip1|untrusted-dl|deprecated-ros|retained-ros-input]
```

Enable warnings; host quota diagnostics default off. Select all_host_calls[_wasip1|_wasithread|_ros1|_ros2], all_host_denied/all_host_limited, or host_denied_FAMILY.API / host_limited_FAMILY.API.

## `--help`

```text
[--help|-h] ([all|global|debug|update|wasm|secure|log|runtime|wasi|rosv2])
```

Show help information.

## `--log-color`

```text
[--log-color|-log-c] [enable|disable]
```

Force colored log output on or off.

## `--log-output`

```text
[--log-output|-log] [out|err|file <file:path>]
```

Set the uwvm log output target. Default: err.

## `--log-verbose`

```text
[--log-verbose|-log-vb]
```

Enable verbose logging.

## `--mode`

```text
[--mode|-m] [section-details|validation|run|debug]
```

Select the operation mode. Default: run.

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

## `--run`

```text
[--run|-r|--] <file argv[0]:path> <argv[1]:str> <argv[2]:str> ...
```

Run a WebAssembly module.

## `--runtime-aot`

```text
[--runtime-aot|-Raot]
```

Select the full-module LLVM AOT backend.

## `--runtime-compile-threads`

```text
[--runtime-compile-threads|-Rct] [default|aggressive|<count:ssize_t>]
```

Set the runtime compile thread count or adaptive policy.

## `--runtime-compiler-log`

```text
[--runtime-compiler-log|-Rclog] [out|err|file <file:path>]
```

Write runtime compiler logs to stdout, stderr, or a dedicated file.

## `--runtime-debug`

```text
[--runtime-debug|-Rdbg]
```

Start the LLVM JIT debugger; default to full debug permissions unless --debug-permissions is explicit.

## `--runtime-disable-memtag`

```text
[--runtime-disable-memtag]
```

Disable WasmMTE tag execution and checks while retaining memory-zeroing semantics; extension validation remains enabled.

## `--runtime-int`

```text
[--runtime-int|-Rint]
```

Select the full-module uwvm interpreter backend.

## `--runtime-llvm-jit-cache-path`

```text
[--runtime-llvm-jit-cache-path|-Rllvm-cache-path] [disable|default|path <path>]
```

Select the persistent LLVM cache directory or disable it. Persistence requires a non-dumpable Linux process, authenticated current image and kernel-key origin; otherwise it is disabled.

## `--runtime-llvm-jit-cache-seal-process`

```text
[--runtime-llvm-jit-cache-seal-process|-Rllvm-cache-seal-process]
```

Explicit Linux-only process seal (PR_SET_DUMPABLE=0) before execution for authenticated LLVM disk cache eligibility. Disables normal core dumps and usual same-UID ptrace attachment; DBG/VTune compatibility requires separate qualification. Default process policy is unchanged; no automatic restoration. A seal does not guarantee a cache hit.

## `--runtime-llvm-jit-exception-dispatch`

```text
[--runtime-llvm-jit-exception-dispatch|-Rllvm-exception-dispatch] [auto|native-unwind]
```

Use native LLVM exception dispatch; auto resolves to native-unwind. No instruction-based EH fallback.

## `--runtime-llvm-jit-full-policy`

```text
[--runtime-llvm-jit-full-policy|-Rllvm-full-policy] [auto|debug|legacy-light|pb-o1|pb-o2|pb-o3]
```

Select the full-module LLVM AOT strategy policy.

## `--runtime-llvm-jit-policy`

```text
[--runtime-llvm-jit-policy|-Rllvm-policy] [debug|default|fast-compile|balanced|max]
```

Select the high-level full-module LLVM AOT strategy policy.

## `--runtime-memtag-action`

```text
[--runtime-memtag-action] <crash|silent-log <out|err|path <PATH>>>
```

MTE action: crash (default), or silent-log with an explicit independent out, err, or path destination. Record every violation and continue; path appends JSON records. --log-output never redirects MTE reports. Bounds and host permissions remain enforced; report I/O failure terminates.

## `--runtime-memtag-backend`

```text
[--runtime-memtag-backend] <software|hardware>
```

MTE backend: software (default), or hardware. Unsupported hardware/OS combinations are launch errors. Silent-log diagnostics use precise software detection to preserve every violation.

## `--runtime-memtag-mode`

```text
[--runtime-memtag-mode] <sync|async>
```

MTE timing: sync (default), or async. Independent of backend and fault action; ordinary sandbox bounds remain immediate.

## `--runtime-scheduling-policy`

```text
[--runtime-scheduling-policy|-Rsp] [func_count <count:size_t>|code_size <bytes:size_t>]
```

Set the scheduling policy for extra runtime compile threads.

## `--runtime-uwvm-int-disable-delay-local`

```text
[--runtime-uwvm-int-disable-delay-local|-Rint-no-delay-local]
```

Disable uwvm-int delay-local peepholes.

## `--runtime-uwvm-int-disable-loop-unwind`

```text
[--runtime-uwvm-int-disable-loop-unwind|-Rint-no-loop-unwind]
```

Disable uwvm-int loop-unwind translation.

## `--runtime-uwvm-int-enable-instruction-reorder`

```text
[--runtime-uwvm-int-enable-instruction-reorder|-Rint-reorder]
```

Enable experimental uwvm-int register-ring-aware instruction reorder.

## `--runtime-uwvm-int-loop-unwind-max-size`

```text
[--runtime-uwvm-int-loop-unwind-max-size|-Rint-loop-unwind-size] <bytes:size_t>
```

Set the uwvm-int loop-unwind Wasm body byte-size budget.

## `--runtime-uwvm-int-set-opcode-conbination-level`

```text
[--runtime-uwvm-int-set-opcode-conbination-level|-Rint-op-conbine-level] [disable|soft]
```

Set the uwvm-int opcode conbination peephole level.

## `--secure`

```text
[--secure] [help|status|module <wasm:path> <protected-policy:path>|named-module <wasm:path> <protected-policy:path> <expected-id>]
```

Configure offline cryptographic verification for explicitly selected WASM modules. Use --secure help for subcommands.

## `--update-call-export`

```text
[--update-call-export] <name>
```

Permit one named business export on the update channel; scalar ABI only, disabled by default.

## `--update-contract`

```text
[--update-contract] <hex>
```

Initial approved permission contract reference; changes require a separately signed policy transition.

## `--update-control-fd`

```text
[--update-control-fd] <fd>
```

Production patch manager on an authenticated inherited Unix stream; separate from debugging.

## `--update-public-key`

```text
[--update-public-key] <hex>
```

Trusted local deployment Ed25519 public key (32 bytes, hexadecimal).

## `--update-scope`

```text
[--update-scope] <hex>
```

Stable deployment/component/device scope SHA-256 assigned by the trusted launcher.

## `--version`

```text
[--version|-v|-ver]
```

Show version information.

## `--warning-as-error`

```text
[--warning-as-error] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|vm|parser|untrusted-dl|deprecated-ros|retained-ros-input|depend|wasip1|runtime|runtime-compile-threads]
```

Treat selected warnings as fatal errors.

## `--wasi-disable-utf8-check`

```text
[--wasi-disable-utf8-check|-Iu8relax]
```

Disable WASI path/name UTF-8 checks for command-line and runtime; does not affect wasm parsing or module-name validation.

## `--wasi-thread-signals`

```text
[--wasi-thread-signals] <read-fd> <cookie> <max-frames> <max-pipe-bytes> <grace-ms> <retire-ms>
```

Explicit Linux CLI SIGINT/SIGTERM delivery to the Source-owned WASI pipe; separately requires --wasi-threads.

## `--wasi-threads`

```text
[--wasi-threads] <max-active> <max-spawns> <native-stack-bytes> <per-child-storage> <per-child-code> <shared-memory-bytes>
```

Select WASI Threads; max-active/max-spawns accept unlimited. Four finite storage capacities bound each Guest/compiler mapping, including guards.

## `--wasip1-allow-overlapping-mount-paths`

```text
[--wasip1-allow-overlapping-mount-paths]
```

Allow duplicate or overlapping WASI Preview 1 mount guest paths.

## `--wasip1-disable-mount-path-normalization`

```text
[--wasip1-disable-mount-path-normalization]
```

Store raw WASI Preview 1 mount guest paths instead of normalized paths.

## `--wasip1-global-add-module`

```text
[--wasip1-global-add-module] <module:str>
```

Bind a module to the shared Global WASIp1 environment. Conflicts with Single/Group membership; grants no authority.

## `--wasip1-global-add-or-replace-environment`

```text
[--wasip1-global-add-or-replace-environment] <env:str> <value:str>
```

Add or replace one environment variable for the Global group.

## `--wasip1-global-delete-system-environment`

```text
[--wasip1-global-delete-system-environment] <env:str>
```

Delete one inherited host environment variable for the Global group.

## `--wasip1-global-deny`

```text
[--wasip1-global-deny] <permission:str>
```

Explicitly deny one finite WASI permission for the Global group.

## `--wasip1-global-disable`

```text
[--wasip1-global-disable]
```

Disable WASI Preview 1 for the Global group.

## `--wasip1-global-disable-utf8-check`

```text
[--wasip1-global-disable-utf8-check]
```

Disable WASI Preview 1 UTF-8 checks for the Global group.

## `--wasip1-global-enable`

```text
[--wasip1-global-enable]
```

Enable WASI Preview 1 for the Global group.

## `--wasip1-global-enable-utf8-check`

```text
[--wasip1-global-enable-utf8-check]
```

Enable WASI Preview 1 UTF-8 checks for the Global group.

## `--wasip1-global-force-args`

```text
[--wasip1-global-force-args] <num:size_t> [arg:str]...
```

Replace the Global group's WASI Preview 1 argv vector with an explicit argument list.

## `--wasip1-global-grant`

```text
[--wasip1-global-grant] <permission:str>
```

Explicitly grant one finite WASI permission for the Global group.

## `--wasip1-global-inherit-system-environment`

```text
[--wasip1-global-inherit-system-environment]
```

Inherit host environment variables into the Global group.

## `--wasip1-global-interface`

```text
[--wasip1-global-interface] <api:str> <hidden|deny|allow>
```

Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden.

## `--wasip1-global-limit`

```text
[--wasip1-global-limit] <limit:str> <value:u64|unlimited>
```

Set one explicit WASI resource limit (decimal or unlimited; no implicit grant).

## `--wasip1-global-limit-action`

```text
[--wasip1-global-limit-action] <limit:str> <reject|wait>
```

Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency.

## `--wasip1-global-limit-exit-action`

```text
[--wasip1-global-limit-exit-action] <limit:str> <cancel|wait>
```

Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel.

## `--wasip1-global-mount-dir`

```text
[--wasip1-global-mount-dir] <wasi dir:str> <system dir:path>
```

Mount a host directory into the Global group's WASI Preview 1 sandbox.

## `--wasip1-global-mount-dir-rights`

```text
[--wasip1-global-mount-dir-rights] <guest-dir:str> <base:u64> <inheriting:u64>
```

Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged.

## `--wasip1-global-noinherit-system-environment`

```text
[--wasip1-global-noinherit-system-environment]
```

Do not inherit host environment variables into the Global group.

## `--wasip1-global-set-argv0`

```text
[--wasip1-global-set-argv0] <argv0:str>
```

Override WASI Preview 1 argv[0] for the Global group.

## `--wasip1-global-set-fd-limit`

```text
[--wasip1-global-set-fd-limit] <limit:size_t>
```

Set the WASI Preview 1 fd limit for the Global group.

## `--wasip1-global-socket-rights`

```text
[--wasip1-global-socket-rights] <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

## `--wasip1-global-socket-tcp-connect`

```text
[--wasip1-global-socket-tcp-connect] <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 TCP client socket for the Global group.

## `--wasip1-global-socket-tcp-listen`

```text
[--wasip1-global-socket-tcp-listen] <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Listen on a WASI Preview 1 TCP server socket for the Global group.

## `--wasip1-global-socket-udp-bind`

```text
[--wasip1-global-socket-udp-bind] <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Bind a WASI Preview 1 UDP socket for the Global group.

## `--wasip1-global-socket-udp-connect`

```text
[--wasip1-global-socket-udp-connect] <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 UDP socket for the Global group.

## `--wasip1-group-add-module`

```text
[--wasip1-group-add-module] <group:str> <module:str>
```

Bind one module name to an existing named WASI Preview 1 group.

## `--wasip1-group-add-or-replace-environment`

```text
[--wasip1-group-add-or-replace-environment] <group:str> <env:str> <value:str>
```

Add or replace one environment variable for one named group.

## `--wasip1-group-create`

```text
[--wasip1-group-create] <group:str>
```

Create one named shared WASI Preview 1 module group.

## `--wasip1-group-delete-system-environment`

```text
[--wasip1-group-delete-system-environment] <group:str> <env:str>
```

Delete one inherited host environment variable for one named group.

## `--wasip1-group-deny`

```text
[--wasip1-group-deny] <group:str> <permission:str>
```

Explicitly deny one finite WASI permission for the named group.

## `--wasip1-group-disable`

```text
[--wasip1-group-disable] <group:str>
```

Disable WASI Preview 1 for one named group.

## `--wasip1-group-disable-utf8-check`

```text
[--wasip1-group-disable-utf8-check] <group:str>
```

Disable WASI Preview 1 UTF-8 checks for one named group.

## `--wasip1-group-enable`

```text
[--wasip1-group-enable] <group:str>
```

Enable WASI Preview 1 for one named group.

## `--wasip1-group-enable-utf8-check`

```text
[--wasip1-group-enable-utf8-check] <group:str>
```

Enable WASI Preview 1 UTF-8 checks for one named group.

## `--wasip1-group-force-args`

```text
[--wasip1-group-force-args] <group:str> <num:size_t> [arg:str]...
```

Replace one named group's WASI Preview 1 argv vector with an explicit argument list.

## `--wasip1-group-grant`

```text
[--wasip1-group-grant] <group:str> <permission:str>
```

Explicitly grant one finite WASI permission for the named group.

## `--wasip1-group-inherit-system-environment`

```text
[--wasip1-group-inherit-system-environment] <group:str>
```

Inherit host environment variables into one named group.

## `--wasip1-group-interface`

```text
[--wasip1-group-interface] <group:str> <api:str> <hidden|deny|allow>
```

Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden.

## `--wasip1-group-limit`

```text
[--wasip1-group-limit] <group:str> <limit:str> <value:u64|unlimited>
```

Set one explicit WASI resource limit (decimal or unlimited; no implicit grant).

## `--wasip1-group-limit-action`

```text
[--wasip1-group-limit-action] <group:str> <limit:str> <reject|wait>
```

Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency.

## `--wasip1-group-limit-exit-action`

```text
[--wasip1-group-limit-exit-action] <group:str> <limit:str> <cancel|wait>
```

Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel.

## `--wasip1-group-mount-dir`

```text
[--wasip1-group-mount-dir] <group:str> <wasi dir:str> <system dir:path>
```

Mount a host directory into one named group's WASI Preview 1 sandbox.

## `--wasip1-group-mount-dir-rights`

```text
[--wasip1-group-mount-dir-rights] <group:str> <guest-dir:str> <base:u64> <inheriting:u64>
```

Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged.

## `--wasip1-group-noinherit-system-environment`

```text
[--wasip1-group-noinherit-system-environment] <group:str>
```

Do not inherit host environment variables into one named group.

## `--wasip1-group-set-argv0`

```text
[--wasip1-group-set-argv0] <group:str> <argv0:str>
```

Override WASI Preview 1 argv[0] for one named group.

## `--wasip1-group-set-fd-limit`

```text
[--wasip1-group-set-fd-limit] <group:str> <limit:size_t>
```

Set the WASI Preview 1 fd limit for one named group.

## `--wasip1-group-socket-rights`

```text
[--wasip1-group-socket-rights] <group:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

## `--wasip1-group-socket-tcp-connect`

```text
[--wasip1-group-socket-tcp-connect] <group:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 TCP client socket for one named group.

## `--wasip1-group-socket-tcp-listen`

```text
[--wasip1-group-socket-tcp-listen] <group:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Listen on a WASI Preview 1 TCP server socket for one named group.

## `--wasip1-group-socket-udp-bind`

```text
[--wasip1-group-socket-udp-bind] <group:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Bind a WASI Preview 1 UDP socket for one named group.

## `--wasip1-group-socket-udp-connect`

```text
[--wasip1-group-socket-udp-connect] <group:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 UDP socket for one named group.

## `--wasip1-single-add-or-replace-environment`

```text
[--wasip1-single-add-or-replace-environment] <module:str> <env:str> <value:str>
```

Add or replace one environment variable for one single module.

## `--wasip1-single-create`

```text
[--wasip1-single-create] <module:str>
```

Create one anonymous module-local WASI Preview 1 group.

## `--wasip1-single-delete-system-environment`

```text
[--wasip1-single-delete-system-environment] <module:str> <env:str>
```

Delete one inherited host environment variable for one single module.

## `--wasip1-single-deny`

```text
[--wasip1-single-deny] <module:str> <permission:str>
```

Explicitly deny one finite WASI permission for the single module.

## `--wasip1-single-disable`

```text
[--wasip1-single-disable] <module:str>
```

Disable WASI Preview 1 for one single module.

## `--wasip1-single-disable-utf8-check`

```text
[--wasip1-single-disable-utf8-check] <module:str>
```

Disable WASI Preview 1 UTF-8 checks for one single module.

## `--wasip1-single-enable`

```text
[--wasip1-single-enable] <module:str>
```

Enable WASI Preview 1 for one single module.

## `--wasip1-single-enable-utf8-check`

```text
[--wasip1-single-enable-utf8-check] <module:str>
```

Enable WASI Preview 1 UTF-8 checks for one single module.

## `--wasip1-single-expose-host-api`

```text
[--wasip1-single-expose-host-api] <module:str>
```

Expose the Preview 1 preload host API to one single module.

## `--wasip1-single-force-args`

```text
[--wasip1-single-force-args] <module:str> <num:size_t> [arg:str]...
```

Replace one single module's WASI Preview 1 argv vector with an explicit argument list.

## `--wasip1-single-grant`

```text
[--wasip1-single-grant] <module:str> <permission:str>
```

Explicitly grant one finite WASI permission for the single module.

## `--wasip1-single-hide-host-api`

```text
[--wasip1-single-hide-host-api] <module:str>
```

Hide the Preview 1 preload host API from one single module.

## `--wasip1-single-inherit-system-environment`

```text
[--wasip1-single-inherit-system-environment] <module:str>
```

Inherit host environment variables into one single module.

## `--wasip1-single-interface`

```text
[--wasip1-single-interface] <single:str> <api:str> <hidden|deny|allow>
```

Configure one exact WASIp1 API: hidden fails linking; deny exposes but rejects; allow retains resource checks. Default hidden.

## `--wasip1-single-limit`

```text
[--wasip1-single-limit] <single:str> <limit:str> <value:u64|unlimited>
```

Set one explicit WASI resource limit (decimal or unlimited; no implicit grant).

## `--wasip1-single-limit-action`

```text
[--wasip1-single-limit-action] <single:str> <limit:str> <reject|wait>
```

Choose reject or synchronous wait for an explicit operation limit; wait supports windows and concurrency.

## `--wasip1-single-limit-exit-action`

```text
[--wasip1-single-limit-exit-action] <single:str> <limit:str> <cancel|wait>
```

Choose cancel or wait on guest exit for one window/concurrency quota or native-wait; default cancel.

## `--wasip1-single-mount-dir`

```text
[--wasip1-single-mount-dir] <module:str> <wasi dir:str> <system dir:path>
```

Mount a host directory into one single module's WASI Preview 1 sandbox.

## `--wasip1-single-mount-dir-rights`

```text
[--wasip1-single-mount-dir-rights] <single:str> <guest-dir:str> <base:u64> <inheriting:u64>
```

Set explicit base/inheriting rights for one previously declared preopen; other roots are unchanged.

## `--wasip1-single-noinherit-system-environment`

```text
[--wasip1-single-noinherit-system-environment] <module:str>
```

Do not inherit host environment variables into one single module.

## `--wasip1-single-set-argv0`

```text
[--wasip1-single-set-argv0] <module:str> <argv0:str>
```

Override WASI Preview 1 argv[0] for one single module.

## `--wasip1-single-set-fd-limit`

```text
[--wasip1-single-set-fd-limit] <module:str> <limit:size_t>
```

Set the WASI Preview 1 fd limit for one single module.

## `--wasip1-single-socket-rights`

```text
[--wasip1-single-socket-rights] <single:str> <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

## `--wasip1-single-socket-tcp-connect`

```text
[--wasip1-single-socket-tcp-connect] <module:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 TCP client socket for one single module.

## `--wasip1-single-socket-tcp-listen`

```text
[--wasip1-single-socket-tcp-listen] <module:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Listen on a WASI Preview 1 TCP server socket for one single module.

## `--wasip1-single-socket-udp-bind`

```text
[--wasip1-single-socket-udp-bind] <module:str> <fd:i32> [<ipv4|ipv6>:<port>|unix <path>]
```

Bind a WASI Preview 1 UDP socket for one single module.

## `--wasip1-single-socket-udp-connect`

```text
[--wasip1-single-socket-udp-connect] <module:str> <fd:i32> [<ipv4|ipv6|dns>:<port>|unix <path>]
```

Connect a WASI Preview 1 UDP socket for one single module.

## `--wasip1-socket-abi`

```text
[--wasip1-socket-abi] <standard|wasix>
```

Choose the immutable wasm32 sock_accept ABI for this process: standard (default, 3 arguments) or wasix (4 arguments, peer address). Grants no interfaces or socket rights.

## `--wasithread-global-disable`

```text
[--wasithread-global-disable|-I1Tdisable]
```

Hide this scope; no new thread creation is authorized.

## `--wasithread-global-enable`

```text
[--wasithread-global-enable|-I1Tenable]
```

Enable this explicit scope; interfaces remain separately controlled.

## `--wasithread-global-inspect`

```text
[--wasithread-global-inspect|-I1Tinspect]
```

Read I1T policy and reservation counters without changing authority.

## `--wasithread-global-interface`

```text
[--wasithread-global-interface|-I1Tinterface] thread-spawn <hidden|deny|allow>
```

Set thread-spawn hidden, deny or allow; independent of WASIp1.

## `--wasithread-global-limit`

```text
[--wasithread-global-limit|-I1Tlimit] <max-extra-threads|max-threads|max-spawns> <uint|unlimited>
```

Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group.

## `--wasithread-global-limit-ratio`

```text
[--wasithread-global-limit-ratio|-I1Tlimit-ratio] <max-extra-threads|max-threads> <ratio:f32>
```

Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted.

## `--wasithread-group-add-module`

```text
[--wasithread-group-add-module|-I1TGadd-module] <group> <module>
```

Bind an exact module to this Group once; unspecified modules use Global.

## `--wasithread-group-create`

```text
[--wasithread-group-create|-I1TGcreate] <group>
```

Create a denied scope; Single also binds its exact module name.

## `--wasithread-group-disable`

```text
[--wasithread-group-disable|-I1TGdisable] <group>
```

Hide this scope; no new thread creation is authorized.

## `--wasithread-group-enable`

```text
[--wasithread-group-enable|-I1TGenable] <group>
```

Enable this explicit scope; interfaces remain separately controlled.

## `--wasithread-group-inspect`

```text
[--wasithread-group-inspect|-I1TGinspect] <group>
```

Read I1T policy and reservation counters without changing authority.

## `--wasithread-group-interface`

```text
[--wasithread-group-interface|-I1TGinterface] <group> thread-spawn <hidden|deny|allow>
```

Set thread-spawn hidden, deny or allow; independent of WASIp1.

## `--wasithread-group-limit`

```text
[--wasithread-group-limit|-I1TGlimit] <group> <max-extra-threads|max-threads|max-spawns> <uint|unlimited>
```

Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group.

## `--wasithread-group-limit-ratio`

```text
[--wasithread-group-limit-ratio|-I1TGlimit-ratio] <group> <max-extra-threads|max-threads> <ratio:f32>
```

Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted.

## `--wasithread-single-create`

```text
[--wasithread-single-create|-I1TScreate] <module>
```

Create a denied scope; Single also binds its exact module name.

## `--wasithread-single-disable`

```text
[--wasithread-single-disable|-I1TSdisable] <module>
```

Hide this scope; no new thread creation is authorized.

## `--wasithread-single-enable`

```text
[--wasithread-single-enable|-I1TSenable] <module>
```

Enable this explicit scope; interfaces remain separately controlled.

## `--wasithread-single-inspect`

```text
[--wasithread-single-inspect|-I1TSinspect] <module>
```

Read I1T policy and reservation counters without changing authority.

## `--wasithread-single-interface`

```text
[--wasithread-single-interface|-I1TSinterface] <module> thread-spawn <hidden|deny|allow>
```

Set thread-spawn hidden, deny or allow; independent of WASIp1.

## `--wasithread-single-limit`

```text
[--wasithread-single-limit|-I1TSlimit] <module> <max-extra-threads|max-threads|max-spawns> <uint|unlimited>
```

Attenuate admitted thread counts. Zero is a real zero; Global caps also cover Single and Group.

## `--wasithread-single-limit-ratio`

```text
[--wasithread-single-limit-ratio|-I1TSlimit-ratio] <module> <max-extra-threads|max-threads> <ratio:f32>
```

Floor a finite f32 ratio in [0,1] of the observable system thread ceiling. No authority is granted.

## `--wasm-depend-recursion-limit`

```text
[--wasm-depend-recursion-limit|-Wdeplim] <depth:size_t>
```

Set the dependency-check recursion limit (0 = unlimited).

## `--wasm-feature-disable-bulk-memory`

```text
[--wasm-feature-disable-bulk-memory|-WFD-bulk-memory]
```

Disable the WebAssembly bulk-memory feature.

## `--wasm-feature-disable-exceptions`

```text
[--wasm-feature-disable-exceptions|-WFD-exceptions]
```

Disable the WebAssembly exceptions feature.

## `--wasm-feature-disable-extended-const`

```text
[--wasm-feature-disable-extended-const|-WFD-extended-const]
```

Disable the WebAssembly extended-const feature.

## `--wasm-feature-disable-function-references`

```text
[--wasm-feature-disable-function-references|-WFD-function-references]
```

Disable the WebAssembly function-references feature.

## `--wasm-feature-disable-gc`

```text
[--wasm-feature-disable-gc|-WFD-gc]
```

Disable the WebAssembly gc feature.

## `--wasm-feature-disable-memory64`

```text
[--wasm-feature-disable-memory64|-WFD-memory64]
```

Disable the WebAssembly memory64 feature.

## `--wasm-feature-disable-memtag-extension`

```text
[--wasm-feature-disable-memtag-extension]
```

Reject the nonstandard WasmMTE extension (enabled by default); also disable its execution.

## `--wasm-feature-disable-multi-memory`

```text
[--wasm-feature-disable-multi-memory|-WFD-multi-memory]
```

Disable the WebAssembly multi-memory feature.

## `--wasm-feature-disable-multi-value`

```text
[--wasm-feature-disable-multi-value|-WFD-multi-value]
```

Disable the WebAssembly multi-value feature.

## `--wasm-feature-disable-multiple-tables`

```text
[--wasm-feature-disable-multiple-tables|-WFD-multiple-tables]
```

Disable the WebAssembly multiple-tables feature.

## `--wasm-feature-disable-nontrapping-float-to-int`

```text
[--wasm-feature-disable-nontrapping-float-to-int|-WFD-nontrapping-float-to-int]
```

Disable the WebAssembly nontrapping-float-to-int feature.

## `--wasm-feature-disable-reference-types`

```text
[--wasm-feature-disable-reference-types|-WFD-reference-types]
```

Disable the WebAssembly reference-types feature.

## `--wasm-feature-disable-relaxed-simd`

```text
[--wasm-feature-disable-relaxed-simd|-WFD-relaxed-simd]
```

Disable the WebAssembly relaxed-simd feature.

## `--wasm-feature-disable-sign-extension`

```text
[--wasm-feature-disable-sign-extension|-WFD-sign-extension]
```

Disable the WebAssembly sign-extension feature.

## `--wasm-feature-disable-simd`

```text
[--wasm-feature-disable-simd|-WFD-simd]
```

Disable the WebAssembly simd feature.

## `--wasm-feature-disable-table-initializer`

```text
[--wasm-feature-disable-table-initializer|-WFD-table-initializer]
```

Disable the WebAssembly table-initializer feature.

## `--wasm-feature-disable-table-instructions`

```text
[--wasm-feature-disable-table-instructions|-WFD-table-instructions]
```

Disable the WebAssembly table-instructions feature.

## `--wasm-feature-disable-table64`

```text
[--wasm-feature-disable-table64|-WFD-table64]
```

Disable the WebAssembly table64 feature.

## `--wasm-feature-disable-tail-call`

```text
[--wasm-feature-disable-tail-call|-WFD-tail-call]
```

Disable the WebAssembly tail-call feature.

## `--wasm-feature-disable-threads`

```text
[--wasm-feature-disable-threads|-WFD-threads]
```

Disable the WebAssembly threads feature.

## `--wasm-feature-enable-bulk-memory`

```text
[--wasm-feature-enable-bulk-memory|-WFE-bulk-memory]
```

Enable the WebAssembly bulk-memory feature.

## `--wasm-feature-enable-exceptions`

```text
[--wasm-feature-enable-exceptions|-WFE-exceptions]
```

Enable the WebAssembly exceptions feature.

## `--wasm-feature-enable-extended-const`

```text
[--wasm-feature-enable-extended-const|-WFE-extended-const]
```

Enable the WebAssembly extended-const feature.

## `--wasm-feature-enable-function-references`

```text
[--wasm-feature-enable-function-references|-WFE-function-references]
```

Enable the WebAssembly function-references feature.

## `--wasm-feature-enable-gc`

```text
[--wasm-feature-enable-gc|-WFE-gc]
```

Enable the WebAssembly gc feature.

## `--wasm-feature-enable-memory64`

```text
[--wasm-feature-enable-memory64|-WFE-memory64]
```

Enable the WebAssembly memory64 feature.

## `--wasm-feature-enable-multi-memory`

```text
[--wasm-feature-enable-multi-memory|-WFE-multi-memory]
```

Enable the WebAssembly multi-memory feature.

## `--wasm-feature-enable-multi-value`

```text
[--wasm-feature-enable-multi-value|-WFE-multi-value]
```

Enable the WebAssembly multi-value feature.

## `--wasm-feature-enable-multiple-tables`

```text
[--wasm-feature-enable-multiple-tables|-WFE-multiple-tables]
```

Enable the WebAssembly multiple-tables feature.

## `--wasm-feature-enable-nontrapping-float-to-int`

```text
[--wasm-feature-enable-nontrapping-float-to-int|-WFE-nontrapping-float-to-int]
```

Enable the WebAssembly nontrapping-float-to-int feature.

## `--wasm-feature-enable-reference-types`

```text
[--wasm-feature-enable-reference-types|-WFE-reference-types]
```

Enable the WebAssembly reference-types feature.

## `--wasm-feature-enable-relaxed-simd`

```text
[--wasm-feature-enable-relaxed-simd|-WFE-relaxed-simd]
```

Enable the WebAssembly relaxed-simd feature.

## `--wasm-feature-enable-sign-extension`

```text
[--wasm-feature-enable-sign-extension|-WFE-sign-extension]
```

Enable the WebAssembly sign-extension feature.

## `--wasm-feature-enable-simd`

```text
[--wasm-feature-enable-simd|-WFE-simd]
```

Enable the WebAssembly simd feature.

## `--wasm-feature-enable-table-initializer`

```text
[--wasm-feature-enable-table-initializer|-WFE-table-initializer]
```

Enable the WebAssembly table-initializer feature.

## `--wasm-feature-enable-table-instructions`

```text
[--wasm-feature-enable-table-instructions|-WFE-table-instructions]
```

Enable the WebAssembly table-instructions feature.

## `--wasm-feature-enable-table64`

```text
[--wasm-feature-enable-table64|-WFE-table64]
```

Enable the WebAssembly table64 feature.

## `--wasm-feature-enable-tail-call`

```text
[--wasm-feature-enable-tail-call|-WFE-tail-call]
```

Enable the WebAssembly tail-call feature.

## `--wasm-feature-enable-threads`

```text
[--wasm-feature-enable-threads|-WFE-threads]
```

Enable the WebAssembly threads feature.

## `--wasm-feature-wasm1p1`

```text
[--wasm-feature-wasm1p1|--wasm-feature-wasm1.1|-WF1.1|-WF1p1|-WFwasm1.1|-WFwasm1p1]
```

Select the WebAssembly 1.1 baseline; explicit feature enable/disable switches override it.

## `--wasm-feature-wasm2`

```text
[--wasm-feature-wasm2|-WF2|-WFwasm2]
```

Select the WebAssembly 2.0 baseline; explicit feature enable/disable switches override it.

## `--wasm-feature-wasm3`

```text
[--wasm-feature-wasm3|-WF3|-WFwasm3]
```

Select the default WebAssembly 3.0 baseline plus threads; WasmMTE remains enabled unless explicitly disabled. Explicit feature enable/disable switches override the baseline.

## `--wasm-feature-wasmmvp`

```text
[--wasm-feature-wasmmvp|--wasm-feature-mvp|-WFmvp|-WFwasmmvp|-WFwasm1|-WF1]
```

Select the MVP baseline; explicit feature enable/disable switches override it.

## `--wasm-memory-grow-strict`

```text
[--wasm-memory-grow-strict|-Wmemstrict]
```

Report recoverable host allocation/commit failures from `memory.grow` as -1; Wasm limit failures return -1 in either mode.

## `--wasm-preload-library`

```text
[--wasm-preload-library|-Wpre] <wasm:path> (<rename:str>)
```

Preload a Wasm module as a dynamic library; an optional rename sets its module name.

## `--wasm-reset-import`

```text
[--wasm-reset-import|-Wresimp] <module:str> <import-module:str> <import-extern:str> <new-import-module:str> <new-import-extern:str>
```

Rewrite a Wasm file module's import binding during runtime initialization; original exports are not renamed.

## `--wasm-set-initializer-limit`

```text
[--wasm-set-initializer-limit|-Wilim] <type:str> <limit:size_t>
```

Override a runtime initializer reserve limit.

## `--wasm-set-main-module-name`

```text
[--wasm-set-main-module-name|-Wname] <name:str>
```

Override the main Wasm module name used by uwvm.

## `--wasm-set-memory-limit`

```text
[--wasm-set-memory-limit|-Wmemlim] <module:str> [<index:size_t>|all] <min:size_t> (<max:size_t>)
```

Override runtime page limits for selected local-defined Wasm memories.

## `--wasm-set-parser-limit`

```text
[--wasm-set-parser-limit|-Wplim] <type:str> <limit:size_t>
```

Override a parser limit for Wasm modules.

## `--wasm-set-preload-module-attribute`

```text
[--wasm-set-preload-module-attribute|-Wpreattr] <module:str> <memory-access:none|copy|mmap> (<memory_index:list>)
```

Set a preload module's memory access mode; omit <memory_index:list> to apply it to all memories.

## `--wasm-set-resource-limit`

```text
[--wasm-set-resource-limit] <stage:str> <resource:str> <bytes:size_t>
```

Set host allocation budgets before loading: parser (input-bytes, type-metadata-bytes, function-body-bytes, module-code-bytes), validation (single-bytes, live-bytes), initializer (allocation-bytes), runtime (gc/table/memory/memtag/auxiliary/threads-single-bytes or -live-bytes). Bytes; 0 refuses new allocation. Shared by actual process backings, independent of import permissions.

## `--wasm-set-start-func`

```text
[--wasm-set-start-func|-Wstart] <local-func:u32> [[i32|i64|f32|f64] ...]
```

Run a main-module local defined Wasm function as the entry function.
