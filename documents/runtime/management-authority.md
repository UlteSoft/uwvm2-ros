# Robot management authority

The robot product (`UWVM_ROS_ZERO_HOST_DEFAULTS`) starts with no WASIp1, ROS1 or
ROS2 interface exposure or operation authority. Selecting DEV, diagnostics or
updates does not change that contract. Base namespace enablement supplies its
ABI; individual host operations remain separately granted. Explicit operation
grants default to unlimited quotas. The ordinary uwvm2 product retains its own
launch defaults; these robot-only defaults and RELEASE debug sealing are gated
by the product configuration.

| Authority | DEV debugger | RELEASE diagnostic observer | Signed production update | Guest Wasm |
|---|---|---|---|---|
| Inspect/mutate guest memory, locals and typed state | Within valid stopped DEV domain | Denied | No general primitive | Its ordinary Wasm semantics |
| Pause/step/break/evaluate/script | Granted DEV operations | Denied | Private fence only | Denied |
| Replace guest function | Validated DEV replacement | Denied | Exact signed scope/bytes | Denied |
| Reconstruct module source | Validated main/preload world rebuild within supported profile | Denied | Supported declared module transition only | Denied |
| Read approved metadata | DEV stopped views | Bounded redacted snapshots | Transaction facts | Only explicitly exposed guest ABI |
| Change host permissions/quotas | Denied | Denied | Separately approved signed policy transition | Denied |
| Restore closed descriptors or retired credentials | Denied | Denied | Denied | Denied |
| Undo an authenticated COMMIT | Not a production update API | Denied | Denied | Denied |
| Enable full debugging after RELEASE seal | Denied | Denied | Denied | Denied |

DEV approval applies to a mutable guest execution domain, not to an unchanged
publisher-signed artifact. Function editing keeps that domain's existing grants,
quota consumption and revocation. Full-world rebuild keeps the exact supported
WASI environment/resource owners; it does not reconstruct startup grants. DEV
state cannot be promoted into a trusted production deployment using a debugger.

A RELEASE diagnostic endpoint is a separate launch capability. A production
update endpoint additionally requires exact domain-separated Ed25519 permits
bound to the instance nonce, actual base/target bytes, declaration, effective
contract, prepared/READY measurements, policy/revocation generations, deployment
scope, expiry and monotonic transaction decisions. Neither endpoint gives the
other role's authority. Full debug denial remains in the controller and cold
runtime APIs, including status-wrapped actions, checkpoint and source mutation.

The supervisor owns durable decisions, latest-deployment recovery, external
review and device safety. cosign/publisher validation and uwgit/history/diff stay
outside the VM. The VM does not consume Git/repository/registry fields, run those
tools or require the SDK as a build dependency. A successful local COMMIT receipt
is not evidence of a globally atomic Gateway/device transition.

The currently supported production backend is Linux LLVM full, one owned module,
exclusive drain and publication. Shared/imported state, preloads, ROS/native
provider handoff, arbitrary graph/layout replacement, isolated concurrent epochs
and WSEA-required updates are explicitly refused. These exclusions preserve
actual native ownership and state safety; they are not implicit permission
fallbacks. In-flight work cannot be declared complete by cancellation or timeout.

Detailed interfaces and concrete restrictions:

- [Linux target ABIs and native Unwind](ros-linux-debug-targets.md)
- [DEV world rebuild](dev-world-rebuild.md)
- [RELEASE diagnostics](release-diagnostics.md)
- [Production update wire protocol and state transitions](production-update.md)
- [WASIp1 operations and quotas](../command-line/wasi.md)
- [ROS1 permissions](../command-line/ros1.md) and [ROS2 permissions](../command-line/ros2.md)
