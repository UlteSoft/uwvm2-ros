# Artifact floating-point boundary

`artifact.open`, `artifact.read` and `artifact.describe` retain an explicit
floating-point control guard. On supported LLVM builds their concrete scene
wrappers save and restore the native control registers instead of asking the
outer callback dispatcher to call `fegetenv` and `fesetenv`. The guard starts
before caller identification or allocation and ends after writing the result
tuple. It therefore covers successful operations, refusals, allocation failures,
cancellation, partial memory-copy failures and failed result delivery.

The eligible paths are closed: open and describe use deployment-pinned
metadata, while read copies the private volatile cache or reads a concrete
sealed-memfd snapshot with `pread`. A guest cannot choose an extensible native
provider, a host path or lazy input provisioning through these operations.
Owning allocations use the recoverable fast_io allocator; allocation failure
returns the existing error and keeps unpublished state private. Allocator
interposition may still alter control registers, so integer-only code and the
allocator choice do not justify removing the guard.

The existing native guard covers x87 precision, rounding and exception masks
and the SSE MXCSR control fields on x86; FPCR on supported AArch64 targets;
and the floating-point rounding register on supported RISC-V targets. Unknown
architectures and interpreter-only builds retain the complete outer boundary.
Wasm cannot inspect accrued exception flags; public execution entry and exit
still preserve the embedding application's complete floating-point environment.
This callback optimization does not weaken that public contract.

Imported-memory descriptors, reads and writes keep their independent runtime
guards. A third-party memory provider must not leak rounding or flush controls
into the remainder of the artifact callback. Generated-only bridge capabilities
remain suspended during native callbacks. Bounds, memory tags, normalized
overlap checks, authority, cancellation and retirement checks are unchanged.

The separate two-phase `artifact.publish` path remains outside this whitelist.
Its final authorization callback and rollback flow require separate boundary
qualification. This is a conservative qualification boundary, not a claim that
volatile publication performs native filesystem I/O. New operations never
inherit the whitelist automatically.

`artifact-fp-core-first/results.json` records seven successful commands against
the retained `artifact-fp-source` cohort: the policy fixture and full `-O3 -g`
ASan/UBSan and native builds of `artifact_wrapper_fp.cpp`. Each executable runs
48 control profiles, 1,632 hostile callbacks and 336 fast_io allocation probes.
The fixture calls the actual wrapper and real sealed-input/cache providers;
its runtime identity and memory callbacks are controlled test implementations.
They deliberately poison controls without an inner guard to test the outer
wrapper on successful and failed memory access. This receipt does not by itself
qualify actual imported-memory runtime callbacks or Wasm execution.

`artifact-memory-guard-first/results.json` separately records three successful
commands for the existing `llvm_jit_local_imported_provider_callback_guard.cc`
fixture, linked against the frozen `vm-transform-central` runtime and host API
objects. Its actual generated Wasm retains nearest rounding and gradual
underflow after hostile memory/global/signature callbacks. The embedding
thread's controls are restored, and a child process verifies that a native
provider cannot reuse the generated-only raw bridge. This independently checks
the runtime boundary retained by the artifact optimization.

All qualification commands run in the existing 64 GiB, zero-swap cgroup.
Performance evidence must use the same source/binary cohort, full `-O3 -g`,
VTune source attribution for hotspots and instruction attribution for
microarchitecture results. Removing one measured boundary cost does not prove
native parity or explain unregistered JIT sample addresses.

The retained `vm-artifact-fp/uwvm2-ros` binary has SHA256
`c0f1a858f76219595554f21e53802e7dfdd28204f8d0d93d440734abb7934860`.
`artifact-fp-acceptance.json` joins 13 complete receipts to its 3,451-file
production manifest, including 55 artifact VM cases, 48 artifact boundaries,
320 TF/FP/MTE cases, two module builds and exact native/Wasm XML output checks.
This is the historical candidate before subsequent callback-authority hardening.

The three-round paired `-O3 -g` XML measurement gave 60.699860 microseconds
before, 60.216844 after and 22.041219 native. The 0.992043 after/before ratio
is inside round variation: a stable timing gain has not been established.
After/native remains 2.732011 for this serialization/cache fixture; this does
not compare native ROS middleware. Four official VTune collections, frozen
source joins, 44 assembly artifacts and five instruction-level function reports
are retained in `artifact-fp-profile`, `artifact-fp-guard-assembly-final` and
`artifact-fp-instructions`. The actual wrappers save controls before dispatch
and restore controls after it. The measured fegetenv/fesetenv share fell to
1.678%; 55.609% unknown JIT samples remain unqualified for guest source mapping.

`callback-artifact-fp-first` separately requalifies the four-header callback
authority change against `scene-callback-source-qualified`: seven commands,
48 profiles and 1,632 hostile callbacks/336 recoverable allocator probes per
executable. This does not transfer the historical candidate's timing result
to the changed VM.
