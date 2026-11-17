# Local managed lifecycle qualification

The `uwvm_ros_v2` lifecycle provider represents one logical managed node per
explicitly configured alias. It preserves the public ROS transition/state and
callback-result numbers without creating a native ROS node, calling a native
callback or granting process/device authority. Guest code pulls a deferred
callback record, performs separately authorized work, then responds with the
exact request identity and state generation. A logical finalized state is not
a physical robot safe-stop.

`test/0084.ros-runtime/run_scene_lifecycle.py` exercises all six imports through
the real VM carrier. It generates memory32 and memory64 guests at memory index
zero and one and runs each through the interpreter and LLVM AOT. The guests
import only `uwvm_ros_v2`; deadlines come from an explicitly granted
`clock.steady` handle. No WASI, filesystem or socket authorization is needed.

Deployment starts with no exposure and no grant. Each test alias receives an
exact `lifecycle` grant with `local-lifecycle` binding, then the finite plan:

```text
--ros-scene-lifecycle ENV ALIAS HANDLES DEADLINE_NS TRANSITIONS_MASK ROLES_MASK
```

The deadline bound limits the interval between admission and the supplied
absolute steady deadline. It is not an implicit wait or infinite timeout.
Transition mask bit N permits public transition N; only bits 1 through 7 are
valid. Roles are client=1 and server=2. A guest opens exactly one role per
handle. Zero handles, roles, transition mask or deadline provide no allowance
in the corresponding dimension. Capacity is separate from optional broker rate
attenuation, and resource/capability reopening does not reset the logical
state, generation or request history.

The successful guest includes 501 named assertions. It covers every public
transition, including shutdown from unconfigured, inactive and active states;
configure, activate, deactivate and cleanup; callback failure; error processing
and successful recovery; submission and response replay; conflicting, skipped
and stale requests; role misuse; capacity exhaustion; exact alias ownership;
state generation changes; and handle close/reopen without state reset.
Undelivered callbacks cannot be acknowledged. Short result spans are rejected
before mutation or callback consumption, with the exact required byte count.
Invalid memory indices, overflowed/out-of-bounds output locations and physical
length-output aliasing are rejected as well.

Separate aliases qualify zero roles, zero handle capacity, a zero grant resource
limit, client-only access, an empty transition mask and zero deadline bound.
Two expiry cases use a 50 ms deadline, waiting via bounded `clock.read` calls:
a late ordinary response and a late error-phase callback take. Both finalize
the logical node with timeout status, and stale responses cannot revive it.
The test runner also enforces an external process timeout.

Each memory layout has explicit interface-denied and no-grants guests. Their
six lifecycle operations must return the appropriate refusal without implying
authority from the imported ABI alone. CLI cases reject missing and duplicate
plans, plans before their grant, unknown aliases, unlimited/invalid capacities,
excessive deadlines, invalid transition/role masks and wrong purpose, binding
or action. Default-hidden and static validation/section-detail paths are
included in the actual VM run.

Prepare first, then execute against the completed VM:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_lifecycle.py --mode prepare \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-vm-prepared

/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 8-15 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_lifecycle.py --mode execute \
  --runtime /absolute/path/to/qualified/uwvm2-ros \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-vm-prepared
```

The receipt pins the generator, shared guest builder, exact wire contract,
generated WAT/Wasm files and executed VM binary. `preparation_complete=true`
records successful generation/compilation only. `complete=true` requires all
actual VM and CLI cases to pass. Every compiler and VM invocation runs inside
the existing 64 GiB zero-swap cgroup; no replacement cgroup or unbounded runner
is used. These fixtures qualify the local lifecycle ABI and safety behavior,
not complete `rclcpp_lifecycle` package portability, native callback execution,
DDS discovery, a device watchdog or native-performance parity.

The completed local run is preserved at
`../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-vm-prepared/results.json`.
All **44 actual VM/CLI cases passed** against the complete O3, three-translation-
unit safe VM with SHA-256
`92354c74047cc872e6b4c6161beafcf0c49f65e2168cbd454a9f8a4743ae0121`.
The receipt has `complete=true`; all four memory layouts passed both engines,
including the default-denied and no-grants guests. The executable is tied to
`vm-lifecycle-safe/full-build-results.json` and its 2641-file frozen source
manifest. A concurrent shared-allocator migration in the live workspace is
recorded as drift in that build receipt and is not covered by this executable.
The receipt spans preparation and execution; shared cgroup `memory.events max`
changed from 0 to 683 over that interval, while `oom`, `oom_kill` and
`oom_group_kill` remained zero. The memory/swap limits were never changed.

The actual debugger run is preserved at
`../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-dap/results.json`.
All three debugger-permission cases (`full`, `read-only`, `limited`) completed
against the same safe VM. The full/read-only views contain 11 lifecycle
bindings covering the ten primary/intermediate state values and an expired
pending callback, plus the steady clock row. Repeated and paginated inspection
preserves the exact state, generation, callback-delivery flags and admission
counters. In particular, inspecting an elapsed deadline does not expire or
consume the pending callback. Limited permission refuses scene metadata; a
request asserting a retired full-debug stop is rejected. The closed DTO omits
request identities, retry keys, payloads, native pointers and filesystem paths.

The lifecycle-specific carrier run is preserved at
`../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-boundaries-safe/results.json`.
All **184 actual VM cases passed** from 92 generated Wasm fixtures, with the
same safe executable and stable frozen-production/source-fixture pins. Every
lifecycle import is tested for request, result and length-slot tag faults,
independently across the two engines and four memory layouts. Diagnostic-mode
physical aliases must be rejected before effects; the subsequent valid
open/submit/take/respond sequence must still succeed. Short buffers preserve
canaries and report exact required sizes. The FP cases perform 12 dynamic
arithmetic checks at each return site, including success, short result,
submission/response replay, hidden-interface refusal and missing grants.
These tests exercise software memory tags; they do not qualify hardware MTE,
native callback execution or a physical stop mechanism.

The frozen-source qualification can be repeated into a fresh output directory:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py taskset -c 6-7 \
  /usr/bin/python3 test/0084.ros-runtime/run_scene_lifecycle_boundaries.py \
  --runtime ../uwvm2-ros-sdk/.tmp/ros-scene-security/vm-lifecycle-safe/uwvm2-ros \
  --source-root ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-source-safe \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-boundaries-repeat
```

The scheduling carrier's software memory-tag regression was repeated with
production-header pins resolved from the frozen build tree:
`../uwvm2-ros-sdk/.tmp/ros-scene-security/lifecycle-scheduling-memtag-safe/results.json`.
All **40 actual VM cases passed**, covering both engines, memory32/64, memory
index zero/one, matching tags, normalized physical aliases and independent
request/output/length tag faults. The earlier `lifecycle-scheduling-memtag`
receipt remains incomplete because a live workspace source changed during
that run, despite all its execution cases passing; it is not used as the
completed source qualification. The repeated run used the existing configured
cgroup path with `memory.max=68719476736` and `memory.swap.max=0`. Its recorded
inode is 12409, while the earlier runs recorded 12495; these separate cgroup
observations must not be collapsed into one cumulative event counter.
