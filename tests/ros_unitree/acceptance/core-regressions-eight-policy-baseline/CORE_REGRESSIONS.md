# Linux policy and SDK-free ticket regression owner

`run_core_regressions.py` executes two actual Python policy suites and one
freshly compiled native contract under the selected original 64 GiB, zero-swap
Linux cgroup. Root invokes it only after this entry and its dependencies enter
a new complete source freeze. Importing this entry starts no command. Frozen
Source43 and its old raw reports remain unchanged.

The eight Python tests are five output-budget/cache policy cases and three
actor-retirement-ledger cases. Their own fake cgroup/process providers isolate
policy behavior; they cannot establish a real cgroup, pidfd, ROS or Runtime
result. The outer regression owner separately checks its actual cgroup and
owns every real Python child through retained birth identities and pidfds.

The native target is exactly `tests/ros_ticket_lifetime/contract.cc`. It runs
the seven declared ticket/shared-Service groups without a ROS SDK, node,
Master, Gateway, Worker, Wasm Guest or simulator. It does not compile or execute
the separate SDK-dependent `action_ros1.cc` or `action_ros2.cc` targets. Those
affected native Gateway rebuilds and actual Service/Action suites remain
separate acceptance.

## Actual input contract

Use the same closed native compiler configuration consumed by
`build_interpreter_workers.py`, with **empty selections for every profile**.
No registry definition, SDK profile flag, LLVM Runtime provider or reused
object is admitted. The configuration selects the actual Clang23 driver,
GCC15 installation, libstdc++, linker, Python and five other explicitly pinned
tool paths. No executable is discovered through the ambient PATH. All eight
selected tools receive independently opened full-seven observations before
the first child; invocation aliases are preserved separately because multicall
tool basenames matter. Unused tools are inventoried, not executed.

`core-regressions-native-config.template.json` preserves the closed schema.
Its null fields and empty roots are deliberately unusable. Root may instead
reuse its already reviewed actual native configuration, clearing all four
`profile_compile_flags` arrays. Populate every path and flag from genuine
opened observations. The paired `cflags`, `cxxflags` and `link_flags` must retain
the existing explicit `--gcc-install-dir=ACTUAL_GCC15` and libstdc++ associations.
The actual operator configuration also supplies target architecture, optional
sysroot, compiler-only environment and complete immutable input roots. A
metadata source root is a catalog input, never an LLVM provider or include
replacement.

Invoke the maintained entry with actual values:

```text
ACTUAL_PYTHON -I -B ACTUAL_FROZEN_SOURCE/tests/ros_unitree/run_core_regressions.py
  --source ACTUAL_FROZEN_SOURCE
  --manifest ACTUAL_COMPLETE_MANIFEST --manifest-sha256 ACTUAL_MANIFEST_SHA256
  --config ACTUAL_NATIVE_CONFIG --config-sha256 ACTUAL_NATIVE_CONFIG_SHA256
  --out FRESH_OWNED_OUTPUT
```

The capitalized words describe required actual inputs; they are not forecast
paths, physical pins, process identities or a future source version. Root's
outer owner must already supervise the selected cgroup and invoke the admitted
entry bytes. The entry reopens its manifest, own source and configuration as
stable bounded ordinary files, then proves the actual selected cgroup using
only stdlib before importing any project helper. The source-bound probe
subsequently checks all ancestors and zero OOM counters. Complete source and
input-root inventories run before and after the commands.

## Serial commands and evidence

The owner starts exactly six commands: the Clang23 version observation, the
five-test Python suite, the three-test Python suite, one compile, one link and
one native execution. Both Python suites run with `-I -B` and verbose unittest
output. Success requires exit zero, the actual complete test count and every
`... ok` marker. The test fixtures live only below the owned TMPDIR.

The native command is C++20, `-O2 -g0 -UNDEBUG -pthread`, without fast math or
floating-point contraction. Its include roots are the actual current `src`
and fast_io tree. Full `-MD -MF -MT` records must include the real primary,
shared Service header, probe helper and `fast_io.h`; this qualifies the C++20
fast_io formatting branch rather than the C++17 compatibility branch. Fresh
owned object and executable ELF62 identities, every full dependency, original
compiler argv/cwd, linker trace, all native archive members and opened link
inputs are preserved. The complete linker trace must include the owned object
and the selected libstdc++ input.

Native execution requires exit zero and exactly this original single record:

```text
PASS ticket-lifetime groups=7 completed=1048608 queue_rejected=1048608 exact_uint8_nonzero_domain=255 parallel_ids=131072 parallel_Service_ids=8192 no_ROS_or_Guest_execution
```

The ordinary retained pidfd owner qualifies child birth, resource bounds and
terminal status. This narrow entry does **not** request `--owned-stop`, inspect
live native map_files or claim a mapped-stop proof. Its report records that
boundary as `NOT_PERFORMED_NO_LIVE_MAPPING_CLAIM`. All source/SDK/library byte
catalog and compiler/link evidence remain mandatory.

## Resource and finalization boundaries

Commands are serial, each bounded by 600 seconds, 4 GiB owned RSS and 16 MiB
logs. Coordinator RSS is bounded by 1 GiB. The active overall deadline is the
smaller of the actual configuration's `build_seconds` and 1,800 seconds; a
separate additional 600-second terminal allowance covers cleanup/preservation.
Actual current memory must leave child RSS plus 2 GiB supervisor headroom
below 60 GiB before starting the first command.

All artifact/backing ancestors retain the 8 GiB admission reserve during active
work. Output is bounded to 1 GiB; only its walk is cached for at most 0.5 seconds,
while the complete cgroup and backing-capacity checks run on every guard call.
Policy fixtures intentionally create and remove tiny temporary files, including
an alias-refusal fixture. During active output auditing only, vanished entries
below the owned TMPDIR and the shared atomic publisher's `.json.pending`
entries are permitted. Existing temporary aliases must resolve within that
same TMPDIR, are never followed as ordinary files, and grant no host authority.
Immutable source/input/full-seven/preservation checks have no such exception.

On failure the owner performs pidfd cleanup before whole source/input/product
preservation even if storage admission has failed. The terminal output walk
and 8 GiB reserve check are fresh. Failure in cleanup, preservation, terminal
reserve or any earlier command keeps the batch failed. Only the exact complete
result receives
`PASS_EIGHT_POLICY_TESTS_AND_SEVEN_SDK_FREE_TICKET_GROUPS`.

Source readiness and independent source review are not the result of these
pending actual Linux tests. Root must retain the real command logs and terminal
report before reporting any policy or native-contract PASS.
