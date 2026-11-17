# External driver attachment qualification

This record qualifies the attachment-only boundary described in
[the protocol contract](ros-driver-attachment.md). It does not qualify a
deployment service, arbitrary vendor adapters, all ROS packages, or industrial
safety certification. The current implementation has 163 registered ROSv2
operations mapped to 26 capability domains; registration is not evidence that
every provider or robot configuration has been implemented and qualified.

## Qualification identity

All evidence is retained under
`../uwvm2-ros-sdk/.tmp/ros-scene-security/`. The final consolidated receipt is
`driver-attachment-combined-final/results.json`. It binds each result to its
actual binary SHA256, frozen source manifest, fixture hashes and command lines.
The corrected VM build is `vm-driver-attachment-authority/uwvm2-ros`, from
`driver-attachment-authority-source`. Do not substitute an older build merely
because its executable has the same name.
Its SHA256 is `25165df0736898049ba0476aee7b2a264535e030f0ded37d2d951f65f9e5de59`.
The separately compiled receiver helper is not included in that VM's 4,257-file
snapshot; its source/fixture hashes and native executable are recorded under
`driver-receiver-shared-control-final/results.json`.

The preceding `vm-driver-attachment-final` build exposed an admission-order
regression: an explicitly denied driver import returned an invalid-argument
error for poisoned memory fields. The corrected carrier checks authority before
memory normalization or copying, bounds the byte sum without overflow, and
retains the charged authority recheck before effects. Its all-import regression
requires EACCES and unchanged request/result/length canaries for both ordinary
and poisoned calls, including denied preloads called by an authorized parent.
The failed receipt remains in `driver-attachment-final-import-denials`.

All builds, tests, simulations, profilers and child processes execute through
`tools/ros_scene/in_existing_cgroup.py`. It joins the existing group
`/sys/fs/cgroup/system.slice/docker-af71cb24aa7993383f76ad9c06750b17eb66fd090a2c9ba64b544b19cda9c913.scope`
and verifies `memory.max=68719476736` and `memory.swap.max=0`. No replacement
budget, raised limit or separately unbounded test process is permitted.
Compilation uses `-O3 -g`; the VM build checks `.debug_info` and `.debug_line`.

## Reproducible checks

`tools/ros_scene/driver_attachment_checks.py` runs serialized checks against
one frozen VM. Invoke it through the resource gate with `--runtime`, `--output`,
`--mujoco` and `--named-modules`. Its `complete` field describes this bounded
suite, never ecosystem or industrial completion. The suite includes:

- Address/undefined-behavior sanitizers for sessions, exact module authority and
  platform gates; malformed replies, identity/sequence/deadline mismatch,
  descriptor transfer, dead peers, duplicate registration, failed delivery,
  revocation, attenuation, and side-effect-free inspection.
- Recoverable fast_io allocation failure injection and descriptor accounting;
  secure overwrite before release remains tested with optimization enabled.
- Actual driver imports with memory32/memory64, memory index zero/one, and
  interpreter/AOT: valid calls, denied capabilities, malformed schemas, wrong
  identity, wrong sequence/deadline, truncation, oversize and native crash.
- Two actual VM processes sharing one external process over distinct channels.
  The first session releases before the second receives readiness; this proves
  independent connection lifetime, not concurrent actuator arbitration.
- CLI validation without handshake or guest/native effects, including missing
  budgets/schema, malformed identities, invalid socket type/credentials and
  refusal of removed component imports/options.
- Every registered import's deny/hidden behavior, module-permission/exchange
  tests, actual C++ debugger rows, DAP parsing and editor lifecycle regressions.
- Catalogue/schema consistency and separate named-module LLVM/interpreter
  checks. Named-module receipts cover the scene/storage/debug modules and
  isolated host exports; they do not claim a full frontend module-graph build.

