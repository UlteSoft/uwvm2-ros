# Six-language real-delivery application inputs

C/C++ use the existing `sdk/origin/examples/{c,cpp}/06_runtime_delivery` applications;
JS uses the existing true C relay plus `05-runtime-relay.mjs`. Rust, TinyGo and
AssemblyScript have executable checked applications here. They all use the
same independent native peers and exact19/24/23-byte String protocol. They are
source inputs, not execution receipts. The controller's separate real Wasm
main waits for G then calls `_start` from the preloaded `sdk_guest` module.
Permissions select that actual module, never the wrapper's name or a fabricated
Instance. Both modules get explicit no-inherited-environment WASI policy.

Rust is a no_std/no_alloc cdylib using the real public Node, typed String codec
and raw refusal probes. Build offline with `cargo build --release --lib -j1
--target wasm32-wasip1 --no-default-features --features ros1` or ros2; add
`denied` only for its separate empty-ROS-policy product. TinyGo builds with
exactly one ros1/ros2 tag, `-scheduler=none -p 1 -opt=0`; sdk_denied selects its
empty-policy product. Use the manifest's exact compiler paths and frozen copy
context for all recipes. Never fetch dependencies during acceptance.

Inspect the final TinyGo startup imports and actual startup separately. The
selected runtime may require WASIp1 `random` before main. If needed, record an
explicit reviewed startup right in the product manifest for both modes; this
does not grant any ROS right. Do not invent deterministic entropy, enable a
fallback, inherit ambient authority or silently grant random after failure.

AssemblyScript has four explicit ROS1/ROS2 positive/absent entries. Compile
with the selected local asc, stub runtime, `_start` and exported memory, and
`--use abort=abortGuest`; no env.abort import or fake SDK NativeApi is supplied.
Root's serial full validator and exact-import inspector must pass before the
product can be admitted. Native ROS1/ROS2 deliveries and zero-effect denied
rows are established only by `run_delivery.py` and physical native evidence.
