#!/usr/bin/env python3
"""Retain source-bound, finite qualification evidence; never infer an unrun pass."""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import signal
import subprocess
import sys
import time

DIMENSIONS = ("implementation", "build", "wasm_compilation", "execution", "native_ros", "qemu", "performance")
EXCLUDED = {".git", ".tmp", ".xmake", "build", "node_modules", "__pycache__", ".cache"}


def sha(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def save(path, value):
    temporary = path.with_suffix(path.suffix + ".new")
    temporary.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")
    temporary.replace(path)


def source_manifest(source):
    """Include tracked and nonignored new source; record link bytes without following them."""
    result = subprocess.run(["git", "ls-files", "-co", "--exclude-standard", "-z"],
                            cwd=source, check=True, stdout=subprocess.PIPE)
    rows = {}
    for name in sorted(set(os.fsdecode(item) for item in result.stdout.split(b"\0") if item)):
        relative = Path(name)
        if relative.is_absolute() or ".." in relative.parts:
            raise ValueError("noncanonical git input")
        if EXCLUDED.intersection(relative.parts):
            continue
        path = source / relative
        if path.is_symlink():
            rows[name] = {"kind": "symlink", "target": os.readlink(path)}
        elif path.is_file():
            rows[name] = {"kind": "file", "bytes": path.stat().st_size, "sha256": sha(path)}
        elif path.is_dir():
            # A gitlink is an independently versioned source input, not a file.
            nested = source_manifest(path)
            for child, entry in nested.items():
                rows[name + "/" + child] = entry
        else:
            raise ValueError("missing source input: " + name)
    return rows


def canonical_digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True, separators=(",", ":")).encode()).hexdigest()


def owned_tree(parent):
    facts = {}
    for path in Path("/proc").iterdir():
        if path.name.isdecimal():
            try:
                fields = (path / "stat").read_text().rsplit(") ", 1)[1].split()
                facts[int(path.name)] = (int(fields[1]), fields[19])
            except (OSError, ValueError, IndexError):
                pass
    selected = {parent}
    while True:
        found = {pid for pid, (ppid, _) in facts.items() if ppid in selected}
        if found <= selected:
            return {pid: facts[pid][1] for pid in selected if pid in facts}
        selected |= found


def kill_owned(process, known=None):
    """Nested drivers can create sessions; terminate their owned tree, not only one process group."""
    if sys.platform != "linux":
        os.killpg(process.pid, signal.SIGKILL)
    else:
        owned = dict(known or {})
        if process.poll() is None:
            owned.update(owned_tree(process.pid))
        for pid, start in sorted(owned.items(), reverse=True):
            fd = None
            try:
                fd = os.pidfd_open(pid)
                actual = Path(f"/proc/{pid}/stat").read_text().rsplit(") ", 1)[1].split()[19]
                if actual == start:
                    signal.pidfd_send_signal(fd, signal.SIGKILL)
            except ProcessLookupError:
                pass
            except FileNotFoundError:
                pass
            finally:
                if fd is not None:
                    os.close(fd)
    if process.poll() is None:
        process.wait(timeout=10)


def init(args):
    out, source = args.out.resolve(), args.source.resolve(strict=True)
    if out == source or out.is_relative_to(source):
        raise ValueError("receipt output must be outside the source tree")
    out.mkdir(parents=True, exist_ok=False, mode=0o700)
    manifest = source_manifest(source)
    if not manifest:
        raise ValueError("empty source manifest")
    save(out / "source-manifest.json", manifest)
    report = {"schema_version": 1, "lane": args.lane, "source": str(source),
              "source_id": "sha256:" + canonical_digest(manifest),
              "git_sha": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=source, text=True).strip(),
              "started_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
              "host": {"system": platform.system(), "machine": platform.machine(), "release": platform.release()},
              "workflow": {name: os.environ.get(name) for name in
                           ("GITHUB_REPOSITORY", "GITHUB_SHA", "GITHUB_REF", "GITHUB_EVENT_NAME", "GITHUB_RUN_ID", "GITHUB_RUN_ATTEMPT")},
              "required": args.require, "commands": [], "status": "UNTESTED",
              "dimensions": {name: {"status": "UNTESTED", "commands": []} for name in DIMENSIONS}}
    save(out / "receipt.json", report)
    (out / "source-id.txt").write_text(report["source_id"] + "\n")


