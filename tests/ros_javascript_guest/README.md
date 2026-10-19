# Actual JavaScript-in-Wasm Runtime acceptance

This lane runs the compiled embedded QuickJS guests through the **real uwvm
interpreter Worker**. It does not use Node, a host JavaScript callback, a fake
`WebAssembly.Memory`, a model ROS function or a raw Gateway RPC replacement.
Source preparation, compilation/binary qualification, local Runtime behavior
and independently observed native ROS effects remain separate evidence scopes.

All files here are source-prepared until an owner retains actual reports. The
Source33 all30 build/binary report establishes compiled inputs; it establishes
no execution of a guest application or native ROS controller. The separate
MuJoCo model/CRC preflight establishes its recorded physics plumbing and CRC
comparisons, not this lane's native delivery or robot control.

## First run the26 local variants

`run.py` admits the complete all30 binary report, every original preparation,
object/full-MD/link/validator record and all four fresh native Worker profiles.
It reopens the original complete source freezes. Every physical compiled source
input must also match the executing freeze. Adding a harness or updating an
evidence document does not rewrite a compiler's original manifest label.

It separately runs the selected real Wasm validator again and independently
inspects the actual complete import signatures, exports, one bounded memory
and absent start section. Pure libraries retain zero ROS/WASI imports. Command
guests retain exactly their38/43 ROS functions and basic fd_read/fd_write/
proc_exit imports. No import allowance is enlarged to make an artifact pass.

The independent WAT root in `fixtures.py` imports the actual four pure JS
libraries and calls init/status/pump, integer and floating functions, real
motion-to-callback imports and close. It checks callback generation/retirement,
stale responses, invalid pump bounds, f32 rounding, Infinity/NaN class, negative
zero and CLOSED5 alongside explicit scalar fallbacks. No library receives a
WASI or ROS capability. This small caller is separately translated, validated
and pinned before execution.

The22 local command products run unchanged, including codec/ownership/empty
application cases, the two modular checksums, floating-module composition,
actual protected default refusals and the three deliberately armed engine
limits. Limit success requires the exact armed stdout plus actual exit3;
startup failure, timeout, signal or arbitrary trap fails. The persistent
benchmark performs W/P/P/Q on retained instances and compares every8LE result
with a separate Python integer oracle. This is correctness evidence, not a
native benchmark or robot/perception equivalence claim. Empty lesson03/04/06
configurations remain local cases, not service/Action/custom-message delivery.

Two additional default-denied runs execute the ROS1 and ROS2 raw guests through
the actual **dual** Worker. They add profile observations, not recipe products.
The expected local result is25 serial Runtime cases covering26 distinct recipe
variants and all four Worker profiles. The four native-dependent original
recipes remain explicitly pending.

Every Runtime root first writes `JS_RUNTIME_GATE\n` and blocks on G. Before G,
the owner proves the live retained pidfd/birth/current cgroup and streams the
entire actual `/proc/PID/exe` descriptor against the selected native Worker.
The wrapper then calls the preloaded guest's actual `_start`. A command that
returns instead of its declared proc_exit fails202. Each pure library remains
a separate Wasm instance with its own engine and memory.

## Explicit current64GiB inputs

Invoke the entry from its complete new frozen source, with a fresh owned output:

```sh
"$PINNED_PYTHON" -B "$FROZEN_SOURCE/tests/ros_javascript_guest/run.py" \
  --source "$FROZEN_SOURCE" --manifest "$SOURCE_MANIFEST" \
  --manifest-sha256 "$ACTUAL_SOURCE_MANIFEST_SHA256" \
  --config "$PINNED_EXECUTION_CONFIG" --config-sha256 "$ACTUAL_CONFIG_SHA256" \
  --out "$FRESH_OWNED_OUTPUT"
```

The caller must already inhabit the original, explicitly selected64GiB/swap0
cgroup. The shared `tests/ros_unitree/common.py` Frame/Actors and
`tools/ros_distro/common.py` retain current boot/device/inode/CPU identities,
unchanged zero ancestor OOM counters,4GiB memory reserve,8GiB disk reserve on
output and backing ancestors, full source and whole-root inventories, finite
RSS/log/output/deadline bounds, cooperative cancellation and owned pidfd cleanup.
There is no cgroup migration, daemon, package installation or global setting.
Local products/testing belong under `/tmp`; Root selects remote storage.

