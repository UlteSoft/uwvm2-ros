"""Input provenance for the executable debugger qualification; no build actions."""
from __future__ import annotations

import hashlib
import json
from pathlib import Path
import subprocess
import sys


def require(value, message, detail=None):
    if not value:
        raise RuntimeError((message, detail))


def sha(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def source_files(root):
    return {path.relative_to(root).as_posix(): sha(path)
            for path in sorted((root / "src").rglob("*"))
            if path.is_file() and path.name != ".DS_Store" and not path.name.startswith("._")}


def admission(root):
    require(sys.platform == "linux", "qualification may only execute on admitted SSH Linux")
    subprocess.run(["bash", str(root / "tools/ci/require_wasm3_test_cgroup.sh")], check=True)
    return {"membership": Path("/proc/self/cgroup").read_text(),
            "boot_id": Path("/proc/sys/kernel/random/boot_id").read_text().strip()}


def build_proof(root, binary, receipt):
    """Validate an ORIGINAL completed link receipt against the current tree.

    This checks consistency of real keeper records. It cannot establish the
    freshness of compilation merely because a caller supplied valid JSON.
    Runtime/main object provenance remains the keeper's prerequisite.
    """
    binary, receipt = binary.resolve(strict=True), receipt.resolve(strict=True)
    record = json.loads(receipt.read_text())
    require(Path(record["binary_path"]).resolve(strict=True) == binary and
            record["binary_sha256"] == sha(binary), "product differs from original link receipt")
    argv = record["link_argv"]
    require(record["link_returncode"] == 0 and isinstance(argv, list) and argv and
            all(isinstance(value, str) and value for value in argv), "successful original link argv required")
    outputs = [argv[index + 1] for index, value in enumerate(argv[:-1]) if value == "-o"]
    require(len(outputs) == 1, "one original linker output required")
    output = Path(outputs[0])
    if not output.is_absolute():
        output = Path(record["link_cwd"]).resolve(strict=True) / output
    require(output.resolve(strict=True) == binary, "original link did not produce supplied executable")
    before, after = (Path(record[name]).resolve(strict=True)
                     for name in ("source_before_file", "source_after_file"))
    require(sha(before) == sha(after), "original build changed its source/dependency fingerprint")
    fingerprint = json.loads(before.read_text())
    entries = fingerprint["files"]
    require(isinstance(entries, list) and entries and
            len({entry["path"] for entry in entries}) == len(entries), "unique build input entries required")
    canonical = json.dumps(entries, sort_keys=True, separators=(",", ":")).encode()
    require(fingerprint["source_id"] == "sha256:" + hashlib.sha256(canonical).hexdigest(),
            "build source identity does not hash its canonical file list")
    recorded = {entry["path"]: entry["sha256"] for entry in entries}
    current = source_files(root)
    require({name: value for name, value in recorded.items() if name.startswith("src/")} == current,
            "complete current production source differs from original build")
    for name, digest in recorded.items():
        path = Path(name)
        require(not path.is_absolute() and ".." not in path.parts and
                path.parts and path.parts[0] in ("src", "third-parties"), "unexpected build input path", name)
        require(sha(root / path) == digest, "original build dependency changed", name)
    return {"binary_sha256": sha(binary), "build_receipt_sha256": sha(receipt),
            "source_id": fingerprint["source_id"], "production": current,
            "fingerprint_paths": [str(before), str(after)], "dependency_count": len(entries),
            "fresh_object_compilation_established_by_this_runner": False}


def packet_pins(directory):
    return {str(path): sha(path) for path in sorted(directory.rglob("*"))
            if path.is_file() and "__pycache__" not in path.parts and
            path.name != ".DS_Store" and not path.name.startswith("._")}
