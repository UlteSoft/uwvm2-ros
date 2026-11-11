# ROS 1 command line

Build this category with `xmake f --import-ros1=y --use-cxx-module=n`. The Linux runtime host uses the full interpreter (`-Rint`) and does not link ROS libraries. The independent Gateway links roscpp from Noetic.

No permissions are implied. Enabled Targets with resource grants require their explicit connection identities, and grants require their resource/type/behavior fields. Quotas are optional and default to unlimited. Duplicate assignments, unknown fields, unresolved module bindings and unsupported active operations reject startup.

ROS1 is disabled by default, even in a ROS1-enabled build. Its initial Global Target and each newly created Single/Group Target are disabled with empty connection and capability maps. The Global, Single and Group `enable` options only enable the selected Target; they grant no operation. Their `grant` and `set` options configure explicit resource permission without enabling that Target. An enabled Target with no grants still denies every Guest resource operation, and a disabled Target cannot use its stored grants. ROS2 configuration does not enable ROS1 or grant ROS1 authority.

Interface visibility starts empty. `--ros1-single-enable <module>` exposes
this ROS version's complete registered namespace family to that explicitly
created module target. The Group and Global enable options do the same for
their explicitly bound modules. There is no per-function exposure setting.
A module without an explicit Single/Group binding selects Global. Global
starts disabled with hidden interfaces. `--ros1-global-add-module <module>`
is an optional membership declaration; it is not required for default selection.

Enable alone grants no resource, opens no Gateway connection, and needs no
unused network identity. Static validation resolves the enabled namespace and
actual function signature without connecting ROS providers or executing Guest start. Explicit host configuration such as a separately requested WASI directory preopen may be processed earlier by the command line. Once resource grants are
configured, the complete connection/type/security contract remains mandatory;
execution attests it through the real Gateway before Guest start. The cached
native import wrapper checks the actual caller's namespace binding on every
call, and operation/handle checks remain independent.

## Optional quotas and synchronous waits

Enabling a target does not grant a resource. Once its
resource capability is granted, an omitted quota (or unlimited) adds no
quota. A finite zero is a real zero allowance. Resource names, type identities,
validators, QoS choices and other authority fields remain explicit. KEEP_ALL
has no local depth quota; KEEP_LAST requires an explicit positive finite depth.

Use the existing global-set, single-set or group-set command for each
capability field. For example:

    --ros1-single-set controller telemetry max-bytes-per-second 1048576
    --ros1-single-set controller telemetry max-bytes-per-second.on-exhausted wait
    --ros1-single-set controller telemetry max-bytes-per-second.on-proc-exit wait

An exhausted replenishing quota defaults to reject; wait blocks the current
native call until credit or capacity becomes available. Linux waits use actual
timerfd/eventfd/readiness notifications. They do not return an asynchronous
quota handle. No quota failure triggers an automatic resend or a broader grant.
A reply waiting for delivery credit remains the same reply.

Per-request size and lifetime/history ceilings cannot replenish for the same
operation. Their on-exhausted=wait setting is rejected. A request larger than
a full byte bucket is rejected even when ordinary bucket exhaustion waits.
Rate and burst must be finite together, or both omitted/unlimited; burst
sets token capacity and the rate field selects the replenishment wait action.
Capacity waits that require a later request on the same blocked serial IPC
connection are unsupported and must be rejected by deployment validation.

A field's on-proc-exit selects cancel (default) or wait for a quota wait.
The timeout-ms.on-proc-exit field controls the native operation wait; an explicit
native-wait.on-proc-exit overrides it. The first authorized proc_exit from
any module/thread requests process-wide exit. Existing selected wait owners
finish their actual operation before exit; no new work can acquire that
privilege after exit is claimed. Other Wasm modules cannot reset, refund or
remove the user contract. Trusted revocation and physical worker/connection
loss remain independent cancellation causes.

Local queue overflow accepts only reject or wait; old drop-oldest,
drop-newest and backpressure configurations fail validation. Native DDS
KEEP_LAST history remains its separately chosen QoS behavior. Quota failures
return the existing ABI error (for example EAGAIN, EMSGSIZE, ETIMEDOUT or
ECANCELED), and retained audit counters identify rejected diagnostics. Guest
code can mishandle an error and trap; the managed guest-fault boundary stops
that guest while keeping the authorized management channel available.

## Registered parameters

These parameters use uwvm2's existing parameter registry, hash lookup and argument occupancy mechanism. They are also listed by `uwvm2-ros --help ros1`.

