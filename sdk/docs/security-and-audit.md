# Security boundaries and auditable permissions

WasmROS assigns ROS authority at the trusted Runtime/Native Gateway boundary.
Every Guest language can bypass its convenience wrappers and call imports
directly; the Host must still validate identity, memory, resource, operation,
type, QoS, quota and lifetime. An SDK class, selected header or generated alias
does not grant authority.

This guide describes the controls and the evidence needed to evaluate them.
It does not certify the absence of every possible sandbox escape. A source
review, a compiled artifact and a real negative test are distinct evidence.

## What is isolated

Guest source executes in Wasm, including its rebuilt C/C++ dependencies,
CPython or embedded QuickJS where selected. Numeric ROS imports refer to linear
memory and opaque handles; they never supply native ROS object addresses.
Runtime-derived module/instance/generation identity selects the effective
Single/Group binding or Global fallback. The selected complete target is not
an assumed inheritance merge of ROS grants.

The Runtime and native Gateway, selected RMW/ROS libraries, system kernel and
trusted deployment/controller are trusted components. The native Gateway has
its own native memory and network authority. Putting its guest caller in Wasm
does not put that native library in a Wasm sandbox.

The supplied-memory JavaScript bridge and external Node relay have external
embedding/supervisor requirements. HELLO compatibility checks do not
authenticate a supervisor. Node runs with its host process authority. Only the
embedded QuickJS route places the selected JS code inside the Wasm principal.

## Enforced request path

1. The Runtime identifies the actual active Guest from authenticated execution
   context and the current instruction stack. Guest-chosen labels or callback
   arguments are not trusted identity. Retired/cross-module authority refuses.
2. Memory access resolves the actual selected memory descriptor. Integer
   overflow, invalid memory indices, ranges and output capacities must fail
   before Host copying or a native effect.
3. `capability_open(alias)` selects only an enabled, live alias in the effective
   target. Returned opaque handles belong to the actual instance/backend.
4. Each operation independently checks its role and exact resource/type/QoS
   contract, relevant source/observer ownership and limits. Receiving a handle
   does not imply all operations on that resource are authorized.
5. The Worker connects to the explicitly configured private Unix socket.
   Socket/peer ownership and actual Worker UID/PID/pidfd lifetime are checked.
   The Gateway requires the exact session binding manifest, not a guest
   supplied partial grant.
6. Response payloads, correlations, delivery identity and output bounds are
   checked before Guest delivery. A request accepted by the transport is not
   proof that a native effect completed or that replay is safe.

See [architecture](architecture.md), [host setup](host-setup.md) and the
[update/debug boundary](update-and-debug-boundary.md) for implementation
contracts. Preserve the primary error plus all cleanup errors. Error110 can
leave the native effect unknown; retrying an effectful operation is not a
generic recovery strategy. Failed ownership delivery must still retire the
real acquired owner exactly once.

## Permission review for an application

Record the actual module and backend, alias, absolute ROS resource, native
schema/type identity, allowed role, QoS, byte/rate/burst/queue bounds, audit
limits and retirement behavior. Deny by default and add only the roles that
the application needs. Use separate grants for, for example:

| Application role | Separate authority to review |
| --- | --- |
| Telemetry reader | Exact subscription and output data projection; no publisher or command service |
| Topic producer | Exact publication; no subscription, graph enumeration or parameter read |
| Service client | Exact call; no advertise/respond server role |
| Service server | Matched advertise/respond ownership; no unrelated caller authority |
| Action client | Send/feedback/result and optionally cancel-own; cancel-any separately |
| Observer or event listener | Exact source association, filter, bounded queue and delivered-data projection |
| ROS1/ROS2 converter | One explicit input route and one explicit output route; no blanket backend access |
| Robot command component | Exact admitted schema, command/resource and finite value/rate limits |

WASI permissions remain independent. Strict enabled ROS startup rejects socket
standard descriptors, native dynamic/weak plugins, inherited environment and
directory/socket preopens. The same restriction applies to enabled WASI
modules. Freeze/static-embed managed dependencies rather than exposing a host
library tree. A language runtime's clock or randomness requirement needs an
explicit separate grant.

DDS-Security Enforce is an additional native network layer. The local example
policy deliberately uses `disabled-test`; it is not an Enforce recipe.
For Enforce, review the real Gateway keystore/enclave and six derived credential
files, ownership/mode and selected RMW. Credentials belong to the Gateway/RMW,
not Guest memory, WASI environment or preopens. Do not copy keys into reports.

## Compare against a concrete container deployment

A container can provide namespaces, cgroups, seccomp, user separation and
network policy. Its actual configuration determines the boundary. A broad
claim that all containers have weak permissions would be incorrect. Native
libraries within one ordinary process still share that process's address
space unless additional isolation is implemented.

Evaluate the same application and adversarial input against these concrete
configurations, keeping UID, network reachability and ROS security comparable:

| Configuration | Boundary and evidence to retain |
| --- | --- |
| Native application in the measured traditional container | Exact namespaces/capabilities/seccomp/mounts/network/DDS credentials and actual ROS effects |
| Hardened native container | Explicit improvements, separate processes or DDS permissions; their setup and resource cost |
| WasmROS modules under that same outer container | Per-module grants, denied-role traces, isolated Guest memories and exact native Gateway authority |