`driver-attachment-authority-checked/results.json` records 697 passing cases:
72 driver VM, 21 CLI, 4 shared-process, 82 module, 342 all-import refusals,
24 ordinary MuJoCo, 24 namespace MuJoCo and 128 directional device/sensor/control.
The 342 refusals cover all 163 registered operations, including poisoned memory
fields. These counts are test cases, not unique APIs or audited ROS packages.
Sanitizer, allocation, debugger and named-module checks are additional evidence.

## Official MuJoCo simulation

The pinned upstream MuJoCo 3.4.0 archive, provenance record, unmodified humanoid
model and native library hashes are retained. The registered-driver fixture
executes actual `mj_step` outside the VM. It validates the fixed canonical
`device.submit` shape, originating process, contract binding, controller epoch,
units/frame selectors, finite target range and deadline before a physics step.

The success case executes 128 alternating targets and compares final position,
simulation time and step count exactly with a direct native run. Out-of-range
commands produce zero steps. A deliberate segmentation fault after a real step
leaves the VM alive and the session failed with an unknown effect. That is
failure containment, not a stopped-robot claim.

The ordinary and namespace variants each exercise 24 cases. The namespace
variant is test-only external provisioning: separate user, mount, network, IPC
and UTS namespaces; shared PID/time namespaces; a sealed read-only executable,
library and model closure; dropped capabilities; bounded process limits; and
stacked syscall filters. The startup filter specializes only its approved
receive descriptor; the final filter forbids new execution/executable mappings.
Each receiver attempts seven forbidden actions before accepting application
traffic: network socket creation, ptrace, clone, file creation, host filesystem
access, executable mapping and resource-limit modification.

This topology preserves exact credentials in both directions. It does not
qualify arbitrary sibling PID namespaces, time offsets, relays, kernel-device
access or vendor-specific syscall profiles. VM and driver remain in the same
aggregate test cgroup. Separate deployment-controlled per-driver cgroups were
not created by these tests.

A separate 128-case device/sensor/control suite uses fresh PID namespaces and
the existing directional device carrier. It covers crash, hang, corrupt/stale
responses, VM crash/pause, permission/rate refusals and an independent stop
channel. Its results must not be represented as proof that the newer attachment
carrier implements all of those device supervision semantics.

### Concurrent shared ownership

The external `driver_control.h` helper is exercised by two actual VM processes
against one original MuJoCo receiver. Eight combinations cover memory32/memory64,
memory index zero/one and interpreter/AOT. Both clients submit before each
receiver decision. The receiver rejects a competing owner, a stale pre-handover
epoch and a nonowner copying the current epoch. Three authorized steps exactly
match direct native simulation time/position/count; the three rejected commands
cause no physics step. Releasing the old client leaves the new owner functional.
The active owner's release reserves a stop slot, without claiming physical stop.

Seven ASan/UBSan security groups additionally cover default denial, exact origin,
credentials, replay, lease expiry, concurrent admission, in-flight handover
refusal, unknown effects, independent reconciliation and fork-copy refusal.
Injected fast_io allocation failure returns `no_memory`, preserves the output
handle and permits a clean registration retry. These eight additional VM cases
bring the bounded combined count to 705. Vendor integration, independent hardware
stop and durable recovery remain separate acceptance work.

The same seven receiver-control groups also execute successfully on x86_64,
i686, AArch64, ARM soft-float and ARM hard-float, using the retained sysroots,
target-matched libc++/ABI archives and ARM QEMU loaders.
`driver-receiver-five-abi/results.json` retains source/dependency hashes, ELF
target checks and each compiler/run command. These are control-core correctness
tests, not full five-ABI VM/IPC/native-driver or target performance qualification.

## Performance and hardware sampling

Matched trials compare an actual Wasm caller with a native C++ caller using the
same dispatcher, schema checks, explicit authority, secure buffer erasure and
separate receiver process. The workload is a 17-byte request and a 24-byte
response, with 100,000 round trips per trial and alternating native/Wasm order.
Receiver timing excludes startup and JIT compilation. Reported medians are
medians of batch means; they are not per-call P99/max or hard-real-time bounds.
They must not be presented as eight-byte direct-function-call performance.

