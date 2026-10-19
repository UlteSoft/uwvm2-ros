# ARM ROS behavior under QEMU

`run.py` adapts a frozen source tree's existing ROS network and RPC runners to
ARM ELF64 Workers, Gateways and native witnesses. It does not edit those runners
or production sources. Missing SDKs, binaries or 64 GiB cgroup membership are
BLOCKED. The Worker remains the same host PID through the existing STOP/exec
sequence, preserving the Gateway's SO_PEERCRED binding.

The current original cgroup must have `memory.max=68719476736` (64 GiB),
`memory.swap.max=0` and effective CPUs `0-31`. Re-measure its live leaf path,
device/inode and boot for each invocation; historical 64 GiB reports keep their
original scope. Current SDK compiler/model defaults use a 2 GiB (`2048` MiB)
sampled RSS guard and 60 GiB (`61440` MiB) whole-cgroup current-memory guard.
Individual ARM fixture RSS/deadline limits remain documented in their guides.

Supply a verified ARM rootfs workspace with canonical `/tmp/ros1-build` and
`/tmp/ros2-build` native targets. Binfmt mode retains each original chroot ROS
setup command. Explicit mode launches QEMU directly and requires a version-keyed
JSON file of ROS setup variables; native programs that exec another ARM program
still require working binfmt. Binfmt setup is outside this runner.
Supply `--binfmt-evidence /arm/lease.json` from the verified task-owned temporary
lease. Its `name` or `entry_name` must identify a currently enabled fixed-interpreter
ELF183 entry. The runner reads and records the handler and never registers it.

`defaults.py` can run before SDK preparation and requires only the dual ARM
Worker, host QEMU, the frozen source tree, `wat2wasm` and the inherited cgroup:

```sh
python3 tests/ros_aarch64/defaults.py --source /frozen/source \
  --uwvm /arm/product/dual/uwvm \
  --worker-prefix-json '["/usr/bin/qemu-aarch64","-L","/arm/worker-sysroot"]' \
  --out /arm/evidence/defaults --wat2wasm /usr/bin/wat2wasm
```

It checks six ROS default configurations, both cap_open denials, every resource
API registered in the frozen dual Worker (currently ROS1 32 and ROS2 37), a
72-byte output sentinel and zero private IPC accepts. A stdin barrier holds the
actual ARM guest for mapping evidence before releasing it. It tests ROS default
rights; WASIp1 stdio-only defaults remain a separate requirement.

```sh
python3 tests/ros_aarch64/run.py --suite network \
  --source /frozen/source --workspace /arm/workspace --uwvm /arm/product/dual/uwvm \
  --worker-prefix-json '["/usr/bin/qemu-aarch64","-L","/arm/worker-sysroot"]' \
  --binfmt-evidence /arm/lease.json \
  --topic-observers-manifest /arm/observer-preparation/manifest.json \
  --out /arm/evidence/network --mode all

python3 tests/ros_aarch64/run.py --suite rpc \
  --source /frozen/source --workspace /arm/workspace --uwvm /arm/product/dual/uwvm \
  --worker-prefix-json '["/usr/bin/qemu-aarch64","-L","/arm/worker-sysroot"]' \
  --binfmt-evidence /arm/lease.json \
  --topic-observers-manifest /arm/observer-preparation/manifest.json \
  --out /arm/evidence/rpc --mode all --version both --wat2wasm /usr/bin/wat2wasm

python3 tests/ros_aarch64/run.py --suite multi-memory \
  --source /frozen/source --workspace /arm/workspace --uwvm /arm/product/dual/uwvm \
  --worker-prefix-json '["/usr/bin/qemu-aarch64","-L","/arm/worker-sysroot"]' \
  --binfmt-evidence /arm/lease.json \
  --topic-observers-manifest /arm/observer-preparation/manifest.json \
  --out /arm/evidence/multi-memory --wat2wasm /usr/bin/wat2wasm
```

The multi-memory suite compiles two new Wasm fixtures in its fresh output
directory. Each opens an alias in memory1, writes and reads an unaligned handle,
rejects memory2/overflow/end-of-memory ranges without changing a sentinel, and
publishes from memory1 twice. The first publish creates the endpoint; a private
stdin gate releases the second after actual native discovery. A C++17 observer
checks exact String content and records one or two native receives. This supplements the existing memory64
bounds and growth rows.
The delays use existing WASI clock authority; this suite makes no claim about
WASIp1 default stdio-only authority.

`build_topic_observers.py` prepares the two test-owned observers using the ARM
SDK compilers, explicit C++17, and actual GCC dependency output. It can rebuild
one version while verifying the other's original prepared inputs. Normal
network/RPC cases retain their original native probe and use a reader startup
check before resuming the Worker. ROS1 checks Master registration and the actual
node PID; ROS2 checks a new DDS GID and its reported identity/QoS alongside live
ARM process evidence. Fast DDS does not report remote history/depth; the helper
records that limitation and the separately declared local probe QoS.

