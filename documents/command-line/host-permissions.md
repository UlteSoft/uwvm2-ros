# Exact WASIp1 / ROS1 / ROS2 interface authority

All three namespaces start disabled in `uwvm2-ros`. Enabling a namespace does
not expose its functions. Each registered function has exactly one state:

- `hidden` (default): the function is unavailable at import validation/linking.
  The load fails with `[error]` before guest execution.
- `deny`: the exact ABI is available for validation. A call returns WASI
  `ENOTCAPABLE` or the ROS permission-denied status without performing the
  operation. The host emits `[warning]` by default.
- `allow`: the call may proceed through its existing operation, descriptor,
  capability, resource, identity and quota checks. This state grants none of
  those underlying rights.

`proc_exit` cannot return an errno; a denied call produces a controlled guest
failure and a diagnostic. The WASI Threads extension separately requires
`--wasm-feature-enable-threads --wasi-threads ...`; exposing `thread-spawn`
does not create its execution profile or grant thread creation.

## Commands

WASIp1 uses Single, Group and an implicit default Global policy:

~~~text
--wasip1-single-create MODULE
--wasip1-single-enable MODULE
--wasip1-single-interface MODULE API hidden|deny|allow

--wasip1-group-create GROUP
--wasip1-group-enable GROUP
--wasip1-group-add-module GROUP MODULE
--wasip1-group-interface GROUP API hidden|deny|allow

--wasip1-global-enable
--wasip1-global-interface API hidden|deny|allow
~~~

`API` is an exact registered function, for example `fd_write`, `random_get`,
`fd_write_wasm64`, or `thread-spawn`. The memory32 and memory64 names are
independent. Every module without a Single or Group binding selects the same
Global environment. Global starts disabled, with every interface hidden.
`global-enable` enables its namespace; each interface and operation still needs
its own grant. `global-add-module` is an optional explicit membership declaration.
Single and Group targets never inherit Global grants, descriptors or limits.

Every target supports the same resource and environment options. For Global,
use `--wasip1-global-` followed by `grant`, `deny`, `limit`, `limit-action`,
`limit-exit-action`, `set-fd-limit`, `set-argv0`, `force-args`,
`add-or-replace-environment`, `delete-system-environment`,
`inherit-system-environment`, `noinherit-system-environment`, `mount-dir`,
`mount-dir-rights`, `socket-rights`, `socket-tcp-connect`, `socket-tcp-listen`,
`socket-udp-bind`, `socket-udp-connect`, `enable-utf8-check` or
`disable-utf8-check`. Arguments match the corresponding Group option with its
leading group name omitted. `global-disable` explicitly hides the namespace.
No legacy ambient defaults or short aliases are enabled by these options.

Members share their policy's argv, environment, fd table, socket preopens and
quota counters, while their Wasm linear memories remain independent. A named
Group literally called `global` is distinct from Global. A module cannot belong
to two explicit Single/Group targets. Creating a Single or assigning a Group
removes the module from Global even when its new policy is disabled. Duplicate
Single declarations and duplicate Group members are startup errors. Limits
remain unlimited until configured explicitly.

ROS1 and ROS2 use the same `single`, `group` and default `global`
policy scopes. Replace `FAMILY` with `ros1` or `ros2`:

~~~text
--FAMILY-single-interface MODULE NAMESPACE.FUNCTION hidden|deny|allow
--FAMILY-group-interface GROUP NAMESPACE.FUNCTION hidden|deny|allow
--FAMILY-global-interface NAMESPACE.FUNCTION hidden|deny|allow
~~~

Create and enable the chosen target first. ROS keys retain their full namespace,
for example `uwvm_ros1_v1.capability_open` and
`uwvm_ros2_rmw_srv_v1.client_available`. Portable robot interfaces belong to the
ROS2 family. Unknown names, cross-family keys, duplicate state declarations and
wildcards are errors. The authoritative inventory is
[host_interfaces.h](../../src/uwvm2/utils/control/host_interfaces.h).

