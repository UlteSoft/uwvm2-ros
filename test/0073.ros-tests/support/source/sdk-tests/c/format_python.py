#!/usr/bin/env python3
"""Format the three owned C-tool Python sources with the pinned real Black.

Candidates are written only to this finite Linux run. Black's default safe AST
check stays enabled, and a second actual formatter pass must be byte-identical.
No current SDK or another owner's source/tool installation is changed.
"""

import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import tarfile


FILES = ("tools/c_profiles.py", "tools/c_api_documentation.py", "example/platform/ros/c/c/advanced_policy.py")


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        while block := stream.read(1 << 20):
            value.update(block)
    return value.hexdigest()


def main():
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    inputs = run / "inputs"
    work = run / "work"
    pins = json.loads((inputs / "black-tool-pins.json").read_bytes())
    tool = Path(pins["tool_root"])
    if not pins["tool_pins"]:
        raise RuntimeError("nonempty exact formatter dependency pins required")
    for relative, sha256 in pins["tool_pins"].items():
        if digest(tool / relative) != sha256:
            raise RuntimeError("pinned Black dependency changed: " + relative)
    source = work / "source"
    source.mkdir()
    with tarfile.open(inputs / "source.tar") as archive:
        archive.extractall(source, filter="data")
    config = source / "share/platform/ros/c/format/black.toml"
    launcher = work / "black_owned.py"
    launcher.write_text("import sys\nsys.path.insert(0, " + repr(str(tool / "usr/lib/python3/dist-packages"))
                        + ")\nfrom black import patched_main\npatched_main()\n")
    base = ["/usr/bin/python3", "-I", "-S", "-B", str(launcher)]
    commands = []

    def execute(label, argv, data=None):
        result = subprocess.run(argv, input=data, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=90, check=False)
        commands.append({"label": label, "argv": argv, "exit_code": result.returncode,
                         "stdout_sha256": hashlib.sha256(result.stdout).hexdigest(),
                         "stderr_sha256": hashlib.sha256(result.stderr).hexdigest()})
        if result.returncode:
            (work / "failure.stderr").write_bytes(result.stderr)
            raise RuntimeError("real formatter failed " + label)
        return result.stdout

    version = execute("black-version", base + ["--version"]).decode().strip()
    if version.partition(", ")[2] != pins["black_version"].partition(", ")[2]:
        raise RuntimeError("pinned actual Black/Python versions differ")
    rows = []
    for relative in FILES:
        path = source / relative
        original = path.read_bytes()
        argv = base + ["--config", str(config), "--stdin-filename", relative, "--quiet", "-"]
        formatted = execute("format-" + relative, argv, original)
        repeated = execute("idempotence-" + relative, argv, formatted)
        if formatted != repeated:
            raise RuntimeError("Black output is not byte-idempotent: " + relative)
        target = work / "files" / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(formatted)
        rows.append({"path": relative, "input_sha256": hashlib.sha256(original).hexdigest(),
                     "output_sha256": digest(target), "formatter_idempotent": True,
                     "safety": "Black default safe AST-equivalence and stability checks enabled"})
    report = {"format": "task02-owned-c-tools-python-format-v1", "status": "PASS_FORMAT_SCOPE",
              "formatter_version": version, "configuration_sha256": digest(config),
              "tool_pins_manifest_sha256": digest(inputs / "black-tool-pins.json"),
              "python_sha256": digest(Path("/usr/bin/python3")), "tool_root": str(tool),
              "files": rows, "commands": commands, "public_sdk_published": False,
              "export_wire_and_installed_acceptance": "SEPARATE_PENDING_GATES"}
    (work / "python-format.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({"status": report["status"], "files": len(rows), "version": version}), flush=True)


if __name__ == "__main__":
    main()
