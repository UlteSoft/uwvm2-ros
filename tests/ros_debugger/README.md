# ROS debugger components

These are new, unexecuted test sources for the read-only ROS debugger. They do
not replace authenticated Guest/native ROS delivery, native debugger stepping,
platform debugger acceptance or the original final acceptance matrix. No
compiler, test, model, helper, ROS process or remote command was run by the
implementing agents for this increment.

| Source | Meaningful assertions prepared | Scope |
| --- | ---: | --- |
| `core_contract.cc` | 35 | Actual capability registry, quota/audit invariance, real cross-thread lease contention, actual event queue/pending/pinned-record invariance and contention, fixed trace-ring paging/filter/overwrite behavior. |
| `console_contract.cc` | 47 | Nine selectors, strict grammar/refusals, exact u64/stop filters, output preservation, noninjecting rendering, worst escaped state/trace page budgets, static completion and help catalogue. |
| `controller_contract.cc` | 14 | Actual authenticated console-origin controller and cooperative domain; running/refused/stopped/resumed admission, actual old-stop refusal and ROS-off behavior. The DATA provider is explicitly mathematical. |
| `host_contract.cc` | 20 per single backend / 24 dual | Real Host installed bridge and actual Runtime identity API: out-of-context default-denied open, no implicit owner/audit sink, retained metadata trace and real Host registry BUSY. No fake Runtime/Instance/memory or native RPC. |
| `module_probe.cpp` | exit status only | ROS-off named-module consumer of `ros_state` and `command`; no native ROS dependency. |

The first three terminal markers are respectively `PASS_ROS_DEBUG_CORE checks=35`,
`PASS_ROS_DEBUG_CONSOLE checks=47` and `PASS_ROS_DEBUG_CONTROLLER checks=14`.
They are expected source contracts, not observed results. A failure prints the
corresponding `FAIL_ROS_DEBUG_*` assertion and terminates. The queue and registry
contention components use promises/futures to establish that the real mutex is
held; they do not infer contention from a sleep. Root must apply an external
deadline so a broken synchronization path cannot hang its acceptance run.

Root's future build entry should use the current pinned C++26 compiler and
the repository's existing header include closure:

```
$CXX -std=c++26 -O0 -pthread -Isrc \
  -Ithird-parties/fast_io/include \
  tests/ros_debugger/core_contract.cc -o "$OUT/core-contract"
$CXX -std=c++26 -O0 -pthread -Isrc \
  -Ithird-parties/fast_io/include \
  tests/ros_debugger/console_contract.cc -o "$OUT/console-contract"
```

The controller component uses the same current Runtime/control link recipe as
`test/0017.runtime/debug_management_interrupt_timeout.cc`; its required Runtime
symbols must come from that real closure, not stubs added to force linkage.
The Host component also links the real current Runtime closure. Build each
header profile with only the explicit `UWVM_IMPORT_ROS1`, `UWVM_IMPORT_ROS2` or
both definitions, run in a fresh process with no Runtime entry, and expect
`PASS_ROS_DEBUG_HOST checks=20 calls=2` for either single backend or
`PASS_ROS_DEBUG_HOST checks=24 calls=4` for dual. It exercises actual Host
functions outside authenticated Runtime call context and correctly receives
13. It does not claim a Guest has run or authenticated native delivery. No
Runtime symbol stub is allowed to make this component link or pass.

Build the named-module probe with the current existing debugger module DAG,
including the new `uwvm2.uwvm.debugger.ros_state` module. The core DTO and
`event_stream::try_statistics` remain C++17-compatible; the vendored fast_io
and existing controller/debugger unit are compiled with the project's modern
standard. No C++17 native ROS client is needed by these components.

All commands above are recipes for the Root lane. The agent has not executed
them. Root executes one compile/test at a time (`-j1` where a build system is
used), keeps products/logs under its owned `/tmp` or selected task workspace,
and enforces the current 64 GiB/swap-zero group policy and narrower component
budgets. Toolchain identity, source manifest, actual exit status, cleanup and
OOM evidence must be retained. No prior build may supply a current PASS.

For platform integration, add a current-source ROS-off module/header build and
ROS1/ROS2/dual header builds to the existing debugger acceptance lane. At an
actual cooperative stop, compare repeated diagnostic reads with retained
capability quota/audit and queue counters, then resume and verify refusal.
Native-instruction stops and stale controller lifetimes must refuse. Native
threads holding an observation lock should yield explicit BUSY, never empty
success. Existing Linux/macOS/Windows authenticated console transports share
the same command whitelist and renderer; this source-only preparation does
not claim those transports have been executed for this increment.

The public contract, examples, privacy limits and Host/native boundary are in
[`ros_state.md`](../../src/uwvm2/uwvm/debugger/ros_state.md).
[`compile-recipes.json`](compile-recipes.json) supplies explicit common/append
argv arrays, expected markers and real link/module-closure prerequisites for
Root's existing serial runner. It does not launch commands or replace that
runner's source/tool/birth/RSS/deadline/OOM/cleanup proof.

The update/ROS authority retirement increment has separate source contracts in
[`README.management.md`](README.management.md) and
[`management-recipes.json`](management-recipes.json). They keep native manager,
Host/Unix descriptor, injected DAP/raw ABI and real Guest stdout assertions
separate. Their authored state does not change any historical marker or claim
that a genuine Runtime replacement or official ROS suite has executed.
