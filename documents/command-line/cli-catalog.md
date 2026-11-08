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

Advertised primary commands: **285**.

## `--debug`

```text
[--debug] <pause>
```

Report a terminal fault to the host and pause. The deployer may explicitly inspect/dump with an authorized debugger or manage an update; no automatic dump, retry, recovery or new authority. Omit this option for normal fault termination.

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

## `--diagnostic-control-fd`

```text
[--diagnostic-control-fd] <fd>
```

RELEASE-only bounded read-only diagnostics on a separate authenticated Unix seqpacket socket.

## `--disable-warning`

```text
[--disable-warning] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|vm|untrusted-dl|dl|depend|runtime|runtime-compile-threads]
```

Disable selected warning categories.

## `--enable-warning`

```text
[--enable-warning] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|parser|wasip1]
```

Enable warnings; host quota diagnostics default off. Select all_host_calls[_wasip1|_ros1|_ros2], all_host_denied/all_host_limited, or host_denied_FAMILY.API / host_limited_FAMILY.API.

## `--help`

```text
[--help|-h] ([all|global|debug|update|wasm|secure|log|runtime|wasi|ros1|ros2])
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

## `--management-profile`

```text
[--management-profile] <release|dev>
```

Trusted launch profile: release (default, diagnostics only) or explicit dev Guest debugging. Does not grant Guest imports.

## `--mode`

```text
[--mode|-m] [section-details|validation|run|debug]
```

Select the operation mode. Default: run.

## `--portable-robot`

```text
[--portable-robot] <adapter:absolute-path> <adapter:sha256> <policy:absolute-path> <policy:sha256>
```

Select exact portable adapter bytes and a separate explicit deployment grant.

## `--portable-robot-audit-inputs`

```text
[--portable-robot-audit-inputs] <adapter:absolute-path> <policy:absolute-path>
```

Observe exact bounded adapter/policy bytes for denied-call diagnostics; grants no permission and does not enable audit.

## `--portable-robot-audit-run`

```text
[--portable-robot-audit-run] <run-id:32-lowercase-hex>
```

Opt in to at most eight process-correlated portable operation records on stdout; disabled by default.

## `--ros1-check-policy`

```text
[--ros1-check-policy]
```

Validate explicit ROS policy; no Guest execution.

## `--ros1-dump-effective-policy`

```text
[--ros1-dump-effective-policy]
```

Validate explicit ROS policy; no Guest execution.

## `--ros1-global-add-module`

```text
[--ros1-global-add-module] <module:str>
```

Bind one exact loaded module to the explicit Global contract; unbound modules stay denied.

## `--ros1-global-advertise-host`

```text
[--ros1-global-advertise-host] <value:str>
```

Explicit ROS1 global advertise-host configuration; default deny.

## `--ros1-global-audit-deny-burst`

```text
[--ros1-global-audit-deny-burst] <value:str>
```

Explicit ROS1 global audit-deny-burst configuration; default deny.

## `--ros1-global-audit-deny-max-bytes`

```text
[--ros1-global-audit-deny-max-bytes] <value:str>
```

Explicit ROS1 global audit-deny-max-bytes configuration; default deny.

## `--ros1-global-audit-deny-max-rate-hz`

```text
[--ros1-global-audit-deny-max-rate-hz] <value:str>
```

Explicit ROS1 global audit-deny-max-rate-hz configuration; default deny.

## `--ros1-global-audit-deny-max-records`

```text
[--ros1-global-audit-deny-max-records] <value:str>
```

Explicit ROS1 global audit-deny-max-records configuration; default deny.

## `--ros1-global-connect-timeout-ms`

```text
[--ros1-global-connect-timeout-ms] <value:str>
```

Explicit ROS1 global connect-timeout-ms configuration; default deny.

## `--ros1-global-connect-timeout-ms.on-proc-exit`

```text
[--ros1-global-connect-timeout-ms.on-proc-exit] <cancel|wait>
```

ROS1 global connection wait on proc_exit: cancel (default) or wait.

## `--ros1-global-disable`

```text
[--ros1-global-disable]
```

Explicit ROS1 global disable configuration; default deny.

## `--ros1-global-enable`

```text
[--ros1-global-enable]
```

Enable the ROS1 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros1-global-gateway-socket`

