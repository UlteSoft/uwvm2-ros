# ROSv2 native simulation and fault qualification

> Historical qualification cohort. The component process owner described below
> has been removed from the production source tree and retained only in
> `test/0084.ros-runtime/support/legacy_process`. Current driver/model/ROS
> providers use [external ownership and attachment](ros-external-ownership.md).
> Use the exact current VM's receipts for current behavior; these historical
> runs do not qualify the newer attachment path.

This qualification exercises actual Wasm imports, the VM device/control
dispatcher, an external native physics engine, framed sensor ingress and the
component process sandbox. It covers the closed scalar and isolated component
bindings. It does not qualify arbitrary vendor drivers, a complete humanoid
controller, physical stopping, industrial certification or the entire ROS corpus.

## Correctness fixes

`device_execution.h` now resolves a claimed command when its controller is
cancelled before the normal acknowledgement pump. If any command byte has
crossed the transport, the outcome becomes unknown and its observation pins and
arbitration debt remain retained. With no bytes written, the outcome remains no
effect. A later stop acknowledgement cannot resolve that earlier command or
establish physical stopped state. The regression failed against the previous
implementation with both partial and complete writes.

`component_dispatch.h` now preserves failed native execution after process
reaping. Public descriptions and deployment/debug snapshots report `failed`
together with `exit_observed=true`; cleanup no longer masks failure as an
ordinary `exited` state. The successful historical start receipt describes
startup only. Repeating its exact key does not restart the component. Another
execution requires a new deployment incarnation. This uses an existing public
and debugger state without adding a permission or exposing native data.

## Actual physics fixture