Attempt an unauthorized topic, wrong operation on an allowed topic, service
response through a caller role, cancel-any through cancel-own, another module's
handle, stale incarnation, malformed out-of-range memory and bypass through
alternate WASI/native-plugin channels. Include positive controls: authorized
operations must really reach a native peer. Observe **no forbidden native
effect**, rather than only a Guest error string. Record actor identity,
requested operation, effective policy, denial reason and backend result.

The expected advantage to demonstrate is independently controlled ROS
authority for components sharing one Runtime, with an inspectable decision
path. Demonstrate which concrete attacks each deployment denies. A measured
container might also deny some or all of them. Record that result and its
configuration rather than changing the baseline to manufacture an advantage.
Bounded audit queues must expose overflow/drop conditions; incomplete logs
cannot be presented as a complete audit trail.

## Audit each real language and migration route

Audit raw imports as well as typed APIs. C/C++ pointers, Rust/TinyGo buffers,
AssemblyScript memory growth, Java rooted frames, C# pinned arrays, CPython
`Py_buffer`, QuickJS copied scratch and MoonBit's reserved arena each have
different local lifetime rules. Verify full-width opaque values, reentry,
callback-time close, output delivery failure, exact-once close and retained
cleanup errors. GC collection and native pointer representations are not
permission controls.

For rmw/rcl/rclcpp/rclpy/rcljava migrations, retain the pinned real upstream
business source and its actual selected implementation. Exercise the same
scenario with a newly developed application in another supported language.
Facades, original upstream static cross-builds and native host libraries have
different contracts; a facade pass cannot qualify the original library route.
Test new applications' direct raw import misuse independently of the wrapper.

Custom schemas need complete source closure, actual official native identity,
bounded codecs, generated/native adapter checks and malicious-length tests.
Vendor standard messages, vendor services/Actions and proprietary low-level
commands are separate cases. A read-only simulator report is not evidence for
a physical-device command, and a generated registry is not native delivery.

## Code cache and trusted management

Code-cache admission must depend on the actual executing Runtime binary's
SHA256 and generated-code compatibility, not only a manually edited Git or
version string. The implementation under development uses a retained actual
self-executable file identity on qualified Linux, a process-once full digest
and drift checks, plus a real Runtime code anchor. Unprovable image identity
must disable disk-cache reuse. A cache format bump must reject legacy records.
Do not describe this design as qualified before the final code and tests pass.

Binary identity does not authenticate a native object supplied by a writer.
In particular, a WASI writable preopen covering a cache can let Guest code
modify an otherwise owner-only file under the same UID. A deterministic signing
seed derived only from public context cannot establish trusted cache origin.
The private successor therefore also requires independent kernel-held random
origin material and a real Runtime-issued cold-CLI admission. It rejects
unproven providers, filesystem/descriptor exposure and execution contexts.
It additionally requires actual non-dumpable process state: limiting key reads
to cold compilation and wiping explicit buffers does not prove that every
third-party crypto stack spill is absent from a later core dump. This version
does not change process or Host dump policy automatically. Default-dumpable,
unavailable-keyring and unsupported-image cases disable persistent caching;
that fallback is not a cache-hit, ROS cold/warm or performance qualification.
Reusable native embeddings and subsequent trusted Host reconfiguration need
their own contract. The source is still under review and has not been executed.

Binary identity is one component of the key. Preserve Wasm/IR identity,
validator/proposal settings, LLVM/codegen version and options, target/ABI,
actual CPU feature policy and relevant runtime contracts. Check x86 CPUID and
OS-enabled extended state, including downgrade/feature masking. Guest policy,
open handles, session identity and ROS credentials must not be restored from
cached machine code. Every fresh launch must derive its own authority.

Test changed binary bytes with unchanged Git/version, old format, torn or
truncated metadata, wrong Wasm/IR, wrong CPU/ABI, replaced/unlinked executable,
in-place drift, unsafe cache paths, symlinks, ownership/mode and concurrent
writers. Repeat real WASI and ROS1/ROS2 permissions and DBG with cache disabled,
cold and warm. A cache miss may rebuild; a rejected cache is not an application
permission failure. Failed provenance must never become arbitrary code reuse.

Trusted update/debug controls remain outside Guest ROS imports. Old DAP
references and module/handle generations must expire at the specified
lifecycle boundary. Updating machine code does not transfer an old ROS grant
to a fresh module. Debugger commands and cached object loading need their own
trusted channels; application stdout and ROS logs must not become commands.

## Qualification and reporting

All dynamic checks in this task run on SSH Linux inside the existing 64GiB,
zero-swap cgroup, including compilers, controllers, QEMU and native peers.
Retain final source/binary/input hashes, exact arguments, actual process and
peer identities, raw output, failed assertions and verified cleanup. See
[platform qualification](platform-qualification.md).

Report audited scope and actual evidence. An independent source baseline that
found no verified bypass in its reviewed paths is not a completed audit of all
SDK files. Newly added source must be reviewed after integration. Unsupported
routes, incomplete coverage and unexecuted runtime/DBG/cache/architecture
cases remain visible. These limits must survive any product security claim.