`architecture-evidence.json` records the actual kernel `/proc/<pid>/exe` ELF
separately from mapped ARM targets. Under QEMU user the kernel ELF is normally
the host QEMU binary; it is never called an ARM kernel executable. A frozen
target's ARM ELF header and SHA alone do not count as execution: a live process
mapping must open `/proc/PID/map_files/START-END` while its owned birth,
PID/PPid, host executable, cgroup membership and live `cgroup.procs` remain
coherent. The opened descriptor must match the frozen target physical facts
(device, inode and size), full SHA256 and ELF architecture. Each actual
opened FD additionally records mtime and ctime and must stay stable through
hashing. A later pool hardlink can change ctime while preserving bytes: that
invalidates the retained hash cache and requires another full hash against
the original admitted target SHA256, rather than changing its identity. The
raw `/proc/maps` device/inode is display-only: filesystem mount devices can
differ from the actual opened file device. No conversion or raw-text fallback
is accepted. Missing access means unavailable evidence, never a positive.
`OPENED_PROC_MAP_FILES_V1` receipts are mandatory in current qualifiers; old
raw-only records cannot qualify a current report. Non-target DSOs are only
fstat-filtered, never fully hashed. Repeated samples may reuse the first
hashed target descriptor only while that FD and each fresh mapping FD retain
the complete physical identity. The sampler never stops a process. Stopped
barriers separately recheck birth, T state, host executable and the original
leaf immediately before the one selected pidfd CONT. The existing 68719476736-byte limit is read
only. All readable ancestor OOM counters must remain unchanged.

`build_worker.py` cross-compiles four independent C++26 interpreter profiles,
pins actual compiler dependencies, verifies ELF183 objects/executables and
executes each profile through QEMU. `build_gateways.py` builds all 18 original
C++17 native targets with the distribution's ARM compiler. Build and behavior
reports distinguish compilation from execution.

`run_unit.py`, `run_cli.py` and `run_regression.py` execute the original unit,
CLI and regression contracts on ARM. Regression's 36 commands include fixture
compilations; they represent 16 runtime launches and four ARM Trusted-DL plugin
loads. Generic DL fixture sources use a separately frozen addon manifest.
The complete entry now makes the six original native unit rows and all four
regression profiles mandatory, including for selected-suite invocations. See
[current mandatory stages and their limits](README.current-regressions.md).

`run_native.py` runs the original Topic, resource, TF, Lifecycle, Action, service
and SROS2 witnesses. `run_extended.py` adapts the seven additional security,
ingress, revocation, flood, payload, five-module and performance suites.
`run_resources.py` independently builds C++20 probes, then runs unchanged static
parameter initialization, eight Action byte-limit modes and seven Action
lifetime modes against the qualified C++17 Gateway.
Both preparation and execution require `--container-started-at` with the
current container's exact Docker `StartedAt` value. Reuse the prepared
`manifest.json` together with its original `report.json` and
`architecture-evidence.json`; preserve all three and their recorded compiler
logs, dependency files and ELF images. Reuse checks the complete source
manifest, SDK bytes and aliases, and the current kernel boot and cgroup
identity. A preparation from another incarnation must be rebuilt.

`run_all.py` launches the behavior suites in a private mount namespace with a
short workspace bind alias, keeping Unix socket paths within the kernel limit.
Supply the real SDK rootfs paths: the build driver's temporary workspace mounts
are removed after compilation. The optional executable pool makes separate
frozen copies of qualified compiler outputs, then deduplicates snapshots with
hardlinks to those copies. Source/pool SHA and device/inode checks reject
replacement or aliasing back to a compiler output.
When a bind mount prevents a hardlink with `EXDEV`, the pool makes and verifies
a separate copy and records the fallback.

QEMU startup polling has bounded outer envelopes: Master 30 seconds, Gateway
socket/observer startup 12 seconds. Each adjusted original call site and actual
elapsed time is recorded. Native operation, resource, Guest and protocol
deadlines retain their original assertions.

`rebuild_affected.py` records a fresh ROS1 or ROS2 Gateway compilation.
Both SDK versions request system-inclusive `-MD -MF` compilation output. The
driver reads the successful verbose GCC recipe and its final `-MF`, checks the
depfile's object target and recorded C++/ROS SDK headers, and archives that
actual file. Noetic's CMake 3.16 keeps the explicit origin path; Jazzy's CMake
may append its own `-MF` and the driver records that target `.o.d` instead.
The compile database and requested flags alone do not establish header coverage.
`replay_gateway_dependencies.py` captures system-header dependencies with the
original compiler flags and dependency-only `-M`, preserving the original
binary and metadata. `qualify_rebuild.py` can qualify the preserved successful
compilation whose `-MMD` system-header guard failed; the raw FAIL report remains.
`qualify_reuse.py` verifies the Action-close and late-service source changes, original native and
Worker compiler records, complete SDK file sets/bytes and root aliases, and
equivalent code generation/link options before publishing new task-owned views.
These qualification reports assert artifact provenance; ROS behavior has its
own reports.

`run_late_service.py` deliberately holds the original ROS1 native service
launcher until the Gateway is actually waiting and the Master reports the
resource absent. The original 3000 ms call budget stays unchanged. Completed
Worker audit rows are checked afterward because the runtime drains their
bounded audit queue when the call returns. Native execution may occur in the
held launcher or its proven direct child, depending on the host's chroot
implementation; Worker credential binding still requires its actual PID.

