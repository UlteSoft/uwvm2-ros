# Bounded Action cleanup ownership

`unit.cc` tests the shared cleanup pool with a small goal-state stub: capacity,
duplicate IDs, release/reuse, concurrent insertion, and releasing the last
owner outside the pool mutex. These tests do not execute ROS or DDS.

Run `run_unit.py --source SOURCE --out NEW_OUTPUT --compiler CLANGXX` on Linux
inside the existing 64 GiB cgroup. SOURCE should be a small immutable overlay
containing `gateways/ros2/action_cleanup.h` and this test directory. The runner
builds C++17 normal and ASan/UBSan profiles, captures each actual executable,
then resumes its initial SIGSTOP handshake. Invoke the runner rather than the
test executable directly. Each profile has eight test groups.

Native lifetime behavior is tested separately by `../ros_action_late_close`.