Every module is checked against its own effective policy. A preload cannot
borrow its caller's Single permission. Only modules explicitly assigned to a
Group share that Group. Policies, checkpoints and host update digests include
the exact interface states.

A ROS Group or the default Global target owns one shared environment:
connection/session, capability handles, event state, RMW resource associations,
and operation and audit budgets. Any member may use or close a handle created
by another member, subject to the same interface and operation checks. Different
Single targets and differently named Groups have independent environments and
budgets. Identical ROS1 and ROS2 names never merge their environments. Replacing
one member retires that member's authority without closing its peers' shared
resources or refunding consumed credits. Closing a handle invalidates it for
all members of its environment.

A defined Wasm export executes with its defining module's policy, as WASIp1
exports do. A raw re-exported host function uses the actual executing caller's
policy and transfers no authority. A Single target may explicitly select
`--FAMILY-single-call-boundary MODULE same-module` to refuse cross-module
ancestry; `module-exports` is the default. These checks never let Wasm choose a
native caller identity or address another process's ROS session.


For a guest importing only `random_get`, this grants unlimited random access:

~~~sh
uwvm2-ros --wasm-set-main-module-name main \
  --wasip1-single-create main --wasip1-single-enable main \
  --wasip1-single-interface main random_get allow \
  --wasip1-single-grant main random \
  --run random.wasm
~~~

For a libc program, declare each of its actual imported functions separately.
Permitting an unused libc import does not grant its underlying operation.

## Diagnostics and selectors

Hidden imports always fail with `[error]`. The diagnostic distinguishes a
disabled namespace from a hidden function and prints the applicable `create`,
`enable`, and `interface` switches. A genuinely unregistered function is reported
as unknown; granting a permission cannot make it exist.

Exposed denied calls warn by default. Quota rejection and synchronous waiting
warn only when enabled. Limits remain unlimited unless explicitly configured;
warning settings do not change their values or their reject/wait action.

~~~text
--enable-warning SELECTOR
--disable-warning SELECTOR
--warning-as-error SELECTOR
~~~

Supported host selectors are:

- `all_host_calls`: denied and limited calls across all families.
- `all_host_denied`: permission denials only.
- `all_host_limited`: actual quota rejection or synchronous waiting only.
- Any of those three followed by `_wasip1`, `_ros1`, or `_ros2`.
- `host_call_FAMILY.API`: both classes for one exact API.
- `host_denied_FAMILY.API`: denials for one exact API.
- `host_limited_FAMILY.API`: quota events for one exact API.

For ROS, `API` includes its namespace. For example:

~~~text
--disable-warning all_host_calls
--enable-warning all_host_denied_ros2
--enable-warning host_limited_wasip1.random_get
--warning-as-error host_denied_ros2.uwvm_ros2_v1.capability_open
~~~

The last matching host selector wins. Existing `all` also selects every host
diagnostic, including quota events. Promotion to error requests controlled
process exit; it never silently grants a denied operation.

Messages identify the family, exact API, policy scope and target, reason,
suggested host command, and exact warning selector. Quota messages identify the
exhausted field and whether the operation was rejected or is waiting. Resource
denials direct the operator to the corresponding grant/contract. Messages use
the selected `--log-output` destination. Closed/full diagnostic pipes and
allocation failure do not change guest errno or crash the host; explicitly
selected error promotion still applies when a diagnostic cannot be written.

There are no aliases for the removed logging option spellings or compatibility
defaults that expose a whole family. Existing deployment files must be migrated.

Portable Robot handles follow the same policy identity. The first opener must
match the selected deployment's exact module and Wasm hash. Other members of
that same Group/Global may use its handle with their own granted interfaces;
they cannot replace the deployment binding. Another policy cannot use it.
The interpreter and LLVM threaded profile retain the actual calling module
identity and use checked shared-memory copies.

The removed `--wasip1-*-trace` controls have no compatibility aliases. Use the
current explicitly configured debugger/API audit facilities for observations.