```text
[--ros1-global-gateway-socket] <value:str>
```

Explicit ROS1 global gateway-socket configuration; default deny.

## `--ros1-global-gateway-uid`

```text
[--ros1-global-gateway-uid] <value:str>
```

Explicit ROS1 global gateway-uid configuration; default deny.

## `--ros1-global-grant`

```text
[--ros1-global-grant] <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS1 global grant configuration; default deny.

## `--ros1-global-interface`

```text
[--ros1-global-interface] <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros1-global-master-uri`

```text
[--ros1-global-master-uri] <value:str>
```

Explicit ROS1 global master-uri configuration; default deny.

## `--ros1-global-namespace`

```text
[--ros1-global-namespace] <value:str>
```

Explicit ROS1 global namespace configuration; default deny.

## `--ros1-global-node-name`

```text
[--ros1-global-node-name] <value:str>
```

Explicit ROS1 global node-name configuration; default deny.

## `--ros1-global-set`

```text
[--ros1-global-set] <capability:str> <key:str> <value:str>
```

Explicit ROS1 global set configuration; default deny.

## `--ros1-global-transport`

```text
[--ros1-global-transport] <value:str>
```

Explicit ROS1 global transport configuration; default deny.

## `--ros1-group-add-module`

```text
[--ros1-group-add-module] <group:str> <module:str>
```

Explicit ROS1 group add-module configuration; default deny.

## `--ros1-group-advertise-host`

```text
[--ros1-group-advertise-host] <group:str> <value:str>
```

Explicit ROS1 group advertise-host configuration; default deny.

## `--ros1-group-audit-deny-burst`

```text
[--ros1-group-audit-deny-burst] <group:str> <value:str>
```

Explicit ROS1 group audit-deny-burst configuration; default deny.

## `--ros1-group-audit-deny-max-bytes`

```text
[--ros1-group-audit-deny-max-bytes] <group:str> <value:str>
```

Explicit ROS1 group audit-deny-max-bytes configuration; default deny.

## `--ros1-group-audit-deny-max-rate-hz`

```text
[--ros1-group-audit-deny-max-rate-hz] <group:str> <value:str>
```

Explicit ROS1 group audit-deny-max-rate-hz configuration; default deny.

## `--ros1-group-audit-deny-max-records`

```text
[--ros1-group-audit-deny-max-records] <group:str> <value:str>
```

Explicit ROS1 group audit-deny-max-records configuration; default deny.

## `--ros1-group-connect-timeout-ms`

```text
[--ros1-group-connect-timeout-ms] <group:str> <value:str>
```

Explicit ROS1 group connect-timeout-ms configuration; default deny.

## `--ros1-group-connect-timeout-ms.on-proc-exit`

```text
[--ros1-group-connect-timeout-ms.on-proc-exit] <group:str> <cancel|wait>
```

ROS1 group connection wait on proc_exit: cancel (default) or wait.

## `--ros1-group-create`

```text
[--ros1-group-create] <group:str>
```

Explicit ROS1 group create configuration; default deny.

## `--ros1-group-disable`

```text
[--ros1-group-disable] <group:str>
```

Explicit ROS1 group disable configuration; default deny.

## `--ros1-group-enable`

```text
[--ros1-group-enable] <group:str>
```

Enable the ROS1 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros1-group-gateway-socket`

```text
[--ros1-group-gateway-socket] <group:str> <value:str>
```

Explicit ROS1 group gateway-socket configuration; default deny.

## `--ros1-group-gateway-uid`

```text
[--ros1-group-gateway-uid] <group:str> <value:str>
```

Explicit ROS1 group gateway-uid configuration; default deny.

## `--ros1-group-grant`

