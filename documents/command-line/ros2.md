# ROS 2 command line

Build this category with `xmake f --import-ros2=y --use-cxx-module=n`. The Linux runtime host uses the full interpreter (`-Rint`) and does not link ROS libraries. The independent Gateway links rclcpp from Jazzy.

No permissions are implied. All enabled Targets require every connection field. Every grant requires every field in its operation schema. Duplicate assignments, unknown fields, unresolved module bindings and unsupported active operations reject startup.

ROS2 is disabled by default, even in a ROS2-enabled build. Its initial Global Target and each newly created Single/Group Target are disabled with empty connection and capability maps. The Global, Single and Group `enable` options only enable the selected Target; they grant no operation. Their `grant` and `set` options configure explicit resource permission without enabling that Target. An enabled Target with no grants still denies every Guest resource operation, and a disabled Target cannot use its stored grants. ROS1 configuration does not enable ROS2 or grant ROS2 authority.

## Registered parameters

These parameters use uwvm2's existing parameter registry, hash lookup and argument occupancy mechanism. They are also listed by `uwvm --help ros2`.

| Parameter | Arguments |
| --- | --- |
| `--ros2-global-enable` | `` |
| `--ros2-global-disable` | `` |
| `--ros2-global-gateway-socket` | `<value:str>` |
| `--ros2-global-gateway-uid` | `<value:str>` |
| `--ros2-global-audit-deny-max-bytes` | `<value:str>` |
| `--ros2-global-audit-deny-max-records` | `<value:str>` |
| `--ros2-global-audit-deny-burst` | `<value:str>` |
| `--ros2-global-audit-deny-max-rate-hz` | `<value:str>` |
| `--ros2-global-node-name` | `<value:str>` |
| `--ros2-global-namespace` | `<value:str>` |
| `--ros2-global-domain-id` | `<value:str>` |
| `--ros2-global-rmw-implementation` | `<value:str>` |
| `--ros2-global-security-mode` | `<value:str>` |
| `--ros2-global-keystore` | `<value:str>` |
| `--ros2-global-enclave` | `<value:str>` |
| `--ros2-global-connect-timeout-ms` | `<value:str>` |
| `--ros2-global-grant` | `<capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros2-global-set` | `<capability:str> <key:str> <value:str>` |
| `--ros2-single-create` | `<module:str>` |
| `--ros2-single-enable` | `<module:str>` |
| `--ros2-single-disable` | `<module:str>` |
| `--ros2-single-gateway-socket` | `<module:str> <value:str>` |
| `--ros2-single-gateway-uid` | `<module:str> <value:str>` |
| `--ros2-single-audit-deny-max-bytes` | `<module:str> <value:str>` |
| `--ros2-single-audit-deny-max-records` | `<module:str> <value:str>` |
| `--ros2-single-audit-deny-burst` | `<module:str> <value:str>` |
| `--ros2-single-audit-deny-max-rate-hz` | `<module:str> <value:str>` |
| `--ros2-single-node-name` | `<module:str> <value:str>` |
| `--ros2-single-namespace` | `<module:str> <value:str>` |
| `--ros2-single-domain-id` | `<module:str> <value:str>` |
| `--ros2-single-rmw-implementation` | `<module:str> <value:str>` |
| `--ros2-single-security-mode` | `<module:str> <value:str>` |
| `--ros2-single-keystore` | `<module:str> <value:str>` |
| `--ros2-single-enclave` | `<module:str> <value:str>` |
| `--ros2-single-connect-timeout-ms` | `<module:str> <value:str>` |
| `--ros2-single-grant` | `<module:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros2-single-set` | `<module:str> <capability:str> <key:str> <value:str>` |
| `--ros2-group-create` | `<group:str>` |
| `--ros2-group-add-module` | `<group:str> <module:str>` |
| `--ros2-group-enable` | `<group:str>` |
| `--ros2-group-disable` | `<group:str>` |
| `--ros2-group-gateway-socket` | `<group:str> <value:str>` |
| `--ros2-group-gateway-uid` | `<group:str> <value:str>` |
| `--ros2-group-audit-deny-max-bytes` | `<group:str> <value:str>` |
| `--ros2-group-audit-deny-max-records` | `<group:str> <value:str>` |
| `--ros2-group-audit-deny-burst` | `<group:str> <value:str>` |
| `--ros2-group-audit-deny-max-rate-hz` | `<group:str> <value:str>` |
| `--ros2-group-node-name` | `<group:str> <value:str>` |
| `--ros2-group-namespace` | `<group:str> <value:str>` |
| `--ros2-group-domain-id` | `<group:str> <value:str>` |
| `--ros2-group-rmw-implementation` | `<group:str> <value:str>` |
| `--ros2-group-security-mode` | `<group:str> <value:str>` |
| `--ros2-group-keystore` | `<group:str> <value:str>` |
| `--ros2-group-enclave` | `<group:str> <value:str>` |
| `--ros2-group-connect-timeout-ms` | `<group:str> <value:str>` |
| `--ros2-group-grant` | `<group:str> <capability:str> <operation:str> <resource:str> <type:str> <type-identity:str>` |
| `--ros2-group-set` | `<group:str> <capability:str> <key:str> <value:str>` |
| `--ros2-check-policy` | `` |
| `--ros2-dump-effective-policy` | `` |

