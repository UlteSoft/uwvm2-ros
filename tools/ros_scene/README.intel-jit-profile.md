# Optional Intel JIT profiling: implementation boundary and integration design

The independent registration helper and MCJIT listener are **not connected to
the VM runtime yet**. Their tests and controlled executable-code probe do not
establish Wasm guest source mapping or ROS native performance parity. The
ordinary VM build, LLVM build, existing WASIp1/ROS interfaces, debugger grants,
and release performance baseline are unchanged by these new files.

## Configuration and security boundary

`uwvm_runtime_intel_jit_profile.h` compiles without Intel headers or libraries
unless `UWVM_RUNTIME_INTEL_JIT_PROFILE=1` is deliberately defined. Enabled builds
use the official `jitprofiling.h` and `libjitprofiling.a` supplied by the host.
No copied vendor ABI or replacement collector is embedded in production code.
Tests substitute the SDK boundary to record its calls; the real probe links the
official static library.

Build support does not activate profiling. Both registry and listener default
to disabled. Disabled construction, reporting and destruction make no SDK call,
read no environment variable, load no library and read no code bytes. A trusted
host must opt in before code materialization. When explicitly enabled, Intel's
static SDK itself reads `INTEL_JIT_PROFILER64` and loads that collector with
`dlopen` on Linux. Therefore opt-in and that environment belong to the trusted
deployment launcher, never to guest WASI environment overrides, ROS capabilities,
model output or an unauthenticated debugger request. Static linkage alone is
not a no-dynamic-loading security guarantee.

Source labels are bounded display metadata. The helper never calls a filesystem
API to open a source label, resolve a path, or retrieve split/network debug data.
The external profiling operator separately controls which host source files its
VTune viewer may display. Future source remapping must use an explicit trusted
manifest; guest DWARF paths cannot become a VM host-read capability.

Every SDK call is serialized under one component-wide mutex, including active
checks, method IDs, load and unload. Intel's current SDK counter is non-atomic.
The final nonzero ID is allowed once, then exhaustion becomes permanent; no wrap
or reuse is permitted. Embedders sharing this SDK must route profiling through
this component, rather than calling the SDK independently in parallel.

The first trusted activation binds the SDK component to the launching Linux
process before workers or guests can fork. Inherited registries report
unavailable in a fork child before acquiring the SDK mutex or allocating
metadata. Child listener detachment drops its local engine borrow without
calling inherited engine or SDK state. Exec starts fresh state. A real fork
negative control holds the parent SDK mutex across child report, retirement,
new activation and destruction; the child must finish without any SDK event.
Other operating systems currently fail closed until their process identity
boundary is qualified. This does not infer parent identity if the first-ever
trusted activation itself is deliberately performed in an unbound fork child.

## Metadata and lifetime rules

`registry` accepts actual owner-derived addresses and extents. It rejects zero
or overflowing ranges, overlapping live ranges in the same owner, oversized or
control-containing labels, and malformed source tables. The maximum is 65,536
live methods per registry, 65,536 rows per method and 4,096 bytes per label.
Allocation finishes before the synchronous load notification. The collector
copies labels/rows before returning; no caller string borrow is retained.

Intel `LineNumberInfo.Offset` means the **end** of a range. A valid table has
strictly increasing end offsets and ends at the actual method size. Source line
zero means unknown. A method with multiple original source files must be split
into nonoverlapping regions with distinct IDs/source labels, or explicitly omit
the source table. It must never assign all inlined code to its first source file.

Notify load before first execution. After all execution has drained, notify
unload before releasing or reusing executable storage. Reuse of the same address
receives a fresh identity. Failed load notifications are cleaned up. A registry
never sends global shutdown, which could invalidate other live code owners.

`listener` obtains function extents from actual object symbols and allocated
text sections, with a 64 MiB object limit and 4,096 object limit. MCJIT calls
`notifyObjectLoaded` **before** `resolveRelocations`, so that callback now only
copies bounded metadata into pending storage. The actual code owner must call
`publish_finalized()` after successful `finalizeObject` and its memory-manager
permission checks, before execution. Intel's LOAD event then observes finalized
bytes. Calling the object-loaded callback is not evidence of this boundary.
The listener currently reports symbols only. It skips non-text descriptor symbols; this does not claim
qualification of PPC descriptor ABIs or other untested targets. A malformed or
over-limit object has its partial registrations removed and sets an observation
failure. Profiling data never grants execution or debugger authority.

