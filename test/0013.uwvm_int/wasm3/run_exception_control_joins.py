#!/usr/bin/env python3
"""Check EH after control joins, native arithmetic helpers, and non-catchable traps.

Run identical Core Wasm fixtures through each selected platform's
interpreter and LLVM JIT, plus an independent Wasmtime executable. QEMU wrapper
results establish correctness only. Negative imports verify that compiler runtime
relocation helpers do not become guest host interfaces. All output and hashes are saved.
"""
import argparse
import hashlib
import json
from pathlib import Path
import resource
import subprocess

def sha(path):
    with path.open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--vm", action="append", required=True, metavar="ARCH=EXECUTABLE")
    parser.add_argument("--runtime-binary", action="append", default=[], metavar="ARCH=ELF",
                        help="Actual VM binary when --vm names a launcher or QEMU wrapper")
    parser.add_argument("--wasm-tools", type=Path, required=True)
    parser.add_argument("--wasmtime", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args()
    resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
    membership = Path("/proc/self/cgroup").read_text().strip()
    group = Path("/sys/fs/cgroup") / membership.split(":", 2)[2].lstrip("/")
    if (group / "memory.max").read_text().strip() != "68719476736" or \
       (group / "memory.swap.max").read_text().strip() != "0":
        raise RuntimeError("Run in the existing 64 GiB, zero-swap test cgroup")
    runtimes = {}
    binaries = dict(item.split("=", 1) for item in args.runtime_binary)
    for selected in args.vm:
        arch, executable = selected.split("=", 1)
        if arch in runtimes:
            raise ValueError("Duplicate runtime architecture: " + arch)
        wrapper = Path(executable).resolve(strict=True)
        binary = Path(binaries.get(arch, executable)).resolve(strict=True)
        with binary.open("rb") as stream:
            if stream.read(4) != b"\x7fELF":
                raise ValueError("Supply --runtime-binary " + arch + "=ELF for a launcher")
        runtimes[arch] = dict(executable=str(wrapper), executable_sha256=sha(wrapper),
                              binary=str(binary), binary_sha256=sha(binary))
    if set(binaries) - set(runtimes):
        raise ValueError("A runtime binary has no matching --vm")
    tools = {str(p.resolve(strict=True)): sha(p)
             for p in (args.wasm_tools, args.wasmtime, Path(__file__))}
    args.out.mkdir(parents=True, exist_ok=False)
    fixtures = Path(__file__).parent / "fixtures/exception_control_joins"
    inputs = sorted(fixtures.glob("*.wat"))
    report = dict(qualified=False, cgroup=membership, performance_measurement=False,
                  inputs={str(p): sha(p) for p in inputs}, runtimes=runtimes, tools=tools,
                  artifacts={}, commands=[])
    def run(label, command, expected="ok"):
        out = args.out / (label + ".stdout")
        err = args.out / (label + ".stderr")
        with out.open("wb") as stdout, err.open("wb") as stderr:
            result = subprocess.run(list(map(str, command)), stdout=stdout, stderr=stderr, timeout=180)
        text = err.read_text(errors="replace").lower()
        if expected == "unknown_import":
            passed = not out.read_bytes() and (
                (result.returncode == 127 and "missing module dependency: an import has no approved provider" in text) or
                (result.returncode > 0 and "unknown import" in text and "env" in text))
        else:
            passed = result.returncode == 0 if expected == "ok" else \
                result.returncode > 0 and (("exception" in text) if expected == "uncaught" else
                                          ("out of bounds" in text))
        report["commands"].append(dict(name=label, argv=list(map(str, command)),
                                        returncode=result.returncode, passed=passed,
                                        stdout_sha256=sha(out), stderr_sha256=sha(err)))
        (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
        return passed
    ok = True
    for source in inputs:
        wasm = args.out / (source.stem + ".wasm")
        if not run("parse-" + source.stem, [args.wasm_tools, "parse", source, "-o", wasm]):
            raise RuntimeError("WAT parsing failed")
        if not run("validate-" + source.stem, [args.wasm_tools, "validate", wasm]):
            raise RuntimeError("Core Wasm validation failed")
        report["artifacts"][str(wasm)] = sha(wasm)
        expected = "unknown_import" if source.stem.startswith("hidden_native_") else \
            source.stem if source.stem in ("uncaught", "bounds") else "ok"
        ok &= run("wasmtime-" + source.stem,
                  [args.wasmtime, "run", "-Wexceptions=y", wasm], expected)
        for selected in args.vm:
            arch, executable = selected.split("=", 1)
            for engine in ("int", "aot"):
                ok &= run(arch + "-" + engine + "-" + source.stem,
                          [executable, "--wasm-feature-enable-exceptions", "-R" + engine,
                           "--runtime-compile-threads", "0", "--runtime-llvm-jit-policy", "debug",
                           "--run", wasm], expected)
    if report["inputs"] != {str(p): sha(p) for p in inputs}:
        raise RuntimeError("A test input changed")
    for runtime in runtimes.values():
        if sha(Path(runtime["executable"])) != runtime["executable_sha256"] or \
           sha(Path(runtime["binary"])) != runtime["binary_sha256"]:
            raise RuntimeError("A selected runtime changed")
    if any(sha(Path(p)) != digest for p, digest in tools.items()) or \
       any(sha(Path(p)) != digest for p, digest in report["artifacts"].items()):
        raise RuntimeError("A test tool or compiled fixture changed")
    report["qualified"] = bool(ok)
    (args.out / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    if not ok:
        raise RuntimeError("At least one runtime result differs from the required outcome")
    print("PASS: control joins, native arithmetic, private helpers, uncaught exceptions and bounds traps")
if __name__ == "__main__":
    main()