`run_rpc_denial_gate.py` adds a reversible post-assert stdin gate to only the
two typed-event denial Guests. Every original denial/sentinel assertion runs
first. The original native binary then completes its five-second zero-reader
observation before the host releases the Guest. This avoids interpreting a
known DDS GID losing its node association during Gateway shutdown as a newly
created reader. The original 20-second Guest, 15-second native wait and
8-second baseline budgets remain unchanged. The derived Wasm bytes are
identified separately from the original fixtures. `run_rpc_lifecycle.py`
preserves the unsuccessful earlier stop-order experiment; delaying an
explicit stop cannot keep a Gateway alive after its Worker pidfd exits.

Optional SDK environment caching captures the real ARM login-shell setup
before observers start. `run.py --cache-native-sdk-environment` supports the
network suite; `run_extended.py --cache-sdk-environment` supports performance.
Both require binfmt mode. Caller environment, setup inputs, resource limits,
native ELF and argv are checked, and actual native process environments are
recorded. Changed context falls back to the original SDK shell. Capture-only
ARM bash/Python evidence is checked separately from native launch evidence.
`run_all.py` exposes these options as `--network-cache-sdk-environment` and
`--performance-cache-sdk-environment`; `--rpc-denial-gate` selects the denial
synchronization adapter.

`run_dds_discovery.py` compiles a test-owned XML parser check, then runs the
unchanged ROS2 native probe with an explicit participant announcement profile.
It verifies that the profile changes only three discovery settings and records
the actual native process environment. Endpoint QoS and the original discovery
and operation assertions stay unchanged. A successful explicit-profile run
does not change the status of earlier failures with default discovery settings.

`binfmt_lease.py` registers one unique temporary ARM handler, runs a bounded
command and removes its own entry on exit. It verifies the original registry is
restored. The fixed interpreter flag supports the private chroot execution
sequence described in the [Linux kernel binfmt documentation](https://docs.kernel.org/admin-guide/binfmt-misc.html).
No persistent ARM execution registration is installed. Each container restart
is recorded as a distinct cgroup incarnation; memory counters are compared only
within that incarnation.

The original suite reports remain under the prepared workspace. ARM reports
add architecture/cgroup proof and references to those raw reports. A prepared
driver or successfully compiled probe does not count as executed ROS behavior.
Performance reports explicitly describe QEMU host simulation measurements,
without claiming physical ARM latency, throughput or memory-order validation.
# Current Action response and cancellation qualification

The current `run_action_qos.py` has four mandatory C++17 products, including the
two `tests/ros_qos/cancel_*` probes. Preparation format 2 pins their exact recipes,
current source manifest, complete SDK inputs, actual GCC dependencies and driver
helpers. Native compilation and execution require their own exact current-source
Linux reports; source/model checks are not architecture or ROS test results.

Full `run_all.py` invocations must provide `--prepared-action-qos` pointing to a
new four-probe preparation report and `--container-started-at` from the current
Docker provenance. The mandatory Action QoS stage uses a nested private mount
namespace and executes all four probes under the existing 64 GiB cgroup and
parent QEMU lease. The qualified pool contains exactly 30 product paths: four
fresh Workers, 18 original native products, four QoS probes, two ROS1 search/
feedback probes, and two ROS2 local parameter-event probes. Old two-probe
preparations, old product subsets and dependency-reused Workers cannot qualify
the current WASIp1 permission/default changes. See
[the cancellation execution guide](../ros_qos/cancel_README.md) for details.

The current entry also requires `--prepared-ros1-search`,
`--prepared-ros2-local-parameter-events`, `--ros1-gateway-report` and
`--ros2-gateway-report`. Both feature stages are mandatory even when the caller
selects supplementary suites. Their Gateway binaries must come from fresh
system-inclusive targeted compilations of the same complete source manifest.
Use [current native compilation qualification](README.current-native-qualification.md)
to assemble the 18-product view; historical reuse reports cannot qualify it.
The remaining 16 base probes retain their explicitly limited project `-MMD`
dependency scope. All executing Python controllers must be inside the frozen
`source/tests/ros_aarch64` tree and appear in that source manifest.

See [ROS1 preparation and execution](README.ros1-search.md) and
[ROS2 local parameter-event preparation and execution](local-parameter-events.md)
for exact fixture contracts, SDK closure requirements and commands. The two
middleware-free ROS1 contracts are additionally compiled by the mandatory
`ros1-search-contracts` unit stage with explicit C++20 and C++17 standards.
Feature behavior admission binds the raw completion logs, stopped PID/birth,
owned ancestry, live cgroup membership, host QEMU identity and mapped product
device/inode to the same nested architecture report. Parent acceptance checks
these reports and product identities again before returning its final verdict.
Current Worker reports must retain all three original object identities per
profile, the executing artifact helper, one cgroup incarnation, zero swap and
independently recomputed zero OOM deltas. These are acceptance requirements;
source and synthetic models do not establish actual ARM or ROS success.

