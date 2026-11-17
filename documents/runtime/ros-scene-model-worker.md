# Isolated CPU model provider

> Historical VM-owned launcher documentation. The current production model path
> uses [externally provisioned attachment](ros-model-attachment.md). Launcher,
> image, placement and worker-limit CLI options below are removed. The receipts
> here apply only to their recorded historical builds.

This provider executes an explicitly approved synchronous native backend in a
separate Linux process. It implements the execution side of `local-model`;
predictions remain data and cannot grant, accept or execute robot commands.
The initial qualified workload is a real 2×2 matrix model. It does not establish
compatibility with arbitrary LLM runtimes, GPUs, JIT engines or remote services.

## Trusted preparation and ownership

`model_worker::service::prepare(dispatcher, alias, generation, manifest, owner)`
is a deployment operation. The alias must already have an exact `models::plan`.
The manifest binds the same model and task identity/digest and supplies already
opened regular file descriptors with exact byte sizes and SHA-256 digests for
the bootstrap, backend, dynamic loader, every runtime dependency and weights.
The launcher itself is pinned by size and digest. No guest path or executable
selection reaches this API, and no host directory is mounted.

Every file is copied and verified into a sealed memfd. On the available
bubblewrap 0.11.0, `--ro-bind-fd` resolves `/proc/self/fd/N` through `realpath`,
which cannot mount an anonymous memfd. Therefore runtime images use
`--ro-bind-data`: the manifest's `image_bytes` includes **two full copies of
every runtime/bootstrap image**, one copy of each weight and the launcher.
This is a finite explicit preparation reservation, separate from a job's
workspace or optional rate attenuation. The current service retains its sealed
copies until retirement; it does not silently refund the startup copy budget.

All VM-owned containers, image metadata, shared ownership and supervisor stack
use recoverable fast_io allocator adapters. `supervisor_bytes` bounds the
usable thread stack; two host page guard regions are added explicitly. Kernel
pipe buffers, thread TLS and runtime address space remain additionally bounded
by the containing deployment and its cgroup. `address_bytes`, CPU seconds,
open files, scratch bytes/slots, file size, request/response bytes, diagnostic
bytes and startup/poll deadlines must be configured; zero does not authorize
a process. `stderr_bytes=0` explicitly permits no diagnostic bytes.

The returned `provider_lifetime` must be retained by the broker. Revoke calls
nonblocking `request_stop()`. `retire()` joins outside authority locks and must
complete before the referenced dispatcher is destroyed. A supervisor failure
detaches its generation; there is no unapproved automatic restart. A fresh
deployment must explicitly prepare another generation.

## Inert configuration and CLI integration

`model_worker::configuration` is the trusted configuration helper. It accepts
one `limit(name, value)` for each of the following semantic names; duplicates,
unknown names and incomplete settings are rejected. Zero is retained explicitly
and is valid only for fields whose policy permits it, such as `stderr-bytes`.

```
image-bytes       address-bytes       cpu-seconds
file-bytes        open-files          scratch-bytes
scratch-slots     request-bytes       response-bytes
stderr-bytes      startup-ns          poll-ns
supervisor-bytes
```

`launcher(path, bytes, sha256)` and
`image(role, destination, path, bytes, sha256)` record exact images. Paths must
be absolute, nonempty and bounded to 4095 bytes. Image roles are bootstrap,
runtime and weights; logical destinations use the closed sandbox directories.
`freeze(models::plan)` checks the complete configuration and binds the exact
model/task profiles without accessing any path. After freezing, settings cannot
be changed. The central CLI exposes semantic named limits and exact files:

```text
--ros-scene-model-worker-limit ENV ALIAS NAME VALUE
--ros-scene-model-worker-launcher ENV ALIAS ABS_PATH BYTES SHA256
--ros-scene-model-worker-image ENV ALIAS bootstrap|runtime|weights DEST ABS_PATH BYTES SHA256
```

