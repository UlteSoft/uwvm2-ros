# Endpoint-free production Node composition

The production Gateway uses `composed_node` from `node.h`. It constructs the
installed SDK's public NodeBase, graph, logging, timer, topic, service, clock,
parameter and waitable interfaces directly. It does not construct a standard
`rclcpp::Node` or its NodeTimeSource interface.

This distinction is required for the default-denied policy. The original Humble
[Node constructor](https://github.com/ros2/rclcpp/blob/humble/rclcpp/src/rclcpp/node.cpp)
unconditionally attaches a TimeSource; that
[TimeSource](https://github.com/ros2/rclcpp/blob/humble/rclcpp/src/rclcpp/time_source.cpp)
subscribes to `/parameter_events` independently of the options controlling
parameter services and parameter event publication. A matching foreign parameter
event can enable its simulation-clock reader. Disabling publication alone does
not establish an endpoint-free Node.

`composed_node` checks the options before constructing NodeBase. ROS output,
parameter services, parameter event publication and topic statistics must all be
disabled. The default constructor options are quiet. No TimeSource or implicit
simulation-clock subscription is constructed or subsequently detached. A native
ROS clock retains its SDK system-time fallback; a `time.read-ros` operation does
not automatically grant `/clock` access. Explicit simulation-clock integration
requires its own reviewed resource and authority contract.

Every actual Topic, Service and Action still goes through the existing exact
type/codec identity, manifest, quota, actual QoS, session and ownership checks.
The composition adds no operation, import, capability or default vendor. Local
parameter declarations remain local; they do not enable remote parameter
services or a parameter event publisher.

The production CMake target and attached native adapter libraries select
`UWVM_ROS2_USE_COMPOSED_NODE=1` coherently. `main.cc` also requires this selection
when built outside that target and rejects an explicit zero selection. Native
SDK witnesses may keep the original Node type through the separate default
`node_type` alias; their explicitly authorized fixture entities do not qualify a
production Gateway's default-denied behavior.

The `quiet_node` compile/link probe builds the actual composed public interface
constructor, rather than checking only whether three NodeOptions setters exist.
The live protocol witness additionally checks the original Gateway's real graph
with zero grants before and after foreign `use_sim_time` events, and checks that
no `/clock` reader appears. Source availability or a compile-only probe alone is
not a runtime result. SDK constructor compatibility and transport behavior must
be qualified separately for each actual distribution and architecture.
