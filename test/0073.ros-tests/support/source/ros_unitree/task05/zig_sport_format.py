#!/usr/bin/env python3
"""Actual twice-idempotent Zig formatting; no compilation or SDK acceptance."""
from pathlib import Path
import argparse
import hashlib
import json
import shutil
import subprocess
import tarfile


def pin(path):
    body = path.read_bytes()
    return {"bytes": len(body), "sha256": hashlib.sha256(body).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--snapshot", type=Path, required=True)
    parser.add_argument("--zig", type=Path, required=True)
    args = parser.parse_args()
    names = ("sdk/origin/robots/unitree/language/zig/unitree.zig",
             "sdk/origin/robots/unitree/examples/zig/sport_topic_command.zig",
             "test/0073.ros-tests/support/source/ros_platform05/unitree/zig_sport_values.zig")
    work = Path.cwd()
    before = {name: pin(args.snapshot / name) for name in names}
    tool = pin(args.zig)
    version = subprocess.run([str(args.zig), "version"], check=True, capture_output=True).stdout.decode()
    rows = []
    for i, name in enumerate(names):
        target = work / "formatted" / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(args.snapshot / name, target)
        for pass_name in ("first", "second"):
            with (work / (str(i)+"-"+pass_name+".stdout")).open("xb") as out, (work / (str(i)+"-"+pass_name+".stderr")).open("xb") as err:
                subprocess.run([str(args.zig), "fmt", str(target)], stdout=out, stderr=err, check=True)
            if pass_name == "first":
                once = pin(target)
            elif pin(target) != once:
                raise ValueError("actual Zig formatting is not idempotent")
        rows.append({"path": name, "before": before[name], "after": once})
    if pin(args.zig) != tool or any(pin(args.snapshot / name) != value for name, value in before.items()):
        raise ValueError("original source/formatter input changed")
    with tarfile.open(work / "source-candidate.tar", "w") as archive:
        for name in names:
            archive.add(work / "formatted" / name, arcname=name, recursive=False)
    result = {"status": "PASS_ZIG_FORMAT_ONLY", "tool": {"path": str(args.zig), **tool},
              "version": version, "sources": rows, "compiler_runs": 0, "Guest_runs": 0,
              "native_ROS_runs": 0, "default_authority": []}
    (work / "result.json").write_text(json.dumps(result, indent=2)+"\n")
    print(json.dumps({"status": result["status"], "formatted_sources": len(rows)}))


if __name__ == "__main__":
    main()