The configuration uses the existing shared Frame envelope, not an implicit
project environment. Its exact top-level fields are `format_version:1`,
`cgroup`, `input_roots`, `inputs`, `environment`, `limits`, `upstreams:{}`,
`simulation:null`, `benchmark`, `profiler:null`. The historical envelope field
`benchmark` selects this execution role; this lane claims no timings. Use
jobs1, command_seconds<=600, rss_bytes<=536870912 and explicit positive bounded
log/output limits. Every selected file is an input label mapping to the actual
absolute physical `path`, `bytes`, `sha256`. Source, controls, original build
outputs and **complete** original tool/SDK/QuickJS roots must be inventoried;
do not select only the executable while omitting its package or libraries.

The exact `benchmark` selection is:

```json
{
  "schema": "uwvm-embedded-js-runtime-inputs-v1",
  "build_report": "all30-build-report-input-label",
  "build_manifest": "original-build-manifest-input-label",
  "build_source": "/explicit/original/frozen/source",
  "worker_report": "actual-native4-inner-report-input-label",
  "worker_manifest": "original-worker-manifest-input-label",
  "worker_source": "/explicit/original/worker/source",
  "workers": {
    "none": "actual-none-worker-input-label",
    "ros1": "actual-ros1-worker-input-label",
    "ros2": "actual-ros2-worker-input-label",
    "dual": "actual-dual-worker-input-label"
  },
  "validator": "actual-wasm-tools-input-label",
  "native_cases": []
}
```

These strings are **schema examples**, not resolved paths, hashes or permissions.
A Root metadata maker must open all actual inputs and fill the labels. Preserve
the exact original source manifests and all actual producer report associations.
The local status is `PASS_EMBEDDED_JS_LOCAL_RUNTIME_ONLY`; it always records
`actual_native_ROS_execution:false`, `all30_Runtime_qualified:false` and no
independent zero-native-effect qualification. Actual errno13/sentinel checks
alone do not substitute for an independent native witness.

## Native adapter and additional real event fixture

`native.py`, `native_peer.cpp`, `exec_gate.cpp`, `native_event.mjs` and `event_manifest.py` supply
the separate native path. A native producer must first compile and qualify the
actual Gateway/peer against the owner-selected SDK. The peer explicitly uses
C++20 because the selected fast_io headers use concepts. Gateway standard
selection remains its independent real feature probe. The peer never invents
a ROS2 type hash on Humble: real GIDs/MessageInfo identify the writer, while
the native identity producer supplies the independently retained interface
identity/introspection evidence.

The old lesson08 accepts an initial idle event poll and checks resource_closed;
that behavior is ownership-only. The additional `native_event.mjs` first
activates an actual typed subscription, watches it with a separate observer,
waits for an independent native match/publication, and polls one attempt per R.
It requires an actual canonical String topic event followed by a correctly
bound resource_closed event. The additional two event artifacts are not silently
inserted into the already qualified30-product inventory. Run `event_manifest.py`
as a guarded preparation child to write each expanded manifest, then run the
ordinary guest producer and retain every actual compile/full-MD/link/validation
job and actual-build receipt. Neither manifest generation nor publication return
is a native-delivery result.

Native cases require a fresh owner-created loopback-only network namespace,
short owned UNIX socket paths, real per-operation grants, separately owned
Gateway and native participant, current executable/SDK `map_files` FD proofs,
and exact discovery/delivery/quiet-window transcripts. Native positive cases
run the actual JavaScript artifact as the real main principal. A separately
compiled test-only native gate holds the exact Worker descriptor, waits for G,
then atomically replaces itself through execveat, preserving the PID/birth used
by the Gateway's actual SO_PEERCRED check. The local WAT wrapper continues to
serve only empty-ROS-rights cases; it never lends permissions to a preload.
Native SDK environment variables belong only to native actors, while the gate
and actual Worker retain the independently selected host environment.
The native adapter and producer contract must be source-peered before execution;
unselected backends remain unqualified. No signing/update/hub/orchestration
software, physical actuation, vLLM launch or native-performance claim is involved.
The exact native producer/configuration contract, phase-specific mapping scope
and conditional all30 aggregation are documented in [NATIVE.md](NATIVE.md).