| Parameter | Arguments |
| --- | --- |
| `--ros1-global-add-module` | `<module:str>` |
| `--ros1-global-enable` | `` |
| `--ros1-global-disable` | `` |
| `--ros1-global-master-uri` | `<value:str>` |
| `--ros1-global-gateway-socket` | `<value:str>` |
| `--ros1-global-gateway-uid` | `<value:str>` |
| `--ros1-global-audit-deny-max-bytes` | `<value:str>` |
| `--ros1-global-audit-deny-max-records` | `<value:str>` |
| `--ros1-global-audit-deny-burst` | `<value:str>` |
| `--ros1-global-audit-deny-max-rate-hz` | `<value:str>` |
| `--ros1-global-node-name` | `<value:str>` |
| `--ros1-global-namespace` | `<value:str>` |
| `--ros1-global-advertise-host` | `<value:str>` |
| `--ros1-global-transport` | `<value:str>` |
| `--ros1-global-connect-timeout-ms` | `<value:str>` |
| `--ros1-global-connect-timeout-ms.on-proc-exit` | `<cancel\|wait>` |
| `--ros1-global-grant` | `<capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros1-global-set` | `<capability:str> <key:str> <value:str>` |
| `--ros1-single-create` | `<module:str>` |
| `--ros1-single-enable` | `<module:str>` |
| `--ros1-single-call-boundary` | `<module:str> <same-module\|module-exports>` |
| `--ros1-single-disable` | `<module:str>` |
| `--ros1-single-master-uri` | `<module:str> <value:str>` |
| `--ros1-single-gateway-socket` | `<module:str> <value:str>` |
| `--ros1-single-gateway-uid` | `<module:str> <value:str>` |
| `--ros1-single-audit-deny-max-bytes` | `<module:str> <value:str>` |
| `--ros1-single-audit-deny-max-records` | `<module:str> <value:str>` |
| `--ros1-single-audit-deny-burst` | `<module:str> <value:str>` |
| `--ros1-single-audit-deny-max-rate-hz` | `<module:str> <value:str>` |
| `--ros1-single-node-name` | `<module:str> <value:str>` |
| `--ros1-single-namespace` | `<module:str> <value:str>` |
| `--ros1-single-advertise-host` | `<module:str> <value:str>` |
| `--ros1-single-transport` | `<module:str> <value:str>` |
| `--ros1-single-connect-timeout-ms` | `<module:str> <value:str>` |
| `--ros1-single-connect-timeout-ms.on-proc-exit` | `<module:str> <cancel\|wait>` |
| `--ros1-single-grant` | `<module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros1-single-set` | `<module:str> <capability:str> <key:str> <value:str>` |
| `--ros1-group-create` | `<group:str>` |
| `--ros1-group-add-module` | `<group:str> <module:str>` |
| `--ros1-group-enable` | `<group:str>` |
| `--ros1-group-disable` | `<group:str>` |
| `--ros1-group-master-uri` | `<group:str> <value:str>` |
| `--ros1-group-gateway-socket` | `<group:str> <value:str>` |
| `--ros1-group-gateway-uid` | `<group:str> <value:str>` |
| `--ros1-group-audit-deny-max-bytes` | `<group:str> <value:str>` |
| `--ros1-group-audit-deny-max-records` | `<group:str> <value:str>` |
| `--ros1-group-audit-deny-burst` | `<group:str> <value:str>` |
| `--ros1-group-audit-deny-max-rate-hz` | `<group:str> <value:str>` |
| `--ros1-group-node-name` | `<group:str> <value:str>` |
| `--ros1-group-namespace` | `<group:str> <value:str>` |
| `--ros1-group-advertise-host` | `<group:str> <value:str>` |
| `--ros1-group-transport` | `<group:str> <value:str>` |
| `--ros1-group-connect-timeout-ms` | `<group:str> <value:str>` |
| `--ros1-group-connect-timeout-ms.on-proc-exit` | `<group:str> <cancel\|wait>` |
| `--ros1-group-grant` | `<group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros1-group-set` | `<group:str> <capability:str> <key:str> <value:str>` |
| `--ros1-check-policy` | `` |
| `--ros1-dump-effective-policy` | `` |

## Target selection

`single-create` binds that exact loaded Wasm module name to its Target. `group-create` creates policy storage and `group-add-module` binds an existing loaded module by name. A module may have one explicit Target per ROS version. Duplicate membership and Single/Group conflicts fail. Names are matched against the actual loaded main and preload Wasm artifacts; `--wasm-set-main-module-name` and `--wasm-preload-library` control those names.

An explicit Single/Group Target receives no Global capabilities, connection
fields or enabled state, even when the selected Target is disabled or empty.
All other modules select the default Global Target. Each Group and Global owns
a shared connection/session, capability table and quota ledger; a permitted
member may use another member's handle. Different Single/Group/Global policies
have independent environments. ROS1, ROS2 and WASI policy namespaces remain
independent. Duplicate Single declarations or Group members, and assignment to
more than one explicit Single/Group, are rejected before Guest execution.

