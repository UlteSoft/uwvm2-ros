# ROS runtime tests

`test/0073.ros-tests/support/source/` is the repository's ROS extension test directory. It contains the
ROS 1 and ROS 2 runtime, gateway, SDK, policy, vendor, architecture and related
security contracts. The original upstream runtime tests remain in `test/`;
language and robot SDK self-tests remain in their existing SDK directories.

Run maintained commands with the `test/0073.ros-tests/support/source/` prefix. For example, the ROS unit
runner is `test/0073.ros-tests/support/source/ros/run_unit.py`, the distribution contracts are under
`test/0073.ros-tests/support/source/ros_distro/`, and platform SDK qualification is under
`test/0073.ros-tests/support/source/ros_platform_sdk/`. Read each suite's guide for its dependencies,
resource admission, output directory and actual acceptance scope.

For this work, execution belongs on `ssh linux` in the original hard 64 GiB,
zero-swap cgroup, using scratch space below
`$HOME/Documents/uwvm-test-261018`. Local source editing and inert evidence
transport do not qualify as test execution. Selecting a test or an SDK grants
no ROS authority; endpoint and backend permissions must still be explicit.

This directory was renamed from `tests/`. Historical result files, frozen
source snapshots and signed or hashed inventories retain their original bytes
and may therefore mention the old prefix. When locating a historical
repository file in this working tree, substitute `test/0073.ros-tests/support/source/` for its leading
`tests/` component. Do not rewrite archived hashes or claim historical results
as fresh validation of the renamed source. There is no compatibility symlink
at the old root path.