def run_command(args):
    out = args.out.resolve(strict=True)
    report = json.loads((out / "receipt.json").read_text())
    if report.get("sealed"):
        raise ValueError("sealed receipts cannot be extended; start a fresh run")
    if not re.fullmatch(r"[a-zA-Z0-9][a-zA-Z0-9_.-]{0,79}", args.id):
        raise ValueError("command id must be a short filename component")
    if any(row["id"] == args.id for row in report["commands"]):
        raise ValueError("duplicate command id; retain retries in a new receipt")
    argv = args.argv[1:] if args.argv[:1] == ["--"] else args.argv
    if not argv or args.timeout <= 0:
        raise ValueError("explicit command and positive timeout required")
    row = {"id": args.id, "argv": argv, "cwd": str(args.cwd.resolve()),
           "dimensions": args.dimension, "status": "RUNNING", "timeout_seconds": args.timeout,
           "subjects": {str(path.resolve(strict=True)): sha(path) for path in args.subject}}
    report["commands"].append(row)
    save(out / "receipt.json", report)
    process = None
    started = time.monotonic()
    def terminate(signum, _frame):
        if process is not None and process.poll() is None:
            kill_owned(process)
        raise InterruptedError("qualification interrupted by signal " + str(signum))
    old_handlers = {sig: signal.signal(sig, terminate) for sig in (signal.SIGTERM, signal.SIGINT)}
    try:
        with (out / (args.id + ".stdout")).open("xb") as stdout, (out / (args.id + ".stderr")).open("xb") as stderr:
            process = subprocess.Popen(argv, cwd=args.cwd, stdin=subprocess.DEVNULL, stdout=stdout,
                                       stderr=stderr, start_new_session=True)
            row["pid"] = process.pid
            member = Path(f"/proc/{process.pid}/cgroup")
            row["membership"] = member.read_text().splitlines() if member.exists() else None
            try:
                row["returncode"] = process.wait(timeout=args.timeout)
            except subprocess.TimeoutExpired:
                kill_owned(process)
                row.update(status="FAIL", reason="bounded command timeout")
            else:
                row["status"] = "PASS" if row["returncode"] == 0 else "BLOCKED" if row["returncode"] == 77 else "FAIL"
                if row["status"] != "PASS":
                    row["reason"] = "command exit " + str(row["returncode"])
        for path, expected in row["subjects"].items():
            if sha(path) != expected:
                row.update(status="FAIL", reason="executed subject changed during the command")
    except FileNotFoundError as error:
        row.update(status="BLOCKED", reason=str(error))
    except (OSError, InterruptedError) as error:
        if process is not None and process.poll() is None:
            kill_owned(process)
        row.update(status="FAIL", reason=str(error))
    finally:
        for sig, handler in old_handlers.items():
            signal.signal(sig, handler)
        row["seconds"] = round(time.monotonic() - started, 6)
        for stream in ("stdout", "stderr"):
            path = out / (args.id + "." + stream)
            if path.exists():
                row[stream + "_sha256"] = sha(path)
        # A guarded orchestration command may append its own named subcommands.
        # Merge this row into the current receipt instead of dropping their evidence.
        latest = json.loads((out / "receipt.json").read_text())
        latest["commands"] = [row if existing["id"] == args.id else existing for existing in latest["commands"]]
        save(out / "receipt.json", latest)
    print(args.id + ": " + row["status"], flush=True)
    return 0 if row["status"] == "PASS" else 77 if row["status"] == "BLOCKED" else 1


def seal(args):
    out = args.out.resolve(strict=True)
    report = json.loads((out / "receipt.json").read_text())
    if report.get("sealed"):
        raise ValueError("receipt already sealed")
    original = json.loads((out / "source-manifest.json").read_text())
    try:
        report["source_unchanged"] = source_manifest(Path(report["source"])) == original
    except (OSError, ValueError, subprocess.CalledProcessError) as error:
        report["source_unchanged"] = False
        report["source_error"] = str(error)
    stages = {row["id"]: row["status"] for row in report["commands"]}
    report["missing_required"] = [name for name in report["required"] if name not in stages]
    for name in DIMENSIONS:
        rows = [row for row in report["commands"] if name in row["dimensions"]]
        statuses = {row["status"] for row in rows}
        status = ("FAIL" if statuses.intersection({"FAIL", "RUNNING"}) else "BLOCKED" if "BLOCKED" in statuses
                  else "PASS" if statuses == {"PASS"} else "UNTESTED")
        report["dimensions"][name] = {"status": status, "commands": [row["id"] for row in rows]}
    artifacts = {}
    for base in [out, *args.artifact]:
        base = base.resolve(strict=True)
        for path in sorted(base.rglob("*")) if base.is_dir() else [base]:
            if path.is_file() and not path.is_symlink() and path not in (out / "receipt.json", out / "artifact-manifest.json"):
                artifacts[str(path)] = {"bytes": path.stat().st_size, "sha256": sha(path)}
    save(out / "artifact-manifest.json", artifacts)
    report["artifact_manifest_sha256"] = sha(out / "artifact-manifest.json")
    report["job_status"] = args.job_status
    statuses = {row["status"] for row in report["commands"]}
    report["status"] = ("FAIL" if not report["source_unchanged"] or args.job_status != "success" or
                        statuses.intersection({"FAIL", "RUNNING"}) else
                        "BLOCKED" if report["missing_required"] or "BLOCKED" in statuses or not statuses else "PASS")
    report["sealed"] = True
    report["scope"] = "Only the named commands and dimensions; UNTESTED dimensions acquire no claim."
    save(out / "receipt.json", report)
    print(json.dumps({"status": report["status"], "dimensions": report["dimensions"],
                      "source_id": report["source_id"], "missing_required": report["missing_required"]}, indent=2))
    return 0 if report["status"] == "PASS" else 77 if report["status"] == "BLOCKED" else 1


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="operation", required=True)
    begin = commands.add_parser("init")
    begin.add_argument("--source", type=Path, required=True)
    begin.add_argument("--out", type=Path, required=True)
    begin.add_argument("--lane", required=True)
    begin.add_argument("--require", action="append", default=[])
    run = commands.add_parser("command")
    run.add_argument("--out", type=Path, required=True)
    run.add_argument("--id", required=True)
    run.add_argument("--dimension", choices=DIMENSIONS, action="append", default=[])
    run.add_argument("--subject", type=Path, action="append", default=[])
    run.add_argument("--timeout", type=float, required=True)
    run.add_argument("--cwd", type=Path, default=Path.cwd())
    run.add_argument("argv", nargs=argparse.REMAINDER)
    finish = commands.add_parser("seal")
    finish.add_argument("--out", type=Path, required=True)
    finish.add_argument("--artifact", type=Path, action="append", default=[])
    finish.add_argument("--job-status", choices=("success", "failure", "cancelled"), required=True)
    args = parser.parse_args()
    return {"init": init, "command": run_command, "seal": seal}[args.operation](args) or 0


if __name__ == "__main__":
    raise SystemExit(main())
