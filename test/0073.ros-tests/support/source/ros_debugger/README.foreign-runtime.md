# Explicit foreign LLVM-full debugger execution

This source patch adds a version2 cross-build/launch adapter to
`build_runtime.py` and `integration_runtime.py`. The unchanged version1 input
shape and native x86_64 launch path remain available. These files were authored,
read and AST-parsed only. No compiler, QEMU, policy test, debugger or ROS case
has been run by this patch's author. Apply only after Root review and include
all added files in a new complete frozen source manifest.

This is independent of `test/0073.ros-tests/support/source/ros_multiarch_runtime`: that lane prepares real
**interpreter** products and its208 delivery cases cannot qualify an LLVM-full
build, JIT, debugger console or native instruction step. This adapter consumes
only fresh, same-source `ros-debugger-runtime-build` products with actual full
compiler dependencies and patched static LLVM provenance. No interpreter
receipt is converted, relabelled or used as fallback.

## Eight explicit Runtime architectures

| Architecture | ELF machine | Bits | Byte order | LLVM native target macro |
| --- | ---: | ---: | --- | --- |
| x86_64 |62|64|little|X86|
| aarch64 |183|64|little|AArch64|
| arm |40|32|little|ARM|
| i386 |3|32|little|X86|
| riscv64 |243|64|little|RISCV|
| loongarch64 |258|64|little|LoongArch|
| ppc64le |21|64|little|PowerPC|
| s390x |22|64|big|SystemZ|

The table is a closed parser/selection contract, not a list of tested JIT
backends. The native matching-host case uses version1. Version2 requires a
foreign target, its actual matching compiler triple and an explicit user-QEMU
selection. The selected compiler/linker run on the actual host; every Runtime
object, LLVM archive member, extra target library, loader and final executable
must have the selected foreign ELF machine/class/byte order. A target name or
QEMU filename is never sufficient architecture proof.

## Version2 configuration

Keep every existing version1 configuration field and its real provider
requirements; set `format_version` to2 and add exactly one `execution` object
with these keys:

- `architecture`: one exact table key.
- `mode`: exactly `qemu`.
- `qemu`: actual current physical host QEMU full7 pin.
- `sysroot`: canonical physical target sysroot directory; it must equal the
  existing compile `sysroot` and belong to a completely inventoried input root.
- `loader`: actual current physical target dynamic-loader full7 pin inside that
  sysroot. The binary's actual PT_INTERP must resolve to it under bounded guest
  symlink semantics. Absolute guest link targets restart at the target sysroot.
- `environment`: closed object allowing only optional `LD_LIBRARY_PATH`; each
  component is an explicit existing physical directory in inventoried roots.

A full7 is exactly `path`, `bytes`, `sha256`, `device`, `inode`, `mtime_ns` and
`ctime_ns`, observed after opening the current ordinary file. Do not attach an
old SHA to new stat values or forecast a target loader/QEMU identity. QEMU
itself must have actual host ELF identity; the target loader must have actual
foreign identity. Their complete bytes are requalified before/after build and
integration, and target sysroot directory identity is recorded separately.

The compiler-only `environment` remains separate. The actual launch prefix is
constructed with no shell or binfmt discovery:

```text
ACTUAL_PHYSICAL_QEMU -L ACTUAL_SYSROOT
  -U LD_PRELOAD -U LD_LIBRARY_PATH
  [-E LD_LIBRARY_PATH=EXPLICIT_GUEST_DIRECTORIES]
  ACTUAL_IMMUTABLE_RUNTIME_COPY EXISTING_DEBUGGER_RUNTIME_ARGUMENTS...
```

Guest loader environment travels through QEMU `-E`; it is not passed to the
host QEMU's dynamic loader. No comma-separated QEMU environment assignments, arbitrary QEMU flags, gdb server, helper wrapper,
CPU mode, preload or ambient QEMU variable is accepted. The actual selected
QEMU still needs to support these ordinary user-mode options and the target
syscalls used by the production Runtime. A suitable static QEMU avoids an
additional host dynamic-loader dependency; this adapter proves the host QEMU
executable itself and does not claim a complete live QEMU DSO inventory.

Version2 still requires the current patched LLVM source lock, source SDK,
Release CMake origin, actual file-API consumer response and target archives.
The generated LLVM host/default target triples must equal the selected target
triple, and `LLVM_NATIVE_ARCH`, native target/parser/disassembler macros and
archive members must agree with that target. A host-x86 LLVM package cannot be
linked into a LoongArch Runtime by changing JSON labels. Actual target C++26
stdlib, unwinder, crypto, startup libraries and matching ABI remain caller
prerequisites; this adapter installs or manufactures none of them.

## Distinct build and execution evidence

Version1 profile receipts retain their original fields. Version2 adds exactly
`build_configuration` (the original config full7), `build_evidence` (the actual
whole-build terminal receipt full7) and `runtime_execution` (the observed
selection, actual host ELF, sysroot identity and literal prefix).
`target-execution.schema.json` documents those shapes. Build PASS still means
fresh compilation and preserved inputs only, never QEMU/JIT/debugger execution.

For version2 integration, pass the **same actual build configuration** through
new `--runtime-build-config /absolute/original-build-config.json` alongside all
existing integration arguments. The consumer compares its current full7 to the
producer receipt, requalifies QEMU/loader/sysroot, and requires all three real
Runtime TUs with `UWVM_USE_LLVM_JIT`, `UWVM_DISABLE_INT` and C++26 flags. It keeps
original full-MD, original command/object/link/archive/log checks. Version1 accepts no
foreign override and follows its original stopped-host proof.

