# ROS1 legacy LowState and LowCmd applications

`lowlevel.c`, `.cpp`, `.rs`, `.go`, `.ts`, and `.mjs` use the real corresponding
Guest SDK with ROS1 backend 1. They consume the complete freshly generated
Noetic-bound eleven-type `unitree_legged_msgs` family. The adjacent value helpers
set every LowState/LowCmd field, including 20 motors and all reserved fields.
These sources are isolated transport examples, not robot-control commands.

The same-main command entries, exact finite protocol, complete build and native
input requirements are documented in
[the command guide](../../../../../tests/ros_unitree/legacy_command/README.md).
The closed source recipe is
[legacy-command-recipes.json](../../recipes/legacy-command-recipes.json).
The native counterpart is separately qualified by
[NATIVE_LEGACY_ROS1.md](../../../../../tests/ros_unitree/NATIVE_LEGACY_ROS1.md).

Build with actual complete generated files at the documented stage locations;
no manually copied vendor struct, placeholder MD5 or old guest-only native
identity is admissible. `LowState.crc` and `LowCmd.crc` equality is a transported
field check, not hardware CRC authentication. Compilation and future actual
native transport observations must be reported separately. The default test
instance has no ROS grants; the positive instance needs exactly the two explicit
fixture aliases, real native peers, and an isolated loopback ROS master.
