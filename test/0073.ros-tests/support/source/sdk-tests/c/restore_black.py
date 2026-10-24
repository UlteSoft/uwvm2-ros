#!/usr/bin/env python3
"""Restore the exact archived real Black closure into this owned finite run.

Only the published tool members are materialized as ordinary files. Original
Task01 inputs, archives and permissions remain unchanged. This prepares a tool;
it does not format source or supply language/build acceptance.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import subprocess
import sys
import tarfile


ARCHIVE_SHA256 = "00974fd79a23810556d47c9684286d468dd70e530b0893ae999ae8a56ea7722f"


def digest(path):
    value = hashlib.sha256()
    with path.open("rb") as stream:
        while block := stream.read(1 << 20):
            value.update(block)
    return value.hexdigest()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inspect-only", action="store_true")
    settings = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    run = Path(os.environ["UWVM_RESOURCE_RUN"])
    work = run / "work"
    pins_path = run / "inputs/black-tool-pins.json"
    pins = json.loads(pins_path.read_bytes())
    archive_path = run.parent.parent / "01/cpp-black-r42/resource-archives/evidence.tar"
    if digest(archive_path) != ARCHIVE_SHA256:
        raise RuntimeError("original acquired formatter archive changed")
    tool = work / "tool"
    if not settings.inspect_only:
        tool.mkdir()
    restored_bytes = 0
    with tarfile.open(archive_path) as archive:
        for relative, expected in pins["tool_pins"].items():
            name = PurePosixPath(relative)
            if name.is_absolute() or ".." in name.parts:
                raise RuntimeError("confined pinned tool member required")
            member = archive.getmember("work/tool/" + relative)
            # Dereference only within the immutable tar reader, never through
            # a filesystem symlink; verify the published bytes before writing.
            stream = archive.extractfile(member)
            if stream is None:
                raise RuntimeError("pinned tool member has no file bytes")
            body = stream.read()
            if hashlib.sha256(body).hexdigest() != expected:
                raise RuntimeError("archived Black dependency pin differs: " + relative)
            restored_bytes += len(body)
            if restored_bytes > 28 * 1024 * 1024:
                raise RuntimeError("formatter closure exceeds explicit restore scope")
            if not settings.inspect_only:
                target = tool / name
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_bytes(body)
                target.chmod(0o500 if member.mode & 0o111 else 0o400)
    if settings.inspect_only:
        result = {"status": "PASS_ARCHIVED_BLACK_MEMBER_HASH_AND_SIZE_INSPECTION_ONLY",
                  "source_archive_sha256": ARCHIVE_SHA256,
                  "published_input_pins_sha256": digest(pins_path),
                  "pinned_files": len(pins["tool_pins"]), "exact_expanded_bytes": restored_bytes,
                  "files_restored": 0, "source_files_formatted": 0}
        (work / "inspection.json").write_text(json.dumps(result, indent=2) + "\n")
        print(json.dumps(result), flush=True)
        return
    launcher = work / "black_restored.py"
    launcher.write_text("import sys\nsys.path.insert(0," + repr(str(tool / "usr/lib/python3/dist-packages"))
                        + ")\nfrom black import patched_main\npatched_main()\n")
    argv = [sys.executable, "-I", "-S", "-B", str(launcher), "--version"]
    version = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, timeout=30, check=False)
    (work / "version.stdout").write_bytes(version.stdout)
    (work / "version.stderr").write_bytes(version.stderr)
    actual = version.stdout.decode().strip()
    if version.returncode or actual.partition(", ")[2] != pins["black_version"].partition(", ")[2]:
        raise RuntimeError("restored real formatter/interpreter version differs")
    for relative, expected in pins["tool_pins"].items():
        if digest(tool / relative) != expected:
            raise RuntimeError("Black dependency changed during version probe")
    published = dict(pins, tool_root=str(tool))
    (work / "restored-tool-pins.json").write_text(json.dumps(published, indent=2) + "\n")
    result = {"status": "PASS_EXACT_BLACK_RESTORE_AND_VERSION_ONLY", "tool_root": str(tool),
              "source_archive": str(archive_path), "source_archive_sha256": ARCHIVE_SHA256,
              "published_input_pins_sha256": digest(pins_path), "restored_files": len(pins["tool_pins"]),
              "restored_bytes": restored_bytes, "actual_version": actual, "version_argv": argv,
              "original_tools_or_permissions_changed": False, "source_files_formatted": 0}
    (work / "restoration.json").write_text(json.dumps(result, indent=2) + "\n")
    print(json.dumps(result), flush=True)


if __name__ == "__main__":
    main()