The corrected executable's 22 alternating trials measured median batch means
of 3,151.200 ns native and 3,629.603 ns Wasm (1.151816x, a 478.403 ns difference).
`driver-attachment-authority-performance/trials/results.json` retains all trials.
This result qualifies only the specified IPC workload. It is not a near-native
guarantee for every ROS interface, callback or robot.

The receiver ownership gate has a separate non-IPC microbenchmark. Native
arbitration begin/finish measured 129.361 ns; the full registered receiver gate
measured 255.134 ns, adding 125.773 ns for origin, credential, sequence, lease and
global admission checks. Eleven alternating pairs of million-call batches use
`-O3 -g -UNDEBUG`, CPU 2 and fixed synthetic authority time. These numbers are
not physical device latency or per-call tail latency. Assembly review removed
one redundant locked status snapshot while retaining final arbiter admission;
the preceding receiver cohort measured 318.209 ns. The final sanitizer,
allocation-failure and two-VM simulation suites were rerun after that change.
`driver-receiver-performance-final/results.json` retains commands, source hashes
and disassembly; this local comparison is separate from the IPC comparison.

The explicit hardware path samples user-mode `CPU_CLK_UNHALTED.REF_TSC` at
24,000 counts and `INST_RETIRED.ANY` at 240,000 counts, selects the actual client
PID, audits event attributes/sample periods/loss/throttle records, and imports
the trace into VTune for source and assembly reports. Reports also filter by the
actual client PID, excluding the native receiver and harness. The corrected
receipt is `driver-attachment-authority-hardware-client/results.json`; both
clients have verified programmed/sample periods, source/assembly mappings and
zero loss/throttle records. A 2,419,200,300 Hz reference
corresponds to a nominal 9.9206 microsecond reference-event period. Scheduling,
privilege exclusion and interrupt handling make wall-clock sample spacing
different; it is not a guaranteed 0.01 ms wall-clock sampling interval.

Native VTune Hotspots/Microarchitecture Exploration presets failed to generate
a hardware event configuration in the retained attempt. The explicit hardware
import provides source/assembly evidence but does not establish complete
top-down microarchitecture analysis. No failed preset is relabelled successful.
The corrected executable was also retried with the top-level-only preset;
`driver-attachment-microarch-retry/results.json` records exit 1 with the same
event-configuration failure. Installed SEP/PAX/VTSSPP devices were confirmed
loaded for the current user group. No additional privilege or kernel setting
was changed, and the successful explicit-period samples remain separate.

## Outstanding acceptance boundaries

The deployment program is deliberately outside this change. Its own process
creation, cgroup/namespace setup, scheduling/NUMA policy, update/recovery and
durability need separate implementation and acceptance. The VM never obtains
those powers through an indirect lifecycle RPC. In addition:

- Existing VM-managed model/DDS worker providers have not all migrated to
  external provisioning. Their APIs must not be described as attachment-only.
- Generic vendor executable/library adapters and all supported Linux ABIs need
  independent qualification against their exact original binaries. No native
  vendor library is loaded into the VM by this carrier.
- The shared receiver arbiter and bounded multi-VM simulation are implemented.
  Each real vendor path must integrate that same ownership authority, with
  independently qualified physical handover and no alternate actuator path.
- Receiver-side stop reservations, an independent physical watchdog, power-loss
  recovery and reconciliation of unknown effects require machine-specific
  evidence. A reply, disconnect, timeout or process exit does not prove safety.
- Contract digests bind sessions but do not authenticate deployment manifests
  or prove confinement. Whole execution-domain attestation is not implemented.
- The November 13 ecosystem reconciliation observed 9,432 package names,
  8,554 indexed entries and a 9,999-name union; it still records incomplete
  source acquisition and pending exhaustive semantic review. These are
  multi-distribution/community inventories, not 9,000 independently audited
  official libraries. Full package portability remains unproven.

The corresponding machine-readable design flags remain `providers_complete`,
`native_equivalence_proven`, `package_portability_proven` and
`industrial_assurance_proven`: **false**.
