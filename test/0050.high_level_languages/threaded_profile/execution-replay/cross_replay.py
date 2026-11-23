#!/usr/bin/env python3
"""Replay real completed recordings through the installed five-ABI runners.

Input directories are produced by run.py. Run inside the required cgroup; this
test builds and installs nothing. Each target reuses the origin's exact source
and options, not a separately rebuilt, possibly byte-different guest.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import resource
import subprocess
import time

ARCHES = ("x86_64", "i686", "aarch64", "arm", "armhf")
CASES = ("pending", "gc-global", "start-expressions", "peer-memoryless", "mte-wait",
         "gc-references", "gc-exception-references", "gc-cast-fault", "peer-globals", "peer-tags")


def recorded(matrix, arch, case, engine):
    directory = matrix / arch
    rows = json.loads((directory / "results.json").read_text())
    matching = [r for r in rows if r["case"] == case and r["engine"] == engine
                and r["mode"] == "record"]
    if not matching or not matching[-1]["passed"]:
        raise ValueError(f"no completed recording: {arch}/{case}/{engine}")
    argv = json.loads((directory / f"{case}-{engine}-record.argv.json").read_text())
    if not isinstance(argv, list) or any(not isinstance(x, str) or not x for x in argv):
        raise ValueError("expected an explicit embedding argv array")
    split = argv.index("--")
    if split < 3 or argv[split - 2:split] != ["control", engine]:
        raise ValueError("recording is not from the actual control embedding fixture")
    binary = Path(argv[split - 3]).resolve(strict=True)
    history = (directory / f"{case}-{engine}.history").resolve(strict=True)
    return argv[:split + 1], argv[split + 1:], binary, history


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--matrix", type=Path, required=True,
                        help="directory with completed run.py outputs under each ABI name")
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--reference", choices=ARCHES, default="x86_64")
    parser.add_argument("--arch", choices=ARCHES, action="append")
    parser.add_argument("--case", choices=CASES, action="append")
    parser.add_argument("--engine", choices=("int", "llvm"), action="append")
    args = parser.parse_args()
    matrix = args.matrix.resolve(strict=True)
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=False)
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    (output / "cgroup.txt").write_text(Path("/proc/self/cgroup").read_text())
    rows = []
    for arch in dict.fromkeys(args.arch or ARCHES):
        if arch == args.reference:
            continue
        for origin, target in ((args.reference, arch), (arch, args.reference)):
            for engine in dict.fromkeys(args.engine or ("int", "llvm")):
                for case in dict.fromkeys(args.case or CASES):
                    _, options, origin_binary, history = recorded(matrix, origin, case, engine)
                    launch, _, binary, _ = recorded(matrix, target, case, engine)
                    argv = launch + options
                    label = f"{origin}-{target}-{engine}-{case}"
                    (output / (label + ".argv.json")).write_text(json.dumps(argv, indent=2))
                    environment = {k: v for k, v in os.environ.items()
                                   if not k.startswith("UWVM_TEST_")}
                    environment.update(UWVM_TEST_REPLAY="1", UWVM_TEST_PREFIX="1",
                                       UWVM_TEST_HISTORY=str(history))
                    if case == "gc-cast-fault":
                        environment["UWVM_TEST_TRAP"] = "1"
                    start = time.monotonic()
                    try:
                        process = subprocess.run(argv, env=environment, stdout=subprocess.PIPE,
                                                 stderr=subprocess.STDOUT, timeout=90)
                        code, text = process.returncode, process.stdout
                    except subprocess.TimeoutExpired as error:
                        code, text = 124, error.output or b""
                    (output / (label + ".log")).write_bytes(text)
                    row = dict(origin=origin, target=target, engine=engine, case=case,
                               code=code, passed=code == 0 and b"THREAD-ONCE" not in text,
                               seconds=time.monotonic() - start,
                               binary_sha256=hashlib.sha256(binary.read_bytes()).hexdigest(),
                               origin_binary_sha256=hashlib.sha256(origin_binary.read_bytes()).hexdigest(),
                               history_sha256=hashlib.sha256(history.read_bytes()).hexdigest())
                    rows.append(row)
                    (output / "results.json").write_text(json.dumps(rows, indent=2))
                    print(row, flush=True)
                    if not row["passed"]:
                        print(text[-4000:].decode(errors="replace"), flush=True)
                        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