The engine is the official
[MuJoCo 3.4.0 Linux x86-64 release](https://github.com/google-deepmind/mujoco/releases/tag/3.4.0).
Archive SHA256 is
`3b0e47d8880da0005b61fe5e43a4d058eb5097e6e3ba44f4a23ce2b5fab2244d`,
checked against the upstream asset digest. This pins a reproducible release;
it does not claim the latest version.

The unmodified release model `model/humanoid/humanoid.xml` has 21 actuators.
The fixture controls only `abdomen_z`, using a bounded target angle and fixed
PD mapping to the bounded motor input. It is not locomotion or balance control.
Bounded immutable VFS input replaces generic model filesystem resolution.
The native engine performs actual
[`mj_step` integration](https://mujoco.readthedocs.io/en/3.4.0/APIreference/APIfunctions.html#mj-step).

Wasm uses only `uwvm_ros_v2` imports. Device, control, sensor, buffer and clock
capabilities are independent. The external receiver checks production framing,
schema, generation, controller ownership, sequence and deadline. A separate
sensor pipe returns the actual joint position. Sensor generation and capture
sequence are checked separately.

The native executable, libraries and model are mounted from sealed memfds in
fresh user, PID, network, IPC and UTS namespaces. The root is read-only and
contains only the selected closure. Production startup and final seccomp
filters apply. The VM loads no MuJoCo library. The runner acquires the namespace
child's pidfd at a startup barrier, verifies parentage/cgroup, and retains that
pidfd for termination. Killing only the outer launcher is insufficient.

Seven actual denied operations precede readiness: socket creation, ptrace,
process creation, file creation, reading an absent host file, executable memory
mapping and changing a resource limit. Diagnostic capture has a finite byte
limit and drains independently of the physics thread.

## Fault and permission matrix

Interpreter/AOT, wasm32/64 and memory indices zero/one cover these cases:

- No grants, missing source read/export permission, missing device submission,
  denied control submission, out-of-range values and explicit zero rate cause
  no physical effect.
- Exact retries produce one effect. Continuous runs execute 128 alternating
  target cycles with finite reused buffers. Each time, position and velocity
  value matches the corresponding direct native step.
- Real native segmentation faults, hangs, corrupted digests and stale provider
  IDs fence device/control authority. The VM and independent sensor scope survive.
- Missing confirmation followed by stop retains unknown results and evidence.
  The request cannot be released as a known result.
- Killing or pausing the VM after an effect leaves the external receiver able
  to expire its watchdog. Resume does not restore motion authority.
- A native shared object faults inside the production component sandbox.
  Reaping retains `failed`; historical start retries launch no new process.

Stop clears motor inputs here; a falling humanoid may still move. Records
explicitly set `stationary_claim=false`. A whole-process driver hang also stops
its own watchdog. Sending a reserved stop message cannot make a hung process
execute. Real equipment requires an independently enforced stop/watchdog contract.

Authority dependency, attenuation and reduction suites check current scope
composition and irreversible fences with ASan/UBSan, GCC and TSan. The broader
robot VM matrix covers malformed memory, software memory tags, floating-point
boundaries, missing configuration and separate normal/stop lanes. DAP checks
cover full/read-only/limited permissions and repeated nonmutating queries.

## Resources and timing

All compilers, VMs, native workers and debugger descendants stay in the existing
64 GiB, zero-swap cgroup. Native simulation additionally has a 1 GiB address
limit, eight-second CPU limit, 128 descriptors and disabled core dumps.
Repository C++ uses `-O3 -g`; the upstream MuJoCo binary is used as distributed,
without attributing those flags to its build. VM storage retains recoverable
fast_io allocation. The fixture uses fixed C++ storage.

VM placement is CPUs 4–6; the receiver uses CPU 7. This machine has one NUMA
node. The tests use ordinary scheduling, not real-time or multi-NUMA execution.
Changing the explicitly configured device poll interval from 1 ms to 10 us
reduced measured median sample-to-effect latency from 1.297 ms to 0.322 ms;
95th percentiles were 1.347 ms and 0.356 ms. These cover sample to native effect
entry, not whole round trips or worst-case response. The 10 us polling request
is distinct from hardware sampling. More polling consumes more CPU; this does
not justify silently changing deployment policy or claiming native parity.

Cross-Wasm timings and audited VTune sampling have a separate
[performance record](ros-module-exchange-performance.md). Physics-step time,
IPC latency and whole control-loop time must not be conflated.

## Reproduction and evidence

Use `tools/ros_scene/in_existing_cgroup.py` for every command. The launcher
refuses execution before enrollment and verification of both memory limits.
Build snapshots, official binaries and receipts belong under
`../uwvm2-ros-sdk/.tmp/ros-scene-security/`. SDK production code is unchanged.

- `tools/ros_scene/device_execution_checks.py`: cancellation, partial writes,
  retained unknown effects and independent stop completion.
- `tools/ros_scene/component_dispatch_checks.py`: production process sandbox
  and actual shared-object memory faults.
- `test/0084.ros-runtime/run_scene_mujoco.py`: real physics, repeated cycles,
  fault injection and permission denial witnesses.
- `test/0084.ros-runtime/run_scene_component_crash.py`: public Wasm and DAP
  observation of the crashed isolated component.

Successful receipts require `complete=true`, immutable input hashes, expected
exits and unchanged cgroup event counters. Failed development receipts are not
qualification evidence. General vendor packaging/launching, per-incarnation
resource admission, durable recovery, controlled update/restart, physical stop evidence,
all five native Linux ABIs and full ecosystem review remain separate work; see
the [native driver boundary](ros-native-driver-isolation.md).

### Subsequent exact-runtime regression

`worker-born-domain-pinned-extended-qualification.json` retains 2,914 actual
VM cases against binary
`d865f87e3e759a7710f6e878450f9bdbf388c20f623a8ae96278901ad09fa039`.
The aggregate includes the 128 bounded MuJoCo cases repeated against this
binary; it does not reuse a different executable's simulation result. It also
includes receive-only sensor ingress, sealed buffers, the isolated model and
inert proposal chain, volatile recording/replay, local robot controls, graph,
parameters, actions, transforms and diagnostics. Each receipt preserves its
own narrower scope. Four separate native-core receipts include 18 real owner
kill cases and are not counted as full-VM executions. See the
[birth-domain lifecycle record](ros-native-driver-lifecycle.md#kernel-owned-process-domain-from-birth).

### Earlier Linux x86-64 simulation cohort

The final VM SHA256 is
`7486320ed5408b6c5b3d3275ce6bc4e8156c9a55d779f9b52c94537cf335e4e6`.
`vm-robot-fault-simulation/full-build-results.json` pins that executable,
its optimized/debug build recipes and `robot-fault-simulation-source`.
The following paths are relative to the artifact directory above:

- `mujoco-robot-fault-final/results.json`: all 128 combinations of 16 scenarios,
  two engines and four memory layouts passed with an explicitly configured
  10,000 ns device poll interval. The seven denied scenarios account for 56
  executions with zero native effects. Eight continuous-loop executions each
  complete 128 steps, with every step compared against the native reference.
- `component-crash-vm-dap-bounded/results.json`: eight real Wasm/crashing-library
  executions and three authenticated DAP permission modes passed. Public calls
  retain failure after exit and cannot restart it through a historical request.
  The guest requires failure and confirmed exit within two seconds of start
  admission, before the 4.9-second startup deadline and 15-second execution
  lifetime can mask a fault as a timeout. This is a test observation bound, not
  a real-time stopping guarantee. Repeated full/read-only snapshots are equal;
  limited queries are refused.
- `device-stop-inflight-fixed/results.json` and
  `device-stop-dispatch-regression/results.json`: in-flight cancellation and
  dispatcher regressions, including sanitized builds. The failed pre-fix
  receipt is retained as `device-stop-inflight-before/results.json`.
- `component-crash-fixed/results.json`: six production sandbox groups under
  ASan/UBSan, GCC and TSan. The pre-fix regression is retained separately in
  `component-crash-before/results.json`.
- `device-simulation-authority-dependencies/results.json`,
  `device-simulation-authority-attenuation/results.json` and
  `device-simulation-authority-reduction/results.json`: independent authority
  composition, monotonic attenuation, shared accounting and irreversible
  reduction checks under ASan/UBSan, GCC and TSan. These use their own pinned
  compilation inputs; they are not whole-VM or physical-machine claims.
- `device-stop-vm-local-bounded-query/results.json`: 626 robot VM cases on the
  preceding device-fix binary. `device-stop-dap-regression/results.json` checks
  three debug permission modes and five provider DTOs on that binary. The final
  binary adds the component failure reporting fix; those earlier receipts must
  not be relabelled as executions of the final binary.

Both final runners retain the exact fixture/helper sources beneath
`fixture-source/`, with original and retained path hashes in their receipts.
This permits inspection after subsequent workspace edits. Failed intermediate
test-driver runs remain recorded with `complete=false` and are not counted.
The successful cohorts left `memory.events` unchanged, with no OOM events.
`robot-fault-qualification.json` rechecks all 3,694 final VM snapshot files,
the retained fixture sources and accepted receipt hashes. It counts 1,088
external physics steps, including the 1,024 continuous-cycle steps, and finds
no remaining fixture worker processes after cleanup.

Fault timing must be measured without a ptrace tracer. A separate diagnostic
trace observed actual `SEGV_ACCERR` from the component's protected-page write,
but tracing the namespace PID 1 caused repeated signals until the configured
lifetime ended. The same untraced VM invocation completed in 0.106–0.118 seconds
in three observations. Linux deliberately treats traced init tasks differently
in [`force_sig_info_to_task`](https://github.com/torvalds/linux/blob/master/kernel/signal.c);
this explains the observed tracer-dependent behavior. The trace is retained
compressed in `component-crash-signal-witness/`, explicitly excluded from fault
latency qualification. The bounded untraced regression is the accepted check.

Use fresh output directories when reproducing the final integration runs:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 4-7 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_mujoco.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-robot-fault-simulation/uwvm2-ros \
  --source-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/robot-fault-simulation-source \
  --mujoco ../uwvm2-ros-sdk/.tmp/ros-scene-security/mujoco-official-3.4.0/mujoco-3.4.0 \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/mujoco-qualification-repeat \
  --poll-ns 10000

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 4-7 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_component_crash.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-robot-fault-simulation/uwvm2-ros \
  --source-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/robot-fault-simulation-source \
  --component-receipt ../uwvm2-ros-sdk/.tmp/ros-scene-security/component-crash-fixed/results.json \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/component-crash-qualification-repeat
```