```text
[--ros1-group-grant] <group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS1 group grant configuration; default deny.

## `--ros1-group-interface`

```text
[--ros1-group-interface] <group:str> <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros1-group-master-uri`

```text
[--ros1-group-master-uri] <group:str> <value:str>
```

Explicit ROS1 group master-uri configuration; default deny.

## `--ros1-group-namespace`

```text
[--ros1-group-namespace] <group:str> <value:str>
```

Explicit ROS1 group namespace configuration; default deny.

## `--ros1-group-node-name`

```text
[--ros1-group-node-name] <group:str> <value:str>
```

Explicit ROS1 group node-name configuration; default deny.

## `--ros1-group-set`

```text
[--ros1-group-set] <group:str> <capability:str> <key:str> <value:str>
```

Explicit ROS1 group set configuration; default deny.

## `--ros1-group-transport`

```text
[--ros1-group-transport] <group:str> <value:str>
```

Explicit ROS1 group transport configuration; default deny.

## `--ros1-single-advertise-host`

```text
[--ros1-single-advertise-host] <module:str> <value:str>
```

Explicit ROS1 single advertise-host configuration; default deny.

## `--ros1-single-audit-deny-burst`

```text
[--ros1-single-audit-deny-burst] <module:str> <value:str>
```

Explicit ROS1 single audit-deny-burst configuration; default deny.

## `--ros1-single-audit-deny-max-bytes`

```text
[--ros1-single-audit-deny-max-bytes] <module:str> <value:str>
```

Explicit ROS1 single audit-deny-max-bytes configuration; default deny.

## `--ros1-single-audit-deny-max-rate-hz`

```text
[--ros1-single-audit-deny-max-rate-hz] <module:str> <value:str>
```

Explicit ROS1 single audit-deny-max-rate-hz configuration; default deny.

## `--ros1-single-audit-deny-max-records`

```text
[--ros1-single-audit-deny-max-records] <module:str> <value:str>
```

Explicit ROS1 single audit-deny-max-records configuration; default deny.

## `--ros1-single-call-boundary`

```text
[--ros1-single-call-boundary] <module:str> <boundary:str>
```

Select same-module or explicit module-exports ROS delegation for one Single target.

## `--ros1-single-connect-timeout-ms`

```text
[--ros1-single-connect-timeout-ms] <module:str> <value:str>
```

Explicit ROS1 single connect-timeout-ms configuration; default deny.

## `--ros1-single-connect-timeout-ms.on-proc-exit`

```text
[--ros1-single-connect-timeout-ms.on-proc-exit] <module:str> <cancel|wait>
```

ROS1 single connection wait on proc_exit: cancel (default) or wait.

## `--ros1-single-create`

```text
[--ros1-single-create] <module:str>
```

Explicit ROS1 single create configuration; default deny.

## `--ros1-single-disable`

```text
[--ros1-single-disable] <module:str>
```

Explicit ROS1 single disable configuration; default deny.

## `--ros1-single-enable`

```text
[--ros1-single-enable] <module:str>
```

Enable the ROS1 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros1-single-gateway-socket`

```text
[--ros1-single-gateway-socket] <module:str> <value:str>
```

Explicit ROS1 single gateway-socket configuration; default deny.

## `--ros1-single-gateway-uid`

```text
[--ros1-single-gateway-uid] <module:str> <value:str>
```

Explicit ROS1 single gateway-uid configuration; default deny.

## `--ros1-single-grant`

