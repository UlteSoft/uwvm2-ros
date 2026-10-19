#!/usr/bin/env python3
"""Run only the Action QoS policy unit through the existing cgroup ARM driver.

All run_unit.py source/SDK/QEMU/MMD/live mapping assertions remain enabled.
This wrapper selects the new C++26 source, excluding unrelated sorted-row runs.
"""
import sys

sys.dont_write_bytecode = True
import run_unit

run_unit.SUITES = [('action-qos-policy', 'tests/ros_qos/policy.cc')]
if '--skip-cpp17-sorted-rows' not in sys.argv:
    sys.argv.append('--skip-cpp17-sorted-rows')

if __name__ == '__main__':
    raise SystemExit(run_unit.main())
