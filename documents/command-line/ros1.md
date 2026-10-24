# ROS 1 command line

Build this category with `xmake f --import-ros1=y --use-cxx-module=n`. The Linux runtime host uses the full interpreter (`-Rint`) and does not link ROS libraries. The independent Gateway links roscpp from Noetic.

No permissions are implied. All enabled Targets require every connection field. Every grant requires every field in its operation schema. Duplicate assignments, unknown fields, unresolved module bindings and unsupported active operations reject startup.

ROS1 is disabled by default, even in a ROS1-enabled build. Its initial Global Target and each newly created Single/Group Target are disabled with empty connection and capability maps. The Global, Single and Group `enable` options only enable the selected Target; they grant no operation. Their `grant` and `set` options configure explicit resource permission without enabling that Target. An enabled Target with no grants still denies every Guest resource operation, and a disabled Target cannot use its stored grants. ROS2 configuration does not enable ROS1 or grant ROS1 authority.

## Registered parameters

These parameters use uwvm2's existing parameter registry, hash lookup and argument occupancy mechanism. They are also listed by `uwvm2-ros --help ros1`.

| Parameter | Arguments |
| --- | --- |
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
| `--ros1-group-grant` | `<group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros1-group-set` | `<group:str> <capability:str> <key:str> <value:str>` |
| `--ros1-check-policy` | `` |
| `--ros1-dump-effective-policy` | `` |

## Target selection

`single-create` binds that exact loaded Wasm module name to its Target. `group-create` creates policy storage and `group-add-module` binds an existing loaded module by name. A module may have one explicit Target per ROS version. Duplicate membership and Single/Group conflicts fail. Names are matched against the actual loaded main and preload Wasm artifacts; `--wasm-set-main-module-name` and `--wasm-preload-library` control those names.

An explicitly bound Target receives no Global capabilities, connection fields or enabled state. It remains the selected Target even when disabled or empty, with no fallback to Global. An unbound module uses that version's Global Target, which is disabled with an empty capability set initially; explicit Global enable/grant configuration applies to all such unbound modules. Group members copy policy into independent Instance capability tables and open independent Gateway sessions. ROS1, ROS2 and WASI group namespaces are independent.

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

Enabled ROS execution requires the interpreter and rejects additional native DL/weak-symbol plugins and socket standard streams. Every loaded Wasm module whose WASIp1 support is enabled requires explicit host environment inheritance disablement and no directory or socket preopens, including modules with no ROS grants. WASIp1 normally inherits the host environment: use `--wasip1-global-noinherit-system-environment` and ensure applicable WASIp1 Single/Group overrides also disable inheritance. ROS enable/grant options do not alter WASIp1 permissions, and extra WASIp1 host connections reject ROS startup. The [SDK deployment guide](https://github.com/UlteSoft/uwvm2-ros-sdk/blob/main/document/native-run.en_US.md) describes process and network trust boundaries.

`audit-deny-max-rate-hz`, `audit-deny-burst`, `audit-deny-max-records` and `audit-deny-max-bytes` explicitly bound the worker audit sink for unknown aliases, forged handles and refused proxy calls. Every enabled Target requires all four fields. They are worker-local and do not affect ROS network identity. The rate must be finite and positive (at most 1000000), burst/records are positive integers (at most 1048576), and bytes are at most 67108864. No unauthenticated request can choose its own audit budget.