```text
[--ros1-single-grant] <module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS1 single grant configuration; default deny.

## `--ros1-single-interface`

```text
[--ros1-single-interface] <single:str> <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros1-single-master-uri`

```text
[--ros1-single-master-uri] <module:str> <value:str>
```

Explicit ROS1 single master-uri configuration; default deny.

## `--ros1-single-namespace`

```text
[--ros1-single-namespace] <module:str> <value:str>
```

Explicit ROS1 single namespace configuration; default deny.

## `--ros1-single-node-name`

```text
[--ros1-single-node-name] <module:str> <value:str>
```

Explicit ROS1 single node-name configuration; default deny.

## `--ros1-single-set`

```text
[--ros1-single-set] <module:str> <capability:str> <key:str> <value:str>
```

Explicit ROS1 single set configuration; default deny.

## `--ros1-single-transport`

```text
[--ros1-single-transport] <module:str> <value:str>
```

Explicit ROS1 single transport configuration; default deny.

## `--ros2-buffer-audit`

```text
[--ros2-buffer-audit] <run-id:ASCII> <inherited-fd:uint>
```

Opt-in bounded host-only buffer/checked-IPC journal; consumes an empty private write-only regular-file FD. Grants no authority.

## `--ros2-buffer-audit-limits`

```text
[--ros2-buffer-audit-limits] <payload-bytes:uint> <operation-records:uint> <total-file-bytes:uint> <record-bytes:uint> <profile:sha256>
```

Explicit process-global journal quotas/profile reference; must immediately precede --ros2-buffer-audit. Grants no authority.

## `--ros2-check-policy`

```text
[--ros2-check-policy]
```

Validate explicit ROS policy; no Guest execution.

## `--ros2-dump-effective-policy`

```text
[--ros2-dump-effective-policy]
```

Validate explicit ROS policy; no Guest execution.

## `--ros2-global-add-module`

```text
[--ros2-global-add-module] <module:str>
```

Bind one exact loaded module to the explicit Global contract; unbound modules stay denied.

## `--ros2-global-audit-deny-burst`

```text
[--ros2-global-audit-deny-burst] <value:str>
```

Explicit ROS2 global audit-deny-burst configuration; default deny.

## `--ros2-global-audit-deny-max-bytes`

```text
[--ros2-global-audit-deny-max-bytes] <value:str>
```

Explicit ROS2 global audit-deny-max-bytes configuration; default deny.

## `--ros2-global-audit-deny-max-rate-hz`

```text
[--ros2-global-audit-deny-max-rate-hz] <value:str>
```

Explicit ROS2 global audit-deny-max-rate-hz configuration; default deny.

## `--ros2-global-audit-deny-max-records`

```text
[--ros2-global-audit-deny-max-records] <value:str>
```

Explicit ROS2 global audit-deny-max-records configuration; default deny.

## `--ros2-global-connect-timeout-ms`

```text
[--ros2-global-connect-timeout-ms] <value:str>
```

Explicit ROS2 global connect-timeout-ms configuration; default deny.

## `--ros2-global-connect-timeout-ms.on-proc-exit`

```text
[--ros2-global-connect-timeout-ms.on-proc-exit] <cancel|wait>
```

ROS2 global connection wait on proc_exit: cancel (default) or wait.

## `--ros2-global-disable`

```text
[--ros2-global-disable]
```

Explicit ROS2 global disable configuration; default deny.

## `--ros2-global-domain-id`

```text
[--ros2-global-domain-id] <value:str>
```

Explicit ROS2 global domain-id configuration; default deny.

## `--ros2-global-enable`

```text
[--ros2-global-enable]
```

Enable the ROS2 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros2-global-enclave`

```text
[--ros2-global-enclave] <value:str>
```

Explicit ROS2 global enclave configuration; default deny.

## `--ros2-global-gateway-socket`

```text
[--ros2-global-gateway-socket] <value:str>
```

Explicit ROS2 global gateway-socket configuration; default deny.

## `--ros2-global-gateway-uid`

```text
[--ros2-global-gateway-uid] <value:str>
```

Explicit ROS2 global gateway-uid configuration; default deny.

## `--ros2-global-grant`

