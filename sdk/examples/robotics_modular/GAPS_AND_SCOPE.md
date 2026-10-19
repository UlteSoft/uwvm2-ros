# Modular robotics source scope

This directory is a source-prepared example, not an observed robot, simulator,
native ROS, timing or performance result. No compiler or Runtime was run while
authoring it. The integer/String graph computes bounded decisions and sends no
motor command. The additive `unitree_*` sources provide simulator-only scalar
perception, motion and callback composition, consumed by the separately granted
Unitree root examples. They do not establish physical robot behavior or safety.
This directory contains no Unity, Gazebo, camera or physical robot driver.

The current Runtime supplies named Wasm preloads, cross-module function imports,
separate module memories, and per-module ROS/WASIp1 policy. A preload is inside
its Worker process, not a separate OS process. A preloaded module's actual Wasm
start section runs during graph startup; an export called `_start` is not
automatically called as a library initializer. The example therefore explicitly
initializes its callback registry after the whole graph is available.

The motion library calls a separately compiled callback Wasm through a real
function import. Owner/generation tokens in this sample reject a stale local
registration. They are application data, not an unforgeable Runtime caller
identity or authorization grant. Runtime ROS identity remains its own trusted
instance plus generation. Native ROS callbacks never call a Wasm function
pointer: an SDK executor polls and then calls guest code synchronously.

Only files newly added under `sdk/examples/robotics_modular/` are owned by this
change: this scope note, normative README, scalar ABI/header, C/C++ providers
and mains, optional Rust/AssemblyScript providers, an explicit graph in WAT,
and JSON build/launch recipes. No Runtime, Gateway, SDK library, existing test,
robot driver or simulator source is changed. Additional language recipes are
limited to compiler/import/export mechanisms already present in this tree;
the original JavaScript bridge remains an external-host path. The additive
`*_guest.mjs` providers now have explicit recipes in
`sdk/javascript/guest/recipes.json`: a pinned interpreter and closed actual JS
sources compile into each real Wasm principal. Their C scalar boundaries,
separate heaps, default grants and lifecycle need independent binary/Runtime
qualification; an external Node fake cannot satisfy it. The synthetic benchmark
roots do not qualify Unitree motion, image inference or physical equivalence.

The ROS extension keeps Worker, Gateway and independent ROS participant as
separate processes. It does not merge their address spaces or let a library
borrow main's ROS handle. Pure modules get no ROS, WASI filesystem, clock,
randomness, environment, socket or host API right. A separately selected main
may explicitly open one reviewed String-publication alias. Default mode checks
denial with unchanged output storage; positive publication alone is not proof
of independent native receipt.

Source anchors: `src/uwvm2/uwvm/run/run.h` (`run_initialized_module_graph`),
`src/uwvm2/uwvm/cmdline/params/wasm_preload_library.h`,
`src/uwvm2/uwvm/cmdline/params/ros1.h`, `ros2.h`, WASIp1 single-target options,
`src/uwvm2/uwvm/imported/ros/host.h` (`identify`),
`test/0017.runtime/run_cross_module_funcref.sh`, and
`sdk/docs/architecture.md` / `documents/ros/capabilities.md`.
