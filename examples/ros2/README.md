# ROS2 Wasm fixtures

`talker` publishes canonical String `hello` twice; `listener` subscribes and
checks the incoming value within ten seconds. The `_bool`, `_twist` and `_image`
variants exercise generated ROS2 type adapters. `service_client` calls Trigger.
`unauthorized` expects denial with no enabled target.

These fixtures require explicit gateway, DDS domain, QoS and capability settings.
See the [ROS2 CLI](../../documents/command-line/ros2.md) and the real network runner
`../../tests/ros/run_network.py`. Compile WAT with
`wat2wasm --enable-all talker.wat -o talker.wasm`. The C sources use the guest SDK
and require no native ROS library. Typed WAT sources are reproducible with
`python3 ../../tests/ros/generate_fixtures.py`.
Use `--wasip1-global-noinherit-system-environment` with an enabled ROS target;
directory/socket preopens and socket standard descriptors are rejected before
these WASI fixtures execute.