```text
[--ros2-global-grant] <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS2 global grant configuration; default deny.

## `--ros2-global-interface`

```text
[--ros2-global-interface] <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros2-global-keystore`

```text
[--ros2-global-keystore] <value:str>
```

Explicit ROS2 global keystore configuration; default deny.

## `--ros2-global-namespace`

```text
[--ros2-global-namespace] <value:str>
```

Explicit ROS2 global namespace configuration; default deny.

## `--ros2-global-node-name`

```text
[--ros2-global-node-name] <value:str>
```

Explicit ROS2 global node-name configuration; default deny.

## `--ros2-global-rmw-implementation`

```text
[--ros2-global-rmw-implementation] <value:str>
```

Explicit ROS2 global rmw-implementation configuration; default deny.

## `--ros2-global-security-mode`

```text
[--ros2-global-security-mode] <value:str>
```

Explicit ROS2 global security-mode configuration; default deny.

## `--ros2-global-set`

```text
[--ros2-global-set] <capability:str> <key:str> <value:str>
```

Explicit ROS2 global set configuration; default deny.

## `--ros2-group-add-module`

```text
[--ros2-group-add-module] <group:str> <module:str>
```

Explicit ROS2 group add-module configuration; default deny.

## `--ros2-group-audit-deny-burst`

```text
[--ros2-group-audit-deny-burst] <group:str> <value:str>
```

Explicit ROS2 group audit-deny-burst configuration; default deny.

## `--ros2-group-audit-deny-max-bytes`

```text
[--ros2-group-audit-deny-max-bytes] <group:str> <value:str>
```

Explicit ROS2 group audit-deny-max-bytes configuration; default deny.

## `--ros2-group-audit-deny-max-rate-hz`

```text
[--ros2-group-audit-deny-max-rate-hz] <group:str> <value:str>
```

Explicit ROS2 group audit-deny-max-rate-hz configuration; default deny.

## `--ros2-group-audit-deny-max-records`

```text
[--ros2-group-audit-deny-max-records] <group:str> <value:str>
```

Explicit ROS2 group audit-deny-max-records configuration; default deny.

## `--ros2-group-connect-timeout-ms`

```text
[--ros2-group-connect-timeout-ms] <group:str> <value:str>
```

Explicit ROS2 group connect-timeout-ms configuration; default deny.

## `--ros2-group-connect-timeout-ms.on-proc-exit`

```text
[--ros2-group-connect-timeout-ms.on-proc-exit] <group:str> <cancel|wait>
```

ROS2 group connection wait on proc_exit: cancel (default) or wait.

## `--ros2-group-create`

```text
[--ros2-group-create] <group:str>
```

Explicit ROS2 group create configuration; default deny.

## `--ros2-group-disable`

```text
[--ros2-group-disable] <group:str>
```

Explicit ROS2 group disable configuration; default deny.

## `--ros2-group-domain-id`

```text
[--ros2-group-domain-id] <group:str> <value:str>
```

Explicit ROS2 group domain-id configuration; default deny.

## `--ros2-group-enable`

```text
[--ros2-group-enable] <group:str>
```

Enable the ROS2 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros2-group-enclave`

```text
[--ros2-group-enclave] <group:str> <value:str>
```

Explicit ROS2 group enclave configuration; default deny.

## `--ros2-group-gateway-socket`

```text
[--ros2-group-gateway-socket] <group:str> <value:str>
```

Explicit ROS2 group gateway-socket configuration; default deny.

## `--ros2-group-gateway-uid`

```text
[--ros2-group-gateway-uid] <group:str> <value:str>
```

Explicit ROS2 group gateway-uid configuration; default deny.

## `--ros2-group-grant`

