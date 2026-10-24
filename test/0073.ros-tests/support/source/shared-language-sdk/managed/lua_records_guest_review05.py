#!/usr/bin/env python3
"""Execute one exact validated Lua record product as a separate stdio-only Guest.

The enclosing xmake/resource failure stays failed. This consumes the genuine
successful inner compiler/validator receipt without manufacturing a build
receipt, recompiling, or giving xmake credit. Current original owner is required.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess


def pin(path):
    path = path.resolve(strict=True)
    with path.open("rb") as stream:
        return dict(path=str(path), bytes=path.stat().st_size,
                    sha256=hashlib.file_digest(stream, "sha256").hexdigest())


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("build", "runtime", "out", "records"):
        parser.add_argument("--" + name, type=Path, required=True)
    for name in ("build", "runtime", "records"):
        parser.add_argument("--" + name + "-sha256", required=True)
    args = parser.parse_args()
    if os.uname().sysname != "Linux" or os.environ.get("UWVM_RESOURCE_HEAVY_LOCK_HELD") != "1":
        raise ValueError("original admitted Linux finite owner required")
    selected = {name: pin(getattr(args, name)) for name in ("build", "runtime", "records")}
    if any(row["sha256"] != getattr(args, name + "_sha256") for name, row in selected.items()):
        raise ValueError("an explicit immutable actual product/Runtime/record input changed")
    build = json.loads(args.build.read_bytes())
    if (build["status"] != "BUILT_AND_VALIDATED_NOT_GUEST_EXECUTED" or
            build["exception_encoding"] != "exnref-try-table" or build["default_authority"] != [] or
            build["profile"]["target"] != "wasm32-wasip1" or build["profile"]["threads"] is not False or
            build["profile"]["exceptions"] is not True or build["backend"] not in (1, 2)):
        raise ValueError("actual standard-EH Lua compiler/validator product required")
    if any(row["exit"] != 0 for row in build["commands"]):
        raise ValueError("the original inner compiler/validator recipe did not fully succeed")
    if any(pin(Path(row["path"])) != row for row in build["source_inputs"]):
        raise ValueError("a retained real compiler input changed")
    artifact = Path(build["artifact"]["path"])
    if pin(artifact) != build["artifact"]:
        raise ValueError("the actual compiled artifact changed")
    records = json.loads(args.records.read_bytes())
    expected = {row["symbol"]: {field["name"] for field in row["fields"]} for row in records["records"]}
    if not expected or records["kind"] != "complete-lua-record-sources-v1":
        raise ValueError("actual nonempty generated record receipt required")
    args.out.mkdir(parents=True, exist_ok=False)
    argv = [str(args.runtime), "--runtime-int", "-Rct", "0", "--wasm-feature-enable-exceptions",
            "--wasm-set-main-module-name", "main", "--wasip1-global-disable",
            "--wasip1-global-noinherit-system-environment", "--wasip1-single-create", "main",
            "--wasip1-single-enable", "main", "--wasip1-single-hide-host-api", "main",
            "--wasip1-single-noinherit-system-environment", "main"]
    for right in ("realtime-clock", "monotonic-clock", "process-clock", "thread-clock", "random",
                  "proc-raise", "sched-yield", "stdio-full-rights"):
        argv += ["--wasip1-single-deny", "main", right]
    argv += ["--run", str(artifact)]
    with (args.out / "stdout").open("xb") as stdout, (args.out / "stderr").open("xb") as stderr:
        completed = subprocess.run(argv, stdin=subprocess.DEVNULL, stdout=stdout, stderr=stderr, check=False)
    text = (args.out / "stdout").read_text()
    fields, members = {}, {}
    for line in text.splitlines():
        if line.startswith("LUA_RECORD_FIELD\t"):
            _, symbol, field, kind = line.split("\t")
            if field in fields.setdefault(symbol, {}):
                raise ValueError("duplicate actual Guest field")
            fields[symbol][field] = kind
        elif line.startswith("LUA_RECORD_MEMBER\t"):
            _, symbol, name, kind = line.split("\t")
            fields.setdefault(symbol, {})
            if name in members.setdefault(symbol, {}):
                raise ValueError("duplicate actual Guest class member")
            members[symbol][name] = kind
    passed = (completed.returncode == 0 and {key: set(value) for key, value in fields.items()} == expected
        and set(members) == set(expected) and all(all(row.get(name) == "function" for name in
            ("new", "encode", "decode", "_read")) for row in members.values())
        and text.count("PASS_ROS_RECORD_VALUES\t" + str(len(expected))) == 1
        and text.count("PASS_ROS_DEFAULT_DENIED") == 1)
    result = dict(format="task04-actual-lua-record-guest-v1", status="PASS" if passed else "FAILED_RETAINED",
        selected=selected, artifact=pin(artifact), argv=argv, exit=completed.returncode,
        actual_fields=fields, actual_members=members, round_tripped_records=len(expected) if passed else 0,
        stdout=pin(args.out / "stdout"), stderr=pin(args.out / "stderr"), xmake_build_credit=0,
        original_parent_failure_relabelled=False, positive_ros_credit=0, original_upstream_suite_credit=0,
        complete_public_api_documentation_qualified=False, default_authority=[])
    (args.out / "results.json").write_text(json.dumps(result, indent=2) + "\n")
    if not passed:
        raise ValueError("the actual complete Lua record Guest did not pass")


if __name__ == "__main__":
    main()
