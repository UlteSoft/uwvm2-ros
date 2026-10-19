# ROS1 Wasm fixtures

`talker` publishes canonical String `hello` twice; `listener` subscribes and
checks the incoming value within ten seconds. The `_bool`, `_twist` and `_image`
variants exercise typed adapters. `service_client` calls Trigger.

`unauthorized` expects denial with no enabled target. `memory64` checks valid
and overflowed i64 memory offsets; `multi_memory` checks explicit memory indices.
ROS-capable execution requires explicit gateway and capability settings; see
the [ROS1 CLI](../../documents/command-line/ros1.md) and the generated policies in the real
network runner `../../tests/ros/run_network.py`.
Use `--wasip1-global-noinherit-system-environment` when running these WASI
fixtures with an enabled ROS target. Directory/socket preopens and socket
standard descriptors are rejected before Guest startup.

Compile a WAT fixture with `wat2wasm --enable-all talker.wat -o talker.wasm`.
Enable `--wasm-feature-enable-memory64` for `memory64` and
`--wasm-feature-enable-multi-memory` for `multi_memory` when running uwvm.
The C sources use the guest SDK and import WASI clocks directly; they require no
native ROS library. Typed WAT sources are reproducible with
`python3 ../../tests/ros/generate_fixtures.py` from any working directory.
