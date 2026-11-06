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

WASIp1 uses explicit Single or Group policies:

~~~text
--wasip1-single-create MODULE
--wasip1-single-enable MODULE
--wasip1-single-interface MODULE API hidden|deny|allow

--wasip1-group-create GROUP
--wasip1-group-enable GROUP
--wasip1-group-add-module GROUP MODULE
--wasip1-group-interface GROUP API hidden|deny|allow
~~~

`API` is an exact registered function, for example `fd_write`, `random_get`,
`fd_write_wasm64`, or `thread-spawn`. The memory32 and memory64 names are
independent. WASIp1 Global bindings are unavailable in the ROS product.

ROS1 and ROS2 use the same `single`, `group` and existing explicit `global`
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
