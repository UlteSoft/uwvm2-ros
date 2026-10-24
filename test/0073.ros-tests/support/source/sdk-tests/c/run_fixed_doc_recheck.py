#!/usr/bin/env python3
"""Replay the three fixed-v1 headers and original/adapted wire fixture.

The fixed decoder's distinct-storage rule is a caller precondition, unlike
bounded-v2's explicit overlap rejection. This focused replay checks the later
return-contract correction without claiming a new all235-header compiler run.
"""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys


if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
    raise RuntimeError("shared admitted Linux finite owner required")
run = Path(os.environ["UWVM_RESOURCE_RUN"])
inputs = run / "inputs"
selected = {}
for line in (inputs / "generated-headers-current.sha256").read_text().splitlines():
    sha256, spelling = line.split("  ", 1)
    relative = Path(spelling)
    if relative.name != "fixed_records.h":
        continue
    source = inputs / "raw" / relative
    if hashlib.sha256(source.read_bytes()).hexdigest() != sha256:
        raise RuntimeError("pinned raw fixed header changed")
    selected[relative.as_posix()] = {"path": str(source), "sha256": sha256}
if len(selected) != 3:
    raise RuntimeError("exact three fixed-v1 header denominator required")
mapping = run / "work/fixed-input-map.json"
mapping.write_text(json.dumps(selected, indent=2) + "\n")
command = [sys.executable, "-I", "-S", "-B", inputs / "generated_portability.py",
           "--sdk", "/home/macromodel/Documents/uwvm-test-261018/agents/02/c-format-r13/work/source",
           "--test-sources", inputs, "--output", run / "work/generated", "--stream",
           "--adapter", inputs / "c_profiles.py", "--generated-input-map", mapping]
with (run / "work/recheck.stdout").open("wb") as stdout, (run / "work/recheck.stderr").open("wb") as stderr:
    result = subprocess.run([str(value) for value in command], stdin=subprocess.DEVNULL,
                            stdout=stdout, stderr=stderr, timeout=25, check=False)
report = {"format": "task02-fixed-decode-documentation-recheck-v1",
          "command": [str(value) for value in command], "exit_code": result.returncode,
          "header_denominator": 3, "expected_compiler_wire_checks": 26,
          "documentation_helper_sha256": hashlib.sha256((inputs / "c_api_documentation.py").read_bytes()).hexdigest(),
          "full_235_header_current_compiler_replay": False,
          "native_ros_runs": 0, "gateway_runs": 0, "physical_motion_runs": 0}
(run / "work/recheck.json").write_text(json.dumps(report, indent=2) + "\n")
sys.exit(result.returncode)
