# Same-module C bridge include contract

This AOT route consumes the complete existing headers and bridge bodies from
the captured SDK Source closure. It introduces no additional Host ABI.

* `sdk/ros2/abi.h` and `sdk/common/abi.h`: the selected original43 ROS2 imports.
* `sdk/include/roswasm/rmw_service_v1.h`: the exact optional12 numeric imports.
* `sdk/csharp/owned/include/roswasm_csharp_owned_bridge.h`: the real copied-buffer
  C/managed seam used by `NativeOwned`. Compile its actual
  `sdk/csharp/owned/roswasm_owned_bridge.c` with owned1/backend2, plus the
  original `sdk/csharp/roswasm_bridge.c`, into the same final Wasm module.

Use the matching target libc/Mono/WASI include closure. Preserve unsigned64
IDs, indices, complete extents and rooted input/output arrays exactly. No C#,
F# or VB.NET reference, object header, arbitrary native address, host DLL or
function pointer is a valid substitute for that bounded contract.

The selected Mono target must scan the actual two `DllImport` module basenames
before managed-to-native generation and link each object once. `Aot.targets`
performs those explicit steps; `inspect_aot.py` checks their actual emitted
items, registration tables and final signatures after a future real build.
The shared SDK's GCHandle pins, attempt publication, cleanup and permission
rules are preserved. Including or compiling a header alone is not evidence
that the actual final module satisfies those rules.

All related Source, full headers and definitions are pinned in the package
support ledger for independent review. No native compilation, Wasm execution,
debugger or ROS test has run in this authoring task.
