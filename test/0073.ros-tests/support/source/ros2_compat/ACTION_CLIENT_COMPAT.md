# Explicit-options ROS2 Action client compatibility correction

This is source work only. No patch has been applied, no candidate imported, and
no compiler, test, SSH command, ROS context or endpoint has run during authoring.

The existing feature probe accepted `create_client<Action>(node, name)`, while
both production Action adapters supplied an additional explicit-options operand.
The official [Foxy factory](https://github.com/ros2/rclcpp/blob/foxy/rclcpp_action/include/rclcpp_action/create_client.hpp)
has the node/name/group overload without that options operand. Its public
[typed Client constructor](https://github.com/ros2/rclcpp/blob/foxy/rclcpp_action/include/rclcpp_action/client.hpp)
does accept the full native options. The corresponding
[Humble factory](https://github.com/ros2/rclcpp/blob/humble/rclcpp_action/include/rclcpp_action/create_client.hpp)
and [Jazzy factory](https://github.com/ros2/rclcpp/blob/jazzy/rclcpp_action/include/rclcpp_action/create_client.hpp)
are separate upstream API evidence. Distribution names do not select code.

The new C++17 SFINAE wrapper prefers the actual options-capable convenience
factory. If it is absent, the wrapper uses the public typed constructor with
the unchanged goal/result/cancel service and feedback/status topic QoS options,
then registers/removes that client through the public node waitables interface.
There is no default-options factory fallback. A weak waitables owner avoids a
Node ownership cycle. Successful registration publishes one owned lease; the
deleter unregisters once and destroys the native Client even after an exception.

The Action feature probe now instantiates and links the exact selected client
factory/registration/destruction route plus the explicit server-options route.
The probe is never run. An SDK without either public options-preserving route
gets an unavailable Action feature instead of a later unconditional Gateway
compile failure. No Guest right, native type identity, payload codec, action
goal/cancel/result policy or cross-module host-call guard changes.

`test/0073.ros-tests/support/source/ros2_compat/action_client_lease_contract.cc` contains eight SDK-free C++17
ownership cases using the production generic lease. It initializes no native
ROS object. The existing `UWVM_ROS2_BUILD_COMPAT_TESTS=ON` opt-in builds it as
`uwvm-ros2-action-client-lease-contract`; the default target set is unchanged.
Root can alternatively compile that source directly with an actual selected
C++17 compiler and `-UNDEBUG -pthread` inside the original 64-GiB zero-swap
cgroup. No executed result is claimed here. Actual Foxy/Humble/Jazzy/Lyrical
SDK compilation, native Action delivery and cleanup remain separate tests.

The audit also observed that custom Actions currently share the builtin
Fibonacci-dependent `UWVM_ROS2_HAVE_ACTION` feature. A minimal SDK without the
optional tutorial interface package consequently disables custom Actions even
if it has its own selected native Action types. That separate availability
coupling is not changed by this narrow explicit-options correction.