The exact grant and `--ros-scene-model` plan must precede these settings. Every
model alias requires a complete worker manifest, including explicitly set zero
values where supported. `--ros-scene-check` performs only inert validation;
even nonexistent image paths can pass that check. Normal VM preparation uses
`deployment::isolated_model_provider`. All workers across all environments must
prepare successfully before any guest can select an environment. Any failure
stops and joins earlier prepared workers; no partially prepared deployment is
published. Provider generation is the unique trusted environment identity,
independent of guest request IDs. Frozen settings cannot be changed to retry
with different files; a changed policy requires a new deployment.

`configuration::prepare(dispatcher, alias, generation, owner)` performs actual
file opening and invokes the service. It first uses `O_PATH | O_NOFOLLOW` and
`fstat` to reject directories, devices, FIFOs and final-component symlinks
without activating them. It then obtains a read descriptor through the pinned
`/proc/self/fd` identity and checks inode, device and size again. Parent-directory
symlinks are **not** forbidden by `O_NOFOLLOW`; those host paths are trusted
deployment choices. Exact hashes and immutable sealed copies determine the
worker's actual bytes. A wrong digest or any partial startup failure leaves no
published owner. The temporary read descriptors close on every path.

VM-owned storage uses the strict recoverable fast_io adapter: ordinary fatal
allocation is not used to implement these preparation paths. Allocation refusal
returns a resource error and rolls back descriptors, stacks and process state.
Concurrent `retire()` callers are serialized; `request_stop()` remains a
nonblocking atomic signal path and can be called while an authority lease is
held. Retirement itself must remain outside that lease.

## Sandbox and data delivery

The launcher uses explicit user, PID, network, IPC, UTS and cgroup namespace
creation; unsupported isolation fails. It does not use `--unshare-all` because
that option includes best-effort namespace creation in this bubblewrap release.
The original cgroup membership is inherited and never changed. Environment is
cleared, capabilities dropped, nested user namespaces disabled, and every path
is a fixed newly created directory or an exact regular image file. Runtime
mounts are read-only. There is no host `/usr`, `/root`, device tree or general
filesystem mount.

The bootstrap is namespace PID1. The parent obtains bubblewrap's reported PID
while `--block-fd` holds execution, opens a pidfd, checks its parent is the still
live pinned launcher, and verifies both pidfds remain live before release.
Cancellation signals retained pidfds, never an unvalidated or recycled numeric
PID. Cleanup requires both actual namespace init exit and launcher reaping.
Forked copies of the service reject control before touching inherited locks or
signalling descriptors owned by the original process.

The initial seccomp filter has a closed syscall allowlist and rejects the wrong
architecture and x32 syscall encoding. It permits the initial fixed bootstrap
exec and loading approved ELF dependencies. The digest-approved bootstrap loads
the fixed backend, lowers the file-size limit and installs a second filter that
denies further exec, new executable mappings and `mprotect(PROT_EXEC)`. Only
then does it emit its readiness marker and receive model weights.

Weights are **not inherited** by bubblewrap and are not present in the sandbox
filesystem. After the approved bootstrap's readiness marker, the supervisor
transfers sealed NX memfd descriptors over one precreated directional control
socket. The sandbox can only `recvmsg` on that fixed descriptor; socket creation,
connect and sendmsg remain denied. Bootstrap validates exact descriptor count,
metadata, regular-file size and successful seal queries, closes the control
channel and acknowledges attachment. Host data is not available before this
transfer. Backend invocations receive borrowed immutable weight descriptors
with their fixed logical names.

NX memfd seals alone do not prove executable mappings are forbidden. The final
seccomp layer separately rejects executable mmap/mprotect, and actual syscall
probes exercise both. Approved runtime and backend constructors execute before
that final layer and are part of the audited TCB; they must not process model
or inference input. This profile does not load arbitrary plugins, create new
threads/processes or support a JIT after attachment.

