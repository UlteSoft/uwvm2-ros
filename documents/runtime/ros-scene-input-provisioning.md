# Trusted scene input provisioning

`uwvm2/uwvm/imported/scene/input.h` prepares one immutable input selected by a
trusted deployment plan. It is host preparation code, not a guest import or
an extension to WASIp1/ROS v1. The deployment calls it after validating exact
purpose/binding and reserving explicit aggregate byte/object budgets, before
registering or starting the guest. Parsing a plan, checking policy and making
the first guest call must not implicitly provision native files. The loader
also keeps `validation` and `section-details` modes inert, independently of
legacy zero-host build defaults; invalid capacity still fails metadata checks,
but an unavailable source or different file digest is not read or tested there.

The inert plan contains an absolute source path, expected SHA256, positive
generation, exact byte length and a guest transfer bound at most 4 MiB. The
principal is an independently allocated trusted environment/grant identity.
The helper accepts only package, configuration, calibration, map and model
weights purposes. Zero-length input and zero transfer allowance remain valid
explicit values. All object bytes must fit the build's native offset width;
unsupported guarantees are refused rather than narrowed.

On Linux, resolution opens each path component using `O_PATH|O_NOFOLLOW`.
Intermediate components must be directories; the final inode must already be
a regular file before any readable open. Relative paths, parent traversal,
embedded NULs, oversized components, directories, FIFOs, devices and symlinks
are rejected. The regular inode remains pinned and is reopened through its
own `/proc/self/fd` entry. Type, inode and exact size are checked again.
Replacing the original name cannot cause a different inode to be read-opened.

The approved extent is copied into a private memfd using a fixed 64 KiB
workspace. Short reads/writes are handled without skipping or duplicating
bytes; an early EOF or extent mismatch fails preparation. After copying, the
memfd receives `F_SEAL_WRITE|F_SEAL_GROW|F_SEAL_SHRINK|F_SEAL_SEAL`. SHA256 is
computed from those sealed bytes and must exactly match the deployment's
expected digest. Only then does the immutable backend duplicate the sealed
descriptor and accept the principal, purpose and bounds. The original source
can change or disappear afterward without changing the admitted object.

A matching digest verifies bytes selected by the trusted deployment. It does
not validate a model parser, map decoder, dependency bundle or publisher.
Those are separate deployment/provider contracts. The object exposes no
native descriptor or source path; guest reads still require exact scene
authority and a matching live handle. Retained snapshot memory remains
charged even after guest handles close.

The caller's output is replaced only after every step succeeds. Failures close
temporary descriptors and preserve an existing output object. The deployment
must also release its pending aggregate reservation. Unsupported platforms
return `unsupported`; there is no fallback to ordinary mutable descriptors.

`test/0084.ros-runtime/input_provision.cpp` tests the real implementation,
including all five input purposes, source mutation/removal, binary multichunk
copy, empty files, exact digest/size/generation limits, principal isolation,
read permission, malformed paths and descriptor cleanup across repeated
failures. An inotify watch verifies that rejected FIFOs are never read-opened,
including concurrent regular-file/FIFO name exchanges. The tests do not need
root or modify existing deployment files.

Run `tools/ros_scene/input_provision_checks.py --output <fresh SDK .tmp path>`
through `tools/ros_scene/in_existing_cgroup.py` and `taskset -c 8-15`. Clang
AddressSanitizer/UndefinedBehaviorSanitizer and GCC execute inside the existing
64 GiB, zero-swap cgroup using private tmpfs files. The runner records commands,
source hashes and cgroup events, and refuses completed qualification if source
bytes changed during the checks.

`test/0084.ros-runtime/artifact_security.cpp` independently exercises the
broker and deployment with actual sealed snapshots. Failed or throwing open
delivery must free the unpublished resource slot. Repeating a read after
failed delivery returns the same immutable range. Close frees handle capacity
but keeps the deployment's retained snapshot alive. Exact alias, environment,
action, content generation and digest checks prevent confused-handle access;
artifact and scheduling resources share a monotonic identity allocator.

The deployment tests observe real input files with inotify: validation and
debug inspection must not read-open them. Missing or insufficient retained
byte/object budgets reject before invoking a provider. A trusted injected
failure in the second environment discards the first private snapshot and
its descriptors; neither environment becomes selectable. Retrying provision
publishes both together, after which deleting the source files does not affect
guest reads. Member, grant and runtime-generation retirement refuse future
use. Concurrent revocation waits for an admitted read's delivery to complete.

Run `tools/ros_scene/artifact_security_checks.py --output <fresh SDK .tmp path>`
through the same cgroup launcher with `taskset -c 0-3`. This dedicated suite
runs Clang AddressSanitizer/UndefinedBehaviorSanitizer and GCC with warnings as
errors and records exact source hashes. These tests qualify the immutable
input implementation and its authority boundary; they do not establish
whole-package ROS portability, model-decoder safety or native-performance
parity.

Supply `--recipes .tmp/gc-allocation-fix-20261113/build` to that runner to also
compile and execute AArch64 with 64-bit file offsets, ARM32 with both 32- and
64-bit file offsets, and i686 with both file-offset ABIs. AArch64/ARM32 run
through QEMU with an explicit sysroot loader and library search path; i686 uses
the reviewed sysroot's native IA32 loader. QEMU's `-L` path rewriting is avoided
because it can make an opened root directory and absolute `/dev/shm` names
refer to different directory trees. The production path resolver is unchanged.
Each run
prints and checks the selected `off_t` width and tests an input extent at the
2 GiB boundary without allocating a huge file. Narrow offsets refuse before
conversion; wide offsets reach the exact-length check. Both preserve an
existing input object. These runs exercise real memfd seals, scoped principals
and atomic deployment publication; emulation results qualify correctness,
not target hardware performance.

`tools/ros_scene/artifact_thread_checks.py --output <fresh SDK .tmp path>` runs
the same seven artifact safety groups with ThreadSanitizer, including read
delivery concurrent with revocation. Use the same cgroup launcher and CPUs
0–3; `--compiler` can select the reviewed host Clang. The runner retries only
the known pre-main `unexpected memory mapping` initialization failure, up to
eight times, saving every attempt. A race report, assertion or other failure
stops immediately. No sanitizer suppression or policy relaxation is used.

`test/0084.ros-runtime/input_unsupported.cpp` can additionally be compiled with
`-U__linux__ -std=c++23 -Isrc -Ithird-parties/fast_io/include` by a native Clang
or GCC toolchain, inside the same gate. It verifies that the portable headers
can name the opaque input type and that both preparation and sealed-input
deployment admission return `unsupported`. This forced-preprocessor probe is
only a fallback-branch check, not a foreign-platform ABI or filesystem test.