Temporary loader retirement and executable arena retirement are different
events. `detach_keep_code()` unregisters only the listener; its registrations
remain until `retire_code()` or destruction after actual execution drain. The
engine must outlive an attached listener. Caller ownership/order is part of the
internal contract; arbitrary guest addresses are never accepted.

`make_listener_owner` uses the recoverable strict fast_io typed allocator for
the listener itself. Its pointer/deleter carrier performs no allocation.
Allocation or constructor failure returns an empty owner; real allocation
failure injection checks that no SDK activation occurs first. The owner must
be declared after the engine, and reset before any explicit engine reset.

## Restricted frozen VM qualification

`prepare_intel_profile_vm.py` accepts one pinned complete source manifest and
applies a finite set of unique literal replacements to a fresh SDK `.tmp`
copy. It never modifies the live runtime. `build_frozen_vm.py --intel-jit-sdk`
requires that qualification receipt and explicitly adds both trusted build
macros and the official SDK. Compilation remains full `-O3 -g`; ordinary
builds receive no profiling flags or library.

This variant restricts execution to ordinary full LLVM, max policy, zero
additional compile workers and disabled object cache. Interpreter, debugger,
checkpoint, replacement and threaded-child routes are outside this variant's
scope. The observer is attached before materialization, publishes only after
every entry address and memory-manager finalization check, then moves with
the engine into the actual full publication before entry release-stores.
Its member order retires notifications before executable storage. Terminal
retained-code paths detach the engine borrow and retain registrations with
the intentionally retained publication; they must not emit an early UNLOAD.

`qualify_intel_profile_vm.py` compares the actual XML program with pinned
native output, tests denied capabilities and unsupported variant settings,
and links a real runtime embedding to exercise reset/reload and terminal
retention. Its explicit test collector, loaded through the official SDK,
hashes executable bytes synchronously on LOAD and UNLOAD. These tests are
separate from official VTune collection. Neither the variant nor this driver
supplies original guest source rows or establishes production integration.

The current frozen variant is **not an industrial recoverable startup path**.
Its observer returns a recoverable failure and clears registrations, but the
existing full materialization caller treats any false result as fatal. The
real collector-refusal negative control therefore exits with SIGILL before
guest execution. For production integration, return a bounded structured
startup error through cold compilation, keep generated entry/bridge admission
closed, unwind temporary publication owners, and return a diagnostic CLI error
after the execution/publication guards have released. Calling public reset
while still holding those guards can self-wait; silently continuing with a
partially qualified publication is also invalid. This limitation is separate
from the allocator factory's tested non-terminating failure return.

## Planned runtime insertion points

These are documented insertion points, not changes made to the runtime:

1. Add optional build discovery for the official SDK. Do not rebuild LLVM merely
   to toggle `LLVM_USE_INTEL_JITEVENTS`. Add a trusted launch option such as
   `--runtime-jit-profile intel` with default `off`; reject an unsupported request
   clearly. Keep state immutable through materialization and child compilation.
2. Ordinary full LLVM materialization in `uwvm_runtime.default.cpp`: create the
   optional observer before `addObjectFile`/`finalizeObject`, alongside the current
   `pending_llvm_jit_code_ranges` construction. Adopt it with the actual complete
   `full_code_publication`, not a global numeric-PC map. Call `publish_finalized`
   only after relocation and the actual memory-manager finalization check.
   Declare the owner after
   `engine` so reverse destruction unloads before the engine releases its code.
3. Replacement materialization: retain the observer in
   `llvm_jit_debug_replace_transaction` after its actual engine member. Aborted
   preparation retires private registrations; successful publication transfers
   the same owner with the engine, before any new entry becomes executable.
4. `ThreadedLlvmChild`: the temporary MCJIT engine is intentionally destroyed
   after finalization while `native_arena_` still owns executable bytes. Detach
   the observer before dropping the engine, retain registrations with the arena,
   and retire only after group drain. Charging retained metadata to the child
   storage budget must be explicit; optional profiling cannot bypass budgets.
5. Runtime reset, module replacement, cache hits, parallel object emission and
   process-exit leak paths all need explicit lifetime qualification. A path that
   intentionally retains executable code must not retain a dangling engine
   borrow or incorrectly unload it while it can still execute. Lazy/light paths
   must be listed as unsupported until separately wired and tested.

The existing pending-range listener detaches at commit and therefore is not an
adequate owner for profiling unload notifications. Existing debug native-owner
tables and code-inspection permissions are separate and must not be repurposed.

## Original guest source mapping: required next step