Scratch consists only of up to 16 precreated fixed slots in a sized tmpfs.
File creation, temporary-file creation, directory/link creation and file-mode
changes are denied. Both tmpfs total bytes and final per-file `RLIMIT_FSIZE`
apply. Runtime-copy preparation temporarily needs a larger file-size limit;
the fixed bootstrap lowers it before accepting any weight or request data.

## Inference, revocation and cleanup

The supervisor claims jobs through `model_dispatch`, measures the canonical
internal request, reserves its request and response storage from that job's
workspace, then copies each input and parameters under fresh short authority
leases. It holds no authority lock while polling pipes or computing. Every
poll observes cancellation, authority and the admitted absolute steady deadline;
every publication reacquires the full origin/source capability set.

The binary request contains the actual request ID, parameters and each input's
canonical layout plus bytes. Responses are bounded length-prefixed records
containing the same request ID, a closed partial/success/failure tag and payload.
Unknown tags, oversized/truncated data, unsolicited output and excessive stderr
terminate the worker. Partial results remain subject to the model plan's total
byte/item allowance and validators.

The backend ABI is synchronous and must not retain invocation bytes. The fixed
bootstrap buffers a terminal result until the backend has returned and the
request carrier has been destroyed, then publishes terminal output. This is the
cleanup point for successful CPU requests. Cancellation or protocol failure
kills and observes the actual process before confirming job cleanup. Failure to
confirm cleanup leaves the existing unknown-outcome state and retained capacity;
a timeout alone never proves resources are free.

## Qualification boundary

The actual sandbox fixture runs a matrix multiplication from transferred sealed
weights and probes network creation, host file access, file creation, weight
chmod, weight executable mapping, writable-to-executable mapping, exec, memfd
creation, runtime writes and an allowed bounded scratch write. Provider tests
then exercise the same process through real model dispatch, including source
revocation, deadline, malformed output and diagnostic flooding. Receipts must
pin source snapshots, compiler flags, every ELF/runtime input and launcher
version/hash. Performance executables use `-O3 -g`; sanitizer fixtures use their
appropriate instrumentation flags. All runs remain inside the existing 64 GiB,
zero-swap cgroup.

