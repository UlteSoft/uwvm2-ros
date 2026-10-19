# Separate public, builtin and selected custom Action availability

This source patch follows the independently reviewed Foxy explicit-options
client adapter. It preserves the builtin Fibonacci route and removes its
optional tutorial-package dependency from the selected custom Action route.
Neither native support selection nor a passing probe grants Guest authority.
The Runtime still begins with no ROS rights; every operation needs its explicit
alias, exact identities, five QoS contracts, quotas and owned lifetime.

| JSON flag | C++ definition | Actual configure observation |
| --- | --- | --- |
| `action_core` | `UWVM_ROS2_HAVE_ACTION_CORE` | public options, waitables and native Action library link, without tutorial types |
| `action` | `UWVM_ROS2_HAVE_ACTION` | original builtin Fibonacci client/server lifecycle with explicit options |
| `custom_action` | `UWVM_ROS2_HAVE_CUSTOM_ACTION` | every concrete row of the selected paired native catalog passes its full typed client/server lifecycle |

`custom_action=false` is the only conservative unconfigured default. Missing
selected headers, an empty Action macro, a mismatched registry/type/codec, an
unavailable public API, or a missing actual package link produces a failed
compile/link result. A message-only catalog remains usable for messages and
Services but does not make the Action flag true. Builtin Fibonacci absence does
not prevent a different qualified custom Action. Public API presence by itself
does not claim a concrete Action or an rclcpp source-compatibility level.

For a custom catalog, supply the existing three trusted CMake inputs:
`UWVM_ROS_CUSTOM_REGISTRY_HEADER`, `UWVM_ROS_CUSTOM_ROS2_HEADER` and
`UWVM_ROS_CUSTOM_ROS2_PACKAGES`. The absolute header paths must pass the existing
safe-path checks; packages remain an explicit duplicate-free list. The probe
uses both production compile definitions and the same selected package link
interfaces. It compares the complete ROS2 Action macro with the immutable common
registry and all three component descriptors, then instantiates actual native
Goal/Result/Feedback types and both client/server routes for every row. It sets
all five channel options and server result retention timeout explicitly. No
fallback creates an endpoint with SDK default QoS, and no probe executable runs.

The configured header and JSON are finalized after actual custom package/header
selection. Existing cross-build evidence checks both new cache entries and all
source inputs responsible for them. The native matrix configuration admission
rejects impossible concrete-without-core or custom-without-provider observations
without changing any observed value. Historical manifests keep their original
schema, source and compile results; this patch does not rewrite old receipts.

The optional CMake compatibility target `uwvm-ros2-action-catalog-contract`
contains eight SDK-free data groups against the actual production constexpr
pairing helper. `action_capabilities_models.py::suite(validate)` contains nine
injected-policy cases against `tools/ros_distro/probe.py::validate_ros2_action_features`.
A guarded Linux owner must load that function from the complete selected frozen
source and run the suite in its original 64 GiB, zero-swap cgroup. The factory
does not import project code or construct native objects. Existing lease tests
remain separate and unchanged. The native Action ticket fixture only needs the
public core flag because its correlation allocator creates no endpoint and does
not claim selected Action typesupport.

All regressions in this candidate are authored and unexecuted. Physical SDK
compile/link, both typed native routes, actual DDS goals/results/feedback/cancel,
exceptional registration/destruction and cross-architecture behavior remain
required actual tests. There is no all-distribution compatibility claim.