Current `native_provenance.h` uses a synthetic native filename and encodes
**Wasm expression byte offset plus one** in the line field. It is attached for
the explicit debug path, reserves frame pointers on x86 and has other debugger
code-generation requirements. Passing those values to VTune as C/C++ lines would
be false; turning on the entire debugger also changes the benchmark execution.

The profile path needs separate metadata-only compiler support:

1. Parse bounded embedded guest `.debug_line`, `.debug_line_str` and `.debug_str`
   bytes using the existing read-only line parser, with finite resource limits
   and ambiguous intervals left unmapped. Do not invoke its adjacent sidecar
   loading path. Compilation-unit directory data is metadata, not a read grant.
2. Convert validated function expression offsets to Code-section-relative
   offsets and then to original source coordinates. Keep source identity,
   function/generation and module identity explicit. Missing, malformed or
   conflicting data yields an unknown interval, never a guessed nearest line.
3. Attach profiling-only LLVM `DIFile`/`DISubprogram`/`DILocation` metadata before
   optimization, without safe points, debugger hooks, checkpoints, frame-pointer
   reservations or weaker optimization. Include the profile-metadata policy and
   source identity in native object cache identity; do not reuse incompatible
   cached native objects. Parallel emission must preserve the same metadata.
4. Decode the actual relocated native object's line ranges. Carry unknown gaps
   and source-file changes correctly into Intel end-offset tables. Do not fetch
   external DWARF or infer native locations from an IR address/name. Inline
   identity needs either genuine inline events or conservative disjoint source
   chunks; unsupported optimized intervals stay unknown.
5. Validate known guest C/C++ lines through real Wasm compilation, both cold and
   cached materialization, parallel objects, child arenas and replacement. VTune
   reports must show the registered guest symbol AND expected original source
   lines. Symbol-only or host C++ source reports do not satisfy that acceptance.

## Reproduction and evidence

All compilation, execution, sanitizer checks and VTune collection must enter the
existing 64 GiB, zero-swap cgroup using the persistent gate wrapper:

```sh
/usr/bin/python3 tools/ros_scene/in_existing_cgroup.py /usr/bin/python3 \
  test/0084.ros-runtime/run_intel_jit_profile.py \
  --output ../uwvm2-ros-sdk/.tmp/ros-scene-security/intel-jit-profile
```

The driver records compiler version, SDK/source hashes, commands, stdout/stderr and cgroup
events. `--profile` additionally runs the official hardware-hotspots and
microarchitecture collectors on controlled RX generated native code and exports
symbol/source reports. Microarchitecture collection uses detailed sampling;
summary-only PMU counting does not activate this SDK's sampling collector and
cannot qualify JIT symbol/source attribution. The manually mapped probe source is only the probe's
source, never presented as VM guest DWARF qualification. Run collection serially
with other performance work; do not compete for the baseline CPU.

The recorded local acceptance is
`../uwvm2-ros-sdk/.tmp/ros-scene-security/intel-jit-profile-complete/results.json`:
23 commands passed inside the original cgroup (inode 12495), with all memory
event counters zero before and after. Hardware Hotspots sampled source lines
6, 8, 9, 10 and 11; detailed microarchitecture sampling additionally sampled
line 13. Both reports distinguish the two real method lifetimes sharing the
same code address. Clang ASan/UBSan, GCC 13, the real independent LLVM 23 MCJIT
fixture, default-off sentinel and its positive control all passed.

The three optional test fixtures use `.cpp` so xmake's general `test/**.cc`
discovery does not accidentally require the Intel SDK in ordinary builds.
`path-migration.json` alongside that receipt verifies their contents remained
byte-identical after this filename-only packaging change, and that the driver
changed only those three filenames. Failed preliminary reports are retained in
the sibling attempt directories; the complete receipt is the acceptance record.

### Finalized-code notification regression

`intel-jit-finalized-first/results.json` records fifteen successful commands
for the revised independent helper, with all C/C++ variants now using full
`-O3 -g`. The actual MCJIT fixture calls an explicitly mapped external native
function. Its object-loaded callback captures genuine text bytes before
relocation; those bytes differ from the final executable bytes. No Intel LOAD
is emitted at that early point. After explicit finalized publication, the
recorded LOAD bytes exactly equal the final executable range and the function
executes correctly. Repeated publication emits no duplicate LOAD; detached
code retains its registration until explicit retirement. Default-off and real
SDK collector-loading sentinel controls also pass.

This is a helper lifecycle/assembly-integrity regression, not VM integration
or guest source attribution. It does not replace the original controlled
hardware-sampling receipt, and no additional PMU collection was run during
the simultaneous frozen-VM performance qualification.
