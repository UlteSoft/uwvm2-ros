# Guest ROS authority and trusted module replacement

The C, C++, Rust, JavaScript, AssemblyScript and TinyGo SDKs use the same finite
`uwvm_ros1_v1` or `uwvm_ros2_v1` import registry. Neither registry exports a
Runtime update, debugger, policy configuration, module replacement or
management function. SDK objects, callbacks and event executors cannot create
those entry points. An import with an unregistered name cannot resolve as a
ROS Host function.

`capability_revoke` lets a Guest give up its own existing ROS alias. A separately
granted ROS2 `lifecycle_change_state` controls the selected native ROS node's
configured transition. Neither operation changes Runtime code, launch policy,
debugger configuration or the trusted module identity.

## Output and management use separate channels

Guest WASI stdout/stderr contain application bytes. Printing `pause`, `quit`,
`replace`, `wasm-script`, a DAP JSON request or a policy option does not submit
a management request. The Runtime's host console uses its separately owned,
authenticated control channel. An application embedding the Runtime must keep
that channel separate from Guest standard input/output and ROS log messages.
It must not feed Guest output into a console parser or a shell.

The real debugger integration Guest deliberately prints command-looking lines
through `fd_write`. Its runner requires the unchanged text, the later Guest
checkpoint and a running controller queried over the actual authenticated
control descriptor. This assertion applies only after that particular real
integration job executes successfully. A DAP output-event serialization model
alone does not prove a Runtime transport has executed.

## Successful replacement retires the module's ROS authority

Trusted replacement validates and commits code through the existing Runtime
management API. At a genuine successful commit, the Runtime permanently marks
the affected compiled module's current incarnation with
`ros_authority_retired`. This applies to the entire affected module, including
its other functions. It is set before publishing the new entry points. A
preparation or commit failure does not set this flag.

The Runtime primary ROS caller-identity API then refuses that incarnation.
Read-only observed identity remains available for bounded denial diagnostics;
it cannot authorize a Guest operation or recreate an owner. The trusted native
post-commit hook drains already admitted local Host operations, invalidates old
handles, closes both backend connections and discards the module's local queued,
pending and reserved event data. Existing watch request IDs are removed.
Diagnostic rows retain their original identities, counters and explicit retired
state. Another module's registrations, handles and queues remain independent.

The cleanup hook runs after leaving the stopped-domain publication mutex while
the Guest is still paused, before the host acknowledges the replacement. It
does not call Guest code or issue a Gateway RPC. An already admitted remote
effect may have occurred; retiring local authority does not undo a native ROS
effect or justify replaying a timed-out operation.

Reusing ROS requires a **fresh trusted Runtime launch with explicit CLI policy**.
There is no SDK re-enable, regrant, session reconnect or quota-reset operation
for an updated incarnation. Successful replacement does not reuse the old
aliases for new code. A fresh launch without grants remains default-denied.
Whole Runtime retirement runs after execution drain and clears its old Host
registrations before the old Runtime epoch/storage is retired.

## Debugger references are temporary observations

The DAP adapter clears all stopped frame/scope/object references before sending
either `replace` or `wasm-script`, including malformed requests and requests
whose transport later fails. Old numeric IDs are never recycled. After an
acknowledged `replace`, only a complete authoritative stopped status can publish
a new cohort; an incomplete refresh leaves every old reference invalid. A
`wasm-script` transcript does not publish a stopped snapshot. Obtain fresh
status/scopes before reading retired ROS metadata.

This conservative DAP reference rule does not claim a failed replacement
retired ROS authority. Only the genuine successful Runtime commit changes that
authority. `uwvm/rosState` and the ROS Variables scopes remain read-only and
cannot reopen a handle, consume an event, restore a quota or reconnect a session.

The regression sources and their exact evidence limits are described in
[`tests/ros_debugger/README.management.md`](../../tests/ros_debugger/README.management.md).
Source recipes and authored models are not executed ROS, official upstream
test coverage, foreign-architecture acceptance or a current PASS.
