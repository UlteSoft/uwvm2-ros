"""Finite read-only dependency inventory; no model, ROS or simulator starts."""

import importlib.util
import json
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys


def main():
    if os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("shared finite owner required")
    result = {
        "format": "uwvm-nvidia-native-dependency-observation-v1",
        "python": sys.version,
        "architecture": platform.machine(),
        "kernel": platform.release(),
        "cgroup": Path("/proc/self/cgroup").read_text().strip(),
        "tools": {},
        "packages": {},
        "test_cases_run": 0,
        "model_calls": 0,
        "native_ros_exchanges": 0,
        "guest_executions": 0,
    }
    for name in (
        "clang",
        "clang++",
        "gcc",
        "g++",
        "aarch64-linux-gnu-gcc",
        "cmake",
        "ninja",
        "wasm-validate",
        "nvidia-smi",
    ):
        executable = shutil.which(name)
        row = {"path": executable}
        if executable:
            command = (
                [executable, "--version"]
                if name != "nvidia-smi"
                else [
                    executable,
                    "--query-gpu=name,driver_version,memory.total,compute_cap",
                    "--format=csv,noheader",
                ]
            )
            probe = subprocess.run(command, capture_output=True, timeout=10, text=True)
            row.update(
                exit_code=probe.returncode,
                stdout=probe.stdout[:8192],
                stderr=probe.stderr[:8192],
            )
        result["tools"][name] = row
    for name in ("numpy", "rclpy", "isaacsim", "gr00t", "torch"):
        try:
            spec = importlib.util.find_spec(name)
            result["packages"][name] = {
                "present": spec is not None,
                "origin": spec.origin if spec else None,
            }
        except (ImportError, ValueError) as error:
            result["packages"][name] = {"present": False, "error": type(error).__name__}
    host_prefix = (
        "/host"
        if os.environ.get("UWVM_RESOURCE_PARENT", "").startswith("/host/sys/fs/cgroup")
        else ""
    )
    root = Path(
        host_prefix
        + "/home/macromodel/.local/share/uwvm-ros-extension/platform-sdk-inputs-20261020-r56-r2"
    )
    result["reference_ros_inputs_path"] = str(root)
    try:
        result["reference_ros_inputs"] = sorted(p.name for p in root.iterdir())
    except OSError as error:
        result["reference_ros_inputs"] = {
            "status": "UNREADABLE_FROM_UNPRIVILEGED_OWNER",
            "errno": error.errno,
        }
    print(json.dumps(result, indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
