# Scene named-module ownership

The new scene ABI has separate model, configuration and host module owners.
This structure does not change the implementation or authority of WASIp1,
ROS1 v1 or ROS2 v1. The ABI module name is `uwvm_ros_v2`; C++ module paths
describe ownership and do not introduce another API version.

## Ownership and initialization

`uwvm2.imported.ros.scene` owns scene policy types, bounded runtime records,
authority, local scheduling brokers, deployment validation and storage
backends. Importing the module does not instantiate a provider or grant. It
depends only on the standard/platform facilities used by those components.

`uwvm2.uwvm.imported.scene.storage` owns the one deployment configuration used
by CLI policy callbacks, Wasm import validation and the checked host. Named
consumers import it. Directly including its configuration header from another
named module is a compile error; otherwise each module could accidentally
define a separate inline configuration and lose the configured policy.

`uwvm2.uwvm.imported.scene` binds the actual host callbacks to checked runtime
identity and memory APIs. Its lifecycle registration installs trusted scene
retirement and metadata-inspection hooks. Its concrete import tuple and the
explicit floating-point preservation specializations are exported together.
The generic wrapper defaults to a complete host floating-point guard; only
the reviewed closed scheduling, storage and registered-message callbacks opt
into preserving controls when LLVM support is built. The exact whitelist and
its safety rationale are declared together in `scene/host.h`. Such builds independently guard arbitrary imported
memory-provider callbacks. Interpreter-only builds retain the outer complete
guard because their nested provider path has no independent FP guard.

The runtime implementation and debugger must observe the same hooks even
when runtime glue uses textual includes. `scene/lifecycle.h` and
`scene/debug.h` deliberately give their DTOs and inline hook variables global
C++ module attachment using an explicit `extern "C++"` linkage block. Those
hooks confer no guest authority. Metadata inspection remains bounded and
subject to its independent debugger permission.

The CLI scene partition imports the configuration owner. The debugger scene
parser imports only the model and `fast_io`. Loader validation imports the
configuration; the builtin host loader imports the host. These dependencies
avoid having the model depend on the CLI or runtime implementation.

## Verification and current limits

`tools/ros_scene/check_modules.py` compiles the actual model, configuration
and debugger interfaces. It links two separately compiled named consumers
with a textual runtime-glue translation unit. The fixture verifies shared
configuration by address and observable exposure, verifies both hook
addresses, delivers a retirement callback across the boundary and retrieves
detached metadata through the same debugger hook. A negative control rejects
an attempted second named configuration owner.

The script accepts compiler/linker flag arrays as JSON and must be launched
through `tools/ros_scene/in_existing_cgroup.py`. All subprocesses inherit the
existing 64 GiB, zero-swap cgroup. It records commands, exit status, source
hashes and cgroup events in the chosen output directory under SDK `.tmp`.
Driver-only C++ fixtures use `.cpp`/`.cppm` so they are not registered as
standalone `test/**.cc` programs.

`--isolated-host` additionally exports the real host header with the existing
infrastructure textually included in a global module fragment. It compiles a
consumer that checks the sixteen explicit FP-preserving specializations and
the conservative ABI/open/close/new-callback defaults. The header test also
checks that a build without LLVM selects the conservative policy. This isolates new
host export behavior; it is **not** a complete production module build.
It cannot be combined with `--graph` or additional production graph roots.

`--graph` attempts the actual CLI, host, validation, threaded-loader and run
loader roots, preserving an independent error for each root. The current
production graph has pre-existing blockers: the `fast_io` module does not
export `io_dup_t` used by the output module, and the existing imported-ROS
module omits the host-clock/process-exit dependencies used by its synchronous
wait implementation. The frozen legacy sources are not modified to hide
those failures. A passing focused or isolated suite must not be reported as
successful compilation or linking of that complete graph.
