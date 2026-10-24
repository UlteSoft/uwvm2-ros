#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
exec python3 "$root/test/0073.ros-tests/support/source/ros_cgroup/probe.py" --output "${ROS_TEST_OUT:-$root/build/ros-cgroup}/environment.json"