Before starting any actor, version2 additionally re-inventories **all** selected
build provider/tool/stdlib/sysroot roots using the actual current MF-bound
catalog implementation, then requires exact content/identity/alias equality to
the original terminal build catalogs. Original catalogs remain separate pins.
After inherited owned-process cleanup and before terminal evidence can report
PASS, it inventories those same roots again and requires equality to the full
admission baseline. New catalog pins and the complete extended selected pin
list are rechecked. This closes unused/new/changed foreign SDK files too; the
existing ARM SDK checks alone do not supply that result. Only this IntegrationLane
instance wraps that exact terminal-evidence stage; generic Lane, ARM evidence and
native version1 remain unchanged. A missing or partial catalog baseline fails
closed and cleanup still runs.

Existing `--qemu`, `--worker-config`, SDK workspace and binfmt lease describe
the **independent ARM ROS Gateway/peer side**. They do not select or authenticate
the Runtime QEMU. A LoongArch Runtime can therefore have a separately selected
LoongArch user-QEMU while the existing Gateway uses its real AArch64 SDK/QEMU.
No ROS distribution, ABI or Gateway-architecture compatibility is inferred.

## Actual kernel stop, guest mapping and authenticated controller

The existing PID-preserving shell remains stopped before Gateway configuration.
It execs the explicit Runtime QEMU in that same process birth, retaining the
original inherited private SOCK_SEQPACKET descriptor. The production Runtime
must pass `prepare_debug_control_fd_mode`: actual LLVM-only full compilation,
native threads and supported Linux control syscalls. It must return its genuine
prepared console status before any Guest runs; existing actual Host identity,
ROS query windows, sentinel/payload/queue effects and native witnesses remain
mandatory. No DTO callback, fake engine probe or copied console transcript is
introduced.

Once that actual controller is prepared, the owner sends SIGSTOP using its
retained pidfd and requires the **same host PID/birth** in kernel T/t state and
the original live cgroup, plus a bounded unchanged set of actual thread births
all in kernel T/t state. `/proc/PID/exe` must be the exact host QEMU full7.
The shared `opened_target_mappings` opens real map_files descriptors for the
immutable foreign Runtime copy and, for dynamic images, its actual target
loader; it checks fstat facts, full SHA and foreign ELF identity while the held
birth remains stopped. Maps display paths, guest virtual addresses and QEMU's
host ELF alone cannot satisfy the guest-image proof. There is no wait-child
PID substitution. CONT also uses the retained pidfd.

The existing console reply SCM_CREDENTIALS must still equal that actual PID,
UID and GID. Guest pidfd_open/peer authentication and host kernel credential
translation must work through the selected QEMU. Missing syscall translation,
map_files permission, target-image mapping, inherited-FD support or a different
QEMU helper process causes failure; this patch never bypasses these checks.
These are concrete per-emulator/per-architecture prerequisites that still need
actual execution. The original process/mount cleanup always runs, including
when foreign selection requalification fails.

The default-deny job still has no ROS aliases or optional WASI grants. Retained
jobs carry only their existing explicit aliases and queue/audit quotas. All
read-only ROS snapshots, old-stop/running-state refusals, independent native
payload and quiet checks remain unchanged. The native-step option is also
unchanged: `required` needs an actual successful target `step asm`, ROS refusal
at that native stop and restoration of a cooperative Wasm stop. An explicitly
unsupported target may record `NOT_APPLICABLE_SELECTED_BACKEND` with a reason;
that is no native-step PASS and cannot satisfy a required matrix row.

## Authored policy tests and remaining real work

`test_target_execution.py` contains17 unexecuted policy/parser tests: all eight
architecture identities, mismatched class/endian/triple/host rejection, missing
physical selections, forbidden environment/extra keys, exact guest-only argv,
full7 integer validation, root-sensitive guest symlinks and malformed ELF
program tables. Synthetic ELF headers are mathematical parser inputs; no test
pretends to execute them or to establish QEMU/JIT correctness.

Root still needs each actual target's patched LLVM package and cross compiler,
C++/crypto/sysroot/loader closure, serial three-profile fresh build, exact
mapped-image/credential/controller execution, all six debugger ROS jobs and any
required native-step qualification. LoongArch and the other architecture names
remain pending until those real results exist. Interpreter208, source review,
policy tests and a build-only PASS are never substituted for them.


## Current descriptor and IDE projection checks

ELF decoding now compares the newly held descriptor's dev/inode/size/times
with the previously complete-hashed pin before reading its program headers,
and checks that same held descriptor against the named entry before closing.
A stable second file cannot stand in for the admitted image. The additional
real-filesystem regression in `test_target_execution.py` supplies another
inode's pin and requires refusal; it executes no target ELF.

The real integration lane also admits the current complete-MF-bound DAP
adapter and exercises `uwvm/rosState`'s production query method through its
actual credential-checked inherited console. It checks all canonical selectors,
instance/event aliases and current/running/stale/native-stop boundaries while
retaining unchanged Gateway streams and the original native payload oracles.
This provides the same projection route on any actually admitted foreign
Runtime; it does not claim that an IDE or secure-server transport ran. See
[the DAP ROS contract](../../../../../tools/debug/README.ros.md).

`prepare_llvm.py` version2 can additionally build explicitly selected host
backend archives, including LoongArch. See [its exact scope](README.prepare-llvm.md).
Those ELF62 host archives do not substitute for a foreign Runtime's actual
foreign-host LLVM SDK. Interpreter208 remains separate from LLVM-full debugger
acceptance, and no newly authored policy or adapter is an executed target pass.
