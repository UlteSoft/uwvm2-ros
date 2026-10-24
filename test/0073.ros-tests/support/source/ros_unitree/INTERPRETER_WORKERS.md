# Maintained native interpreter Worker producer

`build_interpreter_workers.py` builds four **fresh** x86-64 Linux Workers:
`none`, `ros1`, `ros2`, and `dual`. Each compiles the real `runtime`, `main`, and
`host_api` translation units and separately links them. All twelve objects and
four binaries must have the actual ELF62/64/little-endian target. No Worker,
Guest, ROS client, Master, Gateway, simulator, or robot executes in this lane.
The two tool-version commands execute only the selected compiler and linker.

This maintained successor incorporates the genuine private Worker37 producer
whose source was independently recovered with SHA256
`e367e807620e9b3781793251d4fe225b090950c1eee8258119c00812697fe991`.
The [original source and actual recovery packet](../../../../../documents/ros/continuation-20261016-boot3-evidence/worker-producer-recovery)
remain separate. That older report does not become a successful build of this
new file, a new registry, or a new source snapshot.

## Inputs and invocation

Freeze the complete project with the normal manifest mechanism, including this
producer, `tools/ros_distro/common.py`, `test/0073.ros-tests/support/source/ros_cgroup/probe.py`, and
`test/0073.ros-tests/support/source/ros_debugger/build_runtime.py`. Supply the actual whole-manifest hash and
an immutable operator-owned JSON configuration with its actual hash:

```sh
/absolute/current/python3 -B "$SOURCE/test/0073.ros-tests/support/source/ros_unitree/build_interpreter_workers.py" \
  --source "$SOURCE" --manifest "$MANIFEST" --manifest-sha256 "$MANIFEST_SHA256" \
  --config "$CONFIG" --config-sha256 "$CONFIG_SHA256" --out "$FRESH_OUTPUT"
```

The process must already inhabit the **selected original 64 GiB cgroup**, with
swap disabled. Its leaf device/inode, boot ID, effective CPUs, and actual
membership must agree with the supplied current observations. The producer
checks these before loading any project helper; the manifest-bound probe then
checks the visible ancestor controls and counters. It neither creates a cgroup
nor moves itself or a child to another one.

The closed configuration has these members:

| Member | Required value and purpose |
| --- | --- |
| `format_version` | Integer `1`. |
| `source_root` | An existing physical directory covered by `input_roots`; retained solely as a complete metadata/source input. It is never an implicit LLVM provider. |
| `tools` | Exactly `cc`, `cxx`, `cmake`, `ninja`, `ar`, `ranlib`, `linker`, and `python`, each an explicit executable invocation path covered by complete roots. Invocation aliases and their physical executable pins are both preserved. |
| `target` | Exactly `triple`, `machine`, `bits`, `endian`, `host_machine`; native x86-64 Linux, ELF machine62,64 bits,little-endian. The host is independently checked. |
| `stdlib` | `libstdc++`. |
| `cflags` | Exactly one explicit `--gcc-install-dir=/actual/physical/GCC/15` operand. |
| `cxxflags`, `link_flags` | That same selected GCC installation and `-stdlib=libstdc++`; no extra LLVM/search/configuration provider. |
| `sysroot` | `null` or an existing physical directory covered by complete roots. |
| `input_roots` | One to32 distinct complete physical package/tool/registry roots, disjoint from the frozen source and fresh output. |
| `environment` | Explicit `PATH`, `LANG`, `LC_ALL`, optionally `LD_LIBRARY_PATH`. Every path-list directory has complete physical-root coverage. It is compiler environment, not inherited Guest authority. |
| `cgroup` | Exactly `memory_max`, `memory_swap_max`, `expected_leaf`, `expected_device`, `expected_inode`, `expected_boot_id`, `expected_cpus`, all from actual current observations. |
| `build_seconds` | Integer600..86400; the active build uses the smaller of this value and14400 seconds, with at most600 additional seconds for finalization. |
| `profile_compile_flags` | Optional complete map of the four profile names. Each list is empty or contains one literal `-DUWVM_ROS_CUSTOM_TOPIC_MANIFEST="/physical/registry.h"`; `none` must be empty. Every selected header must be observed in original full-MD dependencies. |

This deliberately retains the observed Clang23/GCC15 tool profile. It is not a
claim that a ROS distribution requires that compiler, or that another compiler
has been tested. Cross-architecture interpreter qualification uses the separate
[multiarchitecture lane](../ros_multiarch_runtime/README.md). Patched LLVM/JIT
Workers use [the debugger producer](../ros_debugger/README.md), not this lane.

For legacy Unitree ROS1, use the **freshly generated complete eleven-definition
ROS1 registry** in the `ros1` and `dual` profiles. A GO/HG/API ROS2 registry, its
old compile report, a filename, or a package label cannot substitute for it.
Before behavioral testing, separately qualify the native identity exporter and
match the registry's complete native identities to that actual export.

## Build and resource guarantees

The build is serial, with a5 GiB command-tree RSS cap,32 MiB command-log cap,
600-second command limit,1 GiB coordinator RSS cap, and16 GiB output cap. Admission
reserves the compiler cap and2 GiB parent headroom below a60 GiB aggregate ceiling;
the unchanged64 GiB kernel cgroup remains the hard boundary. All backing
filesystem ancestors must retain8 GiB available space during active work and in
a fresh terminal success check. Storage exhaustion does not prevent attempting
owned child cleanup. No global cache is dropped, no package is installed, and no
input or earlier product is deleted.

Each real TU uses C++26,`-O1`,`-g0`,`-UNDEBUG`, full `-MD -MF -MT`, the selected
profile macros, and the real complete-source includes. The previously observed
stock Clang23 MachineDCE failure has one explicit fixed workaround:
`-mllvm -disable-machine-dce` on the **main TU only**. Runtime/host_api flags and
all link flags retain their original optimization. The report preserves that
limitation; it does not certify the patched LLVM provider or native-equivalent
performance. No stop shim or test macro replaces application main.

Every object has its actual original complete system-header dependencies. The
real linker trace must contain all three owned objects, selected native standard
library inputs and actual archive members; LLVM provider substitution is
rejected. Executable/alias identity, all source bytes, complete input catalogs,
original command logs and all products are rechecked at the terminal barrier.
Observed children are owned and retired through pidfds, never numeric-PID or
process-group signalling. The half-second advisory output walk avoids repeating
a whole directory traversal for every streamed dependency; cgroup, RSS and
backing-space checks remain active independently.

## Results and limits

A complete build publishes
`PREPARED_FOUR_FRESH_NATIVE_INTERPRETER_WORKERS_NOT_EXECUTED`, with canonical
profile order, chronological commands,12 original TU/object records,16 build
commands plus two tool-version commands, explicit registry MD associations and
fresh physical binary pins. A partial batch remains failed and its raw evidence
stays unchanged. The consumer must still prove the selected executable, live
birth, current cgroup, actual mappings and correct registry **when it executes**.
No default-denial, authorized delivery, TCPROS/DDS, callback, simulation, debugger
or performance result is implied by a successful build.

This maintained successor has been source-authored only. Actual compilation and
behavioral tests require the next complete frozen source and real configuration;
no earlier Worker37 run is counted as its test.