```text
[--ros2-group-grant] <group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS2 group grant configuration; default deny.

## `--ros2-group-interface`

```text
[--ros2-group-interface] <group:str> <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros2-group-keystore`

```text
[--ros2-group-keystore] <group:str> <value:str>
```

Explicit ROS2 group keystore configuration; default deny.

## `--ros2-group-namespace`

```text
[--ros2-group-namespace] <group:str> <value:str>
```

Explicit ROS2 group namespace configuration; default deny.

## `--ros2-group-node-name`

```text
[--ros2-group-node-name] <group:str> <value:str>
```

Explicit ROS2 group node-name configuration; default deny.

## `--ros2-group-rmw-implementation`

```text
[--ros2-group-rmw-implementation] <group:str> <value:str>
```

Explicit ROS2 group rmw-implementation configuration; default deny.

## `--ros2-group-security-mode`

```text
[--ros2-group-security-mode] <group:str> <value:str>
```

Explicit ROS2 group security-mode configuration; default deny.

## `--ros2-group-set`

```text
[--ros2-group-set] <group:str> <capability:str> <key:str> <value:str>
```

Explicit ROS2 group set configuration; default deny.

## `--ros2-single-audit-deny-burst`

```text
[--ros2-single-audit-deny-burst] <module:str> <value:str>
```

Explicit ROS2 single audit-deny-burst configuration; default deny.

## `--ros2-single-audit-deny-max-bytes`

```text
[--ros2-single-audit-deny-max-bytes] <module:str> <value:str>
```

Explicit ROS2 single audit-deny-max-bytes configuration; default deny.

## `--ros2-single-audit-deny-max-rate-hz`

```text
[--ros2-single-audit-deny-max-rate-hz] <module:str> <value:str>
```

Explicit ROS2 single audit-deny-max-rate-hz configuration; default deny.

## `--ros2-single-audit-deny-max-records`

```text
[--ros2-single-audit-deny-max-records] <module:str> <value:str>
```

Explicit ROS2 single audit-deny-max-records configuration; default deny.

## `--ros2-single-call-boundary`

```text
[--ros2-single-call-boundary] <module:str> <boundary:str>
```

Select same-module or explicit module-exports ROS delegation for one Single target.

## `--ros2-single-connect-timeout-ms`

```text
[--ros2-single-connect-timeout-ms] <module:str> <value:str>
```

Explicit ROS2 single connect-timeout-ms configuration; default deny.

## `--ros2-single-connect-timeout-ms.on-proc-exit`

```text
[--ros2-single-connect-timeout-ms.on-proc-exit] <module:str> <cancel|wait>
```

ROS2 single connection wait on proc_exit: cancel (default) or wait.

## `--ros2-single-create`

```text
[--ros2-single-create] <module:str>
```

Explicit ROS2 single create configuration; default deny.

## `--ros2-single-disable`

```text
[--ros2-single-disable] <module:str>
```

Explicit ROS2 single disable configuration; default deny.

## `--ros2-single-domain-id`

```text
[--ros2-single-domain-id] <module:str> <value:str>
```

Explicit ROS2 single domain-id configuration; default deny.

## `--ros2-single-enable`

```text
[--ros2-single-enable] <module:str>
```

Enable the ROS2 namespace; exact interfaces remain hidden until explicitly configured.

## `--ros2-single-enclave`

```text
[--ros2-single-enclave] <module:str> <value:str>
```

Explicit ROS2 single enclave configuration; default deny.

## `--ros2-single-gateway-socket`

```text
[--ros2-single-gateway-socket] <module:str> <value:str>
```

Explicit ROS2 single gateway-socket configuration; default deny.

## `--ros2-single-gateway-uid`

```text
[--ros2-single-gateway-uid] <module:str> <value:str>
```

Explicit ROS2 single gateway-uid configuration; default deny.

## `--ros2-single-grant`

```text
[--ros2-single-grant] <module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>
```

Explicit ROS2 single grant configuration; default deny.

## `--ros2-single-interface`

```text
[--ros2-single-interface] <single:str> <namespace.function:str> <hidden|deny|allow>
```

Configure exact namespace.function: hidden fails linking; deny exposes but rejects; allow retains capability checks. Default hidden.

## `--ros2-single-keystore`

```text
[--ros2-single-keystore] <module:str> <value:str>
```

Explicit ROS2 single keystore configuration; default deny.

## `--ros2-single-namespace`

```text
[--ros2-single-namespace] <module:str> <value:str>
```

Explicit ROS2 single namespace configuration; default deny.

## `--ros2-single-node-name`

```text
[--ros2-single-node-name] <module:str> <value:str>
```

Explicit ROS2 single node-name configuration; default deny.

## `--ros2-single-rmw-implementation`

```text
[--ros2-single-rmw-implementation] <module:str> <value:str>
```

Explicit ROS2 single rmw-implementation configuration; default deny.

## `--ros2-single-security-mode`

```text
[--ros2-single-security-mode] <module:str> <value:str>
```

Explicit ROS2 single security-mode configuration; default deny.

## `--ros2-single-set`

```text
[--ros2-single-set] <module:str> <capability:str> <key:str> <value:str>
```

Explicit ROS2 single set configuration; default deny.

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

Shortcut for -m debug; enter the built-in LLVM JIT full debugger.

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
[--warning-as-error] [all_host_calls[_FAMILY]|all_host_denied[_FAMILY]|all_host_limited[_FAMILY]|host_call_FAMILY.API|host_denied_FAMILY.API|host_limited_FAMILY.API|all|vm|parser|untrusted-dl|dl|depend|wasip1|runtime|runtime-compile-threads]
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

Select finite WASI Threads quotas; the stack value bounds each Guest AND compiler mapping, including guards.

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

## `--wasip1-global-socket-rights`

```text
[--wasip1-global-socket-rights] <initial-fd:i32> <base:csv|none> <accepted-child:csv|none>
```

Grant only this preopened socket or inherited stdio socket: read,write,accept,shutdown,readiness,flags,metadata. Default none; grants no interfaces.

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

## `--wasip1-group-trace`

```text
[--wasip1-group-trace] <group:str> <none|out|err|file <file:path>>
```

Route one named group's WASI Preview 1 trace output.

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

## `--wasip1-single-trace`

```text
[--wasip1-single-trace] <module:str> <none|out|err|file <file:path>>
```

Route one single module's WASI Preview 1 trace output.

## `--wasip1-socket-abi`

```text
[--wasip1-socket-abi] <standard|wasix>
```

Choose the immutable wasm32 sock_accept ABI for this process: standard (default, 3 arguments) or wasix (4 arguments, peer address). Grants no interfaces or socket rights.

## `--wasm-depend-recursion-limit`

```text
[--wasm-depend-recursion-limit|-Wdeplim] <depth:size_t>
```

Set the dependency-check recursion limit (0 = unlimited).

## `--wasm-driver-verify`

```text
[--wasm-driver-verify] <trusted-signer.pem>
```

Opt in to secondary native-driver signature verification with a host-pinned PEM signer certificate. Accept Linux sign-file appended CMS signatures; verify sealed payloads before dlopen. Default is off. External deployment retains full PKI and dependency policy.

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

Enable the WebAssembly 1.1 feature set.

## `--wasm-feature-wasm2`

```text
[--wasm-feature-wasm2|-WF2|-WFwasm2]
```

Select the complete WebAssembly 2.0 feature set.

## `--wasm-feature-wasmmvp`

```text
[--wasm-feature-wasmmvp|--wasm-feature-mvp|-WFmvp|-WFwasmmvp|-WFwasm1|-WF1]
```

Disable WebAssembly 1.1 features and enforce the WebAssembly MVP feature set.

## `--wasm-memory-grow-strict`

```text
[--wasm-memory-grow-strict|-Wmemstrict]
```

Enable strict `memory.grow` semantics.

## `--wasm-preload-library`

```text
[--wasm-preload-library|-Wpre] <wasm:path> (<rename:str>)
```

Preload a Wasm module as a dynamic library; an optional rename sets its module name.

## `--wasm-register-dl`

```text
[--wasm-register-dl|-Wdl] <dl:path> (<rename:str>)
```

Register a native dynamic library as a Wasm module; an optional rename sets its module name.

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

## `--wasm-set-start-func`

```text
[--wasm-set-start-func|-Wstart] <local-func:u32> [[i32|i64|f32|f64] ...]
```

Run a main-module local defined Wasm function as the entry function.
