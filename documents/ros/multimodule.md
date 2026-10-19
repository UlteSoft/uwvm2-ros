# Multiple modules and policy groups

The loaded module name is a loader binding label; it is not a security credential. Set the main label with `--wasm-set-main-module-name <name>` and name a provider with `--wasm-preload-library <file> <name>`. Loading a provider does not invoke arbitrary application exports. A provider's explicit Wasm start section executes during initialization; subsequent exported functions require a real Wasm import/export link. The five-module test uses start sections for each preload and a dual-backend main `_start`, while proxy tests exercise actual intermodule calls.

For each ROS version, a module resolves to exactly one explicit Single or Group target, otherwise that version's Global target. Explicit bindings never inherit Global connections or grants. An unknown bound module fails before execution. Unicode module labels are validated as Wasm UTF-8 names; group and capability aliases use the documented restricted ASCII syntax.

Policy groups are separate across ROS1, ROS2 and WASI. Creating `sensor` in all three namespaces is valid; each binding must be specified independently. Each actual runtime Instance gets independent ROS1/ROS2 handle tables, quota buckets and Gateway sessions even when it shares a policy group. Handles cannot be forwarded to another Instance.

Ordinary cross-module calls are not delegation. The interpreter records cross-module ancestry and preserves it through tail-call replacement. A → B → ROS is denied even when B has a grant or both labels belong to the same policy Group. A helper running entirely within the same module retains that module's authority. No explicit cross-module delegation API currently exists.

The tests in `examples/ros_dual/proxy_caller.wat`, `proxy_tail.wat` and `proxy_provider.wat` exercise the real call and tail-call mechanism. The runtime test report distinguishes actual proxy denial from pure policy table tests. Concurrent native callbacks never select a global current module name.