The installed non-setuid bubblewrap 0.11.0 is **not described as patched** for
[GHSA-pxhw-h44j-8pfx](https://github.com/containers/bubblewrap/security/advisories/GHSA-pxhw-h44j-8pfx).
The launcher design excludes that advisory's attacker-controlled parent-directory
symlink condition: a fresh root, bounded fixed destinations, no directory binds,
and only regular sealed image data. Validation tests must continue to reject
path traversal, file/parent collisions and directory/symlink inputs. The
[upstream source](https://github.com/containers/bubblewrap/blob/v0.11.0/bubblewrap.c)
defines the pinned launcher behavior; the
[kernel memfd documentation](https://docs.kernel.org/userspace-api/mfd_noexec.html)
defines NX seal behavior. These measures and tests are not an industrial safety
certificate or a claim of isolation against every future kernel/runtime flaw.

## Current retained receipt

`model-worker-configured-second/results.json` in the SDK `.tmp/ros-scene-security`
directory records **22 successful commands** from retained source snapshots:
Clang ASan/UBSan and GCC policy, actual sandbox, real dispatcher and configuration
factory checks; **35 actual allocator failure points**, including supervisor
launch allocations; and a successful actual service run under TSan. The service
checks include concurrent retirement. The bootstrap and backend were compiled
with **`-O3 -g`**; instrumented service checks used their recorded sanitizer
flags. Source snapshots, workspace inputs, external image hashes and memory
limits were unchanged throughout that receipt. Its cgroup inode is 12488 and
boot ID is `07b9df6a-6eeb-4296-ac52-365c86a524eb`; memory max/OOM/kill event counters
stayed at zero for this particular new-boot cohort. This does not overwrite
earlier cohorts' nonzero memory-pressure counters.

Earlier `model-worker-isolated-resumed` and `model-worker-configured-first`
receipts remain incomplete: GCC rejected fixture/header warnings before those
cohorts completed. Their diagnostics and subsequent corrections are preserved.
That receipt covers the standalone native service and helper. The later
integration evidence below qualifies the central CLI and actual Wasm launch;
neither receipt measures native latency parity.

## Central VM integration receipt

`model-source-worker-central` retains 3443 production files based on the prior
qualified runtime and the model worker integration. Its
`freeze-model-worker-central.json` records the 10-file delta. Existing WASIp1
and ROS-v1 implementations were not part of that delta. The matching
`vm-model-worker-central/full-build-results.json` records all three production
translation units compiled with **`-O3 -g`**, an unstripped link, and verified
`.debug_info`/`.debug_line`. The VM SHA256 is
`3c10ae39e0b5376642127b03844c4d0c43b412eb851bef3c90b489d007bcb757`.
This frozen cohort deliberately excludes later unrelated workspace changes.

The retained evidence under the same SDK temporary directory is:

- `model-worker-central-first`: 26 successful native commands, including actual
  two-worker deployment. A wrong hash for the second worker reaps the first,
  restores descriptor counts and leaves every guest environment unavailable.
  Successful preparation runs the syscall probe through each deployed alias.
  This repeats the allocation-refusal and TSan checks against the integration.
- `model-deployment-worker-first`: nine compile/run commands across Clang
  ASan/UBSan, GCC and TSan verify inert validation, complete capacities,
  whole-deployment publication, stop/join ordering and injected-owner rollback.
- `model-cli-worker-second` and `model-worker-host-first`: three commands each
  qualify actual CLI callbacks and production host wrappers under ASan/UBSan.
  These focused native fixtures are separate from the actual Wasm runs below.
- `model-worker-modules-llvm-first` and
  `model-worker-modules-interpreter-first`: 22 commands each qualify named-module
  ownership and host FP policy for both backend configurations. Matching
  fixture bytes are preserved in `module-fixture-manifest.json`.
- `model-vm-worker-first`: **31 actual VM cases**, including memory32/64,
  memory indices zero/one, interpreter/LLVM execution, deny/no-grant policies,
  hidden imports, static modes and CLI refusals. The seven model/request imports
  execute a real 2x2 inference yielding 17 and 39, the isolated syscall probe,
  exact retries, cancellation, result consumption and confirmed release.
- `model-worker-boundaries-first`: **192 actual VM cases**. Eight FP runs
  execute real model workflows; each guest contains 58 checked host-return sites
  with dynamic f32/f64 arithmetic. The remaining cases cover matching software
  memory tags, normalized alias rejection and each request/result/length tag
  fault for all seven carriers. MTE fixtures use invalid request handles and
  issue no model job; they qualify host memory boundaries, not backend parsing.
- `model-worker-dap-second`: actual full, read-only and limited DAP sessions.
  The guest completes real inference and retains the request/result reservation.
  Repeated inspection returns only aggregate counters, issues no native model
  operation and reads no payload. Input leases are zero after confirmed native
  completion; retained output/workspace capacity remains charged. Limited
  inspection and stale-stop requests are refused.
- `model-worker-debug-contract-first`: eight checks cover formatter, strict DAP
  schema and editor controller/model behavior, including exact decimal counters
  above JavaScript's safe-integer range. This does not claim graphical IDE
  acceptance. DAP/frontend tools were frozen separately before later TF edits.

All these runs inherited the same existing 64 GiB, zero-swap cgroup. During
this integration cohort `memory.events max` remained at its existing value 567;
OOM and OOM-kill counters remained zero. Incomplete CLI/DAP fixture attempts
were retained with explanations; their corrected receipts use fresh folders.
The approved CPU protocol is qualified here. Arbitrary LLM, vision/world-model
frameworks, GPU/JIT support, native-equivalent latency and industrial safety
certification still require their own implementation and qualification.

## Mapping ABI security boundary

The final worker filter denies legacy pointer-form `mmap` on i386 and ARM.
That syscall receives a pointer to `mmap_arg_struct`; classic seccomp BPF cannot
read the embedded protection bits. Treating the third syscall argument as the
protection field would therefore leave an executable-mapping bypass. Direct
argument `mmap2` remains available for non-executable mappings. x86_64 and
AArch64 continue using their directly inspectable `mmap` arguments. ARM EABI
normally omits the obsolete syscall entirely; an absent entry remains denied
by the startup allowlist. Unknown architectures remain unsupported.

`model-mmap-final/results.json` records twelve successful qualification steps
with effective `-O3 -g` in the same existing 64 GiB, zero-swap cgroup. Native
x86_64 Clang ASan/UBSan and GCC tests install the actual two filters. A native
i386 executable also installs both filters on the host kernel: ordinary
read/write anonymous mappings succeed, whereas executable `mmap2`, executable
`mprotect` and pointer-form `mmap` return the filter's denial. The retained
pre-fix i386 negative control actually creates an executable mapping through
the old pointer syscall, emits `legacy executable mapping admitted`, and exits
through the expected failure assertion. It is not counted as a passing worker.

AArch64 and ARM builds interpret the exact generated classic BPF under QEMU
using each target's syscall constants; they do not claim native seccomp
enforcement under emulation. The frozen `model-mmap-source` differs from
`artifact-fp-source` only in `model_sandbox.h`. Earlier x86_64 VM receipts remain
historical evidence for their exact hashes; they do not imply that those frozen
sources contain this later cross-ABI security fix.

The subsequent complete `-O3 -g` build `vm-model-mmap/uwvm2-ros`, SHA256
`017cb3f6796841f7ab983cf1ccbbb83798da1e09c7413e4a6ddca958fd35ec2d`,
contains that fix in its 3,451-file frozen manifest. `model-mmap-vm` records
31 successful real VM model cases and `model-mmap-boundaries` records 192
successful FP/MTE/carrier cases using the earlier pinned x86_64 worker images.
This verifies the unchanged x86_64 worker ABI with the tightened VM policy;
it does not claim a complete i386 worker launch or native ARM seccomp deployment.

## CPU and NUMA launch placement

`--ros-scene-model-worker-placement` selects the trusted worker's logical CPU
mask and `inherit`, `local`, `bind` or `interleave` memory policy. Configuration
parsing and static validation remain inert. Preparation resolves requested sets
against current kernel eligibility. The direct child applies and reads back the
selection before running the approved launcher; a failed syscall or changed
allowed set rejects startup. Provider code cannot subsequently call affinity or
memory-policy setters through the worker seccomp policy. The parent VM's mask
and existing cgroup membership remain unchanged.

`execution-placement-first/results.json` records 18 successful dependency,
compile and execution commands across Clang ASan/UBSan and GCC, all using
`-O3 -g`. Tests include masks spanning CPU IDs above 1,024, invalid ranges,
unavailable CPUs, real fork/exec selection, parent-application refusal, a
post-preparation narrowed allowed set, CLI duplicate/late/malformed rejection and
two actual recoverable allocation-failure points. All three explicit NUMA modes
were applied and read back on this machine; this is not cross-node performance
qualification.

`model-worker-placement-corrected/results.json` records 26 successful commands:
actual model inference and confinement, factory/deployment rollback, 37 real
allocator failure points and the threaded worker regression. Its in-worker
probe observes exactly the selected logical CPU and confirms that both forbidden
placement setters return the seccomp policy's `ENOSYS`. The first placement
cohort remains a failed historical receipt because that new probe incorrectly
expected `EPERM`; the corrected test checks the existing filter's exact result.
`communication-jobs-native-placement/results.json` separately verifies real ROS
communication with the same launch gate in its worker sandbox.

These are native core/provider receipts. Actual VM, debugger, cross-platform
launch and performance qualification remain separate. Placement is neither
physical-core exclusivity nor a continuous topology monitor; see the
[robot resource contract](ros-robot-capabilities.md#placement-and-resource-guarantees).