## Target selection

`single-create` binds that exact loaded Wasm module name to its Target. `group-create` creates policy storage and `group-add-module` binds an existing loaded module by name. A module may have one explicit Target per ROS version. Duplicate membership and Single/Group conflicts fail. Names are matched against the actual loaded main and preload Wasm artifacts; `--wasm-set-main-module-name` and `--wasm-preload-library` control those names.

An explicitly bound Target receives no Global capabilities, connection fields or enabled state. It remains the selected Target even when disabled or empty, with no fallback to Global. An unbound module uses that version's Global Target, which is disabled with an empty capability set initially; explicit Global enable/grant configuration applies to all such unbound modules. Group members copy policy into independent Instance capability tables and open independent Gateway sessions. ROS1, ROS2 and WASI group namespaces are independent.

`check-policy` and `dump-effective-policy` complete after all configuration options, regardless of where the diagnostic switch appeared. With `--run file.wasm`, the file is parsed for actual module-name binding but Guest code does not execute. Without `--run`, only already loaded preload artifacts can satisfy module bindings. Dump produces one sorted JSON record per requested ROS version on stdout.

## Explicit connection fields

`gateway-socket` is a canonical absolute AF_UNIX path, at most 107 bytes. `gateway-uid` is the expected Linux peer UID, 0 through 4294967294; SO_PEERCRED verifies the connected endpoint. `node-name` is a ROS basename, `namespace` a canonical absolute ROS namespace, and `connect-timeout-ms` is an integer 1 through 60000. Gateway configuration additionally specifies the authorized worker UID and exact PID. A Gateway belongs to one explicitly configured backend identity. The socket path, expected UID and connection timeout are worker-local; the ROS network identity and complete resource grants must match its authoritative policy manifest.

`domain-id` is an integer 0 through 232. `rmw-implementation` must identify a supported installed RMW. `security-mode` is `enforce` or `disabled-test`. In `enforce`, absolute keystore and enclave paths are mandatory and the Gateway validates security material and requires enforced DDS security. In `disabled-test`, both `keystore` and `enclave` must explicitly be `not-applicable`. This test configuration supplies no DDS network security isolation.

## Capability arguments and limits

`grant <alias> <operation> <resource> <type> <type-identity>` declares exactly one operation on exactly one resource. Resource names must already be canonical absolute names: relative names, private names, empty segments and remapping syntax are rejected. Aliases are ASCII letters, digits, underscore, dot and hyphen, with a maximum of 255 bytes. No wildcard, role, level or preset expands authority.

`set <alias> <key> <value>` supplies one required limit or semantic field. A key may be assigned only once. All fields, accepted values, hard protocol ceilings and unsupported operation status are listed in [capabilities.md](../ros/capabilities.md). Trusted ROS1 MD5 and ROS2 RIHS01 hashes come from the generated adapters; Guest declarations cannot establish a trusted type identity.

`service.call`, `service.advertise`, `service.respond`, `param.get`, `param.set`, `param.describe`, `param.list`, `lifecycle.get-state` and `lifecycle.change-state` each require eight explicit fields: `qos.reliability`, `qos.durability`, `qos.history`, `qos.depth`, `qos.deadline`, `qos.lifespan`, `qos.liveliness` and `qos.liveliness-lease-duration`. Missing fields reject configuration. Gateway-owned client and server request/response effective QoS must match the configured profile. Advertise/respond grants for the same service resource and generated type must declare equal profiles; respond never implicitly grants advertise. Local get/set/describe/list still require the fields but create no DDS client when `parameter-node` names the Gateway itself. The local-only `param.declare` and `param.undeclare` operations create no DDS endpoint and reject these fields as inapplicable. Parameter-event QoS remains independent.

## Worker isolation

Enabled ROS execution requires the interpreter and rejects additional native DL/weak-symbol plugins and socket standard streams. Every loaded Wasm module whose WASIp1 support is enabled requires explicit host environment inheritance disablement and no directory or socket preopens, including modules with no ROS grants. WASIp1 normally inherits the host environment: use `--wasip1-global-noinherit-system-environment` and ensure applicable WASIp1 Single/Group overrides also disable inheritance. ROS enable/grant options do not alter WASIp1 permissions, and extra WASIp1 host connections reject ROS startup. [security.md](../ros/security.md) describes the remaining process and network boundary.

`audit-deny-max-rate-hz`, `audit-deny-burst`, `audit-deny-max-records` and `audit-deny-max-bytes` explicitly bound the worker audit sink for unknown aliases, forged handles and refused proxy calls. Every enabled Target requires all four fields. They are worker-local and do not affect ROS network identity. The rate must be finite and positive (at most 1000000), burst/records are positive integers (at most 1048576), and bytes are at most 67108864. No unauthenticated request can choose its own audit budget.
