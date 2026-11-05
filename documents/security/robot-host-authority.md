# Robot host authority boundaries

`UWVM_ROS_ZERO_HOST_DEFAULTS=1` is selected by the mandatory ROS product build
rule. It is independent of imported ROS namespaces, compiler choice and the
DEV/RELEASE profile. Shared source builds without this product policy retain the
ordinary upstream defaults.

| Boundary | Robot default | Explicit authority | Refusal behavior |
|---|---|---|---|
| WASIp1 namespace | absent | bind a named module/group and enable its interface | link fails before start |
| WASIp1 functions | absent until namespace enable | enable exposes the entire compiled registered ABI; operation rights remain empty | unknown names/wrong signatures fail validation; cached bridges recheck caller policy |
| stdin/stdout/stderr | unbound, zero rights | separate read/write grants and explicit metadata/readiness | WASI permission error before I/O |
| fd 0/1/2 | permanently reserved | trusted stdio binding only | close leaves tombstone; renumber cannot repurpose slots |
| args/environment | no inherited content | content and size access require explicit grants | no content or size disclosure |
| random | denied | explicit random permission plus a shared ledger; optional manual limits | reject before native RNG; zero-length calls count |
| clocks | denied | separate time/resolution/timer grants for each clock ID | enforced host granularity and budgets |
| poll | denied for ungranted fd/clock | whole batch must pass before host wait/results | optional manual subscription/wait limits; no partial authorization |
| file/directory | no preopens | explicit resource, base/inheriting rights, object/I/O limits | no path escape; Linux proc/sys traversal rejected |
| sockets | absent | bounded provider not implemented in this profile | explicit unsupported; no permissive fallback |
| process exit | denied | explicit process-exit grant | first exit code wins; default immediate process exit, with independently selected existing waits allowed to finish |
| signal/yield/spawn | denied | independent permissions; threads also need a configured host profile | Core threads do not authorize host spawn |
| ROS1/ROS2 | no module binding, no interfaces | explicit version namespace enable plus independent resource capability | link or per-call denial; Global requires explicit module membership |
| DEV debugging | absent | trusted launch `--management-profile dev` and independent control endpoint | cannot edit host policy/FD resources or restore their capsules |
| RELEASE debugging | permanently sealed | bounded diagnostics only | runtime APIs, typed wrappers, checkpoint and reconnect cannot enable debugger writes |
| production updates | absent | fixed local deployment key, exact signed transaction, real fence | independent update protocol; no debugger grant or rollback |

Robot-product link refusal emits one bounded constant diagnostic for a missing
namespace or function. Untrusted import names cannot inject terminal controls or
turn that refusal into an arbitrary output channel.

All numerical quotas default to unlimited after the independent base grant. A Wasm module cannot alter the host ledger, cancel a user's quota, or refill it through another module, thread or code epoch. Exhaustion either rejects or synchronously waits as configured; no asynchronous quota API is added. Per-request and non-replenishing ceilings cannot wait. There is no fixed 4096-byte guest path limit.

Namespace enablement and operation/resource grants are separate. An enabled
namespace exposes all its compiled registered interfaces, which still return
controlled permission errors for ungranted operations. A disabled namespace
has no resolving stubs. Per-function exposure options and policy lists are removed. New modules do not receive a privileged Global fallback.
A DEV guest can use its existing explicit capabilities after ordinary guest data
or code edits; this does not authorize edits to the host contract or resurrect a
retired credential.

The standard-stream provider accepts bounded pipes/stream sockets, with explicitly
nonblocking pipe input. A regular file or terminal supplied as stdio is not silently
substituted for a bounded provider. SDK supervisors capture guest streams through
bounded pipes and then write their own diagnostic files. This is separate from
VM management logging and from Guest rights.

General filesystem operations currently use the existing synchronous native
implementation with rights and I/O budgets. There is no claim that arbitrary
FUSE/network filesystems support cancellation or a hard native-operation deadline.
Long native work remains a real live lease; an update cannot fabricate its
completion or reclaim its memory. External watchdog/device safety remains outside
the guest and independent of update waits.

The local update signature proves authorization by the preconfigured deployment
key for this VM nonce and exact transition. It does not replace publisher PKI,
cosign verification, source audit, Git history, user approval or external durable
coordination. The supervisor performs those tasks before signing; the VM only
consumes bounded immutable bytes, its actual effective policy and a local permit.
An audit correlation identifier has no authority.

See [WASI command documentation](../command-line/wasi.md),
[debug profile documentation](../command-line/global-debug.md),
[ROS2 command documentation](../command-line/ros2.md), and
[the implemented production update protocol](../runtime/production-update.md).
The latter defines the supported function, immutable/configuration, same-layout
module preserve/reset/migrate and existing-resource policy transactions, and
explicitly rejects arbitrary module graphs, isolated concurrent epochs and
unknown native/ROS handoffs.
