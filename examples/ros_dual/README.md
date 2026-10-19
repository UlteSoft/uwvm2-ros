# Dual ROS and module-isolation Wasm fixtures

`hybrid_1_to_2` and `hybrid_2_to_1` receive and validate String `hello`, explicitly
construct the outgoing value, and publish it to the other backend. Both ROS
capabilities must be granted independently. Typed Bool, Twist and Image
variants follow the same path; Image checks all canonical fields except the
sequence field absent from ROS2.

`proxy_caller` invokes `proxy_provider`, whose ROS call must be denied even when
the provider alone owns the capability. `proxy_tail` makes that call through a
Wasm tail call and requires `--wasm-feature-enable-tail-call`.
`handle_provider` opens its own handle in its start section; `handle_attack`
tries to publish using that other instance's handle and expects error 9.

These fixtures are executed by `../../tests/ros/run_network.py` with explicit
module, group and gateway policy. Compile with
`wat2wasm --enable-all proxy_tail.wat -o proxy_tail.wasm`.
All WASI-enabled loaded modules require explicit
`--wasip1-global-noinherit-system-environment` or their module/group equivalent
when either ROS backend is enabled.

Both backend policies and each module binding remain separate.
