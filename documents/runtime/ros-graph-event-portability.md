# Graph and readiness-stream qualification

The production route consists of capability admission, the canonical carrier,
a worker-local graph dispatcher, a retained weak admission context, and the
existing sealed process pair's finite query channel. A host-local event stream
retains explicit source references; it never grants their operations. Native
graph callbacks retain their admission owner through queue retirement and do
not form a dispatcher/provider ownership cycle.

Actual sealed Jazzy workers are exercised by
`communication_worker_observation_deployment.cpp` through the same deployment
configuration, host router and pair supervisors used by the VM. The test checks
filtered native rows, exact context/node origins, short-output preflight,
failed-page delivery and retry, graph-watch incarnation changes, cross-worker
subscription observation, stream reservation rollback, failed event delivery,
close receipt replay and independent cleanup after source invalidation.

The `observation-deployment-subscription` receipt in the SDK temporary security
workspace records successful Clang ASan/UBSan and GCC builds and executions.
The `event-close-linearization` receipt additionally records TSan. Its concurrent
revoke test checks that the copying authority lease blocks revocation during
the sink and that every later poll is denied. A revocation that linearizes
between copy completion and the final origin check may reject that poll; the
test does not assume that final check wins the scheduler race.

The `observation-debug-contract` receipt verifies the actual C++ formatter,
independent DAP schema validation and editor controller fixtures. It does not
claim that a graphical IDE session or a live VM inspection was exercised.
`run_scene_observations.py` generates actual guest programs for memory32 and
memory64 with memory index zero and one, FP-control checks, invalid ranges,
short outputs, zero-authority denial and finite CLI configuration. Prepared
programs alone are not VM execution evidence; execution receipts must identify
the exact `-O3 -g` binary and immutable source manifest.

Every compiler and process in these receipts runs within the existing 64 GiB,
zero-swap cgroup. Native tests use the sealed process pair and the declared
supervisor stack/poll cadence. CPU placement is explicit test configuration;
these measurements do not establish exclusive cores, NUMA locality or a hard
real-time bound. Graph RPC shares a serialized channel with other operations,
so deployment latency qualification must include contention and bounded query
sizes. No additional event or graph supervisor thread is created.

The currently connected readiness sources are subscriptions, actions and graph
watches. Service, sensor, model, lifecycle and control source adapters require
separate production integration. A readiness record contains no source payload
and establishes neither physical safety nor successful actuator stopping. The
private graph snapshot retains its counted observer through deployment
retirement, including after watch close. Native graph discovery does not
provide authentication or authorize communication with discovered nodes.

These tests do not establish full ROS ecosystem portability, industrial safety
certification, native parameter-service replication, generic ROSIDL support or
Windows/macOS native provider support. The executable capability inventory and
these evidence levels are intentionally distinct.

## Executed VM and node-startup evidence

`observation-vm-budget-order/results.json` records 43 successful actual VM/CLI
cases using the frozen `vm-observation-isolated` binary. All four memory layouts
ran in both engines, including floating-point state, invalid ranges, short
outputs, closed source origins and mandatory budget omissions. The separate
`observation-modules-llvm` and `observation-modules-interpreter` receipts qualify
the named-module exports and full FP boundary for each mode.

A deterministic EINTR injection exposed an additional worker-lifetime defect:
an allocated but uncommitted logical node stopped its worker on destruction.
`node-commit-interrupted-before` retains the failing reproduction.
`node-commit-interrupted-fixed` passes Clang ASan/UBSan and GCC with real sealed
workers: interruption leaves the stage attached, retry commits both nodes, and
retirement confirms actual process cleanup. A provisional allocation now gains
its lifetime effect only after stage commitment.

`node-failure-evidence-terminal` separately qualifies preservation of terminal
startup errors through cancellation and retirement, including unknown effect
and cleanup debt. It passes Clang ASan/UBSan, GCC and TSan. This preserves a
failed operation's reason; it does not erase a prior cancellation or infer
cleanup from elapsed time.

`action-vm-retry-prepared/results.json` records 41 successful actual Action
VM/CLI cases on `vm-observation-retry` after the lifetime fix. The independent
`action-parent-release-retry-final` native receipt verifies that released action
drafts no longer retain the parent's counted native node. These receipts
qualify six action carriers; they precede cancellation-set integration.

## Explicit cancellation-set execution

`selection-vm-prepared/results.json` records 41 successful VM/CLI cases and
sixteen guest compilations for the separate `action.cancel.set` carrier. Its
frozen `vm-action-selection` executable has SHA256
`c35587cdf14570cd3b8f5d80a203f518b29b76db8093d990724facedd472a59f`.
Memory32/memory64 and memory indices zero/one run through both engines, with
real isolated ROS goals, cancel decisions and terminal cancelled results.
Failed guest delivery recovers the same aggregate and per-goal request identities
without resending cancellation; settlement may refine cached result states.
The LLVM and interpreter named-module cohorts each pass 22 commands. The
recoverable-allocation cohort separately passes six commands.

`action-selection-bounds-final` passes nine Clang ASan/UBSan, GCC and TSan
commands. It checks duplicate/foreign/empty selections, pre-effect output
capacity, all-rejected and partially accepted sets, unknown outcomes, exact
retry, closed schema origins, deadlines and exhausted optional quota. Selection
is bounded to 64 explicitly owned goal handles and the immutable endpoint goal
capacity. The aggregate receipt does not imply atomic remote cancellation or a
physical stop. Foreign UUID, timestamp and wildcard cancellation remain a
separate unimplemented legacy-equivalence obligation.