`check-policy` and `dump-effective-policy` complete after all configuration options, regardless of where the diagnostic switch appeared. With `--run file.wasm`, the file is parsed for actual module-name binding but Guest code does not execute. Without `--run`, only already loaded preload artifacts can satisfy module bindings. Dump produces one sorted JSON record per requested ROS version on stdout.

## Explicit library export delegation

ROS calls with cross-module ancestry are denied by default (`same-module`).
A trusted deployment can opt one explicitly named Single module into
`--ros1-single-call-boundary ros_comm module-exports`. Calls performed inside
that module's reviewed exports then use its own ROS capabilities. Calling
modules receive no capabilities or transferable handles. Direct imports from
an ungranted algorithm module still fail, and module generation, revocation and
permanent retirement checks remain active.

This policy delegates the exported operations to callers; it is not a caller
allowlist. Validate the library's export arguments and retained ownership.
Global and Group policies cannot select this option. The setting is worker-local
and does not change the native gateway capability manifest or network identity.
See the [SDK deployment guide](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/native-run.en_US.md) for the
explicit io/algorithm/ros_comm Preload configuration and current acceptance.

## Explicit connection fields

`gateway-socket` is a canonical absolute AF_UNIX path, at most 107 bytes. `gateway-uid` is the expected Linux peer UID, 0 through 4294967294; SO_PEERCRED verifies the connected endpoint. `node-name` is a ROS basename, `namespace` a canonical absolute ROS namespace, and `connect-timeout-ms` is an integer 1 through 60000. Gateway configuration additionally specifies the authorized worker UID and exact PID. A Gateway belongs to one explicitly configured backend identity. The socket path, expected UID and connection timeout are worker-local; the ROS network identity and complete resource grants must match its authoritative policy manifest.

`master-uri` requires `http://host:port`. `advertise-host` is explicit; the Gateway passes the configured Master URI and advertisement address to ROS initialization. `transport` requires `tcpros`. No ROS environment setting is imported from Guest configuration.

## Capability arguments and limits

`grant <alias> <operation> <resource> <type> <type-identity>` declares exactly one operation on exactly one resource. Resource names must already be canonical absolute names: relative names, private names, empty segments and remapping syntax are rejected. Aliases are ASCII letters, digits, underscore, dot and hyphen, with a maximum of 255 bytes. No wildcard, role, level or preset expands authority.

`set <alias> <key> <value>` supplies one required limit or semantic field. A key may be assigned only once. Trusted ROS1 MD5 and ROS2 RIHS01 hashes come from the generated adapters; Guest declarations cannot establish a trusted type identity.

## Worker isolation

Enabled ROS execution requires a supported interpreter or qualified LLVM backend and retains the independent admission rules of explicit native drivers and WASIp1 socket resources. The ROS product starts with no WASIp1 environment, descriptors, preopens, or host functions. Explicit WASIp1 directory preopens retain the owning module's roots, base/inheriting rights and budgets when ROS is enabled. A ROS namespace or resource grant supplies no filesystem authority, and modules cannot borrow each other's preopens. Proc/sys filesystems remain inaccessible, including through an approved ancestor directory. Host environment inheritance remains rejected when ROS is enabled. Explicit directory/socket preopens and inherited socket streams remain available under their WASIp1 interface, operation, per-descriptor rights and manually selected quota rules; a ROS grant supplies none of these authorities. The [SDK deployment guide](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/native-run.en_US.md) describes process and network trust boundaries.

`audit-deny-max-rate-hz`, `audit-deny-burst`, `audit-deny-max-records` and `audit-deny-max-bytes` explicitly bound the worker audit sink for unknown aliases, forged handles and refused proxy calls. Every enabled Target requires all four fields. They are worker-local and do not affect ROS network identity. The rate must be finite and positive (at most 1000000), burst/records are positive integers (at most 1048576), and bytes are at most 67108864. No unauthenticated request can choose its own audit budget.


### Policy-scope resource accounting

The default global policy, each module-specific `single`, and each named
`group` have independent accounting. Members of one named group share that
group's environment and quota; different named groups do not spend each other's
budgets. Identical limit values do not merge independent policies. Group
membership never grants another module's native handle or memory. ROS1 and
ROS2 also retain distinct accounting. Omitted ceilings remain unlimited;
selecting an interface or granting a resource does not install a rate limit.
Code generation replacement and checkpoint-world cloning preserve an existing
policy's consumed budget. Closing/retiring resources releases active capacity,
but does not replenish spent operation/creation credits.
