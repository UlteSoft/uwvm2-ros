#!/usr/bin/env python3
"""Acquire the exact official StyLua2.0.2 tool in the finite Linux owner.

This downloads/version-checks the formatter only. It supplies no formatting,
compiler, SDK-publication or Guest acceptance credit.
"""

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import zipfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--archive-only", action="store_true",
                        help="pin ZIP and executable bytes in memory without extracting/executing")
    parser.add_argument("--apt-inspect", action="store_true",
                        help="inspect existing Debian candidate metadata only; no download or execution")
    args = parser.parse_args()
    if sys.platform != "linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise RuntimeError("shared admitted Linux finite owner required")
    work = Path(os.environ["UWVM_RESOURCE_RUN"]) / "work"
    if args.apt_inspect:
        rows = []
        for package in ("stylua", "lua-stylua"):
            argv = ["/usr/bin/apt-cache", "policy", package]
            policy = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE, timeout=10, check=False)
            (work / (package + "-policy.stdout")).write_bytes(policy.stdout)
            (work / (package + "-policy.stderr")).write_bytes(policy.stderr)
            found = re.search(rb"Candidate:\s*(\S+)", policy.stdout)
            version = found[1].decode() if found else None
            row = {"package": package, "candidate": version, "policy_exit": policy.returncode}
            if version and version != "(none)":
                metadata = subprocess.run(["/usr/bin/apt-cache", "show", package + "=" + version],
                                          stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                                          stderr=subprocess.PIPE, timeout=10, check=False)
                (work / (package + "-metadata.stdout")).write_bytes(metadata.stdout)
                (work / (package + "-metadata.stderr")).write_bytes(metadata.stderr)
                row["metadata_exit"] = metadata.returncode
                row["metadata"] = metadata.stdout.decode()
            rows.append(row)
        (work / "apt-candidates.json").write_text(json.dumps({"status": "METADATA_INSPECTION_ONLY",
            "rows": rows, "downloads": 0, "formatter_runs": 0}, indent=2) + "\n")
        print(json.dumps({"status": "METADATA_INSPECTION_ONLY", "candidates": [row["candidate"] for row in rows]}), flush=True)
        return
    asset = work / "stylua-linux-x86_64.zip"
    url = "https://github.com/JohnnyMorganz/StyLua/releases/download/v2.0.2/stylua-linux-x86_64.zip"
    argv = ["/usr/bin/curl", "--fail", "--location", "--ipv4", "--proto", "=https",
            "--connect-timeout", "10", "--max-time", "45", "--max-filesize", "4194304",
            "--output", str(asset), url]
    fetched = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE, timeout=50, check=False)
    (work / "download.stdout").write_bytes(fetched.stdout)
    (work / "download.stderr").write_bytes(fetched.stderr)
    if fetched.returncode:
        raise RuntimeError("official StyLua release acquisition failed; retained curl diagnostics")
    if asset.stat().st_size != 3011490:
        raise RuntimeError("previously observed exact official release size drifted")
    with zipfile.ZipFile(asset) as archive:
        members = [member for member in archive.infolist() if not member.is_dir()]
        if (len(members) != 1 or members[0].filename != "stylua"
                or not 0 < members[0].file_size < 20 * 1024 * 1024
                or archive.testzip() is not None):
            raise RuntimeError("unexpected bounded official release ZIP contents")
        executable = archive.read(members[0])
    if args.archive_only:
        report = {"format": "task02-official-stylua-archive-v1",
                  "status": "ACQUIRED_ARCHIVE_INSPECTED_NOT_EXECUTED",
                  "source_url": url, "archive_path": str(asset),
                  "archive_bytes": asset.stat().st_size,
                  "archive_sha256": hashlib.sha256(asset.read_bytes()).hexdigest(),
                  "executable_member": "stylua", "executable_bytes": len(executable),
                  "executable_sha256": hashlib.sha256(executable).hexdigest(),
                  "expected_version": "2.0.2", "actual_version": None,
                  "files_formatted": 0, "public_sdk_published": False}
        (work / "stylua-archive.json").write_text(json.dumps(report, indent=2) + "\n")
        print(json.dumps({key: report[key] for key in ("status", "archive_bytes", "executable_bytes")}), flush=True)
        return
    tool = work / "tool"
    tool.mkdir()
    binary = tool / "stylua"
    binary.write_bytes(executable)
    binary.chmod(0o500)
    commands = []
    for label, option in (("version", "--version"), ("help", "--help")):
        result = subprocess.run([str(binary), option], stdin=subprocess.DEVNULL,
                                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                timeout=10, check=False)
        (work / (label + ".stdout")).write_bytes(result.stdout)
        (work / (label + ".stderr")).write_bytes(result.stderr)
        commands.append({"argv": [str(binary), option], "exit_code": result.returncode})
        if result.returncode:
            raise RuntimeError("actual StyLua " + label + " failed")
        if label == "version" and result.stdout.decode().strip() != "stylua 2.0.2":
            raise RuntimeError("actual formatter version differs from exact requested release")
    report = {"format": "task02-official-stylua-acquisition-v1",
              "status": "ACQUIRED_VERSION_CHECKED_NOT_FORMAT_QUALIFIED",
              "source_url": url, "archive_bytes": asset.stat().st_size,
              "archive_sha256": hashlib.sha256(asset.read_bytes()).hexdigest(),
              "tool_root": str(tool), "executable": str(binary),
              "executable_bytes": len(executable),
              "executable_sha256": hashlib.sha256(executable).hexdigest(),
              "actual_version": "stylua 2.0.2", "commands": commands,
              "files_formatted": 0, "public_sdk_published": False}
    (work / "stylua-acquisition.json").write_text(json.dumps(report, indent=2) + "\n")
    print(json.dumps({key: report[key] for key in ("status", "actual_version", "executable_bytes")}), flush=True)


if __name__ == "__main__":
    main()
