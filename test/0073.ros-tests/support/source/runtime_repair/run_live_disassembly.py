#!/usr/bin/env python3
"""Exercise the matching native runtime, broker and DAP adapter on Linux.

@details Reuse the maintained genuine native-window witness: 400 instruction
rows, actual instruction stepping, stale references and copied/executed bytes.
This is protocol execution evidence, not a claim that VS Code's UI was tested.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys

DRIVER = Path(__file__).resolve()
ROOT = DRIVER.parents[5] if len(DRIVER.parents) > 5 else None


def verify_source_inputs(root, manifest):
    """@brief Verify every frozen VM input, with or without the historical vm/ prefix.

    @details A valid manifest identity alone proves no relationship to the files
    used by a debugger session. Require a nonempty inventory, one entry per
    resolved source path, the exact size and the complete SHA-256. Paths must
    remain inside the explicitly selected source tree. No entry is skipped.
    """
    if not isinstance(manifest, list) or not manifest:
        raise ValueError("The VM source manifest must contain file entries")
    root = root.resolve(strict=True)
    seen = set()
    total = 0
    for row in manifest:
        relative = row["path"]
        if relative.startswith("vm/"):
            relative = relative[3:]
        parts = relative.split("/")
        if any(part in ("", ".", "..") for part in parts):
            raise ValueError("Noncanonical VM source path: " + row["path"])
        file = (root / relative).resolve(strict=True)
        if not file.is_relative_to(root) or file in seen:
            raise ValueError("Escaping or duplicate VM source path: " + row["path"])
        seen.add(file)
        if not file.is_file() or file.stat().st_size != row["bytes"]:
            raise ValueError("VM source size differs: " + row["path"])
        with file.open("rb") as stream:
            digest = hashlib.file_digest(stream, "sha256").hexdigest()
        if digest != row["sha256"]:
            raise ValueError("VM source SHA-256 differs: " + row["path"])
        total += row["bytes"]
    return {"files": len(seen), "bytes": total}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("uwvm", "wat2wasm", "build-receipt", "source-manifest", "out"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--ros-version", type=int, choices=(1, 2),
                        help="Exercise the native debugger while the loop calls a real ROS builtin")
    parser.add_argument("--call-stack", choices=("unwind",), default="unwind")
    parser.add_argument("--source-root", type=Path, default=ROOT,
                        help="Immutable checkout used by the recorded build, independent of this driver's location")
    args = parser.parse_args()
    if args.source_root is None:
        parser.error("--source-root is required when the driver is outside the repository")
    root = args.source_root.resolve(strict=True)
    sys.path.insert(0, str(root / "test/0018.debugger"))
    import run_dap_current_broker as live
    args.out.mkdir(parents=True, exist_ok=False)
    report = {"passed": False, "scope": "Linux x86_64 real native broker/DAP; VS Code UI untested"}
    session = None
    try:
        assert sys.platform == "linux" and os.uname().machine == "x86_64"
        cg = Path("/sys/fs/cgroup")
        report["cgroup"] = {name: (cg / name).read_text().strip()
                            for name in ("memory.max", "memory.swap.max", "cgroup.procs")}
        assert report["cgroup"]["memory.max"] == "68719476736"
        assert report["cgroup"]["memory.swap.max"] == "0"
        assert str(os.getpid()) in report["cgroup"]["cgroup.procs"].split()
        build = json.loads(args.build_receipt.read_text())
        assert Path(build["runtime"]["path"]).resolve() == args.uwvm.resolve()
        assert live.sha(args.uwvm) == build["runtime"]["sha256"]
        assert any(row["name"] in ("vm-build", "native-build", "build") and row.get("returncode") == 0
                   for row in build["commands"])
        manifest = json.loads(args.source_manifest.read_text())
        identity = hashlib.sha256(json.dumps(manifest, sort_keys=True, separators=(",", ":")).encode()).hexdigest()
        assert "sha256:" + identity == build["source"]["source_identity"]
        report["verified_source_inputs"] = verify_source_inputs(root, manifest)
        report["source_identity"] = build["source"]["source_identity"]
        report["runtime_sha256"] = live.sha(args.uwvm)
        report["driver_sha256"] = live.sha(Path(__file__))
        guest = args.out / "native-loop.wasm"
        fixture = Path(__file__).with_name("live_disassembly.wat")
        if args.ros_version:
            text = Path(__file__).with_name("ros_live_disassembly.wat").read_text()
            fixture = args.out / "ros-loop.wat"
            fixture.write_text(text.replace("uwvm_ros2_v1", f"uwvm_ros{args.ros_version}_v1"))
        command = [str(args.wat2wasm), "--debug-names", str(fixture), "-o", str(guest)]
        report["compiler_argv"] = command
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=60)
        (args.out / "wat2wasm.stdout").write_bytes(result.stdout)
        (args.out / "wat2wasm.stderr").write_bytes(result.stderr)
        assert result.returncode == 0
        report["guest_sha256"] = live.sha(guest)
        vm_args = ["--runtime-aot", "-Rct", "0", "-Rllvm-call-stack", args.call_stack,
                   "-Rllvm-cache-path", "disable"]
        vm_args += ["--run", str(guest)]
        report["ros_version"] = args.ros_version
        report["call_stack"] = args.call_stack
        session = live.BrokerSession(args.uwvm, root / "tools/debug/secure_server.py",
                                     root / "tools/debug/dap_adapter.py", vm_args, args.out)
        function_imports = ((f"uwvm_ros{args.ros_version}_v1", "capability_open", 0),) if args.ros_version else ()
        report["native_window"] = live.native_case(session, guest, function_imports=function_imports)
        report["passed"] = True
    except BaseException as error:
        report["failure"] = repr(error)
        raise
    finally:
        if session is not None:
            try:
                report["cleanup"] = session.close()
            except BaseException as error:
                report["passed"] = False
                report["cleanup_failure"] = repr(error)
        (args.out / "result.json").write_text(json.dumps(report, indent=2) + "\n")
    if not report["passed"]:
        raise RuntimeError("Native disassembly qualification or cleanup failed")


if __name__ == "__main__":
    main()
