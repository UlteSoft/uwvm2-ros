#!/usr/bin/env bash
set -euo pipefail
root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
exec python3 "$root/tests/ros/run_unit.py" --suite security --out "${ROS_TEST_OUT:-$root/build/ros-security}"
